# -*- coding: utf-8 -*-
"""
Multi-Goal Motion Planner (MGMP)

@author: P.Vana & P.Cizek
"""
import sys
import os
import numpy as np
import math
import GridMap as gmap
import GridPlanner as gplanner
import matplotlib.pyplot as plt
import time

from invoke_LKH import *

class MGMP_Solver:
    def __init__(self):
        self.distances = None
        self.paths = None

    # compute the shortest sequence based on the distance matrix (self.distances)
    def compute_TSP_sequence(self, start_idx = None, end_idx = None):
        n = len(self.distances)
        
        if start_idx is not None and end_idx is not None:
            M = n * np.max(np.max(self.distances))
            for i in range(n):
                self.distances[i, start_idx] = M
                self.distances[end_idx, i] = M
            self.distances[end_idx, start_idx] = 0
        
        fname_tsp = "problem"
        user_comment = "a comment by the user"
        writeTSPLIBfile_FE(fname_tsp, self.distances, user_comment)
        run_LKHsolver_cmd(fname_tsp)
        sequence = read_LKHresult_cmd(fname_tsp)
        return sequence
    
    # compute the shortest tour based on the distance matrix (self.distances)
    def compute_TSP_tour(self, start_idx = None, end_idx = None):
        n = len(self.distances)
        sequence = self.compute_TSP_sequence(start_idx=start_idx, end_idx=end_idx)

        path = []
        if start_idx is not None and end_idx is not None:
            for a in range(0, n - 1):
                b = (a + 1) % n
                a_idx = sequence[a]
                b_idx = sequence[b]
                actual_path = self.paths[(a_idx, b_idx)]
                path = path + actual_path
        else:
            for a in range(0,n):
                b = (a+1) % n
                a_idx = sequence[a]
                b_idx = sequence[b]
                # print(a_idx, b_idx)
                # print(self.paths)
                actual_path = self.paths[(a_idx, b_idx)]
                path = path + actual_path
        
        return path
   
    def plan_tour_etsp(self, gridmap, goals, start_idx = None, end_idx = None):
        n = len(goals)
        self.distances = np.zeros((n, n))
        self.paths = {}

        planner = gplanner.GridPlanner()

        # find path between each pair of goals (a,b)
        for a in range(0, n):
            for b in range(0, n):
                g1 = goals[a]
                g2 = goals[b]
                if a != b:
                    # store distance
                    self.distances[a][b] = gridmap.dist_euclidean(g1, g2)
                    # store path
                    self.paths[(a, b)] = [g1, g2]

        return self.compute_TSP_tour(start_idx, end_idx)
    
    def plan_tour_obstacles(self, gridmap, goals, start_idx = None, end_idx = None):	
        n = len(goals)
        self.distances = np.zeros((n, n))
        self.paths = {}

        planner = gplanner.GridPlanner()

        # find path between each pair of goals (a,b)
        for a in range(0, n):
            print('Shortest paths from city {0}'.format(a))
            for b in range(0, n):
                if a != b:
                    g1 = goals[a]
                    g2 = goals[b]
                
                    # find the shortest path
                    sh_path = planner.plan(gridmap, g1[:2], g2[:2])
                    # store distance
                    dist = 0
                    last = sh_path[0]
                    for act in sh_path:
                        dist += gridmap.dist_euclidean(last, act)
                        last = act
                    self.distances[a][b] = dist
                    # store path
                    self.paths[(a, b)] = sh_path

        return self.compute_TSP_tour(start_idx, end_idx)

    def plan_tour_obstacles_Floyd_Warshall(self, gridmap, goals, start_idx = None, end_idx = None):	

        def size_to_index(coord):
            (width, height) = coord
            return width * gridmap.height + height

        def index_to_size(index):
            return (int(index / gridmap.height), index % gridmap.height)
        
        n = gridmap.width * gridmap.height
        
        # Initialize distance matrix
        self.distances = np.full((n, n), np.inf)

        # Compute estimated distances
        for i in range(n):
            coord = index_to_size(i)
            if gridmap.passable(coord) == False: continue
            self.distances[i][i] = 0
            for neighbor in gridmap.neighbors4(coord):
                index = size_to_index(neighbor)
                self.distances[i][index] = 1

        # Compute predecessors
        predecessors = np.full((n, n), -1)
        for a in range(n):
            for b in range(n):
                if self.distances[a, b] != 0 and self.distances[a, b] != np.inf:
                    predecessors[a][b] = a
        
        # Floyd-Warshall
        for k in range(n):
            if gridmap.passable(index_to_size(k)) == False:
                continue
            
            for i in range(n):
                if gridmap.passable(index_to_size(i)) == False:
                    continue

                for j in range(n):
                    if gridmap.passable(index_to_size(j)) == False:
                        continue

                    if self.distances[i][j] > self.distances[i][k] + self.distances[k][j]:
                        self.distances[i][j] = self.distances[i][k] + self.distances[k][j]
                        predecessors[i][j] = predecessors[k][j]
        
        n = len(goals)
        dist = np.full((n, n), np.inf)
        for i in range(n):
            index1 = size_to_index(goals[i])
            for j in range(n):
                index2 = size_to_index(goals[j])
                dist[i][j] = self.distances[index1][index2]
        
        self.distances = dist

        self.paths = {}
        print("pred")
        for i in range(n):
            index1 = size_to_index(goals[i])
            
            for j in range(n):
                if i == j: continue
                index2 = size_to_index(goals[j])

                path = [goals[j]]
                pred = predecessors[index1, index2]
                counter = 0
                while pred != -1:
                    path = [index_to_size(pred)] + path
                    pred = predecessors[index1, pred]
                    counter += 1
                    if counter > (gridmap.height * gridmap.width):
                        print(i, j, pred)
                self.paths[i, j] = path
                self.paths[j, i] = list(reversed(path))

        tour = self.compute_TSP_tour(start_idx, end_idx)
        print("LENGTH", self.path_length(tour, gridmap))
        return tour
    
    def path_length(self, path, gridmap):
        length = 0
        for i in range(1, len(path)):
            length += gridmap.dist_euclidean(path[i - 1], path[i])

        return length
    
    def plan_tour_obstacles_neighborhood(self, gridmap, goals, neighbors, start_idx = None, end_idx = None):	
        n = len(goals)
        self.distances = np.zeros((n,n))	
        self.paths = {}

        planner = gplanner.GridPlanner()
        np.set_printoptions(threshold=np.nan)

        # compute sequence based on the non-collision Euclidean distances
        for a in range(0,n): 
            print('Shortest paths from city {0}'.format(a))
            for b in range(0,n):
                if a != b:
                    g1 = goals[a]
                    g2 = goals[b]
                
                    # find the shortest path
                    sh_path = planner.plan(gridmap, g1, g2, neigh = 'N8')
                    # store distance
                    dist = 0
                    last = sh_path[0]
                    for act in sh_path:
                        dist += gridmap.dist_euclidean(last, act)
                        last = act 
                    self.distances[a][b] = dist
                    # store path
                    self.paths[(a,b)] = sh_path

        sequence = self.compute_TSP_sequence(start_idx, end_idx)

        number_of_samples = len(neighbors[0])
        n = n * number_of_samples
        
        # Initialize distance matrix
        self.distances = np.full((n, n), np.inf)

        def layer_index_to_index(layer, index):
            return layer * number_of_samples + index

        def index_to_layer_index(index):
            return (int(index / number_of_samples), index % number_of_samples)

        # Compute estimated distances
        for i in range(n):
            (layer, index) = index_to_layer_index(i)
            next_layer = (layer + 1) % len(goals)
            for j in range(number_of_samples):
                index2 = layer_index_to_index(next_layer, j)
                path = planner.plan(gridmap, neighbors[sequence[layer]][index], neighbors[sequence[next_layer]][j], neigh='N8')
                self.distances[i][index2] = self.path_length(path, gridmap)

        # Compute predecessors
        predecessors = np.full((n, n), -1)
        for a in range(n):
            for b in range(n):
                if self.distances[a, b] != 0 and self.distances[a, b] != np.inf:
                    predecessors[a][b] = a
        
        # Floyd-Warshall
        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if self.distances[i][j] > self.distances[i][k] + self.distances[k][j]:
                        self.distances[i][j] = self.distances[i][k] + self.distances[k][j]
                        predecessors[i][j] = predecessors[k][j]
        
        min_index = -1
        min_value = np.inf
        for i in range(number_of_samples):
            dist = self.distances[i][i]
            print(dist)
            if dist < min_value:
                min_value = dist
                min_index = i

        pred = predecessors[min_index][min_index]
        result = []
        while pred != min_index:
            result = [pred] + result
            pred = predecessors[pred][pred]
            
        result = [pred] + result

        n = len(goals)

        selected_samples = [0] * n
        for i in range(n):
            (layer_index, index) = index_to_layer_index(result[i])
            layer = sequence[layer_index]
            selected_samples[layer] = index


        path = []
        if start_idx != None and end_idx != None:
            # TODO - reconstruct only the open-loop
            print('TODO', self.plan_tour_obstacles_neighborhood.__name__)
            exit(1)
        else:
            for a in range(0,n):
                b = (a + 1) % n
                a_idx = sequence[a]
                b_idx = sequence[b]
                a_sample = neighbors[a_idx][selected_samples[a_idx]]
                b_sample = neighbors[b_idx][selected_samples[b_idx]]
                actual_path = planner.plan(gridmap, a_sample, b_sample, neigh='N8')
                path = path + actual_path

        print("LENGTH {}".format(self.path_length(path, gridmap)))

        return path
     
    def plan_tour_obstacles_neighborhood_NB(self, gridmap, goals, neighbors, start_idx=None, end_idx=None):
        # TODO - reconstruct only the open-loop
        print('TODO', self.plan_tour_obstacles_neighborhood_NB.__name__)
        exit(1)
    

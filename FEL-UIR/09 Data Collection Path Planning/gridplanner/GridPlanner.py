#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

import collections
import heapq

import matplotlib.pyplot as plt

import GridMap as gmap

class GridPlanner:

    def __init__(self):
        pass

    def plan(self, gridmap, start, goal, neigh = 'N4'):
        """
        Method to plan the path

        Parameters
        ----------
        gridmap: GridMap
            gridmap of the environment
        start: (int,int)
            start coordinates
        goal:(int,int)
            goal coordinates
        neigh:string(optional)
            type of neighborhood for planning ('N4' - 4 cells, 'N8' - 8 cells)

        Returns
        -------
        list(int,int)
            the path between the start and the goal if there is one, None if there is no path
        """
        came_from, cost_so_far = self.a_star_search(gridmap, start, goal, neigh = neigh)
        if not goal in came_from:
            return None
        path = self.reconstruct_path(came_from, start, goal)
        return path

    def simplify_path(self, gridmap, path):
        """
        Method to implify the found path

        Parameters
        ----------
        gridmap: GridMap
            gridmap of the environment
        path: list(int,int)
            the path found by the planner

        Returns
        -------
        list(int,int)
            the simplified
        """
        return path
    
    #########################################
    # A_STAR
    #########################################
    
    class PriorityQueue:
            def __init__(self):
                    self.elements = []
    
            def empty(self):
                    return len(self.elements) == 0
    
            def put(self, item, priority):
                    heapq.heappush(self.elements, (priority, item))
    
            def get(self):
                    return heapq.heappop(self.elements)[1]
    
    def heuristic(self, a, b):
                    (x1, y1) = a
                    (x2, y2) = b
                    return math.sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))
    
    def a_star_search(self, graph, start, goal, neigh):
            open_set = self.PriorityQueue()
            open_set.put(start, 0)
            came_from = {}
            cost_so_far = {}
            came_from[start] = None
            cost_so_far[start] = 0
            
            while not open_set.empty():
                    current = open_set.get()
                    if current == goal:
                            break

                    if neigh == 'N4':
                        neighborhood = graph.neighbors4(current)
                    if neigh == 'N8':
                        neighborhood = graph.neighbors8(current)
            
                    for next in neighborhood:
                            new_cost = cost_so_far[current] + graph.dist_euclidean(current, next)
                            if next not in cost_so_far or new_cost < cost_so_far[next]:
                                    cost_so_far[next] = new_cost
                                    priority = new_cost + self.heuristic(goal, next)
                                    open_set.put(next, priority)
                                    came_from[next] = current
    
            return came_from, cost_so_far
    
    #########################################
    # backtracking function
    #########################################
    
    def reconstruct_path(self, came_from, start, goal):
            current = goal
            path = [current]
            while current != start:
                    current = came_from[current]
                    path.append(current)
            path.append(start) # optional
            path.reverse() # optional
            return path

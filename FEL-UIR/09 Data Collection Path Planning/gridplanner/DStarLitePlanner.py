#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

import collections
import heapq

import matplotlib.pyplot as plt

import GridMap as gmap
from GridPlanner import GridPlanner

class DStarLitePlanner(GridPlanner):

    def __init__(self):
        self.first_run = True
        self.grid = None
        self.U = None
        
    def getRHSvalue(self, coord=None):
        """
        method to get the RHS values of the D* lite grid
        
        Parameters
        ----------
        coord: (int,int)
            if given, the function returns the RHS value only at the given point, otherwise it returns all the RHS values

        Returns
        -------
        np.array(float) 
            RHS values
        """
        ret = None
        if not coord == None:
            (x, y) = coord
            ret = self.grid["rhs"][x][y] 
        else:
            ret = self.grid["rhs"]
        return ret

    def getGvalue(self, coord = None):
        """
        method to get the G values of the D* lite grid
        
        Parameters
        ----------
        coord: (int,int)
            if given, the function returns the G value only at the given point, otherwise it returns all the G values

        Returns
        -------
        np.array(float) 
            G values
        """
        ret = None
        if not coord == None:
            (x, y) = coord
            ret = self.grid["g"][x][y] 
        else:
            ret = self.grid["g"]
        return ret
                
    def setRHSvalue(self, coord, value):
        """
        method to set the RHS value of the D* lite grid

        Parameters
        ----------
        coord: (int,int)
            coordinates of the cell to be updated
        value: float
            update value
        """
        (x, y) = coord
        self.grid["rhs"][x][y] = value
                
    def setGvalue(self, coord, value):
        """
        method to set the G value of the D* lite grid

        Parameters
        ----------
        coord: (int,int)
            coordinates of the cell to be updated
        value: float
            update value
        """
        (x, y) = coord
        self.grid["g"][x][y] = value


    def calculate_key(self, coord, goal):
        """
        method to calculate the priority queue key

        Parameters
        ----------
        coord: (int, int)
            cell to calculate key for
        goal: (int, int)
            goal location
        
        Returns
        -------
        (float, float)
            major and minor key
        """
        #TODO: implement the key calculation
        (x,y) = coord
        (x_g, y_g) = goal
        g = self.getGvalue(coord)
        rhs = self.getRHSvalue(coord)
        #heuristics = (x - x_g)**2 + (y - y_g)**2
        heuristics = abs(x - x_g) + abs(y - y_g)

        #return (100*np.minimum(g, rhs) + heuristics + np.minimum(g,rhs), np.minimum(g, rhs))
        return (100*(np.minimum(g, rhs) + heuristics) + np.minimum(g,rhs), np.minimum(g, rhs))

    def compute_shortest_path(self, gridmap, start, goal):
        """
        Function to compute the shortest path

        Parameters
        ----------
        gridmap:GridMap
            map of the environment
        start: (int, int)
            start position
        goal: (int, int)
            goal position
        """
        #TODO: implement the compute shortest path method of the dstar lite algorithm
        while self.calculate_key(self.U.top(), start)[0] < self.calculate_key(start, start)[0] or self.getRHSvalue(start) != self.getGvalue(start):
            u = self.U.get()
            u_g = self.getGvalue(u)
            u_rhs = self.getRHSvalue(u)
            
            if u_g > u_rhs:
            #over consistent
                self.setGvalue(u, u_rhs)
                predecessors = gridmap.neighbors4(u)
                for pred in predecessors:
                    self.update_vertex(gridmap, pred, start, goal)
                    
            else:
            #under consistent
                self.setGvalue(u, np.inf)
                self.update_vertex(gridmap, u, start, goal)
                predecessors = gridmap.neighbors4(u)
                for pred in predecessors:
                    self.update_vertex(gridmap, pred, start, goal)
            
            if self.U.empty():
                break


    def update_vertex(self, gridmap, u, start, goal):
        """
        Function for map vertex updating

        Parameters
        ----------
        gridmap:GridMap
            map of the environment
        u: (int, int)
            currently processed position
        start: (int, int)
            start position
        goal: (int, int)
            goal position
        """
        #TODO: implement the update vertex method
        if u != goal:
            successors = gridmap.neighbors4(u)
            cost = np.inf
            for s_prime in successors:
                #calculate distances
                cost = np.minimum(cost, self.getGvalue(s_prime) + 1)
            #update the cost
            self.setRHSvalue(u, cost)
        
        if self.U.contains(u):
            self.U.remove(u)
        
        if self.getGvalue(u) != self.getRHSvalue(u):
            #not consistent
            self.U.put(u, self.calculate_key(u, start)[0])


    def reconstruct_path(self, gridmap, start, goal):
        """
        Function to reconstruct the path

        Parameters
        ----------
        gridmap:GridMap
            map of the environment
        u: (int, int)
            currently processed position
        start: (int, int)
            start position
        goal: (int, int)
            goal position
        
        Returns
        -------
        list(int, int)
            the path
        """
        curr = start
        path = []
        path.append(curr)
        while curr != goal:
            neighbors = gridmap.neighbors4(curr)
            curr = neighbors[0]
            for neighbor in neighbors:
                if self.getGvalue(curr) > self.getGvalue(neighbor):
                    curr = neighbor
            path.append(curr)
        return(path)

    def plan(self, gridmap, start, goal):
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

        Returns
        -------
        list(int,int)
            the path between the start and the goal if there is one, None if there is no path
        """
        if self.first_run:
            self.first_run = False

            #create the grid
            width = gridmap.width
            height = gridmap.height
            self.grid = np.zeros((width, height), dtype=[('rhs',np.dtype(float)),('g',np.dtype(float))])
            
            #fill it with the default values
            self.grid['rhs'] = np.inf
            self.grid['g'] = np.inf
            
            #create the priority queue
            self.U = self.PriorityQueue()
            
            #starting point
            self.grid['rhs'][goal[0]][goal[1]] = 0
            self.U.put(goal, self.calculate_key(goal, start))

            #compute shortest path
            self.compute_shortest_path(gridmap, start, goal)
            
            #reconstruct path
            return self.reconstruct_path(gridmap, start, goal)
        else:
            #find the change to the map
            #TODO: find what has changed in the gridmap
            indices = np.where(gridmap.grid['free'] == False)
            for row,col in zip(indices[0], indices[1]):
                coord = (row,col)
                if self.getGvalue(coord) != np.inf:
                    print(coord)
                    #update the g and rhs values
                    u_g = self.getGvalue(coord)
                    u_rhs = self.getRHSvalue(coord)

                    self.setGvalue(coord, np.inf)
                    self.setRHSvalue(coord, np.inf)

                    #update the neighbors 
                    neighbors = gridmap.neighbors4(coord)
                    for neighbor in neighbors:
                        if self.getGvalue(neighbor) < u_g:
                            continue
                        self.setGvalue(coord, np.inf)
                        self.update_vertex(gridmap, neighbor, start, goal)
                    
                    #update priorities in the priority queue
                    V = self.PriorityQueue()
                    while not self.U.empty():
                        u = self.U.get()
                        V.put(u, self.calculate_key(u, start)[0])

                    self.U = V
                        
                    self.U.put(coord, self.calculate_key(coord, start)[0])

            self.compute_shortest_path(gridmap, start, goal)
                    
            #reconstruct path
            return self.reconstruct_path(gridmap, start, goal)

            #TODO: update the rhs and g values according to the change of the environment


    ###############################
    #priority queue class
    ###############################
    class PriorityQueue:
        def __init__(self):
            self.elements = []
        
        def empty(self):
            return len(self.elements) == 0
        
        def put(self, item, priority):
            heapq.heappush(self.elements, (priority, item))
        
        def get(self):
            return heapq.heappop(self.elements)[1]
        
        def top(self):
            u = self.elements[0]
            return u[1]
        
        def contains(self, element):
            ret = False
            for item in self.elements:
                if element == item[1]:
                    ret = True
                    break
            return ret
        
        def print_elements(self):
            print(self.elements)
            return self.elements
        
        def remove(self, element):
            i = 0
            for item in self.elements:
                if element == item[1]:
                    self.elements[i] = self.elements[-1]
                    self.elements.pop()
                    heapq.heapify(self.elements)
                    break
                i += 1

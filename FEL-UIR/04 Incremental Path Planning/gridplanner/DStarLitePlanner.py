#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

import collections
import heapq

import matplotlib.pyplot as plt

# import GridMap as gmap
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

    def heuristics(self, coord1, coord2):
        """
        Method to calculate heuristics. It uses manhattan distance cost.
        """

        (x1, y1) = coord1
        (x2, y2) = coord2
        return abs(x1 - x2) + abs(y1 - y2)

    def cost(self, coord1, coord2):
        """
        Computes cost of the edge between the nodes
        """
        
        return 1
    
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
        g = self.getGvalue(coord)
        rhs = self.getRHSvalue(coord)
        min_g_rhs = min(g, rhs)
        h = self.heuristics(coord, goal)
        return (min_g_rhs + h, min_g_rhs)
    
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
        start_key = self.calculate_key(start, start)
        start_g = self.getGvalue(start)
        start_rhs = self.getRHSvalue(start)
        
        while self.calculate_key(self.U.top(), start) < start_key or start_rhs != start_g:
            u = self.U.get()
            # top_key_old = self.calculate_key(u, start)
            # self.U.remove(u)

            # if top_key_old < self.calculate_key(u, start):
                # self.U.put(u, self.calculate_key(u, start))
            # el
            if self.getGvalue(u) > self.getRHSvalue(u):
                self.setGvalue(u, self.getRHSvalue(u))
                for predecessor in gridmap.neighbors4(u):
                    self.update_vertex(gridmap, predecessor, start, goal)
            else:
                self.setGvalue(u, np.inf)
                self.update_vertex(gridmap, u, start, goal)
                for predecessor in gridmap.neighbors4(u):
                    self.update_vertex(gridmap, predecessor, start, goal)
            
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
        if u != goal:
            rhs = np.inf
            for successor in gridmap.neighbors4(u):
                new_rhs = self.cost(u, successor) + self.getGvalue(successor)
                rhs = np.minimum(rhs, new_rhs)
            self.setRHSvalue(u, rhs)
        
        if self.U.contains(u):
            self.U.remove(u)
        
        if self.getGvalue(u) != self.getRHSvalue(u):
            self.U.put(u, self.calculate_key(u, start))

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

            # create the grid
            width = gridmap.width
            height = gridmap.height
            self.grid = np.zeros((width, height), dtype=[('rhs', np.dtype(float)), ('g', np.dtype(float))])
            
            # fill it with the default values
            self.grid['rhs'] = np.inf
            self.grid['g'] = np.inf
            
            # create the priority queue
            self.U = self.PriorityQueue()
            
            # starting point
            self.grid['rhs'][goal[0]][goal[1]] = 0
            self.U.put(goal, self.calculate_key(goal, start))

            # compute shortest path
            self.compute_shortest_path(gridmap, start, goal)
            
            # reconstruct path
            return self.reconstruct_path(gridmap, start, goal)
        else:
            # find the change to the map
            indicies = np.where(gridmap.grid['free'] == False)
            for i in range(len(indicies[0])):
                coord = (indicies[0][i], indicies[1][i])

                if self.getGvalue(coord) != np.inf:
                    self.setGvalue(coord, np.inf)
                    self.setRHSvalue(coord, np.inf)
                
                for neighbor in gridmap.neighbors4(coord):
                    self.update_vertex(gridmap, neighbor, start, goal)
            
                # update priority queue
                U_new = self.PriorityQueue()
                while not self.U.empty():
                    u = self.U.get()
                    u_key = self.calculate_key(u, start)
                    U_new.put(u, u_key)
                self.U = U_new
                self.U.put(coord, self.calculate_key(coord, start))

            #recompute the shortest path
            self.compute_shortest_path(gridmap, start, goal)
                    
            #reconstruct path
            return self.reconstruct_path(gridmap, start, goal)

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
        
        def remove(self, element):
            i = 0
            for item in self.elements:
                if element == item[1]:
                    self.elements[i] = self.elements[-1]
                    self.elements.pop()
                    heapq.heapify(self.elements)
                    break
                i += 1

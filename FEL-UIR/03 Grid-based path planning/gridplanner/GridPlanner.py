#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

import collections
import heapq

import matplotlib.pyplot as plt

import GridMap as gmap
import heapq
from Coord import Coord

class GridPlanner:

    def __init__(self):
        pass

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
        visited = {}
        for i in range(gridmap.height):
            for j in range(gridmap.width):
                visited[(i, j)] = not gridmap.passable([i, j])
        
        q = []
        heapq.heappush(q, Coord(start, None, 0, gridmap.cost_manhattan(start, goal)))
        visited[start] = True
        while len(q) > 0:
            first = heapq.heappop(q)
            n = gridmap.neighbors4(first.coord)
            for i in range(len(n)):
                if n[i] == goal:
                    result = [goal, first.coord]
                    node = first
                    while node.parent != None:
                        result.append(node.parent.coord)
                        node = node.parent
                    return result
                
                if visited[n[i]]: continue
                coord = Coord(n[i], first, first.distance + 1, gridmap.cost_manhattan(n[i], goal))
                heapq.heappush(q, coord)
                visited[n[i]] = True
        
        return None

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

        current_index = 0
        line_indexes = [current_index]
        while True:
            is_goal = False
            for i in reversed(range(1, len(path))):
                line = gmap.bresenham_line(path[current_index], path[i])
                is_passable = True
                for j in range(len(line)):
                    if not gridmap.passable(line[j]):
                        is_passable = False
                    
                    if not is_passable:
                        break
                
                if is_passable:
                    current_index = i
                    line_indexes.append(current_index)

                    if i == len(path) - 1:
                        is_goal = True

                    break
            
            if is_goal:
                break
        
        line_indexes.append(len(path) - 1)
        
        return list(map(lambda x: path[x], line_indexes))

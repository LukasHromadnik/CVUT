#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import time
import numpy as np
import matplotlib.pyplot as plt
from collections import deque

sys.path.append('gridmap')
sys.path.append('gridplanner')
 
import GridMap as gmap
import DStarLitePlanner as dplanner

def plot_d_star_map(gridmap, rhs, g):
    """
    method to plot the annotated d star lite map with rhs and g values
    """
    plt.clf()
    gmap.plot_map(gridmap)
    ax = plt.gca()

    for i in range(0, gridmap.width):
        for j in range(0, gridmap.height):
            ax.annotate(g[i][j], xy=(i - 0.2, j - 0.1), color="red")
            ax.annotate(rhs[i][j], xy=(i - 0.2, j + 0.3), color="blue")

if __name__=="__main__":
 
    #define planning problems:
    scenarios = [([(1, 0), (1, 1), (1, 2), (1, 3), (2, 3), (3, 3), (4, 3), (4, 2), (4, 1), (5, 1), (6, 1), (6, 2), (6, 3), (6, 4), (6, 5), (6, 6), (5, 6), (4, 6), (3, 6), (2, 6), (1, 6), (7, 1), (8, 1), (8, 2), (8, 3), (8, 4), (8, 5), (8, 6), (8, 7), (8, 8), (7, 8), (6, 8), (5, 8), (4, 8), (3, 8), (2, 8), (1, 8)]),
                 ([(1, 0), (1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (1, 7), (1, 8), (3, 9), (3, 8), (3, 7), (3, 6), (3, 5), (3, 4), (3, 3), (3, 2), (3, 1), (5, 0), (5, 1), (5, 2), (5, 3), (5, 4), (5, 5), (5, 6), (5, 7), (5, 8), (7, 9), (7, 8), (7, 7), (7, 6), (7, 5), (7, 4), (7, 3), (7, 2), (7, 1), (8, 1)]),
                 ([(1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (7, 1), (8, 1), (6, 1), (8, 2), (8, 3), (8, 4), (7, 4), (7, 5), (7, 6), (9, 6), (8, 6), (8, 7), (8, 8), (7, 8), (6, 8), (5, 8), (4, 8), (3, 8), (2, 8), (1, 8), (1, 7), (1, 6), (1, 4), (1, 3), (1, 2)])]

    start = (4, 4)
    goal = (9, 5)

    plt.ion()

    #fetch individual scenarios
    for scenario in scenarios:
        robot_position = start

        #instantiate the map
        gridmap = gmap.GridMap(10, 10, 1)
        
        #instantiate the d star lite planner
        planner = dplanner.DStarLitePlanner()
        
        #plan the route from start to goal
        path = planner.plan(gridmap, gridmap.world_to_map(robot_position), gridmap.world_to_map(goal))
        
        if path == None:
            print("Destination unreachable")
            break
        else:
            path = deque(path)

        #plot the resulting map and the path 
        plot_d_star_map(gridmap, planner.getRHSvalue(), planner.getGvalue()) 
        gmap.plot_path(path)
        plt.pause(0.1)

        #locomote
        while robot_position != goal:
            #execute the step and update the map
            next_step = path.popleft()
            
            if next_step in scenario: #there is an obstacle in the way
                #update the gridmap - the cell is occupied
                gridmap.set_cell_p(next_step, 0.95)
                #replan
                path = planner.plan(gridmap, gridmap.world_to_map(robot_position), gridmap.world_to_map(goal))

                if path == None:
                    print("Destination unreachable")
                    break
                else:
                    path = deque(path)

            else: #if not, take the next step
                #update the gridmap - the cell is free
                gridmap.set_cell_p(next_step, 0.05)

                #take the next step
                robot_position = next_step

            #plot the resulting map and the path 
            plot_d_star_map(gridmap, planner.getRHSvalue(), planner.getGvalue()) 
            gmap.plot_path(path)
            plt.pause(0.1)

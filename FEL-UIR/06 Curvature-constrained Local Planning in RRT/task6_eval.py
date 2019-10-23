#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import time
import numpy as np
import matplotlib.pyplot as plt
from collections import deque

sys.path.append('environment')
sys.path.append('samplingplanner')
 
import Environment as env
import DubinsPRMPlanner as dubinsprm
import DubinsRRTPlanner as dubinsrrt


if __name__ == "__main__":
    #define the planning scenarios
    #scenario name
    #start configuration
    #goal configuration
    #limits for individual DOFs
    scenarios = [
        ("environments/simple_test", (2,2,0,0,0,0), (-2,-2,0,0,0,math.pi/2), [(-3,3), (-3,3), (0,0), (0,0), (0,0), (0,2*math.pi)]),
        ("environments/squares", (2,-3,0,0,0,0), (4,8,0,0,0,0), [(0,5), (-5,10), (0,0), (0,0), (0,0), (0,2*math.pi)]),
        ("environments/maze1", (2,2,0,0,0,math.pi/4), (35,35,0,0,0,math.pi/2), [(0,40), (0,40), (0,0), (0,0), (0,0), (0,2*math.pi)]),
        ("environments/maze2", (2,2,0,0,0,math.pi/4), (35,26,0,0,0,3*math.pi/2), [(0,40), (0,40), (0,0), (0,0), (0,0), (0,2*math.pi)]),
        ("environments/maze3", (2,2,0,0,0,0), (25,36,0,0,0,0), [(0,40), (0,40), (0,0), (0,0), (0,0), (0,2*math.pi)]),
        ("environments/maze4", (2,2,0,0,0,0), (27,36,0,0,0,math.pi/2), [(0,40), (0,40), (0,0), (0,0), (0,0), (0,2*math.pi)])
    ]

    #enable dynamic drawing in matplotlib
    plt.ion()

    ########################################
    ## EVALUATION OF THE DUBINS RRT PLANNER
    ########################################
    for scenario in scenarios:
        name = scenario[0]
        start = np.asarray(scenario[1])
        goal = np.asarray(scenario[2])
        limits = scenario[3]
        
        print("processing scenario: " + name)

        #initiate environment and robot meshes
        environment = env.Environment()
        environment.load_environment(name)

        #instantiate the planner
        planner = dubinsrrt.DubinsRRTPlanner(limits)
    
        #plan the path through the environment
        path = planner.plan(environment, start, goal)

        #plot the path step by step
        ax = None
        for Pose in path:
            ax = environment.plot_environment(Pose, ax=ax, limits=limits)
            plt.pause(0.01)

    ########################################
    ## EVALUATION OF THE DUBINS PRM PLANNER
    ########################################
    for scenario in scenarios:
        name = scenario[0]
        start = np.asarray(scenario[1])
        goal = np.asarray(scenario[2])
        limits = scenario[3]
        
        print("processing scenario: " + name)

        #initiate environment and robot meshes
        environment = env.Environment()
        environment.load_environment(name)

        #instantiate the planner
        planner = dubinsprm.DubinsPRMPlanner(limits)
    
        #plan the path through the environment
        path = planner.plan(environment, start, goal)

        #plot the path step by step
        ax = None
        for Pose in path:
            ax = environment.plot_environment(Pose, ax=ax, limits=limits)
            plt.pause(0.01)

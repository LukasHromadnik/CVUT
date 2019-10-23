#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys, os, time, re, random
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, PathPatch

sys.path.append('gridmap')
sys.path.append('gridplanner')
sys.path.append('mgmp')
sys.path.append('lkh')

import GridMap as gmap
import MGMPSolver as mgmp

##################################################
# Helper functions
##################################################
def pause(time = 1):
    plt.pause(time)

def plot_points(points, specs = 'r'):
    x_val = [x[0] for x in points]
    y_val = [x[1] for x in points]
    obj = plt.plot(x_val, y_val, specs)
    return obj

def plot_circle(xy, radius):
    ax = plt.gca()
    circle = Circle(xy, radius, facecolor='yellow',edgecolor="orange", linewidth=1, alpha=0.2)
    ax.add_patch(circle)

def plot_map():
    gmap.plot_map(gridmap)
    plot_points(goals, 'ro')
    if sensing_radius != None:
        for goal in goals:
            plot_circle(goal, sensing_radius)
        for neigh in neighborhoods:
            plot_points(neigh, 'g.')

def save_figure(filename):
    plt.gcf().savefig(filename)

def rundom_select(data, number):
    idxs = np.array(range(len(data)))
    random.shuffle(idxs)
    return [data[i] for i in idxs[:number] ]

# prepare samples from the neighborhood given by the sensing radius
def get_neighborhood(gridmap, coords, sensing_radius, max_number):
    possible = []
    for dx in range(-sensing_radius, sensing_radius+1):
        for dy in range(-sensing_radius, sensing_radius+1):
            act = (coords[0] + dx, coords[1] + dy)
            if gridmap.in_bounds(act) and gridmap.passable(act):
                dist = gridmap.dist_euclidean(coords, act)
                if dist >= sensing_radius - 1 and dist <= sensing_radius:
                    possible.append(act)
    return rundom_select(possible, max_number)

################################################
# Testing
################################################
#define planning problems:
    #  map file 
    #  maximum number of goals
    #  start index
    #  end index
    #  sensing radius of the neighborhoods
    #  maximum number of samples in a single neighborhood (None - no neighborhoods)
    #  solver type
scenarios = [
    #("maps/pacman.csv",  50, None, None, None, None, 'ETSP', 'ETSP-closed-loop'),
    #("maps/pacman.csv", 300,    0,  200, None, None, 'ETSP', 'ETSP-open-loop-large'),
    #("maps/pacman.csv", 150,    0,  149, None, None, 'ETSP', 'ETSP-open-loop-medium'),
    #("maps/pacman.csv",  50,    0,   49, None, None, 'ETSP', 'ETSP-open-loop-small'),
    
    # ("maps/pacman.csv",  50, None, None, None, None, 'Obstacles', 'Obstacles-closed-loop'),
    # ("maps/pacman.csv", 300,    0,  200, None, None, 'Obstacles', 'Obstacles-open-loop-large'),
    # ("maps/pacman.csv", 150,    0,  149, None, None, 'Obstacles', 'Obstacles-open-loop-medium'),
    # ("maps/pacman.csv",  50,    0,   49, None, None, 'Obstacles', 'Obstacles-open-loop-small'),
    
    # ("maps/pacman-small.csv", 4, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman-01.csv",  4, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman-02.csv",  4, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman-03.csv",  90, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman-04.csv",  40, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman-small.csv",  6, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman.csv",  50, None, None, None, None, 'FW', 'Floyd-Warshall-closed-loop'),
    # ("maps/pacman.csv", 300,    0,  200, None, None, 'FW', 'Floyd-Warshall-open-loop-large'),
    # ("maps/pacman.csv", 150,    0,  149, None, None, 'FW', 'Floyd-Warshall-open-loop-medium'),
    # ("maps/pacman.csv",  50,    0,   49, None, None, 'FW', 'Floyd-Warshall-open-loop-small'),

    # ("maps/maze08.csv",   3, None, None, 5,   2, 'Neighborhoods', 'Neighborhoods-small'),
    # ("maps/maze08.csv",   5, None, None, 10,   8, 'Neighborhoods', 'Neighborhoods-moderate'),
    ("maps/maze08.csv",   10, None, None, 10,   8, 'Neighborhoods', 'Neighborhoods-large'),

    # ("maps/maze08.csv",   3, None, None, 5,   2, 'NB', 'Noon-Bean-small'),
    # ("maps/maze08.csv",   5, None, None, 10,   4, 'NB', 'Noon-Bean-moderate'),
    # ("maps/maze08.csv",   10, None, None, 10,   4, 'NB', 'Noon-Bean-large'),
]

visualize = False

for scenario in scenarios:
    # Fix the problems
    random.seed(42)
    
    # Settings for the specific scenario
    mapfile = scenario[0]
    targets_max = scenario[1]
    start_idx = scenario[2]  
    end_idx = scenario[3]  
    sensing_radius = scenario[4] 
    number_of_samples = scenario[5] 
    solver_type = scenario[6] 
    title = scenario[7] 

    #instantiate the map
    gridmap = gmap.GridMap()

    #load map from file
    gridmap.load_map(mapfile, 0.1)

    all_passable = []
    for x in range(gridmap.width):
        for y in range(gridmap.height):
            coords = (x,y)
            if gridmap.passable(coords):
                all_passable.append(coords)

    goals = rundom_select(all_passable, targets_max)

    # prepare neighborhoods based on the sensing radius
    neighborhoods = None
    if sensing_radius != None:
        neighborhoods = []
        for goal in goals:
            neigh = get_neighborhood(gridmap, goal, sensing_radius, number_of_samples)
            neighborhoods.append(neigh)

    ######################################
    # plot arena and goals (and their neighborhoods)
    ######################################
    plot_map()
    plt.pause(0.1)

    ######################################
    #tour planning
    ######################################

    #instantiation of planner
    solver = mgmp.MGMP_Solver()

    if solver_type == 'ETSP':
        path = solver.plan_tour_etsp(gridmap, goals, start_idx, end_idx)
    if solver_type == 'Obstacles':
        path = solver.plan_tour_obstacles(gridmap, goals, start_idx, end_idx)
    if solver_type == 'FW':
        path = solver.plan_tour_obstacles_Floyd_Warshall(gridmap, goals, start_idx, end_idx)
    if solver_type == 'Neighborhoods':
        path = solver.plan_tour_obstacles_neighborhood(gridmap, goals, neighborhoods, start_idx, end_idx)
    if solver_type == 'NB':
        path = solver.plan_tour_obstacles_neighborhood_NB(gridmap, goals, neighborhoods, start_idx, end_idx)

    ######################################
    # plot result
    ######################################
    plot_points(path, 'r')
    pause(2)

    if visualize:
        # simulate path
        for i in range(0, len(path)):
            plot_map()
            plot_points(path, 'r')
            plot_points(path[:i], 'yo')
            plot_points([path[i]], 'go')
            pause(0.01)


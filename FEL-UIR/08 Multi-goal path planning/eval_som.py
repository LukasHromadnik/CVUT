#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: P.Vana & P.Cizek
"""

import sys, os
import re
import numpy as np

import helper as hlp
import som_solver as som

import matplotlib.pyplot as plt

################################################
# Testing
################################################
# 1) file with cities containing coordinates (idx, x, y) separated by new-line symbol
# 2) radius of the neighborhoods
test = ("./problems/burma14.txt", 0)
# test = ("./problems/burma14.txt", 0.5)
# test = ("./problems/burma14.txt", 1)
# test = ("./problems/att48.txt", 0)
# test = ("./problems/att48.txt", 200)
# test = ("./problems/att48.txt", 500)

visualize = True
#visualize = False

# read config with goal positions
goals = []
with open(test[0]) as fp:
	for line in fp:
		label, x, y = line.split()
		goals.append((float(x), float(y)))

radius = test[1]

################################################
# Visualization
################################################

def show_ring(solver, ring, color):
	hlp.clear()
	# show goals of the given TSP instance
	for g in goals:
		hlp.plot_circle(g, radius)
		hlp.plot_points([g], 'ro')

	# show the final ring of the neurons (black)
	hlp.plot_points(ring, color)

	# highlight inhibited neurons with their associated goals (green)
	inhibited_neurons = solver.get_inhibited_neurons()
	for n in inhibited_neurons:
		hlp.plot_points([n.position], 'go')
		hlp.plot_points([n.position, goals[n.goal_idx]], 'g--')
		hlp.plot_points([n.goal_position], 'rx')	
		
	pass

######################################
# Tour planning
######################################
solver = som.SOM_solver(goals, radius)

# compute deviations of the goals
np_goals = np.array([np.array(g) for g in goals])
dev = np.std(np_goals, axis=0)

# stopping criteria
max_iterations = 120
max_error = max(dev) * 1e-5

if visualize:
	show_ring(solver, solver.get_neurons_path(), 'kx-')
	hlp.pause(1)

# start of the main cyclus	
for epoch_idx in range(max_iterations):
	solver.learn_epoch(epoch_idx)

	error = solver.statistics(epoch_idx)
	if error < max_error:
		break

	# show current ring
	if visualize:
		show_ring(solver, solver.get_neurons_path(), 'kx-')
		#os.makedirs('img', exist_ok=True)
		#hlp.save_figure('img/iter-{:04d}.png'.format(epoch_idx))
		hlp.pause(0.05)

path = solver.reconstruct_path()
show_ring(solver, path, 'rx-')
hlp.pause(1)

# active waiting for end
while (visualize):
	hlp.pause(1)


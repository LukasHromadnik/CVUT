import sys
import numpy as np
import math
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, PathPatch

##################################################
# Helper functions for map plotting
##################################################
def clear():
	plt.clf()
	plt.axis('equal')

def pause(time = 1):
	plt.pause(time)

def plot_points(points, specs = 'r'):
	x_val = [x[0] for x in points]
	y_val = [x[1] for x in points]
	plt.plot(x_val, y_val, specs)

def plot_edges(edges, specs = 'g'):
	for tup in edges:
		x_val = [x[0] for x in tup]
		y_val = [x[1] for x in tup] 
		plt.plot(x_val, y_val, specs)

def plot_circle(xy, radius):
	ax = plt.gca()
	circle = Circle(xy, radius, facecolor='yellow',edgecolor="orange", linewidth=1, alpha=0.2)
	ax.add_patch(circle)
	
def save_figure(filename):
	plt.gcf().savefig(filename)

##################################################
# Helper functions for planning
##################################################

#squared the euclidean distance between two points
def cost_euclidean_squared(id1, id2):
	(x1, y1) = id1
	(x2, y2) = id2
	return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)

#the euclidean distance between two points
def cost_euclidean(id1, id2):
	return math.sqrt(cost_euclidean_squared(id1, id2))

#the euclidean distance of the path
def cost_euclidean_path(path):
	dst = 0
	last = path[0]
	for act in path:
		dst = dst + cost_euclidean(last, act)
		last = act
	return dst

#!/usr/bin/env python3
"""
source: https://en.wikipedia.org/wiki/Maze_generation_algorithm
modified by P.Cizek
"""
import sys
import numpy as np
from numpy.random import random_integers as rand
import matplotlib.pyplot as plt
import scipy.ndimage as ndimg

def maze(width=21, height=21, complexity=.75, density=.75):
        # Only odd shapes
        shape = ((height // 2) * 2 + 1, (width // 2) * 2 + 1)
        # Adjust complexity and density relative to maze size
        complexity = int(complexity * (5 * (shape[0] + shape[1])))
        density = int(density * ((shape[0] // 2) * (shape[1] // 2)))
        
        print("Generating maze " + str(shape[0]) + "x" + str(shape[1]))
        # Build actual maze
        Z = np.zeros(shape, dtype=bool)
        # Fill borders
        Z[0, :] = Z[-1, :] = 1
        Z[:, 0] = Z[:, -1] = 1
        # Make aisles
        for i in range(density):
                x, y = rand(0, shape[1] // 2) * 2, rand(0, shape[0] // 2) * 2
                Z[y, x] = 1
                for j in range(complexity):
                        neighbours = []
                        if x > 1: neighbours.append((y, x - 2))
                        if x < shape[1] - 2: neighbours.append((y, x + 2))
                        if y > 1: neighbours.append((y - 2, x))
                        if y < shape[0] - 2: neighbours.append((y + 2, x))
                        if len(neighbours):
                                y_,x_ = neighbours[rand(0, len(neighbours) - 1)]
                                if Z[y_, x_] == 0:
                                        Z[y_, x_] = 1
                                        Z[y_ + (y - y_) // 2, x_ + (x - x_) // 2] = 1
                                        x, y = x_, y_
                                        
        # Make entrance and exit
        xx = round(shape[0]/2)
        #Z[xx,0] = False
        #Z[xx,1] = False
        #Z[xx,-1] = False
        #Z[xx,-2] = False

        sh = Z.shape
        factor = 9
        gmap = np.zeros((factor*sh[0], factor*sh[1]))
        #resample
        for i in range(0, Z.shape[0]):
            for j in range(0, Z.shape[1]):
                if Z[i,j] == True:
                    for k in range(0,factor):
                        for l in range(0,factor):
                            gmap[factor*i+k, factor*j+l] = 1
        
        #binary erosion
        gg = ndimg.distance_transform_edt(gmap)
        gmap[gg < 4] = 0        
        return gmap
        
if __name__ == "__main__":
        if len(sys.argv) < 5:
                print("Usage: maze_gen.py [width][height][density][file_name]")
                sys.exit(1)

        width = int(sys.argv[1])
        height = int(sys.argv[2])
        density = float(sys.argv[3])
        filename = sys.argv[4]
        
        assert 0 < width < 200, "Select width between 0 and 200"
        assert 0 < height < 200, "Select height between 0 and 200"
        assert 0 < density < 1, "Select density between 0 and 1"

        #generate maze
        g_maze = maze(width, height, 0.1, density)
        
        #plot the maze
        plt.figure(figsize=(10, 5))
        plt.imshow(g_maze, cmap=plt.cm.binary, interpolation="nearest")
        plt.xticks([]), plt.yticks([])
        plt.show()
        
        #save the maze
        np.savetxt(filename, g_maze, delimiter=',')

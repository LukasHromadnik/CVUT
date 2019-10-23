#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
import scipy.ndimage as ndimg

import matplotlib.pyplot as plt
import csv


class GridMap:

    def __init__(self, width=0, height=0, scale=0.1):
        """
        Parameters
        ----------
        width : int, optional
            width of the grid map
        height : int, optional
            height of the grid map
        scale : double, optional
            the scale of the map 
        """
        if width > 0 and height > 0:
            self.grid = np.zeros((width, height), dtype=[('p',np.dtype(float)),('free',np.dtype(bool))])
            self.grid['p'] = 0.5    #set all probabilities to 0.5
            self.grid['free'] = True #set all cells to passable
            self.width = width
            self.height = height
            self.scale = scale
        else:
            self.grid = None
            self.width = None
            self.height = None
            self.scale = None

    def load_map(self, filename, scale=0.1):
        """ 
        Load map from file 
        
        Parameters
        ----------
        filename : str 
            path to the map file
        scale : double (optional)
            the scale of the map
        """
        grid = np.genfromtxt(filename, delimiter=',')
        grid = grid*0.95 + 0.025
        grid = np.flip(grid,0) #to cope with the vrep scene

        self.grid = np.zeros(grid.shape, dtype=[('p',np.dtype(float)),('free',np.dtype(bool))])
        self.grid['p'] = grid   #populate the occupancy grid
        self.grid['free'] = True    #assign passable and impassable cells
        self.grid['free'][grid > 0.5] = False

        self.width = self.grid.shape[0]
        self.height = self.grid.shape[1]
        self.scale = scale


    def fuse_laser_scan(self, pose, scan_x, scan_y):
        """
        Method to fuse the laser scanner data into the map

        Parameters
        ----------
        pose: (float, float, float)
            pose of the robot (x, y, orientation)
        scan_x: list(float)
        scan_y: list(float)
            relative x and y coordinates of the points in laser scan w.r.t. the current robot pose
        """
        #TODO: Task02b - Implement the update of the occupancy grid map
        #rotate

        x_off = pose[0]
        y_off = pose[1]
        phi_off = pose[2]
        scan_x = np.array(scan_x)
        scan_y = np.array(scan_y)

        R = np.array([[math.cos(phi_off), -math.sin(phi_off)],
                      [math.sin(phi_off), math.cos(phi_off)]])
        p = np.vstack([scan_x.T, scan_y.T])
        p = self.world_to_map(np.dot(p.T, R.T))

        xx = [x[0] + 10*x_off for x in p]
        yy = [x[1] + 10*y_off for x in p]
        
        start = (int(10*x_off),int(10*y_off))

        plt.clf()
        for i in range(0,len(xx)):
            goal = (int(xx[i]),int(yy[i]))
            self.bresenham_line_update(start,goal)

    def bresenham_line_update(self, start, goal):
        """
        Bresenham's line algorithm
        """
        (x0, y0) = start
        (x1, y1) = goal
        line = []
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        x, y = x0, y0
        sx = -1 if x0 > x1 else 1
        sy = -1 if y0 > y1 else 1
        if dx > dy:
            err = dx / 2.0
            while x != x1:
                self.update_cell_free((y,x))
                #line.append((x,y))
                err -= dy
                if err < 0:
                    y += sy
                    err += dx
                x += sx
        else:
            err = dy / 2.0
            while y != y1:
                self.update_cell_free((y,x))
                #line.append((x,y))
                err -= dx
                if err < 0:
                    x += sx
                    err += dy
                y += sy
        x = goal[0]
        y = goal[1]
        self.update_cell_occupied((y,x))

    def update_cell_free(self, coord):
        if self.in_bounds(coord):
            P_mi = self.get_cell_p(coord)
            p_mi = P_mi*0.05/(P_mi*0.05 + 0.95*(1-P_mi))
            p_mi = np.around(p_mi,2)
            self.set_cell_p((coord), p_mi)
        
    def update_cell_occupied(self, coord):
        if self.in_bounds(coord):
            P_mi = self.get_cell_p(coord)
            p_mi = P_mi*0.95/(P_mi*0.95 + 0.05*(1-P_mi))
            p_mi = np.around(p_mi,2)
            self.set_cell_p((coord), p_mi)

    def grow_obstacles(self, distance):
        """
        Method to blow the obstacles to prevent the robot from hitting the obstacles
        The method sets the 'free' flag for the map points that are closer to the obstacles than a given threshold distance

        Parameters
        ----------
        distance: float
            the distance for blowing the obstacles in world units
        """
        #TODO: Task03 - obstacle blowing
        grid = np.copy(self.grid['p'])
        grid[grid>0.5] = 1  #obstacles
        grid[grid<=0.5] = 0 #free space
        grid = -(grid-1.0)
        
        gg = ndimg.distance_transform_edt(grid)
        dd = distance/self.scale

        plt.imshow(gg, cmap='hot', interpolation='nearest')
        plt.show()

        self.grid['free'] = True  
        self.grid['free'][gg < dd] = False  
         

    def get_cell_p(self, coord):
        """
        Get content of the grid cell

        Parameters
        ----------
        coord : (int, int)
            map coordinate

        Returns
        -------
        float
            cell value or None if the cell is outside of the map
        """
        (x, y) = coord 
        ret = None
        if self.in_bounds(coord):
            ret = self.grid[x, y]['p']
        return ret

    def set_cell_p(self, coord, value):
        """
        Set content of the grid cell

        Parameters
        ----------
        coord : (int, int)
            map coordinate
        value : float
            cell value

        Returns
        -------
        bool
            True if the value has been set, False if the coordinates are outside of the grid map 
        """
        (x, y) = coord
        ret = self.in_bounds(coord)
        if ret:
            if value >= 1: 
                self.grid[x, y]['p'] = 0.95
            elif value <= 0:
                self.grid[x, y]['p'] = 0.05
            else:
                self.grid[x, y]['p'] = value

            if value > 0.5:
                self.grid[x, y]['free'] = False
            else:
                self.grid[x, y]['free'] = True

        return ret


    ##################################################
    # Planning helper functions
    ##################################################

    def in_bounds(self, coord):
        """
        Check the boundaries of the map

        Parameters
        ----------
        coord : (int, int)
            map coordinate

        Returns
        -------
        bool
            if the value is inside or outside the grid map dimensions
        """
        (x,y) = coord
        return 0 <= x < self.width and 0 <= y < self.height

    def passable(self, coord):
        """
        Check the passability of the given cell

        Parameters
        ----------
        coord : (int, int)
            map coordinate

        Returns
        -------
        bool
            if the grid map cell is occupied or not
        """
        ret = False
        if self.in_bounds(coord):
            (x,y) = coord
            ret = self.grid[x,y]['free']
        return ret

    def neighbors4(self, coord):
        """
        Returns coordinates of passable neighbors of the given cell in 4-neighborhood

        Parameters
        ----------
        coord : (int, int)
            map coordinate

        Returns
        -------
        list (int,int)
            list of neighbor coordinates 
        """
        (x,y) = coord
        results = [(x+1, y), (x, y-1), (x-1, y), (x, y+1)]
        results = list(filter(self.in_bounds, results))
        results = list(filter(self.passable, results))
        return results

    def neighbors8(self, coord):
        """
        Returns coordinates of passable neighbors of the given cell in 8-neighborhood

        Parameters
        ----------
        coord : (int, int)
            map coordinate

        Returns
        -------
        list (int,int)
            list of neighbor coordinates 
        """
        (x,y) = coord
        results = [(x+1, y+1), (x+1, y), (x+1, y-1),
                   (x,   y+1),           (x,   y-1),
                   (x-1, y+1), (x-1, y), (x-1, y-1)]
        results = list(filter(self.in_bounds, results))
        results = list(filter(self.passable, results))
        return results

    def dist_euclidean_squared(self, coord1, coord2):
        """
        Returns the direct squared euclidean distance between two points

        Parameters
        ----------
        coord1 : (int, int)
            map coordinate
        coord2 : (int, int)
            map coordinate

        Returns
        -------
        float
            squared euclidean distance between two coordinates 
        """
        (x1, y1) = coord1
        (x2, y2) = coord2
        return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)

    def dist_euclidean(self, coord1, coord2):
        """
        Returns the direct euclidean distance between two points

        Parameters
        ----------
        coord1 : (int, int)
            map coordinate
        coord2 : (int, int)
            map coordinate

        Returns
        -------
        float
            euclidean distance between two coordinates 
        """
        return math.sqrt(self.dist_euclidean_squared(coord1, coord2))

    def cost_manhattan(self, coord1, coord2):
        """
        Returns the direct manhattan distance between two points

        Parameters
        ----------
        coord1 : (int, int)
            map coordinate
        coord2 : (int, int)
            map coordinate

        Returns
        -------
        float
            manhattan distance between two coordinates 
        """
        (x1, y1) = coord1
        (x2, y2) = coord2
        return abs(x1 - x2) + abs(y1 - y2)

    ##############################################
    ## Helper functions for coordinate calculation
    ##############################################

    def world_to_map(self, coord):
        """
        function to return the map coordinates given the world coordinates

        Parameters
        ----------
        coord: list(float, float)
            list of world coordinates

        Returns
        -------
        list(int, int)
            list of map coordinates using proper scale and closest neighbor
        """
        cc = np.array(coord)/self.scale
        ci = cc.astype(int)
        if len(ci) > 2:
            return tuple(map(tuple, ci))
        else:
            return tuple(ci)

    def map_to_world(self, coord):
        """
        function to return the world coordinates given the map coordinates

        Parameters
        ----------
        coord: list(int, int)
            list of map coordinates

        Returns
        -------
        list(float, float)
            list of world coordinates
        """
        ci = np.array(coord).astype(float)
        cc = ci*self.scale
        if len(cc) > 2:
            return tuple(map(tuple, cc))
        else:
            return tuple(cc)

##############################################
## Helper functions to plot the map
##############################################
def plot_map(grid_map, cmap="Greys", clf = True, data='p'):
    """
    Method to plot the occupany grid map
    
    Parameters
    ----------
    grid_map : GridMap
        the GridMap object
    cmap : String(optional)
        the color map to be used
    clf : bool (optional)
        switch to clear or not to clear the drawing canvas
    data : String (optional)
        The grid map data to be displayed, 'p' for occupancy grid, 'free' for obstacle map
    """
    if clf:  #clear the canvas
        plt.clf()
    if data == 'p':
        plt.imshow(np.transpose(grid_map.grid['p']), cmap=cmap, interpolation='nearest', vmin=0, vmax=1)
    elif data == 'free':
        grid = np.transpose(grid_map.grid['free'])
        grid = np.array(grid, dtype=int)
        plt.imshow(grid, cmap=cmap, interpolation='nearest', vmin=0, vmax=1)
    else:
        print('Unknown data entry')
    plt.draw()

def plot_path(path, color="red"):
    """
    Method to plot the path
    
    Parameters
    ----------
    path: list(int,int)
        the list of individual waypoints
    """
    x_val = [x[0] for x in path]
    y_val = [x[1] for x in path]
    plt.plot(x_val, y_val, 'o-', color=color)
    plt.draw()


if __name__=="__main__":
    #new map
    g = GridMap(10,100,0.1)
    
    point = np.array((5,50))
    print(g.get_cell_p(point))
    print(g.set_cell_p(point, 0.9))
    point += (1,1)
    print(point)
    print(g.neighbors8(point))



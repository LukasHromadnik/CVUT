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
        if not scan_x or not scan_y:
            return
            
        robot_pos = [pose[0], pose[1]]
        indexes_to_delete = []
        for i in range(len(scan_x)):
            if abs(scan_x[i]) > 10 or abs(scan_y[i]) > 10:
                print("Index", i)
                indexes_to_delete.append(i)
        
        for i in reversed(indexes_to_delete):
            print("Delete at", i)
            del scan_x[i]
            del scan_y[i]
            print("Success")

        # Robot heading
        theta = pose[2]
        
        # Rotation matrix
        c, s = np.cos(theta), np.sin(theta)
        rotation_matrix = np.array(((c, -s), (s, c)))
        
        # Coords rotation and translation
        coords = list(zip(scan_y, scan_x))
        coords = np.dot(coords, rotation_matrix)
        coords = np.add(coords, robot_pos)

        # Scale the coordinates
        robot_pos = np.multiply(robot_pos, 10)
        coords = np.multiply(coords, 10)
        
        for point in coords:
            start = np.around(robot_pos).astype(int)
            end = np.around(point).astype(int)
            
            line_points = bresenham_line(start, end)
            for line_point in line_points:
                if line_point[0] == end[0] and line_point[1] == end[1]:
                    sO = 0.9
                    sF = 0
                else:
                    sO = 0
                    sF = 0.9
                pO = (1 + sO - sF) / 2
                pF = 1 - pO
                current_pO = self.get_cell_p(line_point)
                if current_pO == None:
                    continue
                current_pF = 1 - current_pO
                new_value = (pO * current_pO) / ((pO * current_pO) + (pF * current_pF))
                self.set_cell_p(line_point, new_value)

    def grow_obstacles(self, distance):
        """
        Method to blow the obstacles to prevent the robot from hitting the obstacles
        The method sets the 'free' flag for the map points that are closer to the obstacles than a given threshold distance

        Parameters
        ----------
        distance: float
            the distance for blowing the obstacles in world units
        """
        distance = np.ceil(distance)

        new_wall = set()

        for i in range(self.height):
            for j in range(self.width):
                coord = [i, j]
                if self.passable(coord): continue
                neighbors = self.neighbors8(coord)
                for k in range(len(neighbors)):
                    if neighbors[k] and self.passable(neighbors[k]):
                        new_wall.add(neighbors[k])
        
        for coord in new_wall:
            self.set_cell_p(coord, 1)

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

# http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm#Python
def bresenham_line(start, end):
    """Bresenham's Line Algorithm
    Produces a list of tuples from start and end
 
    >>> points1 = get_line((0, 0), (3, 4))
    >>> points2 = get_line((3, 4), (0, 0))
    >>> assert(set(points1) == set(points2))
    >>> print points1
    [(0, 0), (1, 1), (1, 2), (2, 3), (3, 4)]
    >>> print points2
    [(3, 4), (2, 3), (1, 2), (1, 1), (0, 0)]
    """
    # Setup initial conditions
    x1, y1 = start
    x2, y2 = end
    dx = x2 - x1
    dy = y2 - y1
 
    # Determine how steep the line is
    is_steep = abs(dy) > abs(dx)
 
    # Rotate line
    if is_steep:
        x1, y1 = y1, x1
        x2, y2 = y2, x2
 
    # Swap start and end points if necessary and store swap state
    swapped = False
    if x1 > x2:
        x1, x2 = x2, x1
        y1, y2 = y2, y1
        swapped = True
 
    # Recalculate differentials
    dx = x2 - x1
    dy = y2 - y1
 
    # Calculate error
    error = int(dx / 2.0)
    ystep = 1 if y1 < y2 else -1
 
    # Iterate over bounding box generating points between start and end
    y = y1
    points = []
    for x in range(x1, x2 + 1):
        coord = (y, x) if is_steep else (x, y)
        points.append(coord)
        error -= abs(dy)
        if error < 0:
            y += ystep
            error += dx
 
    # Reverse the list if the coordinates were swapped
    if swapped:
        points.reverse()
    return points


if __name__=="__main__":
    #new map
    g = GridMap(10,100,0.1)
    
    point = np.array((5,50))
    print(g.get_cell_p(point))
    print(g.set_cell_p(point, 0.9))
    point += (1,1)
    print(point)
    print(g.neighbors8(point))



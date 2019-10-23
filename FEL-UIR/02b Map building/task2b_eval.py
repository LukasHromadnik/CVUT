#!/usr/bin/env python3
# -*- coding: utf-8 -*-
 
import sys
import math
import time
import numpy as np
import matplotlib.pyplot as plt
import threading as thread
from collections import deque
 
sys.path.append('robot')
sys.path.append('gridmap') 
import Robot as rob
import GridMap as gmap
 
class task2b_eval:
    def __init__(self):
        #instantiate the robot
        self.robot = rob.Robot()
 
        #instantiate the map
        self.gridmap = gmap.GridMap(100, 100, 0.1)
 
        #route
        self.route = deque([(6.0, 8.5, None), (4.0, 6.0, None), (5.0, 5.0, None), (1.5, 1.5, None)])
        self.route_lock = thread.Lock()
 
        #navigation thread stopping 
        self.navigation_stop = False
 
    def fetch_navigation_point(self):
        """
        Method for getting the next navigation point
 
        Returns
        -------
        (float, float)
            Next navigation point
        """
        coord = None
        self.route_lock.acquire()
        try:
            coord = self.route.popleft()
        except IndexError:
            coord = None
        self.route_lock.release()
        return coord
 
    def navigation(self):
        """
        navigation function that executes the trajectory point-by-point
        """
        print("starting navigation")
        while not self.navigation_stop:
 
            pos = self.fetch_navigation_point()
 
            if pos == None:
                print("No further points to navigate")
                self.navigation_stop = True
                break
 
            print("Navigation point " + str(pos))
            #navigate the robot towards the target
 
            status1 = self.robot.goto(pos[0:2],pos[2])
 
 
    def eval(self):
        """
        Evaluation function
        """
        try:
            nav_t = thread.Thread(target=self.navigation)
        except:
            print("Error: unable to start navigation thread")
            sys.exit(1)
        nav_t.start()
 
        plt.ion()
 
        while not self.navigation_stop:
            #get the robot pose
            pose = self.robot.get_pose()
 
            #get the laser scan data
            scan_x, scan_y = self.robot.robot.get_laser_scan()
 
            #fuse the data into the map
            print("fusing new data to the occupancy grid map")
            self.gridmap.fuse_laser_scan(pose, scan_x, scan_y)
 
            #plot the gridmap
            gmap.plot_map(self.gridmap)
            plt.pause(1)
 
        nav_t.join()
 
if __name__ == "__main__":
    task = task2b_eval()
    task.eval()

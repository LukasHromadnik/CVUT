#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
import time
import matplotlib.pyplot as plt

sys.path.append('../robot')
 
import Robot as rob

if __name__=="__main__":
    robot = rob.Robot()
    
    #turn the robot into the default position
    robot.turn_on()

    plt.ion()
    while True:
        #fetch the laser scan data
        scan_x, scan_y = robot.robot.get_laser_scan()
        scan_x = np.asarray(scan_x)
        scan_y = np.asarray(scan_y)
        
        #check that the data are valid
        if scan_x.size == 0 or scan_y.size == 0:
            continue

        #plot the scanned points
        plt.clf() # clear canvas
        plt.plot(scan_x,scan_y,'.-r') #plot scanned points
        plt.axis('equal')
        plt.show()
        plt.pause(0.1)
        

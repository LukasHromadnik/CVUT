#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
sys.path.append('robot')
 
import Robot as rob
 
DISTANCE_THLD = 0.15 #m
ORIENTATION_THLD = math.pi/8
 
def check(pose_set, pose_real):
    """
    Function to check that the navigation towards the goal has been successfull
 
    Parameters
    ----------
    pose_set: (float, float, float)
        desired coordinates to reach (x,y,phi)
    pose_real: (float, float, float)
        real coordinates of the robot (x,y,phi)
 
    Returns
    -------
    bool
        True if the robot real position is in delta neighborhood of the desired position, False otherwise
    """
    ret = True
    (x1, y1, phi1) = pose_set
    (x2, y2, phi2) = pose_real
    dist = (x1 - x2)**2 + (y1-y2)**2
    #check the distance to the target
    if math.sqrt(dist) > DISTANCE_THLD:
        ret = False
    #check the final heading
    if not phi1 == None:
        dphi = phi1 - phi2
        dphi = (dphi + math.pi) % (2*math.pi) - math.pi
        if dphi > ORIENTATION_THLD:
            ret = False
 
    return ret
 
 
if __name__=="__main__":
    robot = rob.Robot()
 
    #navigation points
    route = [(1,1,None), (-1,1,math.pi/2), (-1,-1,None), (1,-1,None)]
 
    #navigate
    for waypoint in route:
        pos_des = waypoint[0:2]
        ori_des = waypoint[2]
 
        print("Navigation point " + str(pos_des) + " " + str(ori_des))
        #navigate the robot towards the target
        status1 = robot.goto(pos_des,ori_des)
 
        #get robot real position
        pose_real = robot.get_pose()
 
        #check that the robot reach the destination
        status2 = check(waypoint, pose_real)
 
        #print the result
        print(status1, status2)

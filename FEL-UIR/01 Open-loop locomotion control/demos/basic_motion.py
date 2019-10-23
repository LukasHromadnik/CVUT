#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
import time

sys.path.append('../robot')
 
import Robot as rob

TIME_FRAME = 36 #ms

if __name__=="__main__":
    robot = rob.Robot()
    
    #turn the robot into the default position
    robot.turn_on()

    #move individual servos by accessing the robotHAL through robot
    angles = np.zeros(18)
    robot.robot.set_all_servo_position(angles)

    time.sleep(10)

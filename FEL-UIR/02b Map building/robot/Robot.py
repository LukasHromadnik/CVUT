#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import math
import time
import numpy as np
import threading as thread

#hexapod robot 
import hexapod_sim.RobotHAL as robothal
#import hexapod_real.RobotHAL as robothal

#cpg network
import cpg.oscilator_network as osc

from RobotConst import *

class Robot:
    def __init__(self):
        self.robot = robothal.RobotHAL(TIME_FRAME)
        self.v_left = 1
        self.v_right = 1
        self.stop = False
        self.steering_lock = thread.Lock()  #mutex for access to turn commands
        self.turning_speed = 0.5
        self.base_speed = 0.3
        self.avoid_speed = 0.7

    def get_pose(self):
        """
        Method to get the robot pose

        Returns
        -------
        np.array (float, float, float)
            position and orientation
        """
        coord = self.robot.get_robot_position()
        while coord == None:
            coord = self.robot.get_robot_position()
        
        phi = self.robot.get_robot_orientation()
        while phi == None:
            phi = self.robot.get_robot_orientation()
        
        return np.append(coord, phi)
    
    def get_robot_position(self):
        coord = self.robot.get_robot_position()
        while coord == None:
            coord = self.robot.get_robot_position()
        
        return coord
    
    def get_robot_orientation(self):
        phi = self.robot.get_robot_orientation()
        while phi == None:
            phi = self.robot.get_robot_orientation()
        
        return phi

    def turn_on(self):
        """
        Method to drive the robot into the default position
        """
        #read out the current pose of the robot
        pose = self.robot.get_all_servo_position()

        #interpolate to the default position
        INTERPOLATION_TIME = 3000 #ms
        interpolation_steps = int(INTERPOLATION_TIME/TIME_FRAME)

        speed = np.zeros(18)
        for i in range(0,18):
            speed[i] = (SERVOS_BASE[i]-pose[i])/interpolation_steps
        
        #execute the motion
        for t in range(0, interpolation_steps):
            self.robot.set_all_servo_position(pose + t*speed)
            pass

    def locomotion(self):
        """
        method for locomotion control of the hexapod robot
        """
        #cpg network instantiation
        cpg = osc.OscilatorNetwork(6)
        cpg.change_gait(osc.TRIPOD_GAIT_WEIGHTS)
        
        coxa_angles= [0, 0, 0, 0, 0, 0]
        cycle_length = [0, 0, 0, 0, 0, 0]
        
        #main locomotion control loop
        while not self.stop:
            # steering - acquire left and right steering speeds
            self.steering_lock.acquire()
            left = np.min([1, np.max([-1, self.v_left])])
            right = np.min([1, np.max([-1, self.v_right])])
            self.steering_lock.release()

            coxa_dir = [left, left, left, right, right, right]      #set directions for individual legs

            #next step of CPG
            cycle = cpg.oscilate_all_CPGs()
            #read the state of the network
            data = cpg.get_last_values()

            #reset coxa angles if new cycle is detected
            for i in range(0, 6):
                cycle_length[i] += 1
                if cycle[i] == True:
                    coxa_angles[i]= -((cycle_length[i]-2)/2)*COXA_MAX
                    cycle_length[i] = 0

            angles = np.zeros(18)
            #calculate individual joint angles for each of six legs
            for i in range(0, 6):
                femur_val = FEMUR_MAX*data[i] #calculation of femur angle
                if femur_val < 0:
                    coxa_angles[i] -= coxa_dir[i]*COXA_MAX  #calculation of coxa angle -> stride phase
                    femur_val *= 0.025
                else:
                    coxa_angles[i] += coxa_dir[i]*COXA_MAX  #calculation of coxa angle -> stance phase
                
                coxa_val = coxa_angles[i]
                tibia_val = -TIBIA_MAX*data[i]       #calculation of tibia angle
                    
                #set position of each servo
                angles[COXA_SERVOS[i] - 1] = SIGN_SERVOS[i]*coxa_val + COXA_OFFSETS[i]
                angles[FEMUR_SERVOS[i] - 1] = SIGN_SERVOS[i]*femur_val + FEMUR_OFFSETS[i]
                angles[TIBIA_SERVOS[i] - 1] = SIGN_SERVOS[i]*tibia_val + TIBIA_OFFSETS[i]
                    
            #set all servos simultaneously
            self.robot.set_all_servo_position(angles)
            

    def goto(self, coord, phi=None):
        """
        open-loop navigation towards a selected goal, with an optional final heading
     
        Parameters
        ----------
        coord: (float, float)
            coordinates of the robot goal 
        phi: float, optional
            optional final heading of the robot
     
        Returns
        -------
        bool
            True if the destination has been reached, False if the robot has collided
        """

        ret = True
        
        # starting the locomotion thread
        try:
            self.stop = False
            thread1 = thread.Thread(target=self.locomotion)
        except:
            print("Error: unable to start locomotion thread")
            sys.exit(1)

        thread1.start()

        # print(self.angle_between_points([1, 0], [0, 0], [0, 1])) => 1.52, 90°
        # Referenční pohled: dolů

        while not self.goal_reached(self.get_robot_position(), coord):
            if self.robot.get_robot_collision():
                return False

            dphi = self.compute_dphi_for_goal(coord)
            left = -dphi + 1
            right = dphi + 1
            self.set_left_right(left, right)

            # Let the robot make a step
            time.sleep(2)

        if not phi == None:
            while not self.phi_reached(self.get_robot_orientation(), phi):
                if self.robot.get_robot_collision():
                    return False
                dphi = self.compute_dphi(phi, self.get_robot_orientation())
                self.set_left_right(-dphi, dphi)

                # Let the robot make a step
                time.sleep(1)
        
        # Stop the locomotion thread
        self.stop = True
        thread1.join()
        
        return ret

    
    def compute_dphi(self, target, robot):
        if target > robot:
            delta = target - robot
        else:
            delta = -(robot - target)

        if delta < 0 and abs(delta) > np.pi:
            return delta + 2 * np.pi
        elif delta > np.pi:
            return delta - 2 * np.pi
        else:
            return delta

    def goal_reached(self, position, goal):
        delta_x = position[0] - goal[0]
        delta_y = position[1] - goal[1]

        return (abs(delta_x) < DISTANCE_THLD) and (abs(delta_y) < DISTANCE_THLD)

    def phi_reached(self, robot, goal):
        orientation_thld = math.pi/8
        return abs(robot - goal) < orientation_thld

    def angle_between_points(self, p0, p1, p2):
        a = (p1[0] - p0[0]) ** 2 + (p1[1] - p0[1]) ** 2
        b = (p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2
        c = (p2[0] - p0[0]) ** 2 + (p2[1] - p0[1]) ** 2
        result = np.arccos((a + b - c) / np.sqrt(4 * a * b))

        if p2[1] < p0[1]:
            return 2 * np.pi - result
        else:
            return result

    def set_left_right(self, left, right):
        self.steering_lock.acquire()
        self.v_left = left
        self.v_right = right
        self.steering_lock.release()
    
    def get_robot_down_position(self):
        robot_pos = self.get_robot_position()
        return [robot_pos[0] + 1, robot_pos[1]]

    def get_angle_between_robot_and_point(self, goal):
        return self.angle_between_points(self.get_robot_down_position(), self.get_robot_position(), goal)

    def compute_dphi_for_goal(self, goal):
        target_angle = self.get_angle_between_robot_and_point(goal)
        return self.compute_dphi(target_angle, self.get_robot_orientation())

    def compute_distance(self, p1, p2):
        return np.sqrt((p1[0] - p2[0])** 2 + (p1[1] - p2[1])** 2)

    def goto_reactive(self, coord):
        """
        Navigate the robot towards the target with reactive obstacle avoidance 
     
        Parameters
        ----------
        coord: (float, float)
            coordinates of the robot goal 

        Returns
        -------
        bool
            True if the destination has been reached, False if the robot has collided
        """
        ret = True
        
        # starting the locomotion thread
        try:
            self.stop = False
            thread1 = thread.Thread(target=self.locomotion)
        except:
            print("Error: unable to start locomotion thread")
            sys.exit(1)

        thread1.start()

        while not self.goal_reached(self.get_robot_position(), coord):
            if self.robot.get_robot_collision():
                return False

            try:
                scan_x, scan_y = self.robot.get_laser_scan()
            except:
                print('Unable to laser scan data')
                time.sleep(2)
                continue

            if not scan_x or not scan_y:
                time.sleep(2)
                continue

            distances = np.array([np.sqrt(scan_x[i]** 2 + scan_y[i]** 2) for i in range(len(scan_x))])
            middle = int(np.floor(len(distances) / 2))
            
            left_distances = distances[middle:]
            right_distances = distances[:middle + 1]
            min_left = np.amin(left_distances)
            min_right = np.amin(right_distances)
            
            dphi = self.compute_dphi_for_goal(coord)
            left = (1 / min_left) * self.avoid_speed - dphi * self.turning_speed + self.base_speed
            right = (1 / min_right) * self.avoid_speed + dphi * self.turning_speed + self.base_speed
            self.set_left_right(left, right)
            time.sleep(2)

        return ret


if __name__=="__main__":
    robot = Robot()
    #drive the robot into the default position
    robot.turn_on()
    time.sleep(3)
    robot.goto((1,1), phi=math.pi)

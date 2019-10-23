#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
import collections
import matplotlib.pyplot as plt
from Planner import Planner
from PriorityQueue import PriorityQueue

class RRTPlanner(Planner):

    def plan(self, environment, start, goal):
        """
        Method to plan the path

        Parameters
        ----------
        environment: Environment
            Map of the environment that provides collision checking 
        start: numpy array (6x1)
            start configuration of the robot (x,y,z,phi_x,phi_y,phi_z)
        goal: numpy array (6x1)
            goal configuration of the robot (x,y,z,phi_x,phi_y,phi_z)

        Returns
        -------
        list(numpy array (4x4))
            the path between the start and the goal Pose in SE(3) coordinates
        """

        n_samples = 1
        
        nodes = []
        nodes.append(start)
        
        predecessors = []
        predecessors.append(-1)

        n_iterations = 0
        
        # environment.plot_environment(self.construct_pose_matrix(start))
        while True:
            # Generate valid sample
            sample = self.generate_samples(n_samples, start.size)[0]
            while environment.check_robot_collision(self.construct_pose_matrix(sample)):
                sample = self.generate_samples(n_samples, start.size)[0]

            # Find the nearest node to the sample
            min_node_index = -1
            min_distance = np.inf
            for i in range(len(nodes)):
                distance = np.linalg.norm(nodes[i][0:3] - sample[0:3])
                if min_distance > distance:
                    min_distance = distance
                    min_node_index = i

            # distance_queue = PriorityQueue()
            # for i in range(len(nodes)):
            #     distance = np.linalg.norm(nodes[i][0:3] - sample[0:3])
            #     distance_queue.push(i, distance)
            
            # Add small step toward the sample
            # while distance_queue.empty() == False:
            # _, top = distance_queue.pop()
            min_node = nodes[min_node_index]
            
            # Compute the new point      
            computed_path, _ = self.compute_path_between(min_node, sample)
            new_node = computed_path[0]

            # Check if the new node is valid
            if environment.check_robot_collision(self.construct_pose_matrix(new_node)):
                continue

            nodes.append(new_node)
            predecessors.append(min_node_index)
            # self.plot_nodes([new_node])

            # Check if the goal is reachable
            if self.is_goal_reachable(nodes[-1], goal, environment):
                break
            
            if n_iterations % 100 == 0:
                print("Iteration:", n_iterations)
            n_iterations += 1
        
        # Add the goal state to the nodes list
        predecessors.append(len(nodes) - 1)
        nodes.append(goal)

        # Path reconstruction
        path = self.reconstruct_path(nodes, predecessors, len(nodes) - 1)

        return list(map(lambda x: self.construct_pose_matrix(x), path))

    def is_goal_reachable(self, node, goal, environment):
        path, _ = self.compute_path_between(node, goal)
        return self.is_path_valid(path, environment)

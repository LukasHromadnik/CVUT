#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np
import collections
import matplotlib.pyplot as plt
from PriorityQueue import PriorityQueue
from Planner import Planner

class PRMPlanner(Planner):

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

        # Generate random configurations
        n_points = 30

        # Random samples
        samples = self.generate_samples(n_points, start.size)
        
        # samples = samples.T 
        
        # Filter samples that have collision
        samples_new = []
        samples_new.append(start)
        for sample in samples:
            pose = self.construct_pose_matrix(sample)
            if environment.check_robot_collision(pose) == False:
                samples_new.append(sample)
        samples_new.append(goal)
        samples = samples_new
        
        # Connect the graph
        n_samples = len(samples)
        distances = np.full(n_samples, np.inf)
        distances[0] = 0
        predecessors = np.full(n_samples, -1)
        queue = PriorityQueue()
        queue.push(0, 0)

        # Dijkstra
        while queue.empty() == False:
            priority, top = queue.pop()
            if priority > distances[top]:
                continue
            for i in range(n_samples):
                if i == top:
                    continue
                computed_path, computed_distance = self.compute_path_between(samples[top], samples[i])
                if self.is_path_valid(computed_path, environment) == False:
                    continue
                    
                alt = distances[top] + computed_distance
                if alt < distances[i]:
                    distances[i] = alt
                    predecessors[i] = top
                    queue.push(i, distances[i])
        
        # Path reconstruction
        path = self.reconstruct_path(samples, predecessors, len(samples) - 1)

        return list(map(lambda x: self.construct_pose_matrix(x), path))

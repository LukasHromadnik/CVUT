#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from RRTPlanner import RRTPlanner
from DubinsPlanner import DubinsPlanner

class DubinsRRTPlanner(RRTPlanner):

    def setup(self):
        self.dubins_planner = DubinsPlanner()
    
    def compute_path_between(self, start, goal):
        path, distance = self.dubins_planner.compute_path_between(start, goal)
        return (path[1:], distance)
    
    def reconstruct_path(self, nodes, predecessors, end_index):
        pred = end_index
        path = []

        if predecessors[pred] == -1:
            return path
        to_goal = True
        while pred != 0:
            if to_goal:
                computed_path, _ = self.compute_path_between(nodes[predecessors[pred]], nodes[pred])
                to_goal = False
            else:
                computed_path = [nodes[pred]]
            pred = predecessors[pred]
            path = computed_path + path
        
        return path

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from PRMPlanner import PRMPlanner
from DubinsPlanner import DubinsPlanner

class DubinsPRMPlanner(PRMPlanner):

    def setup(self):
        self.dubins_planner = DubinsPlanner()
    
    def compute_path_between(self, start, goal):
        return self.dubins_planner.compute_path_between(start, goal)

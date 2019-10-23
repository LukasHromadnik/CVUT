#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

import collections
import heapq

import matplotlib.pyplot as plt

import GridMap as gmap

class GridPlanner:

    def __init__(self):
        pass

    def plan(self, gridmap, start, goal):
        """
        Method to plan the path

        Parameters
        ----------
        gridmap: GridMap
            gridmap of the environment
        start: (int,int)
            start coordinates
        goal:(int,int)
            goal coordinates

        Returns
        -------
        list(int,int)
            the path between the start and the goal if there is one, None if there is no path
        """
        #TODO: Task03 - Implement a grid-based planner

        return None

    def simplify_path(self, gridmap, path):
        """
        Method to implify the found path

        Parameters
        ----------
        gridmap: GridMap
            gridmap of the environment
        path: list(int,int)
            the path found by the planner

        Returns
        -------
        list(int,int)
            the simplified
        """
        #TODO: Task03 - Implement path simplification

        return path

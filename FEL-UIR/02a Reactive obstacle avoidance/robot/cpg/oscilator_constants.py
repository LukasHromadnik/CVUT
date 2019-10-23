# -*- coding: utf-8 -*-
from numpy import array
import numpy as np

debug = False

T_r = 1.7 # > 0 for oscilatory waves, A dicreases with increment of T_r and P increases, T_r -> (0, 2>
T_a = 1.5 # > 0 for oscilatory waves, P and A increase with the increment of T_a -> (0, 3>
s = [0.8, 0.8] # the higher s is the higher the amplitude is, should be the same values and > 0 for oscilatory waves
beta = [7, 7] # > 3.5 for oscilatory waves, the higher beta is, the lower A and P is, beta -> <3.5, 8>
w_fe = -4 # -> <-4, 0> the higher w is, the lower A is, in <-1, 0> P decreases with increment of w

SLEEP_TIME = 0.001
frequency = 8
STABILIZATION = 30 * frequency

TRIPOD_GAIT_WEIGHTS = array([
	[0, -0.4, 0.4, -0.4, 0.4, -0.4],
	[-0.4, 0, -0.4, 0.4, -0.4, 0.4],
	[0.4, -0.4, 0, -0.4, 0.4, -0.4],
	[-0.4, 0.4, -0.4, 0, -0.4, 0.4],
	[0.4, -0.4, 0.4, -0.4, 0, -0.4],
	[-0.4, 0.4, -0.4, 0.4, -0.4, 0]])

QUADRUPED_GAIT_WEIGHTS = array([
	[0, -0.4, -0.4, -0.4, 0.4, -0.4],
	[-0.4, 0, -0.4, -0.4, -0.4, 0.4],
	[-0.4, -0.4, 0, 0.4, -0.4, -0.4],
	[-0.4, -0.4, 0.4, 0, -0.4, -0.4],
	[0.4, -0.4, -0.4, -0.4, 0, -0.4],
	[-0.4, 0.4, -0.4, -0.4, -0.4, 0]])

SUPPORT_GAIT_WEIGHTS = array([
	[   0, -1.0, -1.0, -1.0, -1.0, -1.0],
	[-1.0,    0, -1.0, -1.0, -1.0, -1.0],
	[-1.0, -1.0,    0, -1.0, -1.0, -1.0],
	[-1.0, -1.0, -1.0,    0, -1.0, -1.0],
	[-1.0, -1.0, -1.0, -1.0,    0, -1.0],
	[-1.0, -1.0, -1.0, -1.0, -1.0,    0]])


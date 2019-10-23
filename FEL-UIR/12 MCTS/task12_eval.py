#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import math
import time
import numpy as np
import matplotlib.pyplot as plt

sys.path.append('gridmap')
sys.path.append('gridplanner')
sys.path.append('game')
sys.path.append('player')
 
import GridMap as gmap
import Game as gm
import Player as pl

TIMEOUT=1.0

if __name__=="__main__":
 
    #define games:
    #  problem/map name 
    #  game definition file
    games = [("grid", "games/grid_1.game"),
             ("grid", "games/grid_2.game"),
             ("pacman_small", "games/pacman_small_1.game"),
             ("pacman_small", "games/pacman_small_2.game"),
             ("pacman", "games/pacman_1.game"),
             ("pacman", "games/pacman_2.game")]
    

    #fetch individual scenarios
    for game in games:

        #instantiate the map
        gridmap = gmap.GridMap()
        
        #load map from file
        gridmap.load_map("maps/" + game[0] + ".csv")

        #load the game configuration
        players = []
        with open(game[1]) as fp:
            for line in fp:
                if line[0] == "#": # skip comments
                    continue
                q = line.split()
                
                role = q[0]   #player role
                policy = q[1] #player policy
                color = q[2]  #player color
                robots = []   #list of player's robots
                for x in range(3,len(q),2):
                    robots.append((int(q[x]),int(q[x+1])))
                
                players.append(pl.Player(robots, role, policy=policy, color=color, timeout=TIMEOUT, game=game[0]))

        #instantiate the game
        pursuit_game = gm.Game(gridmap, players)
        pursuit_game.plot_game()
        pursuit_game.plot_pause(2)

        ######################################
        # Simulate the game
        ######################################

        # simulate game of n_steps
        n_steps = 100

        for i in range(0, n_steps):
            #if not all the evaders have been captured
            if not pursuit_game.is_end():
                #make a turn
                pursuit_game.step()

                #plot the game result
                pursuit_game.plot_game()
                pursuit_game.plot_pause(0.01)
                
        if not pursuit_game.is_end():
            print("Evader escaped")
        else:
            print("GAME OVER")
    
        # waiting for end
        pursuit_game.plot_pause(2)
            

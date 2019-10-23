from carddeck import *
import random
import numpy as np

class SarsaAgent:
    '''
    Here you will provide your implementation of SARSA method.
    You are supposed to implement train() method. If you want
    to, you can split the code in two phases - training and
    testing, but it is not a requirement.

    For SARSA explanation check AIMA book or Sutton and Burton
    book. You can choose any strategy and/or step-size function
    (learning rate).
    '''
    def __init__(self, env, number_of_epochs):
        self.env = env
        self.number_of_epochs = number_of_epochs
        
        self.number_of_player_states = 32
        self.number_of_dealer_states = 12
        self.number_of_actions = 2

        self.Q = [[[0 for i in range(self.number_of_actions)] for k in range(self.number_of_dealer_states)] for j in range(self.number_of_player_states)]
        self.n = [[[0 for i in range(self.number_of_actions)] for k in range(self.number_of_dealer_states)] for j in range(self.number_of_player_states)]

    def train(self):
        alpha = 0.01
        discount_factor = 0.5

        for i in range(self.number_of_epochs):
            observation = self.env.reset()
            self.dealer_hand = observation.dealer_hand.value()
            # self.dealer_hand = 0
            
            done = False
            reward = 0
            state = observation.player_hand.value()
            action = self.make_step(state, i, done)
           
            while not done:
                # self.env.render()
                new_observation, reward, done, _ = self.env.step(action)
            
                state_ = new_observation.player_hand.value()
                action_ = self.make_step(state, i, done)
                
                current_utility = self.Q[state][self.dealer_hand][action]
                next_utility = self.Q[state_][self.dealer_hand][action_]
                coef = self.n[state][self.dealer_hand][action]
                
                self.Q[state][self.dealer_hand][action] += (alpha ** coef) * (reward + discount_factor * next_utility - current_utility)
                # self.Q[state][self.dealer_hand][action] += alpha * (reward + discount_factor * next_utility - current_utility)
                self.n[state][self.dealer_hand][action] += 1

                observation = new_observation
                state = state_
                action = action_
    
    def make_step(self, state, epoch, terminal):
        if terminal:
            return 0

        if epoch < self.number_of_epochs * 0.7:
            if state < 17:
                epsilon = 1
            else:
                epsilon = 0.4
        else:
            epsilon = 0.2

        if random.uniform(0, 1) < epsilon:
            return self.env.action_space.sample()
        else:
            max_value = -1000
            max_action = -1
            for action in range(self.number_of_actions):
                current_utility = self.Q[state][self.dealer_hand][action]
                if current_utility > max_value:
                    max_value = current_utility
                    max_action = action
            return max_action

    def print_utility(self):
        for i in range(self.number_of_player_states):
            for j in range(self.number_of_dealer_states):
            # print("{:5.2f} ".format(sum(self.U[i])), end=" ")
                print("{:5.2f}".format(sum(self.Q[i][j])), end=" ")
            print()
        # for i in range(self.number_of_player_states):
        #     total = 0
        #     for j in range(self.number_of_dealer_states):
        #         total += sum(self.Q[i][j])
            
        #     print("{:5.2f}".format(total), end=" ")
        #     # for j in range(12):
        #     # print("{:5.2f} ".format(sum(self.Q[i])), end=" ")
        #         # print("{:5.2f} ".format(self.U[i][j]), end='')
        # print()

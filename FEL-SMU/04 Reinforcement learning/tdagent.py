from carddeck import *
from tabulate import tabulate

class TDAgent:
    '''
    Implementaion of an agent that plays the same strategy as the dealer.
    This means that the agent draws a card when sum of cards in his hand
    is less than 17.

    Your goal is to modify train() method to learn the state utility function.
    I.e. you need to change this agent to a passive reinforcement learning
    agent that learns utility estimates using temporal diffrence method.
    '''
    def __init__(self, env, number_of_epochs):
        self.env = env
        self.number_of_epochs = number_of_epochs

        self.U = [[0 for i in range(12)] for j in range(30)]
        self.ns = [[0 for i in range(12)] for j in range(30)]

    def train(self):
        alpha = 0.2
        discount_factor = 0.5
        
        for i in range(self.number_of_epochs):
            observation = self.env.reset()
            self.dealer_hand = observation.dealer_hand.value()

            state = observation.player_hand.value()
            done = False
            reward = 0

            while not done:
                # render method will print you the situation in the terminal
                #self.env.render()
                action = self.make_step(state)
                new_observation, reward, done, _ = self.env.step(action)
                state_ = new_observation.player_hand.value()

                # print(state, self.dealer_hand)
                current_utility = self.U[state][self.dealer_hand]
                next_utility = self.U[state][self.dealer_hand]
                coef = self.ns[state][self.dealer_hand]

                self.U[state][self.dealer_hand] += alpha ** self.ns[state][self.dealer_hand] * (reward + discount_factor * next_utility - current_utility)
                self.ns[state][self.dealer_hand] += 1

                observation = new_observation
                state = state_

    def make_step(self, state):
        return 1 if state < 17 else 0

    def print_utility(self):
        for i in range(30):
            for j in range(12):
            # print("{:5.2f} ".format(sum(self.U[i])), end=" ")
                print("{:5.2f} ".format(self.U[i][j]), end='')
            print()

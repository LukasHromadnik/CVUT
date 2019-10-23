from carddeck import *

class RandomAgent:
    '''
    implementaion of an agent that decides completely at random
    '''
    def __init__(self, env, number_of_epochs):
        self.env = env
        self.number_of_epochs = number_of_epochs

    def train(self):
        for i in range(self.number_of_epochs):
            observation = self.env.reset()
            terminal = False
            reward = 0
            while not terminal:
                # self.env.render() # you may want to see the game actually being played
                action = self.make_step(observation, reward, terminal)
                observation, reward, terminal, _ = self.env.step(action)
            # self.env.render()

    def make_step(self, observation, reward, terminal):
        return self.env.action_space.sample()

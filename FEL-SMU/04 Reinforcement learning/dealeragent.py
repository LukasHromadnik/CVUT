from carddeck import *

class DealerAgent:
    '''
    Implementaion of an agent that plays the same strategy as the dealer.
    This means that the agent draws a card when sum of cards in his hand
    is less than 17.
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
                action = self.make_step(observation, reward, terminal)
                observation, reward, terminal, _ = self.env.step(action)

    def make_step(self, observation, reward, terminal):
        return 1 if observation.player_hand.value() < 17 else 0

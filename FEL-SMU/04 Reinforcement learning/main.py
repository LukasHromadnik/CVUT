from dealeragent import DealerAgent
from evaluate import *
import gym
from gym import wrappers
from gym.envs.registration import register
from randomagent import RandomAgent
from sarsaagent import SarsaAgent
from tdagent import TDAgent

def get_env():
    '''
    Creates the environment. Check the OpenAI Gym documentaion.
    '''
    env = gym.make('smu-blackjack-v0')
    return wrappers.Monitor(env, 'smuproject4', force=True, video_callable=False)

if __name__ == "__main__":
    '''
    Registers the environment so that it can be used
    '''
    register(
             id='smu-blackjack-v0',
             entry_point='blackjack:BlackjackEnv'
             )
    #### IMPORTANT: do not modify the code above this line! ###

    # here you can play with the code yourself
    # for example you may want to split the code to two phases - training and testing
    # or you may want to compare two agents
    # feel free to modify the number of games played
    # ... or whatever

    env = get_env()
    number_of_epochs = 100

    # agent = RandomAgent(env, number_of_epochs)
    # agent = DealerAgent(env, number_of_epochs)
    agent = TDAgent(env, number_of_epochs)
    # agent = SarsaAgent(env, number_of_epochs)
    agent.train()

    # in evaluate.py are some ideas that you might want to use to evaluate the agent
    # feel free to modify the code as you want to
    evaluate(env.get_episode_rewards())

    agent.print_utility()

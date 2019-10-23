import gym
from gym import spaces
from gym import Space
from gym.utils import seeding
from carddeck import *
import copy

class BlackjackEnv(gym.Env):
    '''
    our implementation of environment for blackjack game env.
    check superclass implementation in
    https://github.com/openai/gym/blob/master/gym/core.py

    you are not allowed to write to instances of this object
    or read values that you should not (for example what is
    the next card that will be drawn)

    However the variables are still visible to your code for
    purposes of evaluation and testing.
    '''

    metadata = {'render.modes': ['human']}

    def __init__(self):
        self.action_space = spaces.Discrete(2)
        self.seed()
        self.reset()
        self._owns_render = False

    def step(self, action):
        '''
        Does one step. At the end of the game you are responsible yourself
        for calling env.reset().

        Accepts action 1 (draw a card) or 0 (stick and let dealer move)

        Returns 4 values:
          observation (instance of BlackjackObservation)
          reward (1.0 for win, -1.0 for loose, 0.0 throughout the game or for a tie)
          done (boolean, True for reaching terminal state)
          info (empty dictionary, ignore in this setting)

        For more details visit:
        https://github.com/openai/gym/blob/master/gym/core.py
        '''
        assert self.action_space.contains(action)

        if action == 1:
            self.player_hand.draw_card(self.deck)
            if self.player_hand.is_bust():
                return self._get_observation(), -1, True, {}
            else:
                return self._get_observation(), 0, False, {}
        else:
            # now play the dealer
            while self.dealer_hand.value() < 17:
                self.dealer_hand.draw_card(self.deck)
            player_value = self.player_hand.value()
            dealer_value = self.dealer_hand.value()
            if player_value > dealer_value or self.dealer_hand.is_bust():
                return self._get_observation(), 1, True, {}
            if player_value < dealer_value:
                return self._get_observation(), -1, True, {}
            return self._get_observation(), 0, True, {}


    def reset(self):
        '''
        Starts a new game. A new card deck is created, it is shuffled,
        player gets two cards and dealer has one visible.

        This method returns observation in BlackjackObservation object.
        '''
        self.deck = CardDeck()
        self.deck.shuffle(self.np_random)
        #print(self.deck.cards)
        self.player_hand = BlackjackHand()
        self.dealer_hand = BlackjackHand()
        self.player_hand.draw_card(self.deck)
        self.player_hand.draw_card(self.deck)
        self.dealer_hand.draw_card(self.deck) # drawing 1 is equivalent to have 1 hidden
        #self._render()
        return self._get_observation()

    def render(self, mode='human', close=False):
        '''
        Prints the situation to the terminal. Call env.render() any time
        you need to see the game. This method is useful for debugging.
        '''
        print("player: "+str(self.player_hand))
        print("dealer: "+str(self.dealer_hand))

    def _get_observation(self):
        return BlackjackObservation(copy.deepcopy(self.player_hand), copy.deepcopy(self.dealer_hand))

    def seed(self, seed=None):
        self.np_random, seed = seeding.np_random(seed)
        return [seed]

class BlackjackObservation(Space):
    '''
    Observation that is given to you after each step
    you are not allowed to write to this object
    '''
    def __init__(self, player_hand, dealer_hand):
        self.player_hand = player_hand
        self.dealer_hand = dealer_hand

    def __repr__(self):
        return "Blackjack(player: "+str(self.player_hand)+", dealer: "+str(self.dealer_hand)+")"

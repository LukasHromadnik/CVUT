# in this file you can find a model for card, card deck and player hand

from enum import Enum
import numpy as np

class Suit(Enum):
    # for Czechs: krizova
    CLUB = 1
    # karova
    DIAMOND = 2
    # srdcova
    HEART = 3
    # pikova
    SPADES = 4

class Rank(Enum):
    # eso
    ACE = 1
    TWO = 2
    THREE = 3
    FOUR = 4
    FIVE = 5
    SIX = 6
    SEVEN = 7
    EIGHT = 8
    NINE = 9
    TEN = 10
    # spodek
    JACK = 11
    QUEEN = 12
    KING = 13

class Card:
    '''
    Card object. Each card has suit and rank. For example spades ten.
    '''
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank

    def value(self):
        '''
        Gets value of the card. Ace is counted 1, face cards 10.
        '''
        if self.rank.value > 10:
            return 10
        return self.rank.value

    def __repr__(self):
        return self.suit.name + ' ' + self.rank.name

class CardDeck:
    '''
    A standard card deck with 52 cards.
    There are all possible suit and rank combinations.
    '''
    def __init__(self):
        self.cards = []
        for suit in Suit:
            for rank in Rank:
                self.cards.append(Card(suit, rank))

    def shuffle(self, np_random=np.random):
        '''
        Shuffles the deck.
        '''
        np_random.shuffle(self.cards)

    def draw_card(self):
        '''
        Removes a single card from the deck and retuns it.
        '''
        return self.cards.pop()

class BlackjackHand:
    '''
    Player's hand. Follows the blackjack setting.
    '''
    def __init__(self):
        self.cards = []

    def draw_card(self, deck):
        '''
        Takes one card from the card deck.
        '''
        self.cards.append(deck.draw_card())

    def value(self):
        '''
        Gets the value of the hand. Value is a sum of values of all cards
        in the deck. If there is an ace, it can be counted 1 or 11. 11 is
        used only if the final sum does not exceed 21.
        '''
        ace = False
        value = 0
        for card in self.cards:
            if card.rank is Rank.ACE:
                ace = True
            value += card.value()

        if ace and value + 10 <= 21:
            value = value + 10
        return value

    def is_bust(self):
        '''
        Hand is bust if the value is more than 21. This is equivallent to
        loosing a blackjack game.
        '''
        return self.value() > 21

    def __str__(self):
        return str(self.cards) + " ("+str(self.value())+")"

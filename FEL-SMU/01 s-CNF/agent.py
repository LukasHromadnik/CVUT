import numpy as np
from itertools import combinations
from tutorial3 import Disjunction
from math import factorial

def comb(n, k):
    return factorial(n) / factorial(k) / factorial(n - k)

class Agent:
    def __init__(self, epsilon, delta):
        self.epsilon = epsilon 
        self.delta = delta

    def setup(self):
        self.hypothesis = []
        for i in range(1, self.s + 1):
            for c in list(combinations(range(self.n_variables * 2), i)):
                dis = np.zeros(self.n_variables * 2, dtype=bool)
                for j in range(len(c)):
                    dis[c[j]] = True
                positive = dis[:int(len(dis) / 2)]
                negative = dis[int(len(dis) / 2):]
                disjunction = Disjunction(positive, negative)
                if ~disjunction.is_tautology():
                    self.hypothesis.append(disjunction)


    def compute_required_training_dataset_size(self):
        all_size = np.sum([(2 ** i) * comb(self.n_variables, i) for i in range(1, self.s + 1)])
        return int(np.ceil(1 / self.epsilon) * np.log(all_size / self.delta))

    def process_first_observation(self, interpretation):
        self.setup()
        
        self.last_predication = False
        self.last_interpretation = interpretation
        return self.last_predication

    def predict(self, interpretation, reward):
        if (reward is not None) and (reward == 0) and (self.last_predication == False):
            for d in self.hypothesis.copy():
                if ~d.evaluate(self.last_interpretation):
                    self.hypothesis.remove(d)

        self.last_predication = True
        self.last_interpretation = interpretation
        for d in self.hypothesis:
            if ~d.evaluate(interpretation):
                self.last_predication = False
                break
        
        return self.last_predication

    def interact_with_oracle(self, oracle_session):
        self.n_variables, self.s = oracle_session.request_parameters()

        m = self.compute_required_training_dataset_size()
        first_sample = oracle_session.request_dataset(m)
        prediction = self.process_first_observation(first_sample)

        while oracle_session.has_more_samples():
            interpretation, reward = oracle_session.predict(prediction)
            prediction = self.predict(interpretation, reward)

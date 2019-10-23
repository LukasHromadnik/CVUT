from cnf import CNF
import numpy as np

class OracleSession:
    __TEST_SIZE = 1000
    def __init__(self):
        self.n_variables = np.random.randint(4, 6)
        self.j = np.random.randint(2, 4)

        self.__cnf = CNF.random(self.n_variables, self.j)
        self.__test_reward = 0

    def request_parameters(self):
        return (self.n_variables, self.j)

    def get_requested_dataset_size(self):
        return self.__m
    
    def request_dataset(self, m):
        if  m <= 0:
            raise ValueError("Requested dataset size must be positive.")
        if not isinstance(m, int):
            raise TypeError("Requested dataset size must be an integer.")

        self.__m = m
        self.__interpretations, self.__evaluations = \
                self.__cnf.generate_training_dataset(m + self.__TEST_SIZE)

        self.__i = 0
        return self.__interpretations[self.__i]

    def has_more_samples(self):
        return (self.__i + 1) < len(self.__evaluations)

    def predict(self, prediction):
        reward = int(prediction == self.__evaluations[self.__i])
        if self.__i > self.__m:
            self.__test_reward += reward 
            reward = None

        if self.has_more_samples():
            self.__i += 1
            sample = self.__interpretations[self.__i]
        else:
            raise StopIteration()

        return sample, reward

    def was_successful(self, epsilon):
        assert not self.has_more_samples(), "Session has not finished!"
        acc = self.__test_reward / self.__TEST_SIZE
        threshold = 1.0 - epsilon
        print("Acc = %.2f, Threshold = %.2f" % (acc, threshold))
        return acc >= threshold

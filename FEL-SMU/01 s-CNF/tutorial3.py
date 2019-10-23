import numpy as np
import operator

class AssociativeBinaryBooleanOperation:
    """ Common functionality for conjunctions and disjunctions. """
    def __init__(self, positive_literals, negative_literals):
        """ Initialize from arrays of literals.

        :param positive_literals: Indicator vector od positive literals.
        :type negative_literals: Iterable[bool]
        :param negative_literals: Indicator vector od negative literals.
        :type negative_literals: Iterable[bool]
        """

        if len(positive_literals) != len(negative_literals):
            raise ValueError("positive_literals and negative_literals vectors must have the same length!")

        self.positive_literals = positive_literals
        self.negative_literals = negative_literals
        self.n_variables = len(positive_literals)
        self.labels = ["var"+str(i) for i in range(self.n_variables)]
   
    @classmethod
    def random(cls, n_variables, q = None, s = None):
        """
        Randomly initialize a formula.
        There won't be a positive and negative literal for the same variable.

        :param n_variables: Number of propositionl variables
        :type n_variables: int
        :param q: There will be Bin(n_variables, q) literals
        :type q: float
        """

        if s is None:
            s = n_variables
        if q is None:
            q = 1 / np.log2(s + 1)
        n_literals = np.random.binomial(s, q)

        literal_indices = np.random.choice(
                np.arange(n_variables),
                n_literals,
                replace=False)


        positive_literals = np.zeros(n_variables, dtype=bool)
        negative_literals = np.zeros(n_variables, dtype=bool)

        negative = np.random.choice((False, True), n_literals)
        positive_literals[literal_indices[~negative]] = True
        negative_literals[literal_indices[negative]] = True

        return cls(positive_literals, negative_literals)

    def evaluate(self, interpretations):
        """
        Evaluate if the formula is True or False w.r.t. the given interpretation.
        
        :param interpretation: A propositional interpretation
        :type interpretation: np.array[bool]
        :return: The interpreted value of the conjunction.
        :rtype: bool
        """

        return self.fold((
                self.fold(~interpretations[self.negative_literals]),
                self.fold( interpretations[self.positive_literals])),
                axis=0)

    def generate_training_dataset(self, n_samples):
        """
        Generate random interpretations and their evaluations for supervised learning of the concept.

        :param n_samples: The requested size of the dataset.
        :type n_samples: int
        :return: Generated interpretations and the appropriate labels in sklearn-style.
        :rtype: 2-tuple: (matrix of interpretations, vector of classification labels)
        """

        interpretations = np.random.choice((False, True), (n_samples, self.n_variables))
        evaluations = self.evaluate(interpretations)

        return interpretations, evaluations

    def __repr__(self):
        p = [int(x) for x in self.positive_literals]
        n = [int(x) for x in self.negative_literals]
        return "%s(\n  %s,\n  %s)" % (self.__class__.__name__, p, n)

    def __str__(self):
        """ Human readable string representation. """
        return self.operator_str.join(
                sign + self.labels[i]
                for i, variable
                in enumerate(zip(self.positive_literals, self.negative_literals))
                for sign, literal
                in zip(('', '~'), variable)
                if literal)

    def __eq__(self, other):
        return type(self) is type(other) and \
                np.all(self.positive_literals == other.positive_literals) and \
                np.all(self.negative_literals == other.negative_literals)

    def __hash__(self):
        return int(np.packbits((self.positive_literals, self.negative_literals))[0])

class Disjunction(AssociativeBinaryBooleanOperation):
    fold = staticmethod(np.any)
    operator_str = " | "

    def is_satisfiable(self):
        return np.any((self.positive_literals, self.negative_literals))

    def is_tautology(self):
        return any(map(operator.and_, self.positive_literals, self.negative_literals))

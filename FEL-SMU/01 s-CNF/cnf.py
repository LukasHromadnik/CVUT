from tutorial3 import Disjunction
import numpy as np

class CNF:
    def __init__(self, n_variables, s, clauses):
        self.clauses = clauses
        self.n_variables = n_variables
        self.s = s

    def resolve(self, clause):
        if not clause.is_satisfiable(): return False

        resolved = self.clauses.copy()
        to_resolve = {clause}

        while to_resolve:
            clause = to_resolve.pop()
            if clause in resolved: continue
    
            for other in resolved:
                for i, (a, b) in enumerate(zip(clause.positive_literals, other.negative_literals)):
                    if a and b:
                        resolvent = Disjunction(
                                clause.positive_literals | other.positive_literals,
                                clause.negative_literals | other.negative_literals,
                                )
                        resolvent.positive_literals[i] = resolvent.negative_literals[i] = False
                        if not resolvent.is_satisfiable():
                            return False
                        if not resolvent.is_tautology():
                            to_resolve.add(resolvent)
            resolved.add(clause)

        return True

    def evaluate(self, interpretations):
        interpreted_clauses = np.array([
                    np.any( interpretations[:, clause.positive_literals], axis=1) | \
                    np.any(~interpretations[:, clause.negative_literals], axis=1) 
                for clause in self.clauses],
                dtype=bool).reshape(len(self.clauses), len(interpretations))
        return np.all(interpreted_clauses, axis = 0)

    @classmethod
    def random(cls, n_variables, s):
        q = s / n_variables ** s
        n_clauses = 1 + np.random.binomial(n_variables ** s - 1, q)
        cnf = cls(n_variables, s, set())

        for _ in range(n_clauses):
            clause = Disjunction.random(n_variables, s=s)
            if cnf.resolve(clause):
                cnf.clauses.add(clause)

        return cnf
        
    def generate_training_dataset(self, n_samples):
        interpretations = np.random.choice((False, True), (n_samples, self.n_variables))
        # print(self, interpretations)
        evaluations = self.evaluate(interpretations)

        return interpretations, evaluations

    def __str__(self):
        return " & ".join(
                "(%s)" % x
                for x 
                in sorted(
                    self.clauses,
                    key=lambda c: (tuple(~c.positive_literals), tuple(~c.negative_literals))))


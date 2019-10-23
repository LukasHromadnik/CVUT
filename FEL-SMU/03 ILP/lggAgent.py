from logic import *
from dataset import Sample
from bridge import *

class LGGResolver:
    ''' 
    Your task is to implement the first-order generalization agent based on LGG algorithm here, as discussed in the 
    lecture (see 4.2 Generalization of Clauses in SMU textbook version of year 201/).

    The class represents an generalization agent of first-order clauses based on the LGG algorithm. He initially starts
    with no hypothesis at all. Each time he gets an observation (in form of Sample, consisting of a class and a clause, 
    by calling his method, seeObservation(Sample)), he should change his hypothesis accordingly; i.e. in the case that
    prediction of the observation by the agent's hypothesis differs from the sample class. Recall that the agent predict
    the observation as positive iff the observed clause is theta subsumed by the agent hypothesis. Also recall that we 
    assume that there is no noise in the data. 
    
    One can obtain current hypothesis of the agent (Clause) by calling getHypothesis().

    Your first task is to implement the agent/LGG algorithm here in the method seeObservation(Sample). 
    Your second task is to implement lgg with the clause reduction step, which is called by seeObservation(Sample,reduceClause=True).
    Your third task is to implement lgg with taxonomical extension. Taxonomical information about constants are given to 
    the agent by the class constructor, e.g. LGGResolver(taxonomical=info) where info is a set of literals of a form 
     'isa(dog,mammal)'. It is ensured that from this set a forest can be formed, i.e. set of rooted oriented trees. 
    '''

    def __init__(self, taxonomical: Set[Literal] = None):
        '''
        Constructs new LGGResolver.
        
        Parameter taxonomical contains set of literals describing taxonomical information about the domain. It either
        may be None, i.e. no taxonomy provided, or it consists of literal of pairs isa/2 hierarchy, e.g. isa(car, vehicle).
        It is always ensured that literals in the set describes a forest, i.e. set of rooted oriented trees.
        
        :type taxonomical : Set of Literal
        :rtype: LGGResolver
        '''
        self.hypothesis: Clause = None
        self.taxonomical = taxonomical
        
        # if taxonomical != None:
        #     for item in taxonomical:
                # print(item)

    def getHypothesis(self) -> Clause:
        '''
        Returns current hypothesis of the agent.
        
        :rtype: Clause 
        '''
        return self.hypothesis

    def seeObservation(self, sample: Sample, reduceClause: bool = False) -> None:
        '''
        Performs LGG with the current hypothesis stored in the agent iff the the sample has positive label but the agent does predict the opposite class for the sample given.
        
        If reduction is set to True, then the agent process also the reduction step. You do not have to implement the 
        whole functionality, i.e. subsumption engine. To test whether one clause subsumes another one, 
        e.g. \alpha \subseq_{\theta} \beta, use library method subsume from package logic, e.g. subsume(\alpha,\beta).   

        :type sample: Sample
        :type reduceClause : bool
        :rtype: None 
        '''

        self.variableCounter = 0
        self.mapping = {}
        # print(sample.data, sample.positiveClass)

        if self.hypothesis == None and sample.positiveClass:
            literals = []
            for literal in sample.data:
                literals.append(literal)
            self.hypothesis = Clause(literals)
        elif sample.positiveClass:
            newLiterals = []
            for literal in sample.data:
                for hypotesisLiteral in self.hypothesis:
                    if literal.atom.predicate.name == hypotesisLiteral.atom.predicate.name:
                        newLiteral = self.lgg(literal, hypotesisLiteral)
                        newLiterals.append(newLiteral)
        
            self.hypothesis = Clause(newLiterals)

        if reduceClause:
            self.reduce()

        pass

    def lgg(self, left: Literal, right: Literal) -> Literal:
        terms = []
        
        for i in range(len(left)):
            leftTerm = left.atom.terms[i]
            rightTerm = right.atom.terms[i]

            if not leftTerm in self.mapping:
                self.mapping[leftTerm] = {}

            if not rightTerm in self.mapping[leftTerm]:
                self.mapping[leftTerm][rightTerm] = None
            
            if self.mapping[leftTerm][rightTerm] == None:
                if type(leftTerm) == type(rightTerm) and isinstance(leftTerm, Constant):
                    if str(leftTerm) == str(rightTerm):
                        self.mapping[leftTerm][rightTerm] = leftTerm
                    elif self.taxonomical != None:
                        leftTax = [leftTerm]
                        rightTax = [rightTerm]

                        currentLeft = leftTerm
                        currentRight = rightTerm
                        
                        leftFound = True
                        rightFound = True
                        while leftFound or rightFound:
                            leftFound = False
                            rightFound = False
                            for tax in self.taxonomical:
                                term = tax.atom.terms[0]
                                if currentLeft == term:
                                    currentLeft = tax.atom.terms[1]
                                    leftTax.append(currentLeft)
                                    leftFound = True
                                if currentRight == term:
                                    currentRight = tax.atom.terms[1]
                                    rightTax.append(currentRight)
                                    rightFound = True

                        found = False
                        for leftItem in leftTax:
                            for rightItem in rightTax:
                                if leftItem == rightItem and not found:
                                    self.mapping[leftTerm][rightTerm] = leftItem
                                    found = True
                        if not found:
                            self.mapping[leftTerm][rightTerm] = self.createNewVariable()
                    else:
                        self.mapping[leftTerm][rightTerm] = self.createNewVariable()
                else:
                    self.mapping[leftTerm][rightTerm] = self.createNewVariable()
            
            terms.append(self.mapping[leftTerm][rightTerm])

        predicate = left.atom.predicate
        atom = Atom(predicate, terms)
        return Literal(atom, left.positive)
    
    def createNewVariable(self) -> Variable:
        var = Variable("X" + str(self.variableCounter))
        self.variableCounter += 1

        return var

    def reduce(self):
        newHypo = self.hypothesis

        for i in range(len(self.hypothesis)):
            literal = self.hypothesis.literals[i]
            literals = []
            for j in range(len(newHypo)):
                if literal == newHypo.literals[j]: continue
                literals.append(self.hypothesis.literals[j])
            clause = Clause(literals)

            if subsume(self.hypothesis, clause):
                newHypo = clause
        
        self.hypothesis = newHypo
        
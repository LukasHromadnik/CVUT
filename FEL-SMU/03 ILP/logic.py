import os

os.environ['CLASSPATH'] = os.sep.join([".", "smu.jar"])

import itertools
from typing import Set, Iterable, List, Tuple, Dict, Iterator
from jnius import autoclass
import types
from tools import toBytes, fromBytes, unionSets

'''
This file contains various support for first order predicate (FOL) logic.

Note that all of the symbols, except of variable, should start with a lower-case letter. Only variable can start with upper-case letter.
Also note that symbols (names of variables, constants, functors, compound terms, predicates) should not contain bracket.
'''

JTerm = autoclass('ida.ilp.logic.Term')
JConstant = autoclass('ida.ilp.logic.Constant')
JVariable = autoclass('ida.ilp.logic.Variable')
JFunction = autoclass('ida.ilp.logic.Function')
JLiteral = autoclass('ida.ilp.logic.Literal')
JClause = autoclass('ida.ilp.logic.Clause')
JMED = autoclass('logicStuff.learning.datasets.MEDataset')
JMatching = autoclass('ida.ilp.logic.subsumption.Matching')
JSMU = autoclass("ida.courses.ilp.SMU")
Sugar = autoclass('ida.utils.Sugar')
JList = autoclass('java.util.List')
JSystem = autoclass("java.lang.System")


class Term:
    '''
    An interface for FOL term.
    '''

    def getVariables(self) -> Set['Variable']:
        '''
        Returns set of the term's variables.

        :rtype: set of Variable
        '''
        raise NotImplementedError("Method not implemented.")

    def getConstants(self) -> Set['Constant']:
        '''
        Returns set of constants of the term.

        :rtype: set of Constant
        '''
        raise NotImplementedError("Method not implemented.")

    def substitute(self, substitution) -> 'Term':
        '''
        Returns the same term but substituted by the substitution represented by dict of (Variable,Term).

        :type substitution: dict of (Variable,Term)
        :rtype: Term
        '''
        raise NotImplementedError("Method not implemented.")

    def getFunctors(self) -> Set['Functor']:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        raise NotImplementedError("Method not implemented.")


def toStr(term: 'Term or Atom') -> str:
    return fromBytes(JSMU.name(term))


class Variable(Term):
    '''
    Represents FOL variable.
    '''

    def __init__(self, name: str):
        self.var: JVariable = name if isinstance(name, JVariable) else JVariable.construct(toBytes(str(name)))

    def __hash__(self):
        return hash(str(self))

    def __str__(self):
        return toStr(self.var)

    def __eq__(self, o: object) -> bool:
        return isinstance(o, self.__class__) and str(self) == str(o)

    def __ne__(self, o: object) -> bool:
        return not self.__eq__(o)

    def getVariables(self) -> Set['Variable']:
        '''
        Returns set of the term's variables.

        :rtype: set of Variable
        '''
        r = set()
        r.add(self)
        return r

    def substitute(self, substitution: Dict['Variable', Term]) -> Term:
        '''
        Returns the same term but substituted by the substitution represented by dict of (Variable,Term).

        :type substitution: dict of (Variable,Term)
        :rtype: Term
        '''
        if self in substitution:
            return substitution[self]
        return self

    def getConstants(self) -> Set['Constant']:
        '''
        Returns set of constants of the term.

        :rtype: set of Constant
        '''
        return set()

    def getFunctors(self) -> Set['Functor']:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        return set()


class Constant(Term):
    '''
    Represents FOL constant.
    '''

    def __init__(self, name: str):
        self.const: JConstant = name if isinstance(name, JConstant) else JConstant.construct(toBytes(str(name)))

    def __str__(self):
        return toStr(self.const)

    def __eq__(self, other):
        return isinstance(other, self.__class__) and str(self) == str(other)

    def __ne__(self, other):
        return not self == other

    def __hash__(self):
        return hash(str(self))

    def getVariables(self) -> Set[Variable]:
        '''
        Returns set of the term's variables.

        :rtype: set of Variable
        '''
        return set()

    def substitute(self, substitution: Dict[Variable, Term]) -> Term:
        '''
        Returns the same term but substituted by the substitution represented by dict of (Variable,Term).

        :type substitution: dict of (Variable,Term)
        :rtype: Term
        '''
        return self

    def getConstants(self) -> Set['Constant']:
        '''
        Returns set of constants of the term.

        :rtype: set of Constant
        '''
        r = set()
        r.add(self)
        return r

    def getFunctors(self) -> Set['Functor']:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        return set()


class Functor:
    '''
    Represents FOL functor.
    '''

    def __init__(self, name: str, arity: int):
        self.name: str = name
        self.arity: int = arity

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.name == other.name and self.arity == other.arity

    def __ne__(self, other):
        return not self == other

    def __hash__(self):
        return (self.name, self.arity).__hash__()

    def __str__(self):
        return "{}/{}".format(self.name, self.arity)


class CompoundTerm(Term):
    '''
    Use .functor to get Functor of this composed term.
    Use .terms to get tuple of terms.
    '''

    def __init__(self, functor: Functor, terms: Iterable[Term]):
        '''
        Creates and returns a new compound term which is constructed by a functor applied to list of arguments.

        :type functor: Functor
        :type terms: iterable of Term   
        :rtype: CompoundTerm
        '''
        terms = tuple(terms)
        if functor.arity != len(terms):
            raise ValueError(
                "functor's arity '{}' is different than arguments given '{}'".format(functor,
                                                                                     ', '.join(map(str, terms))))
        self.func: JFunction = JFunction.parseFunction(
            toBytes("{}({})".format(functor.name, ', '.join(map(str, terms)))))
        self.functor: Functor = functor
        self.terms: Tuple[Term] = terms

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.func.equals(other)

    def __ne__(self, other):
        return not self == other

    def __hash__(self):
        return self.func.hashCode()

    def __str__(self):
        return self.func.toString()

    def __iter__(self) -> Iterator[Term]:
        return iter(self.terms)

    def __len__(self):
        return len(self.terms)

    def getVariables(self) -> Set[Variable]:
        '''
        Returns set of the term's variables.

        :rtype: set of Variable
        '''
        return unionSets(term.getVariables() for term in self.terms)

    def substitute(self, substitution: Dict[Variable, Term]) -> Term:
        '''
        Returns the same term but substituted by the substitution represented by dict of (Variable,Term).

        :type substitution: dict of (Variable,Term)
        :rtype: Term
        '''
        return CompoundTerm(self.functor, [term.substitute(substitution) for term in self.terms])

    def getConstants(self) -> Set[Constant]:
        '''
        Returns set of constants of the term.

        :rtype: set of Constant
        '''
        return set(term.getConstants() for term in self.terms)

    def getFunctors(self) -> Set[Functor]:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        result = set()
        result.add(self.functor)
        result = result.union(unionSets(map(lambda term: term.getFunctors(), self.terms)))
        return result


class Predicate:
    def __init__(self, name: str, arity: int):
        '''
        Creates and returns a new predicate by given name and arity. Length of the name should be longer than zero.

        :param name: str
        :param arity: int,>=0
        :rtype: Predicate
        '''
        self.name: str = name
        self.arity: int = arity

    def __eq__(self, other):
        return isinstance(other, self.__class__) and self.name == other.name and self.arity == other.arity

    def __ne__(self, other):
        return not self == other

    def __hash__(self):
        return (self.name, self.arity).__hash__()

    def __str__(self):
        '''
        Get Prolog-like notation.

        :rtype: str
        '''
        return "{}/{}".format(self.name, self.arity)


class Atom:
    def __init__(self, predicate: Predicate, terms: Iterable[Term]):
        '''
        Creates new atom given predicate and list of terms.

        :type predicate: Predicate
        :type terms: list of Term
        :rtype: Atom
        '''
        terms = tuple(terms)
        if isinstance(predicate, Predicate) and predicate.arity != len(terms):
            raise ValueError(
                "predicate's '{}' arity differs from the arguments given '{}'".format(predicate,
                                                                                      ', '.join(map(str, terms))))
        self.predicate: Predicate = predicate if isinstance(predicate, Predicate) else Predicate(predicate, len(terms))
        self.terms: Tuple[Term] = terms
        self.arity: int = predicate.arity

    def __str__(self):
        if 1 > self.arity:
            return self.predicate.name
        return "{}({})".format(self.predicate.name, ", ".join(map(str, self.terms)))

    # just lexical comparison
    def __eq__(self, other):
        return isinstance(other, Atom) and str(self) == str(other)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(str(self))

    def __iter__(self) -> Iterator[Term]:
        return iter(self.terms)

    def __len__(self):
        return len(self.terms)

    def getVariables(self) -> Set[Variable]:
        '''
        Returns set of variables in the atom.

        :rtype: set of Variable
        '''
        return unionSets(term.getVariables() for term in self.terms)

    def substitute(self, substitution: Dict[Variable, Term]) -> Term:
        '''
        Returns new atom obtained by applying the substitution on this atom.

        :type substitution: dict of (Variable,Term)
        :rtype: Atom
        '''
        return Atom(self.predicate, [term.substitute(substitution) for term in self.terms])

    def isGround(self) -> bool:
        '''
        Returns true iff the atom is ground.

        :rtype: bool
        '''
        return len(self.getVariables()) < 1

    def getConstants(self) -> Set[Constant]:
        '''
        Returns set of constants of the atom.

        :rtype: set of Constant
        '''
        return unionSets(term.getConstants() for term in self.terms)

    def getFunctors(self) -> Set[Functor]:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        return unionSets(map(lambda term: term.getFunctors(), self.terms))


class Literal:
    def __init__(self, atom: Atom, positive: bool = True):
        '''
        Creates and returns a new literal from the atom. The literal is negation of the atom if positive is set to False.

        :type atom: Atom
        :type positive: bool
        :rtype: Literal
        '''
        self.lit: JLiteral = JLiteral(toBytes(atom.predicate.name), not positive, toJava(atom.terms))
        self.atom: Atom = atom
        self.positive: bool = positive

    def __str__(self):
        return "{}{}".format("" if self.positive else "!", str(self.atom))

    # just lexical comparison
    def __eq__(self, other):
        return isinstance(other, self.__class__) and str(self) == str(other)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(str(self))

    def __iter__(self) -> Iterator[Atom]:
        return iter(self.atom)

    def __len__(self):
        return len(self.atom)

    def getPredicate(self) -> Predicate:
        '''
        Return the predicate.

        :rtype: Predicate
        '''
        return self.atom.predicate

    def getVariables(self) -> Set[Variable]:
        '''
        Returns variables in the literal.

        :rtype: set of Variable
        '''
        return self.atom.getVariables()

    def substitute(self, substitution: Dict[Variable, Term]) -> 'Literal':
        '''
        Returns new literal obtained by applying the substitution on itself.

        :type substitution: dict of (Variable,Term)
        :rtype: Literal
        '''
        return Literal(self.atom.substitute(substitution), positive=self.positive)

    def isGround(self) -> bool:
        '''
        Returns true iff the atom is ground.

        :rtype: bool
        '''
        return self.atom.isGround()

    def getConstants(self) -> Set[Constant]:
        '''
        Returns set of constants of the literal.

        :rtype: set of Constant
        '''
        return self.atom.getConstants()

    def getFunctors(self) -> Set[Functor]:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        return self.atom.getFunctors()


class Clause(Iterable[Literal]):
    def __init__(self, literals: Iterable[Literal]):
        '''
        Creates a clause given the list of literals.

        :type literals: iterable of Literal
        :rtype: Clause
        '''
        tup = tuple(literals)
        self.claus: JClause = JClause(toJava(tup))
        self.literals: Tuple[Term] = tup

    def __str__(self, endingDot=True):
        '''
        Set endingDot to False if you want to obtain string description of the clause without the ending dot.

        :type endingDot: bool
        :rtype: str
        '''
        if not self.literals:
            return "{{}}{}".format("." if endingDot else "")
        return '{}{}'.format(
            ' | '.join(sorted(map(str, self.literals))),
            "." if endingDot else "")

    # just lexical comparison
    def __eq__(self, other):
        return isinstance(other, self.__class__) and str(self) == str(other)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(str(self))

    def __iter__(self) -> Iterator[Literal]:
        return iter(self.literals)

    def __len__(self):
        return len(self.literals)

    def getPredicates(self) -> Set[Predicate]:
        '''
        Returns set of predicates in the clause.

        :rtype: set of Predicate
        '''
        return set(literal.getPredicate() for literal in self.literals)

    def getVariables(self) -> Set[Variable]:
        '''
        Returns set of variables in the clause.

        :rtype: set of Variable
        '''
        if not self.literals:
            return set()
        return unionSets(literal.getVariables() for literal in self.literals)

    def extend(self, literal) -> 'Clause':
        '''
        Creates and returns a new clause by appending the given literal to the set of literals of this clause.

        :type literal: Literal
        :rtype: Clause
        '''
        literals = set(literal for literal in self.literals)
        literals.add(literal)
        return Clause(literals)

    def getCanonicString(self) -> str:
        '''
        Returns canonical string for the clause.

        :rtype: str
        '''
        variables = list(self.getVariables())
        count = len(variables)
        if count < 1:
            return str(self)
        minimal = None

        for permutation in itertools.permutations(list(map(lambda x: Constant("x" + str(x)), range(count))), count):
            substitution = {variables[idx]: permutation[idx] for idx in range(count)}
            substituted = str(self.substitute(substitution))
            if minimal is None or substituted < minimal:
                minimal = substituted

        return minimal

    def substitute(self, substitution: Dict[Variable, Term]) -> Term:
        '''
        Returns new clause by applying the substitution on itself.

        :type substitution: dict of (Variable,Term)
        :rtype: Clause
        '''
        return Clause(literal.substitute(substitution) for literal in self.literals)

    def getNumberOfLiterals(self) -> int:
        return len(self.literals)

    def getNegativeLiterals(self) -> Set[Literal]:
        '''
        Returns set of negative literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.literals if not literal.positive)

    def getNegativeLiteralsAsClause(self) -> 'Clause':
        '''
        Returns subset of literals, only the negative ones, in a new clause.

        :rtype: Clause
        '''
        return Clause(self.getNegativeLiterals())

    def getPositiveLiterals(self) -> Set[Literal]:
        '''
        Returns set of positive literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.literals if literal.positive)

    def isGround(self) -> bool:
        '''
        Returns true iff the clause is ground.

        :rtype: bool
        '''
        return all(literal.isGround() for literal in self.literals)

    def getGroundLiterals(self) -> Set[Literal]:
        '''
        Returns set of ground literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.literals if literal.isGround())

    def getNonGroundLiterals(self) -> Set[Literal]:
        '''
        Returns set of non-ground literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.literals if not literal.isGround())

    def getNegativeNonGroundLiterals(self) -> Set[Literal]:
        '''
        Returns set of negative non-ground literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.getNonGroundLiterals() if not literal.positive)

    def getNonGroundAtoms(self) -> Set[Atom]:
        '''
        Returns set of non-ground atoms.

        :rtype: set of Atom
        '''
        return set(literal.atom for literal in self.getNonGroundLiterals())

    def getGroundAtoms(self) -> Set[Atom]:
        '''
        Returns set of ground atoms.

        :rtype: set of Atom
        '''
        return set(literal.atom for literal in self.getGroundLiterals())

    def replaceWithFreshVariables(self, variableFactory: 'VariableFactory') -> 'Clause':
        '''
        Returns a copy of the given clause containing fresh new variables.
    
        :type clause: Clause
        :type variableFactory: VariableFactory
        :rtype: Clause
        '''
        substitution = {}
        for variable in self.getVariables():
            substitution[variable] = variableFactory.create()
        return self.substitute(substitution)

    @staticmethod
    def parse(line: str) -> 'Clause':
        return toPython(JClause.parse(toBytes(line)))


class CNF:
    def __init__(self, clauses: Iterable[Clause]):
        '''
        Creates a new CNF given list of clauses.

        :type clauses: iterable of Clause
        :type variableFactory: VariableFactory
        :rtype: CNF
        '''
        self.clauses: Tuple[Clause] = tuple(clauses)

    def __str__(self):
        return ' & '.join('{{{}}}'.format(clause.__str__(endingDot=False)) for clause in self.clauses)

    # just lexical comparison
    def __eq__(self, other):
        return isinstance(other, CNF) and str(self) == str(other)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash(str(self))

    def __iter__(self) -> Iterator[Clause]:
        return iter(self.clauses)

    def __len__(self):
        return len(self.clauses)

    def getPredicates(self) -> Set[Predicate]:
        '''
        Returns set of predicates in the CNF.

        :rtype: set of Predicate
        '''
        return unionSets(clause.getPredicates() for clause in self.clauses)

    def copy(self) -> 'CNF':
        '''
        Returns copy of itself.

        :rtype: CNF
        '''
        return CNF([clause for clause in self.clauses])

    def getGroundLiterals(self) -> Set[Literal]:
        '''
        Returns set of ground literals.

        :rtype: set of Literal
        '''
        return unionSets(clause.getGroundLiterals() for clause in self.clauses)

    def getGroundAtoms(self) -> Set[Atom]:
        '''
        Returns set of ground atoms.

        :rtype: set of Atom
        '''
        return set(literal.atom for literal in self.getGroundLiterals())

    def getGroundNegativeLiteral(self) -> Set[Literal]:
        '''
        Returns set of ground negative literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.getGroundLiterals() if not literal.positive)

    def getGroundPositiveLiterals(self) -> Set[Literal]:
        '''
        Returns set of ground positive literals.

        :rtype: set of Literal
        '''
        return set(literal for literal in self.getGroundLiterals() if literal.positive)

    def isGround(self) -> bool:
        '''
        Returns True if the CNF is ground.

        :rtype: bool
        '''
        return all(clause.isGround() for clause in self.clauses)

    def getNegativeLiterals(self) -> Set[Literal]:
        '''
        Returns set of negative literals.

        :rtype: set of Literal
        '''
        return unionSets(clause.getNegativeLiterals() for clause in self.clauses)

    def getPositiveLiterals(self) -> Set[Literal]:
        '''
        Returns set of positive literals.

        :rtype: set of Literal
        '''
        return unionSets(clause.getPositiveLiterals() for clause in self.clauses)

    def getLiterals(self) -> Set[Literal]:
        '''
        Returns set of literals.

        :rtype: set of Literal
        '''
        return unionSets(clause.literals for clause in self.clauses)


class Interpretation:
    """
    An interpretation is a set of ground atoms.

    Use .literals to get ground atoms of the interpretation in a tuple.
    """

    def __init__(self, literals: Iterable[Literal]):
        '''
        Creates new interpretation given ground literals.

        :type literals: iterable of Literal
        :rtype: Interpretation
        '''
        lits = tuple(literals)
        assert all(literal.isGround() for literal in lits)
        assert all(literal.positive for literal in lits)
        self.atoms: Tuple[Atom] = tuple(literal.atom for literal in lits)

    def __iter__(self) -> Iterator[Atom]:
        return iter(self.atoms)

    def __len__(self):
        return len(self.atoms)

    def __str__(self):
        return "{{{}}}".format(", ".join(map(str, self.atoms)))

    def getPredicates(self) -> Set[Predicate]:
        '''
        Returns set of predicates of the interpretation.

        :rtype: set of Predicate
        '''
        return set(atom.predicate for atom in self.atoms)

    def isGround(self) -> bool:
        '''
        Returns true iff the interpretation is ground, which should be (just a check).

        :rtype: bool
        '''
        return all(atom.isGround() for atom in self.atoms)

    def getConstants(self) -> Set[Constant]:
        '''
        Returns set of constants.

        :rtype: set of Constant
        '''
        return unionSets(atom.getConstants() for atom in self.atoms)

    def getFunctors(self) -> Set[Functor]:
        '''
        Returns set of functors.

        :rtype: set of Functor
        '''
        return unionSets(map(lambda atom: atom.getFunctors(), self.atoms))


def toJava(term: 'PythonLogic') -> 'JavaLogic':
    if isinstance(term, JLiteral) or isinstance(term, JConstant) or isinstance(term, JVariable) or isinstance(term,
                                                                                                              JClause):
        return term
    if isinstance(term, Variable):
        return term.var
    elif isinstance(term, Constant):
        return term.const
    elif isinstance(term, CompoundTerm):
        return term.func
    elif isinstance(term, Literal):
        return term.lit
    elif isinstance(term, Clause):
        return term.claus
    elif isinstance(term, List) or isinstance(term, Tuple) or isinstance(term, types.GeneratorType):
        jList = Sugar.list()
        for t in term:
            jList.add(toJava(t))
        return jList
    elif isinstance(term, CNF):
        return toJava(c for c in term)
    raise ValueError("not implemented for '{}' of type '{}'".format(str(term), str(type(term))))


def toPython(java: 'JavaLogic') -> 'PythonLogic':
    if isinstance(java, Clause) or isinstance(java, Literal) or isinstance(java, Constant) \
            or isinstance(java, CompoundTerm) or isinstance(java, Variable) or isinstance(java, CNF):
        return java
    if isinstance(java, JList):
        return list(toPython(java.get(idx)) for idx in range(0, java.size()))
    if isinstance(java, JClause):
        literals = java.literals().toArray()
        return Clause(toPython(literals[idx]) for idx in range(0, java.literals().size()))
    if isinstance(java, JVariable):
        return Variable(java)
    if isinstance(java, JConstant):
        return Constant(java)
    if isinstance(java, JLiteral):
        arguments = list(toPython(java.get(idx)) for idx in range(0, java.arity()))
        atom = Atom(Predicate(toStr(java), java.arity()), arguments)
        return Literal(atom, positive=not java.isNegated())
    if isinstance(java, JFunction):
        jterms = [toPython(java.get(idx)) for idx in range(0, java.arity())]
        return CompoundTerm(Functor(toStr(java), java.arity()), jterms)
    if isinstance(java, list) or isinstance(java, tuple) or isinstance(java, types.GeneratorType):
        return list(map(toPython, java))
    if JSMU.isCompoundTerm(java):
        return toPython(JSMU.toFunction(java))
    if isinstance(java, JTerm):
        if JSMU.isVariable(java):
            return Variable(toStr(java))
        if JSMU.isConstant(java):
            return Constant(toStr(java))
    raise ValueError("not implemented for '{}' of type '{}'".format(str(java), str(type(java))))


class VariableFactory:
    def __init__(self):
        '''
        Constructs a new variable factory.
        '''
        self.memory: List[Variable] = []

    def create(self) -> Variable:
        '''
        Returns a new variable.

        :rtype: Variable
        '''
        count = len(self.memory)
        variable = Variable("X{}".format(count))
        self.memory.append(variable)
        return variable

from logic import *
from typing import List, Set
from pandas import DataFrame
from tools import toBytes, fromBytes

StringTransfer = autoclass('ida.courses.ilp.StringTransfer')

'''
This file contains method for calling java writen ILP code, e.g. search, propositionalization, etc.
'''


def featureBFS(maxLiterals: int, maxVariables: int, minSupport: int, subsumption: str,
               pathToDataset: str, minutesMax: int) -> List[Clause]:
    JSMU.setProperty("ida.logicStuff.constraints.maxLiterals", toBytes("1"))
    JSMU.setProperty("ida.logicStuff.constraints.minSupport", toBytes("1"))
    JSMU.setProperty("ida.logicStuff.constraints.learner", toBytes("none"))
    JSMU.setProperty("ida.searchPruning.runner.overallLimit", toBytes(str(minutesMax * 60)))
    JSMU.setProperty("ida.searchPruning.mining", toBytes("bfs"))
    JSMU.setProperty("ida.searchPruning.minSupport", toBytes(str(minSupport)))
    JSMU.setProperty("ida.searchPruning.maxDepth", toBytes(str(maxLiterals)))
    JSMU.setProperty("ida.searchPruning.maxVariables", toBytes(str(maxVariables)))
    JSMU.setProperty("ida.searchPruning.storeOutput", toBytes(toBytes("none")))

    # TODO !!! System.setProperty("ida.searchPruning.modeDeclaration", "molecular");
    JSMU.setProperty("ida.searchPruning.datasetSubsumption", toBytes(toBytes(subsumption)))
    JSMU.setProperty("ida.searchPruning.input", toBytes(pathToDataset))

    return toPython(JSMU.bfsFeatureMining())


def propositionalize(pathToConjunctiveFeatures: str, pathToDataset: str, subsumption="theta") -> DataFrame:
    JSystem.setProperty("ida.searchPruning.propositialization.method", "existential")
    if subsumption.lower() == "theta":
        subsumptionMode = JMatching.THETA_SUBSUMPTION
    elif subsumption.lower() == "oi":
        subsumptionMode = JMatching.OI_SUBSUMPTION
    else:
        raise ValueError('unknown subsumption type:\t{}'.format(subsumption))
    grounded = JSMU.propositionalize(toBytes(pathToConjunctiveFeatures), toBytes(pathToDataset), subsumptionMode)

    with open(pathToConjunctiveFeatures, 'r') as input:
        features = list(
            c for c in list(Clause.parse(line) for line in input.readlines() if len(line.strip()) > 0) if len(c) > 0)

    features.append("class")
    df = DataFrame(columns=features)
    for sampleIdx in range(0, grounded.size()):
        sample = grounded.get(sampleIdx)
        df = df.append({features[idx]: sample.get(idx) for idx in range(0, len(features))}
                       , ignore_index=True)
    return df


def generateMostSpecificSTRangeRestrictedClauses(predicates: Set[Predicate], s: int, t: int, constants=[],
                                                 functors=[]) -> CNF:
    '''
    Returns the most specific FOL CNF given set of predicates, constants, functors and constrains s and t.

    st-clause is such a clause which has at most s literals, each of them is composed by at most t term and predicate symbols.

    Range-restricted clause is such a clause that all of its variables in positive literals occur in some negative literals, e.g. all head's variable occure in body in case of horn clause.

    So 

    :type predicates: set of Predicate
    :type s: int,>=0
    :type t: int,>=0
    :type constants: set of Constant
    :type functors: set of Functor
    :rtype: CNF
    '''
    predicates = list(map(str, predicates))
    functions = list(map(str, constants))
    functions.extend(list(map(str, functors)))

    if len(functors) > 0:
        raise ValueError('So far, functionality for st-range restricted generation with function is not supported.')

    jfunctions = StringTransfer.create()
    for f in functions:
        jfunctions.add(toBytes(toBytes(str(f))))

    jpredicates = StringTransfer.create()
    for p in predicates:
        jpredicates.add(toBytes(str(p)))

    clauses = JSMU.STRangeRestrictedClauses(s, t, jpredicates, jfunctions, s)
    pc = [toPython(clauses.get(idx)) for idx in range(0, clauses.size()) if clauses.get(idx).countLiterals() > 0]
    return CNF(pc)


def clauseToStr(clause: Clause) -> str:
    return fromBytes(JSMU.str(clause.claus))


def subsume(alpha: Clause, beta: Clause) -> bool:
    '''
    Returns true iff alpha theta-subsumes beta.
    
    :param alpha: Clause 
    :param beta: Clause
    :return: bool 
    '''
    if not isinstance(alpha, Clause) or not isinstance(beta, Clause):
        raise ValueError('The both given clauses must be of a type Clause!')
    return JSMU.subsumes(alpha.claus, beta.claus)

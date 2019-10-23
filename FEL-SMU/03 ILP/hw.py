import os
from dataset import ClauseDataset, parseClauses
from logic import Clause
from lggAgent import LGGResolver


def runAgent(pathToData: str, useReduction: bool, pathToTaxonomy: str) -> Clause:
    '''
    This method runs your implementation of LGGResolver on a set of clauses from the given file.
    
    :type pathToData: str 
    :rtype: Clause  
    '''
    data = ClauseDataset(pathToData)
    taxonomy = set(
        literal for c in parseClauses(pathToTaxonomy) for literal in c) if pathToTaxonomy is not None else None
    resolver = LGGResolver(taxonomy)

    for sample in data:
        # print('observe\n\t{}'.format(sample))
        resolver.seeObservation(sample, reduceClause=useReduction)

    print('final clause learned by LGG:', resolver.getHypothesis())
    return resolver.getHypothesis()


if __name__ == "__main__":
    pathToClausesDataset = os.sep.join([".", "data", "daughter"])
    taxoInfo = None

    # to try taxonomy use this dataset
    pathToClausesDataset = os.sep.join([".", "data", "animals"])
    taxoInfo = os.sep.join([".", "data", "animalsTaxonomy"])

    clause = runAgent(pathToClausesDataset, True, taxoInfo)

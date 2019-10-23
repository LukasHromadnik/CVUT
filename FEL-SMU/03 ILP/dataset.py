from logic import *
import tools


def parseClauses(pathToFile: str) -> List[Clause]:
    '''
    Returns list of clauses parsed from the file.
    
    :param pathToFile: str 
    :return: list of Clause
    '''
    jlist = JSMU.parseClauses(tools.toBytes(pathToFile))
    return [toPython(jlist.get(idx)) for idx in range(0, jlist.size())]


class Sample:
    '''
    Stores a sample which consists of a class label and a data point, e.g. interpretation, clause, CNF, etc.

    Class is either positive '+' or negative '-'.
    Delimiter ':' splits class and the interpretation which is ended by a dot.

    use .positiveClass to get label of the example, e.g. true iff class is +, false iff class is - 
    use .data to get the data point, e.g. an interpretation, clause, CNF, etc.
    '''

    def __init__(self, data, positiveClass: bool):
        self.data = data
        self.positiveClass: bool = positiveClass

    def __str__(self):
        return "{} : {}".format("+" if self.positiveClass else "-", self.data)

    def __iter__(self) -> Iterable:
        return iter(self.data)

    def __len__(self):
        return len(self.data)


class ClauseDataset:
    '''
    Storage of clauses; use .clauses to get list of clauses (:type: Clause).
    '''

    def __init__(self, pathToFile: str):
        '''
        Creates new dataset of clauses from path to the file. The clauses can be accessed by .clauses.

        :param pathToFile: str 
        '''
        med = JSMU.loadMED(tools.toBytes(pathToFile), JMatching.THETA_SUBSUMPTION)

        targets = med.getTargets()
        examples = med.getExamples().toArray()

        self.med: JMED = med
        self.samples: List[Sample] = tuple(Sample(toPython(examples[idx]), targets[idx] > 0.5) for
                                           idx in range(0, med.size()))

    def __iter__(self) -> Iterable[Clause]:
        return iter(self.samples)

    def __len__(self):
        return len(self.samples)


class InterpretationDataset:
    '''
    Storage of samples. Use .samples to get list of samples (:type: Sample).
    '''

    def __init__(self, path: str):
        med = JSMU.loadMED(tools.toBytes(path), JMatching.THETA_SUBSUMPTION)

        targets = med.getTargets()
        examples = med.getExamples().toArray()

        self.med: JMED = med
        self.samples: List[Sample] = [Sample(Interpretation(toPython(examples[idx]).literals), targets[idx] > 0.5) for
                                      idx in
                                      range(0, med.size())]

    def __iter__(self) -> Iterable[Sample]:
        return iter(self.samples)

    def __len__(self):
        return len(self.samples)

    def getPredicates(self) -> Set[Predicate]:
        '''
        Returns set of all predicates used in the dataset.

        :rtype: set of Predicate
        '''
        return tools.unionSets(map(lambda sample: sample.data.getPredicates(), self.samples))

    def getConstants(self) -> Set[Constant]:
        '''
        Returns all constants in the dataset.

        :rtype: set of Constant
        '''
        return tools.unionSets(map(lambda sample: sample.data.getConstants(), self.samples))

    def getFunctors(self) -> Set[Functor]:
        '''
        Returns all functors in the dataset.
        
        :rtype: set of Functor 
        '''
        return tools.unionSets(map(lambda sample: sample.data.getFunctors(), self.samples))

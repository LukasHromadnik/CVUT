import functools
import array

from typing import Set, Generic, Iterable


def unionSets(iterableOfSets: Iterable[Set['Generic']]) -> Set['Generic']:
    '''
    Taking iterable of set, this method makes one big set by union of all the set given

    :type iterableOfSets: iterable of set of T
    :rtype: set of T
    '''
    iterableOfSets = tuple(iterableOfSets)
    if len(iterableOfSets) == 1:
        # special case, trailing comma at the end of the tuple makes the problem
        return iterableOfSets[0]
    try:
        if len(iterableOfSets) < 0:
            return set()
        return functools.reduce(lambda x, y: x.union(y), iterableOfSets)
    except Exception as e:
        raise ValueError('unionSets: {}\n{}'.format(iterableOfSets, e))


def toBytes(val: str) -> bytearray:
    return val.encode('utf-8')


def fromBytes(val: bytearray) -> str:
    return array.array('b', val).tostring().decode('utf-8')

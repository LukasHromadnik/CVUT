class Coord:

    def __init__(self, coord, parent, distance, heuristic):
        self.coord = coord
        self.parent = parent
        self.distance = distance
        self.heuristic = heuristic

    def value(self):
        return self.distance + self.heuristic
    

    def __lt__(self, other):
        return self.value() < other.value()

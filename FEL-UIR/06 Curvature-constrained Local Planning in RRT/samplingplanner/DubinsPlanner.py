import dubins as dubins
import numpy as np

class DubinsPlanner:

    def __init__(self, turning_radius=0.5, step_size=0.1):
        self.turning_radius = turning_radius
        self.step_size = step_size

    def from_dubins(self, item):
        return np.array([item[0], item[1], 0, 0, 0, -item[2]])

    def to_dubins(self, item):
        return [item[0], item[1], -item[5]]
    
    def compute_path_between(self, start, goal):
        q0 = self.to_dubins(start)
        q1 = self.to_dubins(goal)

        path = dubins.shortest_path(q0, q1, self.turning_radius)
        configurations, _ = path.sample_many(self.step_size)
        length = path.path_length()
        configurations = list(map(lambda x: self.from_dubins(x), configurations))
        
        return (configurations, length)

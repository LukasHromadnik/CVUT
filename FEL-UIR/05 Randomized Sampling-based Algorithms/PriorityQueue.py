import heapq

class PriorityQueue:
    def __init__(self):
        self.elements = []
    
    def empty(self):
        return len(self.elements) == 0
    
    def push(self, item, priority):
        heapq.heappush(self.elements, (priority, item))
    
    def pop(self):
        return heapq.heappop(self.elements)
    
    def top(self):
        u = self.elements[0]
        return u[1]
    
    def contains(self, element):
        for item in self.elements:
            if element == item[1]:
                return True

        return False
    
    def remove(self, element):
        i = 0
        for item in self.elements:
            if element == item[1]:
                self.elements[i] = self.elements[-1]
                self.elements.pop()
                heapq.heapify(self.elements)
                break
            i += 1
    
    def copy(self):
        queue = PriorityQueue()
        queue.elements = self.elements.copy()
        return queue
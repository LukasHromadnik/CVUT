using namespace std;

class CrossbreedingStrategy {
public:
	virtual Individual * crossbreed(Individual *parent1, Individual *parent2) = 0;
};

// ----------------------------------------------

class SinglePointCrossbreedingStrategy: public CrossbreedingStrategy {
public:
	Individual * crossbreed(Individual *parent1, Individual *parent2);
};

Individual * SinglePointCrossbreedingStrategy::crossbreed(Individual *parent1, Individual *parent2) {
	int individualSize = parent1->size();
	int randomPosition = rand() % individualSize;
	
	Individual *child1 = new Individual(parent1->m_Problem);
	Individual *child2 = new Individual(parent1->m_Problem);

	for (int i = 0; i < individualSize; i++) {
		Individual *firstChildParent = (i < randomPosition) ? parent1 : parent2;
		Individual *secondChildParent = (i < randomPosition) ? parent2 : parent1;
		
		child1->m_Configuration.push_back(firstChildParent->m_Configuration[i]);
		child2->m_Configuration.push_back(secondChildParent->m_Configuration[i]);
	}

	return (child1 > child2) ? child1 : child2;
}

// ----------------------------------------------

class DoublePointCrossbreedingStrategy: public CrossbreedingStrategy {
public:
	Individual * crossbreed(Individual *parent1, Individual *parent2);
};

Individual * DoublePointCrossbreedingStrategy::crossbreed(Individual *parent1, Individual *parent2) {
	int individualSize = parent1->size();
	int randomPosition1 = rand() % individualSize;
	int randomPosition2 = rand() % individualSize;
	int index1 = min(randomPosition1, randomPosition2);
	int index2 = max(randomPosition1, randomPosition2);
	
	Individual *child1 = new Individual(parent1->m_Problem);
	Individual *child2 = new Individual(parent1->m_Problem);

	for (int i = 0; i < individualSize; i++) {
		Individual *firstChildParent = (i < index1 || i > index2) ? parent1 : parent2;
		Individual *secondChildParent = (i < index1 || i > index2) ? parent2 : parent1;
		
		child1->m_Configuration.push_back(firstChildParent->m_Configuration[i]);
		child2->m_Configuration.push_back(secondChildParent->m_Configuration[i]);
	}

	return (child1 > child2) ? child1 : child2;
}

// ----------------------------------------------

class UniformCrossbreedingStrategy: public CrossbreedingStrategy {
public:
	Individual * crossbreed(Individual *parent1, Individual *parent2);
};

Individual * UniformCrossbreedingStrategy::crossbreed(Individual *parent1, Individual *parent2) {	
	Individual *child1 = new Individual(parent1->m_Problem);
	Individual *child2 = new Individual(parent1->m_Problem);

	for (int i = 0; i < parent1->size(); i++) {
		int uniformValue = rand() % 2;
		Individual *firstChildParent = (uniformValue == 1) ? parent1 : parent2;
		Individual *secondChildParent = (uniformValue == 1) ? parent2 : parent1;
		
		child1->m_Configuration.push_back(firstChildParent->m_Configuration[i]);
		child2->m_Configuration.push_back(secondChildParent->m_Configuration[i]);
	}

	return (child1 > child2) ? child1 : child2;
}

// ----------------------------------------------

class Crossbreeding {
private:
	CrossbreedingStrategy *m_Strategy;
	Individual *m_Parent1;
	Individual *m_Parent2;

public:
	Crossbreeding(CrossbreedingStrategy *strategy, Individual *parent1, Individual *parent2);
	Individual * crossbreed();
};

Crossbreeding::Crossbreeding(CrossbreedingStrategy *strategy, Individual *parent1, Individual *parent2) {
	m_Strategy = strategy;
	m_Parent1 = parent1;
	m_Parent2 = parent2;
}

Individual * Crossbreeding::crossbreed() {
	return m_Strategy->crossbreed(m_Parent1, m_Parent2);
}

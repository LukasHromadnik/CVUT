using namespace std;

class ReductionStrategy {
public:
	virtual int childrenCount(Population *population) = 0;
	virtual void reduce(Population *population, int populationCount) = 0;
	virtual void reduceAfterCrossbreeding(Population *population, int populationCount) = 0;
};

// ----------------------------------------------

class PartialReductionStrategy: public ReductionStrategy {
private:
	int m_ChildrenCount;
public:
	PartialReductionStrategy(int childrenCount);
	void reduce(Population *population, int populationCount);
	void reduceAfterCrossbreeding(Population *population, int populationCount);
	int childrenCount(Population *population);
};

PartialReductionStrategy::PartialReductionStrategy(int childrenCount) {
	m_ChildrenCount = childrenCount;
}

int PartialReductionStrategy::childrenCount(Population *populationCount) {
	return m_ChildrenCount;
}

void PartialReductionStrategy::reduce(Population *population, int populationCount) {
	for (Individual *child : population->m_Children) {
		population->m_Items.push_back(child);
	}
	population->m_Children = vector<Individual *>();

	while (population->size() > populationCount) {
		int minIndex = -1;
		int minValue = INT_MAX;
		for (int i = 0; i < population->size(); i++) {
			if (population->m_Items[i]->value() < minValue) {
				minValue = population->m_Items[i]->value();
				minIndex = i;
			}
		}
		population->eraseAtIndex(minIndex);
	}
}

void PartialReductionStrategy::reduceAfterCrossbreeding(Population *population, int populationCount) { }

// ----------------------------------------------

class SubstitutionReductionStrategy: public ReductionStrategy {
public:
	int childrenCount(Population *population);
	void reduce(Population *population, int populationCount);
	void reduceAfterCrossbreeding(Population *population, int populationCount);
};

int SubstitutionReductionStrategy::childrenCount(Population *population) {
	return population->size();
}

void SubstitutionReductionStrategy::reduce(Population *population, int populationCount) {
	population->m_Items = population->m_Children;
	population->m_Children = vector<Individual *>();
}

void SubstitutionReductionStrategy::reduceAfterCrossbreeding(Population *population, int populationCount) { }

// ----------------------------------------------

class StableReductionStrategy: public ReductionStrategy {
private:
	int m_ChildrenCount;
public:
	StableReductionStrategy(int childrenCount);
	int childrenCount(Population *population);
	void reduce(Population *population, int populationCount);
	void reduceAfterCrossbreeding(Population *population, int populationCount);
};

StableReductionStrategy::StableReductionStrategy(int childrenCount) {
	m_ChildrenCount = childrenCount;
}

int StableReductionStrategy::childrenCount(Population *population) {
	return m_ChildrenCount;
}

void StableReductionStrategy::reduce(Population *population, int populationCount) { }

void StableReductionStrategy::reduceAfterCrossbreeding(Population *population, int populationCount) {
	int minIndex = -1;
	int minValue = INT_MAX;

	for (int i = 0; i < population->size(); i++) {
		if (population->m_Items[i]->value() < minValue) {
			minValue = population->m_Items[i]->value();
			minIndex = i;
		}
	}
	
	population->eraseAtIndex(minIndex);
	
	population->m_Items.push_back(population->m_Children[0]);
	population->m_Children = vector<Individual *>();
}

// ----------------------------------------------

class Reduction {
private:
	ReductionStrategy *m_Strategy;
	Population *m_Population;
	int m_PopulationCount;
public:
	Reduction(ReductionStrategy *strategy, int populationCount, Population *population);
	void reduce();
	int childrenCount();
	void reduceAfterCrossbreeding();
};

Reduction::Reduction(ReductionStrategy *strategy, int populationCount, Population *population) {
	m_Strategy = strategy;
	m_Population = population;
	m_PopulationCount = populationCount;
}

void Reduction::reduce() {
	return m_Strategy->reduce(m_Population, m_PopulationCount);
}

int Reduction::childrenCount() {
	return m_Strategy->childrenCount(m_Population);
}

void Reduction::reduceAfterCrossbreeding() {
	return m_Strategy->reduceAfterCrossbreeding(m_Population, m_PopulationCount);
}

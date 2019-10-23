using namespace std;

class SelectionStrategy {
public:
	virtual Individual * select(Population *population) = 0;
};

// ----------------------------------------------

class RouletteSelectionStrategy: public SelectionStrategy {
public:
	Individual * select(Population *population);
};

Individual * RouletteSelectionStrategy::select(Population *population) {
	int randomValue = (rand() % population->sumOfValues()) + 1;
	int sum = 0;
	Individual *selected = NULL;

	for (Individual *item : population->m_Items) {
		sum += item->value();
		if (sum >= randomValue) {
			selected = item;
			break;
		}
	}

	return selected;
}

// ----------------------------------------------

class TournamentSelectionStrategy: public SelectionStrategy {
private:
	int m_Size;
public:
	TournamentSelectionStrategy(int size);
	Individual * select(Population *population);
};

TournamentSelectionStrategy::TournamentSelectionStrategy(int size) {
	m_Size = size;
}

Individual * TournamentSelectionStrategy::select(Population *population) {
	vector<int> indexes;

	while (indexes.size() != m_Size) {
		int randomIndex = rand() % population->size();

		if (find(indexes.begin(), indexes.end(), randomIndex) == indexes.end()) {
			indexes.push_back(randomIndex);
		}
	}

	int maxValue = INT_MIN;
	Individual *selected = NULL;

	for (int index : indexes) {
		Individual *item = population->m_Items[index];

		if (item->value() > maxValue) {
			selected = item;
		}
	}

	return selected;
}

// ----------------------------------------------

class Selection {
private:
	SelectionStrategy *m_Strategy;
	Population *m_Population;

public:
	Selection(SelectionStrategy *strategy, Population *population);
	Individual * select();
};

Selection::Selection(SelectionStrategy *strategy, Population *population) {
	m_Strategy = strategy;
	m_Population = population;
}

Individual * Selection::select() {
	return m_Strategy->select(m_Population);
}


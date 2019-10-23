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
	int randomValue = rand() % population->sumOfValues();
	int sum = 0;
	Individual *selected = NULL;

	// Iterate over items
	for (Individual *item : population->m_Items) {

		// Add item's value to the sum
		sum += item->value();

		// If the index is smaller than the current sum, it's the selected item
		if (randomValue < sum) {
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

	// Generate array of random index
	while ((int) indexes.size() != m_Size) {
		int randomIndex = rand() % population->size();

		// If the index is already in set, skip it
		if (find(indexes.begin(), indexes.end(), randomIndex) == indexes.end()) {
			indexes.push_back(randomIndex);
		}
	}

	int maxValue = INT_MIN;
	Individual *selected = NULL;

	// Iterate over indexes
	for (int index : indexes) {
		Individual *item = population->m_Items[index];

		// Find the item with the highest fitness
		if (item->value() > maxValue) {
			selected = item;
			maxValue = item->value();
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

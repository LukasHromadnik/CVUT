using namespace std;

struct Population {
	vector<Individual *> m_Items;
	vector<Individual *> m_Children;

	Population();
	Population(int count, Problem *problem);

	void debug() const;
	int sumOfValues() const;
	int size() const;
	void addChild(Individual *item);
	int value() const;
	void eraseAtIndex(int index);
	Individual * bestIndividual() const;
};

Population::Population() {	}

Population::Population(int count, Problem *problem) {
	for (int i = 0; i < count; i++) {
		Individual *item = new Individual(problem, true);
		m_Items.push_back(item);
	}
}

void Population::debug() const {
	// for (Individual *item : m_Items) {
		// item->debug();
		// cout << "-----" << endl;
	// }
	// cout << "=====" << endl;
	cout << value() << endl;
	// cout << "=====" << endl;
}

int Population::sumOfValues() const {
	int sum = 0;
	for (Individual *item : m_Items) {
		sum += item->value();
	}

	return sum;
}

int Population::size() const {
	return m_Items.size();
}

void Population::addChild(Individual *item) {
	m_Children.push_back(item);
}

int Population::value() const {
	return bestIndividual()->value();
}

Individual * Population::bestIndividual() const {
	Individual *best = NULL;
	int max = -1;

	for (Individual *item : m_Items) {
		if (item->value() > max) {
			max = item->value();
			best = item;
		}
	}

	return best;
}

void Population::eraseAtIndex(int index) {
	m_Items.erase(m_Items.begin() + index);
}

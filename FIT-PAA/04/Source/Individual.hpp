using namespace std;

struct Individual {
	vector<int> m_Configuration;
	Knapsack *m_Knapsack;

	Individual(Knapsack *knapsack);
	Individual(int size, Knapsack *knapsack);
	void debug() const;
	int value() const;
	int size() const;
};

Individual::Individual(Knapsack *knapsack) {
	m_Knapsack = knapsack;
}

Individual::Individual(int size, Knapsack *knapsack) {
	for (int i = 0; i < size; i++) {
		m_Configuration.push_back(rand() % 2);
	}
	m_Knapsack = knapsack;
}

void Individual::debug() const {
	for (int i = 0; i < size(); i++) {
		cout << m_Configuration[i] << " ";
	}
	cout << "| " << value();
	cout << endl;
}

int Individual::value() const {
	int sumPrice = 0;
	int sumWeight = 0;
	for (int i = 0; i < size(); i++) {
		sumPrice += (m_Knapsack->m_Items[i]->m_Price * m_Configuration[i]);
		sumWeight += (m_Knapsack->m_Items[i]->m_Volume * m_Configuration[i]);
	}

	return (sumWeight > m_Knapsack->m_Capacity) ? 0 : sumPrice;
}

int Individual::size() const {
	return m_Configuration.size();
}

// ----------------------------------------------

bool operator <(Individual const& left, Individual const& right) {
	return left.value() < right.value();
}

bool operator >(Individual const& left, Individual const& right) {
	return left.value() > right.value();
}

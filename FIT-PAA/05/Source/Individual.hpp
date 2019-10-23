using namespace std;

struct Individual {
	vector<int> m_Configuration;
	Problem *m_Problem;

	Individual(Problem *problem, bool generate);
	void debug() const;
	int value() const;
	int size() const;
};

Individual::Individual(Problem *problem, bool generate = false) {
	if (generate) {
		for (int i = 0; i < problem->configurationSize(); i++) {
			m_Configuration.push_back(rand() % 2);
		}
	}
	m_Problem = problem;
}

void Individual::debug() const {
	for (int i = 0; i < size(); i++) {
		cout << m_Configuration[i] << " ";
	}
	cout << "| " << value();
	cout << endl;
	m_Problem->debug(m_Configuration);
}

int Individual::value() const {
	return m_Problem->value(m_Configuration);
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

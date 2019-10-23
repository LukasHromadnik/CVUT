using namespace std;

class Problem {
private:
	vector<Clause *> m_Items;
	vector<int> m_Weights;
	int m_ConfigurationSize;
public:
	void setConfigurationSize(int configurationSize);
	void setItems(vector<Clause *> items);
	void debug(vector<int> evaluation);
	bool isSatisfiable(vector<int> evaluation) const;
	int value(vector<int> evaluation) const;
	int configurationSize() const;
	void setWeights(vector<int> weights);
};

void Problem::setConfigurationSize(int configurationSize) {
	m_ConfigurationSize = configurationSize;
}

void Problem::setItems(vector<Clause *> items) {
	m_Items = items;
}

void Problem::debug(vector<int> evaluation) {
	// for (int i = 0; i < m_Items.size(); i++) {
	// 	Clause *item = m_Items[i];
	// 	item->debug(evaluation);
	// }
	cout << "Satisfiable: " << isSatisfiable(evaluation) << endl;
}

bool Problem::isSatisfiable(vector<int> evaluation) const {
	for (Clause *item : m_Items) {
		if ( ! item->isSatisfiable(evaluation)) return false;
	}

	return true;
}

int Problem::value(vector<int> evaluation) const {
	int sum = 1;

	// if ( ! isSatisfiable(evaluation)) return sum;

	for (int i = 0; i < (int) evaluation.size(); i++) {
		sum += m_Weights[i] * evaluation[i];
	}

	return (isSatisfiable(evaluation)) ? sum : 1;

	// return (sum == 0) ? 1 : sum * 10;
	// return sum;
}

int Problem::configurationSize() const {
	return m_ConfigurationSize;
}

void Problem::setWeights(vector<int> weights) {
	m_Weights = weights;
}

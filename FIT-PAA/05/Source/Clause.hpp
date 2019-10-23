using namespace std;

class Clause {
private:
	vector<int> m_Configuration;

public:
	Clause(vector<int> configuration, int numberOfVariables);
	bool isSatisfiable(vector<int> evaluation) const;
	void debug(vector<int> evaluation);
};

Clause::Clause(vector<int> configuration, int numberOfVariables) {
	m_Configuration = configuration;
}

bool Clause::isSatisfiable(vector<int> evaluation) const {
	for (int i = 0; i < (int) m_Configuration.size(); i++) {
		int indexIsNegative = m_Configuration[i] < 0;
		int index = abs(m_Configuration[i]) - 1;
		int eval = evaluation[index];

		if ((eval == 1 && !indexIsNegative) || (eval == 0 && indexIsNegative)) return true;
	}

	return false;
}

void Clause::debug(vector<int> evaluation) {
	bool sat = isSatisfiable(evaluation);
	cout << "(" << sat << ")";
	for (int i = 0; i < (int) m_Configuration.size(); i++) {
		cout << " " << setw(2) << m_Configuration[i];
	}
	cout << endl;
}

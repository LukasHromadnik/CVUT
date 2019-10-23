using namespace std;

class EvolutionSolution {
public:
	Population *m_Population;
	int m_NumberOfGenerations;

	EvolutionSolution(Population *population, int numberOfGenerations);
	void debug() const;
};

EvolutionSolution::EvolutionSolution(Population *population, int numberOfGenerations) {
	m_Population = population;
	m_NumberOfGenerations = numberOfGenerations;
}

void EvolutionSolution::debug() const {
	Individual *best = m_Population->bestIndividual();
	cout << "Value: " << m_Population->value() << endl;
	best->m_Problem->debug(best->m_Configuration);
	cout << "-----" << endl;
}
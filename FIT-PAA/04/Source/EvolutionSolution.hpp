using namespace std;

class EvolutionSolution {
public:
	Population *m_Population;
	int m_NumberOfGenerations;

	EvolutionSolution(Population *population, int numberOfGenerations);
};

EvolutionSolution::EvolutionSolution(Population *population, int numberOfGenerations) {
	m_Population = population;
	m_NumberOfGenerations = numberOfGenerations;
}

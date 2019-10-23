#include "Individual.hpp"
#include "Population.hpp"

#include "Selection.hpp"
#include "Crossbreeding.hpp"
#include "Mutation.hpp"
#include "EvolutionSolution.hpp"
#include "Reduction.hpp"

using namespace std;

class Evolution {
private:
	SelectionStrategy *m_SelectionStrategy;
	CrossbreedingStrategy *m_CrossbreedingStrategy;
	ReductionStrategy *m_ReductionStrategy;

	int m_PopulationCount;
	int m_MutationProbability;
	int m_MaxNumberOfGenerations;

public:
	Evolution(SelectionStrategy *selectionStrategy, CrossbreedingStrategy *crossbreedingStrategy, ReductionStrategy *reductionStrategy, int populationCount, int mutationProbability, int maxNumberOfGenerations);

	EvolutionSolution * solve(Problem *problem);
};

Evolution::Evolution(SelectionStrategy *selectionStrategy, CrossbreedingStrategy *crossbreedingStrategy, ReductionStrategy *reductionStrategy, int populationCount, int mutationProbability, int maxNumberOfGenerations) {
	m_SelectionStrategy = selectionStrategy;
	m_CrossbreedingStrategy = crossbreedingStrategy;
	m_ReductionStrategy = reductionStrategy;

	m_PopulationCount = populationCount;
	m_MutationProbability = mutationProbability;
	m_MaxNumberOfGenerations = maxNumberOfGenerations;
}

EvolutionSolution * Evolution::solve(Problem *problem) {
	// Initial population
	Population *population = new Population(m_PopulationCount, problem);

	// population->debug();

	// Bounds
	int numberOfGenerations = 0;

	// Reduction
	Reduction *reduction = new Reduction(m_ReductionStrategy, m_PopulationCount, population);

	// Genetic algorithm
	while (numberOfGenerations < m_MaxNumberOfGenerations) {

		// Generate new children
		for (int i = 0; i < reduction->childrenCount(); i++) {

			// Selection
			Selection *selection = new Selection(m_SelectionStrategy, population);
			Individual *parent1 = selection->select();
			Individual *parent2 = selection->select();

			// Crossbreeding
			Crossbreeding *crossbreeding = new Crossbreeding(m_CrossbreedingStrategy, parent1, parent2);
			Individual *child = crossbreeding->crossbreed();

			// Mutation
			Mutation *mutation = new Mutation(child, m_MutationProbability);
			child = mutation->mutate();

			// Add new child to the existing population
			population->addChild(child);

			reduction->reduceAfterCrossbreeding();
		}

		// Reduce population number
		reduction->reduce();

		// Counter
		numberOfGenerations++;

		// population->debug();
	}

	return new EvolutionSolution(population, numberOfGenerations);
}

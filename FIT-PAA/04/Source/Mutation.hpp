using namespace std;

class Mutation {
private:
	Individual *m_Item;
	int m_Probability;

public:
	Mutation(Individual *item, int probability);
	Individual * mutate();
};

Mutation::Mutation(Individual *item, int probability) {
	m_Item = item;
	m_Probability = probability;
}

Individual * Mutation::mutate() {
	int randomProbability = rand() % 100;
	
	if (randomProbability >= m_Probability) return m_Item;

	int randomIndex = rand() % m_Item->size();
	m_Item->m_Configuration[randomIndex] = !m_Item->m_Configuration[randomIndex];

	return m_Item;
}

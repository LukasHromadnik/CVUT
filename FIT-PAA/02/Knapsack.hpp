#include <vector>

using namespace std;

struct Knapsack {
	int m_Identifier;
	int m_Capacity;
	vector<KnapsackItem *> m_Items;

	Knapsack();
};

// ------------------------------------------------------------------------------------------------

Knapsack::Knapsack() {
	m_Identifier = 0;
	m_Capacity = 0;
	m_Items = vector<KnapsackItem *>();
}

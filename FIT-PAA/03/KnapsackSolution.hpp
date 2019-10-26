#include <vector>

using namespace std;

struct KnapsackSolution {
	int m_Price;
	vector<bool> m_Items;

	KnapsackSolution(int size);
};

// ------------------------------------------------------------------------------------------------

KnapsackSolution::KnapsackSolution(int size) {
	m_Price = 0;
	m_Items = vector<bool>(size, 0);
}

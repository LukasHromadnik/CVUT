#include <sys/time.h>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

void setupRandomNumberGenerator() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	srand(ms);

	// srand(time(NULL));
}

vector<Knapsack *> loadInstance(int size) {
	vector<Knapsack *> knapsacks;

	string fileName = "./input/knap_" + to_string(size) + ".txt";

	string line;
	ifstream inputFile (fileName);

	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {

			stringstream input(line);
			int tmp;
			vector<int> data;

			while (input >> tmp) {
				data.push_back(tmp);
			}	

			Knapsack *knapsack = new Knapsack();
			for (int i = 0; i < data.size(); i++) {
				if (i == 2) {
					knapsack->m_Identifier = data[0];
					knapsack->m_Capacity = data[2];
				} else if (i > 2 && i % 2 == 0) {
					KnapsackItem *item = new KnapsackItem();
					item->m_Price = data[i];
					item->m_Volume = data[i - 1];
					knapsack->m_Items.push_back(item);
				}
			}

			knapsacks.push_back(knapsack);
		}
	}

	return knapsacks;
}

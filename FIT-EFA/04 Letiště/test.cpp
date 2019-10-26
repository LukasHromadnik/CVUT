#include <iostream>
#include <math.h>

using namespace std;

int main(int argc, char const *argv[])
{
	int max = 10001;
	int * arr = new int [max];

	cout << 2000 << endl;
	for (int i = 0; i <= 2000; i++) {
		for (int j = 0; j < 2; j++) {
			int random = rand() % max;
			while (arr[random] > 1) {
				random = rand() % max;
			}
			arr[random] += 1;
			cout << random << " ";
		}
		cout << endl;
	}
	return 0;
}

#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#if defined (__cplusplus) && __cplusplus > 199711L
/* C++ 11 */
#include <thread>
#include <mutex>     
#include <condition_variable>
#endif /* __cplusplus */

#include <unistd.h>

using namespace std;

struct TRect
{
	int m_X;
	int m_Y;
	int m_W;
	int m_H;
};

struct TCostProblem
{
	int ** m_Values;
	int m_Size;
	int m_MaxCost;
	void (* m_Done) (const TCostProblem *, const TRect *);
};

struct TCrimeProblem
{
	double ** m_Values;
	int m_Size;
	double m_MaxCrime;
	void (* m_Done) (const TCrimeProblem *, const TRect *);
};

#endif /* __PROGTEST__ */

bool FindByCost(int ** values, int size, int maxCost, TRect * res)
{
	// Inicializace
	int **mat = new int*[size];
	for (int i = 0; i < size; i++) {
		mat[i] = new int[size];
	}

	// Inicializace TRect
	res->m_X = 0;
	res->m_Y = 0;
	res->m_W = 0;
	res->m_H = 0;

	int maxArea = 0;

	// Predpocitani matice
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {

			if (i == 0 && j == 0) {
				mat[j][i] = values[j][i];
			} else if (i == 0) {
				mat[j][i] = values[j][i] + mat[j - 1][i];
			} else if (j == 0) {
				mat[j][i] = values[j][i] + mat[j][i - 1];
			} else {
				mat[j][i] = mat[j - 1][i] + mat[j][i - 1] - mat[j - 1][i - 1] + values[j][i];
			}
		}
	}

	// Vypocet
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = i; k < size; k++) {
				for (int l = j; l < size; l++) {

					// Indexy pro pomocne prvky
					int mI = ((i - 1) > 0) ? i - 1 : 0;
					int mJ = ((j - 1) > 0) ? j - 1 : 0;
					
					// Hodnoty vsech promennych ve vypoctu
					double topLeft = mat[mI][mJ];
					double topRight = mat[mI][l];
					double bottomLeft = mat[k][mJ];
					double bottomRight = mat[k][l];

					// Osetreni os
					if (i == 0) {
						topRight = 0;
						topLeft = 0;
					}

					if (j == 0) {
						bottomLeft = 0;
						topLeft = 0;
					}

					double result = bottomRight - topRight - bottomLeft + topLeft;

					if (result > maxCost) {
						continue;
					}

					int height = k - i + 1;
					int width = l - j + 1;
					int area = width * height;

					if (area > maxArea) {
						res->m_X = j;
						res->m_Y = i;
						res->m_W = width;
						res->m_H = height;
						maxArea = area;
					}
				}
			}
		}
	}

	return (maxArea != 0);
}

bool FindByCrime(double ** values, int size, double maxCrime, TRect * res)
{
	// Inicializace pomocne matice
	int **mat = new int*[size];
	for (int i = 0; i < size; i++) {
		mat[i] = new int[size];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			mat[j][i] = (values[i][j] > maxCrime) ? 0 : (j == 0) ? 1 : mat[j - 1][i] + 1;
		}
	}

	// Init stacku
	stack<int> s;

	// Nejvetsi spocitana oblast
	int maxArea = 0;

	// Hodnota na topu stacku
	int top;

	// Oblast pro dany sloupec
	int currentMaxArea = 0;

	for (int rowIndex = 0; rowIndex < size; rowIndex++) {

		// Index
		int i = 0;

		// Nacitam postupne sloupce a pocitam oblasti pro sloupce vyrazene
		while (i < size) {
			// Pokud je stack prazdny nebo hodnota je mensi nez aktualni -> push
			if (s.empty() || mat[rowIndex][s.top()] < mat[rowIndex][i]) {
				s.push(i);
				i++;
			}
			// Pokud je hodnota mensi, provadim vypocet
			else {
				// Nactu vrchni hodnotu
				top = s.top();

				// Odstranim vrchni hodnotu
				s.pop();

				// Provedu vypocet
				// i - s.top() - 1 vypocita hodnotu "pusobnosti" pro pocitany sloupec
				int width = mat[rowIndex][top];
				int height = (s.empty()) ? i : i - s.top() - 1;
				currentMaxArea = width * height;

				// Aktualizuji nejvetsi hodnotu
				if (currentMaxArea > maxArea) {
					maxArea = currentMaxArea;
					res->m_Y = i - 1 - height + 1;
					res->m_X = rowIndex - width + 1;
					res->m_W = width;
					res->m_H = height;
				}
			}
		}

		// Vypocitam zbyle sloupce
		while (s.empty() != true) {
			// Nactu vrchni hodnotu
			top = s.top();

			// Odstranim vrchni hodnotu
			s.pop();

			// Provedu vypocet
			// i - s.top() - 1 vypocita hodnotu "pusobnosti" pro pocitany sloupec
			int width = mat[rowIndex][top];
			int height = (s.empty()) ? i : i - s.top() - 1;
			currentMaxArea = width * height;

			// Aktualizuji nejvetsi hodnotu
			if (currentMaxArea > maxArea) {
				maxArea = currentMaxArea;
				res->m_Y = i - 1 - height + 1;
				res->m_X = rowIndex - width + 1;
				res->m_W = width;
				res->m_H = height;
			}
		}
	}

	return (maxArea != 0);
}

struct SMainProblem
{
	string type;
	void *problem;
};

struct SNullProblem : SMainProblem
{
	SNullProblem() { type = "null"; };
};

struct SCostProblem : SMainProblem
{
	SCostProblem(const TCostProblem *p, string t) { problem = p; type = t; }
	const TCostProblem *problem;
};

struct SCrimeProblem : SMainProblem
{
	SCrimeProblem(const TCrimeProblem *p, string t) { problem = p; type = t; }
	const TCrimeProblem *problem;
};

queue<SMainProblem *> problems;
int g_ProblemsMax, g_ProblemsCount = 0, g_ReturnValue = 0;
pthread_cond_t g_Con, g_Prod;
pthread_mutex_t g_Counter, g_Return;

void loadProblem(const TCostProblem * (* costFunc) (void))
{
	const TCostProblem * costProblem;

	while (1) {
		costProblem = costFunc();

		if ( ! costProblem) {
			pthread_mutex_lock(&g_Return);
			g_ReturnValue++;
			pthread_mutex_unlock(&g_Return);
			return;
		}

		pthread_mutex_lock(&g_Counter);
		while ((int)problems.size() >= g_ProblemsMax) {
			pthread_cond_wait(&g_Prod, &g_Counter);
		}

		problems.push(new SCostProblem(costProblem, "cost"));
		pthread_mutex_unlock(&g_Counter);
		pthread_cond_signal(&g_Con);
	}
}

void loadProblem(const TCrimeProblem * (* crimeFunc) (void))
{
	const TCrimeProblem * crimeProblem;

	while (1) {
		crimeProblem = crimeFunc();

		if ( ! crimeProblem) {
			pthread_mutex_lock(&g_Return);
			g_ReturnValue++;
			pthread_mutex_unlock(&g_Return);
			break;
		}

		pthread_mutex_lock(&g_Counter);
		while ((int)problems.size() >= g_ProblemsMax) {
			pthread_cond_wait(&g_Prod, &g_Counter);
		}

		problems.push(new SCrimeProblem(crimeProblem, "crime"));
		pthread_mutex_unlock(&g_Counter);
		pthread_cond_signal(&g_Con);
	}
}

void solve()
{
	while (1) {
		SMainProblem *problem;

		pthread_mutex_lock(&g_Counter);
		while (problems.size() == 0) {
			pthread_cond_wait(&g_Con, &g_Counter);
		}

		problem = problems.front();
		problems.pop();

		pthread_mutex_unlock(&g_Counter);
		pthread_cond_signal(&g_Prod);

		TRect *res = new TRect();

		if (problem->type == "cost") {
			SCostProblem *costProblem = (SCostProblem *)problem;
			FindByCost(costProblem->problem->m_Values, costProblem->problem->m_Size, costProblem->problem->m_MaxCost, res);
			costProblem->problem->m_Done(costProblem->problem, res);
		} else if (problem->type == "crime") {
			SCrimeProblem *crimeProblem = (SCrimeProblem *)problem;
			FindByCrime(crimeProblem->problem->m_Values, crimeProblem->problem->m_Size, crimeProblem->problem->m_MaxCrime, res);
			crimeProblem->problem->m_Done(crimeProblem->problem, res);
		} else if (problem->type == "null") {
			return;
		}
	}
}

void MapAnalyzer(int threads, const TCostProblem * (* costFunc) (void), const TCrimeProblem* (* crimeFunc) (void))
{
	// Inicializace pthread srand
	pthread_cond_init(&g_Prod, NULL);
	pthread_cond_init(&g_Con, NULL);
	pthread_mutex_init(&g_Counter, NULL);
	pthread_mutex_init(&g_Return, NULL);

	// Pole s producenty
	vector<thread> producers;

	// Pole s konzumenty
	vector<thread> consumers;

	// Nastaveni velikosti fronty
	g_ProblemsMax = threads;

	// Ukazatele na "problemove" funkce
	void (*loadCost)(const TCostProblem * (*) (void));
	loadCost = &loadProblem;

	void (*loadCrime)(const TCrimeProblem * (*) (void));
	loadCrime = &loadProblem;

	// Vytvoreni producentu
	producers.push_back(thread(loadCost, costFunc));
	producers.push_back(thread(loadCrime, crimeFunc));

	for (int i = 0; i < threads; i++) {
		consumers.push_back(thread(solve));
	}

	// Cekani na ukonceni producentu
	for (auto &t : producers) {
		t.join();
	}

	// cout << "Prod end" << endl;

	for (int i = 0; i < threads; i++) {
		pthread_mutex_lock(&g_Counter);
		problems.push(new SNullProblem());
		pthread_mutex_unlock(&g_Counter);
		pthread_cond_signal(&g_Con);
	}

	// Cekani na ukonceni konzumentu
	for (auto &t : consumers) {
		// pthread_cond_signal(&g_Con);
		t.join();
	}

	// Zniceni pthread srand
	pthread_cond_destroy(&g_Con);
	pthread_cond_destroy(&g_Prod);
	pthread_mutex_destroy(&g_Counter);
	pthread_mutex_destroy(&g_Return);

	// cout << "Done" << endl << "-------" << endl;
}

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
#include <iostream>
#include <iomanip>
#include <set>
#include <math.h>
#include <vector>
#include <deque>
#include <queue>
#include <sstream>
#include <fstream>
#include <mpi.h>
#include <omp.h>
#include <chrono>

#define GRAPH_BIPARTITE 0
#define GRAPH_NOT_BIPARTITE 1

#define GRAPH_COMPLETE 0
#define GRAPH_NOT_COMPLETE 1

#define SOLUTION_FOUND 0
#define SOLUTION_NOT_FOUND 1

#define TAG_WORK 0
#define TAG_SOLUTION 1
#define TAG_END 2

#define USE_DEBUG 1

using namespace std;

typedef vector<int> Graph;

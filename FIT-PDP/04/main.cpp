#include "application.cpp"
#include <mpi.h>

Graph combine(Graph a, Graph b) {
	Graph result;
	result.reserve(a.size() + b.size());
	result.insert(result.end(), a.begin(), a.end());
	result.insert(result.end(), b.begin(), b.end());

	return result;
}

int main(int argc, char * argv[]) {

	// Declare variables
	int numberOfNodes, maxDepthProcesses, maxDepthThreads;

	// Loaded graph
	Graph graph;

	// Load input from a given file
	loadInput(argc, argv, numberOfNodes, graph, maxDepthProcesses, maxDepthThreads);

	// Number of items in the graph matrix
	int numberOfItems = pow(numberOfNodes, 2);

	// Number of items in the pipe
	int graphSize = numberOfItems + 2;

	// Size of the data for the stream
	int dataSize = graphSize * 2;

	// Prepare solution variable
	Graph solutionGraph(graphSize, 0);

	// Rank of the current process and total number of processes
	int processNumber, numberOfProcesses;

	MPI_Init(NULL, NULL);

	// Set the rank
	MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

	// Set the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

	if (processNumber == 0) {

		Timer runTimer("finish computation");

		Problem solution(solutionGraph, 0, 1);
		Problem rootProblem(graph, 0, 1);
		
		deque<Problem> problems = generateBFS(rootProblem, solution, maxDepthProcesses);

		if (solution.numberOfEdges() > 0) {
			for (int destination = 1; destination < numberOfProcesses; destination++) {
				Graph data = combine(solutionGraph, solutionGraph);
				MPI_Send(&data[0], dataSize, MPI_INT, destination, TAG_END, MPI_COMM_WORLD);
			}

			printSolution(solution);
			MPI_Finalize();
			return 0;
		}

		for (int destination = 1; destination < numberOfProcesses; destination++) {
			Problem problem = problems.front();
			problems.pop_front();
			Graph data = combine(problem.serialized(), solutionGraph);
			MPI_Send(&data[0], dataSize, MPI_INT, destination, TAG_WORK, MPI_COMM_WORLD);
		}

		int numberOfWorkingSlaves = numberOfProcesses - 1;

		while (numberOfWorkingSlaves > 0) {
			MPI_Status status;
			Graph temp(dataSize, 0);
			
			MPI_Recv(&temp[0], dataSize, MPI_INT, MPI_ANY_SOURCE, TAG_SOLUTION, MPI_COMM_WORLD, &status);

			Graph newSolutionGraph(temp.begin(), temp.begin() + dataSize / 2);
			Problem newSolution = Problem::deserialize(newSolutionGraph);

			if (newSolution.numberOfEdges() > solution.numberOfEdges()) {
				solution = newSolution;
				solutionGraph = newSolutionGraph;
			}

			if ( ! problems.empty()) {
				Problem problem = problems.front();
				problems.pop_front();
				Graph data = combine(problem.serialized(), solutionGraph);
				MPI_Send(&data[0], dataSize, MPI_INT, status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
			} else {
				MPI_Send(&temp[0], dataSize, MPI_INT, status.MPI_SOURCE, TAG_END, MPI_COMM_WORLD);
				numberOfWorkingSlaves--;
			}
		}

		printSolution(solution);
		runTimer.end();
	} else {
		while (true) {
			Graph temp(dataSize, 0);
			MPI_Status status;
			
			MPI_Recv(&temp[0], dataSize, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			if (status.MPI_TAG == TAG_END) break;
			else {
				Graph newProblemGraph(temp.begin(), temp.begin() + dataSize / 2);
				Graph newSolutionGraph(temp.begin() + dataSize / 2, temp.end());

				Problem newProblem = Problem::deserialize(newProblemGraph);
				Problem newSolution = Problem::deserialize(newSolutionGraph);

				int currentSolutionEdges = newSolution.numberOfEdges();

				deque<Problem> problems = generateBFS(newProblem, newSolution, maxDepthThreads);

				if (newSolution.numberOfEdges() == currentSolutionEdges) {
					#pragma omp parallel for
					for (int i = 0; i < (int) problems.size(); i++) {
						iteration03(problems[i], newSolution);
					}
				}

				Graph foundSolution = newSolution.serialized();
				Graph data = combine(foundSolution, foundSolution);

				MPI_Send(&data[0], dataSize, MPI_INT, 0, TAG_SOLUTION, MPI_COMM_WORLD);
			}
		}
	}

	// End MPI task
	MPI_Finalize();

	return 0;
}

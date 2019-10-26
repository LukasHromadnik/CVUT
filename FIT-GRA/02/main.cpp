#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <list>
#include <stdint.h>

using namespace std;

#endif /* __PROGTEST__ */

///////////////////////////////////////////////////////////////////////////////////////////////////

bool bipartiteMatching(int **graph, int numberOfRooms, int u, bool seen[], int match[]) {
	for (int v = 0; v < numberOfRooms; v++) {
		if (graph[u][v] && ! seen[v]) {
			seen[v] = true;

			if (match[v] < 0 || bipartiteMatching(graph, numberOfRooms, match[v], seen, match)) {
				match[v] = u;

				return true;
			}
		}
	}

	return false;
}

int maximumBipartiteMatching(int** graph, int numberOfClasses, int numberOfRooms) {
	int * match = new int[numberOfRooms];

	for (int i = 0; i < numberOfRooms; i++) {
		match[i] = -1;
	}

	int result = 0;
	for (int i = 0; i < numberOfClasses; i++) {
		bool * seen = new bool[numberOfRooms];
		for (int j = 0; j < numberOfRooms; j++) {
			seen[j] = false;
		}

		if (bipartiteMatching(graph, numberOfRooms, i, seen, match)) {
			result++;
		}

		delete[] seen;
	}

	delete[] match;

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

string loadInput(const char *inFile) {
	string line, text;
	ifstream in(inFile);

	while (getline(in, line)) {
		text += line + "\n";
	}

	return text;
}

void rozvrh(const char *inFile, const char *outFile) {
	string line, text;
	ifstream in(inFile);
	
	int numberOfClasses = 0;
	int numberOfRooms = 0;

	int lineNumber = 0;
	int lastIndex = 0;
	int ** graph = NULL;

	while (getline(in, line)) {
		if (lineNumber == 0) {
			for (int i = 0; i < (int)line.length(); i++) {
				if (line[i] == ' ') {
					numberOfClasses = stoi(line.substr(lastIndex, i - lastIndex));
					lastIndex = i + 1;
				} else if (i + 1 == (int)line.length()) {
					numberOfRooms = stoi(line.substr(lastIndex, i - lastIndex + 1));

					graph = new int * [numberOfClasses];
					for (int j = 0; j < numberOfClasses; j++) {
						graph[j] = new int[numberOfRooms];

						for (int k = 0; k < numberOfRooms; k++) {
							graph[j][k] = 0;
						}
					}

					lastIndex = 0;
				}
			}
		} else {
			lastIndex = 0;
			for (int i = 0; i < (int)line.length(); i++) {
				int roomIndex = -1;
				if (line[i] == ' ') {
					if (i == lastIndex) {
						lastIndex++;
						continue;
					}
					roomIndex = stoi(line.substr(lastIndex, i - lastIndex));
				} else if (i + 1 == (int)line.length()) {
					roomIndex = stoi(line.substr(lastIndex, i - lastIndex + 1));
				}

				if (roomIndex == -1) {
					continue;
				}

				graph[lineNumber - 1][roomIndex - 1] = 1;

				lastIndex = i + 1;
			}
		}

		lineNumber += 1;
	}

	int numberOfEdges = maximumBipartiteMatching(graph, numberOfClasses, numberOfRooms);

	ofstream out(outFile);
	out << numberOfEdges << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __PROGTEST__

int main(int argc, char * argv [])
{
	if (argc != 3) {
		cout << "usage: binary inFile outFile" << endl;
		return 1;
	}

	rozvrh(argv[1], argv[2]);

	return 0;
}

#endif /* __PROGTEST__ */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

struct Automaton {
    Automaton(int numberOfStates, int numberOfTransitions, int numberOfLetters);

    void addTransition(int fromState, int transition, int toState);
    vector<int> transitionsForStateAndTransition(int state, int transition);
    
    int numberOfStates;
    int numberOfTransitions;
    int numberOfLetters;
    vector<int> *transitions;
    vector<int> endingStates;
};

Automaton::Automaton(int numberOfStates, int numberOfTransitions, int numberOfLetters) {
    this->numberOfStates = numberOfStates;
    this->numberOfTransitions = numberOfTransitions;
    this->numberOfLetters = numberOfLetters;

    transitions = new vector<int>[numberOfStates * numberOfLetters];
}

void Automaton::addTransition(int fromState, int transition, int toState) {
    transitions[fromState * numberOfLetters + transition].push_back(toState);
}

vector<int> Automaton::transitionsForStateAndTransition(int state, int transition) {
    return transitions[state * numberOfLetters + transition];
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

bool isFinalState(int state, vector<int> endingStatesA, vector<int> endingStatesB, int numberOfStatesB) {
    for (int i = 0; i < (int) endingStatesA.size(); i++) {
        for (int j = 0; j < (int) endingStatesB.size(); j++) {
            int endingState = endingStatesA[i] * numberOfStatesB + endingStatesB[j];
            if (state == endingState) return true;
        }
    }

    return false;
}

int main(int argc, const char * argv[])
{   
    // Helpers for input
    string line, word;
    int lineNumber = 0;

    map<char, int> transitionToIndex;
    map<int, char> indexToTransition;
    string alphabet;
    int numberOfLetters = 0;

    Automaton *automatonA = NULL;
    Automaton *automatonB = NULL;

    // Iterate over all input lines
    while (getline(cin, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Parse input line
        vector<string> tokens;

        // Split input line by space and convert the split result to the integer value
        // Then push it to the `tokens` vector
        while (ss >> word) {
            tokens.push_back(word);
        }

        if (lineNumber == 0) {
            // Automaton alphabet
            alphabet = tokens[0];
            numberOfLetters = (int) alphabet.size();
            for (int i = 0; i < numberOfLetters; i++) {
                transitionToIndex.insert(pair<char, int>(alphabet[i], i));
                indexToTransition.insert(pair<int, char>(i, alphabet[i]));
            }
        } else if (lineNumber == 1) {
            int numberOfStates = stoi(tokens[0]);
            int numberOfTransitions = stoi(tokens[1]);

            automatonA = new Automaton(numberOfStates, numberOfTransitions, numberOfLetters);
        } else if (lineNumber > 1 && lineNumber < automatonA->numberOfTransitions + 2) {
            int fromState = stoi(tokens[0]);
            int toState = stoi(tokens[2]);
            char transition = tokens[1][0];
            automatonA->addTransition(fromState, transitionToIndex[transition], toState);
        } else if (lineNumber == automatonA->numberOfTransitions + 2) {
            vector<int> endingStates;
            for (string token : tokens) {
                endingStates.push_back(stoi(token));
            }
            automatonA->endingStates = endingStates;
        } else if (lineNumber == automatonA->numberOfTransitions + 3) {
            int numberOfStates = stoi(tokens[0]);
            int numberOfTransitions = stoi(tokens[1]);

            automatonB = new Automaton(numberOfStates, numberOfTransitions, numberOfLetters);
        } else if (lineNumber > automatonA->numberOfStates + 3 && lineNumber < automatonA->numberOfTransitions + automatonB->numberOfTransitions + 4) {
            int fromState = stoi(tokens[0]);
            int toState = stoi(tokens[2]);
            char transition = tokens[1][0];
            automatonB->addTransition(fromState, transitionToIndex[transition], toState);
        } else if (lineNumber == automatonA->numberOfTransitions + 2 + automatonB->numberOfTransitions + 2) {
            vector<int> endingStates;
            for (string token : tokens) {
                endingStates.push_back(stoi(token));
            }
            automatonB->endingStates = endingStates;
        }

        // Increase line number
        lineNumber++;
    }

    Automaton *automatonFinal = new Automaton(automatonA->numberOfStates * automatonB->numberOfStates, 0, numberOfLetters);

    for (int i = 0; i < automatonA->numberOfStates; i++) {
        for (int j = 0; j < automatonB->numberOfStates; j++) {
            for (int k = 0; k < numberOfLetters; k++) {
                vector<int> transitionsA = automatonA->transitionsForStateAndTransition(i, k);
                vector<int> transitionsB = automatonB->transitionsForStateAndTransition(j, k);
                
                for (int m = 0; m < (int) transitionsA.size(); m++) {
                    int resultA = transitionsA[m];
                    
                    for (int n = 0; n < (int) transitionsB.size(); n++) {
                        int resultB = transitionsB[n];
                        
                        int fromState = i * automatonB->numberOfStates + j;
                        int toState = resultA * automatonB->numberOfStates + resultB;
                        automatonFinal->addTransition(fromState, k, toState);
                    }
                }
            }
        }
    }

    queue<int> states;
    vector<int> backtrack = vector<int>(automatonFinal->numberOfStates, -1);
    vector<int> backtrackTransition = vector<int>(automatonFinal->numberOfStates, -1);
    vector<string> results;

    states.push(0);
    while ( ! states.empty()) {
        int state = states.front();
        states.pop();

        if (isFinalState(state, automatonA->endingStates, automatonB->endingStates, automatonB->numberOfStates)) {
            int parent = backtrack[state];
            stack<int> transitionFinal;
            transitionFinal.push(backtrackTransition[state]);
            while (parent != 0) {
                transitionFinal.push(backtrackTransition[parent]);
                parent = backtrack[parent];
            }
            string result = "";
            while ( ! transitionFinal.empty()) {
                int transition = transitionFinal.top();
                transitionFinal.pop();
                result += indexToTransition[transition];
            }

            if (results.empty()) {
                results.push_back(result);
            } else if (result.size() == results.back().size()) {
                results.push_back(result);
            } else {
                break;
            }
        }

        for (int i = 0; i < numberOfLetters; i++) {
            vector<int> transitionsFinal = automatonFinal->transitionsForStateAndTransition(state, i);
            for (int j = 0; j < (int) transitionsFinal.size(); j++) {
                int toState = transitionsFinal[j];
                if (state == toState || backtrack[toState] >= 0) continue;
                backtrack[toState] = state;
                backtrackTransition[toState] = i;
                states.push(toState);
            }
        }
    }

    sort(results.begin(), results.end());

    cout << results[0] << endl;

    return 0;
}

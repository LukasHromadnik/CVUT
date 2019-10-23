#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Task {
    int identifier;
    int processingTime;
    int releaseTime;
    int deadline;
};

struct taskCompare {
    inline bool operator() (Task * left, Task * right) {
        if (left->releaseTime != right->releaseTime) {
            return left->releaseTime < right->releaseTime;
        } else {
            return left->deadline < right->deadline;
        }
    }
};

ostream & operator << (ostream & os, Task & task) {
    os << task.identifier;
    // os << " |";
    // os << " " << task.processingTime;
    // os << " " << task.releaseTime;
    // os << " " << task.deadline;
    return os;
}

struct Input {
    vector<Task *> tasks;
    int numberOfTasks;

    Input(int numberOfTasks);
};

Input::Input(int numberOfTasks) {
    this->numberOfTasks = numberOfTasks;
    this->tasks = vector<Task *>(numberOfTasks);
}

Input * loadInput(const char * argv[]) {
    ifstream inFile(argv[1]);

    string line;
    string word;
    int lineNumber = 0;

    Input *input = NULL;

    while (getline(inFile, line)) {

        // Create string stream from the loaded input line
        stringstream ss(line);

        // Count word count
        int wordNumber = 0;

        Task * task = new Task();
        int taskIndex = lineNumber - 1;

        // Iterate over all "token"
        while (ss >> word) {

            // Convert input to the integer
            int value = stoi(word);

            if (lineNumber == 0) {
                input = new Input(value);
            } else if (wordNumber == 0) {
                task->processingTime = value;
            } else if (wordNumber == 1) {
                task->releaseTime = value;
            } else {
                task->deadline = value;
            }

            // Increase word number
            wordNumber++;
        }

        if (taskIndex >= 0) {
            task->identifier = taskIndex;
            input->tasks[taskIndex] = task;
        }

        // Increase line number
        lineNumber++;
    }

    // sort(input->tasks.begin(), input->tasks.end(), taskCompare());

    return input;
}

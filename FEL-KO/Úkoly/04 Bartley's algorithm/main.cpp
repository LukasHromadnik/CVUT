#include <iostream>
#include <stack>
#include <vector>
#include <climits>
#include <queue>

#include "Source/input.cpp"

using namespace std;

struct Node {
    vector<Task *> tasks;
    int totalCompletionTime;
    int numberOfTasks;

    Node(Node * parent);

    void addTask(Task * task);
};

Node::Node(Node * parent) {
    this->totalCompletionTime = 0;
    this->numberOfTasks = 0;
    this->tasks = vector<Task *>();

    if (parent != NULL) {
        this->totalCompletionTime = parent->totalCompletionTime;
        this->numberOfTasks = parent->numberOfTasks;
        for (int i = 0; i < parent->numberOfTasks; i++) {
            this->tasks.push_back(parent->tasks[i]);
        }
    }
}

ostream & operator << (ostream & os, Node & node) {
    for (int i = 0; i < node.numberOfTasks; i++) {
        os << *node.tasks[i] << " ";
    }
    os << "| " << node.totalCompletionTime;
    return os;
}

void Node::addTask(Task * task) {
    totalCompletionTime = max(totalCompletionTime, task->releaseTime) + task->processingTime;
    tasks.push_back(task);
    numberOfTasks++;
}

bool checkBetterSolution(Node * node, Node * solution) {
    return node->totalCompletionTime < solution->totalCompletionTime;
}

bool checkOptimality(Node * node) {
    int startTime = 0;
    
    for (int i = 0; i < node->numberOfTasks; i++) {
        Task * task = node->tasks[i];
        if (startTime > task->releaseTime) return false;
        startTime = task->releaseTime;
    }

    return true;
}

int main(int argc, const char * argv[]) {
    Input *input = loadInput(argv);

    // for (int i = 0; i < input->numberOfTasks; i++) {
    //     Task * task = input->tasks[i];
    //     cout << task->processingTime << " " << task->releaseTime << " " << task->deadline << endl;
    // }
    // return 0;

    Node *solution = NULL;

    stack<Node *> nodes;

    // Insert the root node
    Node *root = new Node(NULL);
    nodes.push(root);
    // bool solutionIsOptimal = false;

    while ( ! nodes.empty()) {
        Node *front = nodes.top();
        // Node *front = nodes.front();
        nodes.pop();
        // cout << endl << "===== ITERATION ======" << endl;
        cout << "[FRONT] " << *front << endl;

        if (front->numberOfTasks == input->numberOfTasks) {
            cout << "[SOLUTION] " << *front << endl;
            if (solution == NULL || checkBetterSolution(front, solution)) {
                solution = front;
            }

            continue;
        }

        // Check which tasks are not used
        vector<bool> used(input->numberOfTasks, false);
        for (int i = 0; i < front->numberOfTasks; i++) {
            used[front->tasks[i]->identifier] = true;
        }

        // Compute minimal release time of the one of the unscheduled tasks
        int minReleaseTime = INT_MAX;
        for (int i = 0; i < input->numberOfTasks; i++) {
            if (used[i]) continue;
            Task * task = input->tasks[i];
            minReleaseTime = min(minReleaseTime, task->releaseTime);
        }

        // Check if the current node has partial optimal solution
        // cout << front->totalCompletionTime << " <= " << minReleaseTime << endl;
        if (front->totalCompletionTime <= minReleaseTime) {
            nodes = stack<Node *>();
            // nodes = queue<Node *>();
            cout << "[OPTIMAL SOLUTION BOUND]" << endl;
        }

        // Check the estimate of the subtree
        if (solution != NULL) {
            int totalProcessingTime = 0;
            for (int i = 0; i < input->numberOfTasks; i++) {
                if (used[i]) continue;
                Task * task = input->tasks[i];
                totalProcessingTime += task->processingTime;
            }

            if (max(front->totalCompletionTime, minReleaseTime) + totalProcessingTime >= solution->totalCompletionTime) {
                cout << "[NOT FEASIBLE PARTIAL BOUND]" << endl;
                continue;
            }
        }

        bool shouldContinue = false;
        for (int i = 0; i < input->numberOfTasks; i++) {
            Task * task = input->tasks[i];
            if (used[task->identifier]) continue;

            int totalCompletionTime = max(front->totalCompletionTime, task->releaseTime) + task->processingTime;
            if (task->deadline < totalCompletionTime) {
                shouldContinue = true;
                break;
            }
        }
        if (shouldContinue) {
            cout << "[CANNOT FIT BOUND]" << endl;
            continue;
        }
        
        // Create new nodes in the search
        for (int i = 0; i < input->numberOfTasks; i++) {
            Task * task = input->tasks[i];
            if (used[task->identifier]) continue;

            Node *node = new Node(front);
            node->addTask(task);
            nodes.push(node);
        }
    }

    ofstream outFile(argv[2]);

    if (solution == NULL) {
        cout << "-1" << endl;
        outFile << "-1";
    } else {
        vector<int> releaseTimes = vector<int>(input->numberOfTasks);
        int startTime = 0;
        for (Task * task : solution->tasks) {
            startTime = max(startTime, task->releaseTime);
            releaseTimes[task->identifier] = startTime;
            startTime += task->processingTime;
        }
        for (int i = 0; i < input->numberOfTasks; i++) {
            cout << releaseTimes[i] << endl;
            outFile << releaseTimes[i];
            if (i + 1 < input->numberOfTasks) {
                outFile << endl;
            }
        }
    }

    return 0;
}

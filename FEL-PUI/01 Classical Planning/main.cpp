#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include "problem.h"
#include <string>
#include <map>
#include <stack>
#include <climits>

using namespace std;

bool isPreconditionAdmissibleInState(vector<int> precondition, vector<int> state) {
    int preconditionIndex = 0;
    for (int i = 0; i < (int) state.size(); i++) {
        // Found all preconditions
        if (preconditionIndex >= (int) precondition.size()) return true;

        // Current item is not in the precoditions
        if (precondition[preconditionIndex] > state[i]) continue;

        // Precondition not found
        if (precondition[preconditionIndex] < state[i]) return false;

        if (precondition[preconditionIndex] == state[i]) {
            preconditionIndex++;
        }
    }

    return preconditionIndex == (int) precondition.size();
}

struct StripsOperator {
    StripsOperator() { };
    StripsOperator(strips_operator_t op);

    string name;
    int cost;
    vector<int> pre;
    vector<int> add;
    vector<int> del;
};

StripsOperator::StripsOperator(strips_operator_t op) {
    this->name = string(op.name);
    this->cost = op.cost;
    
    for (int i = 0; i < op.pre_size; i++) {
        this->pre.push_back(op.pre[i]);
    }

    for (int i = 0; i < op.add_eff_size; i++) {
        this->add.push_back(op.add_eff[i]);
    }

    for (int i = 0; i < op.del_eff_size; i++) {
        this->del.push_back(op.del_eff[i]);
    }
}

struct Strips {
    Strips() { };
    Strips(strips_t strips);

    vector<string> facts;
    vector<int> init;
    vector<int> goal;
    vector<StripsOperator *> operators;
};

Strips::Strips(strips_t strips) {
    for (int i = 0; i < strips.num_operators; i++) {
        StripsOperator *op = new StripsOperator(strips.operators[i]);
        this->operators.push_back(op);
    }

    for (int i = 0; i < strips.num_facts; i++) {
        this->facts.push_back(string(strips.fact_names[i]));
    }

    for (int i = 0; i < strips.init_size; i++) {
        this->init.push_back(strips.init[i]);
    }

    for (int i = 0; i < strips.goal_size; i++) {
        this->goal.push_back(strips.goal[i]);
    }
}

struct StateNode {
    int index;
    int value;
};

struct compareStateNodes {
    bool operator() (const StateNode* l, const StateNode* r) {  
        return l->value > r->value;
    }
};

int hMax(Strips *strips, vector<int> state) {
    vector<int> values(strips->facts.size(), INT_MAX);
    for (int i = 0; i < (int) state.size(); i++) {
        values[state[i]] = 0;
    }

    vector<int> numberOfPreconditions(strips->operators.size());
    vector<vector<int> > preconditions(strips->facts.size());
    for (int i = 0; i < (int) strips->operators.size(); i++) {
        StripsOperator *op = strips->operators[i];
        numberOfPreconditions[i] = op->pre.size();
        for (int pre : op->pre) {
            preconditions[pre].push_back(i);
        }
    }

    priority_queue<StateNode *, vector<StateNode *>, compareStateNodes> facts;
    for (int fact : state) {
        StateNode *node = new StateNode();
        node->index = fact;
        node->value = values[fact];
        facts.push(node);
    }

    while ( ! facts.empty()) {
        // Check if the goal state is reached
        bool goalStateReached = true;
        for (int goal : strips->goal) {
            if (values[goal] == INT_MAX) {
                goalStateReached = false;
                break;
            }
        }
        if (goalStateReached) break;

        StateNode *fact = facts.top();
        facts.pop();

        for (int opIndex : preconditions[fact->index]) {
            numberOfPreconditions[opIndex]--;

            if (numberOfPreconditions[opIndex] != 0) continue;
        
            StripsOperator *op = strips->operators[opIndex];

            for (int add : op->add) {
                StateNode *s = new StateNode();
                s->index = add;
                if (values[add] == INT_MAX) {
                    facts.push(s);
                }
                int maxPrecondition = INT_MIN;
                for (int pre : op->pre) {
                    maxPrecondition = max(maxPrecondition, values[pre]);
                }
                values[add] = min(values[add], maxPrecondition + op->cost);
                s->value = values[add];
            }
        }
    }

    // Check if the goal state is reached
    bool goalStateReached = true;
    for (int goal : strips->goal) {
        if (values[goal] == -1) {
            goalStateReached = false;
        }
    }
    if ( ! goalStateReached) return INT_MAX;

    // Compute the hMax
    int hMax = INT_MIN;
    for (int goal : strips->goal) {
        hMax = max(hMax, values[goal]);
    }

    return hMax;
}

struct SearchNode {
    SearchNode();

    set<int> state;
    int distance;
    int estimate;
    SearchNode *parent;
    StripsOperator *op;
    
    string identifier(Strips *strips) const;
    vector<int> vectorState();
    int value() const;
};

SearchNode::SearchNode() {
    this->distance = 0;
    this->estimate = INT_MAX;
    this->parent = NULL;
}

string SearchNode::identifier(Strips *strips) const {
    string identifier;
    vector<int> facts(strips->facts.size());
    for (int fact : state) {
        facts[fact] = 1;
    }
    for (int i = 0; i < (int) facts.size(); i++) {
        identifier += to_string(facts[i]);
    }

    return identifier;
}

vector<int> SearchNode::vectorState() {
    vector<int> _state;
    for (int i : state) {
        _state.push_back(i);
    }
    return _state;
}

int SearchNode::value() const {
    return distance + estimate;
}

struct compare {
    bool operator() (const SearchNode* l, const SearchNode* r) {  
        return l->value() > r->value();
    }
};

void showVector(vector<int> data, Strips *strips) {
    int dataIndex = 0;
    for (int i = 0; i < (int) strips->facts.size(); i++) {
        if (dataIndex < (int) data.size() && data[dataIndex] == i) {
            cout << "1";
            dataIndex++;
        } else {
            cout << "0";
        }
    }
    cout << endl;
}

bool isGoal(SearchNode *node, Strips *strips) {
    return isPreconditionAdmissibleInState(strips->goal, node->vectorState());
}

void showSolution(SearchNode *node) {
    SearchNode *root = node;
    stack<StripsOperator *> operators;
    while (root->parent != NULL) {
        operators.push(root->op);
        root = root->parent;
    }

    cout << ";; Optimal cost: " << node->distance << endl;
    cout << ";; h^max for init: " << root->estimate << endl;
    cout << endl;
    while ( ! operators.empty()) {
        StripsOperator *op = operators.top();
        operators.pop();
        cout << op->name << endl;
    }
}

int main(int argc, char *argv[]) {
 
    // Check the input
    if (argc != 3) {
        fprintf(stderr, "Usage: %s problem.strips problem.fdr\n", argv[0]);
        return -1;
    }
 
    // Load the planning problem and converted to the new interface
    strips_t stripsInput;
    stripsRead(&stripsInput, argv[1]);
    Strips *strips = new Strips(stripsInput);
    stripsFree(&stripsInput);

    // Initialize priority queue for A*
    priority_queue<SearchNode *, vector<SearchNode *>, compare> open;

    // Create init node
    SearchNode *node = new SearchNode();
    
    // Set the node's state equal to the problem initial state
    node->state = set<int>(strips->init.begin(), strips->init.end());
    
    // Set initial node distance to zero
    node->distance = 0;

    // Compute estimated distance to the goal
    node->estimate = hMax(strips, node->vectorState());

    // Push the initial node onto the search priority queue
    open.push(node);

    // Initial map of the already visited nodes
    map<string, int> closed;

    // Iterate over all discovered nodes
    while ( ! open.empty()) {
       
        // Load the node with the lowest estimated distance to the goal
        SearchNode *node = open.top();
        open.pop();

        // Get node identifier
        string nodeIdentifier = node->identifier(strips);

        // Check if the node was already discovered
        auto it = closed.find(nodeIdentifier);
        
        // If the node is fresh, create new record for it
        if (it == closed.end()) {
            closed.insert(pair<string, int>(nodeIdentifier, node->distance));
        }
        // If the node was already discovered and the new distance
        // is better than the previous, update the distance value
        else if (node->distance < it->second) {
            closed[nodeIdentifier] = node->distance;
        }
        // The current node was already discovered and the new distance is worse
        // Skip the node and continue
        else continue;

        // Check if the node is the goal node
        if (isGoal(node, strips)) {
            showSolution(node);
            break;
        }

        // Iterate over all operators
        for (StripsOperator *op : strips->operators) {

            // Check if the operator is applicable in the given state
            if ( ! isPreconditionAdmissibleInState(op->pre, node->vectorState())) continue;

            // Create new search node
            SearchNode *child = new SearchNode();

            // Set the child distance from the parent
            child->distance = node->distance + op->cost;

            // Update state according to the operator
            set<int> state = node->state;
            for (int i : op->add) {
                state.insert(i);
            }
            for (int i : op->del) {
                state.erase(i);
            }
            child->state = state;

            // Set the child's parent
            child->parent = node;

            // Set the child's operator
            child->op = op;
            
            // Compute estimated distance for the child searchNode
            child->estimate = hMax(strips, child->vectorState());

            // If the distance is admissible, push the child on the queue
            if (child->estimate < INT_MAX) {
                open.push(child);
            }
        }
    }

    return 0;
}

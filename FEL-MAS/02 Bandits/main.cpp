#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <set>

#define DANGER_PLACE "E"
#define BANDIT "B"
#define OBSTACLE "#"
#define START "S"
#define FINISH "D"
#define GOLD "G"
#define FREE "-"
#define UP "U"
#define LEFT "L"
#define DOWN "D"
#define RIGHT "R"

using namespace std;

struct Point {
    int x;
    int y;

    Point(int x, int y) { this->x = x; this->y = y; }
};

enum NodeState { Agent, Bandit, Alert };
enum PlayerAction { Up, Left, Down, Right, Undefined };

string playerActionToString(PlayerAction action) {
    switch (action) {
        case Up: return "U";
        case Left: return "L";
        case Down: return "D";
        case Right: return "R";
        case Undefined: return "";
    }

    return "";
}

struct Node {
    Point *point;
    Node *parent;
    vector<Node *> children;
    int value;
    float multiplier;
    NodeState state;
    bool wasOnDanger;
    PlayerAction action;
    bool hasTransformation;

    Node(Point *point, Node *parent, int value, PlayerAction action);
};

Node::Node(Point *point, Node *parent, int value, PlayerAction action) {
    this->point = point;
    this->parent = parent;
    this->value = value;
    this->state = Agent;
    this->multiplier = (parent != NULL) ? parent->multiplier : 1;
    this->wasOnDanger = (parent != NULL) ? parent->wasOnDanger : false;
    this->children = vector<Node *>(4, NULL);
    this->action = action;
    this->hasTransformation = false;
}

void displayPartOne(set<string> agentIdentifiers, vector<string> banditIdentifiers) {
    cout << "AGENT:" << endl;
    int index = 1;
    for (string identifier: agentIdentifiers) {
        cout << "S" << to_string(index++) << ":" << identifier << endl;
    }
    cout << endl << "ATTACKER:" << endl;
    index = 1;
    for (string identifier: banditIdentifiers) {
        cout << "Q" << to_string(index++) << ":" << identifier << endl;
    }
}

bool isPointEqualToPoint(Point *first, Point *second) {
    return (first->x == second->x) && (first->y == second->y);
}

bool isPossibleMove(Point *point, string **game) {
    return game[point->x][point->y] != OBSTACLE;
}

void displayTree(Node *node, int depth = 0) {
    if (node == NULL) return;
    
    if (depth) {
        cout << setw(depth * 2) << ' ';
    }
    cout << "[" << node->point->x << "," << node->point->y << "]";
    
    bool isDeadEnd = true;
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != NULL) {
            isDeadEnd = false;
            break;
        }
    }

    if (isDeadEnd) {
        cout << " " << node->value * node->multiplier;
    }
    cout << endl;

    for (int i = 0; i < (int) node->children.size(); i++) {
        if (node->children[i] == NULL) continue;
        displayTree(node->children[i], depth + 1);
    }
}

vector<string> displayNode(Node *node) {
    if (node == NULL) return vector<string>();
    vector<string> identifier = displayNode(node->parent);
    if (node->action != Undefined && ! node->hasTransformation) {
        identifier.push_back(playerActionToString(node->action));
    }

    return identifier;
}

void displayStrategies(Node *node, set<string> &identifiers) {
    if (node == NULL) return;
    
    bool isDeadEnd = true;
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != NULL) {
            isDeadEnd = false;
            break;
        }
    }

    if (isDeadEnd) {
        vector<string> subidentifier = displayNode(node);
        string identifier = "";
        for (int i = 0; i < (int) subidentifier.size(); i++) {
            identifier = identifier + subidentifier[i];
        }
        identifiers.insert(identifier);
        // cout << identifier << " [" << node->value * node->multiplier << "]" << endl;
    }

    for (int i = 0; i < (int) node->children.size(); i++) {
        if (node->children[i] == NULL) continue;
        displayStrategies(node->children[i], identifiers);
    }
}

void displayGame(string **game, int numberOfRows, int numberOfCols) {
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfCols; j++) {
            cout << game[i][j];
        }
        cout << endl;
    }
}

string ** copyGame(string **baseGame, int numberOfRows, int numberOfCols) {
    string **game = new string *[numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        game[i] = new string [numberOfCols];
        for (int j = 0; j < numberOfCols; j++) {
            if (baseGame[i][j] == BANDIT) {
                game[i][j] = DANGER_PLACE;
            } else {
                game[i][j] = baseGame[i][j];
            }
        }
    }

    return game;
}

string ** createNewGameWithPositions(string ** baseGame, int numberOfRows, int numberOfCols, vector<int> places, vector<Point *> dangerPlaces) {
    string **game = copyGame(baseGame, numberOfRows, numberOfCols);
    for (int i = 0; i < (int) places.size(); i++) {
        vector<Point *> bandits;
        if (places[i] == 1) {
            bandits.push_back(dangerPlaces[i]);
        }
        for (int j = 0; j < (int) bandits.size(); j++) {
            Point *bandit = bandits[j];
            game[bandit->x][bandit->y] = BANDIT;
        }
    }

    return game;
}

float utility(vector<string **> games, vector<Node *> startNodes, string agentStrategy, string banditStrategy) {
    queue<int> children;
    for (int i = 0; i < (int) agentStrategy.size(); i++) {
        string letter = agentStrategy.substr(i, 1);
        if (letter == UP) {
            children.push(0);
        } else if (letter == LEFT) {
            children.push(1);
        } else if (letter == DOWN) {
            children.push(2);
        } else if (letter == RIGHT) {
            children.push(3);
        }
    }

    int gameNumber = -1;
    int transformationNumber = -1;
    for (int i = 0; i < (int) banditStrategy.size(); i++) {
        if (banditStrategy[i] == 'T') {
            gameNumber = stoi(banditStrategy.substr(1, i - 1));
            transformationNumber = stoi(banditStrategy.substr(i + 1, banditStrategy.size() - i));
            break;
        }
    }

    Node *node = startNodes[gameNumber];
    while ( ! children.empty()) {
        int index = children.front();
        children.pop();
        if (node->hasTransformation) {
            node = node->children[transformationNumber];
        }

        node = node->children[index];
    }

    return node->value * node->multiplier;
}

vector<string **> generateGames(string **baseGame, vector<Point *> dangerPlaces, int numberOfBandits, int numberOfRows, int numberOfCols) {
    vector<string **> games;
    vector<int> places = vector<int>(dangerPlaces.size(), 0);
    
    int sum = 0;
    
    while (sum != (int) places.size()) {
        if (sum == numberOfBandits) {
            string **game = createNewGameWithPositions(baseGame, numberOfRows, numberOfCols, places, dangerPlaces);
            games.push_back(game);
        }
        for (int i = 0; i < (int) places.size(); i++) {
            if (places[i] == 1) {
                places[i] = 0;
            } else {
                places[i] = 1;
                break;
            }
        }
        sum = 0;
        for (int i = 0; i < (int) places.size(); i++) {
            sum += places[i];
        }
    }
    if (sum == numberOfBandits) {
        string **game = createNewGameWithPositions(baseGame, numberOfRows, numberOfCols, places, dangerPlaces);
        games.push_back(game);
    }

    return games;
}

void generateTree(Node *node, string **game, float successfullAttackProbability, vector<Point *> dangerPlaces, int numberOfBandits, int numberOfRows, int numberOfCols) {
    string nodeKey = game[node->point->x][node->point->y];
    
    // Check if the node is destination node
    if (nodeKey == FINISH) {
        node->value = max(node->value, 0) + 10;
        return;
    }

    // Check if the node is on the gold
    else if (nodeKey == GOLD) {
        node->value = max(node->value, 0) + 1;
    }

    // Check if the node is battle node
    else if (nodeKey == BANDIT) {
        node->multiplier *= successfullAttackProbability;
        node->wasOnDanger = true;
    }

    // Check if the node is able to make alarm
    else if (nodeKey == DANGER_PLACE && ! node->wasOnDanger) {
        node->wasOnDanger = true;
        node->hasTransformation = true;
        vector<Point *> dangerPlacesWithoutCurrentPlace;
        for (int i = 0; i < (int) dangerPlaces.size(); i++) {
            Point *place = dangerPlaces[i];
            if (isPointEqualToPoint(place, node->point)) continue;
            dangerPlacesWithoutCurrentPlace.push_back(place);
        }
        vector<string **> games = generateGames(game, dangerPlacesWithoutCurrentPlace, numberOfBandits, numberOfRows, numberOfCols);
        for (int i = 0; i < (int) games.size(); i++) {
            Node *child = new Node(node->point, node, node->value, node->action);
            if (i < 4) {
                node->children[i] = child;
            } else {
                node->children.push_back(child);
            }
            generateTree(child, games[i], successfullAttackProbability, dangerPlaces, numberOfBandits, numberOfRows, numberOfCols);
        }
        return;
    }

    // Generate all possible moves
    Point *up = new Point(node->point->x - 1, node->point->y);
    Point *left = new Point(node->point->x, node->point->y - 1);
    Point *down = new Point(node->point->x + 1, node->point->y);
    Point *right = new Point(node->point->x, node->point->y + 1);

    // Store them in array
    Point * points[4] = { up, left, down, right };

    // Iterate over all moves
    for (int i = 0; i < 4; i++) {
        
        // If the move goes to the obstacle remove it
        if ( ! isPossibleMove(points[i], game)) {
            node->children[i] = NULL;
            continue;
        }

        // Check if the move is already in the path
        Node *parent = node->parent;
        bool isAlreadyInPath = false;
        while (parent != NULL) {

            // If the moves are equal then the current move is already in the path
            if (isPointEqualToPoint(parent->point, points[i])) {
                node->children[i] = NULL;
                isAlreadyInPath = true;
                break;
            }
            parent = parent->parent;
        }

        if (isAlreadyInPath) continue;

        // Add new possible move as a child
        PlayerAction action;
        switch (i) {
            case 0:
                action = Up;
                break;
            case 1:
                action = Left;
                break;
            case 2:
                action = Down;
                break;
            case 3:
                action = Right;
                break;
            default:
                action = Undefined;
                break;
        }
        node->children[i] = new Node(points[i], node, node->value, action);
    }

    // Check if the current node is a dead end
    bool isDeadEnd = true;
    for (int i = 0; i < 4; i++) {
        if (node->children[i] == NULL) continue;

        isDeadEnd = false;

        // Generate subtrees for all possible children
        generateTree(node->children[i], game, successfullAttackProbability, dangerPlaces, numberOfBandits, numberOfRows, numberOfCols);
    }

    // Set value for the dead end
    if (isDeadEnd) node->value = 0;
}

int main(int argc, const char * argv[]) {
    // Helpers for input
    string line, word;
    int lineNumber = 0;

    int numberOfRows = 0;
    int numberOfCols = 0;
    int numberOfBandits = 0;
    float successfullAttackProbability = 0;
    string **baseGame = NULL;
    
    Point *start = NULL;
    vector<Point *> dangerPlaces;

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
            numberOfRows = stoi(tokens[0]);
        } else if (lineNumber == 1) {
            numberOfCols = stoi(tokens[0]);

            baseGame = new string * [numberOfRows];
            for (int i = 0; i < numberOfRows; i++) {
                baseGame[i] = new string[numberOfCols];
            }
        } else if (lineNumber == 2 + numberOfRows) {
            numberOfBandits = stoi(tokens[0]);
        } else if (lineNumber == 3 + numberOfRows) {
            successfullAttackProbability = stof(tokens[0]);
        } else {
            int x = lineNumber - 2;
            for (int i = 0; i < (int) tokens[0].size(); i++) {
                string letter = tokens[0].substr(i, 1);
                if (letter == START) {
                    start = new Point(x, i);
                } else if (letter == DANGER_PLACE) {
                    dangerPlaces.push_back(new Point(x, i));
                }
                baseGame[x][i] = letter;
            }
        }

        // Increase line number
        lineNumber++;
    }

    vector<string **> games = generateGames(baseGame, dangerPlaces, numberOfBandits, numberOfRows, numberOfCols);
    vector<Node *>startNodes = vector<Node*>(games.size(), NULL);
    set<string> agentIdentifiers;
    for (int i = 0; i < (int) games.size(); i++) {
        Node *startNode = new Node(start, NULL, -1, Undefined);
        startNodes[i] = startNode;
        string **game = games[i];
        generateTree(startNode, game, 1 - successfullAttackProbability, dangerPlaces, numberOfBandits, numberOfRows, numberOfCols);
        displayStrategies(startNode, agentIdentifiers);
    }

    vector<Point *> newDangerPlaces;
    for (int i = 1; i < (int) dangerPlaces.size(); i++) {
        newDangerPlaces.push_back(dangerPlaces[i]);
    }

    vector<string> banditIdentifiers;
    vector<string **> transformations = generateGames(baseGame, newDangerPlaces, numberOfBandits, numberOfRows, numberOfCols);
    for (int i = 0; i < (int) games.size(); i++) {
        for (int j = 0; j < (int) transformations.size(); j++) {
            string identifier = "G" + to_string(i) + "T" + to_string(j);
            banditIdentifiers.push_back(identifier);
        }
    }

    vector<string> agentIDs(agentIdentifiers.begin(), agentIdentifiers.end());
    float **utilities = new float * [agentIDs.size()];
    for (int i = 0; i < (int) agentIDs.size(); i++) {
        utilities[i] = new float [banditIdentifiers.size()];
        for (int j = 0; j < (int) banditIdentifiers.size(); j++) {
            utilities[i][j] = utility(games, startNodes, agentIDs[i], banditIdentifiers[j]);
        }
    }

    displayPartOne(agentIdentifiers, banditIdentifiers);

    cout << endl;

    cout << "AGENT\\ATTACKER |";
    for (int i = 0; i < (int) banditIdentifiers.size(); i++) {
        cout << "  Q" << setw(7) << left << i + 1 << "|";
    }
    cout << endl;

    int lineWidth = 16 + banditIdentifiers.size() * 11;
    for (int i = 0; i < lineWidth; i++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < (int) agentIDs.size(); i++) {
        cout << "S" << setw(14) << left << i + 1 << "|";
        for (int j = 0; j < (int) banditIdentifiers.size(); j++) {
            cout << " " << setw(8) << setprecision(2) << fixed << right << utilities[i][j] << " |";
        }
        cout << endl;
    }

    for (int i = 0; i < lineWidth; i++) {
        cout << "-";
    }
    cout << endl;
    
    return 0;
}

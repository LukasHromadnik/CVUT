using namespace std;

class Node;

class Edge {
public:
    Edge(Node *firstNode, Node *secondNode, int value);
    pair<Node *, Node *> m_Nodes;
    int m_Value;
};

Edge::Edge(Node *firstNode, Node *secondNode, int value) {
    m_Nodes = pair<Node *, Node *>(firstNode, secondNode);
    m_Value = value;
}

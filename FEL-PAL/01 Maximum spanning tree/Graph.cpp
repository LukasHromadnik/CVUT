class Node;
class Edge;

using namespace std;

class Graph {
public:
    Graph(int numberOfNodes, int numberOfEdges);
    vector<Node *> m_Nodes;
    vector<Edge *> m_Edges;
    int m_NumberOfNodes;
    int m_NumberOfEdges;

    void addEdge(int firstNodeIndex, int secondNodeIndex, int value);
};

Graph::Graph(int numberOfNodes, int numberOfEdges) {
    m_Nodes = vector<Node *>(numberOfNodes);
    m_NumberOfNodes = numberOfNodes;

    m_NumberOfEdges = numberOfEdges;
}

void Graph::addEdge(int firstNodeIndex, int secondNodeIndex, int value) {
    if (m_Nodes[firstNodeIndex] == NULL) {
        m_Nodes[firstNodeIndex] = new Node(firstNodeIndex);
    }

    if (m_Nodes[secondNodeIndex] == NULL) {
        m_Nodes[secondNodeIndex] = new Node(secondNodeIndex);
    }

    Node *node1 = m_Nodes[firstNodeIndex];
    Node *node2 = m_Nodes[secondNodeIndex];
    Edge *edge = new Edge(node1, node2, value);
    node1->addEdge(edge);
    node2->addEdge(edge);
    m_Edges.push_back(edge);
}

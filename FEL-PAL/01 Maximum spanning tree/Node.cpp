using namespace std;

class Edge;

class Node {
public:
    Node(int index);
    int m_Index;
    set<Edge *> m_Edges;
    Node *m_Parent;
    int m_Rank;

    void addEdge(Edge *edge);
    int parentIndex();
    int debugIndex();
    Node *parent();
};

Node::Node(int index) {
    m_Index = index;
    m_Parent = this;
    m_Rank = 0;
}

void Node::addEdge(Edge *edge) {
    m_Edges.insert(edge);
}

Node * Node::parent() {
    if (m_Parent->m_Index != m_Index) {
        Node *newParent = m_Parent->parent();
        m_Parent = newParent;
    }

    return m_Parent;
}

int Node::debugIndex() {
    return m_Index + 1;
}

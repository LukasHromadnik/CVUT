class Edge;

class Span {
public:
    Span(int maxEdgeValue);
    vector<Edge *> m_Edges;
    vector<int> m_Counts;
    int m_MaxCountOfSimilarEdges;
    long long int m_Value;

    void addEdge(Edge *edge);
};

Span::Span(int maxEdgeValue) {
    m_Counts = vector<int>(maxEdgeValue);
    m_Value = 0;
    m_MaxCountOfSimilarEdges = 0;
}

void Span::addEdge(Edge *edge) {
    int value = edge->m_Value;
    m_Edges.push_back(edge);
    m_Value += value;
    m_Counts[value - 1] += 1;

    m_MaxCountOfSimilarEdges = max(m_Counts[value - 1], m_MaxCountOfSimilarEdges);
}

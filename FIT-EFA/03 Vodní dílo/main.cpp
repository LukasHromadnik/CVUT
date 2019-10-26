#include <iostream>
#define MODULO 536870909

using namespace std;

class Node
{
public:
	Node(long long value) : m_Count(0), m_Value(value), m_Children(NULL), m_Ping(0), m_PingValue(0), m_With(1), m_Without(0) {};
	void Add(Node * node);
	void Result();
	void Print();

	long long m_Count;
	long long m_Value;
	Node ** m_Children;
	long long m_Ping;
	long long m_PingValue;
	long long m_With;
	long long m_Without;
};

void Node::Add(Node * node)
{
	if ( ! m_Children && m_Count) {
		m_Children = new Node * [m_Count];
		m_Count = 0;
	}

	m_Children[m_Count++] = node;
	m_PingValue += node->m_Value;
}

void Node::Result()
{
	for (long long i = 0; i < m_Count; i++) {
		Node * child = m_Children[i];

		if (child->m_Value == m_PingValue) {
			continue;
		}

		m_With *= ((child->m_With + 1) % MODULO);
		m_Without += ((child->m_With + child->m_Without) % MODULO);
		
		m_With %= MODULO;
		m_Without %= MODULO;
	}
}

void Node::Print()
{
	cout << m_Value << ":" << endl;
	cout << "With: " << m_With << endl;
	cout << "Without: " << m_Without << endl;
	cout << "-----" << endl;
}

int main(int argc, char const *argv[])
{
	long long count;
	cin >> count;

	if (count == 1) {
		cout << "1" << endl;

		return 0;
	}

	long long i;
	long long stackIndex = 0;

	Node ** nodes = new Node * [count];
	Node ** stack = new Node * [count];
	for (i = 0; i < count; i++) {
		nodes[i] = new Node(i);
		stack[i] = NULL;
	}
	
	for (i = 0; i < count; i++) {
		long long children;
		cin >> children;

		Node * current = nodes[i];
		current->m_Count = children;

		for (long long j = 0; j < children; j++) {
			long long child;
			cin >> child;

			current->Add(nodes[child]);
		}

		if (children == 1) {
			stack[stackIndex++] = current;
		}
	}

	long long index = 0;

	while (index != count) {
		Node * elem = stack[index++];
		
		if (index == count) {
			elem->m_PingValue = -1;
		}
		elem->Result();
		
		if (elem->m_PingValue == -1) {
			continue;
		}
		
		Node * parent = NULL;
		for (i = 0; i < elem->m_Count; i++) {
			if (elem->m_Children[i]->m_Value == elem->m_PingValue) {
				parent = elem->m_Children[i];
				break;
			}
		}

		parent->m_Ping++;
		parent->m_PingValue -= elem->m_Value;
		
		if (parent->m_Ping + 1 == parent->m_Count) {
			stack[stackIndex++] = parent;
		}
	}

	Node * root = stack[stackIndex - 1];
	cout << ((root->m_With + root->m_Without) % MODULO) << endl;

	return 0;
}

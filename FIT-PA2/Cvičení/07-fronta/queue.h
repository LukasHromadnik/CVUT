#include <iostream>
using namespace std;

template <class Elem>
class Queue {
public:
	Queue (void);
	~Queue (void);
	void insert (int x);
	int remove ();
	int isEmpty (void) const;
private:
	Elem * head;
	Elem * tail;
};

template <class Elem>
Queue<Elem>::Queue (void) {
	this->head = NULL;
	this->tail = NULL;
}

template <class Elem>
void Queue<Elem>::insert (int x) {
	Elem * newElem = new Elem(x);

	if (Queue::isEmpty())
		this->head = newElem;
	else
		this->tail->m_Next = newElem;

	this->tail = newElem;
}

template <class Elem>
int Queue<Elem>::isEmpty() const {
	return ! this->head;
}

template <class Elem>
Queue<Elem>::~Queue (void) {
	Elem * tmp = NULL;
	while (this->head) {
		tmp = this->head->m_Next;

		delete this->head;

		this->head = tmp;
	}
}

template <class Elem>
int Queue<Elem>::remove () {
	int res = this->head->m_Val;
	Elem * tmp = this->head->m_Next;

	delete this->head;

	this->head = tmp;

	return res;
}

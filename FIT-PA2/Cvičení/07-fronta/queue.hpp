#include <iostream>
using namespace std;

#include "queue.h"

template <class Elem>
void Queue<Elem>::Queue() {
	this->head = NULL;
	this->tail = NULL;
}

template <class Elem>
void Queue<Elem>::Elem::Elem(int val, Elem * next) {
	m_Val = val;
	m_Next = next;
}

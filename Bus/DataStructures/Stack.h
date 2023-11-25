#pragma once
#include "Node.h"
#include "Container.h"
template<typename T>
class Stack : public Container
{
	Node<T>* Head;
public:
	void Push(T val) {
		Node<T>* temp = new Node<T>(val);
		if (size == 0)
			Head = temp;
		else {
			temp->setNext(Head);
			Head = temp;
		}
		size++;
	}

	T Peek() { return Head->getItem(); }

	T Pop() {
		T item = Head->getItem();
		Head = Head->getNext();
		size--;
		return item;
	}
};


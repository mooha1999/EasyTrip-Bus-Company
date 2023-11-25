#pragma once
#include "Node.h"
template<typename T, typename P>
class PriorityNode : public Node<T>
{
	PriorityNode<T, P>* next;
	P priority;
public:
	PriorityNode(T data, P priority) : Node<T>(data) {
		next = nullptr;
		this->priority = priority;
	}
	
	P getPriority() { return priority; }

	PriorityNode<T, P>* getNext() { return next; }

	void setNext(PriorityNode<T, P>* Next) {
		this->next = Next;
	}
};


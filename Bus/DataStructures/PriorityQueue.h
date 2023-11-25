#pragma once
#include "PriorityNode.h"
#include "Container.h"
#include <assert.h>
using namespace std;
template<typename T, typename P = int>
class PriorityQueue : public Container//<T>
{
	PriorityNode<T, P>* Head;
	PriorityNode<T, P>* Tail;
	class Iterator {
	private:
		Node<T>* current;

	public:
		Iterator(Node<T>* node) : current(node) {}

		T operator*() const {
			return current->getItem();
		}

		Iterator& operator++() {
			current = current->getNext();
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return current != other.current;
		}
	};
public:
	PriorityQueue() {
		Head = Tail = nullptr;
		size = 0;
	}
	void Insert(T val, P priority) {
		PriorityNode<T, P>* temp = new PriorityNode<T, P>(val, priority);
		if (size == 0)
			Head = Tail = temp;
		else if (priority >= Head->getPriority()) {
			temp->setNext(Head);
			Head = temp;
		}
		else if (priority <= Tail->getPriority()) {
			Tail->setNext(temp);
			Tail = temp;
		}
		else {
			for (PriorityNode<T, P>* i = Head; i; i = i->getNext()) {
				if (priority > i->getNext()->getPriority()) {
					temp->setNext(i->getNext());
					i->setNext(temp);
					break;
				}
			}
		}
		size++;
	}
	T Top() { return Head->getItem(); }
	P TopPriority() { return Head->getPriority(); }
	T Pop() {
		assert(size > 0);
		T item = Head->getItem();
		Head = Head->getNext();
		size--;
		return item;
	}
	Iterator begin() const {
		return Iterator(Head);
	}

	Iterator end() const {
		return Iterator(nullptr);
	}
	//virtual void Add(T item, P priority) { Insert(item, priority); }
};

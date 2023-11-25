#pragma once
#include "Container.h"
#include "Node.h"
template<typename T>
class Queue : public Container
{
	Node<T>* Head, * Tail;
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
	Queue() { Head = Tail = nullptr; }

	void Push(T val) {
		Node<T>* temp = new Node<T>(val);
		if (size == 0)
			Head = Tail = temp;
		else {
			Tail->setNext(temp);
			Tail = temp;
		}
		size++;
	}

	T Pop() {
		T item = Head->getItem();
		Head = Head->getNext();
		size--;
		return item;
	}

	T Peek() { return Head->getItem(); }

	Iterator begin() const {
		return Iterator(Head);
	}

	Iterator end() const {
		return Iterator(nullptr);
	}
};

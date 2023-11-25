#pragma once
#include "Node.h"
#include "Container.h"
using namespace std;
template<typename T>
class LinkedList : public Container//<T>
{
	Node<T>* Head;
	Node<T>* Tail;
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
	LinkedList()
	{
		Head = nullptr;
		Tail = nullptr;
		size = 0;
	}
	void InsertEnd(T data) {
		Node<T>* temp = new Node<T>(data);
		if (Head == nullptr) {
			Head = Tail = temp;
		}
		else {
			Tail->setNext(temp);
			Tail = Tail->getNext();
		}
		size++;
	}
	void Remove(T data) {
		if (size == 0)
			return;
		if (data == Head->getItem())
		{
			Head = Head->getNext();
		}
		else
		{
			for (Node<T>* h = Head; h; h = h->getNext()) {
				if (h->getNext()->getItem() == data) {
					if (h->getNext() == Tail)
					{
						Tail = h;
						Tail->setNext(nullptr);
					}
					else
						h->setNext(h->getNext()->getNext());
					break;
				}
			}
		}
		size--;
	}
	T operator[](int index) {
		Node<T>* h = Head;
		if (index == size - 1)
			return Tail->getItem();
		for (int i = 0; i < index; h = h->getNext(), i++);
		return h->getItem();
	}
	Iterator begin() const {
		return Iterator(Head);
	}

	Iterator end() const {
		return Iterator(nullptr);
	}
};
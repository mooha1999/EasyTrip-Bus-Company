#pragma once
#include "Node.h"
template<typename T>
class Queue
{
	Node<T>* head; 
	Node<T>* tail;
	int count;
public:
	Queue() {
		head = tail = nullptr;
		count = 0;
	}
	bool isEmpty() {
		return count==0;
	}
	void enqueue(T val) {
		count++;
		Node<T>* temp = new Node<T>(val);
		if (isEmpty()) {
			head = tail = temp;
			return;
		}
		tail->setNext(temp);
		tail = temp;
	}
	T dequeue() {
		if (isEmpty())
			return;
		count--;
		head = head->getNext();
	}
	T peek() {
		return isEmpty() ? T() : head->getData();
	}
	int size() {
		return count;
	}

};


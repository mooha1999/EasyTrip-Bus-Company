#pragma once
template<typename T>
class Node
{
    T data;
    Node<T>* next;

public:
    Node(T value) {
        data = value;
        next = nullptr;
    }

    // Setter for the data
    void setData(T value) {
        data = value;
    }

    // Getter for the data
    T getData() {
        return data;
    }

    // Setter for the next pointer
    void setNext(Node<T>* node) {
        next = node;
    }

    // Getter for the next pointer
    Node<T>* getNext() {
        return next;
    }
};


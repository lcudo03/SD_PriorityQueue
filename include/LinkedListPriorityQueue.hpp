#ifndef LINKEDLISTPRIORITYQUEUE_HPP
#define LINKEDLISTPRIORITYQUEUE_HPP

#include "PriorityQueue.hpp"
#include <stdexcept>

template <typename T>
class LinkedListPriorityQueue : public PriorityQueue<T> {
public:
    LinkedListPriorityQueue();
    ~LinkedListPriorityQueue();
    
    void insert(const T& e, int p) override;
    T extractMax() override;
    const T& findMax() const override;
    void modifyKey(const T& e, int p) override;
    size_t size() const override;
    bool empty() const override;

private:
    struct Node {
        T element;
        int priority;
        Node* next;
        Node(const T& e, int p) : element(e), priority(p), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    size_t count;
    
    Node* findMaxNode() const;
    Node* findNode(const T& e) const;
    Node* findPrevNode(Node* target) const;
};


template <typename T>
LinkedListPriorityQueue<T>::LinkedListPriorityQueue() 
    : head(nullptr), tail(nullptr), count(0) {}

template <typename T>
LinkedListPriorityQueue<T>::~LinkedListPriorityQueue() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
void LinkedListPriorityQueue<T>::insert(const T& e, int p) {
    Node* newNode = new Node(e, p);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

template <typename T>
T LinkedListPriorityQueue<T>::extractMax() {
    Node* maxNode = findMaxNode();
    T maxElement = maxNode->element;
    
    Node* prev = findPrevNode(maxNode);
    
    if (prev == nullptr) {
        head = maxNode->next;
    } else {
        prev->next = maxNode->next;
    }
    
    if (maxNode == tail) {
        tail = prev;
    }
    
    delete maxNode;
    count--;
    return maxElement;
}

template <typename T>
const T& LinkedListPriorityQueue<T>::findMax() const {
    Node* maxNode = findMaxNode();
    return maxNode->element;
}

template <typename T>
void LinkedListPriorityQueue<T>::modifyKey(const T& e, int p) {
    Node* node = findNode(e);
    node->priority = p;
}

template <typename T>
size_t LinkedListPriorityQueue<T>::size() const {
    return count;
}

template <typename T>
bool LinkedListPriorityQueue<T>::empty() const {
    return head == nullptr;
}

template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findMaxNode() const {
    if (empty()) {
        throw std::runtime_error("Kolejka priorytetowa jest pusta");
    }

    Node* maxNode = head;
    Node* current = head->next;

    while (current != nullptr) {
        if (current->priority > maxNode->priority) {
            maxNode = current;
        }
        current = current->next;
    }

    return maxNode;
}

template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findNode(const T& e) const {
    Node* current = head;

    while (current != nullptr) {
        if (current->element == e) {
            return current;
        }
        current = current->next;
    }

    throw std::runtime_error("Brak elementu w kolejce");
}

template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findPrevNode(Node* target) const {
    if (target == head) {
        return nullptr;
    }

    Node* current = head;
    while (current != nullptr && current->next != target) {
        current = current->next;
    }
    return current;
}
#endif // LINKEDLISTPRIORITYQUEUE_HPP
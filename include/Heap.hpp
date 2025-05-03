// Implementacja kolejki priorytetowej pod postacią kopca binarnego

#ifndef HEAPPRIORITYQUEUE_HPP
#define HEAPPRIORITYQUEUE_HPP

#include "PriorityQueue.hpp"
#include "DynamicArray.hpp"
#include <utility>

template <typename T>
class Heap : public PriorityQueue<T> {
public:
    Heap() = default;
    
    void insert(const T& e, int p) override;
    T extractMax() override;
    const T& findMax() const override;
    void modifyKey(const T& e, int p) override;
    size_t size() const override;
    bool empty() const override;

private:
    DynamicArray<std::pair<T, int>> heap;
    
    void heapifyUp(size_t index);
    void heapifyDown(size_t index);
    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t leftChild(size_t i) const { return 2 * i + 1; }
    size_t rightChild(size_t i) const { return 2 * i + 2; }
    
    // Pomocnicza funkcja do znajdowania indeksu elementu
    size_t findElementIndex(const T& e) const;
};

template <typename T>
void Heap<T>::insert(const T& e, int p) {
    //
    heap.push_back({e, p});
    heapifyUp(heap.getSize() - 1);
}

template <typename T>
T Heap<T>::extractMax() {
    if (empty()) {
        throw std::runtime_error("Kolejka jest pusta");
    }
    
    T maxElement = heap[0].first;
    
    if (heap.getSize() > 1) {
        heap[0] = heap[heap.getSize() - 1];
    }
    
    heap.pop_back();
    
    if (!empty()) {
        heapifyDown(0);
    }
    
    return maxElement;
}

template <typename T>
const T& Heap<T>::findMax() const {
    if (empty()) {
        throw std::runtime_error("Kolejka jest pusta");
    }
    return heap[0].first;
}

template <typename T>
void Heap<T>::modifyKey(const T& e, int p) {
    size_t index = findElementIndex(e);
    int oldPriority = heap[index].second;
    heap[index].second = p;
    
    if (p > oldPriority) {
        heapifyUp(index);
    } else if (p < oldPriority) {
        heapifyDown(index);
    }
}

template <typename T>
size_t Heap<T>::size() const {
    return heap.getSize();
}

template <typename T>
bool Heap<T>::empty() const {
    return heap.empty();
}

template <typename T>
void Heap<T>::heapifyUp(size_t index) {
    while (index > 0 && heap[parent(index)].second < heap[index].second) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

template <typename T>
void Heap<T>::heapifyDown(size_t index) {
    size_t maxIndex = index;
    size_t left = leftChild(index);
    size_t right = rightChild(index);
    
    if (left < heap.getSize() && heap[left].second > heap[maxIndex].second) {
        maxIndex = left;
    }
    
    if (right < heap.getSize() && heap[right].second > heap[maxIndex].second) {
        maxIndex = right;
    }
    
    if (index != maxIndex) {
        std::swap(heap[index], heap[maxIndex]);
        heapifyDown(maxIndex);
    }
}

template <typename T>
size_t Heap<T>::findElementIndex(const T& e) const {
    for (size_t i = 0; i < heap.getSize(); ++i) {
        if (heap[i].first == e) {
            return i;
        }
    }
    throw std::runtime_error("Niee znaleziono elementu w kolejce");
}

#endif
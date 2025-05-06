#ifndef HEAP_HPP
#define HEAP_HPP

#include "PriorityQueue.hpp"
#include "DynamicArray.hpp"
#include <stdexcept>  // Do obsługi wyjątków
#include <utility>    // Dla std::pair
#include <iostream>   // Do wyświetlania

template <typename T>
class Heap : public PriorityQueue<T> {
public:
    Heap() = default;  // Domyślny konstruktor
    
    // Interfejs PriorityQueue
    void insert(const T& e, int p) override;
    T extractMax() override;
    const T& findMax() const override;
    void modifyKey(const T& e, int p) override;
    size_t size() const override;
    bool empty() const override;

    void display() const override;  // Metoda pomocnicza do wyświetlania

    // Zwraca priorytet elementu o najwyższym priorytecie
    // Złożoność: O(1)
    int findMaxPriority() const {
        if (empty()) throw std::runtime_error("Kolejka jest pusta");
        return heap[0].second;  // Priorytet korzenia
    }
    
private:
    DynamicArray<std::pair<T, int>> heap;  // Przechowuje pary (element, priorytet)
    
    // Funkcje pomocnicze do utrzymywania własności kopca
    void heapifyUp(size_t index);    // Przywraca własność kopca w górę
    void heapifyDown(size_t index);  // Przywraca własność kopca w dół
    
    // Funkcje pomocnicze do nawigacji po kopcu
    size_t parent(size_t i) const { return (i - 1) / 2; }      // Indeks rodzica
    size_t leftChild(size_t i) const { return 2 * i + 1; }     // Indeks lewego dziecka
    size_t rightChild(size_t i) const { return 2 * i + 2; }    // Indeks prawego dziecka
    
    // Znajduje indeks elementu w kopcu
    // Złożoność: O(n) - liniowe przeszukiwanie
    size_t findElementIndex(const T& e) const;
};

// Implementacja metod szablonowych

/**
 * Wstawia nowy element do kopca
 * param e element do wstawienia
 * param p priorytet elementu
 * Złożoność: O(log n)
 */
template <typename T>
void Heap<T>::insert(const T& e, int p) {
    heap.push_back({e, p});          // Dodanie na koniec (O(1) amortyzowane)
    heapifyUp(heap.getSize() - 1);   // Naprawa kopca (O(log n))
}

/**
 * Usuwa i zwraca element o najwyższym priorytecie
 * return element o najwyższym priorytecie
 * Złożoność: O(log n)
 */
template <typename T>
T Heap<T>::extractMax() {
    if (empty()) {
        throw std::runtime_error("Kolejka jest pusta");
    }
    
    T maxElement = heap[0].first;    // Zapamiętanie elementu korzenia
    
    if (heap.getSize() > 1) {
        heap[0] = heap[heap.getSize() - 1];  // Przeniesienie ostatniego elementu do korzenia
    }
    
    heap.pop_back();                 // Usunięcie ostatniego elementu
    
    if (!empty()) {
        heapifyDown(0);              // Naprawa kopca od korzenia (O(log n))
    }
    
    return maxElement;
}

/**
 * Zwraca element o najwyższym priorytecie bez usuwania
 * return referencja do elementu o najwyższym priorytecie
 * Złożoność: O(1)
 */
template <typename T>
const T& Heap<T>::findMax() const {
    if (empty()) {
        throw std::runtime_error("Kolejka jest pusta");
    }
    return heap[0].first;  // Element korzenia ma najwyższy priorytet
}

/**
 * Modyfikuje priorytet elementu
 * parametr e element do zmiany
 * parametr p nowy priorytet
 * Złożoność: O(n) dla wyszukiwania + O(log n) dla naprawy = O(n)
 */
template <typename T>
void Heap<T>::modifyKey(const T& e, int p) {
    size_t index = findElementIndex(e);  // O(n)
    int oldPriority = heap[index].second;
    heap[index].second = p;              // Aktualizacja priorytetu
    
    // Naprawa kopca w odpowiednim kierunku
    if (p > oldPriority) {
        heapifyUp(index);    // O(log n)
    } else if (p < oldPriority) {
        heapifyDown(index);  // O(log n)
    }
}

/**
 * Zwraca liczbę elementów w kopcu
 * zwraca rozmiar kopca
 * Złożoność: O(1)
 */
template <typename T>
size_t Heap<T>::size() const {
    return heap.getSize();  // Deleguje do DynamicArray
}

/**
 * Sprawdza czy kopiec jest pusty
 * zwraca true jeśli kopiec jest pusty
 * Złożoność: O(1)
 */
template <typename T>
bool Heap<T>::empty() const {
    return heap.empty();  // Deleguje do DynamicArray
}

/**
 * Przywraca własność kopca przesuwając element w górę
 * parametr indeks elementu do wyniesienia
 * Złożoność: O(log n)
 */
template <typename T>
void Heap<T>::heapifyUp(size_t index) {
    while (index > 0 && heap[parent(index)].second < heap[index].second) {
        std::swap(heap[index], heap[parent(index)]);  // Zamiana z rodzicem
        index = parent(index);                        // Przejście do rodzica
    }
}

/**
 * Przywraca własność kopca przesuwając element w dół
 * parametr "index" indeks elementu do opuszczenia
 * Złożoność: O(log n)
 */
template <typename T>
void Heap<T>::heapifyDown(size_t index) {
    size_t maxIndex = index;
    size_t left = leftChild(index);
    size_t right = rightChild(index);
    
    // Znajdź największy element spośród rodzica i dzieci
    if (left < heap.getSize() && heap[left].second > heap[maxIndex].second) {
        maxIndex = left;
    }
    
    if (right < heap.getSize() && heap[right].second > heap[maxIndex].second) {
        maxIndex = right;
    }
    
    // Jeśli rodzic nie jest największy, zamień z większym dzieckiem i kontynuuj
    if (index != maxIndex) {
        std::swap(heap[index], heap[maxIndex]);
        heapifyDown(maxIndex);
    }
}

/**
 * Znajduje indeks elementu w kopcu
 * parametr e element do znalezienia
 * zwraca indeks elementu
 * Złożoność: O(n)
 */
template <typename T>
size_t Heap<T>::findElementIndex(const T& e) const {
    for (size_t i = 0; i < heap.getSize(); ++i) {
        if (heap[i].first == e) {
            return i;
        }
    }
    throw std::runtime_error("Nie znaleziono elementu w kolejce");
}

/**
 * Wyświetla zawartość kopca w porządku malejących priorytetów
 * Złożoność: O(n log n) - tworzenie kopii i n operacji extractMax
 */
template <typename T>
void Heap<T>::display() const {
    if (empty()) {
        std::cout << "Kopiec jest pusty." << std::endl;
        return;
    }

    // Utwórz kopię do wyświetlenia
    Heap<T> copy;
    for (size_t i = 0; i < heap.getSize(); ++i) {
        copy.insert(heap[i].first, heap[i].second);
    }

    std::cout << "Zawartosc kopca (element: priorytet):" << std::endl;
    while (!copy.empty()) {
        T element = copy.extractMax();
        int priority = copy.empty() ? 0 : copy.findMaxPriority();
        std::cout << element << ": " << priority << std::endl;
    }
}

#endif // HEAP_HPP
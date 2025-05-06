#ifndef LINKEDLISTPRIORITYQUEUE_HPP
#define LINKEDLISTPRIORITYQUEUE_HPP

#include "PriorityQueue.hpp"
#include <stdexcept>
#include <iostream>

// Szablon klasy LinkedListPriorityQueue dziedziczący po PriorityQueue
template <typename T>
class LinkedListPriorityQueue : public PriorityQueue<T> {
public:
    LinkedListPriorityQueue();  // Konstruktor
    ~LinkedListPriorityQueue(); // Destruktor
    
    // Metody interfejsu PriorityQueue
    void insert(const T& e, int p) override; // Wstawia element z priorytetem
    T extractMax() override;                 // Usuwa i zwraca element o najwyższym priorytecie
    const T& findMax() const override;       // Zwraca element o najwyższym priorytecie
    void modifyKey(const T& e, int p) override; // Modyfikuje priorytet elementu
    size_t size() const override;            // Zwraca rozmiar kolejki
    bool empty() const override;             // Sprawdza czy kolejka jest pusta

    void display() const override;           // Wyświetla zawartość kolejki

    // Dodatkowa metoda zwracająca maksymalny priorytet
    int findMaxPriority() const {
        Node* maxNode = findMaxNode();
        return maxNode->priority;
    }
private:
    // Wewnętrzna struktura węzła
    struct Node {
        T element;      // Przechowywany element
        int priority;   // Priorytet elementu
        Node* next;     // Wskaźnik na następny węzeł
        
        // Konstruktor węzła
        Node(const T& e, int p) : element(e), priority(p), next(nullptr) {}
    };
    
    Node* head;     // Wskaźnik na początek listy
    Node* tail;     // Wskaźnik na koniec listy
    size_t count;   // Licznik elementów
    
    // Metody pomocnicze
    Node* findMaxNode() const;       // Znajduje węzeł z maksymalnym priorytetem
    Node* findNode(const T& e) const; // Znajduje węzeł z danym elementem
    Node* findPrevNode(Node* target) const; // Znajduje poprzednik danego węzła
};

// Konstruktor - inicjalizuje pustą kolejkę
template <typename T>
LinkedListPriorityQueue<T>::LinkedListPriorityQueue() 
    : head(nullptr), tail(nullptr), count(0) {}

// Destruktor - zwalnia pamięć wszystkich węzłów
template <typename T>
LinkedListPriorityQueue<T>::~LinkedListPriorityQueue() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// Wstawia nowy element z priorytetem na koniec listy
template <typename T>
void LinkedListPriorityQueue<T>::insert(const T& e, int p) {
    Node* newNode = new Node(e, p); // Tworzy nowy węzeł
    if (tail == nullptr) {         // Jeśli kolejka jest pusta
        head = tail = newNode;     // Nowy węzeł jest głową i ogonem
    } else {
        tail->next = newNode;      // Dodaj nowy węzeł na koniec
        tail = newNode;            // Przesuń ogon na nowy węzeł
    }
    count++;                       // Zwiększ licznik
}

// Usuwa i zwraca element o najwyższym priorytecie
template <typename T>
T LinkedListPriorityQueue<T>::extractMax() {
    Node* maxNode = findMaxNode();  // Znajdź węzeł z maksymalnym priorytetem
    T maxElement = maxNode->element; // Zapisz element
    
    Node* prev = findPrevNode(maxNode); // Znajdź poprzednik
    
    // Usuń węzeł z listy
    if (prev == nullptr) {
        head = maxNode->next;       // Usuwamy głowę listy
    } else {
        prev->next = maxNode->next; // Omijamy usuwany węzeł
    }
    
    if (maxNode == tail) {
        tail = prev;               // Aktualizuj ogon jeśli usuwamy ostatni
    }
    
    delete maxNode;                // Zwolnij pamięć
    count--;                       // Zmniejsz licznik
    return maxElement;             // Zwróć element
}

// Zwraca referencję do elementu o najwyższym priorytecie
template <typename T>
const T& LinkedListPriorityQueue<T>::findMax() const {
    Node* maxNode = findMaxNode();
    return maxNode->element;
}

// Modyfikuje priorytet danego elementu
template <typename T>
void LinkedListPriorityQueue<T>::modifyKey(const T& e, int p) {
    Node* node = findNode(e);      // Znajdź węzeł z elementem
    node->priority = p;            // Zaktualizuj priorytet
}

// Zwraca liczbę elementów w kolejce
template <typename T>
size_t LinkedListPriorityQueue<T>::size() const {
    return count;
}

// Sprawdza czy kolejka jest pusta
template <typename T>
bool LinkedListPriorityQueue<T>::empty() const {
    return head == nullptr;
}

// Znajduje węzeł z najwyższym priorytetem (przeszukuje całą listę)
template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findMaxNode() const {
    if (empty()) {
        throw std::runtime_error("Kolejka priorytetowa jest pusta");
    }

    Node* maxNode = head;          // Zacznij od głowy
    Node* current = head->next;    // Porównuj z następnymi

    while (current != nullptr) {
        if (current->priority > maxNode->priority) {
            maxNode = current;     // Znaleziono wyższy priorytet
        }
        current = current->next;   // Przejdź do następnego
    }

    return maxNode;
}

// Znajduje węzeł zawierający dany element
template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findNode(const T& e) const {
    Node* current = head;

    while (current != nullptr) {
        if (current->element == e) {
            return current;        // Znaleziono element
        }
        current = current->next;   // Przejdź do następnego
    }

    throw std::runtime_error("Brak elementu w kolejce");
}

// Znajduje poprzednik danego węzła
template <typename T>
typename LinkedListPriorityQueue<T>::Node* 
LinkedListPriorityQueue<T>::findPrevNode(Node* target) const {
    if (target == head) {
        return nullptr;            // Głowa nie ma poprzednika
    }

    Node* current = head;
    while (current != nullptr && current->next != target) {
        current = current->next;   // Szukaj aż znajdziesz poprzednik
    }
    return current;
}

// Wyświetla zawartość kolejki w kolejności malejących priorytetów
template <typename T>
void LinkedListPriorityQueue<T>::display() const {
    if (empty()) {
        std::cout << "Lista jest pusta." << std::endl;
        return;
    }

    // Utwórz kopię do wyświetlenia (aby nie modyfikować oryginału)
    LinkedListPriorityQueue<T> copy;
    Node* current = head;
    while (current != nullptr) {
        copy.insert(current->element, current->priority);
        current = current->next;
    }

    // Wyświetl elementy w kolejności od najwyższego priorytetu
    std::cout << "Zawartosc listy (element: priorytet):" << std::endl;
    while (!copy.empty()) {
        T element = copy.extractMax();
        int priority = copy.empty() ? 0 : copy.findMaxPriority();
        std::cout << element << ": " << priority << std::endl;
    }
}

#endif // LINKEDLISTPRIORITYQUEUE_HPP
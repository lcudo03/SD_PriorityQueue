#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

// Szablon klasy LinkedList dla dowolnego typu T
template <typename T>
class LinkedList {
public:
    // Wewnętrzna struktura reprezentująca węzeł listy
    struct Node {
        T data;         // Dane przechowywane w węźle
        Node* next;     // Wskaźnik na następny węzeł
        
        // Konstruktor węzła inicjalizujący dane i ustawiający next na nullptr
        Node(const T& data) : data(data), next(nullptr) {}
    };
    
    // Konstruktor - inicjalizuje pustą listę (head i tail nullptr, rozmiar 0)
    LinkedList() : head(nullptr), tail(nullptr), listSize(0) {}
    
    // Destruktor - zwalnia pamięć wszystkich węzłów przez wywołanie clear()
    ~LinkedList() {
        clear();
    }
    
    // Dodaje nowy element na końcu listy
    void push_back(const T& value) {
        Node* newNode = new Node(value); // Tworzy nowy węzeł
        if (tail == nullptr) {           // Jeśli lista jest pusta
            head = tail = newNode;       // Nowy węzeł jest zarówno głową jak i ogonem
        } else {
            tail->next = newNode;        // Dodaj nowy węzeł za obecnym ogonem
            tail = newNode;             // Przesuń ogon na nowy węzeł
        }
        ++listSize;                     // Zwiększ licznik elementów
    }
    
    // Usuwa węzeł toDelete, wymaga podania jego poprzednika (prev)
    void erase(Node* prev, Node* toDelete) {
        if (toDelete == nullptr) return; // Nic do usunięcia
        
        if (prev == nullptr) {          // Usuwamy głowę listy
            head = toDelete->next;      // Przesuń głowę na następny węzeł
            if (head == nullptr) {
                tail = nullptr;         // Jeśli lista stała się pusta, zresetuj ogon
            }
        } else {
            prev->next = toDelete->next; // Omijamy węzeł toDelete w liście
            if (toDelete == tail) {
                tail = prev;            // Jeśli usuwamy ogon, zaktualizuj wskaźnik tail
            }
        }
        
        delete toDelete;               // Zwolnij pamięć
        --listSize;                    // Zmniejsz licznik elementów
    }
    
    // Czyści całą listę, zwalniając pamięć wszystkich węzłów
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next; // Zapamiętaj następny węzeł
            delete current;             // Usuń obecny węzeł
            current = next;             // Przejdź do następnego węzła
        }
        head = tail = nullptr;          // Zresetuj wskaźniki
        listSize = 0;                   // Zresetuj licznik
    }
    
    // Zwraca wskaźnik na pierwszy węzeł (głowę) listy
    Node* begin() const { return head; }
    
    // Zwraca nullptr (wskaźnik za ostatnim elementem)
    Node* end() const { return nullptr; }
    
    // Zwraca liczbę elementów w liście
    size_t size() const { return listSize; }
    
    // Sprawdza czy lista jest pusta (true jeśli rozmiar == 0)
    bool empty() const { return listSize == 0; }
    
private:
    Node* head;     // Wskaźnik na pierwszy węzeł listy
    Node* tail;     // Wskaźnik na ostatni węzeł listy
    size_t listSize; // Licznik elementów w liście
};

#endif // LINKEDLIST_HPP
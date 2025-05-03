#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

template <typename T>
class LinkedList {
public:
    // Węzeł listy
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };
    
    // Konstruktor - inicjalizuje pustą listę
    LinkedList() : head(nullptr), tail(nullptr), listSize(0) {}
    
    // Destruktor - zwalnia wszystkie węzły
    ~LinkedList() {
        clear();
    }
    
    // Dodaje element na końcu listy
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) { // Lista pusta
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        ++listSize;
    }
    
    // Usuwa węzeł (wymaga podania poprzednika)
    void erase(Node* prev, Node* toDelete) {
        if (toDelete == nullptr) return;
        
        if (prev == nullptr) { // Usuwamy głowę
            head = toDelete->next;
            if (head == nullptr) {
                tail = nullptr; // Lista stała się pusta
            }
        } else {
            prev->next = toDelete->next;
            if (toDelete == tail) {
                tail = prev; // Aktualizujemy ogon jeśli usuwamy ostatni
            }
        }
        
        delete toDelete;
        --listSize;
    }
    
    // Czyści całą listę
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        listSize = 0;
    }
    
    // Zwraca wskaźnik na początek listy
    Node* begin() const { return head; }
    
    // Zwraca nullptr (koniec listy)
    Node* end() const { return nullptr; }
    
    // Zwraca rozmiar listy
    size_t size() const { return listSize; }
    
    // Sprawdza czy lista jest pusta
    bool empty() const { return listSize == 0; }
    
private:
    Node* head;     // Wskaźnik na początek listy
    Node* tail;     // Wskaźnik na koniec listy
    size_t listSize; // Liczba elementów
};

#endif // L
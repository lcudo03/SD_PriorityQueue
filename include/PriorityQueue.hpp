// Klasa abstrakcyjna repreezentująca strukturę kolejki priorytetowej

#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

template <typename T>
class PriorityQueue {
public:
    virtual ~PriorityQueue() = default;
    
    // Dodanie elementu e o priorytecie p
    virtual void insert(const T& e, int p) = 0;
    
    // Usunięcie i zwrócenie elementu o największym priorytecie
    virtual T extractMax() = 0;
    
    // Zwrócenie elementu o największym priorytecie bez usuwania
    virtual const T& findMax() const = 0;
    
    // Zmiana priorytetu elementu e na p
    virtual void modifyKey(const T& e, int p) = 0;
    
    // Zwrócenie rozmiaru kolejki
    virtual size_t size() const = 0;
    
    // Sprawdzenie czy kolejka jest pusta
    virtual bool empty() const = 0;

    const T& peek() const { return findMax(); }

    T pop() { return extractMax(); }
    
};

#endif
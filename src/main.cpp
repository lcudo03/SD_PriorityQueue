#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include "Heap.hpp"
//#include "LinkedList.hpp"

template <typename PQ>
void testPriorityQueue(PQ& pq, const std::string& name) {
    std::cout << "Testing " << name << ":\n";
    
    // Wstawianie elementow o losowym priorytecie(zakres 1-100)
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10; ++i) {
        int priority = dist(gen);
        pq.insert("Element" + std::to_string(i), priority);
        std::cout << "Wstawiono: Element" << i << " o priorytecie " << priority << "\n";
    }
    
    auto insertTime = std::chrono::high_resolution_clock::now();
    std::cout << "Czas wstawiania: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(insertTime - start).count() 
              << " μs\n";
    
    // 
    std::cout << "Aktualny element o najwiekszy prio: " << pq.peek() << "\n";
    
    // 
    try {
        pq.modifyKey("Element5", 200);
        std::cout << "Zmodyfikowano priorytet Element5 na 200\n";
    } catch (const std::exception& e) {
        std::cout << "Bład modyfikowania klucza: " << e.what() << "\n";
    }
    
    std::cout << "Nowy element o najwiekszym priorytecie: " << pq.peek() << "\n";
    
    // 
    std::cout << "Wyswietlanie elementow zaleznie od priorytetu(od najwiekszego do najmniejszego):\n";
    while (!pq.empty()) {
        std::cout << pq.extractMax() << " ";
    }
    std::cout << "\n";
    
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Calkowity czas: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() 
              << " μs\n\n";
}

int main() {
    Heap<std::string> binaryHeapPQ;
    //LinkedList<std::string> linkedListPQ;
    
    testPriorityQueue(binaryHeapPQ, "Binary Heap Priority Queue");
    //testPriorityQueue(linkedListPQ, "Linked List Priority Queue");
    
    return 0;
}
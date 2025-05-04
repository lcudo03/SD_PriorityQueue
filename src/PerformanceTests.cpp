#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

#include "Heap.hpp"
#include "LinkedListPriorityQueue.hpp"

// Generator liczb losowych
class RandomGenerator {
public:
    RandomGenerator(int min, int max) 
        : engine(std::random_device{}()), distribution(min, max) {}
    
    int generate() {
        return distribution(engine);
    }

private:
    std::mt19937 engine;
    std::uniform_int_distribution<int> distribution;
};

// Pomiar czasu wykonania funkcji z wielokrotnym powtórzeniem
template<typename Func>
double measureAvgTime(Func f, int repetitions = 1000) {
    std::vector<long long> times;
    times.reserve(repetitions);
    
    for (int i = 0; i < repetitions; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        f();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        times.push_back(duration);
    }
    
    // Oblicz średnią czasów (w mikrosekundach)
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    return (sum / repetitions) / 1000.0; // Konwersja do mikrosekund
}

// Test wydajności dla danej struktury
template<typename PriorityQueue>
void testStructurePerformance(const std::vector<std::pair<int, int>>& data, 
                             const std::string& structureName) {
    std::cout << "Testing " << structureName << "...\n";
    
    // Liczba powtórzeń dla operacji
    const int reps = 1000;

    // Test insert (wykonany raz, bo i tak wstawiamy wszystkie elementy)
    PriorityQueue pq;
    double insertTime = measureAvgTime([&]() {
        for (const auto& item : data) {
            pq.insert(item.first, item.second);
        }
    }, 1); // Tylko 1 wykonanie dla insert

    double sizeTime = measureAvgTime([&]() {
        volatile auto s = pq.size(); // volatile aby zapobiec optymalizacji
    }, reps);
    
    // Test findMax - wykonany wielokrotnie
    double findMaxTime = measureAvgTime([&]() {
        volatile auto max = pq.findMax(); // volatile aby zapobiec optymalizacji
    }, reps);
    
    // Test extractMax - wykonany dla wszystkich elementów
    PriorityQueue pq2;
    for (const auto& item : data) {
        pq2.insert(item.first, item.second);
    }
    
    double extractMaxTime = measureAvgTime([&]() {
        std::vector<int> extracted;
        extracted.reserve(data.size());
        while (!pq2.empty()) {
            extracted.push_back(pq2.extractMax());
        }
    }, 1); // Tylko 1 wykonanie pełnej ekstrakcji
    
    // Średni czas per operacja extractMax
    double avgExtractMaxTime = extractMaxTime / data.size();
    
    // Test modifyKey (na losowych elementach)
    PriorityQueue pq3;
    for (const auto& item : data) {
        pq3.insert(item.first, item.second);
    }
    RandomGenerator rg(0, data.size()-1);
    
    double modifyKeyTime = measureAvgTime([&]() {
        for (int i = 0; i < reps; ++i) {
            int index = rg.generate();
            pq3.modifyKey(data[index].first, data[index].second + 1000);
        }
    }, 1); // Tylko 1 wykonanie grupy operacji
    
    // Średni czas per operacja modifyKey
    double avgModifyKeyTime = modifyKeyTime / reps;
    
    // Zapisz wyniki
    std::ofstream out(structureName + "_results.csv", std::ios::app);
    out << data.size() << ","
        << insertTime << ","
        << sizeTime << ","
        << findMaxTime << ","
        << avgExtractMaxTime << ","
        << avgModifyKeyTime << "\n";
    out.close();
}

int main() {
    // Rozmiary do testowania
    const std::vector<int> sizes = {5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000, 200000, 500000, 1000000};
    
    // Nagłówki w plikach CSV
    std::ofstream bh_out("Heap_results.csv");
    bh_out << "Size,InsertTime,FindMaxTime,ExtractMaxTime,ModifyKeyTime,ReturnigSize\n";
    bh_out.close();
    
    std::ofstream ll_out("LinkedList_results.csv");
    ll_out << "Size,InsertTime,FindMaxTime,ExtractMaxTime,ModifyKeyTime\n";
    ll_out.close();
    
    for (int size : sizes) {
        std::cout << "Testing size: " << size << "\n";
            // Generuj te same dane dla obu struktur
            RandomGenerator rg(0, 1000000);
            std::vector<std::pair<int, int>> data;
            for (int i = 0; i < size; ++i) {
                data.emplace_back(i, rg.generate());
            }
            
            // Test obu struktur na tych samych danych
            testStructurePerformance<Heap<int>>(data, "Heap");
            testStructurePerformance<LinkedListPriorityQueue<int>>(data, "LinkedList");
    }
    
    std::cout << "Koniec";
    return 0;
}
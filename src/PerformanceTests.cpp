#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

#include "Heap.hpp"
#include "LinkedListPriorityQueue.hpp"

// Klasa generatora liczb losowych z określonego zakresu
class RandomGenerator {
public:
    // Konstruktor inicjalizujący generator z podanym zakresem
    RandomGenerator(int min, int max) 
        : engine(std::random_device{}()), distribution(min, max) {}
    
    // Generuje i zwraca losową liczbę z zadanego zakresu
    int generate() {
        return distribution(engine);
    }

private:
    std::mt19937 engine;                // Silnik generujący liczby pseudolosowe
    std::uniform_int_distribution<int> distribution; // Rozkład równomierny
};

// Funkcja mierząca średni czas wykonania funkcji
template<typename Func>
double measureAvgTime(Func f, int repetitions = 1000) {
    std::vector<long long> times;       // Wektor przechowujący czasy wykonania
    times.reserve(repetitions);
    
    // Wykonaj funkcję określoną liczbę razy
    for (int i = 0; i < repetitions; ++i) {
        auto start = std::chrono::high_resolution_clock::now(); // Początek pomiaru
        f();                               // Wykonanie mierzonej funkcji
        auto end = std::chrono::high_resolution_clock::now();   // Koniec pomiaru
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        times.push_back(duration);        // Zapisz czas wykonania
    }
    
    // Oblicz średnią czasów i konwertuj na mikrosekundy
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    return (sum / repetitions) / 1000.0;
}

// Test wydajności dla danej struktury danych
template<typename PriorityQueue>
void testStructurePerformance(const std::vector<std::pair<int, int>>& data, 
                             const std::string& structureName) {
    std::cout << "Testing " << structureName << "...\n";
    
    const int reps = 1000;  // Liczba powtórzeń dla operacji

    // Test wstawiania elementów
    PriorityQueue pq;
    double insertTime = measureAvgTime([&]() {
        for (const auto& item : data) {
            pq.insert(item.first, item.second);
        }
    }, 1);

    // Test czasu pobierania rozmiaru
    double sizeTime = measureAvgTime([&]() {
        volatile auto s = pq.size(); // volatile zapobiega optymalizacji
    }, reps);
    
    // Test znajdowania maksimum
    double findMaxTime = measureAvgTime([&]() {
        // Zmiana priorytetu losowego elementu, aby maksimum mogło się zmienić
        int randomIndex = rand() % data.size();
        pq.modifyKey(data[randomIndex].first, data[randomIndex].second + 1);
        
        volatile auto max = pq.findMax(); // Wymusza pełne przeszukanie
    }, reps);
    
    // Test usuwania maksimum (dla wszystkich elementów)
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
    }, 1);
    
    // Oblicz średni czas na operację extractMax
    double avgExtractMaxTime = extractMaxTime / data.size();
    
    // Test modyfikacji klucza (na losowych elementach)
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
    }, 1);
    
    // Oblicz średni czas na operację modifyKey
    double avgModifyKeyTime = modifyKeyTime / reps;
    
    // Zapisz wyniki do pliku CSV
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
    // Rozmiary danych do testowania
    const std::vector<int> sizes = {5000, 8000, 10000, 16000, 20000, 
                                   40000, 60000, 100000, 200000, 500000};
    
    // Przygotowanie plików wynikowych
    std::ofstream bh_out("Heap_results.csv");
    bh_out << "Size,InsertTime,SizeTime,FindMaxTime,ExtractMaxTime,ModifyKeyTime\n";
    bh_out.close();
    
    std::ofstream ll_out("LinkedList_results.csv");
    ll_out << "Size,InsertTime,SizeTime,FindMaxTime,ExtractMaxTime,ModifyKeyTime\n";
    ll_out.close();
    
    // Test dla każdego rozmiaru danych
    for (int size : sizes) {
        std::cout << "Testing size: " << size << "\n";
        
        // Generowanie danych testowych (para: element, priorytet)
        RandomGenerator rg(0, 1000000);
        std::vector<std::pair<int, int>> data;
        for (int i = 0; i < size; ++i) {
            data.emplace_back(i, rg.generate());
        }
        
        // Testowanie obu struktur na tych samych danych
        testStructurePerformance<Heap<int>>(data, "Heap");
        testStructurePerformance<LinkedListPriorityQueue<int>>(data, "LinkedList");
    }
    
    std::cout << "Koniec";
    return 0;
}
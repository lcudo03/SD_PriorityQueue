#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <stdexcept>
#include "PriorityQueue.hpp"
#include "Heap.hpp"
#include "LinkedListPriorityQueue.hpp"

using namespace std;

// Funkcje pomocnicze
void clearScreen() {
    system("cls");
}

int getIntInput(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc. Wprowadz liczbe od " 
                 << min << " do " << max << endl;
        } else {
            cin.ignore();
            return value;
        }
    }
}

// Funkcje operacji dla menu
template<typename T>
void buildFromFile(PriorityQueue<T>& pq) {
    string filename;
    cout << "Podaj nazwe pliku: ";
    getline(cin, filename);
    
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Nie mozna otworzyc pliku!" << endl;
        return;
    }
    
    // Najpierw wyczyść kolejkę
    while (!pq.empty()) {
        pq.extractMax();
    }
    
    T element;
    int priority;
    while (file >> element >> priority) {
        pq.insert(element, priority);
    }
    
    cout << "Zbudowano kolejke z " << pq.size() << " elementow." << endl;
}

template<typename T>
void createRandom(PriorityQueue<T>& pq) {
    int size = getIntInput("Podaj rozmiar kolejki (min 1): ", 1);
    int minPriority = getIntInput("Podaj minimalny priorytet: ");
    int maxPriority = getIntInput("Podaj maksymalny priorytet: ", minPriority);
    
    // Najpierw wyczyść kolejkę
    while (!pq.empty()) {
        pq.extractMax();
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(minPriority, maxPriority);
    
    for (int i = 0; i < size; ++i) {
        pq.insert(static_cast<T>(i), dist(gen)); // Rzutowanie na typ T
    }
    
    cout << "Wygenerowano losowa kolejke o rozmiarze " << size << endl;
}

// Menu dla konkretnej struktury
template<typename T>
void structureMenu(PriorityQueue<T>& pq, const string& structureName) {
    while (true) {
        clearScreen();
        cout << "=== Menu " << structureName << " ===" << endl;
        cout << "1. Zbuduj z pliku" << endl;
        cout << "2. Dodaj element" << endl;
        cout << "3. Usun maksymalny element" << endl;
        cout << "4. Znajdz maksymalny element" << endl;
        cout << "5. Utworz losowo" << endl;
        cout << "6. Wyswietl" << endl;
        cout << "7. Modyfikuj priorytet" << endl;
        cout << "8. Powrot do menu glownego" << endl;
        
        int choice = getIntInput("Wybierz opcje: ", 1, 8);
        
        switch (choice) {
            case 1:
                buildFromFile<T>(pq);
                break;
            case 2: {
                T element;
                cout << "Podaj element: ";
                cin >> element;
                int priority = getIntInput("Podaj priorytet: ");
                pq.insert(element, priority);
                cout << "Dodano element." << endl;
                break;
            }
            case 3: {
                if (pq.empty()) {
                    cout << "Kolejka jest pusta!" << endl;
                } else {
                    T element = pq.extractMax();
                    cout << "Usunieto element: " << element << endl;
                }
                break;
            }
            case 4: {
                if (pq.empty()) {
                    cout << "Kolejka jest pusta!" << endl;
                } else {
                    T element = pq.findMax();
                    cout << "Maksymalny element: " << element << endl;
                }
                break;
            }
            case 5:
                createRandom<T>(pq);
                break;
            case 6:
                pq.display();
                break;
            case 7: {
                if (pq.empty()) {
                    cout << "Kolejka jest pusta!" << endl;
                    break;
                }
                T element;
                cout << "Podaj element do modyfikacji: ";
                cin >> element;
                int newPriority = getIntInput("Podaj nowy priorytet: ");
                try {
                    pq.modifyKey(element, newPriority);
                    cout << "Zmodyfikowano priorytet." << endl;
                } catch (const runtime_error& e) {
                    cout << e.what() << endl;
                }
                break;
            }
            case 8:
                return;
        }
        cout << "Nacisnij Enter, aby kontynuowac...";
        cin.ignore();
        cin.get();
    }
}

// Menu główne
int main() {
    Heap<int> heapQueue;
    LinkedListPriorityQueue<int> linkedListQueue;
    
    while (true) {
        clearScreen();
        cout << "=== MENU GLOWNE ===" << endl;
        cout << "1. Testuj kopiec binarny" << endl;
        cout << "2. Testuj liste wiazana" << endl;
        cout << "3. Zakoncz program" << endl;
        
        int choice = getIntInput("Wybierz opcje: ", 1, 3);
        
        switch (choice) {
            case 1:
                structureMenu<int>(heapQueue, "Kopiec Binarny");
                break;
            case 2:
                structureMenu<int>(linkedListQueue, "Lista Wiazana");
                break;
            case 3:
                return 0;
        }
    }
}
#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <stdexcept>  // Do obsługi wyjątków

template <typename T>
class DynamicArray {
public:
    // Konstruktor - alokuje początkową pamięć (domyślnie 10 elementów)
    // Złożoność: O(1)
    DynamicArray() : capacity(10), size(0) {
        data = new T[capacity];  // Alokacja pamięci
    }

    // Destruktor - zwalnia pamięć
    // Złożoność: O(1)
    ~DynamicArray() {
        delete[] data;  // Zwolnienie zaalokowanej pamięci
    }

    // Dodaje element na końcu tablicy
    // Złożoność: O(1) (amortyzowane), O(n) w przypadku resize
    void push_back(const T& value) {
        if (size >= capacity) {
            resize();  // Powiększa tablicę 2x gdy brak miejsca
        }
        data[size++] = value;  // Dodanie elementu i inkrementacja rozmiaru
    }

    // Usuwa ostatni element
    // Złożoność: O(1)
    void pop_back() {
        if (size == 0) throw std::out_of_range("Array is empty");
        --size;  // Dekrementacja rozmiaru (bez fizycznego usuwania)
    }

    // Operator dostępu do elementów z kontrolą zakresu
    // Złożoność: O(1)
    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data[index];  // Zwraca referencję do elementu
    }

    // Wersja const operatora [] (dla obiektów const)
    // Złożoność: O(1)
    const T& operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data[index];  // Zwraca const referencję do elementu
    }

    // Zwraca rozmiar tablicy
    // Złożoność: O(1)
    size_t getSize() const { return size; }

    // Sprawdza czy tablica jest pusta
    // Złożoność: O(1)
    bool empty() const { return size == 0; }

    // Czyści tablicę (nie zwalnia pamięci, tylko zeruje rozmiar)
    // Złożoność: O(1)
    void clear() {
        size = 0;  // Reset rozmiaru bez zmiany capacity
    }

private:
    T* data;         // Wskaźnik na dane (początek tablicy)
    size_t capacity; // Całkowita pojemność tablicy
    size_t size;     // Aktualna liczba elementów w tablicy

    // Powiększa tablicę 2x
    // Złożoność: O(n) - musi skopiować wszystkie elementy
    void resize() {
        capacity *= 2;  // Podwajamy pojemność
        T* newData = new T[capacity];  // Nowa tablica
        
        // Kopiowanie elementów do nowej tablicy
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];  // Kopiowanie elementów
        }
        
        delete[] data;  // Zwolnienie starej pamięci
        data = newData; // Ustawienie nowej tablicy
    }
};

#endif // DYNAMICARRAY_HPP
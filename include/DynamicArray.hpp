#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <stdexcept>

template <typename T>
class DynamicArray {
public:

 // Konstruktor - alokuje początkową pamięć (domyślnie 10 elementów)
 DynamicArray() : capacity(10), size(0) {
    data = new T[capacity];
}

// Destruktor - zwalnia pamięć
~DynamicArray() {
    delete[] data;
}

// Dodaje element na końcu tablicy
void push_back(const T& value) {
    if (size >= capacity) {
        resize(); // Powiększa tablicę 2x gdy brak miejsca
    }
    data[size++] = value;
}

// Usuwa ostatni element
void pop_back() {
    if (size == 0) throw std::out_of_range("Array is empty");
    --size;
}

// Operator dostępu do elementów z kontrolą zakresu
T& operator[](size_t index) {
    if (index >= size) throw std::out_of_range("Index out of range");
    return data[index];
}

// Wersja const operatora []
const T& operator[](size_t index) const {
    if (index >= size) throw std::out_of_range("Index out of range");
    return data[index];
}

// Zwraca rozmiar tablicy
size_t getSize() const { return size; }

// Sprawdza czy tablica jest pusta
bool empty() const { return size == 0; }

// Czyści tablicę (nie zwalnia pamięci, tylko zeruje rozmiar)
void clear() {
    size = 0;
}

private:
T* data;         // Wskaźnik na dane
size_t capacity; // Całkowita pojemność
size_t size;     // Aktualna liczba elementów

// Powiększa tablicę 2x
void resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i]; // Kopiuje elementy
    }
    delete[] data;
    data = newData;
}
};

#endif // DYNAMICARRAY_HPP
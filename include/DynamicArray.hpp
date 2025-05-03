#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <stdexcept>

template <typename T>
class DynamicArray {
public:
    DynamicArray() : capacity(10), size(0) {
        data = new T[capacity];
    }
    
    ~DynamicArray() {
        delete[] data;
    }
    
    void push_back(const T& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    void pop_back() {
        if (size == 0) {
            throw std::out_of_range("Tablica jest pusta");
        }
        --size;
    }
    
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Indeks poza zasięgiem");
        }
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Indeks poza zasięgiem");
        }
        return data[index];
    }
    
    size_t getSize() const { return size; }
    bool empty() const { return size == 0; }
    
    void clear() {
        size = 0;
    }
    
private:
    T* data;
    size_t capacity;
    size_t size;
    
    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
};

#endif // DYNAMICARRAY_HPP
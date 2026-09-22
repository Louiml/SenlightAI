/*
Write a C++ function named `reserveStorage` that takes a reference to a templated `myVector` class (as defined in the snippet, with private members `data`, `size`, and `capacity`, public constructors/destructor, `push_back`, `print_vector`, and getters) and an integer `new_capacity`. The function should resize the vector's underlying dynamic array so that its `capacity` becomes at least `new_capacity` (if `new_capacity` is larger than the current capacity). The existing elements in the vector must be preserved in their original order, and the function must handle the case where `new_capacity` is less than or equal to the current capacity by doing nothing. After calling `reserveStorage`, the `getCapacity()` should return the new (expanded) capacity, and `getSize()` should remain unchanged. Assume the template type `T` is copy-constructible and assignable.
*/

#include <cstddef>

template <typename T>
class myVector; // forward declaration

template <typename T>
void reserveStorage(myVector<T>& vec, size_t new_capacity);

template <typename T>
class myVector {
    private:
        T* data;
        size_t size;
        size_t capacity;
        friend void reserveStorage<T>(myVector<T>&, size_t);
    public:
        myVector(size_t init_capacity = 2) : data(new T[init_capacity]), size(0), capacity(init_capacity) {}
        ~myVector() { delete[] data; }
        size_t getSize() const { return size; }
        size_t getCapacity() const { return capacity; }
        void push_back(const T& val) {
            if (size == capacity) {
                size_t new_cap = capacity * 2;
                T* new_data = new T[new_cap];
                for (size_t i = 0; i < size; ++i) new_data[i] = data[i];
                delete[] data;
                data = new_data;
                capacity = new_cap;
            }
            data[size++] = val;
        }
};

// Free function to reserve capacity
template <typename T>
void reserveStorage(myVector<T>& vec, size_t new_capacity) {
    if (new_capacity <= vec.capacity) {
        return;
    }
    T* new_data = new T[new_capacity];
    for (size_t i = 0; i < vec.size; ++i) {
        new_data[i] = vec.data[i];
    }
    delete[] vec.data;
    vec.data = new_data;
    vec.capacity = new_capacity;
}

#include <cassert>

int main() {
    myVector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    reserveStorage(v1, 5);
    assert(v1.getSize() == 2);
    assert(v1.getCapacity() == 5);
    assert(v1.getData()[0] == 10);
    assert(v1.getData()[1] == 20);

    // No-op if new_capacity <= current capacity
    size_t old_cap = v1.getCapacity();
    reserveStorage(v1, old_cap);
    assert(v1.getCapacity() == old_cap);
    reserveStorage(v1, 1);
    assert(v1.getCapacity() == old_cap);

    // Reserve larger capacity after multiple pushes
    myVector<double> v2;
    v2.push_back(1.1);
    v2.push_back(2.2);
    v2.push_back(3.3);
    reserveStorage(v2, 10);
    assert(v2.getSize() == 3);
    assert(v2.getCapacity() == 10);
    assert(v2.getData()[0] == 1.1);
    assert(v2.getData()[2] == 3.3);

    // Large reserve for string type
    myVector<char> v3;
    v3.push_back('a');
    reserveStorage(v3, 1000);
    assert(v3.getSize() == 1);
    assert(v3.getCapacity() == 1000);
    assert(v3.getData()[0] == 'a');
}

// The solution requires direct access to the private members of `myVector`. Since the function is a free function, we need to make it a friend of the class or, more elegantly, implement it as a member function. However, the task specifies a free function, so the cleanest approach is to declare it as a friend inside the class template. The algorithm: check if `new_capacity` > current `capacity`. If not, return immediately. Otherwise, allocate a new dynamic array of size `new_capacity`, copy all existing elements from the old array to the new one (using a loop or `std::copy`), delete the old array, assign the new array pointer to `data`, and update `capacity` to `new_capacity`. Edge cases: `new_capacity` could be 0 (allocate a zero-sized array is technically valid but we should handle gracefully, though if `new_capacity` is 0 and capacity > 0, we treat it as no-op because 0 < capacity). Time complexity is O(size) for copying existing elements; space complexity is O(new_capacity) for the new array.

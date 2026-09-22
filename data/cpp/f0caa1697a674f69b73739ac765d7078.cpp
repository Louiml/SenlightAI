// Design a C++ class named `VectorList` that manages a dynamically allocated integer array with automatic resizing. The class must support inserting an element at a specified index (shifting subsequent elements right), removing an element at a specified index (shifting subsequent elements left), displaying all elements in order, and returning the current number of elements. The class should start with an initial capacity of 4, double its capacity when full during insert operations, and properly free memory in its destructor. All public methods must perform bounds checking for index-based operations—if an index is invalid (negative or greater than current size), the operation should be ignored with an appropriate error message printed to `std::cout`. Implement the class with `const` correct accessors where applicable, and provide a method `bool isEmpty()` that returns `true` when no elements are stored. The solution must be provided as a single self-contained header-like code block with no `main` function.
// The core of this task is building a dynamic array with two main algorithms: insertion at an index and removal at an index. For insertion at index `idx`: first validate `0 <= idx <= size` (allowing insert at the end). If the array is full, double the capacity and copy existing elements. Then shift all elements from `idx` onward one position to the right (starting from the last element) to make space, place the new value at `idx`, and increment `size`. Removal at index `idx`: validate `0 <= idx < size`; if valid, shift elements from `idx+1` to the end left by one position, then decrement `size`. No capacity shrink is required. Key edge cases include inserting at index 0 (shift entire array), inserting at index equal to size (append), removing the last element, and invalid indices like negative or `size` for removal. The display method should handle the empty array case gracefully. Time complexity: insertion at index is `O(n)` due to shifting, as is removal; resizing is `O(n)` when it occurs. Space complexity is `O(capacity)`, and the amortized cost per insertion remains `O(1)` due to doubling.
#include <iostream>

class VectorList {
private:
    int* data;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        int* newData = new int[capacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    VectorList() : size(0), capacity(4) {
        data = new int[capacity];
    }

    ~VectorList() {
        delete[] data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void insertAt(int index, int value) {
        if (index < 0 || index > size) {
            std::cout << "Invalid index." << std::endl;
            return;
        }
        if (size == capacity) {
            resize();
        }
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++size;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) {
            std::cout << "Invalid index." << std::endl;
            return;
        }
        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    void display() const {
        if (isEmpty()) {
            std::cout << "Array is empty." << std::endl;
            return;
        }
        std::cout << "Current array: ";
        for (int i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};
#include <cassert>
#include <sstream>

// Helper to capture display output
std::string captureDisplay(const VectorList& list) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    list.display();
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    VectorList list;
    assert(list.isEmpty());
    assert(list.getSize() == 0);

    // Insert at end
    list.insertAt(0, 10);
    list.insertAt(1, 20);
    list.insertAt(2, 30);
    assert(list.getSize() == 3);
    assert(captureDisplay(list) == "Current array: 10 20 30 \n");

    // Insert in middle
    list.insertAt(1, 15);
    assert(list.getSize() == 4);
    assert(captureDisplay(list) == "Current array: 10 15 20 30 \n");

    // Insert at front
    list.insertAt(0, 5);
    assert(list.getSize() == 5);
    assert(captureDisplay(list) == "Current array: 5 10 15 20 30 \n");

    // Remove from middle
    list.removeAt(2);
    assert(list.getSize() == 4);
    assert(captureDisplay(list) == "Current array: 5 10 20 30 \n");

    // Remove from front
    list.removeAt(0);
    assert(list.getSize() == 3);
    assert(captureDisplay(list) == "Current array: 10 20 30 \n");

    // Remove last
    list.removeAt(2);
    assert(list.getSize() == 2);
    assert(captureDisplay(list) == "Current array: 10 20 \n");

    // Invalid index insert
    list.insertAt(5, 99);
    assert(list.getSize() == 2);

    // Invalid index remove
    list.removeAt(5);
    assert(list.getSize() == 2);

    // Trigger multiple resizes
    for (int i = 0; i < 10; ++i) {
        list.insertAt(list.getSize(), i * 2);
    }
    assert(list.getSize() == 12);
    assert(captureDisplay(list) == "Current array: 10 20 0 2 4 6 8 10 12 14 16 18 \n");

    // Clear all
    while (!list.isEmpty()) {
        list.removeAt(list.getSize() - 1);
    }
    assert(list.isEmpty());
    assert(captureDisplay(list) == "Array is empty.\n");

    return 0;
}

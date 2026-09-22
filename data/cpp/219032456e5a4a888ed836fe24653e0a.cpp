Write a C++ function named `resizeDynamicArray` that takes a reference to a dynamically allocated integer pointer, the current logical size `n`, and the new desired size `m` (where `m > n`), and resizes the array so that it can hold `m` integers. The function must preserve all existing elements, deallocate the old memory, and update the caller's pointer to point to the new block. Additionally, implement a small helper function `appendInteger` that, given an array pointer reference, its current capacity, and the number of elements used so far, appends a new integer by doubling the capacity when necessary (starting from capacity 1). The task is to demonstrate manual memory management without using `std::vector` or `realloc`. You only need to provide the functions; no `main` is required in the solution, but you must ensure the code is self-contained with necessary headers.

#include <cassert>
#include <cstddef>

// The solution functions are assumed to be included above.

int main() {
    // Test resizeDynamicArray with increasing size.
    int* a = new int[2];
    a[0] = 10;
    a[1] = 20;
    resizeDynamicArray(a, 2, 5);
    assert(a[0] == 10);
    assert(a[1] == 20);
    // The rest of the new block is uninitialized, but we can write.
    a[2] = 30;
    a[3] = 40;
    a[4] = 50;
    assert(a[4] == 50);
    delete[] a;

    // Test appendInteger from empty.
    int* b = nullptr;
    std::size_t cap = 0;
    std::size_t used = 0;
    appendInteger(b, cap, used, 100);
    assert(cap == 1);
    assert(used == 1);
    assert(b[0] == 100);

    // Force a resize by appending second element.
    appendInteger(b, cap, used, 200);
    assert(cap == 2);
    assert(used == 2);
    assert(b[0] == 100);
    assert(b[1] == 200);

    // Force another resize by filling to capacity 2 then adding third.
    appendInteger(b, cap, used, 300);
    assert(cap == 4);
    assert(used == 3);
    assert(b[2] == 300);

    // Continue adding to verify all preserved.
    appendInteger(b, cap, used, 400);
    appendInteger(b, cap, used, 500);
    assert(cap == 8);
    assert(used == 5);
    assert(b[0] == 100 && b[1] == 200 && b[2] == 300 && b[3] == 400 && b[4] == 500);

    delete[] b;

    // Test resize with n=0 (edge case).
    int* c = nullptr;
    resizeDynamicArray(c, 0, 3);
    assert(c != nullptr);
    c[0] = 7;
    c[1] = 8;
    c[2] = 9;
    assert(c[2] == 9);
    delete[] c;

    return 0;
}

#include <cstddef>

// Resize a dynamically allocated integer array from logical size n to new capacity m (m > n).
// Preserves existing elements, deletes old allocation, and updates the caller's pointer.
void resizeDynamicArray(int*& arr, std::size_t n, std::size_t m) {
    if (m <= n) {
        return; // No resizing needed or invalid input; keep original.
    }

    // Allocate new larger block.
    int* newArr = new int[m];

    // Copy old elements (only up to n, assuming m >= n).
    for (std::size_t i = 0; i < n; ++i) {
        newArr[i] = arr[i];
    }

    // Deallocate old memory.
    delete[] arr;

    // Update caller's pointer.
    arr = newArr;
}

// Append an integer to a dynamically allocated array, growing capacity by doubling when full.
// 'arr' is the pointer (by reference), 'capacity' is the allocated size, 'used' is the logical size.
// After the call, 'used' is incremented and 'capacity' is updated if resized.
void appendInteger(int*& arr, std::size_t& capacity, std::size_t& used, int value) {
    // If there's no capacity yet, start with 1.
    if (capacity == 0) {
        capacity = 1;
        arr = new int[capacity];
    }

    // If the array is full, double the capacity.
    if (used == capacity) {
        std::size_t newCapacity = capacity * 2;
        resizeDynamicArray(arr, capacity, newCapacity);
        capacity = newCapacity;
    }

    // Place the new value and increment used count.
    arr[used] = value;
    ++used;
}

// The solution has two parts. The core `resizeDynamicArray` function allocates a new block of size `m`, copies the first `n` elements from the old array, deletes the old block, and reassigns the reference to point to the new block. The critical edge case is that the pointer must be passed by reference so the caller's pointer is updated; otherwise, only a local copy would change. Also, `m` must be strictly greater than `n` per the specification, but we can still handle the case by copying the minimum of `n` and `m` if needed (though not required). The `appendInteger` function uses exponential growth (doubling capacity) to amortize resizing cost—each element is copied at most a logarithmic number of times. Beginning with capacity 1, it doubles when full. The algorithm for `resizeDynamicArray` takes `O(m)` time (copying `n` elements) and uses `O(m)` extra space temporarily. The `appendInteger` function amortizes to `O(1)` per append on average, and `O(n)` for the worst-case single resizing step.

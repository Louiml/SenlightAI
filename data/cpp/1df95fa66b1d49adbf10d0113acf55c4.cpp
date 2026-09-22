Write a C++ function that takes an integer `n` and an array of integers (passed as a pointer along with its size) and returns a new dynamically allocated array containing the elements of the original array in reverse order. The function must not modify the original array, and it must handle the case where `n` is zero by returning `nullptr`. The function signature should be `int* reverseArray(const int* arr, int n)`, and the caller is responsible for freeing the returned memory.

The solution iterates over the input array from index `0` to `n-1` and writes each element into a new array such that the element at position `i` in the input goes to position `n-1-i` in the output. This can be done by counting forward in the input and backward in the output, or more simply by using a loop that copies `arr[i]` to `result[n-1-i]`. Edge cases: if `n == 0`, return `nullptr` (no allocation). If `n == 1`, the reverse is the same array, so the copy works naturally. The algorithm runs in `O(n)` time because each element is copied once, and uses `O(n)` auxiliary space for the new array (not counting the input array). It is important to allocate `new int[n]` and not use `malloc` to ensure proper C++ memory management. The input array is read-only, so the parameter is `const int*`.

#include <cstddef> // for nullptr

// Returns a new dynamically allocated array containing the elements of arr in reverse order.
// If n == 0, returns nullptr. The caller must delete[] the returned pointer.
int* reverseArray(const int* arr, int n) {
    if (n <= 0) {
        return nullptr;
    }
    int* result = new int[n];
    for (int i = 0; i < n; ++i) {
        result[n - 1 - i] = arr[i]; // Copy arr[i] into the mirrored position
    }
    return result;
}

#include <cassert>
#include <iostream>

// Declaration of the function under test
int* reverseArray(const int* arr, int n);

int main() {
    // Test 1: Normal case
    int a[] = {1, 2, 3, 4, 5};
    int* r1 = reverseArray(a, 5);
    assert(r1 != nullptr);
    for (int i = 0; i < 5; ++i) {
        assert(r1[i] == 5 - i); // 5,4,3,2,1
    }
    delete[] r1;

    // Test 2: Single element
    int b[] = {42};
    int* r2 = reverseArray(b, 1);
    assert(r2 != nullptr);
    assert(r2[0] == 42);
    delete[] r2;

    // Test 3: Zero length
    int* r3 = reverseArray(nullptr, 0);
    assert(r3 == nullptr);

    // Test 4: Even number of elements
    int c[] = {-1, 0, 1, 2};
    int* r4 = reverseArray(c, 4);
    assert(r4 != nullptr);
    assert(r4[0] == 2);
    assert(r4[1] == 1);
    assert(r4[2] == 0);
    assert(r4[3] == -1);
    delete[] r4;

    // Test 5: Negative numbers
    int d[] = {-3, -2, -1};
    int* r5 = reverseArray(d, 3);
    assert(r5 != nullptr);
    assert(r5[0] == -1);
    assert(r5[1] == -2);
    assert(r5[2] == -3);
    delete[] r5;

    // Test 6: Verify original array is not modified
    int e[] = {10, 20, 30};
    int* r6 = reverseArray(e, 3);
    assert(r6 != nullptr);
    assert(e[0] == 10 && e[1] == 20 && e[2] == 30);
    assert(r6[0] == 30 && r6[1] == 20 && r6[2] == 10);
    delete[] r6;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

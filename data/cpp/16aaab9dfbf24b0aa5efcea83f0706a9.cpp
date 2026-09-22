// Write a C++ function that takes ownership of a dynamically allocated integer array along with its size, and returns a new dynamically allocated array containing the elements of the original array in non-decreasing (ascending) order using only the selection sort algorithm. The function must handle arrays with negative numbers, duplicates, and arrays of size 0 or 1 correctly. The function signature should be `int* sortedArray(const int* arr, int n)` where the returned pointer must point to a newly allocated array that is a sorted copy; the original array is left unmodified. The function must be `const`-correct with respect to the input array (i.e., the input pointer should be `const int*`), and it must not use any standard sorting functions or modify the input. The caller is responsible for deleting the returned array.
#include <cassert>
#include <iostream>

// (The solution function is assumed to be declared above.)

int main() {
    // Test 1: Normal case with unsorted positive numbers.
    int a1[] = {3, 1, 2};
    int* s1 = sortedArray(a1, 3);
    assert(s1[0] == 1 && s1[1] == 2 && s1[2] == 3);
    // Original unchanged
    assert(a1[0] == 3 && a1[1] == 1 && a1[2] == 2);
    delete[] s1;

    // Test 2: Negative numbers and duplicates.
    int a2[] = {-5, 3, -1, -5, 2};
    int* s2 = sortedArray(a2, 5);
    assert(s2[0] == -5 && s2[1] == -5 && s2[2] == -1 && s2[3] == 2 && s2[4] == 3);
    delete[] s2;

    // Test 3: Already sorted array.
    int a3[] = {1, 2, 3};
    int* s3 = sortedArray(a3, 3);
    assert(s3[0] == 1 && s3[1] == 2 && s3[2] == 3);
    delete[] s3;

    // Test 4: Single element.
    int a4[] = {42};
    int* s4 = sortedArray(a4, 1);
    assert(s4[0] == 42);
    delete[] s4;

    // Test 5: Zero-size array returns nullptr.
    int* s5 = sortedArray(nullptr, 0);
    assert(s5 == nullptr);

    // Test 6: All identical elements.
    int a6[] = {7, 7, 7};
    int* s6 = sortedArray(a6, 3);
    assert(s6[0] == 7 && s6[1] == 7 && s6[2] == 7);
    delete[] s6;

    // Test 7: Large numbers and reverse order.
    int a7[] = {100, -200, 300, -400};
    int* s7 = sortedArray(a7, 4);
    assert(s7[0] == -400 && s7[1] == -200 && s7[2] == 100 && s7[3] == 300);
    delete[] s7;

    std::cout << "All tests passed." << std::endl;
    return 0;
}
#include <cstddef>  // for nullptr

// Return a newly allocated sorted copy of the input array using selection sort.
// The input array is not modified. Returns nullptr when n == 0.
int* sortedArray(const int* arr, int n) {
    if (n <= 0) {
        return nullptr;
    }

    // Allocate a copy to sort.
    int* copy = new int[n];
    for (int i = 0; i < n; ++i) {
        copy[i] = arr[i];
    }

    // Selection sort on the copy.
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (copy[j] < copy[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = copy[i];
            copy[i] = copy[minIndex];
            copy[minIndex] = temp;
        }
    }

    return copy;
}
// The solution approach is to implement selection sort on a *copy* of the input array. First, allocate a new array of size `n` and copy all elements from the input. Then, for each position `i` from 0 to `n-1`, find the index of the minimum element in the subarray from `i` to `n-1`, and swap it with the element at position `i`. This ensures that after the `i`-th iteration, the first `i+1` elements are in their final sorted positions. Edge cases: if `n` is 0, we should return `nullptr` (or a valid empty allocation, but returning `nullptr` is simplest and avoids undefined behavior for zero-size allocations; the caller must handle it). If `n` is 1, the array is trivially sorted and we just return a copy. Negative numbers and duplicates are handled naturally since comparisons use `<`. Complexity: time is O(n²) due to the nested loops (for each `i`, scanning `n-i` elements), and space is O(n) for the copy (excluding the returned array itself, which is O(n) as well).

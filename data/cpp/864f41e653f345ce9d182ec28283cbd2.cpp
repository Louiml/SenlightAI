// Write a C++ function named `findOddOccurrence` that takes a non-empty array of integers (passed as a pointer/reference to the first element) and its size, and returns the first integer in the array that appears an odd number of times. If no such integer exists (i.e., every element appears an even number of times), the function must return -1. The array may contain negative numbers, zeros, and duplicates. The function must be const-correct (i.e., it must not modify the input array), and should handle edge cases such as a single-element array or an array where every element appears an even count. The solution must use a simple nested-loop approach (no hash maps or sorting) to count occurrences for each distinct element in order, and return the first element that meets the odd-count condition.
// The algorithm iterates over each element in the array using an outer loop. For each element at index `i`, an inner loop counts how many times that exact value appears anywhere in the entire array (from index 0 to `arr_size-1`). After counting, it checks if the count is odd (i.e., `count % 2 != 0`). If yes, the function immediately returns the value at `arr[i]`. Since the outer loop processes elements in their original order, the first element that satisfies the odd-count condition is returned, which matches the required behavior. Edge cases: if the array has size 1, the count for that element is 1 (odd), so it returns that element. If all elements appear an even number of times (e.g., every element appears exactly twice), the loops complete without returning, and the function returns -1. Time complexity is O(n²) due to the nested loops, where n = arr_size. Space complexity is O(1) because only a few integer variables are used for counting and indexing; no additional data structures are allocated.
#include <vector>

// Returns the first integer in the array that appears an odd number of times,
// or -1 if every integer appears an even number of times.
// The input array is treated as read-only (const correctness).
int findOddOccurrence(const int arr[], int arr_size) {
    for (int i = 0; i < arr_size; ++i) {
        int count = 0;
        // Count occurrences of arr[i] in the whole array
        for (int j = 0; j < arr_size; ++j) {
            if (arr[i] == arr[j]) {
                ++count;
            }
        }
        // If the count is odd, return this value (first occurrence in order)
        if (count % 2 != 0) {
            return arr[i];
        }
    }
    // No element appears odd number of times
    return -1;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (same as in solution)
int findOddOccurrence(const int arr[], int arr_size);

int main() {
    // Test 1: Single element, odd count
    int arr1[] = {7};
    assert(findOddOccurrence(arr1, 1) == 7);

    // Test 2: All even counts -> returns -1
    int arr2[] = {4, 5, 4, 5};
    assert(findOddOccurrence(arr2, 4) == -1);

    // Test 3: First element with odd count is 2 (appears 3 times)
    int arr3[] = {2, 3, 5, 2, 2};
    assert(findOddOccurrence(arr3, 5) == 2);

    // Test 4: Negative numbers, odd count at value -1 appears 3 times
    int arr4[] = {-1, -1, -1, 0, 0};
    assert(findOddOccurrence(arr4, 5) == -1);

    // Test 5: Zeros and ones, value 0 appears odd count (5 times)
    int arr5[] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
    assert(findOddOccurrence(arr5, 9) == 0);

    // Test 6: Larger array with multiple odd counts, first in order is 3
    int arr6[] = {3, 9, 9, 3, 9, 9, 3};
    assert(findOddOccurrence(arr6, 7) == 3);

    // Test 7: Two distinct elements each appear twice -> -1
    int arr7[] = {10, 20, 10, 20};
    assert(findOddOccurrence(arr7, 4) == -1);

    // Test 8: Mixed with duplicates, value -5 appears once (odd)
    int arr8[] = {-5, 0, 0, 1};
    assert(findOddOccurrence(arr8, 4) == -5);

    // Test 9: All same element, even count -> -1
    int arr9[] = {2, 2, 2, 2};
    assert(findOddOccurrence(arr9, 4) == -1);

    // Test 10: Using vector to pass to function (array pointer)
    std::vector<int> vec = {1, 2, 1, 3, 2, 4};
    assert(findOddOccurrence(vec.data(), vec.size()) == 3); // 3 appears once

    return 0;
}

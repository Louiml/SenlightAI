/*
Write a C++ function named `sortIntegers` that takes a reference to a `std::vector<int>` and sorts its elements in ascending order using the bubble sort algorithm. The function must modify the vector in place and return `void`. The input vector may contain zero or more elements, including duplicates and negative numbers. The function should be robust to empty vectors (no operation performed) and should not use any standard library sorting functions (e.g., `std::sort`). Implement the bubble sort with an optimization: if a full pass over the array results in no swaps, the array is already sorted, so terminate early.
*/
#include <vector>

// Sort a vector of integers in ascending order using an optimized bubble sort.
// Modifies the input vector in place.
void sortIntegers(std::vector<int>& arr) {
    size_t n = arr.size();
    if (n <= 1) return; // Nothing to sort

    bool swapped;
    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;
        // Last i elements are already sorted, so skip them
        for (size_t j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no swaps occurred, the array is already sorted
        if (!swapped) break;
    }
}
#include <cassert>
#include <vector>

// Include the solution function here (for real usage, include the header)
void sortIntegers(std::vector<int>& arr);

int main() {
    // Test 1: Basic unsorted vector
    std::vector<int> v1 = {5, 2, 9, 1, 5, 6};
    sortIntegers(v1);
    assert((v1 == std::vector<int>{1, 2, 5, 5, 6, 9}));

    // Test 2: Already sorted
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    sortIntegers(v2);
    assert((v2 == std::vector<int>{1, 2, 3, 4, 5}));

    // Test 3: Reverse sorted
    std::vector<int> v3 = {9, 7, 5, 3, 1};
    sortIntegers(v3);
    assert((v3 == std::vector<int>{1, 3, 5, 7, 9}));

    // Test 4: Duplicates and negatives
    std::vector<int> v4 = {-3, 0, -1, 5, -3, 2, 0};
    sortIntegers(v4);
    assert((v4 == std::vector<int>{-3, -3, -1, 0, 0, 2, 5}));

    // Test 5: Single element
    std::vector<int> v5 = {42};
    sortIntegers(v5);
    assert((v5 == std::vector<int>{42}));

    // Test 6: Empty vector
    std::vector<int> v6;
    sortIntegers(v6);
    assert(v6.empty());

    // Test 7: Two elements out of order
    std::vector<int> v7 = {10, -1};
    sortIntegers(v7);
    assert((v7 == std::vector<int>{-1, 10}));

    // Test 8: All duplicates
    std::vector<int> v8 = {7, 7, 7, 7};
    sortIntegers(v8);
    assert((v8 == std::vector<int>{7, 7, 7, 7}));

    return 0;
}
// The solution uses the classic bubble sort algorithm: repeatedly traverse the vector from the beginning, comparing adjacent elements and swapping them if they are out of order. After each full pass, the largest element among the unsorted portion "bubbles" to the correct end position. We can reduce the inner loop’s range each outer iteration because the last `i` elements are already in final position. Additionally, we track a `swapped` flag; if no swaps occur during a pass, the vector is fully sorted and we can break early, improving best-case time to O(n) for already-sorted input. Edge cases: empty or single-element vectors require no work. Duplicates are handled naturally because the comparison uses `>` (strict), so equal elements are not swapped, preserving stability. Time complexity: average and worst-case O(n²), best-case O(n) with the flag optimization. Space complexity: O(1) auxiliary, using only a few local variables.

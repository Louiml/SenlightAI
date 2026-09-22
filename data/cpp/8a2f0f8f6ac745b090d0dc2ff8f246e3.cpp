Write a C++ function named `stableBubbleSort` that takes a `std::vector<double>` by reference and sorts it in ascending order using a **stable** bubble sort algorithm. The function must maintain the relative order of equal elements (i.e., if two elements are equal, they should remain in their original order in the sorted output). Additionally, the function must not modify the vector if it is empty or contains only one element (i.e., no unnecessary swaps). The function should be `void` and must work for vectors containing negative numbers, zeros, and positive numbers, including duplicate values. After sorting, the vector must be exactly sorted in non-decreasing order.
The bubble sort algorithm repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order. To make it **stable**, we use the condition `if (arr[j] > arr[j + 1])` (strictly greater) rather than `>=`, which ensures that equal elements are never swapped, preserving their original relative order. The outer loop runs `n-1` times, but we can optimize by noting that after each pass, the largest remaining element is placed at its final position. The inner loop compares adjacent elements up to the unsorted portion. For empty or single-element vectors, the loops naturally do nothing, so no special case is needed. Time complexity is \(O(n^2)\) in the worst and average cases, \(O(n)\) in the best case (when already sorted and we add an optimization flag), but for simplicity we can implement the classic version. Space complexity is \(O(1)\) auxiliary space (only swaps). Edge cases: duplicate values (stable sort preserves order), negative numbers, and vectors of size 0 or 1.
#include <vector>
#include <utility> // for std::swap

// Sorts a vector of doubles in ascending order using a stable bubble sort.
// Stability is maintained by only swapping when the left element is strictly greater.
void stableBubbleSort(std::vector<double>& arr) {
    const size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        // After each pass, the largest element in the unsorted part moves to its final position.
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
#include <cassert>
#include <vector>
#include <algorithm> // for std::equal, std::stable_sort (for comparison)

int main() {
    // Test 1: Basic sorting
    std::vector<double> v1 = {5.0, 2.0, 8.0, 1.0};
    stableBubbleSort(v1);
    assert((v1 == std::vector<double>{1.0, 2.0, 5.0, 8.0}));

    // Test 2: Empty vector
    std::vector<double> v2;
    stableBubbleSort(v2);
    assert(v2.empty());

    // Test 3: Single element
    std::vector<double> v3 = {42.0};
    stableBubbleSort(v3);
    assert((v3 == std::vector<double>{42.0}));

    // Test 4: Duplicates preserved in stable order
    std::vector<double> v4 = {3.0, 1.5, 3.0, 2.0, 1.5};
    std::vector<double> original4 = v4;
    stableBubbleSort(v4);
    // Stable sort would keep original order of equal elements, so compare against a stable sort
    std::vector<double> expected4 = original4;
    std::stable_sort(expected4.begin(), expected4.end());
    assert(v4 == expected4);

    // Test 5: All equal elements remain in original order
    std::vector<double> v5 = {7.7, 7.7, 7.7};
    stableBubbleSort(v5);
    assert((v5 == std::vector<double>{7.7, 7.7, 7.7}));

    // Test 6: Already sorted (worst-case for stability)
    std::vector<double> v6 = {-3.0, -2.0, 0.0, 1.0, 5.0};
    stableBubbleSort(v6);
    assert((v6 == std::vector<double>{-3.0, -2.0, 0.0, 1.0, 5.0}));

    // Test 7: Reverse sorted
    std::vector<double> v7 = {9.0, 4.0, 0.0, -1.0, -8.0};
    stableBubbleSort(v7);
    assert((v7 == std::vector<double>{-8.0, -1.0, 0.0, 4.0, 9.0}));

    // Test 8: Negative numbers and zeros
    std::vector<double> v8 = {0.0, -2.5, 0.0, -1.0, 3.2};
    stableBubbleSort(v8);
    assert((v8 == std::vector<double>{-2.5, -1.0, 0.0, 0.0, 3.2}));

    // Test 9: Large vector (100 elements) compared to stable_sort
    std::vector<double> v9;
    for (int i = 0; i < 100; ++i) v9.push_back((i * 37) % 100 * 0.5);
    std::vector<double> expected9 = v9;
    std::stable_sort(expected9.begin(), expected9.end());
    stableBubbleSort(v9);
    assert(v9 == expected9);

    return 0;
}

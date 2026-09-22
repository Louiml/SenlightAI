Write a C++ function named `bubbleSortDescending` that takes a non-empty `std::vector<int>` and sorts it in ascending order using the bubble sort algorithm, but with an optimization that stops early if no swaps occur in a full pass. The function must modify the vector in-place and return nothing (void). The input vector may contain duplicate values, large numbers (up to `INT_MAX`), and its size can be up to 100,000. The function should not use any standard sorting algorithms (e.g., `std::sort`), only manual comparisons and swaps. Ensure the function is robust for any valid integer vector, including already sorted, reverse-sorted, and single-element vectors.
The solution repeatedly traverses the vector from left to right, comparing adjacent elements and swapping them if they are out of order (i.e., if the left element is greater than the right). After each full pass, the largest unsorted element bubbles to its final position at the end, so the next pass can ignore the last `i` elements (where `i` is the pass count). The key optimization is tracking the number of swaps performed in each pass; if a pass results in zero swaps, the vector is already sorted and we break early. This reduces best-case time to O(n) for an already sorted vector. Worst and average case are O(n²) due to nested loops. Space complexity is O(1) auxiliary, since only a few integer variables are used. Edge cases include: single-element vector (trivially sorted, loop does not execute), already sorted vector (early break after first pass), and vectors with duplicate values (no special handling needed, swaps only occur when strictly greater). For correctness with large `n`, avoid integer overflow in loop counters (use `int` but n ≤ 100,000 is safe). The implementation uses `std::swap` for element exchange and applies `const` only where appropriate (e.g., loop bounds, but vector is non-const as it is modified).
#include <vector>
#include <utility> // for std::swap

// Sorts the given vector in ascending order using optimized bubble sort.
// The sort stops early if a pass completes with no swaps.
void bubbleSortDescending(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n <= 1) {
        return;
    }

    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no swaps, array is already sorted
        if (!swapped) {
            break;
        }
    }
}
#include <cassert>
#include <vector>
#include <algorithm> // for std::is_sorted

int main() {
    // Test 1: Reverse-sorted vector of 10 elements
    std::vector<int> v1 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    bubbleSortDescending(v1);
    assert(std::is_sorted(v1.begin(), v1.end()));

    // Test 2: Already sorted vector (early break optimization)
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    bubbleSortDescending(v2);
    assert(v2 == std::vector<int>({1, 2, 3, 4, 5}));

    // Test 3: Single element
    std::vector<int> v3 = {42};
    bubbleSortDescending(v3);
    assert(v3 == std::vector<int>({42}));

    // Test 4: Duplicate values
    std::vector<int> v4 = {5, 1, 5, 3, 1, 5};
    bubbleSortDescending(v4);
    assert(v4 == std::vector<int>({1, 1, 3, 5, 5, 5}));

    // Test 5: All identical elements
    std::vector<int> v5 = {7, 7, 7, 7};
    bubbleSortDescending(v5);
    assert(v5 == std::vector<int>({7, 7, 7, 7}));

    // Test 6: Large numbers and negative values
    std::vector<int> v6 = {INT_MAX, -1000000, 0, INT_MIN, 5000};
    bubbleSortDescending(v6);
    assert(std::is_sorted(v6.begin(), v6.end()));
    assert(v6[0] == INT_MIN && v6[4] == INT_MAX);

    // Test 7: Larger vector of 1000 elements scrambled
    std::vector<int> v7;
    for (int i = 0; i < 1000; ++i) v7.push_back((i * 37) % 1000);
    std::vector<int> expected7 = v7;
    std::sort(expected7.begin(), expected7.end());
    bubbleSortDescending(v7);
    assert(v7 == expected7);

    // Test 8: Two elements already sorted
    std::vector<int> v8 = {3, 9};
    bubbleSortDescending(v8);
    assert(v8 == std::vector<int>({3, 9}));

    // Test 9: Two elements reverse order
    std::vector<int> v9 = {9, 3};
    bubbleSortDescending(v9);
    assert(v9 == std::vector<int>({3, 9}));

    // Test 10: Empty vector (should not crash; size 0)
    std::vector<int> v10;
    bubbleSortDescending(v10);
    assert(v10.empty());

    return 0;
}

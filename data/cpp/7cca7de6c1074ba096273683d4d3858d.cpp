/*
Write a C++ function named `minMaxFromArray` that accepts a `const` reference to a `std::array<int, 5>` (or equivalently a C‑style array passed by pointer with size 5) and returns a `std::pair<int, int>` where the first element is the minimum value and the second element is the maximum value among all five integers. The function must scan the array from both ends simultaneously using two pointers (or indices) to reduce the number of comparisons compared to a naive linear scan. It must correctly handle duplicate values, negative numbers, and all values being equal. The function should have `const` correctness (the input array is not modified) and use no global variables. The caller should be able to directly compare the returned pair with `std::make_pair(min, max)`.
*/

#include <array>
#include <utility>
#include <climits>

// Returns {min, max} of a 5-element array by scanning from both ends.
// Uses two pointers (i from start, j from end) to process pairs.
std::pair<int, int> minMaxFromArray(const std::array<int, 5>& arr) {
    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    const int n = static_cast<int>(arr.size()); // 5

    // Process pairs from both ends toward the center.
    // For n=5, i goes 0,1,2. i=2 is the middle element (index 2) but we must not double-process it.
    for (int i = 0; i <= n / 2; ++i) {
        int j = n - 1 - i; // other end index
        // Update max with arr[i] and arr[j]
        if (arr[i] > maxVal) maxVal = arr[i];
        if (arr[j] > maxVal) maxVal = arr[j];
        // Update min with arr[i] and arr[j]
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[j] < minVal) minVal = arr[j];
    }

    return {minVal, maxVal};
}

#include <cassert>
#include <array>
#include <utility>

int main() {
    // Test 1: positive numbers
    std::array<int, 5> a1 = {3, 1, 4, 5, 2};
    auto r1 = minMaxFromArray(a1);
    assert(r1 == std::make_pair(1, 5));

    // Test 2: all negative
    std::array<int, 5> a2 = {-5, -1, -10, -3, -2};
    auto r2 = minMaxFromArray(a2);
    assert(r2 == std::make_pair(-10, -1));

    // Test 3: duplicates
    std::array<int, 5> a3 = {7, 7, 7, 7, 7};
    auto r3 = minMaxFromArray(a3);
    assert(r3 == std::make_pair(7, 7));

    // Test 4: mixed with zero and negative positive
    std::array<int, 5> a4 = {0, -4, 8, 0, -9};
    auto r4 = minMaxFromArray(a4);
    assert(r4 == std::make_pair(-9, 8));

    // Test 5: ascending order
    std::array<int, 5> a5 = {1, 2, 3, 4, 5};
    auto r5 = minMaxFromArray(a5);
    assert(r5 == std::make_pair(1, 5));

    // Test 6: descending order
    std::array<int, 5> a6 = {9, 7, 5, 3, 1};
    auto r6 = minMaxFromArray(a6);
    assert(r6 == std::make_pair(1, 9));

    // Test 7: large and small ints
    std::array<int, 5> a7 = {INT_MAX, INT_MIN, 0, -1, 1};
    auto r7 = minMaxFromArray(a7);
    assert(r7 == std::make_pair(INT_MIN, INT_MAX));

    // Test 8: single pair with center distinct
    std::array<int, 5> a8 = {-100, 100, -50, 50, 0};
    auto r8 = minMaxFromArray(a8);
    assert(r8 == std::make_pair(-100, 100));

    return 0;
}

// The classic two‑pointer min/max search reduces the number of comparisons from 2·(n−1) to roughly 1.5·n. For an array of size 5, we can process pairs from the outside in: index `i` from 0 upward and index `j = n-1-i` from the end. In each iteration we compare both elements against the current minimum and maximum. However, a simpler and correct approach for small fixed size (5) is to initialize `min` and `max` with the first element and then iterate from index 1 to 4, updating both. The code snippet provided contains a bug (the loop condition `i<=5/2` with `5/2=2` runs `i=0,1,2` but accesses `arr[4-i]` for `i=2` giving index `2`, so it actually covers indices 0..4 correctly but unnecessarily re‑checks index 2 twice). The task asks for a clean implementation that is robust and self‑contained. Edge cases: array of exactly 5 elements (no out‑of‑bounds), duplicates (no effect), all equal (returns same min and max), negative values (work fine with standard comparison). Time complexity: O(5) constant, space O(1). The two‑pointer method still gives constant time but we can also implement a standard single‑loop for clarity; the problem’s spirit is to scan from both ends, so we implement that correctly with `for (int i=0; i<=n/2; ++i)` and carefully handle the middle element when `n` is odd.

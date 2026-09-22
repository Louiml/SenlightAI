Given an array of `n` integers (where `n` is even and at least 2), write a C++ function `int maxSumOfHalf(const std::vector<int>& arr)` that sorts the array in non-increasing order (largest to smallest) and returns the sum of the first `n/2` elements (i.e., the sum of the largest half of the numbers). The function must not modify the original array. Assume the input vector is non-empty and its size is even. For example, for input `{5, 3, 8, 1}`, the sorted order is `{8, 5, 3, 1}`, and the sum of the first 2 elements is `8+5=13`. The function should be efficient for large `n` (up to 10^5 elements).
#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    // Basic case with positive numbers.
    assert(maxSumOfHalf({5, 3, 8, 1}) == 13);
    // All negative numbers.
    assert(maxSumOfHalf({-1, -2, -3, -4}) == -3); // largest half: -1 + (-2) = -3
    // Mixed positive and negative.
    assert(maxSumOfHalf({10, -20, 5, -1}) == 15); // largest half: 10 + 5 = 15
    // Duplicates.
    assert(maxSumOfHalf({7, 7, 7, 7}) == 14);
    // Zero values.
    assert(maxSumOfHalf({0, 0, 0, 0}) == 0);
    // Larger even-sized vector.
    assert(maxSumOfHalf({100, 1, 50, 49, 99, 2}) == 249); // sorted: 100,99,50,49,2,1 → 100+99+50=249
    // Already sorted descending.
    assert(maxSumOfHalf({9, 8, 7, 6}) == 17);
    // Already sorted ascending.
    assert(maxSumOfHalf({1, 2, 3, 4}) == 7);
    // Minimum even length (n=2).
    assert(maxSumOfHalf({-5, 5}) == 5);
    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>

// Returns the sum of the largest half of the array after sorting in descending order.
int maxSumOfHalf(const std::vector<int>& arr) {
    // Work on a copy to preserve the original array.
    std::vector<int> sorted = arr;
    // Sort in descending order (largest first).
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    // Sum the first half of the sorted array.
    const std::size_t half = sorted.size() / 2;
    return std::accumulate(sorted.begin(), sorted.begin() + half, 0);
}
// The core idea is straightforward: sort the input vector in descending order and sum the first `n/2` elements. Since the function must not modify the original array, we create a copy of the input vector, sort the copy, and then accumulate the first half. The comparator used is a descending order (larger first). Edge cases: because `n` is guaranteed even and at least 2, no issues with odd sizes or empty input. Negative numbers are handled naturally by the sort and sum. Time complexity is dominated by the sort: `O(n log n)`. Space complexity is `O(n)` due to the copied vector (excluding the input itself). The algorithm is simple and deterministic.

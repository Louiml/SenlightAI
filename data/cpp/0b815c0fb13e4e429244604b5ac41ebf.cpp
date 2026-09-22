Given an array of `n` integers (1 ≤ n ≤ 10^5), write a C++ function that finds the first adjacent pair whose absolute difference is strictly greater than 1. If such a pair exists, return a `std::pair<int,int>` with the 1-based indices of the two elements (the smaller index first). If no such pair exists, return `{-1, -1}`. The function should handle arrays of size 1 (where no adjacent pair exists) and must not modify the input array.
The solution is a straightforward linear scan of the array, checking each adjacent pair `(a[i-1], a[i])` for `abs(a[i-1] - a[i]) > 1`. The first pair that satisfies this condition is the answer. Since we return the first occurrence, we can iterate from `i=1` to `n-1` and break immediately upon finding the condition. If the loop completes without finding any such pair, we return `{-1, -1}`. Edge cases include an array of size 1 (no adjacent pairs), arrays where all consecutive differences are at most 1 (e.g., `[1,2,3]` or `[5,5,5]`), and arrays with multiple qualifying pairs where we must pick the first. The algorithm runs in O(n) time and uses O(1) auxiliary space since we only store indices and the input vector is read-only (passed by `const` reference).
#include <vector>
#include <cstdlib>
#include <utility>

// Finds the first adjacent pair (1-based indices) whose absolute difference > 1.
// Returns {index1, index2} with index1 < index2, or {-1, -1} if none exists.
std::pair<int, int> findFirstGapPair(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n < 2) {
        return {-1, -1};
    }
    for (int i = 1; i < n; ++i) {
        if (std::abs(arr[i - 1] - arr[i]) > 1) {
            return {i, i + 1}; // 1-based indices
        }
    }
    return {-1, -1};
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // No gaps: consecutive differences are 1
    std::vector<int> a1 = {1, 2, 3, 4};
    assert(findFirstGapPair(a1) == std::make_pair(-1, -1));

    // Single element
    std::vector<int> a2 = {5};
    assert(findFirstGapPair(a2) == std::make_pair(-1, -1));

    // Gap at the very beginning
    std::vector<int> a3 = {1, 5, 6, 7};
    assert(findFirstGapPair(a3) == std::make_pair(1, 2));

    // Gap in the middle, multiple gaps
    std::vector<int> a4 = {2, 2, 7, 10};
    assert(findFirstGapPair(a4) == std::make_pair(2, 3));

    // All equal values
    std::vector<int> a5 = {3, 3, 3};
    assert(findFirstGapPair(a5) == std::make_pair(-1, -1));

    // Gap at the end
    std::vector<int> a6 = {10, 9, 8, 0};
    assert(findFirstGapPair(a6) == std::make_pair(3, 4));

    // Negative values
    std::vector<int> a7 = {-2, -1, 5, 6};
    assert(findFirstGapPair(a7) == std::make_pair(2, 3));

    // Large gap with values far apart
    std::vector<int> a8 = {0, 100, 101};
    assert(findFirstGapPair(a8) == std::make_pair(1, 2));

    // No gap with strictly increasing by 1
    std::vector<int> a9 = {-10, -9, -8};
    assert(findFirstGapPair(a9) == std::make_pair(-1, -1));

    // Gap with negative and positive numbers
    std::vector<int> a10 = {-3, 0, 1};
    assert(findFirstGapPair(a10) == std::make_pair(1, 2));

    return 0;
}

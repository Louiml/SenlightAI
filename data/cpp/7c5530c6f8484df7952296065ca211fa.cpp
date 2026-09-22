Given a sequence of integers (size up to 100,000) that may contain duplicates and is not necessarily sorted, write a C++ function `longestStrictlyIncreasingSubsequenceLength(const std::vector<int>& numbers)` that returns the length of the longest strictly increasing subsequence (LIS). The subsequence must preserve the original order but can skip elements. The function must handle edge cases such as an empty vector (return 0), a single element (return 1), all equal elements (return 1), and strictly decreasing sequences (return 1). Use an efficient algorithm based on binary search.
#include <cassert>
#include <vector>

int main() {
    // Empty input
    assert(longestStrictlyIncreasingSubsequenceLength({}) == 0);

    // Single element
    assert(longestStrictlyIncreasingSubsequenceLength({5}) == 1);

    // Already strictly increasing
    assert(longestStrictlyIncreasingSubsequenceLength({1, 2, 3, 4, 5}) == 5);

    // Strictly decreasing
    assert(longestStrictlyIncreasingSubsequenceLength({5, 4, 3, 2, 1}) == 1);

    // Duplicate elements at the end (strictly increasing means no equals)
    assert(longestStrictlyIncreasingSubsequenceLength({1, 2, 2, 3, 3, 4}) == 4);

    // All duplicates
    assert(longestStrictlyIncreasingSubsequenceLength({7, 7, 7, 7}) == 1);

    // Mixed with negative numbers
    assert(longestStrictlyIncreasingSubsequenceLength({-5, -1, -10, 0, 2, -3, 1}) == 4); // -5,-1,0,2 or -5,-3,0,1

    // Large mixed sequence with known LIS length 6
    std::vector<int> vec = {10, 9, 2, 5, 3, 7, 101, 18};
    assert(longestStrictlyIncreasingSubsequenceLength(vec) == 4); // 2,3,7,101 or 2,5,7,101

    // Stress-like: 100k increasing elements
    std::vector<int> large(100000);
    for (int i = 0; i < 100000; ++i) large[i] = i;
    assert(longestStrictlyIncreasingSubsequenceLength(large) == 100000);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence in `numbers`.
// Uses a greedy binary-search approach: maintains `tails` where tails[k] is the
// smallest possible tail value of any increasing subsequence of length k+1.
// Time: O(n log n), Space: O(n).
int longestStrictlyIncreasingSubsequenceLength(const std::vector<int>& numbers) {
    std::vector<int> tails;
    tails.reserve(numbers.size());

    for (int value : numbers) {
        // Find the first position in tails where value <= tails[pos] (or end)
        auto it = std::lower_bound(tails.begin(), tails.end(), value);

        if (it == tails.end()) {
            // value is larger than all tails, so it can extend the longest LIS
            tails.push_back(value);
        } else {
            // Replace the smallest tail that is >= value to keep tails minimal
            *it = value;
        }
    }

    return static_cast<int>(tails.size());
}
// The classic greedy + binary search solution for LIS maintains an auxiliary array `tails` where `tails[k]` stores the smallest possible tail value of any increasing subsequence of length `k+1`. For each element `x` in the input, we find the first position in `tails` (from index 0 to current length-1) where the value is greater than or equal to `x` (using `lower_bound`). If no such position exists (i.e., `x` is greater than all tails), we append `x` to `tails`, increasing the current LIS length by 1. Otherwise, we replace that found position with `x`, ensuring that for any given length, we keep the smallest possible tail, which allows future elements to extend subsequences more easily. Because we replace the first element `>= x`, the sequence remains strictly increasing (no duplicates). The answer is the current length of `tails` at the end. The algorithm runs in O(n log n) time due to binary search for each element, and O(n) auxiliary space for `tails`. Edge cases: empty input returns 0; if the vector has one element, `tails` will get that element and length becomes 1; all equal elements: each new equal element replaces the first position (index 0), length remains 1. Duplicate handling: `lower_bound` finds a position with value `>= x`, so equal values replace, ensuring strict increase by not allowing equal values to extend.

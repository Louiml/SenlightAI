// Write a C++ function `int longestStrictlyIncreasingSubsequenceLength(const std::vector<int>& sequence)` that returns the length of the longest strictly increasing subsequence (LIS) of the given non-empty vector. A subsequence is obtained by deleting zero or more elements without changing the order of the remaining elements. The function must handle vectors of any size (including large up to 10^5) and values ranging from -10^9 to 10^9. It must work correctly with duplicate values (since strictly increasing means duplicates cannot be adjacent in the subsequence), negative numbers, and very large inputs without excessive time or memory overhead. The solution must use an efficient algorithm that runs in O(n log n) time, not the naive O(n^2) dynamic programming approach. The function should be const-correct and use appropriate standard library facilities.

// The problem reduces to finding the longest strictly increasing subsequence. The classic O(n log n) approach maintains a vector `tails` where `tails[k]` stores the smallest possible tail value of any increasing subsequence of length `k+1` found so far. Iterate through the input sequence. For each element `x`:
// - If `x` is strictly greater than the last element of `tails`, then we can extend the longest subsequence found so far, so append `x` to `tails`.
// - Otherwise, find the first element in `tails` that is greater than or equal to `x` (using `std::lower_bound`, which works for strictly increasing subsequences because we need to replace the smallest tail that is not less than `x`; using `lower_bound` ensures strictness because if `x` equals an existing tail, we replace it with `x`, which preserves strictness since the previous tail was not less than `x`). Replace that element with `x`, which keeps `tails` sorted and minimal.
// At the end, `tails.size()` is the length of the LIS.
//
// Edge cases: 
// - Single element: returns 1.
// - All duplicates: returns 1 because strictly increasing requires difference.
// - Strictly decreasing sequence: returns 1.
// - Already increasing: returns n.
// - Negative and positive mixed values: handled normally.
//
// Time complexity: O(n log n) due to binary search on `tails` for each element. Space complexity: O(n) in the worst case for `tails` (since in the best case of increasing sequence it is O(n)). The algorithm is correct for both strict and non-strict by choosing the appropriate binary search (`lower_bound` for strict, `upper_bound` for non-strict).

#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestStrictlyIncreasingSubsequenceLength(const std::vector<int>& sequence) {
    // tails[k] stores the smallest possible tail value of any increasing subsequence of length k+1.
    std::vector<int> tails;

    for (int value : sequence) {
        auto it = std::lower_bound(tails.begin(), tails.end(), value);
        if (it == tails.end()) {
            // value extends the longest subsequence found so far.
            tails.push_back(value);
        } else {
            // Replace the first tail that is >= value to keep tails minimal.
            *it = value;
        }
    }

    return static_cast<int>(tails.size());
}

#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Basic increasing sequence
    assert(longestStrictlyIncreasingSubsequenceLength({1, 2, 3, 4}) == 4);
    // Decreasing sequence
    assert(longestStrictlyIncreasingSubsequenceLength({5, 4, 3, 2, 1}) == 1);
    // Duplicates: strictly increasing cannot use equal values
    assert(longestStrictlyIncreasingSubsequenceLength({2, 2, 2, 2}) == 1);
    // Mixed with negatives and duplicates: LIS = [ -5, 0, 3, 7 ] length 4
    assert(longestStrictlyIncreasingSubsequenceLength({-5, 0, -1, 3, 0, 7, 2}) == 4);
    // Single element
    assert(longestStrictlyIncreasingSubsequenceLength({42}) == 1);
    // LIS can be non-contiguous: [1, 2, 3, 4] from [2,1,3,0,4]
    assert(longestStrictlyIncreasingSubsequenceLength({2, 1, 3, 0, 4}) == 3);
    // Large values
    assert(longestStrictlyIncreasingSubsequenceLength({-1000000000, 1000000000}) == 2);
    // Strictness: [1, 1, 2] LIS = [1,2] length 2, but [1,1] not allowed
    assert(longestStrictlyIncreasingSubsequenceLength({1, 1, 2}) == 2);
    // Already sorted increasing with gap
    assert(longestStrictlyIncreasingSubsequenceLength({-10, -5, 0, 5, 10}) == 5);
    // All same except one bigger: LIS = [5, 6] length 2
    assert(longestStrictlyIncreasingSubsequenceLength({5, 5, 5, 6}) == 2);
    // Random mix: LIS = [1, 2, 5, 6] length 4
    assert(longestStrictlyIncreasingSubsequenceLength({3, 1, 2, 5, 0, 6}) == 4);
}

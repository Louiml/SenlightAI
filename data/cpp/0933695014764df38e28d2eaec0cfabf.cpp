Write a C++ function `longestNonDecreasingRemovals` that takes a vector of integers and returns the minimum number of elements that must be removed so that the remaining sequence is non-decreasing (i.e., each element is less than or equal to the next). The function should handle empty input, negative values, duplicates, and large inputs. For example, given `[3, 1, 2, 1, 4]`, the longest non-decreasing subsequence is `[1, 2, 4]` (length 3), so the answer is `5 - 3 = 2`. The solution must compute this efficiently without brute-force enumeration of all subsequences.

// The problem reduces to finding the length of the longest non-decreasing subsequence (LNDS) and subtracting it from the total size. The snippet uses a classic patience-sorting approach with `lower_bound` on a vector that stores the smallest possible tail value for each subsequence length. For each input element, find the first position in the tail vector that is strictly greater than the current element; if the element is greater than or equal to all tails, it extends the longest subsequence, otherwise it replaces that position. The length of the tail vector after processing all elements is the LNDS length. The answer is `n - len`. Edge cases: empty vector returns 0; all duplicates yield LNDS = n, so answer 0; strictly decreasing input yields LNDS = 1, so answer n-1. Time complexity is O(n log n), space O(n) for the tail vector (but we only need O(n) worst-case; could be O(n) auxiliary). The solution uses `std::lower_bound` which is O(log n) per insertion.

#include <vector>
#include <algorithm>

// Returns the minimum number of elements to remove so that the remaining sequence
// is non-decreasing (each element <= next). Equivalent to total size minus length
// of the longest non-decreasing subsequence.
int longestNonDecreasingRemovals(const std::vector<int>& nums) {
    std::vector<int> tails;
    tails.reserve(nums.size());

    for (const int value : nums) {
        // lower_bound returns first position where tails[pos] >= value,
        // so we replace that position with value. This maintains non-decreasing.
        auto it = std::lower_bound(tails.begin(), tails.end(), value);
        if (it == tails.end()) {
            tails.push_back(value);
        } else {
            *it = value;
        }
    }

    // The length of tails is the length of the longest non-decreasing subsequence.
    // Removals needed = total size - LNDS length.
    return static_cast<int>(nums.size()) - static_cast<int>(tails.size());
}

#include <cassert>
#include <vector>

// Declaration of the function under test (must match the solution).
int longestNonDecreasingRemovals(const std::vector<int>& nums);

int main() {
    // Empty vector: no removals needed.
    assert(longestNonDecreasingRemovals({}) == 0);

    // Single element: already non-decreasing.
    assert(longestNonDecreasingRemovals({5}) == 0);

    // Already non-decreasing: no removals.
    assert(longestNonDecreasingRemovals({1, 2, 3, 4}) == 0);
    assert(longestNonDecreasingRemovals({1, 1, 1}) == 0);

    // Strictly decreasing: only one element can stay.
    assert(longestNonDecreasingRemovals({5, 4, 3, 2, 1}) == 4);

    // Mixed example from the description.
    assert(longestNonDecreasingRemovals({3, 1, 2, 1, 4}) == 2);

    // Duplicates and negatives.
    assert(longestNonDecreasingRemovals({-3, -1, -2, 0, -2}) == 2); // LNDS length 3

    // Large input (stress test with 10000 decreasing numbers).
    std::vector<int> big(10000);
    for (int i = 0; i < 10000; ++i) big[i] = 10000 - i;
    assert(longestNonDecreasingRemovals(big) == 9999);

    return 0;
}

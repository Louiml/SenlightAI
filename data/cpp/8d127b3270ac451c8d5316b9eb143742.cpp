// Write a C++ function that takes a vector of integers and returns the length of the longest strictly increasing subsequence (LIS). If the array is empty, the function should return 0. For a non-empty array, at least one element is always a valid increasing subsequence of length 1. The function must handle arrays with all equal elements, strictly increasing, strictly decreasing, and unsorted sequences. Your solution must be efficient for large inputs (up to 10^5 elements), and you must not use any global mutable state. Return an integer representing the maximum possible length of a strictly increasing subsequence.
// The task is the classic Longest Increasing Subsequence problem. A naive dynamic programming approach, as shown in the provided code snippet, runs in O(n²) time — for each element, it scans all previous elements to find the best predecessor. This is too slow for n up to 10^5. The optimal approach uses patience sorting with a binary search: maintain an array `tails` where `tails[i]` stores the smallest possible tail value for an increasing subsequence of length `i+1` encountered so far. For each element `x` in the input, we find the first position in `tails` that is not less than `x` (using `lower_bound`), and replace it with `x`. If no such position exists (i.e., `x` is greater than all tails), we append it, increasing the length. The final size of `tails` is the LIS length. This works because `tails` is always kept sorted, and strictly increasing subsequences require that when replacing, we do not place a value equal to an existing one (but since we only replace when `*it >= x` and we place `x` there, and `x` might equal `*it`, but that’s fine because we are not extending with equal values — `lower_bound` returns the first not less than, so if equals, we replace it with the same value, preserving subsequence length). Edge cases: empty input returns 0; all equal values — each new equal value replaces the first element, so `tails` size stays 1; decreasing sequence — each element replaces the first position, size stays 1. Time complexity: O(n log n) due to binary search for each of the n elements. Space complexity: O(n) for the `tails` vector.
#include <vector>
#include <algorithm>

// Computes the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& a) {
    if (a.empty()) {
        return 0;
    }
    std::vector<int> tails;
    for (int x : a) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

// Declaration of the function under test.
int longestIncreasingSubsequence(const std::vector<int>& a);

int main() {
    // Basic cases
    assert(longestIncreasingSubsequence({}) == 0);
    assert(longestIncreasingSubsequence({5}) == 1);
    assert(longestIncreasingSubsequence({1, 2, 3, 4}) == 4);
    assert(longestIncreasingSubsequence({4, 3, 2, 1}) == 1);
    assert(longestIncreasingSubsequence({1, 1, 1}) == 1);
    
    // Mixed sequences
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // 2,3,7,101
    assert(longestIncreasingSubsequence({0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15}) == 6); // 0,2,6,9,11,15
    assert(longestIncreasingSubsequence({-5, -1, -10, 0, 20, 15}) == 4); // -5,-1,0,20 or -10,0,15? Actually LIS is -5,-1,0,20 length 4
    assert(longestIncreasingSubsequence({3, 2, 5, 1, 7}) == 3); // 2,5,7 or 3,5,7
    
    // Large input to ensure O(n log n) doesn't blow up memory/time (just a sanity check)
    std::vector<int> large(100000);
    for (int i = 0; i < 100000; ++i) large[i] = i; // strictly increasing
    assert(longestIncreasingSubsequence(large) == 100000);
    
    return 0;
}

// Given a sequence of \( n \) distinct integers, write a C++ function that returns the length of the longest strictly increasing subsequence. The function must process the integers in the order they are given and may assume the input contains no duplicates. Use a binary-search–based approach that maintains the "tails" of candidate increasing subsequences: for each new value, either extend the longest subsequence if it is smaller than the smallest tail, or replace the first tail that is greater than or equal to the new value. The function should take a `const std::vector<int>&` and return an `int`.
The task is the classic Longest Increasing Subsequence (LIS) problem, but for strictly increasing subsequences with distinct elements. The algorithm maintains an array `tails` where `tails[i]` is the smallest possible tail value for an increasing subsequence of length `i+1`. For each incoming value `x`, we perform a binary search to find the first position where `tails[pos] >= x` (strictly greater than or equal to `x` works because all input values are distinct, so equality never occurs, but using `>=` is safe). If no such position exists (i.e., `x` is greater than all tails), we append `x` to the end, increasing the length. Otherwise, we replace that position with `x`, which keeps future possibilities open. The length of `tails` after processing all elements is the LIS length. Edge cases: empty input should return 0; a single element returns 1. Since values are distinct, no special tie-breaking is needed, but using `>=` in the binary search guarantees correctness even if duplicates were accidentally included (though the problem states they are distinct). Time complexity is \( O(n \log n) \) due to binary search for each of the \( n \) elements, and space complexity is \( O(n) \) for the tails array.
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
// The input is assumed to contain distinct integers.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    std::vector<int> tails;
    tails.reserve(nums.size());
    
    for (int x : nums) {
        // Find the first index in tails where tails[index] >= x.
        // Since input values are distinct, >= and > would behave identically here.
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);  // x extends the longest subsequence
        } else {
            *it = x;             // replace the tail with a smaller value
        }
    }
    
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

int main() {
    // Basic strictly increasing sequence
    assert(longestIncreasingSubsequence({1, 2, 3, 4, 5}) == 5);
    
    // Decreasing sequence, LIS length 1
    assert(longestIncreasingSubsequence({5, 4, 3, 2, 1}) == 1);
    
    // Mixed sequence with a known LIS of length 4
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    
    // Sequence requiring replacement to achieve optimal length
    assert(longestIncreasingSubsequence({3, 1, 2, 4}) == 3);
    
    // Single element
    assert(longestIncreasingSubsequence({7}) == 1);
    
    // Empty input
    assert(longestIncreasingSubsequence({}) == 0);
    
    // Distinct but with large range, LIS length 3
    assert(longestIncreasingSubsequence({0, 8, 4, 12, 2}) == 3);
    
    // Negative numbers mixed
    assert(longestIncreasingSubsequence({-5, -1, -7, 0, 3}) == 4);
    
    // Two-element increasing
    assert(longestIncreasingSubsequence({1, 2}) == 2);
    
    // Two-element decreasing
    assert(longestIncreasingSubsequence({2, 1}) == 1);
    
    return 0;
}

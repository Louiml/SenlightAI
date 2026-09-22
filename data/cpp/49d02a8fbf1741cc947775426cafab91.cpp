/*
Given a vector of distinct integers, write a C++ function `findLASLength` that returns the length of the Longest Alternating Subsequence (LAS), where a subsequence is alternating if its elements strictly alternate in direction (e.g., `a1 > a2 < a3` or `a1 < a2 > a3`). For any sequence of length 1 or 2, the entire sequence is trivially alternating. The function must handle an empty input vector by returning 0. Your solution should be efficient for vectors containing up to 1000 elements, so avoid an exponential-time recursive approach. Provide a function signature `int findLASLength(const std::vector<int>& nums)`. This function must be self-contained, using only standard headers, and must not rely on any external classes or global state.
*/

#include <vector>
#include <algorithm>

// Returns the length of the longest alternating subsequence.
int findLASLength(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int n = nums.size();
    std::vector<int> up(n, 1);   // length of LAS ending at i with last step up
    std::vector<int> down(n, 1); // length of LAS ending at i with last step down
    
    int maxLen = 1;
    
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                up[i] = std::max(up[i], down[j] + 1);
            } else if (nums[j] > nums[i]) {
                down[i] = std::max(down[i], up[j] + 1);
            }
        }
        maxLen = std::max(maxLen, std::max(up[i], down[i]));
    }
    
    return maxLen;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (assume it is included from above)
int findLASLength(const std::vector<int>& nums);

int main() {
    // Example 1 from the statement
    assert(findLASLength({1, 2, 3, 4}) == 2);
    
    // Example 2
    assert(findLASLength({3, 2, 1, 4}) == 3);
    
    // Example 3
    assert(findLASLength({1, 3, 2, 4}) == 4);
    
    // Empty input
    assert(findLASLength({}) == 0);
    
    // Single element
    assert(findLASLength({5}) == 1);
    
    // Two distinct elements always alternate
    assert(findLASLength({10, 20}) == 2);
    assert(findLASLength({20, 10}) == 2);
    
    // Decreasing sequence
    assert(findLASLength({5, 4, 3, 2, 1}) == 2);
    
    // Increasing sequence
    assert(findLASLength({1, 2, 3, 4, 5}) == 2);
    
    // Non-monotonic with multiple ups and downs
    assert(findLASLength({1, 2, 1, 2, 1}) == 5);
    
    // Longer random pattern
    assert(findLASLength({4, 1, 3, 2, 5, 0, 6}) == 6);
    
    return 0;
}

// The longest alternating subsequence can be found using a greedy dynamic programming approach that tracks the length of the longest alternating subsequence ending at each element with a given trend. For each new element `nums[i]`, we compare it with all previous elements `nums[j]` where `j < i`. If `nums[j] < nums[i]`, then we can extend any alternating subsequence ending at `j` that had a descending trend (i.e., the last step was downward) by adding `nums[i]` to get an ascending step. Similarly, if `nums[j] > nums[i]`, we extend a subsequence ending at `j` that had an ascending trend. We maintain two arrays: `up[i]` = length of longest alternating subsequence ending at `i` with the last step being upward (i.e., `nums[i-1] < nums[i]`), and `down[i]` = length with last step downward. Initialize both to 1 for each element because a single element is a valid subsequence of length 1. For each `i` from 1 to n-1, iterate over all `j < i` and update `up[i] = max(up[i], down[j] + 1)` if `nums[j] < nums[i]`, and `down[i] = max(down[i], up[j] + 1)` if `nums[j] > nums[i]`. The answer is the maximum value over all `up[i]` and `down[i]`. The time complexity is `O(n^2)` and space complexity is `O(n)` for the two arrays. Edge cases: empty input returns 0, single element returns 1, and sequences with equal adjacent elements (though the problem states distinct, the algorithm still works correctly since equal elements cannot extend an alternating pattern). For any sequence length 2, the result is always 2 because any two distinct elements alternate trivially.

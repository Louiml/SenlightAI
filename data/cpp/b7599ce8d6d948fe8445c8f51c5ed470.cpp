Given a vector of positive integers `arr` and a positive integer `k`, write a C++ function `partitionMaxSum` that partitions the array into contiguous segments, each of length at most `k` (a segment can be shorter than `k` only when it reaches the end of the array). For each segment, replace every element in that segment with the maximum value in that segment, then sum all the replaced values. The goal is to maximize this total sum by choosing the optimal partition boundaries. The function should return this maximum possible sum. For example, with `arr = {1,15,7,9,2,5,10}` and `k = 3`, one optimal partition is `[1,15,7]`, `[9]`, `[2,5,10]` giving sum = 15*3 + 9*1 + 10*3 = 84. The array length is at most 500, and `k` is at most the array length. All values are positive (you may assume non-negative). Write an efficient solution using dynamic programming with memoization.
// The problem is a classic dynamic programming on intervals (or prefixes). Let `dp[i]` represent the maximum sum achievable by partitioning the subarray `arr[i..n-1]`. The base case is `dp[n] = 0` (empty suffix). For index `i`, we consider forming the next segment starting at `i` with length `len` from 1 to `min(k, n-i)`. For each possible length, the segment's value is `len * max(arr[i..i+len-1])`, and the total for that choice is that product plus `dp[i+len]`. We take the maximum over all lengths. The answer is `dp[0]`. Edge cases include when `k = 1` (each element is its own segment, sum is sum of arr), when `k >= n` (one segment covering the whole array gives `n * max_element`), and when the array has length 1. Since we process each index once and at each index loop up to `k` times, the time complexity is O(n*k). The space complexity is O(n) for the memoization array.
#include <vector>
#include <algorithm>
#include <cstring>

// Returns the maximum sum after partitioning arr into contiguous segments of length at most k.
// Each segment's contribution is its length times the maximum element in that segment.
int partitionMaxSum(const std::vector<int>& arr, int k) {
    int n = static_cast<int>(arr.size());
    // dp[i] = max sum for suffix starting at index i
    std::vector<int> dp(n + 1, 0);
    
    // Process from the end towards the beginning
    for (int i = n - 1; i >= 0; --i) {
        int maxVal = 0;
        int best = 0;
        // Try segment lengths 1..min(k, remaining elements)
        for (int len = 1; len <= k && i + len <= n; ++len) {
            maxVal = std::max(maxVal, arr[i + len - 1]);
            int candidate = maxVal * len + dp[i + len];
            best = std::max(best, candidate);
        }
        dp[i] = best;
    }
    
    return dp[0];
}
#include <cassert>
#include <vector>

int partitionMaxSum(const std::vector<int>& arr, int k);

int main() {
    // Example from description
    assert(partitionMaxSum({1,15,7,9,2,5,10}, 3) == 84);
    
    // Single element
    assert(partitionMaxSum({5}, 1) == 5);
    assert(partitionMaxSum({5}, 5) == 5);
    
    // All elements same
    assert(partitionMaxSum({3,3,3}, 2) == 9);
    
    // k = 1 means each element is its own segment
    assert(partitionMaxSum({1,2,3,4}, 1) == 10);
    
    // k >= n means one whole segment
    assert(partitionMaxSum({4,1,2}, 5) == 12);
    
    // Simple case
    assert(partitionMaxSum({2,1,3}, 2) == 8);
    
    // Non-trivial case
    assert(partitionMaxSum({1,4,1,5,7,3,6,1,9,9,3}, 4) == 83);
    
    // Edge: all zeros? Use non-negative assumption
    assert(partitionMaxSum({0,0,0}, 2) == 0);
    
    // Larger k than array size handled
    assert(partitionMaxSum({7,2,9}, 10) == 27);
    
    return 0;
}

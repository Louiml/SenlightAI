// Write a C++ function `longestPositiveRun` that takes a non-empty vector of integers and returns the length of the longest contiguous subarray whose sum is strictly greater than zero. If no such subarray exists, return `0`. The input may contain negative numbers, zeros, and positive numbers in any order, and the vector may have only one element. For example, `{-2, 1, -3, 4, -1, 2, 1, -5, 4}` has a longest positive-sum run of length 3 (the subarray `{4, -1, 2}` sums to 5, and `{1, -3, 4}` sums to 2, but `{4, -1, 2, 1}` sums to 6 and has length 4; however the longest run with positive sum is `{4, -1, 2, 1}` which sums to 6, length 4 — the check is the maximum length among all contiguous subarrays with sum > 0, so the answer should be 4). Ensure your function is named `longestPositiveRun`, takes `const std::vector<int>&`, and returns an `int`. The function must handle all edge cases including all-negative arrays, arrays with zeros, and single-element arrays.

// The solution uses a sliding window approach combined with prefix sums to find the longest subarray with a positive sum. A brute-force O(n²) approach would test every subarray, but we can do it in O(n) using a monotonic queue or a simpler two-pointer method with prefix sums. However, a simpler robust approach is to iterate through all starting indices and expand the window while keeping track of the maximum length with sum > 0; that is O(n²) in worst-case. For an O(n) solution, maintain a prefix sum array and a map from prefix sum to the earliest index where that sum occurs. For each right endpoint, we want the smallest left index such that prefixSum[right] - prefixSum[left] > 0, i.e., prefixSum[left] < prefixSum[right]. To find the earliest such left, we can keep a running minimum of prefix sums seen so far. Specifically, iterate from left to right computing prefix sums, and maintain the minimum prefix sum encountered and its index. For each position, the longest valid subarray ending at this position starts right after the index of the smallest prefix sum seen so far (if that smallest prefix sum is strictly less than current prefix sum). This yields O(n) time and O(1) extra space. Edge cases: all negative numbers → no positive-sum subarray → return 0; single positive → length 1; zeros present → they can be included if the sum stays > 0. Space complexity is O(1), time O(n).

#include <vector>
#include <algorithm>

// Returns the length of the longest contiguous subarray whose sum is > 0.
// If no such subarray exists, returns 0.
int longestPositiveRun(const std::vector<int>& nums) {
    int n = nums.size();
    long long prefix = 0;
    long long minPrefix = 0;
    int minIndex = -1; // index before the start of the subarray
    int best = 0;
    
    for (int i = 0; i < n; ++i) {
        prefix += nums[i];
        // If current prefix is greater than the smallest prefix seen,
        // the subarray from (minIndex+1) to i has positive sum.
        if (prefix > minPrefix) {
            best = std::max(best, i - minIndex);
        }
        // Update minimum prefix if we see a smaller one
        if (prefix < minPrefix) {
            minPrefix = prefix;
            minIndex = i;
        }
    }
    return best;
}

#include <cassert>
#include <vector>

int longestPositiveRun(const std::vector<int>&);

int main() {
    // Basic positive run
    assert(longestPositiveRun({1, 2, 3}) == 3);
    // Mixed with negatives, longest is entire array? sum=1-2+3=2>0 length3
    assert(longestPositiveRun({1, -2, 3}) == 3);
    // Longest positive run not the whole array
    assert(longestPositiveRun({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 4);
    // All negative -> no positive subarray
    assert(longestPositiveRun({-1, -2, -3}) == 0);
    // Single positive
    assert(longestPositiveRun({5}) == 1);
    // Single negative
    assert(longestPositiveRun({-5}) == 0);
    // Zeros and positives
    assert(longestPositiveRun({0, 0, 2, 0}) == 3); // sum=2>0, length 3
    // Alternating with equal prefix, still works
    assert(longestPositiveRun({1, -1, 1, -1, 1}) == 1); // any single 1 gives length1, longer sums become zero or negative
    // Large mix with negative at start
    assert(longestPositiveRun({-100, 1, 2, 3, -50, 10}) == 3); // subarray {1,2,3} sum=6 length3; {1,2,3,-50,10} sum=-34 not; {10} length1
    return 0;
}

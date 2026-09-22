/*
Given a sequence of `n` integers (where `n` is at least 1) read from standard input, write a C++ function named `maximumSubarraySum` that accepts a `const std::vector<long long>&` and returns the maximum possible sum of a contiguous non-empty subarray (also known as the maximum subarray sum or Kadane’s algorithm). The function must handle large positive and negative values (up to 64-bit range) and should not modify the input vector. The result must be returned as a `long long`. The function may assume the input is non-empty, but must still be correct for arrays with all negative numbers (returning the largest negative value) and arrays with all positive numbers (returning the total sum). You are not required to output the subarray itself, only its maximum sum.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum sum of any contiguous non-empty subarray.
long long maximumSubarraySum(const std::vector<long long>& arr) {
    long long currentSum = arr[0];
    long long bestSum = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        currentSum = std::max(arr[i], currentSum + arr[i]);
        bestSum = std::max(bestSum, currentSum);
    }
    return bestSum;
}

#include <cassert>
#include <vector>
#include <cstdint>

long long maximumSubarraySum(const std::vector<long long>& arr);

int main() {
    // Single element
    assert(maximumSubarraySum({5}) == 5);
    assert(maximumSubarraySum({-3}) == -3);

    // All positive
    assert(maximumSubarraySum({1, 2, 3, 4}) == 10);

    // All negative
    assert(maximumSubarraySum({-5, -1, -10}) == -1);

    // Mixed
    assert(maximumSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
    assert(maximumSubarraySum({1, -2, 3, 4, -1, 2, -3}) == 8);

    // Includes zero
    assert(maximumSubarraySum({0, -1, 0, 3}) == 3);

    // Large values within long long
    assert(maximumSubarraySum({1000000000000LL, -1, 1000000000000LL}) == 1999999999999LL);

    return 0;
}

// The solution uses Kadane’s algorithm, which is a dynamic programming approach that processes the array from left to right while maintaining two values: `currentSum` (the maximum sum of a subarray ending at the current position) and `bestSum` (the maximum sum of any subarray seen so far). For each element, `currentSum` is updated as the maximum of the element itself or `currentSum + element`, because starting a new subarray at the current position may be better than extending the previous one if the previous sum is negative. Then `bestSum` is updated to the maximum of its previous value and `currentSum`. This approach works for all integer arrays including all-negative arrays (where the answer is the largest negative element) and all-positive arrays (where the answer is the total sum). The algorithm runs in O(n) time and uses O(1) auxiliary space. Edge cases include a single-element array, all negative numbers, and large sums that fit within `long long`.

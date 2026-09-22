Write a C++ function that takes a non-empty vector of integers (which may contain negative numbers, zeros, and duplicates) and returns the smallest possible positive starting value such that, when you repeatedly add each element of the vector in order to a running sum (beginning with this starting value), the running sum never drops below 1 at any point (including after the last element). The starting value itself must be at least 1. If the vector is empty, the function should handle it gracefully by returning 1. The function should be named `minimumStartValueForPositiveRunningSum` and should accept the vector by const reference. You may assume the vector contains valid `int` values, but the running sum could theoretically overflow if the input is extreme; for simplicity, assume the input is chosen so that all intermediate sums fit within a standard `long long`.

#include <cassert>
#include <vector>

// Assume the function from the solution is declared above.

int main() {
    // Example: [-3, 2, -3, 4, 2] -> running sums with start 5: 5, 2, 4, 1, 3, 5 (never below 1)
    assert(minimumStartValueForPositiveRunningSum({-3, 2, -3, 4, 2}) == 5);

    // All positive -> start 1
    assert(minimumStartValueForPositiveRunningSum({1, 2, 3}) == 1);

    // Single negative -> start = -min + 1
    assert(minimumStartValueForPositiveRunningSum({-7}) == 8);

    // Single positive -> start 1
    assert(minimumStartValueForPositiveRunningSum({5}) == 1);

    // Mixed with zero
    assert(minimumStartValueForPositiveRunningSum({0, -1, 0, -2}) == 4);

    // Empty vector -> returns 1
    std::vector<int> empty;
    assert(minimumStartValueForPositiveRunningSum(empty) == 1);

    // Large negative prefix with later positives
    assert(minimumStartValueForPositiveRunningSum({-5, 10, -5}) == 6);

    // All zeros
    assert(minimumStartValueForPositiveRunningSum({0, 0, 0}) == 1);

    // Duplicates
    assert(minimumStartValueForPositiveRunningSum({-2, -2, -2}) == 7);

    // Positive then negative
    assert(minimumStartValueForPositiveRunningSum({3, -5, 3}) == 3);

    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Given a vector of integers, return the smallest positive starting value such
// that the cumulative sum never drops below 1 after each addition.
int minimumStartValueForPositiveRunningSum(const std::vector<int>& nums) {
    long long currentSum = 0;
    long long minPrefixSum = 0; // prefix of length 0 has sum 0

    for (int num : nums) {
        currentSum += num;
        minPrefixSum = std::min(minPrefixSum, currentSum);
    }

    // We need startingValue + minPrefixSum >= 1, so startingValue >= 1 - minPrefixSum.
    // Also, startingValue must be at least 1.
    long long required = 1LL - minPrefixSum;
    if (required < 1LL) {
        return 1;
    }
    return static_cast<int>(required);
}

// The key observation is that the running sum after processing the first `k` elements, given a starting value `S`, equals `S + prefixSum(k)`, where `prefixSum(k)` is the sum of the first `k` elements of the vector. The constraint is that this value must be at least 1 for every `k` (including `k=0`, which is just `S` itself). Therefore, we need `S >= 1 - prefixSum(k)` for all `k`. The most restrictive condition occurs at the minimum prefix sum across all prefixes. Let `minPrefix` be the smallest prefix sum encountered (including the empty prefix, which is 0). Then the smallest valid `S` is `max(1, 1 - minPrefix)`. This matches the logic: compute the running total, track the minimum total seen so far, then compute the answer. Edge cases: all positive numbers → min prefix is 0 → answer 1. Negative numbers causing a very low prefix → answer is larger. If the vector is empty, min prefix is 0, answer 1. The algorithm runs in O(n) time, where n is the number of elements, and uses O(1) extra space. We use a `long long` accumulator to avoid overflow from the sum of the vector, although the task states inputs are safe.

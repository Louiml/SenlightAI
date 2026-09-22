Write a C++ function named `maxEarningsFromNegativeOffers` that takes a vector of integers representing daily earnings (positive values are profits, negative values are losses) and an integer `k` representing the maximum number of losses the person can choose to offset. The function should return the maximum total absolute value of the chosen losses, but only losses (negative numbers) can be selected, at most `k` of them, and they must be the smallest (most negative) values available. Read the input from standard input as in the snippet: first two integers `n` and `m` (where `m` is the limit) followed by `n` integers. The function should ignore positive values entirely, and if fewer than `m` negatives exist, select all negatives. Return the absolute sum of the selected negative numbers.

The solution first sorts the vector in ascending order so that the most negative numbers come first. Then we iterate through the sorted vector, but only up to the first `m` elements and only while the current element is negative (≤ 0). Since sorting places all negatives before positives, stopping at the first non-positive ensures we never pick a positive or zero (zero adds nothing but can be safely included without changing the sum, though the snippet stops at `v[i] <= 0` so it includes zeros). For each qualifying negative, we add its value (which is negative) to a running sum, then at the end return the absolute value of that sum. Edge cases: if `m` is 0, the loop condition `i < m` fails immediately, returning 0. If there are no negatives, the loop condition `v[i] <= 0` fails on the first element (which would be positive after sorting), returning 0. If there are fewer negatives than `m`, the loop naturally ends when the list ends. Complexity is O(n log n) due to sorting, and O(1) auxiliary space beyond the input vector.

#include <vector>
#include <algorithm>
#include <cstdlib>

// Given a vector of integers and a limit k, compute the sum of absolute values
// of the k most negative numbers (or all negatives if fewer than k exist).
// Positive numbers are ignored. Returns the absolute sum of selected negatives.
int maxEarningsFromNegativeOffers(std::vector<int> earnings, int k) {
    // Sort ascending so most negative values come first.
    std::sort(earnings.begin(), earnings.end());

    int sum = 0;
    // Iterate up to k elements, but stop early if we encounter a non-negative.
    for (int i = 0; i < k && i < static_cast<int>(earnings.size()) && earnings[i] <= 0; ++i) {
        sum += earnings[i];  // earnings[i] is non-positive, so sum is non-positive.
    }

    // Return absolute value of the sum (i.e., total losses offset).
    return std::abs(sum);
}

#include <cassert>
#include <vector>

// Prototype of the function under test (declared here for clarity).
int maxEarningsFromNegativeOffers(std::vector<int> earnings, int k);

int main() {
    // Basic mixed values with positive and negative.
    assert(maxEarningsFromNegativeOffers({3, -2, 5, -7, 1}, 2) == 9); // -7 and -2 sum to -9
    // Only one negative, limit larger than count.
    assert(maxEarningsFromNegativeOffers({4, 6, -3, 8}, 5) == 3);
    // All positive, nothing to offset.
    assert(maxEarningsFromNegativeOffers({1, 2, 3, 4}, 3) == 0);
    // All negative, limited by k.
    assert(maxEarningsFromNegativeOffers({-1, -2, -3, -4}, 2) == 3);
    // Limit zero.
    assert(maxEarningsFromNegativeOffers({-10, -20}, 0) == 0);
    // Mixed with zero, zeros ignored because they don't change sum.
    assert(maxEarningsFromNegativeOffers({0, -5, 0, -10, 0}, 1) == 5);
    // Duplicate negatives, pick smallest two.
    assert(maxEarningsFromNegativeOffers({-2, -2, -2, 5}, 2) == 4);
    // Single element negative.
    assert(maxEarningsFromNegativeOffers({-42}, 1) == 42);
    // Single element positive.
    assert(maxEarningsFromNegativeOffers({42}, 1) == 0);
    // Unsorted input, should still work correctly.
    assert(maxEarningsFromNegativeOffers({5, -8, 2, -1, -3, 7}, 3) == 12); // -8, -3, -1 sum to -12

    return 0;
}

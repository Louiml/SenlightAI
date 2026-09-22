Write a C++ function `bool canPartitionEvenly(const int arr[], int n)` that determines whether a given array of positive integers can be partitioned into two subsets with equal sums. The function should return `true` if such a partition exists, and `false` otherwise. The array may contain duplicate values and zero or more elements; handle the edge case where the array is empty (return `true`, since an empty set can be split into two empty subsets with equal sum 0). The total sum of the array is guaranteed not to exceed 10,000 so that dynamic programming on the sum is feasible. Do not use any input/output inside the function—only return the boolean result.

#include <cassert>

int main() {
    // Basic positive case
    int arr1[] = {3, 1, 5, 9, 12};
    assert(canPartitionEvenly(arr1, 5) == true);

    // Odd total sum -> false
    int arr2[] = {1, 2, 3, 4, 5};
    assert(canPartitionEvenly(arr2, 5) == false);

    // Empty array -> true
    assert(canPartitionEvenly(nullptr, 0) == true);

    // Single element -> false unless zero
    int arr3[] = {5};
    assert(canPartitionEvenly(arr3, 1) == false);

    int arr4[] = {0};
    assert(canPartitionEvenly(arr4, 1) == true);

    // Duplicate values
    int arr5[] = {1, 1, 2, 2};
    assert(canPartitionEvenly(arr5, 4) == true);

    // Already equal halves
    int arr6[] = {1, 2, 3, 4, 5, 5};
    assert(canPartitionEvenly(arr6, 6) == true);

    // Large values but bounded sum
    int arr7[] = {1000, 2000, 3000, 4000};
    assert(canPartitionEvenly(arr7, 4) == true);

    // Case where only zero-sum split works
    int arr8[] = {0, 0, 0};
    assert(canPartitionEvenly(arr8, 3) == true);

    // Case with many duplicates
    int arr9[] = {2, 2, 2, 2, 2, 2};
    assert(canPartitionEvenly(arr9, 6) == true);

    return 0;
}

#include <vector>

// Determine if the array can be partitioned into two subsets with equal sums.
// The array contains non-negative integers, and n is its length.
bool canPartitionEvenly(const int arr[], int n) {
    int totalSum = 0;
    for (int i = 0; i < n; ++i) {
        totalSum += arr[i];
    }

    // If total sum is odd, equal split is impossible.
    if (totalSum % 2 != 0) {
        return false;
    }

    int target = totalSum / 2;
    std::vector<bool> dp(target + 1, false);
    dp[0] = true;

    // Process each element; iterate backwards to avoid reusing an element.
    for (int i = 0; i < n; ++i) {
        int value = arr[i];
        for (int sum = target; sum >= value; --sum) {
            if (dp[sum - value]) {
                dp[sum] = true;
            }
        }
    }

    return dp[target];
}

// The problem is equivalent to the "Partition Equal Subset Sum" problem. If the total sum `S` is odd, no partition into two equal integer sums exists, so return `false`. If `S` is even, we need to determine whether a subset of the array sums to exactly `S/2`. Use a 1D boolean dynamic programming array `dp` of size `S/2 + 1`, where `dp[i]` is `true` if a subset of the processed elements sums to `i`. Initialize `dp[0] = true` and all other entries `false`. For each element `value` in the array, iterate `i` from `S/2` down to `value` and set `dp[i] = dp[i] || dp[i - value]`. This reverse iteration ensures each element is used at most once (0/1 knapsack style). If `dp[S/2]` is `true` after processing all elements, the partition exists. Edge cases: empty array (sum 0, return `true`), sum odd (return `false`), and large numbers must be handled by the bounded sum. Time complexity is `O(n * S/2)`, and space complexity is `O(S/2)`.

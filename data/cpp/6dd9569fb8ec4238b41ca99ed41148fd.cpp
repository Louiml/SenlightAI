// Write a C++ function `canPartitionEqualSubset` that takes a reference to a `const std::vector<int>&` and returns a `bool` indicating whether the array can be partitioned into two non-empty subsets with equal sum. The function must handle empty and single-element arrays, and must reject any input whose total sum is odd (since odd sums cannot be equally divided). The solution must use dynamic programming with a 1D boolean array where `dp[j]` indicates whether a subset sum of `j` can be formed using elements processed so far. Your implementation should be efficient in both time and space, and must handle positive integers only (assume all `nums[i]` are > 0).
// The core idea is to first compute the total sum of all elements. If this sum is odd, an equal partition is impossible because the sum must be divisible by 2 to split into two equal integer parts. If the sum is even, the target subset sum becomes `sum / 2`. The problem then reduces to checking whether there exists any subset of the array whose sum equals exactly `target`. We solve this using a 1D boolean dynamic programming array of size `target+1`, initially all `false`. We set `dp[0] = true` (empty subset sums to 0). For each element `num`, we iterate `j` from `target` down to `num` (inclusive) to avoid using the same element multiple times (0/1 knapsack). For each such `j`, we update `dp[j] = dp[j] || dp[j - num]`. The downward iteration ensures that when we update `dp[j]`, `dp[j-num]` still reflects the state before we considered the current element. After processing all elements, the answer is `dp[target]`. Edge cases: If the array is empty or has only one element, the total sum is either 0 (trivially partitionable into two empty subsets? But the problem says non-empty subsets, so we must decide: for empty input, return `false` because you cannot form two non-empty subsets; for single element, total sum is that element, if it is 0? But we assume positive integers, so sum > 0, odd/even? For single positive element, sum is positive odd or even; if sum is even and target equals that element, but you cannot form a subset of sum equal to the whole array because you'd need to use the element itself and leave nothing else — so you'd need another subset with sum 0, but all numbers positive, so impossible. So we can simply check if `nums` is empty → return `false`; if total sum odd → `false`; then run DP. Also, if target is 0 (only when sum is 0, which cannot happen with positive integers unless empty), we return `false` because non-empty subsets required. We will explicitly handle empty input by returning `false`. Time complexity is O(n * target) where n is the number of elements and target is sum/2, and space complexity is O(target). Since target can be large (up to sum/2), this is acceptable for typical constraints. The solution correctly handles duplicates and any order.
#include <vector>
#include <numeric>

// Determine if a vector of positive integers can be partitioned into two subsets with equal sum.
bool canPartitionEqualSubset(const std::vector<int>& nums) {
    // An empty container cannot be split into two non-empty subsets.
    if (nums.empty()) {
        return false;
    }
    
    // Compute total sum of all elements.
    int total_sum = 0;
    for (int value : nums) {
        total_sum += value;
    }
    
    // If total sum is odd, equal partition is impossible.
    if (total_sum % 2 != 0) {
        return false;
    }
    
    int target = total_sum / 2;
    
    // If target is 0 (only possible when total_sum is 0, but nums is non-empty with positive ints, so this won't happen),
    // but we keep a guard for completeness.
    if (target == 0) {
        return false;
    }
    
    // dp[j] = true if a subset sum of j is achievable using processed elements.
    std::vector<bool> dp(target + 1, false);
    dp[0] = true; // empty subset always achievable.
    
    // Process each number in 0/1 knapsack fashion.
    for (int value : nums) {
        // Iterate downward to avoid reusing the same element.
        for (int j = target; j >= value; --j) {
            if (dp[j - value]) {
                dp[j] = true;
            }
        }
    }
    
    return dp[target];
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Example 1: [1, 5, 11, 5] -> sum=22, target=11, subset {11} and {5,5,1} works.
    std::vector<int> nums1 = {1, 5, 11, 5};
    assert(canPartitionEqualSubset(nums1) == true);

    // Example 2: [1, 2, 3, 5] -> sum=11 odd, impossible.
    std::vector<int> nums2 = {1, 2, 3, 5};
    assert(canPartitionEqualSubset(nums2) == false);

    // Example 3: Single positive element, cannot split into two non-empty equal subsets.
    std::vector<int> nums3 = {7};
    assert(canPartitionEqualSubset(nums3) == false);

    // Example 4: Empty vector, no subsets.
    std::vector<int> nums4;
    assert(canPartitionEqualSubset(nums4) == false);

    // Example 5: Two equal elements -> each subset is one element.
    std::vector<int> nums5 = {3, 3};
    assert(canPartitionEqualSubset(nums5) == true);

    // Example 6: [2, 2, 4] -> sum=8 target=4, subset {4} and {2,2}.
    std::vector<int> nums6 = {2, 2, 4};
    assert(canPartitionEqualSubset(nums6) == true);

    // Example 7: [1, 1, 1, 1, 1, 1] -> sum=6 target=3, possible (three 1's each).
    std::vector<int> nums7 = {1, 1, 1, 1, 1, 1};
    assert(canPartitionEqualSubset(nums7) == true);

    // Example 8: [1, 2, 5] -> sum=8 target=4, no subset sums to 4.
    std::vector<int> nums8 = {1, 2, 5};
    assert(canPartitionEqualSubset(nums8) == false);

    // Example 9: [10, 10, 10, 10] -> sum=40 target=20, possible (two 10's each).
    std::vector<int> nums9 = {10, 10, 10, 10};
    assert(canPartitionEqualSubset(nums9) == true);

    // Example 10: [1, 2, 3, 4, 5, 5] -> sum=20 target=10, subset {5,5} and {1,2,3,4}.
    std::vector<int> nums10 = {1, 2, 3, 4, 5, 5};
    assert(canPartitionEqualSubset(nums10) == true);

    return 0;
}

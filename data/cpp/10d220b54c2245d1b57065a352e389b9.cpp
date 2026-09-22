/*
Write a C++ function `int maxNonOverlappingSubarraySums(const std::vector<int>& nums, int target)` that, given a vector of integers `nums` (length n, possibly empty) and an integer `target`, returns the maximum number of non-overlapping subarrays such that each selected subarray has a sum equal to `target`. Subarrays must be contiguous and cannot share any elements; you may leave some elements unused. The order of elements must be preserved (subarrays appear in the original sequence from left to right). For example, with `nums = {1, 1, 1, 1, 1}` and `target = 2`, the maximum is 2 (e.g., first two elements and last two elements). With `nums = {5, -2, 3, 4, -1}` and `target = 6`, the maximum is 1 (the subarray `[-2,3,4,-1]` sums to 4, not 6, but `[5,-2,3]` sums to 6, or `[4,-1]` sums to 3; you can only find one valid non-overlapping subarray). Note: subarray sums can be negative, and the elements can be any integers (including zero, negative, and positive). The function must handle empty input (return 0) and cases where no valid subarray exists (return 0). The vector should not be modified.
*/
#include <vector>
#include <unordered_map>
#include <algorithm>

// Return the maximum count of non-overlapping contiguous subarrays
// whose sum equals target. The input vector is not modified.
int maxNonOverlappingSubarraySums(const std::vector<int>& nums, int target) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // dp[i] = max count using first i elements, prefix[i] = sum of first i elements
    std::vector<int> dp(n + 1, 0);
    std::vector<int> prefix(n + 1, 0);

    // Build prefix sums: prefix[i] = sum of nums[0..i-1]
    for (int i = 1; i <= n; ++i) {
        prefix[i] = prefix[i - 1] + nums[i - 1];
    }

    // Map from prefix sum value to the latest index i (0..n) where it occurs.
    std::unordered_map<int, int> latestIndex;
    latestIndex[0] = 0; // prefix[0] = 0

    for (int i = 1; i <= n; ++i) {
        // Option 1: ignore the i-th element (0-based index i-1)
        dp[i] = dp[i - 1];

        // Option 2: if there exists j < i with prefix[i] - prefix[j] == target,
        // then take subarray nums[j..i-1] and add 1 to dp[j]
        int needed = prefix[i] - target;
        auto it = latestIndex.find(needed);
        if (it != latestIndex.end()) {
            int j = it->second;
            dp[i] = std::max(dp[i], dp[j] + 1);
        }

        // Update map with current prefix sum (latest index)
        latestIndex[prefix[i]] = i;
    }

    return dp[n];
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxNonOverlappingSubarraySums({1, 1, 1, 1, 1}, 2) == 2);
    assert(maxNonOverlappingSubarraySums({1, 2, 3, 4, 5}, 5) == 2); // [2,3] and [5]
    assert(maxNonOverlappingSubarraySums({5, -2, 3, 4, -1}, 6) == 1); // [5,-2,3] or [4,-1]? actually [4,-1] sums 3, so only [5,-2,3] sums 6
    assert(maxNonOverlappingSubarraySums({-1, -1, -1}, -2) == 1); // first two or last two
    assert(maxNonOverlappingSubarraySums({}, 5) == 0);
    assert(maxNonOverlappingSubarraySums({1, 2, 3}, 10) == 0); // no subarray sums to 10
    assert(maxNonOverlappingSubarraySums({0, 0, 0}, 0) == 3); // each single zero works
    assert(maxNonOverlappingSubarraySums({1, -1, 1, -1}, 0) == 2); // [1,-1] and [1,-1]
    assert(maxNonOverlappingSubarraySums({3, 3, 3, 3}, 6) == 2); // [3,3] and [3,3]
    assert(maxNonOverlappingSubarraySums({10, -5, 5, 1, 2}, 5) == 2); // [10,-5] and [5]? Actually [10,-5] sums 5, [1,2] sums 3, so better [10,-5] and [5]? But [5] overlaps? No, indices 0-1 and 2 are fine. Plus [1,2]? no. So max 2.
    return 0;
}
// The problem is solved using dynamic programming with prefix sums. Let `presum[i]` be the sum of elements from index 0 to i-1 (using 1-based indexing internally). Define `dp[i]` as the maximum number of valid non-overlapping subarrays that can be found using only the first `i` elements. The recurrence: `dp[i] = dp[i-1]` (ignore the i-th element), but if there exists some index `j < i` such that `presum[i] - presum[j] == target`, then `dp[i] = max(dp[i], dp[j] + 1)`, meaning we take the subarray from index j to i-1 as one valid subarray and add the optimal count from the first j elements. To find such j efficiently, we store a hash map from each prefix sum to the latest index where that sum appeared; because we want to allow non-overlapping subarrays, using the latest index minimizes overlap risk and is sufficient due to the DP structure. We initialize `Map[0] = 0` (prefix sum 0 occurs at index 0). For each i from 1 to n, we compute `presum[i]`, update `dp[i]` as described, then update `Map[presum[i]] = i` (overwriting any earlier occurrence; this is safe because a later index with the same sum can only help or not hurt). The answer is `dp[n]`. Edge cases: empty vector returns 0; all subarray sums not equal to target yields 0; negative numbers are handled naturally because prefix sums can decrease; zero-length subarrays are not considered because j < i. Time complexity O(n) and space complexity O(n) for the dp and prefix arrays, plus O(n) for the hash map in the worst case.

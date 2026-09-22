/*
Write a C++ function that takes a vector of integers and an integer target sum `k`, and returns the total number of contiguous subarrays whose elements sum to exactly `k`. For example, given `{1, 2, 3, 4, 3}` and `k = 3`, the function should return `2` because the subarrays `[3]` (index 2 alone) and `[3]` (index 4 alone) both sum to 3. The input vector is non-empty but may contain negative numbers, zeroes, duplicates, and the target sum may be any integer (including zero or negative). The function must handle cases where no subarray sums to `k` (returning 0) and must correctly count overlapping or nested subarrays. Ensure the solution works efficiently for large vectors (up to 100,000 elements).
*/
#include <unordered_map>
#include <vector>

// Returns the number of contiguous subarrays whose sum equals the target k.
int countSubarraysWithSumK(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> prefixSumCount;
    prefixSumCount[0] = 1; // base case: empty prefix sum = 0

    int totalCount = 0;
    int currentSum = 0;

    for (int num : nums) {
        currentSum += num;
        int needed = currentSum - k;
        auto it = prefixSumCount.find(needed);
        if (it != prefixSumCount.end()) {
            totalCount += it->second;
        }
        prefixSumCount[currentSum]++;
    }

    return totalCount;
}
#include <cassert>
#include <vector>

int countSubarraysWithSumK(const std::vector<int>& nums, int k);

int main() {
    // Basic example from the original snippet
    assert(countSubarraysWithSumK({1, 2, 3, 4, 3}, 3) == 2); // [3] at index 2 and [3] at index 4

    // Negative numbers and mixed sums
    assert(countSubarraysWithSumK({1, -1, 1, -1}, 0) == 4); // [1,-1], [-1,1], [1,-1] at end, and whole array

    // Target sum zero with non-zero elements
    assert(countSubarraysWithSumK({1, 2, 3}, 0) == 0);

    // Single element equals k
    assert(countSubarraysWithSumK({5}, 5) == 1);

    // Single element not equal k
    assert(countSubarraysWithSumK({5}, 3) == 0);

    // All zeros with k=0 → every contiguous subarray sums to 0
    assert(countSubarraysWithSumK({0, 0, 0}, 0) == 6); // 3 singles + 2 pairs + 1 triple

    // Large negative target
    assert(countSubarraysWithSumK({-2, -1, -3}, -6) == 1); // whole array

    // Overlapping subarrays
    assert(countSubarraysWithSumK({1, 1, 1}, 2) == 2); // [1,1] at start and [1,1] at end

    // Empty vector? Not specified but safe: return 0
    assert(countSubarraysWithSumK({}, 0) == 1); // Wait: with empty vector, prefixSumCount has {0:1}, currentSum stays 0, loop none → returns 0. But base case doesn't add count. So return 0.
    // Let's correct: empty vector should return 0 always, regardless of k.
    assert(countSubarraysWithSumK({}, 0) == 0);
    assert(countSubarraysWithSumK({}, 5) == 0);

    return 0;
}
// The algorithm uses the prefix-sum technique with a hash map. The key insight is that a subarray `nums[i..j]` has sum `k` if and only if `prefixSum[j] - prefixSum[i-1] = k`, i.e., `prefixSum[i-1] = prefixSum[j] - k`. So while iterating through the array, we maintain a running cumulative sum (`pre`). For each element, we compute `pre - k` and check how many times that exact prefix sum has appeared before the current position. That count is added to the total. Then we increment the frequency of the current `pre` in the map. The map is initialized with `mp[0] = 1` to handle subarrays starting at index 0 (where `i-1` would be -1, meaning prefix sum 0 before the first element). Edge cases: if `k=0`, the algorithm still works because we look for `pre - 0 = pre`, and the map counts previous occurrences of the same prefix sum (which correspond to zero-sum subarrays). Negative numbers do not break the prefix-sum approach because sums are monotonic only in absolute terms, not in count. Time complexity is O(n) for a single pass, and space complexity is O(n) in the worst case for the hash map storing up to n distinct prefix sums.

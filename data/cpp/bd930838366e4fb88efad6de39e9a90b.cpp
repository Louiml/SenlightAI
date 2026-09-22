/*
Write a C++ function `int countZeroSumSubarrays(const std::vector<int>& arr)` that, given a non-empty vector of integers, returns the number of contiguous subarrays whose sum equals zero. The input may contain negative numbers, zeros, and duplicate values. A single element equal to zero counts as a valid zero-sum subarray. The function must count every distinct contiguous subarray, meaning subarrays that appear at different starting indices (even if their elements are identical) are counted separately.
*/
#include <vector>
#include <unordered_map>

/**
 * Count the number of contiguous subarrays whose sum equals zero.
 * @param arr A non-empty vector of integers.
 * @return Number of subarrays with sum zero.
 */
int countZeroSumSubarrays(const std::vector<int>& arr) {
    std::unordered_map<long long, int> prefixCount;
    prefixCount[0] = 1; // empty prefix before first element
    long long currentSum = 0;
    int count = 0;
    
    for (int value : arr) {
        currentSum += value;
        auto it = prefixCount.find(currentSum);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        prefixCount[currentSum]++;
    }
    
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Single zero
    assert(countZeroSumSubarrays({0}) == 1);
    // All positive
    assert(countZeroSumSubarrays({1, 2, 3}) == 0);
    // Mixed with one zero
    assert(countZeroSumSubarrays({1, -1, 2}) == 1); // subarray [1, -1]
    // Multiple zeros
    assert(countZeroSumSubarrays({0, 0}) == 3); // [0], [0], [0,0]
    // Negative and positive cancel
    assert(countZeroSumSubarrays({3, -2, -1, 4, -4}) == 2); // [3,-2,-1] and [4,-4]
    // Larger example
    assert(countZeroSumSubarrays({1, 2, -3, 0, 1, -1}) == 4); // [1,2,-3], [0], [1,-1], [0,1,-1]
    // All zeros
    assert(countZeroSumSubarrays({0, 0, 0}) == 6); // 3 singletons + 2 pairs + 1 triplet
    // No zero-sum subarrays
    assert(countZeroSumSubarrays({5, 5, 5}) == 0);
    // Negative values only
    assert(countZeroSumSubarrays({-1, -1}) == 0);
    // Single positive
    assert(countZeroSumSubarrays({5}) == 0);
    return 0;
}
// The solution uses a prefix sum and a hash map to count occurrences of each prefix sum. The key insight: a subarray from index `i+1` to `j` (inclusive) has sum zero if `prefix[j] == prefix[i]`, where `prefix[k]` is the sum of elements from index 0 to `k`. By iterating through the array, we maintain a running sum. For each new running sum, any previous occurrence of the same sum indicates a zero-sum subarray ending at the current index. So we add the number of previous occurrences of the current running sum to a total counter, then increment the count of that running sum in the map. The running sum equal to zero before any elements (prefix sum of -1) is implicitly considered by initializing the map with `{0, 1}`. This handles subarrays starting at index 0.
//
// Time complexity is O(n) since we traverse the array once and each map insertion/lookup is O(1) on average. Space complexity is O(n) in the worst case due to storing up to n+1 distinct prefix sums. Edge cases: all positive numbers yield zero; an array with a single zero yields one; an array with multiple zeros (e.g., [0,0]) counts each zero individually plus the combined subarray, so the result is 3 (subarrays: [0] at index0, [0] at index1, [0,0]).

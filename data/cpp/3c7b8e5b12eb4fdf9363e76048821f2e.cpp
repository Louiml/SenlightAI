/*
Given a binary array `nums` (containing only 0s and 1s) and an integer `goal`, write a C++ function that returns the number of non-empty contiguous subarrays whose sum equals `goal`. The function should handle empty arrays, non-zero goals with all-zero arrays, and large arrays efficiently. For example, for `nums = {1,0,1,0,1}` and `goal = 2`, the output should be `4` because the subarrays `[1,0,1]`, `[1,0,1,0]`, `[0,1,0,1]`, and `[1,0,1]` (the second occurrence of the pattern) sum to 2. Implement the solution with a single pass using a hash map.
*/
#include <vector>
#include <unordered_map>

// Counts the number of non-empty subarrays whose sum equals goal.
// nums: binary array (0s and 1s)
// goal: target sum
// Returns the count as an integer.
int numSubarraysWithSum(const std::vector<int>& nums, int goal) {
    std::unordered_map<int, int> prefixCount;
    prefixCount[0] = 1; // empty prefix has sum 0
    int currentSum = 0;
    int count = 0;
    
    for (int value : nums) {
        currentSum += value;
        // If a prefix with sum (currentSum - goal) exists, add its frequency
        auto it = prefixCount.find(currentSum - goal);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        // Increment the frequency of the current prefix sum
        prefixCount[currentSum]++;
    }
    return count;
}
#include <cassert>
#include <vector>

// Function declaration (defined above)
int numSubarraysWithSum(const std::vector<int>& nums, int goal);

int main() {
    // Test cases
    std::vector<int> nums1 = {1, 0, 1, 0, 1};
    assert(numSubarraysWithSum(nums1, 2) == 4);

    std::vector<int> nums2 = {0, 0, 0, 0};
    assert(numSubarraysWithSum(nums2, 0) == 10); // all subarrays
    
    std::vector<int> nums3 = {1, 1, 1};
    assert(numSubarraysWithSum(nums3, 3) == 1);
    
    std::vector<int> nums4 = {0, 1, 0};
    assert(numSubarraysWithSum(nums4, 1) == 4); // single ones and subarrays with two zeros
    
    std::vector<int> nums5 = {};
    assert(numSubarraysWithSum(nums5, 5) == 0);
    
    std::vector<int> nums6 = {1, 0, 0, 1};
    assert(numSubarraysWithSum(nums6, 1) == 6);
    
    std::vector<int> nums7 = {0, 0, 1, 0, 0};
    assert(numSubarraysWithSum(nums7, 1) == 9);
    
    std::vector<int> nums8 = {1};
    assert(numSubarraysWithSum(nums8, 0) == 0);
    
    std::vector<int> nums9 = {1};
    assert(numSubarraysWithSum(nums9, 1) == 1);
    
    std::vector<int> nums10 = {0, 0, 0};
    assert(numSubarraysWithSum(nums10, 0) == 6);
    
    return 0;
}
// The key insight is to use a prefix sum technique. For each position in the array, maintain a running sum of elements from the start. If we have seen a prefix sum equal to `(current_sum - goal)` earlier, then the subarray between that earlier index (exclusive) and the current index (inclusive) has sum `goal`. By storing the frequency of each prefix sum encountered so far in a hash map, we can count how many times each needed prefix appeared. Initialize the map with `0 → 1` to handle subarrays that start from the beginning. For each element, update the running sum, add the count of `(sum - goal)` from the map to the answer, then increment the frequency of the current sum in the map. Edge cases: empty array (should return 0), `goal = 0` (count subarrays of all zeros), and negative goals (not possible in binary arrays, but the function should still work if given). Time complexity is O(n), space complexity is O(n) due to the hash map.

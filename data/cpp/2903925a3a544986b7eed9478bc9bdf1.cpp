/*
Write a C++ function that accepts a vector of integers `nums` whose size is a positive multiple of 3, and a positive integer `k`. The function must partition the elements of `nums` into groups of exactly 3 elements each, such that within every group the difference between the maximum and minimum element is at most `k`. Return a 2D vector where each inner vector is one valid group of 3 elements. If such a partition is impossible, return an empty 2D vector. If multiple valid partitions exist, return any one of them. The elements in the original array may appear in any order, and each element must be used exactly once.
*/

#include <vector>
#include <algorithm>

/**
 * @brief Partition nums into groups of 3 where each group's max-min <= k.
 * 
 * @param nums Vector of integers, size must be a positive multiple of 3.
 * @param k Positive integer representing the maximum allowed difference.
 * @return std::vector<std::vector<int>> A 2D vector of groups, or empty if impossible.
 */
std::vector<std::vector<int>> divideArrayIntoGroups(std::vector<int> nums, int k) {
    // If size is not a multiple of 3, return empty (should not happen per constraints)
    if (nums.size() % 3 != 0) {
        return {};
    }
    
    std::sort(nums.begin(), nums.end());
    
    std::vector<std::vector<int>> result;
    result.reserve(nums.size() / 3);
    
    for (size_t i = 0; i < nums.size(); i += 3) {
        // Check if the group of three consecutive elements satisfies the condition
        if (nums[i + 2] - nums[i] > k) {
            return {};
        }
        result.push_back({nums[i], nums[i + 1], nums[i + 2]});
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Function declaration (must match the solution)
std::vector<std::vector<int>> divideArrayIntoGroups(std::vector<int> nums, int k);

// Helper to check if a partition is valid
bool isValidPartition(const std::vector<int>& nums, const std::vector<std::vector<int>>& groups, int k) {
    if (groups.empty()) return nums.empty(); // Empty result only valid if input empty (not per constraints)
    if (nums.size() % 3 != 0) return false;
    if (groups.size() != nums.size() / 3) return false;
    
    // Flatten groups and check all elements are used exactly once
    std::vector<int> flat;
    for (const auto& g : groups) {
        if (g.size() != 3) return false;
        for (int x : g) flat.push_back(x);
        if (g[2] - g[0] > k) return false; // groups should be sorted internally
    }
    
    std::sort(flat.begin(), flat.end());
    std::vector<int> sorted_nums = nums;
    std::sort(sorted_nums.begin(), sorted_nums.end());
    return flat == sorted_nums;
}

int main() {
    // Example 1 from the problem
    std::vector<int> nums1 = {1, 3, 4, 8, 7, 9, 3, 5, 1};
    auto result1 = divideArrayIntoGroups(nums1, 2);
    assert(isValidPartition(nums1, result1, 2));
    
    // Example 2: impossible
    std::vector<int> nums2 = {1, 3, 3, 2, 7, 3};
    auto result2 = divideArrayIntoGroups(nums2, 3);
    assert(result2.empty());
    
    // Single group
    std::vector<int> nums3 = {5, 1, 9};
    auto result3 = divideArrayIntoGroups(nums3, 10);
    assert(result3.size() == 1 && result3[0].size() == 3);
    
    // Single group that fails
    std::vector<int> nums4 = {5, 1, 9};
    auto result4 = divideArrayIntoGroups(nums4, 2);
    assert(result4.empty());
    
    // Larger test with duplicates
    std::vector<int> nums5 = {2, 2, 2, 4, 4, 4, 6, 6, 6};
    auto result5 = divideArrayIntoGroups(nums5, 2);
    assert(isValidPartition(nums5, result5, 2));
    
    // Values all identical
    std::vector<int> nums6 = {7, 7, 7, 7, 7, 7};
    auto result6 = divideArrayIntoGroups(nums6, 0);
    assert(isValidPartition(nums6, result6, 0));
    
    // Unsorted input
    std::vector<int> nums7 = {9, 1, 5, 8, 2, 6, 3, 7, 4};
    auto result7 = divideArrayIntoGroups(nums7, 3);
    assert(isValidPartition(nums7, result7, 3));
    
    // Impossible with large gap
    std::vector<int> nums8 = {1, 2, 100, 101, 102, 103};
    auto result8 = divideArrayIntoGroups(nums8, 1);
    assert(result8.empty());
    
    return 0;
}

// A greedy sorting approach solves this problem. First, sort `nums` in ascending order. Because any valid group must have its three elements within a range of `k`, and since the sorted order makes the smallest remaining element the most restrictive, we process the array from left to right in blocks of three. For each index `i = 0, 3, 6, ...`, take the triple `(nums[i], nums[i+1], nums[i+2])`. If `nums[i+2] - nums[i] > k`, then no valid partition exists because the smallest remaining element cannot be paired with any two larger elements without exceeding the difference constraint—any other pairing would include an element at least as large as `nums[i+2]`. If all such consecutive triples satisfy the condition, each triple forms a valid group. This works because grouping the smallest three remaining elements is always optimal: if they cannot fit together, no other combination can fit them with larger elements. The algorithm runs in O(n log n) time due to sorting, and O(n) auxiliary space for the result vector. Edge cases include `n = 3` (single group), large values, and cases where the answer is an empty vector.

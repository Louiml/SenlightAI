/*
Given a vector of integers that may contain duplicates, write a C++ function `vector<vector<int>> findTripletsWithSumZero(vector<int>& nums)` that returns all unique triplets `[a, b, c]` such that `a + b + c == 0` and the triplets are in non-decreasing order (i.e., `a <= b <= c`). The solution must not contain duplicate triplets, meaning that if the input has repeated values, each distinct combination of values should appear only once. The output order of triplets does not matter, but within each triplet the values must be sorted ascending. The function should handle empty input, input with fewer than 3 elements, and inputs with all zeros or with large negative/positive numbers. You must use an approach based on sorting and the two-pointer technique, and ensure the algorithm is efficient for up to `10^4` elements.
*/
#include <vector>
#include <algorithm>

// Find all unique triplets in the sorted order that sum to zero.
std::vector<std::vector<int>> findTripletsWithSumZero(std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(nums.size());
    if (n < 3) {
        return result;
    }
    std::sort(nums.begin(), nums.end());
    
    for (int i = 0; i < n - 2; ++i) {
        // Skip duplicate values for the first element
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }
        int left = i + 1;
        int right = n - 1;
        int target = 0 - nums[i];
        while (left < right) {
            int currentSum = nums[left] + nums[right];
            if (currentSum == target) {
                result.push_back({nums[i], nums[left], nums[right]});
                // Skip duplicates for the second element
                while (left < right && nums[left] == nums[left + 1]) {
                    ++left;
                }
                // Skip duplicates for the third element
                while (left < right && nums[right] == nums[right - 1]) {
                    --right;
                }
                ++left;
                --right;
            } else if (currentSum < target) {
                ++left;
            } else {
                --right;
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Basic test with multiple triplets
    std::vector<int> nums1 = {-1, 0, 1, 2, -1, -4};
    auto res1 = findTripletsWithSumZero(nums1);
    std::vector<std::vector<int>> expected1 = {{-1, -1, 2}, {-1, 0, 1}};
    assert(res1.size() == 2);
    // Since order of triplets may differ, sort both for comparison
    std::sort(res1.begin(), res1.end());
    std::sort(expected1.begin(), expected1.end());
    assert(res1 == expected1);

    // Empty input
    std::vector<int> nums2;
    assert(findTripletsWithSumZero(nums2).empty());

    // Only zeroes - just one triplet
    std::vector<int> nums3 = {0, 0, 0, 0};
    auto res3 = findTripletsWithSumZero(nums3);
    assert(res3.size() == 1);
    assert(res3[0] == std::vector<int>({0, 0, 0}));

    // No triplets
    std::vector<int> nums4 = {1, 2, 3};
    assert(findTripletsWithSumZero(nums4).empty());

    // Larger array with duplicates
    std::vector<int> nums5 = {-2, -2, 0, 0, 2, 2};
    auto res5 = findTripletsWithSumZero(nums5);
    assert(res5.size() == 1);
    assert(res5[0] == std::vector<int>({-2, 0, 2}));

    // All negative numbers
    std::vector<int> nums6 = {-1, -2, -3};
    assert(findTripletsWithSumZero(nums6).empty());

    // Mixed with 0
    std::vector<int> nums7 = {-1, 0, 1};
    auto res7 = findTripletsWithSumZero(nums7);
    assert(res7.size() == 1);
    assert(res7[0] == std::vector<int>({-1, 0, 1}));

    return 0;
}
// The core idea is to sort the array first, which allows us to use two pointers to find pairs that sum to a target. For each index `i`, we set the target as `-nums[i]` and then search for two numbers `nums[l]` and `nums[r]` (with `l = i+1` and `r = nums.size()-1`) that sum to that target. If the sum is too large, we decrement `r`; if too small, we increment `l`. When we find an exact match, we record the triplet `[nums[i], nums[l], nums[r]]`, then skip over any duplicate values of `nums[l]` and `nums[r]` to avoid duplicate triplets. After finishing the inner loop, we also skip duplicate values of `nums[i]` to avoid duplicate triplets for the same first element. Edge cases: (1) For `i` loop, we must stop when `i < nums.size()-2` because we need at least two elements after `i`; the snippet’s condition `i<nums.size()-1` would cause an out-of-bounds when `nums.size()` is 0 because `nums.size()-1` underflows to a huge unsigned number. In the reference solution, we handle this by using `int n = nums.size();` and checking `i < n-2`. (2) Input with fewer than 3 elements returns an empty vector. (3) Duplicate handling is crucial: after finding a valid pair, we skip all consecutive equal `l` and `r` values; after finishing the inner loop for a given `i`, we skip all consecutive equal `i` values. Time complexity is `O(n^2)` due to sorting `O(n log n)` plus the nested loop. Space complexity is `O(1)` auxiliary (excluding the output storage).

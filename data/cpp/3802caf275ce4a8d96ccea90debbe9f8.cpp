// Write a C++ function named `findFourSum` that takes a vector of integers `nums` and an integer `target`, and returns a `vector<vector<int>>` containing all unique quadruplets `(a, b, c, d)` such that `a + b + c + d == target`, where the elements in each quadruplet appear in non-decreasing order and the quadruplets themselves are unique (no duplicate quadruplets). The function must handle vectors with fewer than 4 elements by returning an empty result. For example, given `nums = {1, 0, -1, 0, -2, 2}` and `target = 0`, the output should be `{{-2, -1, 1, 2}, {-2, 0, 0, 2}, {-1, 0, 0, 1}}` (order of quadruplets does not matter, but each quadruplet must be sorted internally).

#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is assumed to be included above.

int main() {
    // Example from the code snippet
    std::vector<int> nums1 = {1, 0, -1, 0, -2, 2};
    std::vector<std::vector<int>> res1 = findFourSum(nums1, 0);
    assert(res1.size() == 3);
    assert(std::find(res1.begin(), res1.end(), std::vector<int>{-2, -1, 1, 2}) != res1.end());
    assert(std::find(res1.begin(), res1.end(), std::vector<int>{-2, 0, 0, 2}) != res1.end());
    assert(std::find(res1.begin(), res1.end(), std::vector<int>{-1, 0, 0, 1}) != res1.end());
    
    // Empty or too small vectors
    std::vector<int> nums2 = {};
    assert(findFourSum(nums2, 0).empty());
    std::vector<int> nums3 = {1, 2, 3};
    assert(findFourSum(nums3, 6).empty());
    
    // All zeros
    std::vector<int> nums4 = {0, 0, 0, 0};
    auto res4 = findFourSum(nums4, 0);
    assert(res4.size() == 1);
    assert(res4[0] == std::vector<int>({0, 0, 0, 0}));
    
    // Duplicate inputs
    std::vector<int> nums5 = {2, 2, 2, 2, 2};
    auto res5 = findFourSum(nums5, 8);
    assert(res5.size() == 1);
    assert(res5[0] == std::vector<int>({2, 2, 2, 2}));
    
    // Negative numbers
    std::vector<int> nums6 = {-3, -2, -1, 0, 1, 2, 3};
    auto res6 = findFourSum(nums6, 0);
    assert(res6.size() > 0);
    // Manually check a known quadruplet
    assert(std::find(res6.begin(), res6.end(), std::vector<int>{-3, -2, 2, 3}) != res6.end());
    
    // Unsorted input
    std::vector<int> nums7 = {3, -1, 0, 2, -2, 1};
    auto res7 = findFourSum(nums7, 0);
    assert(res7.size() == 3);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Return all unique quadruplets in `nums` that sum to `target`.
// Each quadruplet is sorted internally; no duplicate quadruplets are returned.
std::vector<std::vector<int>> findFourSum(std::vector<int> nums, int target) {
    std::vector<std::vector<int>> result;
    int n = static_cast<int>(nums.size());
    if (n < 4) {
        return result;
    }
    
    std::sort(nums.begin(), nums.end());
    
    for (int i = 0; i <= n - 4; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }
        for (int j = i + 1; j <= n - 3; ++j) {
            if (j > i + 1 && nums[j] == nums[j - 1]) {
                continue;
            }
            
            int left = j + 1;
            int right = n - 1;
            int remaining = target - nums[i] - nums[j];
            
            while (left < right) {
                int currentSum = nums[left] + nums[right];
                if (currentSum == remaining) {
                    result.push_back({nums[i], nums[j], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) {
                        ++left;
                    }
                    while (left < right && nums[right] == nums[right - 1]) {
                        --right;
                    }
                    ++left;
                    --right;
                } else if (currentSum < remaining) {
                    ++left;
                } else {
                    --right;
                }
            }
        }
    }
    
    return result;
}

// The solution uses a sorted array combined with a nested loop and a two-pointer technique. First, sort the input vector to enable duplicate skipping and the two-pointer approach. Then, fix the first element `i` from index 0 to `n-4` (since we need at least 4 elements), skipping duplicates by checking `nums[i] == nums[i-1]`. For each fixed `i`, fix a second element `j` from `i+1` to `n-3`, again skipping duplicates. For each pair `(i, j)`, use two pointers `left = j+1` and `right = n-1` to find the remaining two numbers that sum to `target - nums[i] - nums[j]`. If the sum equals the target, record the quadruplet, then skip duplicate values for `left` and `right` before moving inward. If the sum is less than the target, increment `left`; otherwise decrement `right`. Edge cases include arrays with fewer than 4 elements (return empty), duplicate values that must be skipped to avoid duplicate quadruplets, and negative numbers. Time complexity is O(n^3) due to the two outer loops and the two-pointer scan, with O(1) extra space excluding the output vector (ignoring sorting overhead O(n log n) for the sort itself). Space complexity is O(n) for the sort if we consider the input vector mutated in-place, plus O(k) for the result where k is the number of unique quadruplets.

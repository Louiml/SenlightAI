/*
Write a C++ function that takes a vector of integers `nums` and a positive integer `k` (where `1 <= k <= nums.size()`) and returns a vector of integers representing, for each contiguous subarray of length `k`, the maximum element if that subarray consists of strictly consecutive increasing integers (each element is exactly one greater than the previous), otherwise `-1`. For example, with `nums = [1,2,3,4,3,2,1]` and `k = 3`, the valid windows are `[1,2,3] -> 3` and `[2,3,4] -> 4`, while `[3,4,3]`, `[4,3,2]`, `[3,2,1]` are invalid, yielding `-1`. The function should be named `resultsArray` and must be `const`‑correct, returning a new vector without modifying the input. Assume the input vector may contain negative numbers, duplicates, and is not necessarily sorted. Handle edge cases such as `k = 1` (every single element is trivially consecutive, length‑1 subarray, so return the element itself), and when no window is valid, return a vector filled with `-1`.
*/

#include <vector>
#include <algorithm>

// For each contiguous subarray of length k in nums, return the maximum element
// if the subarray consists of strictly increasing consecutive integers (each
// element is exactly 1 greater than the previous), otherwise return -1.
std::vector<int> resultsArray(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    int n = static_cast<int>(nums.size());
    
    // Edge case: k > n is not allowed per problem constraints, but guard anyway.
    if (k <= 0 || k > n) return result;
    
    for (int i = 0; i <= n - k; ++i) {
        bool isConsecutive = true;
        for (int j = i; j < i + k - 1; ++j) {
            if (nums[j] + 1 != nums[j + 1]) {
                isConsecutive = false;
                break;
            }
        }
        if (isConsecutive) {
            // The last element in the window is the maximum because values are strictly increasing by 1.
            result.push_back(nums[i + k - 1]);
        } else {
            result.push_back(-1);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with mixed valid/invalid windows
    std::vector<int> nums1 = {1, 2, 3, 4, 3, 2, 1};
    std::vector<int> expected1 = {3, 4, -1, -1, -1};
    assert(resultsArray(nums1, 3) == expected1);
    
    // k = 1: every element is a length‑1 consecutive window
    std::vector<int> nums2 = {5, -2, 0, 10};
    assert(resultsArray(nums2, 1) == nums2);
    
    // k equals the whole array, valid case
    std::vector<int> nums3 = {7, 8, 9, 10};
    assert(resultsArray(nums3, 4) == std::vector<int>{10});
    
    // k equals the whole array, invalid case
    std::vector<int> nums4 = {1, 3, 5, 7};
    assert(resultsArray(nums4, 4) == std::vector<int>{-1});
    
    // Contains negative numbers and duplicates, no valid window
    std::vector<int> nums5 = {-3, -2, -1, 0, 0, 1};
    // k=2: windows: [-3,-2] -> -2, [-2,-1] -> -1, [-1,0] -> 0, [0,0] -> -1, [0,1] -> 1
    std::vector<int> expected5 = {-2, -1, 0, -1, 1};
    assert(resultsArray(nums5, 2) == expected5);
    
    // Large k with invalid pair in the middle
    std::vector<int> nums6 = {10, 11, 12, 14, 15};
    assert(resultsArray(nums6, 3) == (std::vector<int>{-1, -1, -1})); // no valid length‑3 window
    
    // Single element array, k=1
    std::vector<int> nums7 = {42};
    assert(resultsArray(nums7, 1) == std::vector<int>{42});
    
    // All consecutive from start to end, k=2
    std::vector<int> nums8 = {1, 2, 3, 4, 5};
    assert(resultsArray(nums8, 2) == (std::vector<int>{2, 3, 4, 5}));
    
    return 0;
}

// The straightforward approach is a sliding window scan. For each starting index `i` from `0` to `n - k` inclusive, inspect the subarray `nums[i..i+k-1]` and check whether it is strictly increasing by 1 at each step. The maximum value in such a subarray is simply `nums[i + k - 1]` because consecutive increasing values imply the last element is the largest. However, we can also track the maximum manually if desired. If any adjacent pair violates the +1 condition, the entire window is invalid and we push `-1`. The time complexity is `O(n*k)` in the worst case because we might scan each window fully. A more efficient approach using a sliding window with a counter of "broken consecutive pairs" can reduce it to `O(n)`, but the simplest correct method is acceptable. Edge cases: `k = 1` always returns the original array (since every single element is consecutive); `k = n` checks the whole array once; negative numbers and duplicates work naturally because the +1 check is strict. Space complexity is `O(n-k+1)` for the result vector, plus `O(1)` auxiliary.

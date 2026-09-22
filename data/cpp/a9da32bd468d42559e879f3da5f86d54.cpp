Write a standalone C++ function `vector<int> slidingWindowMax(const vector<int>& nums, int k)` that, given a non-empty array of integers and a positive window size `k` (where `k <= nums.size()`), returns a vector containing the maximum value in every contiguous subarray of length `k`. For example, for `nums = {1, 3, -1, -3, 5, 3, 6, 7}` and `k = 3`, the result should be `{3, 3, 5, 5, 6, 7}`. The solution must be efficient and handle duplicate values, negative numbers, and cases where `k` equals the array size or `k` is 1. The function should not modify the input vector and should be `const`-correct.

The core approach is to maintain a multiset (or a similar ordered container) of the current window's elements. Initially, insert the first `k` elements into a `multiset<int, greater<int>>` (which sorts in descending order). The maximum of the current window is always the first element of the multiset (`*begin()`). Then, slide the window one step at a time: for each new element from index `k` to the end, insert the new element and erase the element that is leaving the window (at index `i - k`). After each insertion and deletion, the new maximum is again `*begin()`. This works because the multiset keeps all elements sorted, so we never need to recalculate the maximum from scratch. Edge cases include: when `k == 1`, the multiset always has one element, and the output is the input itself; when `k == nums.size()`, the output contains only one element (the global maximum). Duplicate values are handled naturally by the multiset. Time complexity is `O(n log k)` due to `insert` and `erase` operations on the multiset, and space complexity is `O(k)` for the multiset plus `O(n - k + 1)` for the output vector.

#include <vector>
#include <set>

// Return the maximum of every contiguous subarray of length k.
std::vector<int> slidingWindowMax(const std::vector<int>& nums, int k) {
    std::multiset<int, std::greater<int>> window;
    for (int i = 0; i < k; ++i) {
        window.insert(nums[i]);
    }
    
    std::vector<int> result;
    result.reserve(nums.size() - k + 1);
    result.push_back(*window.begin());
    
    for (int i = k; i < static_cast<int>(nums.size()); ++i) {
        window.insert(nums[i]);
        window.erase(window.find(nums[i - k]));
        result.push_back(*window.begin());
    }
    
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example from problem statement
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    assert(slidingWindowMax(nums1, 3) == std::vector<int>({3, 3, 5, 5, 6, 7}));
    
    // k = 1 (each element is its own window)
    std::vector<int> nums2 = {4, -2, 0, 9};
    assert(slidingWindowMax(nums2, 1) == std::vector<int>({4, -2, 0, 9}));
    
    // k equals array size
    std::vector<int> nums3 = {5, 1, 8, -3};
    assert(slidingWindowMax(nums3, 4) == std::vector<int>({8}));
    
    // Negative numbers and duplicates
    std::vector<int> nums4 = {-5, -1, -5, -2};
    assert(slidingWindowMax(nums4, 2) == std::vector<int>({-1, -1, -2}));
    
    // Larger k with duplicates
    std::vector<int> nums5 = {7, 7, 7, 7};
    assert(slidingWindowMax(nums5, 2) == std::vector<int>({7, 7, 7}));
    
    // Non-constant values with mixed positive/negative
    std::vector<int> nums6 = {2, 0, -3, 4, 1};
    assert(slidingWindowMax(nums6, 3) == std::vector<int>({2, 4, 4}));
    
    return 0;
}

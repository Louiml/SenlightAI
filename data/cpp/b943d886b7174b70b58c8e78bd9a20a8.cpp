// Write a C++ function named `maxSlidingWindow` that takes a vector of integers `nums` and a positive integer `k` (where `k <= nums.size()`), and returns a vector of integers representing the maximum value in every contiguous subarray (window) of size `k` as the window slides from left to right. The function must be efficient for large inputs and should not modify the original input vector. If `nums` is empty or `k` is 0, return an empty vector. For example, given `nums = {1,3,-1,-3,5,3,6,7}` and `k = 3`, the function returns `{3,3,5,5,6,7}`.

The classic efficient solution uses a monotonic double-ended queue (deque) that stores indices of array elements. The deque maintains indices in decreasing order of their corresponding values (from front to back), so the front always holds the index of the current window's maximum. For each element index `i` from 0 to n-1:
- Remove indices from the back while the value at those indices is less than or equal to `nums[i]`, because they can never be the maximum for any future window once `i` appears.
- Push the current index `i` to the back.
- Remove indices from the front if they fall outside the current window (i.e., index <= i - k).
- When the window has at least `k` elements (i.e., when `i >= k-1`), record `nums[front]` as the result for that window start.
Edge cases: `k == 1` returns the input unchanged; `k == nums.size()` returns a single-element vector with the global maximum; empty input or `k == 0` returns empty; duplicate maximum values are handled correctly because we remove elements that are equal to the current value from the back, ensuring the newest index is kept. Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(k) for the deque, or O(n) in the worst case if k is large, but typically O(k). The result vector requires O(n) space, which is necessary for the output.

#include <vector>
#include <deque>

// Returns the maximum value for each sliding window of size k.
// If nums is empty or k is 0, returns an empty vector.
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    if (nums.empty() || k <= 0 || k > static_cast<int>(nums.size())) {
        return {};
    }

    std::deque<int> dq; // Stores indices of potential maxima, decreasing values.
    std::vector<int> res(nums.size() - k + 1);

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // Remove indices whose values are <= current value (they are obsolete).
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);

        // Remove front index if it is outside the current window.
        if (i - k >= dq.front()) {
            dq.pop_front();
        }

        // When we have at least k elements, record the maximum for the window ending at i.
        if (i >= k - 1) {
            res[i - k + 1] = nums[dq.front()];
        }
    }

    return res;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example from the problem statement.
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    std::vector<int> res1 = maxSlidingWindow(nums1, 3);
    assert(res1 == std::vector<int>({3, 3, 5, 5, 6, 7}));

    // Single element window.
    std::vector<int> nums2 = {4, 1, 9, 2};
    std::vector<int> res2 = maxSlidingWindow(nums2, 1);
    assert(res2 == std::vector<int>({4, 1, 9, 2}));

    // Window size equals full array.
    std::vector<int> nums3 = {5, 2, 8, 1};
    std::vector<int> res3 = maxSlidingWindow(nums3, 4);
    assert(res3 == std::vector<int>({8}));

    // Duplicate maximum values.
    std::vector<int> nums4 = {2, 2, 2, 2};
    std::vector<int> res4 = maxSlidingWindow(nums4, 2);
    assert(res4 == std::vector<int>({2, 2, 2}));

    // Negative numbers.
    std::vector<int> nums5 = {-1, -5, -2, -8, -3};
    std::vector<int> res5 = maxSlidingWindow(nums5, 3);
    assert(res5 == std::vector<int>({-1, -2, -2}));

    // Mixed positive and negative.
    std::vector<int> nums6 = {1, -1, 3, -2, 5};
    std::vector<int> res6 = maxSlidingWindow(nums6, 2);
    assert(res6 == std::vector<int>({1, 3, 3, 5}));

    // Empty input.
    std::vector<int> empty;
    assert(maxSlidingWindow(empty, 3).empty());

    // k = 0 (edge case).
    assert(maxSlidingWindow(nums1, 0).empty());

    // k larger than array size.
    assert(maxSlidingWindow(nums1, 100).empty());

    return 0;
}

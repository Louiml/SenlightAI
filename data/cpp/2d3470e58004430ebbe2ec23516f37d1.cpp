Given a vector of integers and a window size `k`, write a C++ function `maxSlidingWindow` that returns a vector containing the maximum value in every contiguous subarray (sliding window) of length `k`. The function must handle empty input vectors and `k == 0` by returning an empty vector. For example, given `nums = {1,3,-1,-3,5,3,6,7}` and `k = 3`, the output should be `{3,3,5,5,6,7}`. The solution must work for positive, negative, and duplicate integers, and must not modify the input vector.
// The straightforward approach is to iterate through all possible window starting positions (`0` to `n - k`, inclusive) and for each window, scan all `k` elements to find the maximum, yielding O(n*k) time. However, this can be optimized. A common O(n) solution uses a monotonic decreasing deque (or priority queue). But given the prompt's original code uses a `priority_queue<int>` for each window, a simpler O(n*k) implementation is acceptable if clearly explained. We can also improve by using `std::max_element` on each window iterator range, which is still O(n*k). For a more efficient solution, we can maintain a deque of indices where the front holds the index of the current window's maximum. For each new element, remove indices that fall out of the window and pop from the back while the back's value is less than the current element, then push the current index. The front index gives the maximum for the current window. Edge cases: if `nums` is empty or `k == 0` or `k > n`, return empty vector. If `k == 1`, return the original vector. Time complexity: O(n) for deque approach, O(n*k) for simple approach. Space complexity: O(n) for result vector, O(k) for deque.
#include <vector>
#include <deque>

// Returns a vector containing the maximum of each sliding window of size k.
std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
    if (nums.empty() || k <= 0 || k > static_cast<int>(nums.size())) {
        return {};
    }
    std::vector<int> result;
    std::deque<int> dq; // stores indices of potential maxima
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        // Remove indices that are out of the current window
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // Remove from back while the back's value is less than current value
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
        // Once we have a full window, record the maximum
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic test from problem statement
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    std::vector<int> expected1 = {3, 3, 5, 5, 6, 7};
    assert(maxSlidingWindow(nums1, 3) == expected1);

    // Empty input
    std::vector<int> nums2;
    assert(maxSlidingWindow(nums2, 3).empty());

    // k = 0
    std::vector<int> nums3 = {1, 2, 3};
    assert(maxSlidingWindow(nums3, 0).empty());

    // k = 1 (returns original)
    std::vector<int> nums4 = {5, 2, 8, -1};
    assert(maxSlidingWindow(nums4, 1) == nums4);

    // All negative numbers
    std::vector<int> nums5 = {-4, -2, -9, -1};
    std::vector<int> expected5 = {-2, -1, -1};
    assert(maxSlidingWindow(nums5, 2) == expected5);

    // Duplicate values
    std::vector<int> nums6 = {7, 7, 7, 7};
    std::vector<int> expected6 = {7, 7, 7};
    assert(maxSlidingWindow(nums6, 2) == expected6);

    // Single element, k=1
    std::vector<int> nums7 = {42};
    assert(maxSlidingWindow(nums7, 1) == std::vector<int>{42});

    // k larger than n returns empty
    std::vector<int> nums8 = {1, 2, 3};
    assert(maxSlidingWindow(nums8, 5).empty());

    // Mixed positives/negatives and edge window
    std::vector<int> nums9 = {-7, -8, 7, 5, 7, 1, 6, 0};
    std::vector<int> expected9 = {7, 7, 7, 7, 7};
    assert(maxSlidingWindow(nums9, 4) == expected9);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

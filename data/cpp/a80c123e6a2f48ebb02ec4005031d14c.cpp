// Write a standalone C++ function named `maxInSlidingWindow` that takes a vector of integers `nums` and a positive integer `k` (where `1 ≤ k ≤ nums.size()`), and returns a vector of integers containing the maximum element of every contiguous subarray (window) of length `k` in `nums`, in the order the windows appear. The function must handle duplicates gracefully (a duplicate maximum value should not be removed from the active window if the same value appears elsewhere) and must not modify the input array. The returned vector should have size exactly `nums.size() - k + 1`. For example, given `nums = {1,3,-1,-3,5,3,6,7}` and `k = 3`, the output should be `{3,3,5,5,6,7}`. The implementation should be efficient for large inputs, and you must provide a reference solution and tests.
The most straightforward approach is to use a `multiset` to maintain the current window’s elements. Since a `multiset` keeps elements sorted and allows duplicates, the maximum of the current window is simply the last element (via `rbegin()`). For each window: insert the first `k` elements, output the maximum, then slide the window by erasing the element that leaves (using `find` to remove only one occurrence) and inserting the new element, then again output the maximum. This correctly handles duplicates because erasing via `find` removes only one instance. Edge cases: `k = 1` (each window is a single element, output the array itself), `k = n` (only one window, output the global maximum). The complexity is O(n log k) time due to insertion/erasure in the multiset, and O(k) auxiliary space for the multiset. This is optimal enough for typical constraints. An alternative O(n) monotonic deque approach exists, but the multiset solution is simpler to implement correctly and matches the inspiration snippet.
#include <vector>
#include <set>

// Returns a vector containing the maximum of every contiguous subarray of length k in nums.
// The input vector is not modified. k must be >= 1 and <= nums.size().
std::vector<int> maxInSlidingWindow(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    if (nums.empty() || k <= 0 || k > static_cast<int>(nums.size())) {
        return result; // or handle as per specification; here we return empty for invalid input
    }

    std::multiset<int> window;

    // Insert first k elements
    for (int i = 0; i < k; ++i) {
        window.insert(nums[i]);
    }
    result.push_back(*window.rbegin());

    // Slide the window
    for (int i = k; i < static_cast<int>(nums.size()); ++i) {
        // Remove the element that leaves the window (only one occurrence)
        auto it = window.find(nums[i - k]);
        if (it != window.end()) {
            window.erase(it);
        }
        // Insert the new element
        window.insert(nums[i]);
        result.push_back(*window.rbegin());
    }

    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above; here we test it.

int main() {
    // Example from the description
    std::vector<int> nums1 = {1, 3, -1, -3, 5, 3, 6, 7};
    assert(maxInSlidingWindow(nums1, 3) == std::vector<int>({3, 3, 5, 5, 6, 7}));

    // k = 1
    std::vector<int> nums2 = {5, 2, 8, 1};
    assert(maxInSlidingWindow(nums2, 1) == std::vector<int>({5, 2, 8, 1}));

    // k = n
    std::vector<int> nums3 = {4, 1, 9, 2};
    assert(maxInSlidingWindow(nums3, 4) == std::vector<int>({9}));

    // All duplicates
    std::vector<int> nums4 = {7, 7, 7, 7};
    assert(maxInSlidingWindow(nums4, 2) == std::vector<int>({7, 7, 7}));

    // Negative numbers and mixed
    std::vector<int> nums5 = {-3, -2, -1, 0, 1};
    assert(maxInSlidingWindow(nums5, 3) == std::vector<int>({-1, 0, 1}));

    // Single element
    std::vector<int> nums6 = {42};
    assert(maxInSlidingWindow(nums6, 1) == std::vector<int>({42}));

    // Large repeated values
    std::vector<int> nums7 = {10, 10, 10};
    assert(maxInSlidingWindow(nums7, 2) == std::vector<int>({10, 10}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

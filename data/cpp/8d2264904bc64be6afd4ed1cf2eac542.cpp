Write a C++ function `std::vector<int> slidingWindowMaximum(const std::vector<int>& nums, size_t k)` that, given a non-empty vector of integers `nums` and a positive window size `k`, returns a vector containing the maximum element of every contiguous subarray (window) of length `k` in the original order. If `k` is larger than the size of `nums`, return an empty vector. The function must not modify the input vector, and the input vector may contain duplicate values, negative numbers, and zero. The implementation must run in \(O(n)\) time on average, where \(n\) is the number of elements in `nums`, and use auxiliary space proportional to \(O(k)\). Do not use built‑in data structures that maintain sorted order (e.g., `std::multiset` or a sorted container) — you must implement the deque‑based approach.
#include <cassert>
#include <vector>

int main() {
    // Basic single window
    std::vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};
    assert(slidingWindowMaximum(a, 3) == std::vector<int>({3, 3, 5, 5, 6, 7}));

    // Window size 1
    std::vector<int> b = {4, 2, 9, 1};
    assert(slidingWindowMaximum(b, 1) == std::vector<int>({4, 2, 9, 1}));

    // Window equals whole array
    std::vector<int> c = {2, 5, 1, 8, 3};
    assert(slidingWindowMaximum(c, 5) == std::vector<int>({8}));

    // k larger than array size -> empty
    std::vector<int> d = {1, 2};
    assert(slidingWindowMaximum(d, 3) == std::vector<int>());

    // All equal values
    std::vector<int> e = {7, 7, 7, 7};
    assert(slidingWindowMaximum(e, 2) == std::vector<int>({7, 7, 7}));

    // Negative and zero values
    std::vector<int> f = {-5, -2, -1, -8};
    assert(slidingWindowMaximum(f, 2) == std::vector<int>({-2, -1, -1}));

    // Non‑empty vector with k=1 and duplicates
    std::vector<int> g = {0, 0, 1, 0};
    assert(slidingWindowMaximum(g, 1) == std::vector<int>({0, 0, 1, 0}));

    // Empty input vector
    std::vector<int> h;
    assert(slidingWindowMaximum(h, 2) == std::vector<int>());

    // Window size 2 with increasing then decreasing sequence
    std::vector<int> i = {1, 4, 2, 5, 3};
    assert(slidingWindowMaximum(i, 2) == std::vector<int>({4, 4, 5, 5}));

    // Large repeated values and equal maximums
    std::vector<int> j = {9, 9, 8, 9, 9};
    assert(slidingWindowMaximum(j, 3) == std::vector<int>({9, 9, 9}));

    return 0;
}
#include <vector>
#include <deque>

// Returns the maximum of every contiguous subarray of length k in nums.
// If k > nums.size(), returns an empty vector.
std::vector<int> slidingWindowMaximum(const std::vector<int>& nums, size_t k) {
    std::vector<int> result;
    if (nums.empty() || k == 0 || k > nums.size()) {
        return result;
    }

    std::deque<size_t> window; // stores indices, values in decreasing order
    for (size_t i = 0; i < nums.size(); ++i) {
        // Remove indices that are out of the current window
        if (!window.empty() && window.front() + k <= i) {
            window.pop_front();
        }

        // Remove from the back indices whose values are <= current value
        while (!window.empty() && nums[window.back()] <= nums[i]) {
            window.pop_back();
        }

        // Add current index
        window.push_back(i);

        // When the window has at least k elements, record the maximum
        if (i + 1 >= k) {
            result.push_back(nums[window.front()]);
        }
    }
    return result;
}
// The classic efficient solution uses a monotonic deque (double-ended queue) that stores indices of `nums` in decreasing order of their values. For each new element at index `i`, we first remove from the front of the deque any index that has fallen outside the current window (i.e., whose index ≤ `i - k`). Then we remove from the back all indices whose corresponding values are ≤ the new element, because those values can never be the maximum in any future window as long as the new element is present (and the new element is more recent and larger or equal). After pushing the current index to the back, the front of the deque always holds the index of the maximum for the current window. We push `nums[front]` to the result for every window that has at least `k` elements. Edge cases: when `k` equals 1, the result is simply the input itself; when `k` is larger than `nums.size()`, return an empty vector; when all values are equal, each window’s maximum is that value, and the deque correctly keeps only one index (the most recent) due to the "≤" removal condition. Time complexity is \(O(n)\) because each index is pushed and popped at most once. Space complexity is \(O(k)\) for the deque, which at any time holds no more than `k` indices.

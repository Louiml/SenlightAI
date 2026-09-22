Given an array of `n` integers and a positive integer `k` (where `1 <= k <= n`), write a C++ function that returns a `std::vector<int>` containing the maximum value from each contiguous subarray (window) of size `k`, in the order they appear from left to right. The function must handle duplicate values and negative numbers correctly. The array is 0-indexed, and the first window starts at index 0. The returned vector should have exactly `n - k + 1` elements. If the input array is empty or `k` is invalid (less than 1 or greater than `n`), return an empty vector.
#include <cassert>
#include <vector>

// Declare the solution function (it is defined above; here we just test it).
std::vector<int> slidingWindowMax(const std::vector<int>& nums, int k);

int main() {
    // Basic case
    assert(slidingWindowMax({1, 3, -1, -3, 5, 3, 6, 7}, 3) == std::vector<int>({3, 3, 5, 5, 6, 7}));
    // k = 1
    assert(slidingWindowMax({2, 5, 1}, 1) == std::vector<int>({2, 5, 1}));
    // k = n (single window)
    assert(slidingWindowMax({4, 1, 9}, 3) == std::vector<int>({9}));
    // All equal elements
    assert(slidingWindowMax({5, 5, 5, 5}, 2) == std::vector<int>({5, 5, 5}));
    // Negative numbers
    assert(slidingWindowMax({-1, -3, -2}, 2) == std::vector<int>({-1, -2}));
    // Decreasing sequence
    assert(slidingWindowMax({10, 9, 8, 7}, 3) == std::vector<int>({10, 9}));
    // Increasing sequence
    assert(slidingWindowMax({1, 7, 9, 12}, 2) == std::vector<int>({7, 9, 12}));
    // Duplicate values causing equal comparisons
    assert(slidingWindowMax({8, 8, 8, 6}, 2) == std::vector<int>({8, 8, 8}));
    // Invalid k > n
    assert(slidingWindowMax({1, 2}, 5).empty());
    // Invalid k < 1
    assert(slidingWindowMax({1, 2}, 0).empty());
    // Empty array
    assert(slidingWindowMax({}, 2).empty());
    return 0;
}
#include <deque>
#include <vector>

// Returns the maximum of each contiguous subarray (window) of size k.
// The input array 'nums' is read-only. If k is invalid (k < 1 or k > n) or the array is empty, an empty vector is returned.
std::vector<int> slidingWindowMax(const std::vector<int>& nums, int k) {
    std::vector<int> result;
    int n = static_cast<int>(nums.size());
    if (n == 0 || k < 1 || k > n) {
        return result;
    }

    std::deque<int> windowIndices; // stores indices, values decreasing from front to back
    int left = 0; // left boundary of the window
    int right = 0; // right boundary (current index)

    while (right < n) {
        // Remove indices whose values are less than or equal to current value
        while (!windowIndices.empty() && nums[windowIndices.back()] <= nums[right]) {
            windowIndices.pop_back();
        }
        windowIndices.push_back(right);

        // When the window has exactly size k
        if (right - left + 1 == k) {
            // The front index holds the maximum value in the current window
            result.push_back(nums[windowIndices.front()]);

            // Slide left boundary: if the front index is leaving the window, pop it
            if (windowIndices.front() == left) {
                windowIndices.pop_front();
            }
            ++left;
        }
        ++right;
    }
    return result;
}
// The solution uses the sliding window maximum technique with a monotonic deque that stores indices of array elements. The deque is maintained such that the values at those indices are in strictly decreasing order from front to back. For each new element at index `j`, we first remove all indices from the back whose corresponding values are less than or equal to the current value (since they can never be the maximum for the current or any future window that includes `j`). We then push `j` to the back. If the window size becomes exactly `k` (i.e., `j - i + 1 == k`, where `i` is the left boundary), the front of the deque gives the index of the maximum element in that window, and we record `v[de.front()]`. Before sliding the window forward (incrementing `i`), if the front index equals `i`, we pop it from the front because it will no longer be in the next window. We continue until `j` reaches `n`. Edge cases include: `k == 1` (each element is its own window maximum), `k == n` (only one window), duplicate values (equal values are popped from back to preserve the earliest index, but the result is still correct because the maximum value is the same), negative numbers (comparison works normally), and invalid inputs returning an empty vector. Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(k) for the deque, though in the worst case it can hold up to n elements.

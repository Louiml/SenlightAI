/*
Write a C++ function `slidingWindowMin(const std::vector<int>& values, int windowSize)` that, given a sequence of integers and a positive window size `L`, returns a vector containing the minimum of every contiguous subarray of length `L`, in the order they appear. For example, for `values = {1, 3, -1, -3, 5, 3, 6, 7}` and `L = 3`, the result should be `{-1, -3, -3, -3, 3, 3}`. The input vector may be empty, in which case the returned vector must be empty. The window size is guaranteed to be at least 1, and if it is larger than the input size, the function should still return an empty vector (since no complete window exists). Use a monotonic deque or any linear-time approach.
*/
#include <vector>
#include <deque>
#include <utility>

// Return the minimum of each contiguous subarray of length windowSize.
std::vector<int> slidingWindowMin(const std::vector<int>& values, int windowSize) {
    std::vector<int> result;
    if (values.empty() || windowSize <= 0 || windowSize > static_cast<int>(values.size())) {
        return result;
    }

    std::deque<std::pair<int, int>> dq; // (value, index)

    for (int i = 0; i < static_cast<int>(values.size()); ++i) {
        // Remove elements outside the current window from the front.
        while (!dq.empty() && dq.front().second < i - windowSize + 1) {
            dq.pop_front();
        }

        // Maintain increasing order: pop back while back value >= current value.
        while (!dq.empty() && dq.back().first >= values[i]) {
            dq.pop_back();
        }

        dq.push_back({values[i], i});

        // When the first full window is reached, record its minimum.
        if (i >= windowSize - 1) {
            result.push_back(dq.front().first);
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is already declared above; here we test it.
int main() {
    // Basic example
    std::vector<int> v1 = {1, 3, -1, -3, 5, 3, 6, 7};
    std::vector<int> r1 = slidingWindowMin(v1, 3);
    std::vector<int> expected1 = {-1, -3, -3, -3, 3, 3};
    assert(r1 == expected1);

    // Single-element window (output equals input)
    std::vector<int> v2 = {5, 2, 8, 1};
    std::vector<int> r2 = slidingWindowMin(v2, 1);
    assert(r2 == v2);

    // Window size equal to whole vector (one output)
    std::vector<int> v3 = {4, 2, 1, 5};
    std::vector<int> r3 = slidingWindowMin(v3, 4);
    assert(r3 == std::vector<int>{1});

    // Empty input
    std::vector<int> v4;
    std::vector<int> r4 = slidingWindowMin(v4, 3);
    assert(r4.empty());

    // Window larger than input
    std::vector<int> v5 = {1, 2};
    std::vector<int> r5 = slidingWindowMin(v5, 3);
    assert(r5.empty());

    // All equal values
    std::vector<int> v6 = {7, 7, 7, 7};
    std::vector<int> r6 = slidingWindowMin(v6, 2);
    assert(r6 == std::vector<int>({7, 7, 7}));

    // Increasing sequence
    std::vector<int> v7 = {1, 2, 3, 4, 5};
    std::vector<int> r7 = slidingWindowMin(v7, 3);
    assert(r7 == std::vector<int>({1, 2, 3, 4}));

    // Decreasing sequence
    std::vector<int> v8 = {9, 7, 5, 3, 1};
    std::vector<int> r8 = slidingWindowMin(v8, 3);
    assert(r8 == std::vector<int>({5, 3, 1}));

    return 0;
}
// The problem is a classic sliding window minimum. The main algorithm uses a deque that stores pairs `(value, index)` in strictly increasing order of value from front to back. For each new element at index `i`, we first remove from the front any elements whose index is older than `i - L + 1` because they fall out of the current window. Then, we remove from the back all elements whose value is greater than or equal to the current element (actually greater than is enough for minimum; using `>` keeps duplicates distinct and is safe—here we use `>=` to avoid stale duplicates, but `>` is fine too). This maintains the deque's front as the minimum of the current window. After inserting the current element, if the window is complete (i.e., `i >= L-1`), we output the front's value. Edge cases: empty input returns empty; `L` greater than input length yields empty because no complete window exists; single-element windows simply return a copy of the input. The time complexity is O(n) because each element is pushed and popped at most once. The space complexity is O(L) for the deque.

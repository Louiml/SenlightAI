// Write a C++ function `std::vector<int> slidingWindowMax(const std::vector<int>& arr, int w)` that takes a non-empty vector of integers and a positive window size `w` (where `w ≤ arr.size()`), and returns a vector containing the maximum value in each contiguous subarray (sliding window) of length `w`, in left-to-right order. The function must not modify the input, must handle negative numbers and duplicates correctly, and must be implemented using a simple nested-loop approach (not a deque or priority queue) to mirror the given snippet’s style. The output vector size must be exactly `arr.size() - w + 1`. Assume the input vector is valid per the constraints.

The solution directly scans each possible window start index `i` from `0` to `n - w` (inclusive), where `n` is the array length. For each window, initialize a local `max` to `arr[i]`, then iterate through the remaining elements of that window (`j` from `i+1` to `i+w-1`) and update `max` whenever a larger element is found. Append the resulting `max` to the result vector. Edge cases: when `w == 1`, the result is a copy of the input; when `w == n`, the result contains a single element which is the global maximum. Negative numbers and duplicates are handled naturally because comparisons use `>` (not `>=`), so if a duplicate maximum exists, the first occurrence sets the max and later duplicates do not change it—though the result is the value itself, not an index, so duplicates produce the same value. Time complexity is O(n·w) because for each of the `n - w + 1` windows, we scan `w` elements. Auxiliary space is O(n) for the output vector (excluding the input), which is necessary to store the result; no extra temporary data structures are used.

#include <vector>

// Returns a vector of the maximum value in each sliding window of size w.
// The input vector is not modified; w is assumed to be positive and <= arr.size().
std::vector<int> slidingWindowMax(const std::vector<int>& arr, int w) {
    const int n = static_cast<int>(arr.size());
    std::vector<int> result;
    result.reserve(n - w + 1);

    for (int i = 0; i <= n - w; ++i) {
        int current_max = arr[i];
        for (int j = i + 1; j < i + w; ++j) {
            if (arr[j] > current_max) {
                current_max = arr[j];
            }
        }
        result.push_back(current_max);
    }

    return result;
}

#include <cassert>
#include <vector>

// Include the solution function here (or link it).

int main() {
    // Basic example from the snippet
    std::vector<int> arr1 = {3, 1, -1, -3, 5, 3, 6, 7};
    std::vector<int> res1 = slidingWindowMax(arr1, 3);
    assert((res1 == std::vector<int>{3, 1, 5, 5, 6, 7}));

    // Window size 1: result equals the input
    std::vector<int> arr2 = {-5, 0, 2, -1};
    std::vector<int> res2 = slidingWindowMax(arr2, 1);
    assert((res2 == arr2));

    // Window size equals full array: single global maximum
    std::vector<int> arr3 = {10, -2, 8, 7};
    std::vector<int> res3 = slidingWindowMax(arr3, 4);
    assert((res3 == std::vector<int>{10}));

    // All negative numbers
    std::vector<int> arr4 = {-3, -1, -5, -2};
    std::vector<int> res4 = slidingWindowMax(arr4, 2);
    assert((res4 == std::vector<int>{-1, -1, -2}));

    // Duplicate maxima within a window
    std::vector<int> arr5 = {4, 4, 4};
    std::vector<int> res5 = slidingWindowMax(arr5, 2);
    assert((res5 == std::vector<int>{4, 4}));

    // Single element vector
    std::vector<int> arr6 = {42};
    std::vector<int> res6 = slidingWindowMax(arr6, 1);
    assert((res6 == std::vector<int>{42}));

    // Larger test with known results
    std::vector<int> arr7 = {1, 3, -1, -3, 5, 3, 6, 7};
    std::vector<int> res7 = slidingWindowMax(arr7, 2);
    assert((res7 == std::vector<int>{3, 3, -1, 5, 5, 6, 7}));

    // All same values
    std::vector<int> arr8 = {2, 2, 2, 2};
    std::vector<int> res8 = slidingWindowMax(arr8, 3);
    assert((res8 == std::vector<int>{2, 2}));

    return 0;
}

// Write a C++ function `rankTransform` that takes a constant reference to a vector of distinct integers and returns a new vector where each original element is replaced by its rank when the elements are sorted in ascending order: the smallest element becomes `0`, the second smallest becomes `1`, and so on up to `N-1` for the largest, where `N` is the size of the input vector. The output vector must preserve the original order of elements—only their values are replaced by ranks. The input vector contains `N` distinct integers (no duplicates). Your function should be robust for any size `N >= 0` (including an empty vector), handle both positive and negative integers, and not modify the input vector.
// The solution approach is to first make a copy of the input vector and sort this copy in ascending order. This sorted copy gives us the mapping from each original value to its rank: the smallest value is at index `0`, the next smallest at index `1`, etc. Then, for each element in the original vector, we find its position (index) in the sorted copy, which directly gives the rank since the copy is sorted and elements are distinct, so each value appears exactly once. We assign that rank to the corresponding position in the result vector. For an empty input, the function returns an empty vector. The algorithm runs in `O(N log N)` time due to sorting, and uses `O(N)` auxiliary space for the sorted copy and result vector. Edge cases include negative numbers, large magnitudes, and single-element vectors, all handled naturally because distinctness guarantees unique ranks.
#include <vector>
#include <algorithm>

// Replace each element with its rank (0-based) in sorted order.
// Input must contain distinct integers. Returns a new vector.
std::vector<int> rankTransform(const std::vector<int>& input) {
    if (input.empty()) {
        return {};
    }

    // Create a sorted copy to map values to ranks.
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    // Build the result by finding each value's rank in the sorted copy.
    std::vector<int> result;
    result.reserve(input.size());
    for (int value : input) {
        // Since all values are distinct, lower_bound gives the exact index.
        auto it = std::lower_bound(sorted.begin(), sorted.end(), value);
        result.push_back(static_cast<int>(it - sorted.begin()));
    }
    return result;
}
#include <cassert>
#include <vector>

// Function prototype from the solution
std::vector<int> rankTransform(const std::vector<int>& input);

int main() {
    // Basic example from the prompt
    assert(rankTransform({19, 12, 23, 18, 16, 8, 10}) == std::vector<int>({5, 2, 6, 4, 3, 0, 1}));
    
    // Already sorted ascending
    assert(rankTransform({4, 7, 9}) == std::vector<int>({0, 1, 2}));

    // Already sorted descending
    assert(rankTransform({9, 7, 4}) == std::vector<int>({2, 1, 0}));

    // Negative and positive mixed
    assert(rankTransform({-3, 5, 0, -1}) == std::vector<int>({0, 3, 2, 1}));

    // Single element
    assert(rankTransform({42}) == std::vector<int>({0}));

    // Empty vector
    assert(rankTransform({}) == std::vector<int>({}));

    // Large numbers and many elements
    assert(rankTransform({1000000, -1000000, 0}) == std::vector<int>({2, 0, 1}));

    // All negative numbers
    assert(rankTransform({-5, -2, -8}) == std::vector<int>({1, 2, 0}));

    return 0;
}

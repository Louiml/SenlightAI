// Given a non-empty sequence of integers, write a C++ function that returns the smallest possible absolute difference between any two adjacent values after the sequence has been sorted in non-decreasing order. The input is provided as a vector of integers; the function should compute the sorted order, then examine the gaps between consecutive sorted elements, and return the minimum of those gaps. For example, for the vector `{4, 1, 7, 3}`, after sorting to `{1, 3, 4, 7}`, the adjacent differences are `2, 1, 3`, so the result is `1`. The vector may contain duplicate values, in which case the minimum difference will be zero. The vector length is at least 2, and all integers are assumed to fit in a standard `int`.

// The core idea is to sort the input vector in ascending order. Once sorted, the smallest absolute difference between any two adjacent elements in the sorted array equals the minimum absolute difference between any two elements in the original unsorted list (because sorting preserves adjacency for the closest pairs, assuming the minimum gap is achieved by two adjacent values in the sorted order; if there are duplicates, the gap is zero). After sorting, iterate over the sorted vector from index 1 to `size()-1`, computing `sorted[i] - sorted[i-1]` for each `i`, and track the minimum value. Initialize the minimum with the difference between the first two elements (or a very large number) to ensure at least one comparison. Edge cases: if the vector has exactly two elements, the loop runs once and returns their difference. Duplicates yield a difference of zero, which immediately becomes the minimum; no further comparisons are needed because zero is the theoretical lower bound. If the input is already sorted or reverse-sorted, the algorithm still works because sorting normalizes the order. Time complexity: sorting dominates and takes \(O(n \log n)\) using `std::sort`, and the single pass to find the minimum takes \(O(n)\). Space complexity: we can either copy the vector or sort in place; if we do not want to modify the original, we pass by value or use a copy, giving \(O(n)\) auxiliary space, but we can also sort in place if we are allowed to modify the input (the task does not prohibit modification). For clarity, we will sort a local copy to preserve the caller's data.

#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns the smallest absolute difference between any two adjacent values
// after sorting the input vector in non-decreasing order.
int minAdjacentDifferenceAfterSort(const std::vector<int>& input) {
    // Work on a copy so the original vector remains unchanged.
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    // Initialize with the difference between the first two sorted elements.
    int minimum = sorted[1] - sorted[0];

    // Scan the remaining adjacent pairs.
    for (std::size_t i = 2; i < sorted.size(); ++i) {
        int diff = sorted[i] - sorted[i - 1];
        if (diff < minimum) {
            minimum = diff;
        }
    }

    return minimum;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with a clear minimum.
    assert(minAdjacentDifferenceAfterSort({4, 1, 7, 3}) == 1);
    // Already sorted vector.
    assert(minAdjacentDifferenceAfterSort({1, 2, 3, 4}) == 1);
    // Duplicate values cause zero difference.
    assert(minAdjacentDifferenceAfterSort({5, 2, 8, 2}) == 0);
    // Negative numbers and large gaps.
    assert(minAdjacentDifferenceAfterSort({-10, -3, 5, 100}) == 7); // gaps: 7, 8, 95 → 7
    // Two-element vector.
    assert(minAdjacentDifferenceAfterSort({10, -5}) == 15);
    // Reverse sorted.
    assert(minAdjacentDifferenceAfterSort({9, 6, 3, 1}) == 2); // sorted: 1,3,6,9 → gaps:2,3,3
    // All identical.
    assert(minAdjacentDifferenceAfterSort({7, 7, 7}) == 0);
    // Large gap between two groups.
    assert(minAdjacentDifferenceAfterSort({100, 200, 300, 305}) == 5);
    // Unsorted with duplicates.
    assert(minAdjacentDifferenceAfterSort({-1, 0, -1, 0}) == 0);
    // Mixed positive and negative.
    assert(minAdjacentDifferenceAfterSort({-2, -2, 3, 3}) == 0); // duplicates
}

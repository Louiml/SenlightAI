/*
Given a sequence of `n` (1 ≤ n ≤ 10^5) positive integers `a[0..n-1]`, write a C++ function `int maximumMountainWidth(const std::vector<int>& heights)` that returns the maximum possible width of a "mountain" in the array. A mountain is defined as a contiguous subarray where the heights strictly increase from the start to a peak and then strictly decrease to the end. The width is the number of elements in that subarray. However, the mountain does not need to be contiguous in the original array; instead, you must consider the original positions, and the "height" of each position is constrained by the given value. Specifically, for each index `i`, define `left[i]` as the maximum possible height of a peak at `i` if you go leftwards (always increasing by at least 1 per step, but not exceeding `a[i]`), and `right[i]` similarly going rightwards. The width of a valid mountain centered at `i` is `min(left[i], right[i])`. The answer is the maximum over all `i` of this value. Note that the "width" is not the number of steps but the height of the peak (which is the minimum of the two sides). Return that maximum value.
*/

#include <vector>
#include <algorithm>

// Returns the maximum possible peak height of a "mountain" over all centers.
// A mountain's height at center i is min(left[i], right[i]),
// where left[i] is the max height reachable from the left (increasing by 1 each step)
// and right[i] similarly from the right.
int maximumMountainWidth(const std::vector<int>& heights) {
    const int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    std::vector<int> left(n);
    left[0] = 1;
    for (int i = 1; i < n; ++i) {
        left[i] = std::min(left[i - 1] + 1, heights[i]);
    }

    std::vector<int> right(n);
    right[n - 1] = 1;
    for (int i = n - 2; i >= 0; --i) {
        right[i] = std::min(right[i + 1] + 1, heights[i]);
    }

    int answer = 0;
    for (int i = 0; i < n; ++i) {
        answer = std::max(answer, std::min(left[i], right[i]));
    }
    return answer;
}

#include <cassert>
#include <vector>

int maximumMountainWidth(const std::vector<int>& heights); // declaration from solution

int main() {
    // Single element
    assert(maximumMountainWidth({5}) == 1);

    // Strictly increasing
    assert(maximumMountainWidth({1, 2, 3, 4, 5}) == 3); // center at 2 gives min(left=3, right=3) = 3

    // Strictly decreasing
    assert(maximumMountainWidth({5, 4, 3, 2, 1}) == 3); // center at index 2

    // All equal
    assert(maximumMountainWidth({7, 7, 7, 7}) == 2); // center at index 1: left=2, right=2

    // Mixed values with large peak
    assert(maximumMountainWidth({1, 100, 1}) == 2); // center at 0 or 2 gives 1, center at 1 gives 2

    // Classic mountain
    assert(maximumMountainWidth({2, 5, 7, 5, 2}) == 3); // center at 2

    // Large plateau in middle
    assert(maximumMountainWidth({1, 2, 10, 10, 10, 2, 1}) == 4); // center at 3 gives min(4,4)=4

    // All ones
    assert(maximumMountainWidth({1, 1, 1, 1, 1}) == 1);

    // Two elements
    assert(maximumMountainWidth({3, 3}) == 1);

    // Random edge: high value at start
    assert(maximumMountainWidth({100, 1, 2, 3}) == 2); // center at 0 gives 1, at 1 gives 2, at 2 gives 2, at 3 gives 1

    return 0;
}

// The problem is equivalent to the original snippet. For each index `i`, we compute `left[i]` = the maximum possible "height" we can achieve at `i` when coming from the left, where each step increases the height by at least 1, and the height cannot exceed the given `a[i]`. This is computed by iterating from left to right: `left[0] = 1` (since the mountain starts at 0, the base height is 1), and for `i>0`, `left[i] = min(left[i-1]+1, a[i])`. Similarly, `right[i]` is computed from right to left: `right[n-1] = 1`, and for `i<n-1`, `right[i] = min(right[i+1]+1, a[i])`. Then for each index, the maximum possible peak height at that center is `min(left[i], right[i])`. The answer is the maximum of these over all `i`. Edge cases: when `n=1`, both left and right are 1, answer is 1. If the array is strictly increasing, then `left` will match the array values and `right` will be small at the start, but the max is still handled. If there are large values far apart, the limitation is the consecutive increases of 1 per step, so the peak cannot exceed the index distance from the ends plus the base 1. Time complexity is O(n) and space complexity is O(n) for the two auxiliary vectors.

Write a C++ function `long long trappedWater(const std::vector<int>& heights)` that, given a vector of non-negative integers representing the elevation map (each element is the height of a bar of width 1), computes the total amount of rainwater that can be trapped after rain. The function must handle empty input (return 0) and large heights (up to 1e5 per bar, up to 2e5 bars) without overflow by using `long long` for the result. You must not modify the input vector, and you must implement the solution efficiently using prefix/suffix maximum arrays.
#include <cassert>
#include <vector>

// The function declaration is assumed from the solution above.
long long trappedWater(const std::vector<int>& heights);

int main() {
    // Empty vector
    assert(trappedWater({}) == 0);

    // Single element
    assert(trappedWater({5}) == 0);

    // Classic example
    std::vector<int> heights1 = {0,1,0,2,1,0,1,3,2,1,2,1};
    assert(trappedWater(heights1) == 6);

    // All equal heights
    assert(trappedWater({3,3,3,3}) == 0);

    // Monotonically increasing
    assert(trappedWater({1,2,3,4}) == 0);

    // Monotonically decreasing
    assert(trappedWater({4,3,2,1}) == 0);

    // Single valley
    assert(trappedWater({2,0,2}) == 2);

    // Large values (check for overflow with long long)
    std::vector<int> heights2 = {100000, 0, 100000, 0, 100000};
    assert(trappedWater(heights2) == 200000LL);

    // Two peaks with a dip
    assert(trappedWater({2,1,2,1,2}) == 2);

    // Complex shape
    assert(trappedWater({5,2,1,3,2,4,1,0,3}) == 9);

    return 0;
}
#include <vector>
#include <algorithm>

// Computes total trapped rainwater given an elevation map.
// Each bar has width 1. Returns 0 for empty input.
long long trappedWater(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    // leftMax[i] = maximum height from 0 to i-1 (0 for i=0)
    std::vector<int> leftMax(n, 0);
    for (int i = 1; i < n; ++i) {
        leftMax[i] = std::max(leftMax[i - 1], heights[i - 1]);
    }

    // rightMax[i] = maximum height from i+1 to n-1 (0 for i=n-1)
    std::vector<int> rightMax(n, 0);
    for (int i = n - 2; i >= 0; --i) {
        rightMax[i] = std::max(rightMax[i + 1], heights[i + 1]);
    }

    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        int waterLevel = std::min(leftMax[i], rightMax[i]);
        if (waterLevel > heights[i]) {
            ans += static_cast<long long>(waterLevel - heights[i]);
        }
    }
    return ans;
}
// The key insight is that for any bar at index `i`, the amount of water trapped above it is determined by the minimum of the tallest bar to its left and the tallest bar to its right, minus the height of the current bar. If either side has no taller bar, no water is trapped. To compute this efficiently, precompute two arrays: `leftMax[i]` = maximum height among bars from index `0` to `i-1` (or 0 for `i=0`), and `rightMax[i]` = maximum height among bars from index `i+1` to `n-1` (or 0 for `i=n-1`). Then for each index, compute `waterLevel = min(leftMax[i], rightMax[i])`; if `waterLevel > heights[i]`, add `waterLevel - heights[i]` to the answer. Edge cases: empty vector (return 0), all bars equal (no water), single peak (no water), and monotonically increasing/decreasing (no water). The algorithm runs in O(n) time and uses O(n) auxiliary space for the two arrays. It is optimal for this problem since each bar must be examined at least once, and the prefix/suffix maxima can be computed in linear time.

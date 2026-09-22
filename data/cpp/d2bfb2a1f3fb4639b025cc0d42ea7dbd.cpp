/*
Given a vector of non-negative integers representing an elevation map where the width of each bar is 1, write a C++ function `trapRainWater` that returns the total amount of water that can be trapped between the bars after rain. The input vector may be empty, may contain a single element, or may contain up to 10^4 elements, with each height between 0 and 10^5. The function must handle cases where no water can be trapped (e.g., strictly increasing or decreasing heights) and must compute the result efficiently without modifying the input.
*/

#include <vector>
#include <algorithm>

// Computes the total trapped rainwater in an elevation map.
// Each bar has width 1, heights are non-negative integers.
int trapRainWater(const std::vector<int>& height) {
    const int n = static_cast<int>(height.size());
    if (n < 3) return 0;

    std::vector<int> leftMax(n), rightMax(n);

    // Left-to-right prefix maximum
    leftMax[0] = height[0];
    for (int i = 1; i < n; ++i) {
        leftMax[i] = std::max(leftMax[i - 1], height[i]);
    }

    // Right-to-left suffix maximum
    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        rightMax[i] = std::max(rightMax[i + 1], height[i]);
    }

    int totalWater = 0;
    for (int i = 0; i < n; ++i) {
        const int waterLevel = std::min(leftMax[i], rightMax[i]);
        if (waterLevel > height[i]) {
            totalWater += waterLevel - height[i];
        }
    }
    return totalWater;
}

#include <cassert>
#include <vector>

int main() {
    // Standard example
    assert(trapRainWater({0,1,0,2,1,0,1,3,2,1,2,1}) == 6);
    // Empty vector
    assert(trapRainWater({}) == 0);
    // Single element
    assert(trapRainWater({5}) == 0);
    // Two elements
    assert(trapRainWater({3,2}) == 0);
    // Strictly increasing
    assert(trapRainWater({0,1,2,3,4}) == 0);
    // Strictly decreasing
    assert(trapRainWater({4,3,2,1,0}) == 0);
    // All equal heights
    assert(trapRainWater({2,2,2,2}) == 0);
    // Single peak with valleys on both sides
    assert(trapRainWater({3,0,0,2,0,4}) == 10);
    // Large flat plateau with dips
    assert(trapRainWater({4,2,0,3,2,5}) == 9);
    // All zeros
    assert(trapRainWater({0,0,0,0}) == 0);
    // Large values
    assert(trapRainWater({10000,0,10000}) == 10000);
    return 0;
}

// The classic approach is the "two-pass prefix/suffix maximum" method. For each bar at index `i`, the maximum water height above it is limited by the smaller of (a) the highest bar to its left (including itself) and (b) the highest bar to its right (including itself). Water can only accumulate above a bar if both sides have a bar higher than the current height, and the trapped water at that position is `min(leftMax, rightMax) - height[i]`. We precompute `leftMax[i]` as the maximum height from index `0` to `i`, and `rightMax[i]` as the maximum height from index `i` to `n-1`. Then we iterate once to sum the positive differences. If the vector is empty or has fewer than 3 elements, the result is 0 because no water can be trapped. The algorithm runs in `O(n)` time and uses `O(n)` extra space for the two auxiliary arrays. Edge cases: all equal heights (result 0), a single peak (result 0), and a valley between two tall walls (correct water amount). The solution is robust for all non-negative inputs.

Write a C++ function that takes a vector of non-negative integers representing the heights of vertical bars at each index, and returns the total amount of rainwater that can be trapped between the bars after it rains. The function should compute the maximum water that can be held above each bar, considering that water can only be trapped if there is a bar on both sides that is taller than the current bar. The function must handle vectors of any length, including empty vectors and vectors with fewer than three bars (which cannot trap any water). The function should be efficient enough to handle large inputs, and must not modify the input vector.
// The classic approach uses precomputed prefix and suffix maximum arrays. For each bar at index `i`, the maximum height of a bar to its left (including `i`) is stored in `leftMax[i]`, and the maximum height to its right (including `i`) is stored in `rightMax[i]`. The amount of water trapped above bar `i` is `min(leftMax[i], rightMax[i]) - height[i]`, but only if this value is positive. Summing these values over all interior bars gives the total trapped water. Edge cases: if the vector has 0, 1, or 2 elements, no water can be trapped (return 0). If all bars are the same height, the result is 0. The algorithm runs in O(n) time because it makes three linear passes over the array, and uses O(n) extra space for the two auxiliary arrays. An in-place two-pointer optimization exists but the precomputed array method is straightforward and clear.
#include <vector>
#include <algorithm>

// Computes total trapped rainwater for a vector of non-negative heights.
// Returns 0 for vectors with fewer than 3 elements.
int trapRainwater(const std::vector<int>& height) {
    int n = static_cast<int>(height.size());
    if (n < 3) {
        return 0;
    }

    std::vector<int> leftMax(n, 0);
    std::vector<int> rightMax(n, 0);

    leftMax[0] = height[0];
    for (int i = 1; i < n; ++i) {
        leftMax[i] = std::max(leftMax[i - 1], height[i]);
    }

    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        rightMax[i] = std::max(rightMax[i + 1], height[i]);
    }

    int totalWater = 0;
    for (int i = 1; i < n - 1; ++i) {
        int waterAbove = std::min(leftMax[i], rightMax[i]) - height[i];
        if (waterAbove > 0) {
            totalWater += waterAbove;
        }
    }

    return totalWater;
}
#include <cassert>
#include <vector>

int trapRainwater(const std::vector<int>& height);

int main() {
    std::vector<int> heights1 = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    assert(trapRainwater(heights1) == 6);

    std::vector<int> heights2 = {4, 2, 0, 3, 2, 5};
    assert(trapRainwater(heights2) == 9);

    std::vector<int> heights3 = {1, 0, 1};
    assert(trapRainwater(heights3) == 1);

    std::vector<int> heights4 = {1, 2, 3, 4, 5};
    assert(trapRainwater(heights4) == 0);

    std::vector<int> heights5 = {5, 4, 3, 2, 1};
    assert(trapRainwater(heights5) == 0);

    std::vector<int> heights6 = {3, 3, 3, 3};
    assert(trapRainwater(heights6) == 0);

    std::vector<int> heights7 = {};
    assert(trapRainwater(heights7) == 0);

    std::vector<int> heights8 = {2};
    assert(trapRainwater(heights8) == 0);

    std::vector<int> heights9 = {0, 2, 0};
    assert(trapRainwater(heights9) == 0);

    std::vector<int> heights10 = {2, 0, 2};
    assert(trapRainwater(heights10) == 2);
}

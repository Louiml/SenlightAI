Write a C++ function `long long trappedRainWater(const std::vector<int>& heights)` that takes a vector of non-negative integers representing the elevation map of a landscape, where each integer is the height of a bar of width 1, and returns the total amount of rainwater that can be trapped between the bars after a rain. The function must handle empty vectors (return 0), vectors with fewer than 3 elements (return 0), and large heights (use `long long` to avoid overflow). Do not modify the input vector; use only constant extra space. The solution must be O(n) time and O(1) space.
#include <cassert>
#include <vector>

// Function declaration from the solution (already defined above, but for clarity)
long long trappedRainWater(const std::vector<int>& heights);

int main() {
    // Basic cases
    assert(trappedRainWater({}) == 0);
    assert(trappedRainWater({1}) == 0);
    assert(trappedRainWater({1, 2}) == 0);

    // Example from LeetCode: [0,1,0,2,1,0,1,3,2,1,2,1] -> 6
    assert(trappedRainWater({0,1,0,2,1,0,1,3,2,1,2,1}) == 6);

    // All increasing
    assert(trappedRainWater({1,2,3,4}) == 0);

    // All decreasing
    assert(trappedRainWater({4,3,2,1}) == 0);

    // Valley in middle
    assert(trappedRainWater({3,0,0,2,0,4}) == 10); // 3+3+2+2 = 10

    // Plateaus
    assert(trappedRainWater({2,2,2,2}) == 0);

    // Single spike with both sides low
    assert(trappedRainWater({5,0,5}) == 5);

    // Large heights to test overflow (sum is 1000000000000, fits in long long)
    std::vector<int> large(3);
    large[0] = 1000000;
    large[1] = 0;
    large[2] = 1000000;
    assert(trappedRainWater(large) == 1000000);

    // Long vector with repeated pattern
    std::vector<int> pattern = {1,0,1,0,1,0,1};
    assert(trappedRainWater(pattern) == 3);

    // Two valleys
    assert(trappedRainWater({4,2,0,3,2,5}) == 9);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the total trapped rainwater for the given elevation map.
// The input vector is not modified. Uses O(1) extra space and O(n) time.
long long trappedRainWater(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    if (n < 3) return 0; // Not enough bars to trap water

    int left = 0;
    int right = n - 1;
    int maxLeft = 0;
    int maxRight = 0;
    long long result = 0;

    while (left <= right) {
        if (heights[left] <= heights[right]) {
            if (heights[left] >= maxLeft) {
                maxLeft = heights[left];
            } else {
                result += static_cast<long long>(maxLeft) - heights[left];
            }
            ++left;
        } else {
            if (heights[right] >= maxRight) {
                maxRight = heights[right];
            } else {
                result += static_cast<long long>(maxRight) - heights[right];
            }
            --right;
        }
    }

    return result;
}
// The main idea is to use the two-pointer technique. Intuitively, the amount of water trapped above any bar is limited by the lower of the maximum height to its left and the maximum height to its right. Instead of precomputing prefix and suffix maximum arrays (which would take O(n) space), we maintain two pointers `left` and `right` starting at the ends, and track `maxLeft` and `maxRight` representing the highest bar seen so far from the left and from the right. At each step, we compare `height[left]` and `height[right]`. The side with the smaller current height determines the limiting boundary for that bar: if `height[left] <= height[right]`, then the water above `left` is bounded by `maxLeft` (because `maxRight` is at least `height[right]` which is >= `height[left]`, so the lower of the two maxima is `maxLeft`). If `height[left] >= maxLeft`, we update `maxLeft` to `height[left]` (no water), otherwise we add `maxLeft - height[left]` to the result. Then we move the left pointer inward. Symmetrically for the right side. The loop continues while `left <= right`. Edge cases: empty vector, size 1 or 2 (no trapping possible, return 0). All bars equal height: no water trapped because `maxLeft` and `maxRight` always equal current heights. Rising or descending slope: water only starts accumulating after a higher bar blocks the runoff. Time complexity O(n) because each element is processed exactly once. Space complexity O(1) because only a few scalar variables are used.

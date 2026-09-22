/*
Write a C++ function `trappedWater` that takes a vector of non-negative integers representing the heights of bars in a histogram, and returns the total amount of water (in units) that can be trapped after rain between the bars. The function must be efficient for large inputs (up to 10^5 elements) and must handle empty vectors, vectors of size 1, vectors with all equal heights, and strictly increasing/decreasing sequences correctly. The output must be a `long long` to avoid overflow for large sums.
*/

#include <vector>
#include <algorithm>

// Compute the total amount of trapped rainwater between histogram bars.
// height: non-negative integers representing bar heights.
// Returns the total trapped water as long long.
long long trappedWater(const std::vector<int>& height) {
    int n = static_cast<int>(height.size());
    if (n == 0) return 0;

    std::vector<int> maxLeft(n, 0);
    std::vector<int> maxRight(n, 0);

    // Precompute the maximum height seen from the left up to each index.
    maxLeft[0] = height[0];
    for (int i = 1; i < n; ++i) {
        maxLeft[i] = std::max(maxLeft[i - 1], height[i]);
    }

    // Precompute the maximum height seen from the right up to each index.
    maxRight[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        maxRight[i] = std::max(maxRight[i + 1], height[i]);
    }

    // Sum the water that can be held above each bar.
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        total += static_cast<long long>(std::min(maxLeft[i], maxRight[i]) - height[i]);
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(trappedWater({0,1,0,2,1,0,1,3,2,1,2,1}) == 6);
    assert(trappedWater({4,2,0,3,2,5}) == 9);
    assert(trappedWater({2,0,2}) == 2);
    assert(trappedWater({3,0,0,2,0,4}) == 10);

    // Edge cases
    assert(trappedWater({}) == 0);
    assert(trappedWater({5}) == 0);
    assert(trappedWater({1,2,3,4,5}) == 0);   // increasing
    assert(trappedWater({5,4,3,2,1}) == 0);   // decreasing
    assert(trappedWater({3,3,3,3}) == 0);     // equal heights

    // Large values to test overflow handling
    assert(trappedWater({1000000,0,0,0,1000000}) == 3000000);

    // Duplicate peaks
    assert(trappedWater({1,2,2,1}) == 0);
    assert(trappedWater({0,2,1,2,0}) == 1);

    return 0;
}

// The solution uses the classic "precompute prefix maximum and suffix maximum" approach. For each bar at index `i`, the amount of water it can hold above itself is `min(maxLeft[i], maxRight[i]) - height[i]`, where `maxLeft[i]` is the highest bar to the left (including itself) and `maxRight[i]` is the highest bar to the right (including itself). This works because water can only be trapped up to the lower of the two tallest barriers on either side. The total water is the sum of these positive differences over all bars. Edge cases: if the vector is empty or size 1, no water can be trapped, so return 0. If heights are all equal, the difference is zero for every bar. For strictly increasing or decreasing sequences, one side has no barrier, so water runs off, resulting in zero total. Time complexity is O(n) for three passes (left, right, and sum). Space complexity is O(n) for the two auxiliary arrays (or O(1) if using a two-pointer method, but here we follow the given snippet's approach).

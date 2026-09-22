// Write a C++ function `int trappedRainWater(const std::vector<int>& height)` that computes the total amount of water that can be trapped between the bars of an elevation map, where each element in the vector represents the height of a bar of width 1. The function must process the array in a single pass using two pointers from the left and right ends, maintaining the maximum height seen so far on each side, and adding the difference between the side's running maximum and the current bar's height whenever that bar is lower than the side maximum. The input vector is not modified, may be empty, and all heights are non-negative integers. Return 0 for empty or single-element arrays.

#include <cassert>
#include <vector>
#include <iostream>

int trappedRainWater(const std::vector<int>& height); // Declaration

int main() {
    // Basic cases
    assert(trappedRainWater({0,1,0,2,1,0,1,3,2,1,2,1}) == 6);
    assert(trappedRainWater({4,2,0,3,2,5}) == 9);
    assert(trappedRainWater({1,0,1}) == 1);
    assert(trappedRainWater({1,1,1}) == 0);
    assert(trappedRainWater({0,0,0}) == 0);
    
    // Edge cases
    assert(trappedRainWater({}) == 0);
    assert(trappedRainWater({5}) == 0);
    assert(trappedRainWater({1,2}) == 0);
    assert(trappedRainWater({1,2,3,4,5}) == 0); // increasing, no trap
    assert(trappedRainWater({5,4,3,2,1}) == 0); // decreasing, no trap

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <algorithm>

// Compute total trapped rain water between bars of an elevation map.
// The function uses a two-pointer single-pass approach.
int trappedRainWater(const std::vector<int>& height) {
    int n = static_cast<int>(height.size());
    if (n < 3) return 0; // with 0,1,2 bars no water can be trapped

    int left = 0;
    int right = n - 1;
    int leftMax = 0;
    int rightMax = 0;
    int total = 0;

    while (left < right) {
        if (height[left] <= height[right]) {
            // Process left side; left side is limiting or equal.
            if (leftMax > height[left]) {
                total += leftMax - height[left];
            } else {
                leftMax = height[left];
            }
            ++left;
        } else {
            // Process right side; right side is the limiting lower side.
            if (rightMax > height[right]) {
                total += rightMax - height[right];
            } else {
                rightMax = height[right];
            }
            --right;
        }
    }

    return total;
}

// The algorithm uses two pointers, `left` starting at index 0 and `right` at the last index, along with two variables `leftMax` and `rightMax` initialized to 0. At each step, compare `height[left]` and `height[right]`. If `height[left] <= height[right]`, we process the left side: if `leftMax > height[left]`, that means the current bar can trap `leftMax - height[left]` units of water because the left side has a higher boundary and the right side is at least as high as `height[left]` (guaranteed by the condition). Otherwise, update `leftMax = height[left]`. Then increment `left`. Symmetrically, when `height[right] < height[left]`, process the right side: if `rightMax > height[right]`, add `rightMax - height[right]`; else update `rightMax`. Then decrement `right`. The loop runs while `left < right`. This works because the side with the smaller current height is the limiting side for water trapping; we always move toward the center from the side that is lower, ensuring the running maximum accurately represents the highest boundary seen from that side. Edge cases: empty array, single element, flat terrain (all equal heights), strictly increasing or decreasing terrain (traps 0), and typical valleys (e.g., [0,1,0,2,1,0,1,3,2,1,2,1] traps 6). Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary (excluding input storage).

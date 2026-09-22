/*
Write a C++ function `int trappedWater(const std::vector<int>& height)` that computes the total amount of rainwater that can be trapped between the bars of an elevation map, where each element in the vector represents the height of a bar of width 1. The function must handle empty and single-element vectors correctly, and must not modify the input vector. The solution should not use any additional data structures beyond O(1) extra space (excluding the input and output), and should run in linear time.
*/
#include <vector>
#include <algorithm>

// Computes total trapped rainwater given elevation heights (width 1 each).
// Uses two-pointer technique, O(n) time, O(1) extra space.
int trappedWater(const std::vector<int>& height) {
    if (height.empty()) return 0;

    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int leftMax = 0;
    int rightMax = 0;
    int totalWater = 0;

    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= leftMax) {
                leftMax = height[left];
            } else {
                totalWater += leftMax - height[left];
            }
            ++left;
        } else {
            if (height[right] >= rightMax) {
                rightMax = height[right];
            } else {
                totalWater += rightMax - height[right];
            }
            --right;
        }
    }

    return totalWater;
}
#include <cassert>
#include <vector>

int trappedWater(const std::vector<int>& height); // declaration from solution

int main() {
    // Example from LeetCode: [0,1,0,2,1,0,1,3,2,1,2,1] => 6
    assert(trappedWater({0,1,0,2,1,0,1,3,2,1,2,1}) == 6);

    // Empty vector
    assert(trappedWater({}) == 0);

    // Single element
    assert(trappedWater({5}) == 0);

    // Two elements of equal height -> no water
    assert(trappedWater({3,3}) == 0);

    // Strictly increasing -> no water
    assert(trappedWater({1,2,3,4,5}) == 0);

    // Strictly decreasing -> no water
    assert(trappedWater({5,4,3,2,1}) == 0);

    // Valley: [4,2,0,3,2,5] => 9
    assert(trappedWater({4,2,0,3,2,5}) == 9);

    // All equal heights -> no water
    assert(trappedWater({7,7,7,7}) == 0);

    // Single valley with tall walls: [2,0,2] => 2
    assert(trappedWater({2,0,2}) == 2);

    // Multiple valleys: [3,0,1,3,0,2] => 6
    assert(trappedWater({3,0,1,3,0,2}) == 6);

    return 0;
}
// The classic two-pointer approach is optimal for this problem. Maintain two pointers, `left` and `right`, starting at the ends of the array, and two variables `leftMax` and `rightMax` representing the maximum height seen so far from the left and right sides. At each step, compare the heights at the two pointers. If `height[left] < height[right]`, then the water trapped at `left` depends only on `leftMax` (because the right side has a higher or equal wall). If the current left height is at least `leftMax`, update `leftMax`; otherwise, add `(leftMax - height[left])` to the answer. Then move `left` inward. Symmetrically, if `height[left] >= height[right]`, process the right pointer similarly. This works because water trapped at any position is bounded by the minimum of the maximum heights on its left and right, and the two-pointer method ensures we always process the side with the smaller current boundary, guaranteeing we have the correct limiting max. Edge cases: empty vector returns 0; single element returns 0; all equal heights return 0; a strictly increasing or decreasing array returns 0. Time complexity is O(n) with one pass, and space complexity is O(1) beyond the input.

/*
Write a C++ function that takes a non-empty vector of integers representing the heights of consecutive blocks and returns the total amount of water (as an integer) that can be trapped between them after a rain, assuming each block has unit width and the ground level is uniform. Specifically, water can only be trapped above a block if there is a taller block to its left and a taller block to its right; the trapped water above each block equals the difference between the minimum of the maximum height on its left and right and the block's own height, but only when that difference is positive. Your function must compute the total trapped water efficiently without using additional arrays beyond the input vector. The vector size is at least 1, and heights are non-negative integers.
*/

#include <vector>
#include <algorithm>

// Compute total trapped water between blocks with unit width.
// Input: non-empty vector of non-negative heights.
// Returns: total units of water trapped.
int trappedWater(const std::vector<int>& heights) {
    int left = 0;
    int right = static_cast<int>(heights.size()) - 1;
    int leftMax = 0;
    int rightMax = 0;
    int totalWater = 0;

    while (left < right) {
        if (heights[left] < heights[right]) {
            leftMax = std::max(leftMax, heights[left]);
            totalWater += leftMax - heights[left];
            left++;
        } else {
            rightMax = std::max(rightMax, heights[right]);
            totalWater += rightMax - heights[right];
            right--;
        }
    }
    return totalWater;
}

#include <cassert>
#include <vector>

// Include the solution function declaration here or via header
int trappedWater(const std::vector<int>& heights);

int main() {
    assert(trappedWater({3, 0, 0, 2, 0, 4}) == 10);
    assert(trappedWater({0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}) == 6);
    assert(trappedWater({5}) == 0);
    assert(trappedWater({2, 0, 2}) == 2);
    assert(trappedWater({4, 2, 3}) == 1);
    assert(trappedWater({1, 2, 3, 4}) == 0);
    assert(trappedWater({4, 3, 2, 1}) == 0);
    assert(trappedWater({2, 2, 2}) == 0);
    assert(trappedWater({5, 0, 3}) == 3);
    assert(trappedWater({0, 5, 0, 5, 0}) == 5);
    return 0;
}

// The solution can be derived by observing that water trapped above each block depends only on the maximum height to its left and the maximum height to its right. A two-pointer approach achieves this in a single pass with constant auxiliary space. Initialize two pointers at the leftmost and rightmost ends, track the current maximum heights seen from the left and from the right. At each step, compare the two current maximums; water can only be trapped on the side with the smaller maximum, because the taller side serves as the boundary. Move the pointer from the side with the smaller current maximum inward, adding the difference between that side's current maximum and the block's height (if positive) to the total. Update the side's maximum accordingly. Continue until the pointers meet. Edge cases include a single block (no water), monotonically increasing or decreasing heights (no water), and equal heights (no water). Time complexity is O(n) for n blocks, space complexity is O(1) beyond the input vector.

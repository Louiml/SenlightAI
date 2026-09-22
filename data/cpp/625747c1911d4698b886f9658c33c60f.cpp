/*
Write a C++ function named `largestRectangleArea` that accepts a vector of non-negative integers representing the heights of adjacent bars in a histogram, where each bar has width 1. The function must return the area of the largest rectangle that can be formed entirely within the histogram, meaning the rectangle's height cannot exceed any bar it spans. The function should handle vectors of any length, including empty vectors (return 0) and vectors with duplicate heights. Use an efficient stack-based approach to achieve O(n) time complexity. The function must be self-contained, take the vector by const reference, and return an integer.
*/

#include <vector>
#include <stack>
#include <algorithm>
#include <climits>

// Computes the area of the largest rectangle that can be formed in a histogram.
// The input vector contains the heights of bars, each of width 1.
// Returns 0 for an empty input.
int largestRectangleArea(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    if (n == 0) {
        return 0;
    }

    // nextSmaller[i] = index of nearest bar to the right with height < heights[i]
    // or n if no such bar exists.
    std::vector<int> nextSmaller(n, n);
    std::stack<int> stack;
    for (int i = n - 1; i >= 0; --i) {
        while (!stack.empty() && heights[stack.top()] >= heights[i]) {
            stack.pop();
        }
        nextSmaller[i] = stack.empty() ? n : stack.top();
        stack.push(i);
    }

    // Clear the stack for the next pass.
    while (!stack.empty()) {
        stack.pop();
    }

    // prevSmaller[i] = index of nearest bar to the left with height < heights[i]
    // or -1 if no such bar exists.
    std::vector<int> prevSmaller(n, -1);
    for (int i = 0; i < n; ++i) {
        while (!stack.empty() && heights[stack.top()] >= heights[i]) {
            stack.pop();
        }
        prevSmaller[i] = stack.empty() ? -1 : stack.top();
        stack.push(i);
    }

    int maxArea = 0;
    for (int i = 0; i < n; ++i) {
        int width = nextSmaller[i] - prevSmaller[i] - 1;
        int area = heights[i] * width;
        maxArea = std::max(maxArea, area);
    }

    return maxArea;
}

#include <cassert>
#include <vector>

int main() {
    // Empty input
    assert(largestRectangleArea({}) == 0);

    // Single bar
    assert(largestRectangleArea({5}) == 5);

    // All equal heights
    assert(largestRectangleArea({2, 2, 2}) == 6);

    // Classic example
    assert(largestRectangleArea({2, 1, 5, 6, 2, 3}) == 10);

    // Increasing heights
    assert(largestRectangleArea({1, 2, 3, 4}) == 6);

    // Decreasing heights
    assert(largestRectangleArea({4, 3, 2, 1}) == 6);

    // Zero heights
    assert(largestRectangleArea({0, 0, 0}) == 0);

    // Mixed with zeros
    assert(largestRectangleArea({1, 0, 1, 2}) == 3);

    // Large single bar with zeros around
    assert(largestRectangleArea({0, 7, 0}) == 7);

    // Duplicate heights with gaps
    assert(largestRectangleArea({1, 2, 2, 1}) == 4);

    return 0;
}

// The key insight is that for each bar, the maximum rectangle that uses this bar as the limiting height extends horizontally from the nearest bar to its left that is strictly shorter (or the imaginary bar at index -1) to the nearest bar to its right that is strictly shorter (or the imaginary bar at index n). The width is the difference between these boundaries minus one. To find these boundaries efficiently, we precompute two arrays using monotonic increasing stacks: `prevSmaller[i]` gives the index of the nearest bar to the left with height < heights[i] (or -1 if none), and `nextSmaller[i]` gives the index of the nearest bar to the right with height < heights[i] (or n if none). Then for each i, the candidate area is heights[i] * (nextSmaller[i] - prevSmaller[i] - 1). We take the maximum over all i. Edge cases: empty vector → 0; all equal heights → each bar spans the entire width; a bar that is the smallest in a segment has its boundaries at the sentinels. Time complexity is O(n) because each index is pushed and popped from the stack once per pass. Space complexity is O(n) for the two arrays and the stacks.

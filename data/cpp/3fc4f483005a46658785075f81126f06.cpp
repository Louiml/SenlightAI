// Write a C++ function named `largestRectangleArea` that takes a non-empty `std::vector<int>` representing the heights of adjacent vertical bars (width 1 each) and returns the area of the largest rectangle that can be formed entirely within the histogram. The input heights are non-negative integers. Use a monotonic stack to achieve an efficient solution. Your function must be `const`-correct (i.e., accept the vector by const reference) and must not modify the input.

#include <cassert>
#include <vector>

int main() {
    // Basic case
    assert(largestRectangleArea({2, 1, 5, 6, 2, 3}) == 10);

    // Single bar
    assert(largestRectangleArea({5}) == 5);

    // Single zero
    assert(largestRectangleArea({0}) == 0);

    // All equal heights
    assert(largestRectangleArea({3, 3, 3, 3}) == 12);

    // Increasing heights
    assert(largestRectangleArea({1, 2, 3, 4, 5}) == 9);

    // Decreasing heights
    assert(largestRectangleArea({5, 4, 3, 2, 1}) == 9);

    // Heights with zeros in middle
    assert(largestRectangleArea({2, 0, 2, 0, 2}) == 2);

    // Large rectangle in middle
    assert(largestRectangleArea({6, 2, 5, 4, 5, 1, 6}) == 12);

    // All zeros
    assert(largestRectangleArea({0, 0, 0}) == 0);

    return 0;
}

#include <vector>
#include <stack>
#include <algorithm>

// Compute the area of the largest rectangle in a histogram.
int largestRectangleArea(const std::vector<int>& heights) {
    if (heights.empty()) return 0;

    std::stack<int> stk;          // stack of indices with increasing heights
    int max_area = 0;
    int n = static_cast<int>(heights.size());

    // Process each bar; add a sentinel of height 0 at the end to flush stack.
    for (int i = 0; i <= n; ++i) {
        int current_height = (i == n) ? 0 : heights[i];

        // Maintain monotonic increasing stack (strictly increasing heights).
        while (!stk.empty() && current_height < heights[stk.top()]) {
            int h = heights[stk.top()];
            stk.pop();
            int left = stk.empty() ? -1 : stk.top();
            int width = i - left - 1;
            max_area = std::max(max_area, h * width);
        }
        stk.push(i);
    }

    return max_area;
}

// The key observation is that for each bar, the largest rectangle that includes that bar as the shortest bar extends from the first bar to its left that is shorter, to the first bar to its right that is shorter. To find these boundaries efficiently, we use a monotonic increasing stack that stores indices of bars in increasing order of height. We iterate from left to right. While the current height is less than the height at the stack top, we pop the top index `h`; the popped bar’s right boundary is the current index `i` (exclusive), and its left boundary is the new stack top after popping (or -1 if the stack becomes empty). The width is `(right - left - 1)`, and the area is `height[h] * width`. This process ensures every bar is popped exactly once, so the total time is O(n). To handle cases where the stack never empties (e.g., increasing heights), we append a sentinel height of 0 at the end to force all bars to be processed. Edge cases include an empty input (though the task says non-empty, we can defensively return 0), single bar, all equal heights, and heights of zero. Space complexity is O(n) for the stack.

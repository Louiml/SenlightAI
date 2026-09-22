Write a C++ function that computes the area of the largest rectangle that can be formed within a given histogram, where the histogram is represented by a vector of non-negative integers indicating the heights of consecutive bars of equal width 1. The function should take a `const std::vector<int>&` and return an `int` representing the maximum rectangle area. For example, for heights `{2, 1, 5, 6, 2, 3}`, the maximum area is 10 (formed by bars of heights 5 and 6, or height 2 across the last four bars). The solution must handle empty input (return 0) and single-element input, and must correctly process bars of height zero since they can act as barriers. You may assume all heights are non-negative.
The classic solution uses a monotonic increasing stack that stores indices of bars. We iterate through all bars, and additionally process a sentinel of height 0 at the end to flush the stack. For each current bar height `currHeight`, while the stack is non-empty and the height of the bar at the stack top is greater than `currHeight`, we pop that index. For each popped bar, the height is `heights[top]`, and the width is computed as:
- If the stack is empty after popping, the width extends from index 0 to `i-1`, so width = `i`.
- Otherwise, the width is `i - s.top() - 1` because the new stack top is the nearest bar to the left that is strictly shorter than the popped height. The right boundary is `i-1` (since `i` is the first bar shorter than the popped height). This gives the maximal rectangle using that popped bar as the limiting height.
We update the maximum area as `height * width`. After processing all pops, we push the current index `i`. This guarantees that for each bar, when it is popped, the stack contains indices of bars that are strictly increasing in height, allowing correct width computation. Edge cases: empty input returns 0; single bar of height h returns h; bars of height 0 are handled naturally because they will pop taller bars and then be pushed, acting as a barrier. Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(n) for the stack in the worst case (strictly increasing heights).
#include <vector>
#include <stack>
#include <algorithm>

// Computes the largest rectangle area in a histogram.
// Each bar has width 1; heights are non-negative.
int largestRectangleArea(const std::vector<int>& heights) {
    if (heights.empty()) {
        return 0;
    }

    std::stack<int> s;
    int maxArea = 0;
    const int n = static_cast<int>(heights.size());

    // Process all bars plus a sentinel of height 0 to flush the stack.
    for (int i = 0; i <= n; ++i) {
        int currHeight = (i == n) ? 0 : heights[i];

        while (!s.empty() && heights[s.top()] > currHeight) {
            int height = heights[s.top()];
            s.pop();
            int width = s.empty() ? i : (i - s.top() - 1);
            maxArea = std::max(maxArea, height * width);
        }

        s.push(i);
    }

    return maxArea;
}
#include <cassert>
#include <vector>

// Function declaration (usually provided by the solution)
int largestRectangleArea(const std::vector<int>& heights);

int main() {
    // Empty histogram
    assert(largestRectangleArea({}) == 0);

    // Single bar
    assert(largestRectangleArea({5}) == 5);

    // Single zero-height bar
    assert(largestRectangleArea({0}) == 0);

    // Example from the problem
    assert(largestRectangleArea({2, 1, 5, 6, 2, 3}) == 10);

    // Strictly increasing heights
    assert(largestRectangleArea({1, 2, 3, 4, 5}) == 9);

    // Strictly decreasing heights
    assert(largestRectangleArea({5, 4, 3, 2, 1}) == 9);

    // Heights with zeros acting as barriers
    assert(largestRectangleArea({2, 1, 0, 5, 6}) == 10);

    // All equal heights
    assert(largestRectangleArea({4, 4, 4, 4}) == 16);

    // Mixed heights with zeros in middle
    assert(largestRectangleArea({1, 0, 2, 3, 0, 4}) == 6);

    // Large values
    assert(largestRectangleArea({100, 1, 100}) == 100);

    return 0;
}

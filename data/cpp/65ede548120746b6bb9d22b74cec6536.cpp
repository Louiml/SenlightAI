/*
Write a C++ function `int largestRectangleArea(const std::vector<int>& heights)` that, given a non-empty vector of non-negative integers representing the heights of adjacent bars of width 1 in a histogram, returns the area of the largest rectangle that can be formed entirely within the histogram. The rectangle must have its base on the horizontal axis (the bottom) and its sides aligned with the vertical bars; it may span multiple consecutive bars, and its height is limited by the shortest bar in the span. The input vector is not modified, and the function should handle edge cases such as a single bar, all equal heights, strictly increasing or decreasing heights, and zeros.
*/
#include <vector>
#include <stack>
#include <algorithm>

// Given histogram bar heights (width 1 each), return area of largest rectangle.
// Uses a monotonic increasing stack of indices. The stack contains indices in
// increasing order of height. When a shorter bar is encountered, we pop and
// compute the rectangle for the popped bar, whose width is determined by the
// current index (right boundary) and the new stack top (left boundary, exclusive).
int largestRectangleArea(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    std::stack<int> st;
    st.push(-1); // sentinel as left boundary
    int maxArea = 0;

    for (int i = 0; i < n; ++i) {
        // While current height is less than height at stack top,
        // pop and compute area for the popped bar.
        while (st.top() != -1 && heights[i] < heights[st.top()]) {
            int h = heights[st.top()];
            st.pop();
            int left = st.top();  // index of nearest strictly shorter bar on the left
            int right = i;        // current index is the right boundary
            int width = right - left - 1;
            maxArea = std::max(maxArea, h * width);
        }
        st.push(i);
    }

    // Process remaining bars: their right boundary is n.
    while (st.top() != -1) {
        int h = heights[st.top()];
        st.pop();
        int left = st.top();
        int right = n;
        int width = right - left - 1;
        maxArea = std::max(maxArea, h * width);
    }

    return maxArea;
}
#include <cassert>
#include <vector>

int largestRectangleArea(const std::vector<int>& heights); // declared from solution

int main() {
    // Single bar
    assert(largestRectangleArea({5}) == 5);
    // Two bars, same height
    assert(largestRectangleArea({2, 2}) == 4);
    // Classic example: [2,1,5,6,2,3] => max rectangle height 5 width 2 => 10
    assert(largestRectangleArea({2,1,5,6,2,3}) == 10);
    // Increasing heights
    assert(largestRectangleArea({1,2,3,4,5}) == 9); // rectangle height 3 width 3, or height 4 width 2, etc.
    // Decreasing heights
    assert(largestRectangleArea({5,4,3,2,1}) == 9); // height 3 width 3, etc.
    // All zeros
    assert(largestRectangleArea({0,0,0}) == 0);
    // Mixed with zero: [3,0,2,1] => max area 3 (bar 3) or 2 (bar 2) or 2 (bars 2 and 1)?? Actually [2,1] gives area 2, [3] gives 3, so answer 3
    assert(largestRectangleArea({3,0,2,1}) == 3);
    // Large plateau
    assert(largestRectangleArea({4,4,4,4}) == 16);
    // Random test from known problem
    assert(largestRectangleArea({6,2,5,4,5,1,6}) == 15); // rectangle height 5 width 3 (bars 2,3,4 heights 5,4,5) -> 5*3=15, or height 4 width 3 -> 12, etc.
    return 0;
}
// This is a classic monotonic stack problem. The key observation: for each bar as the shortest bar (the height of a candidate rectangle), the maximum width extends from the nearest bar on the left that is strictly shorter to the nearest bar on the right that is strictly shorter, exclusive. A monotonic increasing stack of indices (by height) lets us process bars left to right. When the current height is less than the height of the bar at the stack's top, the top bar can no longer extend to the right, so we pop it and compute its rectangle area: width = current index - new top index - 1 (after popping, the new top is the nearest shorter bar on the left), and height = popped bar's height. After processing all bars, any remaining bars in the stack are popped with the right boundary equal to `n` (the vector size). To simplify, we can push a sentinel `-1` at the bottom of the stack as a virtual left boundary, and optionally add a sentinel height `-1` at the end of the heights to force all pops, but since the input is `const`, we must process the remaining stack after the loop. The algorithm runs in O(n) time because each index is pushed and popped at most once, and uses O(n) stack space. Edge cases: empty vector is not allowed per the task (but if it were, return 0); single-element vector works; all increasing heights cause all bars to be popped during the final cleanup; all decreasing heights cause pops on every step; zeros produce area 0 for those bars but may allow larger rectangles on higher neighboring bars.

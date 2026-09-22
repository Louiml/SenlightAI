Write a C++ function `int largestRectangleAreaFromWidthsAndHeights(const std::vector<int>& widths, const std::vector<int>& heights)` that takes two equal-length vectors of positive integers. Each pair `(widths[i], heights[i])` represents a rectangle of that width and height. These rectangles are placed side by side (in order) to form a solid histogram-like shape, but in this case, each rectangle is actually composed of `widths[i]` unit-width bars each of height `heights[i]`. The function must return the area of the largest rectangle that can be formed entirely within this histogram (i.e., the largest rectangular subarea that can be inscribed in the shape, aligned with the base). If either vector is empty, return 0. Guarantee all values are between 1 and 100 (inclusive). Do not read input; the function operates on the vectors directly.
// The core problem is the classic “largest rectangle in histogram” but with repeated bars: each input rectangle with width `w` and height `h` is equivalent to `w` consecutive bars each of height `h`. Therefore, expand the `heights` vector into a new vector where each height `h` is repeated `w` times. Then apply the standard stack-based algorithm for the largest rectangle in a histogram. The algorithm maintains a stack of indices with increasing heights. When a new bar is lower than the bar at the stack’s top, we pop from the stack. For each popped bar, its right boundary is the current index `i`, and its left boundary is the new stack top index (or -1 if stack becomes empty). The width is `i - leftBoundary - 1`, and the area is `height * width`. After processing all bars, we do a final pass over the remaining stack elements, computing areas with the right boundary equal to the total number of bars. Edge cases: single bar, decreasing heights, increasing heights, all equal heights, and empty input (return 0). Time complexity is O(totalBars) where totalBars = sum(widths), and O(totalBars) space for the expanded heights array, plus O(totalBars) worst-case for the stack. This is optimal since the expansion itself is linear in the number of unit bars.
#include <vector>
#include <stack>
#include <algorithm>

// Computes the area of the largest rectangle that can be formed within a histogram
// built by placing rectangles (width, height) side by side, where each rectangle
// is treated as `width` unit-width bars of height `height`.
int largestRectangleAreaFromWidthsAndHeights(const std::vector<int>& widths, const std::vector<int>& heights) {
    if (widths.empty() || heights.size() != widths.size()) {
        return 0;
    }

    // Expand the heights into unit-width bars.
    std::vector<int> expanded;
    expanded.reserve(widths.size() * 100); // reasonable upper bound: each width <=100
    for (size_t i = 0; i < widths.size(); ++i) {
        for (int j = 0; j < widths[i]; ++j) {
            expanded.push_back(heights[i]);
        }
    }

    const int n = static_cast<int>(expanded.size());
    if (n == 0) {
        return 0;
    }

    std::stack<int> st;
    int maxArea = 0;

    for (int i = 0; i < n; ++i) {
        // Each bar is a new element; we process it like the standard algorithm.
        // We keep the stack increasing in height (strictly increasing).
        while (!st.empty() && expanded[i] < expanded[st.top()]) {
            int topIndex = st.top();
            st.pop();
            int height = expanded[topIndex];
            int leftBoundary = st.empty() ? -1 : st.top();
            int width = i - leftBoundary - 1;
            maxArea = std::max(maxArea, height * width);
        }
        st.push(i);
    }

    // Process remaining bars in the stack (they have no smaller bar to the right).
    while (!st.empty()) {
        int topIndex = st.top();
        st.pop();
        int height = expanded[topIndex];
        int leftBoundary = st.empty() ? -1 : st.top();
        int width = n - leftBoundary - 1;
        maxArea = std::max(maxArea, height * width);
    }

    return maxArea;
}
#include <cassert>
#include <vector>

// The solution function is declared above (in the same translation unit).
int main() {
    // Single bar
    assert(largestRectangleAreaFromWidthsAndHeights({1}, {5}) == 5);
    // Wide single bar
    assert(largestRectangleAreaFromWidthsAndHeights({5}, {3}) == 15);
    // Two bars equal heights
    assert(largestRectangleAreaFromWidthsAndHeights({1, 2}, {4, 4}) == 12);
    // Decreasing heights: [3, 2, 1] -> max area is 4 (two bars of height 2) or 3 (first bar)
    assert(largestRectangleAreaFromWidthsAndHeights({1, 1, 1}, {3, 2, 1}) == 4);
    // Increasing heights: [1, 2, 3] -> max area is 4 (bars 2 and 3)
    assert(largestRectangleAreaFromWidthsAndHeights({1, 1, 1}, {1, 2, 3}) == 4);
    // Mixed: [2,2,2] heights [1,2,3] -> expanded heights [1,1,2,2,3,3] -> largest rectangle is 6 (height 2 width 3 or height 3 width 2)
    assert(largestRectangleAreaFromWidthsAndHeights({2, 2, 2}, {1, 2, 3}) == 6);
    // Empty input
    assert(largestRectangleAreaFromWidthsAndHeights({}, {}) == 0);
    // Mismatched lengths
    assert(largestRectangleAreaFromWidthsAndHeights({1, 2}, {1}) == 0);
    // All same height but varying widths
    assert(largestRectangleAreaFromWidthsAndHeights({1, 3, 2}, {7, 7, 7}) == 42);
    // Classic case: [2,1,5,6,2,3] -> expected 10
    assert(largestRectangleAreaFromWidthsAndHeights({1,1,1,1,1,1}, {2,1,5,6,2,3}) == 10);
    return 0;
}

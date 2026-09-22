Write a C++ function `int largestRectangleArea(const std::vector<int>& heights)` that, given a vector of non-negative integers representing the heights of adjacent vertical bars of width 1 in a histogram, returns the area of the largest rectangle that can be formed within the histogram. The rectangle must be aligned with the bars (i.e., its base is contiguous bars) and its height cannot exceed the minimum height of those bars. For example, for heights `{2, 1, 5, 6, 2, 3}`, the largest rectangle has area 10 (from bars of heights 5 and 6, height 5, width 2). Handle empty input by returning 0, and handle a single bar correctly. The function should be efficient for vectors up to 10^5 elements.

// The classical optimal solution uses a monotonic increasing stack to find, for each bar, the nearest smaller bar to the left and right, which defines the maximum width of a rectangle of that bar's height. The algorithm iterates through the heights, maintaining a stack of indices such that the heights are strictly increasing. When a bar with height less than or equal to the height at the stack top is encountered, we pop the stack; for each popped index, the current index is the right boundary (exclusive) and the new stack top after popping is the left boundary (exclusive), giving width = current_index - left_boundary - 1, and we compute the area using the popped height. To handle bars that remain in the stack after the loop, we repeat the process with a sentinel of height 0 (or by setting current index to `n`). This computes the largest rectangle in O(n) time, since each bar is pushed and popped once. Space complexity is O(n) in the worst case for the stack. Edge cases: empty vector returns 0; single bar returns its height; all equal heights – the algorithm still works because the condition `<=` ensures correct boundaries; ties are handled correctly by popping on equal heights (gives correct left boundary as the previous smaller bar).

#include <vector>
#include <stack>
#include <algorithm>

// Computes the largest rectangle area in a histogram.
// Each bar has width 1 and the given height.
int largestRectangleArea(const std::vector<int>& heights) {
    if (heights.empty()) {
        return 0;
    }

    std::stack<int> stack;  // store indices with increasing heights
    int max_area = 0;
    int n = static_cast<int>(heights.size());

    for (int i = 0; i <= n; ++i) {
        // Use height 0 as sentinel at the end to flush stack
        int current_height = (i == n) ? 0 : heights[i];

        while (!stack.empty() && current_height <= heights[stack.top()]) {
            int height = heights[stack.top()];
            stack.pop();
            int width = stack.empty() ? i : (i - stack.top() - 1);
            max_area = std::max(max_area, height * width);
        }
        stack.push(i);
    }

    return max_area;
}

#include <cassert>
#include <vector>

int largestRectangleArea(const std::vector<int>& heights);

int main() {
    // Provided example
    assert(largestRectangleArea({2, 1, 5, 6, 2, 3}) == 10);

    // Empty input
    assert(largestRectangleArea({}) == 0);

    // Single bar
    assert(largestRectangleArea({5}) == 5);

    // All increasing
    assert(largestRectangleArea({1, 2, 3, 4, 5}) == 9); // 3*3

    // All decreasing
    assert(largestRectangleArea({5, 4, 3, 2, 1}) == 9);

    // All equal
    assert(largestRectangleArea({3, 3, 3, 3}) == 12);

    // One very tall bar
    assert(largestRectangleArea({2, 1, 100, 1, 2}) == 100);

    // Random values
    assert(largestRectangleArea({1, 2, 3, 2, 1}) == 6); // 2*3

    // Large value edge
    assert(largestRectangleArea({100000, 1, 100000}) == 100000);

    // Zigzag
    assert(largestRectangleArea({1, 5, 1, 5, 1}) == 5);

    return 0;
}

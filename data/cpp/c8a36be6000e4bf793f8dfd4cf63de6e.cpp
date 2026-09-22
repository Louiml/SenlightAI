// Write a C++ function `long long largestRectangleArea(const std::vector<long long>& heights)` that takes a vector of non-negative integers representing the heights of adjacent vertical bars of width 1, and returns the area of the largest rectangle that can be formed within the histogram (i.e., the maximum area of a rectangle whose base lies along the bottom and whose height is equal to the minimum height among a contiguous set of bars, with width equal to the number of bars in that set). The vector size `n` will be between 1 and `10^5`, and each height is a non-negative integer up to `10^9`. The function must handle edge cases including a single bar, all equal heights, and zeros in the array, and must run efficiently.
#include <cassert>
#include <vector>

// The solution function is declared here (as specified in the Solution section).
long long largestRectangleArea(const std::vector<long long>& heights);

int main() {
    // Single bar
    assert(largestRectangleArea({5}) == 5);
    assert(largestRectangleArea({0}) == 0);

    // All equal heights
    assert(largestRectangleArea({2, 2, 2, 2}) == 8);
    assert(largestRectangleArea({3, 3, 3}) == 9);

    // Classic case: heights [2,1,5,6,2,3] -> max area 10
    assert(largestRectangleArea({2,1,5,6,2,3}) == 10);

    // Ascending and descending
    assert(largestRectangleArea({1,2,3,4,5}) == 9);
    assert(largestRectangleArea({5,4,3,2,1}) == 9);

    // With zeros
    assert(largestRectangleArea({0, 4, 0, 3, 2}) == 4);

    // Large values
    assert(largestRectangleArea({1000000000, 1, 1000000000}) == 1000000000);
    assert(largestRectangleArea({1000000000, 1000000000}) == 2000000000LL);

    // Mixed duplicates
    assert(largestRectangleArea({2, 2, 1, 2, 2}) == 6);
    assert(largestRectangleArea({4, 4, 4, 1, 4, 4}) == 12);

    // Already sorted descending
    assert(largestRectangleArea({100, 80, 60, 40, 20}) == 200);

    return 0;
}
#include <vector>
#include <stack>
#include <algorithm>

// Compute the largest rectangle area in a histogram.
long long largestRectangleArea(const std::vector<long long>& heights) {
    const int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    std::vector<int> pre(n), suf(n);
    std::stack<int> left, right;

    // Nearest strictly smaller to the left
    for (int i = 0; i < n; ++i) {
        while (!left.empty() && heights[i] <= heights[left.top()]) {
            left.pop();
        }
        pre[i] = left.empty() ? -1 : left.top();
        left.push(i);
    }

    // Nearest strictly smaller to the right
    for (int i = n - 1; i >= 0; --i) {
        while (!right.empty() && heights[i] <= heights[right.top()]) {
            right.pop();
        }
        suf[i] = right.empty() ? n : right.top();
        right.push(i);
    }

    long long best = 0;
    for (int i = 0; i < n; ++i) {
        long long width = static_cast<long long>(suf[i] - pre[i] - 1);
        best = std::max(best, heights[i] * width);
    }
    return best;
}
// The solution uses the classic "nearest smaller element" technique with stacks. For each bar `i`, we compute `pre[i]` = the index of the nearest bar to the left that is strictly smaller than `heights[i]` (or -1 if none), and `suf[i]` = the index of the nearest bar to the right that is strictly smaller than `heights[i]` (or `n` if none). This is done by scanning left-to-right and right-to-left while maintaining a monotonic increasing stack (strictly increasing in terms of height). With these indices, the maximum rectangle that includes bar `i` as the shortest bar has width `suf[i] - pre[i] - 1` and height `heights[i]`, so the candidate area is `heights[i] * (suf[i] - pre[i] - 1)`. The answer is the maximum of these candidates over all bars. Edge cases: single bar (pre=-1, suf=n, width=n, correct), duplicate heights are handled by using strict inequality when popping (so for equal heights, the left smaller stops at the first equal element, and the right smaller stops at the first equal from the right, ensuring each rectangle is counted once correctly), and zeros naturally yield zero area. Time complexity is O(n) because each index is pushed and popped at most once per stack; space complexity is O(n) for the stacks and the pre/suf arrays.

// Write a C++ function named `largestRectangularArea` that takes a vector of non-negative integers representing the heights of adjacent bars in a histogram and returns the maximum rectangular area that can be formed by a contiguous set of bars. The width of each bar is assumed to be 1. The function must handle empty vectors (return 0), vectors with a single bar, and vectors with duplicate heights. You may not sort the array or reorder the bars; the solution must process the bars in their original order.

// The optimal solution uses the concept of "nearest smaller elements" on both sides of each bar. For each bar at index `i`, we need to find the index of the nearest bar to the left that is strictly shorter (`pse` – previous smaller element) and the nearest bar to the right that is strictly shorter (`nse` – next smaller element). Then the maximum rectangle that can use this bar as the shortest bar in the rectangle has width `(nse[i] - pse[i] - 1)` and height `arr[i]`. The answer is the maximum over all bars of `arr[i] * width`. To compute `pse` and `nse` efficiently, we use two monotonic stacks, each traversing the array once (left-to-right for `pse`, right-to-left for `nse`). The stack stores indices; while the stack is non-empty and the current height is less than or equal to the height at the stack top, we pop. After popping, if the stack is empty, the boundary is -1 for `pse` (out of bounds) or `n` for `nse` (out of bounds); otherwise the top is the nearest smaller index. Edge cases: empty input returns 0; a single bar has width 1; duplicate heights are handled correctly because we use `<=` when popping, ensuring we find the nearest strictly smaller element. Time complexity is O(n) because each index is pushed and popped at most once per pass. Space complexity is O(n) for the two stacks and the `pse`/`nse` vectors.

#include <vector>
#include <stack>
#include <algorithm>

// Return the maximum rectangular area in a histogram with bar heights given by arr.
// Each bar has width 1. Empty input yields area 0.
int largestRectangularArea(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    std::vector<int> pse(n, -1);  // index of nearest smaller element to the left
    std::vector<int> nse(n, n);   // index of nearest smaller element to the right

    std::stack<int> st;

    // Compute previous smaller element for each index.
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[i] <= arr[st.top()]) {
            st.pop();
        }
        pse[i] = st.empty() ? -1 : st.top();
        st.push(i);
    }

    // Clear stack for next pass.
    while (!st.empty()) st.pop();

    // Compute next smaller element for each index.
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && arr[i] <= arr[st.top()]) {
            st.pop();
        }
        nse[i] = st.empty() ? n : st.top();
        st.push(i);
    }

    int maxArea = 0;
    for (int i = 0; i < n; ++i) {
        int width = nse[i] - pse[i] - 1;
        maxArea = std::max(maxArea, arr[i] * width);
    }

    return maxArea;
}

#include <cassert>
#include <vector>

// Declaration of the tested function (assumed to be defined elsewhere)
int largestRectangularArea(const std::vector<int>& arr);

int main() {
    // Basic sample from problem statement
    std::vector<int> hist1 = {6, 2, 5, 4, 5, 1, 6};
    assert(largestRectangularArea(hist1) == 12);

    // Empty input
    std::vector<int> empty;
    assert(largestRectangularArea(empty) == 0);

    // Single bar
    std::vector<int> single = {7};
    assert(largestRectangularArea(single) == 7);

    // All bars of same height
    std::vector<int> same = {3, 3, 3, 3};
    assert(largestRectangularArea(same) == 12);

    // Strictly increasing heights
    std::vector<int> increasing = {1, 2, 3, 4};
    assert(largestRectangularArea(increasing) == 6);

    // Strictly decreasing heights
    std::vector<int> decreasing = {4, 3, 2, 1};
    assert(largestRectangularArea(decreasing) == 6);

    // Contains zeros
    std::vector<int> withZero = {2, 1, 0, 3, 4};
    assert(largestRectangularArea(withZero) == 6);

    // All zeros
    std::vector<int> allZero = {0, 0, 0};
    assert(largestRectangularArea(allZero) == 0);

    // Large equal-width rectangle from a single tall bar
    std::vector<int> wide = {5, 5, 5, 5, 5};
    assert(largestRectangularArea(wide) == 25);

    return 0;
}

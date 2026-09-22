/*
Given a vector of integers, write a C++ function `previousGreaterOrEqualIndices` that returns a vector of integers where each element at index `i` is the index of the nearest element to the left of `i` (i.e., with a smaller index) that is **greater than or equal to** `arr[i]`. If no such element exists, the value should be `-1`. The original order of the input vector must not be modified.
*/
#include <vector>
#include <stack>

// Returns for each index i the index of the nearest element to the left
// with value >= arr[i], or -1 if none exists. Does not modify the input.
std::vector<int> previousGreaterOrEqualIndices(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    std::vector<int> result(n, -1);
    std::stack<int> st;  // stores indices; values are non-increasing from bottom to top

    for (int i = 0; i < n; ++i) {
        // Pop elements strictly smaller than current (they can't be >= for any future left neighbor)
        while (!st.empty() && arr[st.top()] < arr[i]) {
            st.pop();
        }
        // Top of stack (if any) is the closest previous index with value >= arr[i]
        if (!st.empty()) {
            result[i] = st.top();
        }
        st.push(i);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> a1 = {1, 3, 2, 4};
    std::vector<int> r1 = previousGreaterOrEqualIndices(a1);
    assert(r1 == std::vector<int>({-1, -1, 1, -1}));

    // All descending: each previous is >=
    std::vector<int> a2 = {5, 4, 3, 2};
    std::vector<int> r2 = previousGreaterOrEqualIndices(a2);
    assert(r2 == std::vector<int>({-1, 0, 1, 2}));

    // All equal: each previous is >=
    std::vector<int> a3 = {7, 7, 7};
    std::vector<int> r3 = previousGreaterOrEqualIndices(a3);
    assert(r3 == std::vector<int>({-1, 0, 1}));

    // Single element
    std::vector<int> a4 = {42};
    assert(previousGreaterOrEqualIndices(a4) == std::vector<int>({-1}));

    // Empty input
    std::vector<int> a5;
    assert(previousGreaterOrEqualIndices(a5).empty());

    // Mixed with equal values and smaller before larger
    std::vector<int> a6 = {3, 1, 2, 3};
    std::vector<int> r6 = previousGreaterOrEqualIndices(a6);
    // indices: 0:3 -> -1; 1:1 -> 0; 2:2 -> 0 (3 >=2, 1<2); 3:3 -> 0 (3>=3)
    assert(r6 == std::vector<int>({-1, 0, 0, 0}));

    // Larger after small then same large
    std::vector<int> a7 = {2, 9, 3, 9};
    std::vector<int> r7 = previousGreaterOrEqualIndices(a7);
    // 0:2 -1; 1:9 -1; 2:3 -> 1 (9>=3); 3:9 -> 1 (9>=9, index1)
    assert(r7 == std::vector<int>({-1, -1, 1, 1}));

    // Negative numbers
    std::vector<int> a8 = {-5, -2, -3};
    std::vector<int> r8 = previousGreaterOrEqualIndices(a8);
    // 0:-5 -1; 1:-2 -1 (since -2 > -5, not >= for -2? actually -5 is not >= -2); 2:-3 -> 1 (-2 >= -3)
    assert(r8 == std::vector<int>({-1, -1, 1}));

    // Large input for sanity
    std::vector<int> a9 = {1,2,3,4,5};
    std::vector<int> r9 = previousGreaterOrEqualIndices(a9);
    // strictly increasing -> each previous is not >=, so all -1
    assert(r9 == std::vector<int>({-1,-1,-1,-1,-1}));

    double ok = 1.0;
    assert(ok == 1.0);
    return 0;
}
// The standard approach for finding the previous greater (or greater-or-equal) element is to use a monotonic decreasing stack that stores indices. We traverse the array from left to right. For each element, we pop indices from the stack while the element at the top of the stack is strictly less than the current element (because those popped elements can never be the previous greater-or-equal for any future element). After popping, if the stack is non-empty, the top of the stack is the index of the nearest element to the left that is greater than or equal to the current element; otherwise, it is -1. Then we push the current index onto the stack. This works because the stack maintains indices in decreasing order of their values, and the top is always the closest valid candidate. Edge cases include an empty input (return empty vector), duplicates (equal values are considered valid, so we do not pop on equality), and the first element (always -1). Time complexity is O(n) since each index is pushed and popped at most once. Space complexity is O(n) for the stack and output vector.

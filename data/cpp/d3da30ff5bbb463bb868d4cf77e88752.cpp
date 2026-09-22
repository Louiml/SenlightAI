// Given an array of `n` integers (1 ≤ n ≤ 10^5), write a C++ function that returns a new array (as `std::vector<int>`) where for each index `i`, the result is: first find the nearest element to the right of `i` that is strictly greater than `a[i]` (if none exists, the result is `-1`). If such a greater element exists at index `j`, then find the nearest element to the right of `j` that is strictly smaller than `a[j]` (if none exists, the result is `-1`). The output for index `i` is the value of that smaller element (or `-1` if any step fails). If no such chain exists, output `-1` for that index. The function must handle duplicates correctly (strict comparisons only) and work in linear time.
The task can be decomposed into two separate monotonic stack passes. First, compute `nextGreater[i]`: for each `i`, traverse from right to left, maintaining a stack of indices with strictly increasing values (from bottom to top, values are non-decreasing? Actually we pop while `a[i] >= a[st.top()]` so the stack holds indices with values strictly greater than the current element after popping. The top of stack gives the nearest index to the right with a strictly greater value). If the stack is empty, set `nextGreater[i] = -1`. In a second pass, compute `nextSmaller[i]`: similarly traverse from right to left, maintaining a stack of indices with strictly decreasing values (pop while `a[i] <= a[st.top()]`), so the top gives the nearest index to the right with a strictly smaller value. If stack empty, set `nextSmaller[i] = -1`. Then for each `i`, if `nextGreater[i] != -1` and `nextSmaller[nextGreater[i]] != -1`, the result is `a[nextSmaller[nextGreater[i]]]`, else `-1`. Edge cases: arrays of length 1, all equal elements (no strictly greater or smaller), and elements that have a greater but that greater has no smaller to its right (e.g., all increasing). Time complexity is O(n) per pass, total O(n), space O(n) for the stacks and result arrays.
#include <vector>
#include <stack>

// Returns an array where for each i, result[i] is the value of the nearest
// smaller element to the right of the nearest greater element to the right of i.
std::vector<int> computeTransformedNext(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> nextGreater(n, -1);
    std::vector<int> nextSmaller(n, -1);
    std::vector<int> result(n, -1);

    // First pass: find nearest index to the right with a strictly greater value.
    std::stack<int> st;
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && a[i] >= a[st.top()]) {
            st.pop();
        }
        if (!st.empty()) {
            nextGreater[i] = st.top();
        }
        st.push(i);
    }

    // Second pass: find nearest index to the right with a strictly smaller value.
    std::stack<int> stSmall;
    for (int i = n - 1; i >= 0; --i) {
        while (!stSmall.empty() && a[i] <= a[stSmall.top()]) {
            stSmall.pop();
        }
        if (!stSmall.empty()) {
            nextSmaller[i] = stSmall.top();
        }
        stSmall.push(i);
    }

    // Combine: for each i, go to nextGreater[i], then to its nextSmaller.
    for (int i = 0; i < n; ++i) {
        int j = nextGreater[i];
        if (j != -1 && nextSmaller[j] != -1) {
            result[i] = a[nextSmaller[j]];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Include or paste the solution function here.

int main() {
    // Example from typical test:
    std::vector<int> a1 = {4, 5, 2, 10, 8};
    std::vector<int> r1 = computeTransformedNext(a1);
    std::vector<int> expected1 = {-1, 2, -1, 8, -1};
    assert(r1 == expected1);

    // All equal: no greater or smaller exists.
    std::vector<int> a2 = {3, 3, 3};
    std::vector<int> r2 = computeTransformedNext(a2);
    std::vector<int> expected2 = {-1, -1, -1};
    assert(r2 == expected2);

    // Single element.
    std::vector<int> a3 = {7};
    std::vector<int> r3 = computeTransformedNext(a3);
    std::vector<int> expected3 = {-1};
    assert(r3 == expected3);

    // Strictly decreasing: every element has a greater? no, but for each i, nextGreater is -1 (since no greater to right) except last? Let's check: [5,4,3] -> for index 0, nextGreater? to right, 4 is less, 3 less, none greater -> -1. But index 0 has nextGreater -1, result -1. Similarly all -1.
    std::vector<int> a4 = {5, 4, 3};
    std::vector<int> r4 = computeTransformedNext(a4);
    std::vector<int> expected4 = {-1, -1, -1};
    assert(r4 == expected4);

    // Strictly increasing: each element has a greater, but that greater may have a smaller? For [1,2,3]: i=0, nextGreater=1 (value2), nextSmaller of index1 (value2) is -1 (since to right, 3 is greater, no smaller) -> result -1. i=1, nextGreater=2 (value3), nextSmaller of index2 is -1 -> -1. i=2 -1.
    std::vector<int> a5 = {1, 2, 3};
    std::vector<int> r5 = computeTransformedNext(a5);
    std::vector<int> expected5 = {-1, -1, -1};
    assert(r5 == expected5);

    // Mixed case: [2, 4, 1, 3, 5] 
    // i=0: nextGreater=1 (value4), nextSmaller of index1? from index1 (value4) to right, nearest smaller is index2 (value1) -> result 1.
    // i=1: nextGreater=4 (value5), nextSmaller of index4? to right none -> -1.
    // i=2: nextGreater=3 (value3), nextSmaller of index3? to right, nearest smaller? index3 value3, to right index4 value5 greater, none smaller -> -1.
    // i=3: nextGreater=4 (value5), nextSmaller of index4? none -> -1.
    // i=4: -1.
    // So expected: {1, -1, -1, -1, -1}
    std::vector<int> a6 = {2, 4, 1, 3, 5};
    std::vector<int> r6 = computeTransformedNext(a6);
    std::vector<int> expected6 = {1, -1, -1, -1, -1};
    assert(r6 == expected6);

    // Duplicates with a greater: {3, 5, 3, 4} 
    // i=0: nextGreater=1 (5), nextSmaller of index1? to right: 3 (index2) is smaller -> result 3.
    // i=1: nextGreater=-1 -> -1.
    // i=2: nextGreater=3 (4), nextSmaller of index3? to right none -> -1.
    // i=3: -1.
    std::vector<int> a7 = {3, 5, 3, 4};
    std::vector<int> r7 = computeTransformedNext(a7);
    std::vector<int> expected7 = {3, -1, -1, -1};
    assert(r7 == expected7);

    // Large n test to ensure performance (but not asserted, just run)
    // Construct array of size 1000 with alternating pattern
    std::vector<int> a8;
    a8.reserve(1000);
    for (int i = 0; i < 1000; ++i) a8.push_back((i % 2 == 0) ? 1000 - i : i);
    std::vector<int> r8 = computeTransformedNext(a8);
    assert(r8.size() == 1000); // just ensure no crash

    return 0;
}

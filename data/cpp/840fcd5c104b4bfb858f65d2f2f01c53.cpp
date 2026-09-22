Write a C++ function that sorts a `std::stack<int>` in non-decreasing order (smallest at the bottom, largest at the top) using only recursion. You may not use loops, arrays, vectors, or any other data structure to hold elements during the sorting process. The initial stack can contain duplicate values and may be empty. The function should modify the stack in place and return `void`. After the function returns, the stack must satisfy the property that `stack.top()` is the maximum element, and each element below is less than or equal to the element above it.
// The core idea is to break the problem recursively: remove the top element, recursively sort the remaining stack (which is smaller), then insert the removed element into its correct position in the already sorted stack. The insertion is done via a helper recursive function `insertSorted` that assumes the given stack is already sorted in non-decreasing order (top is max). This helper compares the element to insert with the current top: if the stack is empty or the element is greater than or equal to the top, push it; otherwise, temporarily remove the top, recursively insert the element into the smaller stack, then push the top back to restore the stack. This backtracking ensures no extra storage beyond the call stack. Edge cases include an empty input stack (do nothing) and duplicate values (use `>=` in comparison so duplicates are placed above equal values, preserving the required order). Time complexity is O(n²) in the worst case because for each of the n elements during the main recursion, insertion may traverse up to n elements. Space complexity is O(n) due to the recursion call stack (no auxiliary heap storage).
#include <stack>

// Insert value into a stack that is already sorted with largest at top.
// Keeps the stack sorted after insertion.
void insertSorted(std::stack<int>& stk, int value) {
    if (stk.empty() || value >= stk.top()) {
        stk.push(value);
        return;
    }
    int top = stk.top();
    stk.pop();
    insertSorted(stk, value);
    stk.push(top);
}

// Sort a stack in non-decreasing order (largest at top) using recursion only.
void sortStackRecursive(std::stack<int>& stk) {
    if (stk.empty()) {
        return;
    }
    int top = stk.top();
    stk.pop();
    sortStackRecursive(stk);
    insertSorted(stk, top);
}
#include <cassert>
#include <stack>

// The solution functions are assumed to be included above.

int main() {
    // Empty stack
    std::stack<int> s1;
    sortStackRecursive(s1);
    assert(s1.empty());

    // Single element
    std::stack<int> s2;
    s2.push(5);
    sortStackRecursive(s2);
    assert(s2.size() == 1);
    assert(s2.top() == 5);

    // Already sorted (largest on top)
    std::stack<int> s3;
    s3.push(1);
    s3.push(2);
    s3.push(3);
    sortStackRecursive(s3);
    assert(s3.top() == 3);
    s3.pop();
    assert(s3.top() == 2);
    s3.pop();
    assert(s3.top() == 1);
    s3.pop();
    assert(s3.empty());

    // Reverse sorted (largest at bottom)
    std::stack<int> s4;
    s4.push(3);
    s4.push(2);
    s4.push(1);
    sortStackRecursive(s4);
    assert(s4.top() == 3);
    s4.pop();
    assert(s4.top() == 2);
    s4.pop();
    assert(s4.top() == 1);
    s4.pop();
    assert(s4.empty());

    // Duplicates
    std::stack<int> s5;
    s5.push(2);
    s5.push(1);
    s5.push(2);
    s5.push(1);
    sortStackRecursive(s5);
    assert(s5.top() == 2);
    s5.pop();
    assert(s5.top() == 2);
    s5.pop();
    assert(s5.top() == 1);
    s5.pop();
    assert(s5.top() == 1);
    s5.pop();
    assert(s5.empty());

    // Larger unsorted with negatives and zeros
    std::stack<int> s6;
    int input[] = {-3, 0, 5, -1, 2, 0, -3, 4};
    for (int v : input) {
        s6.push(v);
    }
    sortStackRecursive(s6);
    int expected[] = {4, 2, 0, 0, -1, -3, -3};
    for (int e : expected) {
        assert(s6.top() == e);
        s6.pop();
    }
    assert(s6.empty());

    return 0;
}

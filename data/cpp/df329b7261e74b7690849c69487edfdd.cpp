/*
Write a C++ function `previousGreaterOrEqual` that takes a non-empty vector of integers and returns a new vector of the same size where each element at index `i` (for `i >= 1`) contains the value of the nearest element to the left of `arr[i]` that is **greater than or equal to** `arr[i]`. If no such element exists, the output at that index should be `-1`. For the first element (index 0), the output is always `-1`. The function must preserve the relative order of the input values and handle duplicate values correctly—for example, if an equal value exists to the left, that equal value is the valid predecessor (since the condition is `>=`). The function should work for vectors of any size (including size 1) and for arbitrary integer values, including negatives. Do not modify the input vector.
*/
#include <vector>
#include <stack>

// For each index i, return the value of the nearest element to the left
// that is >= arr[i], or -1 if none exists. Index 0 always gets -1.
std::vector<int> previousGreaterOrEqual(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    std::vector<int> result(n, -1);
    std::stack<int> st;  // stores values of candidate predecessors

    if (n == 0) return result;  // edge case though problem says non-empty

    st.push(arr[0]);  // first element as candidate for later positions
    // result[0] is already -1

    for (int i = 1; i < n; ++i) {
        // Pop elements that are strictly less than current (they can't be valid)
        while (!st.empty() && st.top() < arr[i]) {
            st.pop();
        }
        // Now top is >= arr[i] if stack not empty
        if (!st.empty()) {
            result[i] = st.top();
        }
        // Push current value for future comparisons
        st.push(arr[i]);
    }

    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// The free function is declared in the solution above.
// We'll include it here via a copy for testing purposes.
std::vector<int> previousGreaterOrEqual(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    std::vector<int> result(n, -1);
    std::stack<int> st;
    if (n == 0) return result;
    st.push(arr[0]);
    for (int i = 1; i < n; ++i) {
        while (!st.empty() && st.top() < arr[i]) st.pop();
        if (!st.empty()) result[i] = st.top();
        st.push(arr[i]);
    }
    return result;
}

int main() {
    // Test 1: Given example
    std::vector<int> input1 = {1, 9, 4, 78, 9, 23, 74, 0, 1, 45, 67};
    std::vector<int> expected1 = {-1, -1, 9, -1, 78, 78, 78, 74, 1, 74, 74};
    assert(previousGreaterOrEqual(input1) == expected1);

    // Test 2: Single element
    std::vector<int> input2 = {5};
    std::vector<int> expected2 = {-1};
    assert(previousGreaterOrEqual(input2) == expected2);

    // Test 3: Strictly increasing
    std::vector<int> input3 = {1, 2, 3, 4};
    std::vector<int> expected3 = {-1, -1, -1, -1};
    assert(previousGreaterOrEqual(input3) == expected3);

    // Test 4: Strictly decreasing (each previous is >= current)
    std::vector<int> input4 = {10, 9, 8, 7};
    std::vector<int> expected4 = {-1, 10, 9, 8};
    assert(previousGreaterOrEqual(input4) == expected4);

    // Test 5: Duplicate values
    std::vector<int> input5 = {3, 3, 3};
    std::vector<int> expected5 = {-1, 3, 3};
    assert(previousGreaterOrEqual(input5) == expected5);

    // Test 6: Mixed with negatives and zero
    std::vector<int> input6 = {-2, 0, -1, 5, -3};
    std::vector<int> expected6 = {-1, -2, 0, -1, 5};
    assert(previousGreaterOrEqual(input6) == expected6);

    // Test 7: Input not modified (copy check)
    std::vector<int> original = {4, 2, 5, 2};
    std::vector<int> copy = original;
    auto result7 = previousGreaterOrEqual(original);
    assert(original == copy);
    std::vector<int> expected7 = {-1, 4, 4, 5};
    assert(result7 == expected7);

    // Test 8: Large value at the end
    std::vector<int> input8 = {1, 2, 3, 100};
    std::vector<int> expected8 = {-1, -1, -1, -1};
    assert(previousGreaterOrEqual(input8) == expected8);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem asks for the nearest previous element that is **greater than or equal** to the current element. A monotonic stack works efficiently here. We iterate from left to right. The stack maintains indices (or values) of elements that are potentially useful for future queries, but crucially, we need to handle the `>=` condition carefully. The standard "next greater" stack pattern pops while the top is **strictly less** than the current element, because those popped elements are too small to be valid predecessors for the current or any later element (since the current element is larger and closer). After popping all smaller elements, if the stack is empty, no valid predecessor exists (−1). Otherwise, the top of the stack is the nearest previous element that is `>=` the current value. Then we push the current element's value (or index) onto the stack. This works because if a later element finds the current element too small, it also wouldn’t be valid for earlier elements, so popping is safe.
//
// Edge cases: 
// - The first element always has no left neighbor, so output is -1.
// - Negative numbers and zeros are handled naturally since comparisons are numeric.
// - Duplicate values: if `st.top() == arr[i]`, we do **not** pop (because `st.top() < arr[i]` is false), so the equal value becomes the valid predecessor. This correctly gives the nearest `>=` element.
// - If the stack becomes empty, that means no element to the left is `>=` current, so output -1.
//
// Time complexity: O(n) because each element is pushed and popped at most once. Space complexity: O(n) for the output vector and O(n) in the worst case for the stack (e.g., strictly decreasing input).

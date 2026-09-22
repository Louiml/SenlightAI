// Write a C++ function `nextGreaterElement` that takes a non-empty vector of integers and returns a vector of integers of the same size, where each position contains the next greater element to the right of the corresponding element in the input. If no such element exists, the output position should be `-1`. The input may contain duplicate values, negative numbers, and elements in any order. The function must not modify the original input and should be implemented efficiently using a stack-based approach. The result should be returned by value.

// The problem is a classic "Next Greater Element" (NGE) problem. The efficient solution uses a monotonic decreasing stack that stores indices (not values). We iterate through the array from left to right. For each current element, while the stack is non-empty and the current element is greater than the element at the index on the top of the stack, we have found the NGE for that top index — so we assign the current element to the output at that top index and pop it. Then we push the current index. After the loop, any remaining indices in the stack have no greater element to their right, so their output is set to `-1`. Edge cases: empty input (though the task states non-empty, we still handle gracefully), all decreasing sequence (all outputs `-1`), duplicates (only strictly greater values count, not equal), and single-element input (output `-1`). Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(n) for the output vector and the stack.

#include <vector>
#include <stack>

// Returns a vector where each position i contains the next greater element
// to the right of arr[i], or -1 if no such greater element exists.
std::vector<int> nextGreaterElement(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return {};

    std::vector<int> output(n, -1);
    std::stack<int> st;  // stores indices

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && arr[i] > arr[st.top()]) {
            output[st.top()] = arr[i];
            st.pop();
        }
        st.push(i);
    }

    // Remaining indices have no greater element; output is already -1.
    return output;
}

#include <cassert>
#include <vector>

// Assume the solution function nextGreaterElement is declared as above.

int main() {
    std::vector<int> input1 = {1, 6, 3, 5, 6, 3, 2, 5, 9, 4, 2, 3};
    std::vector<int> expected1 = {6, 9, 5, 6, 9, 5, 5, 9, -1, -1, 3, -1};
    assert(nextGreaterElement(input1) == expected1);

    std::vector<int> input2 = {4};
    std::vector<int> expected2 = {-1};
    assert(nextGreaterElement(input2) == expected2);

    std::vector<int> input3 = {5, 4, 3, 2, 1};
    std::vector<int> expected3 = {-1, -1, -1, -1, -1};
    assert(nextGreaterElement(input3) == expected3);

    std::vector<int> input4 = {1, 2, 3, 4, 5};
    std::vector<int> expected4 = {2, 3, 4, 5, -1};
    assert(nextGreaterElement(input4) == expected4);

    std::vector<int> input5 = {2, 2, 2, 2};
    std::vector<int> expected5 = {-1, -1, -1, -1};
    assert(nextGreaterElement(input5) == expected5);

    std::vector<int> input6 = {-5, -1, -10, -2};
    std::vector<int> expected6 = {-1, -10, -2, -1};
    assert(nextGreaterElement(input6) == expected6);

    std::vector<int> input7 = {10, 3, 7, 7, 1, 5};
    std::vector<int> expected7 = {-1, 7, -1, -1, 5, -1};
    assert(nextGreaterElement(input7) == expected7);

    // Verify the original vector is not modified
    std::vector<int> original = {1, 6, 3, 5, 6, 3, 2, 5, 9, 4, 2, 3};
    std::vector<int> copy = original;
    nextGreaterElement(original);
    assert(original == copy);
}

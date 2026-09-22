/*
Write a C++ function that takes a non-empty vector of integers and returns a vector of the same length where each position `i` contains the next greater element to the right of `arr[i]` in the original array. "Next greater element" means the first element encountered when scanning to the right of `arr[i]` that is strictly greater than `arr[i]`. If no such element exists, the output at that position should be `-1`. The function must handle duplicate values (a greater element must be strictly greater, not equal), and it must not modify the input vector. For efficiency, you must implement the algorithm in linear time using a stack-based approach rather than a brute-force double loop.
*/

#include <vector>
#include <stack>

// Return a vector where result[i] is the next strictly greater element to the right of arr[i].
// If none exists, result[i] is -1.
std::vector<int> nextGreaterElementToRight(const std::vector<int>& arr) {
    std::vector<int> result(arr.size(), -1);
    std::stack<int> st;  // stores candidate greater elements, maintained in decreasing order
    
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        while (!st.empty() && st.top() <= arr[i]) {
            st.pop();
        }
        if (!st.empty()) {
            result[i] = st.top();
        }
        st.push(arr[i]);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Example from the original snippet
    std::vector<int> arr1 = {2, 5, 8, 4, 6};
    std::vector<int> res1 = nextGreaterElementToRight(arr1);
    assert((res1 == std::vector<int>{5, 8, -1, 6, -1}));

    // Strictly decreasing array → all -1
    std::vector<int> arr2 = {5, 4, 3, 2, 1};
    std::vector<int> res2 = nextGreaterElementToRight(arr2);
    assert((res2 == std::vector<int>{-1, -1, -1, -1, -1}));

    // Strictly increasing array → each element's next greater is the immediate right neighbor
    std::vector<int> arr3 = {1, 3, 5, 7};
    std::vector<int> res3 = nextGreaterElementToRight(arr3);
    assert((res3 == std::vector<int>{3, 5, 7, -1}));

    // Duplicate values: strictly greater required
    std::vector<int> arr4 = {4, 4, 6, 6, 2};
    std::vector<int> res4 = nextGreaterElementToRight(arr4);
    assert((res4 == std::vector<int>{6, 6, -1, -1, -1}));

    // Single element
    std::vector<int> arr5 = {10};
    std::vector<int> res5 = nextGreaterElementToRight(arr5);
    assert((res5 == std::vector<int>{-1}));

    // Mixed with negative numbers
    std::vector<int> arr6 = {-3, -1, -2, 0};
    std::vector<int> res6 = nextGreaterElementToRight(arr6);
    assert((res6 == std::vector<int>{-1, 0, 0, -1}));

    return 0;
}

// The algorithm scans the array from right to left while maintaining a monotonic decreasing stack of candidate "next greater" values. For each element `arr[i]`, we pop from the stack any value that is less than or equal to `arr[i]` because those cannot be the next greater element for this or any earlier element (they are blocked by the current larger value). After popping, if the stack is empty, there is no greater element to the right, so we assign `-1`; otherwise the top of the stack is the next greater element. Then we push `arr[i]` onto the stack for use by earlier elements. Edge cases include an empty vector (though the problem states non-empty, the function should still handle it gracefully), duplicate values (handled by the `<=` comparison), and a strictly decreasing array where every output is `-1`. Time complexity is O(n) because each element is pushed and popped at most once. Space complexity is O(n) for the stack in the worst case (e.g., a decreasing array) and O(n) for the output vector.

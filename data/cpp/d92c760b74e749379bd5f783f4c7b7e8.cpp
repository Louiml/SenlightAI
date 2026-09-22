/*
Write a C++ function named `previousSmaller` that takes a non-empty vector of integers as input and returns a vector of integers where each element at index `i` is the nearest element to the left of `nums[i]` that is strictly smaller than `nums[i]`. If no such smaller element exists (i.e., all elements to the left are greater than or equal to `nums[i]`), the result should contain `-1` for that position. The function must preserve the original order and handle duplicate values correctly. For example, given `{4, 5, 2, 10, 8}`, the output should be `{-1, 4, -1, 2, 2}` because 4 has no left smaller, 5's nearest left smaller is 4, 2 has none to its left smaller than 2, 10's nearest left smaller is 2, and 8's nearest left smaller is also 2.
*/

#include <vector>
#include <stack>

// Return a vector where result[i] is the nearest element to the left of nums[i]
// that is strictly smaller than nums[i], or -1 if no such element exists.
std::vector<int> previousSmaller(const std::vector<int>& nums) {
    std::vector<int> result;
    result.reserve(nums.size());
    std::stack<int> monotonicStack;

    for (int value : nums) {
        // Remove elements that are greater than or equal to current value
        while (!monotonicStack.empty() && monotonicStack.top() >= value) {
            monotonicStack.pop();
        }

        // If stack is empty, no smaller element exists to the left
        if (monotonicStack.empty()) {
            result.push_back(-1);
        } else {
            result.push_back(monotonicStack.top());
        }

        // Push current value for future elements
        monotonicStack.push(value);
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic example
    std::vector<int> input1 = {4, 5, 2, 10, 8};
    std::vector<int> expected1 = {-1, 4, -1, 2, 2};
    assert(previousSmaller(input1) == expected1);

    // Strictly increasing sequence
    std::vector<int> input2 = {1, 2, 3, 4};
    std::vector<int> expected2 = {-1, 1, 2, 3};
    assert(previousSmaller(input2) == expected2);

    // Strictly decreasing sequence
    std::vector<int> input3 = {5, 4, 3, 2};
    std::vector<int> expected3 = {-1, -1, -1, -1};
    assert(previousSmaller(input3) == expected3);

    // All equal elements
    std::vector<int> input4 = {7, 7, 7};
    std::vector<int> expected4 = {-1, -1, -1};
    assert(previousSmaller(input4) == expected4);

    // Single element
    std::vector<int> input5 = {42};
    std::vector<int> expected5 = {-1};
    assert(previousSmaller(input5) == expected5);

    // Negatives and zeros
    std::vector<int> input6 = {-3, 0, -2, 5, -4, -3};
    std::vector<int> expected6 = {-1, -3, -3, -2, -1, -4};
    assert(previousSmaller(input6) == expected6);

    // Random mixed values with duplicates
    std::vector<int> input7 = {3, 1, 2, 1, 4, 3, 2};
    std::vector<int> expected7 = {-1, -1, 1, -1, 1, 1, 1};
    assert(previousSmaller(input7) == expected7);

    // Large increasing sequence to verify no stack overflow issues
    std::vector<int> input8(10000);
    for (int i = 0; i < 10000; ++i) input8[i] = i;
    std::vector<int> expected8(10000);
    expected8[0] = -1;
    for (int i = 1; i < 10000; ++i) expected8[i] = i - 1;
    assert(previousSmaller(input8) == expected8);

    return 0;
}

// The problem is solved efficiently using a monotonic stack that maintains elements in strictly increasing order from bottom to top. As we iterate from left to right, before processing the current element, we pop all stack elements that are greater than or equal to the current value. This ensures that the stack only contains candidate smaller elements that are strictly less than the current element. After popping, if the stack is empty, no smaller element exists to the left, so we push `-1` to the result. Otherwise, the top of the stack is exactly the nearest smaller element (because any smaller element that is closer would not have been popped, and any equal or larger elements have been removed). After recording the answer, we push the current value onto the stack. This works because larger elements to the left of a future element can never be the nearest smaller for that future element if a smaller or equal element exists between them. Edge cases include an empty vector (though the task specifies non-empty, we can handle it by returning an empty vector), all equal elements (all results are `-1` because no element is strictly smaller to the left), and strictly increasing sequences (each element's answer is the previous element). Time complexity is O(N) since each element is pushed and popped at most once, and space complexity is O(N) in the worst case for the stack.

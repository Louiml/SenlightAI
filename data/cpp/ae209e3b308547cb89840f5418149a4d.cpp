// Write a C++ function named `stackProcess` that accepts a vector of integers as input and returns a vector of integers. The function must first push the provided integers onto a `std::stack<int>` in the given order, then pop the stack entirely while collecting each popped element into a result vector, and finally return that result vector. If the input vector is empty, the function should return an empty vector. The core behavior must rely on standard stack operations (`push`, `pop`, `top`, `size`, and `empty`), and the function must correctly reverse the input order due to LIFO semantics. Additionally, the function must not modify the input vector (use appropriate const correctness) and should handle negative numbers, zeros, duplicates, and large values without special cases.
#include <cassert>
#include <vector>

// Placeholder for the solution function; include the actual code above in practice.
std::vector<int> stackProcess(const std::vector<int>& input);

int main() {
    // Empty input gives empty result.
    assert(stackProcess({}) == std::vector<int>{});

    // Single element.
    assert(stackProcess({42}) == std::vector<int>{42});

    // Basic reversal of multiple elements.
    assert(stackProcess({1, 2, 3}) == std::vector<int>({3, 2, 1}));

    // Negative and zero values.
    assert(stackProcess({-5, 0, 7, -1}) == std::vector<int>({-1, 7, 0, -5}));

    // Duplicate values.
    assert(stackProcess({4, 4, 4}) == std::vector<int>({4, 4, 4}));

    // Larger sequence.
    assert(stackProcess({10, 20, 30, 40, 50}) == std::vector<int>({50, 40, 30, 20, 10}));

    // Mixed large and small numbers.
    assert(stackProcess({100, -100, 0, 999}) == std::vector<int>({999, 0, -100, 100}));

    return 0;
}
#include <vector>
#include <stack>

// Given a vector of integers, push them onto a stack and then pop all elements,
// returning the popped values in the order they come off the stack (reverse of input).
std::vector<int> stackProcess(const std::vector<int>& input) {
    std::stack<int> myStack;

    // Push all input elements onto the stack in the given order.
    for (int value : input) {
        myStack.push(value);
    }

    // Pop all elements and collect them into the result vector.
    std::vector<int> result;
    while (!myStack.empty()) {
        result.push_back(myStack.top());
        myStack.pop();
    }

    return result;
}
// The solution is straightforward: create an empty `std::stack<int>`, then iterate over the input vector and push each element onto the stack. After all elements are pushed, create a result vector and repeatedly check whether the stack is empty; while it’s not, take the top element, append it to the result, and pop it. This naturally outputs the elements in reverse order of the input. Edge cases include an empty input (the stack stays empty and the result is empty), a single element (the result contains that one element), and duplicate or negative values (no special handling needed because the stack operates on value copies). Time complexity is O(n), where n is the size of the input vector, because each push and pop operation is O(1) and we perform exactly n pushes and n pops. Space complexity is O(n) for the stack and the result vector, plus O(1) auxiliary for loop variables.

Write a C++ function `int stackTopAfterOperations(const std::vector<int>& pushValues, int pushLimit, int popCount)` that simulates a fixed-capacity stack using an array-like structure. The function should receive a sequence of values to push, a maximum capacity for the stack, and a number of pop operations to perform after all pushes. It should return the value at the top of the stack after performing all pushes (ignoring any values that would cause overflow) and then performing exactly `popCount` pops (ignoring underflow). If the final stack is empty, return -1. For example, if `pushValues = {1,2,3,4}`, `pushLimit = 3`, and `popCount = 2`, the pushes would result in stack `{2,3,4}` (since 1 is ignored due to overflow after pushing 2,3,4), then two pops leave `{2}`, so the function returns 2.
#include <cassert>
#include <vector>

// Declaration of the solution function (already defined above).
int stackTopAfterOperations(const std::vector<int>&, int, int);

int main() {
    // Basic case: push 1,2,3,4 with capacity 3 → stack {2,3,4}, then 2 pops → {2}
    assert(stackTopAfterOperations({1,2,3,4}, 3, 2) == 2);

    // No overflows, no pops
    assert(stackTopAfterOperations({5,6,7}, 5, 0) == 7);

    // All pushes overflow because capacity is 0
    assert(stackTopAfterOperations({1,2,3}, 0, 0) == -1);

    // Empty input, no pops
    assert(stackTopAfterOperations({}, 5, 0) == -1);

    // Pop more than elements exist → stack becomes empty
    assert(stackTopAfterOperations({10,20}, 3, 5) == -1);

    // Exact fit, then one pop
    assert(stackTopAfterOperations({1,2,3}, 3, 1) == 2);

    // Duplicate values and capacity larger than input
    assert(stackTopAfterOperations({7,7,7}, 10, 2) == 7);

    // Negative values
    assert(stackTopAfterOperations({-1,-2,-3}, 2, 1) == -2);

    // Pop exactly all elements
    assert(stackTopAfterOperations({9,8,7}, 3, 3) == -1);

    // Large capacity, no pops
    assert(stackTopAfterOperations({42}, 100, 0) == 42);

    return 0;
}
#include <vector>

// Simulate a fixed-capacity stack with push values and then pop operations.
// Returns the top element after all operations, or -1 if the stack is empty.
int stackTopAfterOperations(const std::vector<int>& pushValues, int pushLimit, int popCount) {
    // Use a vector as the stack storage with the given capacity.
    std::vector<int> stack(pushLimit);
    int top = -1; // empty stack

    // Push phase: ignore values when the stack is full (overflow).
    for (int value : pushValues) {
        if (top < pushLimit - 1) {
            ++top;
            stack[top] = value;
        }
        // else: overflow, ignore this value
    }

    // Pop phase: perform exactly popCount pops, ignoring underflow.
    for (int i = 0; i < popCount; ++i) {
        if (top >= 0) {
            --top;
        }
        // else: underflow, do nothing
    }

    // Return top element or -1 if the stack is empty.
    return (top >= 0) ? stack[top] : -1;
}
// The solution simulates a stack with a fixed maximum size using a simple array (or `std::vector` for convenience). We maintain a `top` index starting at -1. For each value in `pushValues`, we check if the stack is full (i.e., `top == pushLimit - 1`); if so, we ignore the value (overflow). Otherwise, we increment `top` and store the value at that position. After processing all pushes, we perform exactly `popCount` pops: each pop decrements `top` by 1, but if `top` is already -1, we ignore the operation (underflow). Finally, if `top` is -1, the stack is empty, so return -1; otherwise return the value at index `top`. Edge cases include: an empty `pushValues` (stack remains empty), `pushLimit == 0` (all pushes overflow), and `popCount` exceeding the number of actual elements (stack becomes empty). Time complexity is O(n + popCount) where n is the number of push values, and space complexity is O(pushLimit) for the underlying storage, though we can also use O(1) extra space beyond the input vector if we avoid copying.

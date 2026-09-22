/*
Write a C++ function named `stackPeekAfterPops` that takes a fixed-size array of integers (represented as a `std::vector<int>`), the capacity of the stack (a positive integer), and a sequence of operations (a `std::string` where 'p' means push the next integer from the vector, 'o' means pop from the stack, and 'e' means peek and record the top element). The function must simulate a stack with the given capacity, handle overflow (push when full) and underflow (pop or peek when empty) by simply skipping the operation (no output), and return a `std::vector<int>` containing the results of all peek operations in the order they occurred. The input vector provides the values to push in sequence as needed. For example, if operations = "popepp", capacity = 3, and values = {10,20,30,40}, the simulation: push 10, peek → [10], pop, push 20, push 30, peek → [10,30] (note: after first pop, stack is empty, then push 20, push 30, peek gives 30). For "poe" with values {5}, capacity=1: push 5, pop (empty), peek → underflow skip, result empty. The function must be `const`-correct, meaning it should not modify the input vector or the operations string. Provide the implementation with a descriptive name, include necessary headers, and use `std::vector<int>` for the stack data.
*/

#include <vector>
#include <string>

// Simulate stack operations and return results of peek operations.
// 'p' pushes the next value from values (skipping if full), 'o' pops (skipping if empty),
// 'e' records the top element (skipping if empty).
std::vector<int> stackPeekAfterPops(const std::vector<int>& values, int capacity, const std::string& operations) {
    std::vector<int> stack;
    std::vector<int> result;
    int valueIndex = 0;
    
    for (char op : operations) {
        if (op == 'p') {
            if (valueIndex < static_cast<int>(values.size())) {
                if (static_cast<int>(stack.size()) < capacity) {
                    stack.push_back(values[valueIndex]);
                }
                ++valueIndex;  // consume value regardless of successful push
            }
        } else if (op == 'o') {
            if (!stack.empty()) {
                stack.pop_back();
            }
        } else if (op == 'e') {
            if (!stack.empty()) {
                result.push_back(stack.back());
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.

int main() {
    // Basic case with mixed operations
    std::vector<int> values1 = {10, 20, 30, 40};
    std::vector<int> res1 = stackPeekAfterPops(values1, 3, "popepp");
    assert(res1 == std::vector<int>({10, 30}));

    // Underflow skip on pop and peek
    std::vector<int> values2 = {5};
    std::vector<int> res2 = stackPeekAfterPops(values2, 1, "poe");
    assert(res2.empty());

    // Overflow skip on push (capacity 2, push 3 values)
    std::vector<int> values3 = {1, 2, 3, 4, 5};
    std::vector<int> res3 = stackPeekAfterPops(values3, 2, "pppeep");
    // Push 1, push 2, push 3 (overflow skip), peek→2, pop, peek→1
    assert((res3 == std::vector<int>{2, 1}));

    // No operations
    std::vector<int> res4 = stackPeekAfterPops(values1, 5, "");
    assert(res4.empty());

    // All pops in empty stack
    std::vector<int> res5 = stackPeekAfterPops(values1, 2, "ooo");
    assert(res5.empty());

    // Sequential pushes and peeks with no pops
    std::vector<int> values6 = {1, 2, 3};
    std::vector<int> res6 = stackPeekAfterPops(values6, 3, "pppeee");
    assert((res6 == std::vector<int>{3, 3, 3}));

    // Capacity larger than values, no overflow
    std::vector<int> values7 = {7, 8};
    std::vector<int> res7 = stackPeekAfterPops(values7, 10, "pepe");
    // push 7, peek→7, pop, push 8, peek→8
    assert((res7 == std::vector<int>{7, 8}));

    // Exact capacity, no overflow
    std::vector<int> values8 = {1, 2};
    std::vector<int> res8 = stackPeekAfterPops(values8, 2, "ppe");
    assert((res8 == std::vector<int>{2}));
}

// The solution uses a standard stack implemented as a `std::vector<int>` with a `top` index. We iterate through the operations string character by character. For 'p', we need the next value from the values vector; if the stack is not full (current size < capacity), push the value; otherwise skip (overflow) and still consume the next value (since we can't push, we still take that value from the input sequence). For 'o', if the stack is not empty, pop; else skip underflow. For 'e', if the stack is not empty, record `stack[top]` into the result vector; else skip. The order of operations matters, and we must ensure that for 'p' we consume a value from the values vector even if the push is skipped due to overflow, because the operation sequence defines which value to attempt to push. Edge cases include: empty operations string, all operations are underflow/overflow, capacity larger than number of pushes, and values vector shorter than number of 'p' operations (we can assume it's sufficient as per problem specification, but we can guard by checking index). Time complexity is O(n) where n is the length of operations string. Space complexity is O(capacity + m) where m is the number of peek results.

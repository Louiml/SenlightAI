// Write a C++ function named `applyLessThanOrEqual` that simulates the behavior of a virtual machine stack-based evaluator for the `<=` operator. The function must accept a `std::vector<int>` representing the current stack (where the top of the stack is the last element) and return a new `std::vector<int>` after popping the top two values, computing `arg1 <= arg2` (where `arg1` is the deeper of the two popped values, i.e., the second-to-last element, and `arg2` is the top element), and pushing the boolean result as an integer (`1` for true, `0` for false). If the stack has fewer than 2 elements, the function must throw a `std::runtime_error` with the message `"function doesn't return any value"`. The function must be `const`-correct, not modify the input, and handle edge cases like repeated values and negative numbers. Do not include a `main` function in the solution; only the free function.

// The solution directly mirrors the stack semantics of the original interpreter snippet. We read the last two elements of the input vector as `arg1` (second-to-last) and `arg2` (last). After popping both, we compute the boolean result `arg1 <= arg2` and convert it to `1` or `0`. We then return a new vector that is the original vector minus the last two elements, plus the result appended at the end. Edge cases: if the vector size is less than 2, throw a `std::runtime_error` with the exact message. The algorithm processes the stack in `O(1)` time and `O(n)` space for the returned copy, where `n` is the size of the input vector (due to copying all remaining elements). No special handling for duplicates or negatives is needed because the comparison operator naturally handles them. The solution is straightforward: validate size, extract the two operands, compute the result, and construct the new stack. The main challenge is ensuring correct order of operands (the deeper value is the left operand of `<=`).

#include <vector>
#include <stdexcept>

// Simulates the 'less than or equal' stack operation.
// The input vector represents the stack, with the top at the end.
// Returns a new stack after popping two values and pushing the boolean result.
// Throws std::runtime_error if fewer than 2 elements are present.
std::vector<int> applyLessThanOrEqual(const std::vector<int>& stack) {
    if (stack.size() < 2) {
        throw std::runtime_error("function doesn't return any value");
    }

    // arg1 is deeper, arg2 is the top.
    int arg1 = stack[stack.size() - 2];
    int arg2 = stack[stack.size() - 1];

    // Compute the boolean result and convert to int (1 or 0).
    int result = (arg1 <= arg2) ? 1 : 0;

    // Construct the new stack: all elements except the last two, plus the result.
    std::vector<int> newStack(stack.begin(), stack.end() - 2);
    newStack.push_back(result);

    return newStack;
}

#include <cassert>
#include <vector>
#include <stdexcept>

// The solution function is declared here (or included from a header).
std::vector<int> applyLessThanOrEqual(const std::vector<int>& stack);

int main() {
    // Basic case: 3 <= 5 -> true -> 1
    assert(applyLessThanOrEqual({3, 5}) == std::vector<int>({1}));

    // Case where it's false: 7 <= 2 -> false -> 0
    assert(applyLessThanOrEqual({7, 2}) == std::vector<int>({0}));

    // Equal values: 4 <= 4 -> true -> 1
    assert(applyLessThanOrEqual({4, 4}) == std::vector<int>({1}));

    // Negative numbers: -5 <= -10 -> false -> 0
    assert(applyLessThanOrEqual({-5, -10}) == std::vector<int>({0}));

    // More than two elements: stack [1, 2, 3], pop 2 and 3, 2 <= 3 -> 1, new stack [1, 1]
    assert(applyLessThanOrEqual({1, 2, 3}) == std::vector<int>({1, 1}));

    // Stack with three elements where result is false: [10, 5, 2] -> 5 <= 2 -> 0 -> [10, 0]
    assert(applyLessThanOrEqual({10, 5, 2}) == std::vector<int>({10, 0}));

    // Single element should throw
    bool threw = false;
    try {
        applyLessThanOrEqual({42});
    } catch (const std::runtime_error& e) {
        threw = true;
        assert(std::string(e.what()) == "function doesn't return any value");
    }
    assert(threw);

    // Empty stack should throw
    threw = false;
    try {
        applyLessThanOrEqual({});
    } catch (const std::runtime_error& e) {
        threw = true;
        assert(std::string(e.what()) == "function doesn't return any value");
    }
    assert(threw);
}

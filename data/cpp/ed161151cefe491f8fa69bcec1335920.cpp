/*
Given a sequence of pushes onto a stack interleaved with pop operations, write a C++ function `bool isValidStackSequence(const std::vector<int>& pushed, const std::vector<int>& popped)` that returns `true` if the `popped` sequence can be produced by a single stack using the exact order of pushes and pops implied by `pushed` and `popped`, and `false` otherwise. The push order is the order in `pushed` (all elements pushed exactly once), and the pop order is the order in `popped` (all elements popped exactly once). The function must simulate the process: push elements from `pushed` one by one, and after each push (or at any point), if the top of the stack matches the next element in `popped`, pop it repeatedly until no more matches. At the end, the stack must be empty and all `popped` elements consumed. The vectors are non-empty and of the same length, containing distinct integers.
*/

#include <vector>
#include <stack>

// Determine if a given pop sequence can be produced by a stack
// when elements are pushed in the given push order.
bool isValidStackSequence(const std::vector<int>& pushed, const std::vector<int>& popped) {
    if (pushed.size() != popped.size()) {
        return false;
    }
    if (pushed.empty()) {
        return true;
    }
    
    std::stack<int> stk;
    std::size_t popIndex = 0;
    
    for (int value : pushed) {
        stk.push(value);
        // Pop while the top matches the next expected popped value.
        while (!stk.empty() && popIndex < popped.size() && stk.top() == popped[popIndex]) {
            stk.pop();
            ++popIndex;
        }
    }
    
    // Valid if all elements have been popped (stack empty).
    return stk.empty();
}

#include <cassert>
#include <vector>

// Function declaration (from solution)
bool isValidStackSequence(const std::vector<int>& pushed, const std::vector<int>& popped);

int main() {
    // Basic valid sequence
    assert(isValidStackSequence({1,2,3,4,5}, {4,5,3,2,1}) == true);
    // Basic invalid sequence
    assert(isValidStackSequence({1,2,3,4,5}, {4,3,5,1,2}) == false);
    // Single element
    assert(isValidStackSequence({42}, {42}) == true);
    // All pushes then all pops (reverse order)
    assert(isValidStackSequence({1,2,3}, {3,2,1}) == true);
    // Immediate pops after each push (same order)
    assert(isValidStackSequence({1,2,3}, {1,2,3}) == true);
    // Mismatched lengths
    assert(isValidStackSequence({1,2}, {1}) == false);
    // Empty input
    assert(isValidStackSequence({}, {}) == true);
    // Larger valid example
    assert(isValidStackSequence({1,2,3,4,5,6,7}, {3,2,1,6,5,4,7}) == true);
    // Larger invalid example
    assert(isValidStackSequence({1,2,3,4,5,6,7}, {1,2,3,7,6,4,5}) == false);
    // Duplicate values not expected, but ensure function handles distinct values
    assert(isValidStackSequence({5,1,2,3}, {1,2,3,5}) == true);
    return 0;
}

// The solution simulates the standard stack validation algorithm. Maintain an index `popIndex` into the `popped` vector. For each element `x` in `pushed`, push `x` onto a `std::stack<int>`. Then, while the stack is not empty and the top of the stack equals `popped[popIndex]`, pop from the stack and increment `popIndex`. After processing all pushed elements, the sequence is valid if and only if the stack is empty (and consequently `popIndex` equals the size of `popped`). Edge cases: if `pushed` and `popped` have different lengths, return `false`; if both are empty (though problem says non-empty, handle gracefully) return `true`. The algorithm runs in O(n) time because each element is pushed once and popped at most once, and O(n) auxiliary space for the stack (worst case if no pops occur until the end).

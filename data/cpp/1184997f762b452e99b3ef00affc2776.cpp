Write a standalone C++ function named `stackToReversedArray` that accepts a reference to a `std::stack<int>` (using the standard library container adapter) and returns a `std::vector<int>` containing the elements of the stack in reverse order of how they were pushed (i.e., top of the stack first). The function must not modify the original stack — it should work on a copy or by using a temporary. The input stack may be empty, in which case an empty vector must be returned. The function must be `const`-correct: it should be callable on a `const std::stack<int>&`. Ensure the solution uses only the standard library and does not rely on any global state or non-constant external variables.

The main challenge is to reverse the stack order without mutating the input. Since a `std::stack` only allows access to the top element, we can copy the stack (which preserves order), then repeatedly pop from the copy, pushing each popped value into a vector. Because the copy's top is the original's top, the first pop gives the top of the original stack, which should be the first element in the output vector. Continuing until the copy is empty yields all elements in top-to-bottom order. This avoids modifying the original stack since the copy is a separate object. Edge cases: an empty stack yields an empty vector; a stack with one element yields that single element. Time complexity is O(n) because each element is copied once and popped once. Space complexity is O(n) for the copy of the stack and the output vector. The function signature should be `std::vector<int> stackToReversedArray(const std::stack<int>& stk)` to enforce const-correctness.

#include <stack>
#include <vector>

// Return a vector containing all elements of the stack from top to bottom.
// The input stack is not modified; a copy is used internally.
std::vector<int> stackToReversedArray(const std::stack<int>& stk) {
    std::stack<int> temp = stk;  // copy preserves original order
    std::vector<int> result;
    result.reserve(temp.size());  // avoid reallocations
    while (!temp.empty()) {
        result.push_back(temp.top());
        temp.pop();
    }
    return result;
}

#include <cassert>
#include <stack>
#include <vector>

// Declaration of the solution function
std::vector<int> stackToReversedArray(const std::stack<int>& stk);

int main() {
    // Test 1: empty stack
    std::stack<int> s1;
    assert(stackToReversedArray(s1).empty());

    // Test 2: single element
    std::stack<int> s2;
    s2.push(42);
    assert(stackToReversedArray(s2) == std::vector<int>{42});

    // Test 3: multiple elements in order 1,2,3 (top is 3)
    std::stack<int> s3;
    s3.push(1);
    s3.push(2);
    s3.push(3);
    assert(stackToReversedArray(s3) == (std::vector<int>{3, 2, 1}));

    // Test 4: negative values and duplicates
    std::stack<int> s4;
    s4.push(-5);
    s4.push(0);
    s4.push(-5);
    s4.push(7);
    assert(stackToReversedArray(s4) == (std::vector<int>{7, -5, 0, -5}));

    // Test 5: large stack
    std::stack<int> s5;
    for (int i = 0; i < 100; ++i) s5.push(i);  // top is 99
    std::vector<int> expected;
    for (int i = 99; i >= 0; --i) expected.push_back(i);
    assert(stackToReversedArray(s5) == expected);

    // Test 6: comparison with original stack (must not modify input)
    std::stack<int> s6;
    s6.push(10);
    s6.push(20);
    stackToReversedArray(s6);
    assert(s6.top() == 20);
    assert(s6.size() == 2);
}

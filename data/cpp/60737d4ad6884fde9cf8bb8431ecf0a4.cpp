/*
Write a standalone C++ function named `applySequence` that takes two integer parameters: an initial value and a sequence code (an integer from 0 to 4 inclusive). The function must simulate the following operations from the given code snippet: for code 0, multiply the initial value by 10 and return it; for code 1, subtract 10 from the initial value and return it; for code 2, multiply the initial value by 2 and return it; for code 3, multiply the initial value by 3 and return it; for code 4, subtract 10 from the initial value and return it. For any other sequence code (invalid), return the initial value unchanged. The function should be pure (no side effects) and work with negative integers. Provide a self-contained implementation with appropriate `const` correctness (parameters are passed by value but should be marked `const` where applicable) and no `main` function in the solution section.
*/

// Applies a predetermined arithmetic operation based on the sequence code.
// Codes: 0 -> value * 10, 1 -> value - 10, 2 -> value * 2, 3 -> value * 3,
// 4 -> value - 10. Invalid codes return value unchanged.
int applySequence(const int value, const int code) {
    switch (code) {
        case 0:
            return value * 10;
        case 1:
            return value - 10;
        case 2:
            return value * 2;
        case 3:
            return value * 3;
        case 4:
            return value - 10;
        default:
            return value;
    }
}

#include <cassert>

int main() {
    // Test each valid code with positive and negative values.
    assert(applySequence(10, 0) == 100);
    assert(applySequence(50, 1) == 40);
    assert(applySequence(2, 2) == 4);
    assert(applySequence(30, 3) == 90);
    assert(applySequence(34, 4) == 24);
    
    // Edge cases: negative inputs.
    assert(applySequence(-10, 0) == -100);
    assert(applySequence(-50, 1) == -60);
    assert(applySequence(-2, 2) == -4);
    assert(applySequence(-30, 3) == -90);
    assert(applySequence(-34, 4) == -44);
    
    // Invalid codes return original value.
    assert(applySequence(42, -1) == 42);
    assert(applySequence(42, 5) == 42);
    assert(applySequence(42, 99) == 42);
    
    // Zero value works as expected.
    assert(applySequence(0, 0) == 0);
    assert(applySequence(0, 4) == -10);
    
    return 0;
}

// The task is straightforward: map each valid sequence code (0 to 4) to a specific arithmetic operation on the input value. The main algorithm is a simple switch statement or if-else chain: for each code, perform the corresponding operation (multiplication by 10, subtraction of 10, multiplication by 2, multiplication by 3, subtraction of 10) and return the result. For invalid codes (outside 0–4), return the input value unchanged. Edge cases include negative inputs (which work naturally with multiplication and subtraction) and invalid codes. Since there are only five valid codes, the time complexity is O(1) (constant) and space complexity is O(1) (no extra data structures). The function should be implemented as a free function with a descriptive name, and parameters should be passed by value (but still declared `const` inside to emphasize immutability).

/*
Write a C++ function named `areValuesEqual` that takes two integer parameters and returns a boolean indicating whether their values are equal. The function must use the assignment operator (`==`) for the comparison, as specified. The function should be pure, with no side effects, and must not read from or write to standard input/output. Your task is to implement this function and then verify it with test cases that cover equal numbers, unequal numbers, negative numbers, zero, and large values.
*/

// Check whether two integers are equal using the assignment operator (==).
bool areValuesEqual(int a, int b) {
    return a == b;
}

int main() {
    // Equal positive numbers
    assert(areValuesEqual(5, 5) == true);
    // Unequal positive numbers
    assert(areValuesEqual(5, 7) == false);
    // Both zero
    assert(areValuesEqual(0, 0) == true);
    // One zero, one non-zero
    assert(areValuesEqual(0, -3) == false);
    // Equal negative numbers
    assert(areValuesEqual(-10, -10) == true);
    // Unequal negative numbers
    assert(areValuesEqual(-10, -20) == false);
    // Large values
    assert(areValuesEqual(2147483647, 2147483647) == true);
    // Large unequal values
    assert(areValuesEqual(2147483647, -2147483647) == false);
    // Mixed signs with same magnitude
    assert(areValuesEqual(42, -42) == false);
    // Duplicate testing
    assert(areValuesEqual(100, 100) == true);
    return 0;
}

// The solution is straightforward: the function receives two integers, `a` and `b`, and uses the equality operator `==` to compare them. If `a` equals `b`, return `true`; otherwise, return `false`. This is a direct mapping of the original code’s logic, but isolated into a pure (side-effect-free) function. The main algorithm is a single comparison operation, so time complexity is O(1) and space complexity is O(1). Edge cases include both numbers being zero, both numbers being equal negatives, or one number being zero and the other non-zero. Since the function is `const`-correct (no mutation of parameters), it can be called on any integer values without issue. The implementation is trivial, but the test cases should cover a variety of inputs to ensure correctness.

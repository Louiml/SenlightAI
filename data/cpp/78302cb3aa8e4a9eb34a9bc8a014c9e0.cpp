Write a C++ function named `max_of_four` that takes four integers as parameters and returns the largest among them. The function should be robust, using a simple iterative comparison approach without relying on arrays or standard library algorithms. The solution must handle all possible integer values, including negative numbers, zero, and large positive/negative extremes (within the range of `int`). The function should be `const`-correct and self-contained, meaning it should not depend on any external state or global variables. After implementing the function, verify its correctness by testing it against known inputs, including edge cases where the maximum appears in different positions (first, last, middle) and where values are equal.

#include <cassert>

// The function is declared above this test block.
int main() {
    // Basic positive numbers
    assert(max_of_four(1, 2, 3, 4) == 4);
    // Maximum in first position
    assert(max_of_four(10, 1, 2, 3) == 10);
    // Maximum in second position
    assert(max_of_four(1, 9, 2, 3) == 9);
    // Maximum in third position
    assert(max_of_four(1, 2, 8, 3) == 8);
    // Maximum in last position
    assert(max_of_four(1, 2, 3, 7) == 7);
    // All equal values
    assert(max_of_four(5, 5, 5, 5) == 5);
    // Negative numbers
    assert(max_of_four(-1, -2, -3, -4) == -1);
    // Mixed positive and negative
    assert(max_of_four(-5, 0, 3, -1) == 3);
    // Large values
    assert(max_of_four(2147483647, 1, 2, 3) == 2147483647);
    assert(max_of_four(-2147483648, -5, 0, 1) == 1);
    return 0;
}

// Return the largest of four integers.
int max_of_four(int a, int b, int c, int d) {
    int currentMax = a;
    if (b > currentMax) currentMax = b;
    if (c > currentMax) currentMax = c;
    if (d > currentMax) currentMax = d;
    return currentMax;
}

// The solution approach is straightforward: initialize a variable `currentMax` with the first parameter `a`, then compare it sequentially with `b`, `c`, and `d`, updating `currentMax` whenever a larger value is found. This greedy comparison guarantees that after processing all four inputs, `currentMax` holds the maximum. The algorithm handles edge cases naturally: if all values are equal, the initial value is never updated; if the maximum is at any position, it will be captured during its comparison step; negative numbers and zero work because comparisons use standard `<` operators. Time complexity is \(O(1)\) since exactly three comparisons are performed regardless of input. Space complexity is \(O(1)\) as only a single local variable is used. No special edge cases are problematic because the function operates on primitive `int` types without overflow risks during comparisons.

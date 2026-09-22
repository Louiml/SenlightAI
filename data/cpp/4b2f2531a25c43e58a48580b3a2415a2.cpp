Write a C++ function named `largestOfThree` that accepts three integers by value and returns the largest of the three. The function must not use any standard library functions like `std::max` and must be implemented using only nested `if`/`else` statements or compound conditional expressions. Ensure the function works correctly for negative numbers, zero, duplicate values, and extreme integer limits. The function should be `const`-correct in the sense that it does not modify its inputs and can be called with constant arguments. Provide a standalone implementation that can be tested independently.

The simplest approach is to compare the first number with the second and third, then refine. Instead of nested if/else, a clean way is to use a series of comparisons: first determine if `a` is the largest by checking `(a >= b && a >= c)`. If true, return `a`. Otherwise, check if `b` is the largest by `(b >= a && b >= c)`. If true, return `b`. If neither, then `c` must be the largest, so return `c`. This handles all cases including duplicates because we use `>=`, which means if two numbers are equal and both are the maximum, the earlier check will catch it. For example, if `a=5, b=5, c=3`, `a >= b && a >= c` is true, so returns `a`. Edge cases: negative numbers and zero work naturally. Extreme values like `INT_MAX` and `INT_MIN` also work because comparisons do not overflow. Time complexity is O(1) with a constant number of comparisons; space complexity is O(1) as no extra storage is used.

#include <algorithm> // Not used, but included for completeness; can be removed.

// Return the largest of three integers using only comparisons.
int largestOfThree(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    } else if (b >= a && b >= c) {
        return b;
    } else {
        return c;
    }
}

#include <cassert>
#include <climits>

int main() {
    // Basic positive numbers
    assert(largestOfThree(1, 2, 3) == 3);
    assert(largestOfThree(3, 2, 1) == 3);
    assert(largestOfThree(2, 3, 1) == 3);

    // Negative numbers
    assert(largestOfThree(-5, -1, -10) == -1);
    assert(largestOfThree(-1, -5, -10) == -1);

    // Duplicate values
    assert(largestOfThree(5, 5, 3) == 5);
    assert(largestOfThree(3, 5, 5) == 5);
    assert(largestOfThree(5, 3, 5) == 5);
    assert(largestOfThree(7, 7, 7) == 7);

    // Zero and mixed signs
    assert(largestOfThree(0, 0, 0) == 0);
    assert(largestOfThree(-2, 0, -1) == 0);

    // Extreme values
    assert(largestOfThree(INT_MAX, INT_MIN, 0) == INT_MAX);
    assert(largestOfThree(INT_MIN, INT_MIN, INT_MAX) == INT_MAX);

    // Order variations
    assert(largestOfThree(10, -5, 10) == 10);
    assert(largestOfThree(-3, -3, -4) == -3);

    return 0;
}

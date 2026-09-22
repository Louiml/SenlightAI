// Write a C++ function named `findLargestOfThree` that takes three integers as parameters (`a`, `b`, `c`) and returns the largest among them using only nested `if-else` statements (no ternary operators, no `std::max`, no arrays, no loops). The function must be `const`-correct and handle all possible integer values, including negative numbers, zero, and duplicates. After the function, write a complete program in a `main` function that reads three integers from standard input, calls `findLargestOfThree`, and prints the result followed by a newline. Ensure the function is free-standing (no capture of external state) and returns the correct value for all edge cases like all equal numbers, two equal larger numbers, and one number larger than the other two.
// The core algorithm mirrors the provided snippet: compare `a` and `b` first; if `a > b`, then compare `a` with `c` — if `a > c` return `a`, else return `c`. If `a <= b`, then compare `b` with `c` — if `b > c` return `b`, else return `c`. This nested branching covers all six possible orderings of three distinct numbers, and duplicates naturally fall into the correct branch because strict greater-than comparisons determine precedence correctly: for equal numbers, the `else` branch is taken, which leads to comparing with the third number, and since equal values are not greater, the third number (or the compared one if it's larger) is returned appropriately. Edge cases include all three equal (e.g., `5 5 5` — first `a > b` false, then `b > c` false → return `c` = 5), two equal larger (e.g., `10 10 5` — `a > b` false, then `b > c` true → return 10), and negative numbers (e.g., `-1 -5 -3` — `a > b` true, then `a > c` false → return `c` = -3). Time complexity is \(O(1)\) constant time since only two comparisons are executed, and space complexity is \(O(1)\) besides the input/output overhead. The solution uses no standard library algorithms, just basic `iostream` for I/O. The function is marked `const`-correct by passing integers by value (immutable copies), and uses `const int` parameters where appropriate to emphasize no modification.
#include <iostream>

// Return the largest of three integers using only nested if-else.
int findLargestOfThree(const int a, const int b, const int c) {
    if (a > b) {
        if (a > c) {
            return a;
        } else {
            return c;
        }
    } else {
        if (b > c) {
            return b;
        } else {
            return c;
        }
    }
}
#include <cassert>
#include <iostream>

int findLargestOfThree(const int a, const int b, const int c);

int main() {
    // Basic distinct positive numbers
    assert(findLargestOfThree(3, 1, 2) == 3);
    assert(findLargestOfThree(1, 3, 2) == 3);
    assert(findLargestOfThree(2, 3, 1) == 3);

    // All equal
    assert(findLargestOfThree(5, 5, 5) == 5);

    // Two equal larger numbers
    assert(findLargestOfThree(10, 10, 5) == 10);
    assert(findLargestOfThree(10, 5, 10) == 10);
    assert(findLargestOfThree(5, 10, 10) == 10);

    // Two equal smaller numbers
    assert(findLargestOfThree(1, 1, 2) == 2);
    assert(findLargestOfThree(1, 2, 1) == 2);
    assert(findLargestOfThree(2, 1, 1) == 2);

    // Negative numbers
    assert(findLargestOfThree(-1, -5, -3) == -1);
    assert(findLargestOfThree(-10, -2, -8) == -2);

    // Mixed signs
    assert(findLargestOfThree(-1, 0, 1) == 1);
    assert(findLargestOfThree(0, -1, 1) == 1);
    assert(findLargestOfThree(1, 0, -1) == 1);

    // Zero and duplicates
    assert(findLargestOfThree(0, 0, 0) == 0);
    assert(findLargestOfThree(7, 0, 0) == 7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

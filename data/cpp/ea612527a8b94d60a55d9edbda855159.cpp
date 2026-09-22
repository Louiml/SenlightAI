Write a C++ function named `intSquare` that takes a single non-negative integer parameter and returns its square by repeated addition only (no multiplication operator, no standard library power functions, no bit-shifting tricks). The function must use an iterative loop to add the input value to an accumulator exactly `x` times. Additionally, write a second free function named `sumOfSquares` that takes a non-negative integer `n` and returns the sum of squares from `1` to `n` inclusive, using your `intSquare` function for each term. Both functions must be `const`-correct (i.e., parameters passed by value, no modification of external state), and you must ensure that the result does not overflow for the test cases provided (assume inputs are small enough for `int`). The task is purely about implementing these two functions; no `main` is needed in the solution section. Your functions should be self-contained and include only the necessary standard headers.

The solution approach is straightforward: For `intSquare`, initialize an `int result = 0;` and then loop from `0` to `x-1` (inclusive), adding `x` to `result` each iteration. This simulates multiplication as repeated addition and matches the original snippet’s logic exactly. Edge cases: when `x == 0`, the loop runs zero times and returns `0`; when `x == 1`, the loop runs once and returns `1`. Negative inputs are not expected per the task, but if they occur, the loop condition would be false (since `i < x` fails for negative `x` because `i` starts at 0 and `0 < -1` is false) so the function would return 0 — this is acceptable given the specification but we can note it. For `sumOfSquares`, loop from `1` to `n`, accumulate `intSquare(i)` into a `total` variable. Edge case: `n == 0` returns `0`. Complexity: `intSquare` is O(x) time and O(1) space. `sumOfSquares` is O(n) loop iterations, each calling `intSquare(i)` which is O(i), so total time is O(1+2+...+n) = O(n²). Space remains O(1). Both functions should be marked `const` where applicable — since they take parameters by value, there is no need for `const` on the parameters themselves, but we can declare them as `const int` for clarity, though it’s optional.

// Compute the square of a non-negative integer using repeated addition.
int intSquare(const int x) {
    int result = 0;
    for (int i = 0; i < x; ++i) {
        result += x;
    }
    return result;
}

// Compute the sum of squares from 1 to n inclusive.
int sumOfSquares(const int n) {
    int total = 0;
    for (int i = 1; i <= n; ++i) {
        total += intSquare(i);
    }
    return total;
}

#include <cassert>

// Declare the functions (they are defined elsewhere in the solution).
int intSquare(int x);
int sumOfSquares(int n);

int main() {
    // Test intSquare for typical cases.
    assert(intSquare(0) == 0);
    assert(intSquare(1) == 1);
    assert(intSquare(2) == 4);
    assert(intSquare(3) == 9);
    assert(intSquare(5) == 25);

    // Test sumOfSquares for small n.
    assert(sumOfSquares(0) == 0);
    assert(sumOfSquares(1) == 1);      // 1^2 = 1
    assert(sumOfSquares(2) == 5);      // 1^2 + 2^2 = 1 + 4 = 5
    assert(sumOfSquares(3) == 14);     // 1 + 4 + 9 = 14
    assert(sumOfSquares(4) == 30);     // 1 + 4 + 9 + 16 = 30
    assert(sumOfSquares(5) == 55);     // 1 + 4 + 9 + 16 + 25 = 55

    return 0;
}

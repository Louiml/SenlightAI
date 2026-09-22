// Write a standalone C++ function that computes the factorial of a given non-negative integer using recursion, and returns the result as an `int`. The function must handle negative inputs gracefully by returning `0` to indicate an invalid argument, treat `0!` as `1`, and otherwise compute `n! = n * (n-1)!`. Do not use loops or iterative approaches—recursion is mandatory. The function should be named `factorial` and accept a single `int` parameter. Ensure the function is `const`-correct (though no member data, so just pass by value is fine) and include necessary headers. The solution must be self-contained and not rely on any global state.

The core algorithm is a direct recursive definition: base cases are `n < 0` → return `0` (invalid input), `n == 0` → return `1` (by definition), and otherwise return `n * factorial(n-1)`. The recursion reduces the problem size by 1 each call, so for input `n`, there are exactly `n+1` function calls (including the base case). Time complexity is O(n) because each recursive call performs a constant amount of work (multiplication and comparison). Space complexity is O(n) due to the recursion stack depth, which grows linearly with `n`. Important edge cases: negative input returns `0`; `0` returns `1`; large positive `n` may cause integer overflow (since `int` is typically 32-bit and `13!` exceeds 2^31-1). The task does not require overflow handling, but a robust solution might note that in comments. The recursion is straightforward and terminates because each call passes `n-1` until reaching 0.

#include <cstdint> // not strictly needed but included for clarity

// Compute factorial of a non-negative integer recursively.
// Returns 0 for negative input (invalid), 1 for 0!, and n! for n>0.
// Note: results may overflow for n > 12 (since 13! > INT_MAX).
int factorial(int n) {
    if (n < 0) {
        return 0; // invalid input
    } else if (n == 0) {
        return 1; // base case: 0! = 1
    } else {
        return n * factorial(n - 1); // recursive step
    }
}

#include <cassert>

// Standalone test for factorial function
int main() {
    // Base cases
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);

    // Positive values
    assert(factorial(2) == 2);
    assert(factorial(3) == 6);
    assert(factorial(4) == 24);
    assert(factorial(5) == 120);
    assert(factorial(10) == 3628800);

    // Edge case: negative input returns 0
    assert(factorial(-1) == 0);
    assert(factorial(-10) == 0);
    assert(factorial(-100) == 0);

    // Ensure recursion handles larger value within int range
    assert(factorial(12) == 479001600);

    // No assertion for overflow values (13! > INT_MAX) – test not required.
    return 0;
}

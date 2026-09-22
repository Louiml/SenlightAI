Write a C++ function that, given a positive integer `n`, returns the number of digits in its decimal representation using recursion. The function must not convert the number to a string, must not use loops, and must handle the case `n == 0` by returning `1` (since the decimal representation of `0` has one digit). The function should be named `countDigitsRecursive` and accept an `int` parameter, returning an `int`. Assume the input is a non-negative integer and fits within a standard `int`. Provide an implementation that is correct, clear, and uses const-correctness where appropriate (though the parameter is passed by value, so no `const` qualifier is needed on the parameter itself; use `const` only if you choose to make a local copy).
#include <cassert>

// Forward declaration for testing.
int countDigitsRecursive(int);

int main() {
    // Test basic cases.
    assert(countDigitsRecursive(0) == 1);
    assert(countDigitsRecursive(1) == 1);
    assert(countDigitsRecursive(9) == 1);
    assert(countDigitsRecursive(10) == 2);
    assert(countDigitsRecursive(99) == 2);
    assert(countDigitsRecursive(100) == 3);
    assert(countDigitsRecursive(999) == 3);
    assert(countDigitsRecursive(12345) == 5);
    assert(countDigitsRecursive(2147483647) == 10);
    // Test larger numbers (still within int range).
    assert(countDigitsRecursive(1000000) == 7);
    return 0;
}
// Counts the number of decimal digits in a non-negative integer using recursion.
// Returns 1 for n = 0, and otherwise returns 1 + digits(n/10).
int countDigitsRecursive(int n) {
    // Base case: numbers from 0 to 9 have exactly one digit.
    if (n < 10) {
        return 1;
    }
    // Recursive step: remove the last digit and count the rest, then add 1.
    return 1 + countDigitsRecursive(n / 10);
}
// The solution uses a recursive decomposition: the number of digits of `n` equals `1 + (number of digits of n/10)` for `n >= 10`, and equals `1` for `0 <= n <= 9`. This is a classic recursion pattern: the base case handles single-digit numbers (including zero), and the recursive step strips the last digit by integer division by 10. The recursion depth is proportional to the number of digits, which is about `log10(n) + 1`. Time complexity is `O(d)` where `d` is the number of digits, and space complexity is `O(d)` due to the call stack. Edge cases: `n = 0` must return `1` (not `0` as a naive base case would), because zero is a single digit. Also, `n` large (like `2147483647`) works within recursion limits because the depth is at most 10. No special handling for negative numbers is required per the spec, but it's trivial to add `abs(n)` if needed; the task states non-negative input.

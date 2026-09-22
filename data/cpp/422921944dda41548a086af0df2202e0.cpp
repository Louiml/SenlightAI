/*
Write a C++ function that takes a positive integer `n` as input and returns the `n`-th Fibonacci number, where the sequence is defined as `F(1) = 1`, `F(2) = 1`, and `F(n) = F(n-1) + F(n-2)` for `n > 2`. The function must handle `n` values up to at least 30 (since typical `int` overflows beyond `F(46)`), but the implementation should rely on recursion as the primary logic, not iteration or memoization. You do not need to handle invalid inputs (e.g., `n ≤ 0`), but the function should be robust for `n = 1` and `n = 2`, which are the base cases. The function should be named `fibonacci` and must be `const`-correct (if applicable), and it must use the exact recursive definition given.
*/
// Recursively compute the n-th Fibonacci number (1-indexed).
// Precondition: n >= 1. Returns 1 for n == 1 and n == 2.
int fibonacci(const int n) {
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}
int main() {
    // Base cases
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);

    // Known Fibonacci numbers: 1, 1, 2, 3, 5, 8, 13, 21, 34, 55
    assert(fibonacci(3) == 2);
    assert(fibonacci(4) == 3);
    assert(fibonacci(5) == 5);
    assert(fibonacci(6) == 8);
    assert(fibonacci(7) == 13);
    assert(fibonacci(8) == 21);
    assert(fibonacci(9) == 34);
    assert(fibonacci(10) == 55);

    // A larger value that fits in int (F(20) = 6765)
    assert(fibonacci(20) == 6765);
}
// The solution directly mirrors the given Fibonacci recurrence. The base cases are `n == 1` and `n == 2`, both returning `1`. For any `n > 2`, the function recursively calls itself with `n-1` and `n-2` and sums the results. This is a classic recursion problem with no need for loops or extra data structures. Edge cases to handle: `n = 1` and `n = 2` must return `1` immediately to prevent infinite recursion or incorrect results. For larger `n`, recursion depth is linear (`n`), and the total number of recursive calls is exponential — specifically `O(2^n)` — because each call spawns two more calls. Space complexity is `O(n)` due to the call stack depth. This is intentionally inefficient for large `n`, but it is correct and matches the specification. To be safe, we note that `int` can store Fibonacci numbers up to `F(46) = 1836311903`; beyond that, overflow occurs. For a standalone task, we can document that the function is intended for `n ≤ 46` (or use `long long` if desired, but the task description says `int` per the snippet). We'll use `int` and assume the caller uses values within range.

Write a C++ function named `nthFibonacci` that takes a non-negative integer `n` and returns the `n`-th Fibonacci number, where the sequence is defined as F(0) = 0, F(1) = 1, and F(n) = F(n-1) + F(n-2) for n ≥ 2. The function must be implemented recursively (no loops or iterative formulas) and must correctly handle the base cases. The function should be `const`-correct and use appropriate parameter types. Do not include a `main` function in your solution; only provide the function definition with necessary headers.

// The problem is a classic recursive Fibonacci computation. The main algorithm is straightforward: if `n` is 0 or 1, return `n` directly (these are the base cases that terminate recursion). For any `n > 1`, the function returns the sum of the results of calling itself on `n-1` and `n-2`. Important edge cases include `n = 0` (should return 0) and `n = 1` (should return 1). Also consider that `n` is given as non-negative, but the function should still handle `n = 0` and `n = 1` explicitly. The time complexity is exponential, specifically O(2^n), because each call branches into two recursive calls until reaching the base cases. The space complexity is O(n) due to the maximum recursion depth (the call stack grows linearly with `n`). This is not efficient for large `n`, but it demonstrates the recursive definition clearly.

#include <cstddef>

// Return the n-th Fibonacci number (0-indexed: F(0)=0, F(1)=1).
int nthFibonacci(int n) {
    // Base cases: F(0) = 0 and F(1) = 1.
    if (n == 0 || n == 1) {
        return n;
    }
    // Recursive definition: F(n) = F(n-1) + F(n-2).
    return nthFibonacci(n - 1) + nthFibonacci(n - 2);
}

#include <cassert>

// Assume nthFibonacci is declared above.

int main() {
    // Base cases.
    assert(nthFibonacci(0) == 0);
    assert(nthFibonacci(1) == 1);

    // Known Fibonacci numbers.
    assert(nthFibonacci(2) == 1);  // F(2)=1
    assert(nthFibonacci(3) == 2);  // F(3)=2
    assert(nthFibonacci(4) == 3);  // F(4)=3
    assert(nthFibonacci(5) == 5);  // F(5)=5
    assert(nthFibonacci(6) == 8);  // F(6)=8
    assert(nthFibonacci(10) == 55); // F(10)=55

    // Larger value (still feasible with recursion).
    assert(nthFibonacci(15) == 610);

    return 0;
}

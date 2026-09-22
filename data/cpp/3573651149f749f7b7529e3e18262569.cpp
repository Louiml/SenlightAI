/*
Write a C++ function named `nthFibonacci` that takes an integer `n` (where \(n \ge 1\)) and returns the \(n\)-th term of the Fibonacci sequence, where the sequence is defined as: the 1st term is `0`, the 2nd term is `1`, and each subsequent term is the sum of the two preceding terms. The function must be implemented recursively. Additionally, the function must use `const` appropriately for its parameter (e.g., pass by value but mark it `const` inside the signature) and must handle the base cases correctly. You do not need to handle `n <= 0` — assume the input is always a positive integer, but your implementation should still be robust if somehow tested with `0` (you may return `0` for `n == 0` as an added safety). Your solution must not use any loops or iterative approaches.
*/

#include <cassert>

// Return the n-th Fibonacci number (1-based indexing: F(1)=0, F(2)=1).
// The function is recursive and uses const for the input parameter.
int nthFibonacci(const int n) {
    if (n <= 1) return 0; // covers n==0 and n==1
    if (n == 2) return 1;
    return nthFibonacci(n - 1) + nthFibonacci(n - 2);
}

int main() {
    // Basic base cases
    assert(nthFibonacci(1) == 0);
    assert(nthFibonacci(2) == 1);
    
    // Subsequent terms: 0,1,1,2,3,5,8,13,21,34,...
    assert(nthFibonacci(3) == 1);
    assert(nthFibonacci(4) == 2);
    assert(nthFibonacci(5) == 3);
    assert(nthFibonacci(6) == 5);
    assert(nthFibonacci(7) == 8);
    assert(nthFibonacci(8) == 13);
    assert(nthFibonacci(9) == 21);
    assert(nthFibonacci(10) == 34);
    
    // Edge case: n == 0 (safety, not required but handled)
    assert(nthFibonacci(0) == 0);
    
    return 0;
}

// The Fibonacci sequence is defined recursively: \(F(1) = 0\), \(F(2) = 1\), and for \(n > 2\), \(F(n) = F(n-1) + F(n-2)\). The naive recursive implementation directly mirrors this recurrence. The main algorithm is: if `n == 1`, return `0`; if `n == 2`, return `1`; otherwise, return `nthFibonacci(n-1) + nthFibonacci(n-2)`. Edge cases include `n == 1` (returns 0), `n == 2` (returns 1), and optionally `n == 0` (which we can return 0 to be safe). The time complexity of this naive recursive approach is exponential, specifically \(O(2^n)\), because each call branches into two subcalls, leading to redundant computation (e.g., `fib(3)` is computed multiple times). The space complexity is \(O(n)\) due to the maximum recursion depth, since the call stack grows linearly with `n`. This is a classic example of why memoization or iteration would be preferred for large `n`, but the task explicitly asks for a recursive implementation.

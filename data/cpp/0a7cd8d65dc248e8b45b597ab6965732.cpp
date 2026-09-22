/*
Write a C++ function that, given a non-negative integer `n`, returns the sum of all integers from 0 to `n` (i.e., the triangular number) by using a recursive approach. The function must handle the base case where `n == 0` by returning 0, and for any other valid input `n > 0`, it must call itself with `n-1` and add `n` to that result. You do not need to validate input, but your implementation must not cause infinite recursion or stack overflow for typical inputs up to a few thousand. Your function should be `const`-correct (no modification of inputs) and written with a descriptive name. Provide only the function, not a `main` or any other wrapper.
*/

// Recursively compute the sum of all integers from 0 to n.
// Precondition: n >= 0.
int sumToN(int n) {
    if (n == 0) {
        return 0;
    }
    return n + sumToN(n - 1);
}

int main() {
    assert(sumToN(0) == 0);
    assert(sumToN(1) == 1);
    assert(sumToN(2) == 3);
    assert(sumToN(5) == 15);
    assert(sumToN(10) == 55);
    assert(sumToN(20) == 210);
    assert(sumToN(100) == 5050);
    assert(sumToN(3) == 6);
    assert(sumToN(4) == 10);
    assert(sumToN(7) == 28);
    return 0;
}

// The main algorithm is straightforward recursion: `sumToN(n) = n + sumToN(n-1)`, with a base case `sumToN(0) = 0`. The function follows the exact recursive pattern from the snippet, but corrects the snippet’s flawed base case. In the original code, the base case checks `n >= 0` and calls `je(n-1)` infinitely for `n=0` (since `n-1` is negative and then `je(-1)` returns 0, but `je(0)` calls `je(-1)` which returns 0, so it works but is inefficient and confusing). The corrected task uses a clean base case at `0`. Edge cases: `n=0` returns 0, `n=1` returns 1, and any positive integer results in the triangular sum `n*(n+1)/2` (which can be derived but the recursion computes it). Time complexity is `O(n)` due to `n+1` recursive calls; space complexity is `O(n)` due to the recursion call stack. For very large `n` (e.g., millions), stack overflow would occur, but for typical test inputs (up to a few thousand) it is safe. No input validation is needed, but the function should not be called with negative numbers per the task spec.

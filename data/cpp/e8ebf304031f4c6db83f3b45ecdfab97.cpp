/*
Write a C++ function named `computeNestedSum` that takes a single positive integer `n` and returns the result of a deeply nested computation: it must compute the sum of the last value produced by four nested loops, each ranging from `0` to `n-1`, where the innermost operation assigns `soma = i + j + k + l`. The function must not print anything and must return an `int`. The input `n` can be as large as 100, so your implementation must be efficient enough to handle that without excessive runtime (though the original algorithm is O(n^4), you may optimize if possible, but the primary requirement is correctness and matching the exact semantics of the nested loops). Ensure proper `const` correctness and include necessary headers.
*/
#include <cstddef> // for size_t if needed, but not required

// Computes the final value of a quadruple nested loop assignment.
// Returns the sum of (i+j+k+l) for the last iteration where all indices are n-1.
// If n <= 0, returns 0 because loops do not execute.
int computeNestedSum(const int n) {
    if (n <= 0) {
        return 0;
    }
    // The innermost assignment overwrites soma each time.
    // The final value occurs when i=j=k=l=n-1, so sum = 4*(n-1).
    return 4 * (n - 1);
}
#include <cassert>

int main() {
    // n=1: loops run once, i=j=k=l=0, sum=0
    assert(computeNestedSum(1) == 0);
    // n=2: last iteration i=1,j=1,k=1,l=1, sum=4
    assert(computeNestedSum(2) == 4);
    // n=3: last iteration i=2,j=2,k=2,l=2, sum=8
    assert(computeNestedSum(3) == 8);
    // n=10: last iteration all 9, sum=36
    assert(computeNestedSum(10) == 36);
    // n=100: last iteration all 99, sum=396
    assert(computeNestedSum(100) == 396);
    // Edge case: n=0 (loops don't run) should return 0
    assert(computeNestedSum(0) == 0);
    // Edge case: negative n should return 0
    assert(computeNestedSum(-5) == 0);
    return 0;
}
// The original code defines `algoritmoDemorado(int n)` that initializes `soma = 0`, then runs four nested loops each from `0` to `n-1`. In the innermost loop, it assigns `soma = i + j + k + l`. Since this assignment happens for every combination, the final value of `soma` is simply the value from the very last iteration when all loop variables are at their maximum, i.e., `i = n-1`, `j = n-1`, `k = n-1`, `l = n-1`. Thus the sum is `(n-1) + (n-1) + (n-1) + (n-1) = 4*(n-1)`. However, we must be careful: if `n` is 0 or negative, the loops do not execute, and `soma` remains 0. Since the task specifies positive integer, we can assume `n >= 1`. The optimized solution is O(1) time and O(1) space, but a direct simulation would be O(n^4) time, which is too slow for n=100 (100 million iterations). Important edge cases include `n=1` (all loops run exactly once, result is `0+0+0+0 = 0`) and large `n` such as 100 (result is `4*99=396`). The function must return `int`, and since the maximum value for n=100 is 396, no overflow occurs. For completeness, if someone tests with a larger n, the result grows linearly and could overflow for n around 536 million, but that is outside typical scope. The solution is straightforward: return `4 * (n - 1)` if `n > 0`, else return 0.

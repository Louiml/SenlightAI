Write a C++ function that computes the number of ways to choose `r` items from a set of `n` distinct items (the binomial coefficient \(\binom{n}{r}\)), given non-negative integer inputs `n` and `r` where `0 <= r <= n`. The function should handle edge cases such as `r = 0`, `r = n`, and moderate values of `n` up to 12 (to avoid overflow). It must not use external libraries beyond standard headers, and the returned value should be an `int`. The function should be named `combinations` and take two `int` parameters.
// The binomial coefficient \(\binom{n}{r}\) can be computed directly from the factorial definition:  
// \[
// \binom{n}{r} = \frac{n!}{r! \cdot (n-r)!}
// \]
// The algorithm is straightforward: compute the factorials of `n`, `r`, and `n-r` using a helper function that multiplies integers from 1 to the given argument. Then divide the factorial of `n` by the product of the other two factorials.  
// Edge cases:  
// - If `r == 0` or `r == n`, the result is 1, which the formula naturally yields.  
// - Input validation is unnecessary for the core function because the problem guarantees `0 <= r <= n`, but for robustness one could assert this.  
// - Overflow: since `n!` grows quickly, for `n > 12` the intermediate factorial exceeds 32-bit `int` range. The task specifies `n <= 12` to keep the result within `int`.  
// Time complexity: Each factorial computation is \(O(k)\) for input `k`, and we compute three factorials, so overall \(O(n)\) time. Space complexity is \(O(1)\) since we use only a few integer variables, no recursion or dynamic allocation.
#include <cassert>

// Computes n! for non-negative n. Assumes n <= 12 to avoid overflow.
int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Computes the binomial coefficient C(n, r) = n! / (r! * (n-r)!).
// Precondition: 0 <= r <= n and n <= 12.
int combinations(int n, int r) {
    assert(n >= 0 && r >= 0 && r <= n && n <= 12);
    int numerator = factorial(n);
    int denominator = factorial(r) * factorial(n - r);
    return numerator / denominator;
}
#include <cassert>

int factorial(int n); // forward declaration
int combinations(int n, int r);

int main() {
    // Basic cases
    assert(combinations(0, 0) == 1);
    assert(combinations(5, 0) == 1);
    assert(combinations(5, 5) == 1);
    
    // Symmetry property: C(n, r) == C(n, n-r)
    assert(combinations(10, 3) == combinations(10, 7));
    assert(combinations(8, 2) == combinations(8, 6));
    
    // Known values
    assert(combinations(5, 2) == 10);
    assert(combinations(6, 3) == 20);
    assert(combinations(7, 4) == 35);
    assert(combinations(12, 6) == 924);
    
    // Edge case with r = n-1
    assert(combinations(4, 3) == 4);
    
    return 0;
}

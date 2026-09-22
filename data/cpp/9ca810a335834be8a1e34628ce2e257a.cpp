Write a C++ function named `factorial` that takes a non-negative integer `n` and returns its factorial as a `long long`. The function must handle the special case `n == 0` by returning `1` (since `0! = 1` by mathematical convention), and for any `n > 0`, it must compute the product of all integers from `1` to `n` recursively. The function must be declared as `long long factorial(long long n)` and must not rely on any external mutable state. Assume `n` is small enough that the result fits within the range of `long long` (i.e., `n ≤ 20` for typical platforms where `20! ≈ 2.43e18`). The function should be `const`-correct in the sense that it does not modify its parameter (pass by value is fine) and should include a clear comment describing its purpose and the base case. You do not need to write a `main` function; the solution must only include the function definition and necessary headers.
#include <cassert>

// Forward declaration of the function under test (since no main is in the solution).
long long factorial(const long long n);

int main() {
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);
    assert(factorial(2) == 2);
    assert(factorial(3) == 6);
    assert(factorial(5) == 120);
    assert(factorial(10) == 3628800);
    assert(factorial(15) == 1307674368000LL);
    assert(factorial(20) == 2432902008176640000LL);
    return 0;
}
#include <cstdint>   // for int64_t (long long is typically 64-bit)

// Compute the factorial of a non-negative integer n recursively.
// Precondition: n >= 0, and n! must be representable in long long (n <= 20 typically).
long long factorial(const long long n) {
    if (n == 0) {
        return 1;   // base case: 0! = 1
    }
    return n * factorial(n - 1);   // recursive step
}
// The factorial of a non-negative integer `n` is defined as the product of all positive integers from `1` to `n`. A natural recursive definition is: `factorial(0) = 1` (base case), and for `n > 0`, `factorial(n) = n * factorial(n-1)`. The base case stops recursion when `n` reaches `0` (or `1`, but using `0` is cleaner because it directly matches the mathematical definition and avoids an extra case). Important edge case: when `n == 0`, the function must return `1`, not loop infinitely or return `0`. Since the function uses recursion, each call reduces `n` by `1`, so for input `n`, there are `n+1` total calls (including the base case). Each call does constant work (one multiplication), so the time complexity is `O(n)` and the auxiliary space complexity is `O(n)` due to the call stack depth. For `n = 20`, the result fits in a `long long` (since `20! ≈ 2.43e18 < 9.22e18`), but for `n = 21`, it overflows; the problem statement restricts input to values that produce a valid `long long` result. The implementation should use `long long` throughout to match the return type and avoid truncation.

Write a C++ function that takes two non-negative integers `n` and `m` as input and returns the result of `floor(n * m / 2)`. The function must use only integer arithmetic, must handle potentially large values (up to 10^9 for each), and must avoid overflow when multiplying. The result is defined as the largest integer less than or equal to the exact real-valued product divided by 2. For example, if `n = 3` and `m = 4`, the exact product is 12, half is 6, so the result is 6. If `n = 3` and `m = 3`, the product is 9, half is 4.5, and the floor is 4. The function should be named `floorHalfProduct` and accept two `int` parameters, returning `long long` to accommodate the result. Do not use floating-point arithmetic.

The straightforward computation `n * m / 2` using `int` can overflow if `n * m` exceeds 2^31 - 1, since `n` and `m` can each be up to 10^9, making the product up to 10^18. To avoid overflow, we need to compute the floor division by 2 carefully. One approach is to divide one of the numbers by 2 first if it is even, then multiply, and if both are odd, handle the remainder separately. Specifically, we can use the identity: for integers `a` and `b`, `floor(a*b/2) = (a/2)*b + (a%2)*(b/2)` if we assume `a` and `b` are non-negative. This works because the product can be split into an integer part and a fractional part when both are odd. More generally, we can compute `(n/2)*(m) + (n%2)*(m/2)` but this might still overflow if both `n/2` and `m` are large (e.g., `n=10^9`, `m=10^9` gives `(5e8)*(1e9)=5e17`, which fits in 64-bit but not 32-bit). Therefore, we should use `long long` for intermediate multiplication. A safe method: convert `n` and `m` to `long long`, then compute `(n * m) / 2` using 64-bit integer arithmetic, which will not overflow because the maximum product is 10^18, well within `long long` limits. Edge cases: zero inputs produce zero. Negative inputs are not allowed per the problem statement, but if they were, the division semantics would need care; we assume non-negative. Time complexity is O(1), space complexity O(1).

#include <cstdint>

// Returns floor(n * m / 2) for non-negative integers n and m.
// Uses 64-bit arithmetic to avoid overflow from the multiplication.
long long floorHalfProduct(int n, int m) {
    // Cast to long long before multiplication to avoid 32-bit overflow.
    long long nn = static_cast<long long>(n);
    long long mm = static_cast<long long>(m);
    return (nn * mm) / 2;
}

#include <cassert>
#include <cstdint>

long long floorHalfProduct(int n, int m); // forward declaration

int main() {
    // Basic cases
    assert(floorHalfProduct(3, 4) == 6);
    assert(floorHalfProduct(3, 3) == 4);
    assert(floorHalfProduct(0, 5) == 0);
    assert(floorHalfProduct(5, 0) == 0);
    
    // Symmetry
    assert(floorHalfProduct(4, 3) == 6);
    
    // Odd and even combinations
    assert(floorHalfProduct(2, 5) == 5);
    assert(floorHalfProduct(5, 2) == 5);
    assert(floorHalfProduct(1, 1) == 0);
    
    // Large values that would overflow 32-bit multiplication
    assert(floorHalfProduct(1000000000, 1000000000) == 500000000000000000LL);
    assert(floorHalfProduct(1000000000, 999999999) == 499999999500000000LL);
    assert(floorHalfProduct(123456789, 987654321) == 60941993474158110LL);
    
    // Stress with all ones
    assert(floorHalfProduct(1, 1) == 0);
    assert(floorHalfProduct(1, 1000000000) == 500000000);
    
    return 0;
}

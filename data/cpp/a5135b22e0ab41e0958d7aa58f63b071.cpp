// Given two integers `n` and `m` (where `1 ≤ n ≤ 10^6` and `0 ≤ m ≤ n^2`), write a C++ function that computes and returns the number of pairs of distinct ordered pairs (i, j) from the set {1, 2, ..., n} such that the pair (i, j) is NOT equal to any of the first `m` specified pairs. The input `m` represents the number of already-removed (forbidden) pairs, and the function must return `n*n - m`. However, note that the problem simplifies to computing the square of `n` and subtracting `m`, but the original code uses `long long` to avoid overflow. Your task is to implement a function that takes `n` and `m` as `long long` parameters, computes the result using 64-bit arithmetic, and returns the result as a `long long`. The function must correctly handle large values of `n` (up to 10^6) where `n*n` can exceed 32-bit integer range.

The problem is straightforward: we are given `n` and `m`, and we need to output `n*n - m`. The key insight is that `n` can be up to 10^6, so `n*n` can be up to 10^12, which exceeds the 32-bit `int` range. Therefore, we must use a 64-bit integer type (e.g., `long long`) for the computation. The main algorithm is simply: compute `n * n` using 64-bit multiplication, then subtract `m`. No edge cases beyond the type overflow need consideration, but we must ensure the function parameters and return type are `long long` to handle large inputs. Time complexity is O(1) and space complexity is O(1).

#include <cstdint>

// Computes the number of allowed pairs given the total grid size n and m forbidden pairs.
long long countAllowedPairs(long long n, long long m) {
    // Compute n*n as 64-bit to avoid overflow, then subtract m.
    return n * n - m;
}

#include <cassert>
#include <cstdint>

long long countAllowedPairs(long long n, long long m);

int main() {
    // Basic small cases
    assert(countAllowedPairs(1, 0) == 1);
    assert(countAllowedPairs(1, 1) == 0);
    assert(countAllowedPairs(2, 0) == 4);
    assert(countAllowedPairs(2, 3) == 1);
    assert(countAllowedPairs(3, 5) == 4);
    
    // Edge case where n*n exceeds 32-bit range
    assert(countAllowedPairs(1000000, 0) == 1000000000000LL);
    assert(countAllowedPairs(1000000, 999999999999LL) == 1);
    assert(countAllowedPairs(1000000, 1000000000000LL) == 0);
    
    // Random mid-range checks
    assert(countAllowedPairs(100, 50) == 9950);
    assert(countAllowedPairs(50000, 123456789) == 2500000000LL - 123456789);
    
    return 0;
}

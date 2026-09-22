/*
You are given integers `x`, `y`, `p`, and `q` (all non‑negative, with `y` ≥ `x` and `p`, `q` > 0). In one operation, you may increase `x` by `p` and decrease `y` by `q` simultaneously (both changes always happen together). You may perform this operation any non‑negative integer number of times `k`. After `k` operations, the two values become `x + p*k` and `y - q*k`. Determine whether it is possible for the first value to equal the second value exactly after some number of operations. If yes, return the smallest non‑negative integer `k` that achieves equality. If no such `k` exists, return `-1`. Write a C++ function `long long findMinOperations(long long x, long long y, long long p, long long q)` that implements this logic. Note that all intermediate and final values may exceed 32‑bit range, so use 64‑bit integers.
*/

#include <cstdint>

// Returns the smallest non-negative k such that x + p*k == y - q*k,
// or -1 if no such k exists. All parameters are non-negative, p>0, q>0.
long long findMinOperations(long long x, long long y, long long p, long long q) {
    // If y < x, equality is impossible because y decreases while x increases.
    if (y < x) {
        return -1;
    }
    long long diff = y - x;
    long long step = p + q;
    // If diff is not divisible by step, no integer solution exists.
    if (diff % step != 0) {
        return -1;
    }
    return diff / step;
}

#include <cassert>

// Forward declaration of the solution function (included from the solution).
long long findMinOperations(long long, long long, long long, long long);

int main() {
    // Basic cases from the original snippet style
    assert(findMinOperations(1, 10, 1, 2) == 3); // 1+3=4, 10-6=4
    assert(findMinOperations(5, 5, 2, 3) == 0);  // already equal
    assert(findMinOperations(1, 2, 1, 1) == -1); // 1+k == 2-k => 2k=1 no integer
    assert(findMinOperations(0, 6, 1, 1) == 3);  // 0+3=3, 6-3=3
    assert(findMinOperations(10, 20, 3, 2) == 2); // 10+6=16, 20-4=16
    assert(findMinOperations(3, 7, 2, 4) == -1); // diff=4, step=6, 4%6 !=0
    assert(findMinOperations(100, 100, 7, 9) == 0);
    assert(findMinOperations(1, 1000000000000LL, 1, 999999999999LL) == 999999999999LL);
    // Large numbers do not overflow because we use long long.
    assert(findMinOperations(0, 1, 1, 1) == -1); // diff=1, step=2, not divisible
    assert(findMinOperations(4, 10, 2, 1) == 2); // 4+4=8, 10-2=8
    return 0;
}

// The problem reduces to solving the linear equation `x + p*k == y - q*k` for integer `k ≥ 0`. Rearranging gives `(p+q)*k == y - x`. If `y - x` is negative or not divisible by `(p+q)`, then no non‑negative integer solution exists, so return `-1`. Otherwise, the unique solution is `k = (y - x) / (p + q)`, which is automatically non‑negative because `y ≥ x`. This direct arithmetic solution is correct because the equation is linear and monotone. Edge cases: if `y == x` and `(p+q)` divides `0` (which is true for any positive divisor), then `k=0` is valid. If `y < x` (though not in constraints, but safe to handle), return `-1` because `y - q*k` would become negative faster than `x` grows, so equality cannot occur. Time complexity: O(1), space O(1).

Given an integer `n` (1 ≤ n ≤ 10^18), write a C++ function `ll ceilHalf(ll n)` that returns the smallest integer greater than or equal to `n/2`. More precisely, if `n` is even, return `n/2`; if `n` is odd, return `(n/2) + 1`. This is equivalent to the ceiling of `n/2`. The function must handle very large inputs up to 10^18, so use a 64‑bit integer type (`long long`). Do not use floating‑point arithmetic or the `ceil` library function; implement it using integer arithmetic only. The function should be pure (no side effects) and `const`‑correct.
#include <cassert>

int main() {
    assert(ceilHalf(0) == 0);
    assert(ceilHalf(1) == 1);
    assert(ceilHalf(2) == 1);
    assert(ceilHalf(3) == 2);
    assert(ceilHalf(4) == 2);
    assert(ceilHalf(17) == 9);
    assert(ceilHalf(1000000000000000000LL) == 500000000000000000LL);
    assert(ceilHalf(999999999999999999LL) == 500000000000000000LL);
    assert(ceilHalf(9) == 5);
    assert(ceilHalf(10) == 5);
}
#include <cstdint>

using ll = long long;

// Returns the ceiling of n/2 for a non-negative integer n.
ll ceilHalf(ll n) {
    if (n % 2 == 0) {
        return n / 2;
    } else {
        return n / 2 + 1;
    }
}
// The problem is trivial: the ceiling of `n/2` can be computed directly. For even `n`, integer division `n/2` already gives the exact result. For odd `n`, integer division truncates toward zero (which for positive numbers is floors), so adding 1 gives the ceiling. A single formula that works for all positive integers is `(n + 1) / 2`, but this can overflow for `n = 10^18` (since `n+1` is still within `long long` range up to 9.22×10^18, so it’s safe). However, to be extra safe and clear, the branch‑based approach avoids any possibility of overflow and is simpler. Edge cases: `n = 1` → returns 1; `n = 2` → returns 1; `n = 3` → returns 2; `n = 10^18` (even) → returns 5×10^17. Time complexity is O(1), space complexity is O(1).

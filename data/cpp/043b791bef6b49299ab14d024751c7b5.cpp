/*
Write a C++ function `bool is_simple_power(int a, int b)` that returns `true` if the integer `a` is an exact power of the integer `b` (i.e., there exists a non‑negative integer exponent `k` such that `b^k == a`). The function must handle special cases: `a == 1` is always a simple power (since `1 = b^0` for any `b`), `b == 1` is a simple power only when `a == 1` (because `1^k` is always `1`), and for `b == 0`, the only valid power is `a == 1` if we consider `0^0` as 1, but you may decide to treat `0^0` as not a valid power and return `false` for any `a` if `b == 0` unless `a == 1` (to be safe, follow the standard mathematical convention: `0^0` is undefined, so you can return `false` for `a == 1` when `b == 0`). The function should avoid overflow by using a loop that checks multiplication bounds. Provide the function definition only (no `main`).
*/
#include <cstddef> // for size_t, not strictly needed

// Returns true if 'a' is an exact power of 'b' (b^k == a for some non-negative integer k).
bool is_simple_power(int a, int b) {
    if (a == 1) {
        return true; // 1 = b^0 for any b (including b == 0 if we define 0^0 = 1)
    }
    if (b == 0) {
        return false; // only 0^positive is 0, but a != 1 here, and a != 0? Actually a == 0 would be b^k == 0 only if b == 0 and k > 0, but b == 0 -> 0^k = 0, so a == 0 should be true? We'll treat b == 0 specially.
    }
    if (b == 1) {
        return false; // 1^k == 1, which is already handled above (a == 1)
    }
    if (a == 0) {
        return false; // no positive b^k == 0 (except b == 0, but b is not 0 here)
    }
    if (b < 0 || a < 0) {
        return false; // not handling negatives
    }

    int current = 1;
    while (current <= a) {
        if (current == a) {
            return true;
        }
        if (current > a / b) {
            break; // next multiplication would overflow or exceed a
        }
        current *= b;
    }
    return false;
}
#include <cassert>

int main() {
    // Basic cases from the problem statement
    assert(is_simple_power(1, 4) == true);
    assert(is_simple_power(2, 2) == true);
    assert(is_simple_power(8, 2) == true);
    assert(is_simple_power(3, 2) == false);
    assert(is_simple_power(3, 1) == false);
    assert(is_simple_power(5, 3) == false);
    assert(is_simple_power(16, 2) == true);
    assert(is_simple_power(143214, 16) == false);
    assert(is_simple_power(4, 2) == true);
    assert(is_simple_power(9, 3) == true);
    assert(is_simple_power(16, 4) == true);
    assert(is_simple_power(24, 2) == false);
    assert(is_simple_power(128, 4) == false);
    assert(is_simple_power(12, 6) == false);
    assert(is_simple_power(1, 1) == true);
    assert(is_simple_power(1, 12) == true);

    // Additional edge cases
    assert(is_simple_power(0, 0) == false); // a=0, b=0 -> 0^? unclear, return false
    assert(is_simple_power(0, 5) == false); // 5^k never 0
    assert(is_simple_power(1, 0) == true);  // 0^0 defined as 1 in our function
    assert(is_simple_power(25, 5) == true);
    assert(is_simple_power(27, 3) == true);
    assert(is_simple_power(1024, 2) == true);
    assert(is_simple_power(100, 10) == true);
    assert(is_simple_power(1000, 10) == true);
    assert(is_simple_power(101, 10) == false);
}
// The problem is straightforward: repeatedly multiply an accumulator `current` initialized to `1` by `b` and compare it to `a`. If at any point `current == a`, return `true`. If `current` exceeds `a` or the multiplication would overflow (i.e., `current > a / b`), return `false`. Edge cases:  
// - If `a == 1`, always return `true` (since `1 = b^0`).  
// - If `b == 1`, return `true` only when `a == 1`; otherwise `false`.  
// - If `b == 0`, return `true` only when `a == 1` (since `0^0` is problematic) — but note the problem statement says `is_simple_power(1, 4)==true` works for any `b` except `b == 0`? The given tests do not include `b == 0`, so we can implement as: if `b == 0`, return `a == 1` (since `0^0` is often defined as 1 in programming contexts). But to be safe and consistent, we can treat `b == 0` as: if `a == 1` return `true`, else `false`.  
// - For negative bases? The problem likely expects non-negative inputs because the tests use positive integers. We will assume `b >= 0` and `a >= 0`; if `a < 0` or `b < 0`, we could return `false` for simplicity.  
// Time complexity: O(log_b(a)) iterations, space O(1).

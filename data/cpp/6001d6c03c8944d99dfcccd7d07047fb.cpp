/*
Write a C++ function that, given three integers `N`, `a`, and `b`, determines whether `N` can be expressed as `a^k + b*m` for some non-negative integers `k` and `m`. More precisely, decide if there exists an integer `k ≥ 0` such that `(N - a^k)` is divisible by `b` (with the usual convention that `a^0 = 1`). The function should return `true` if possible, `false` otherwise. Handle edge cases where `a = 1` and `b = 0` appropriately. For this task, assume `N`, `a`, `b` are non-negative integers, with `N ≥ 1`, `a ≥ 1`, `b ≥ 0`. Note: when `b = 0`, divisibility means the remainder must be exactly `0`, so you must check `N == a^k` exactly. When `a = 1`, there is only one term in the sequence of powers (1, 1, 1, ...), so handle that separately.
*/

#include <cstdint>

// Determines whether N can be written as a^k + b*m for some k >= 0, m >= 0.
// a >= 1, b >= 0, N >= 1.
bool canRepresentAsPowerPlusMultiple(long long N, long long a, long long b) {
    long long power = 1; // a^0
    
    while (power <= N) {
        if (b == 0) {
            if (power == N) return true;
        } else {
            if ((N - power) % b == 0) return true;
        }
        
        // Avoid overflow: if next power would exceed N, break.
        if (a == 1) {
            break; // infinite loop otherwise; but a==1 is handled below
        }
        if (power > N / a) break;
        power *= a;
    }
    
    // Note: if a == 1, the while loop runs only once (power stays 1) and returns true if condition holds.
    // If a > 1, we've checked all powers up to N.
    return false;
}

#include <cassert>

int main() {
    // Basic cases with a > 1, b > 0
    assert(canRepresentAsPowerPlusMultiple(10, 2, 3) == true);  // 2^3 + 3*? = 8 + 2 = 10? Actually 8+3*? -> 8+3*0=8, 8+3*2=14, but 2^2+3*2=4+6=10
    assert(canRepresentAsPowerPlusMultiple(7, 2, 1) == true);   // 2^2 + 1*3 = 7
    assert(canRepresentAsPowerPlusMultiple(9, 3, 2) == true);   // 3^2 + 2*0 = 9
    assert(canRepresentAsPowerPlusMultiple(5, 2, 2) == false);  // 1,2,4,8... none leave remainder divisible by 2 (1%2=1, 3%2=1, 5%2=1)
    
    // Edge case a = 1
    assert(canRepresentAsPowerPlusMultiple(10, 1, 3) == true);  // 1 + 3*3 = 10
    assert(canRepresentAsPowerPlusMultiple(10, 1, 4) == false); // 1 + 4*m = 10 -> m=2.25 no
    
    // Edge case b = 0
    assert(canRepresentAsPowerPlusMultiple(16, 2, 0) == true);  // 2^4 = 16
    assert(canRepresentAsPowerPlusMultiple(17, 2, 0) == false); // no power of 2 equals 17
    
    // Edge case N=1
    assert(canRepresentAsPowerPlusMultiple(1, 5, 7) == true);   // 5^0 = 1, remainder 0 divisible by 7
    assert(canRepresentAsPowerPlusMultiple(1, 1, 0) == true);   // 1^0 = 1
    
    // Larger N
    assert(canRepresentAsPowerPlusMultiple(1000000000000000000LL, 10, 2) == true); // 10^18 = 10^18 + 2*0
    assert(canRepresentAsPowerPlusMultiple(1000000000000000001LL, 10, 2) == false); // cannot because 10^18 leaves 1, 10^17 etc leave different remainders, none divisible by 2
}

// The key observation is that we only need to check successive powers of `a` until the power exceeds `N` (because after that, `a^k` alone is larger than `N`, and since `b*m` is non-negative, the sum cannot equal `N`). For each power `p = a^k`, we check if `(N - p)` is a multiple of `b` (i.e., `(N - p) % b == 0`). Special cases: if `a == 1`, then all powers equal `1`, so only the condition `(N - 1) % b == 0` matters (or `N == 1` if `b == 0`). If `b == 0`, we require `N - p == 0` exactly. Also, when `a > 1`, the value of `p` grows at least exponentially, so the loop runs at most `O(log_a N)` iterations, which is `O(log N)` time. Space complexity is `O(1)`. Note that using `long long` is necessary to avoid overflow when computing powers, as `N` could be up to, say, 1e18, and `a^k` could grow quickly. The loop condition should carefully prevent overflow by checking `p <= N / a` before multiplication.

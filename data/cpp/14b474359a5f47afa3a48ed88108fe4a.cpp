/*
Create a C++ function that takes two non-negative integers `a` and `b` (up to 10^18) and returns a pair of long long integers. If `a` equals `b`, the function should return `{0, 0}`. Otherwise, the first element should be the maximum possible value of `gcd(a + k, b + k)` over all non-negative integers `k`, and the second element should be the minimum non-negative value of `k` that achieves this maximum. The function should handle cases where `a` and `b` are given in any order.
*/
#include <utility>
#include <algorithm>

// Returns {maximum_gcd_value, minimum_k_to_achieve_it}.
// If a == b, returns {0, 0}.
std::pair<long long, long long> maxGcdWithShift(long long a, long long b) {
    if (a == b) return {0, 0};
    if (a > b) std::swap(a, b);

    long long diff = b - a;
    long long rem = a % diff;
    long long min_k = (rem == 0) ? 0 : std::min(rem, diff - rem);
    return {diff, min_k};
}
#include <cassert>

int main() {
    // Sample from problem link
    assert(maxGcdWithShift(8, 12) == std::make_pair(4LL, 0LL));
    assert(maxGcdWithShift(9, 9) == std::make_pair(0LL, 0LL));
    assert(maxGcdWithShift(20, 24) == std::make_pair(4LL, 0LL));
    
    // General cases
    assert(maxGcdWithShift(10, 15) == std::make_pair(5LL, 0LL));
    assert(maxGcdWithShift(1, 5) == std::make_pair(4LL, 3LL));   // k=3 gives gcd(4,8)=4
    assert(maxGcdWithShift(0, 6) == std::make_pair(6LL, 0LL));   // k=0 gives gcd(0,6)=6
    assert(maxGcdWithShift(5, 5) == std::make_pair(0LL, 0LL));
    
    // Large numbers within 10^18
    assert(maxGcdWithShift(1000000000000000000LL, 999999999999999999LL) == std::make_pair(1LL, 1LL));
    assert(maxGcdWithShift(999999999999999999LL, 1000000000000000000LL) == std::make_pair(1LL, 1LL));
    
    // When a is already a multiple of diff
    assert(maxGcdWithShift(6, 10) == std::make_pair(4LL, 2LL));  // k=2 gives gcd(8,12)=4
    assert(maxGcdWithShift(6, 14) == std::make_pair(8LL, 2LL));  // k=2 gives gcd(8,16)=8
}
// The key observation is that `gcd(a + k, b + k)` equals `gcd(b - a, a + k)` because subtracting the two arguments doesn't change the gcd. For any non-zero difference `d = abs(a - b)`, the gcd is always a divisor of `d`. The maximum possible gcd is therefore `d` itself, achieved when `d` divides `a + k`, i.e., `a + k` is a multiple of `d`. The smallest such non-negative `k` is the distance from `a` to the nearest multiple of `d` in the forward direction, which is `min(a % d, d - (a % d))` if `a % d != 0`, otherwise `0`. Since `k` must be non-negative, this distance formula works. If `a == b`, the gcd becomes arbitrarily large (any `k` yields `gcd(a+k, a+k) = a+k`), so we return `{0, 0}` as per the output specification (meaning no finite maximum). The algorithm runs in O(1) time and O(1) space, using only modular arithmetic. Edge cases: when `a` or `b` is 0, the difference is the other number, and the formula still works; when `a % d == 0`, the minimum is 0.

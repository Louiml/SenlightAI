/*
Given a positive integer `a`, write a C++ function `splitIntoTwoParts` that returns a `std::pair<long long, long long>` representing two non-negative integers `(x, y)` such that `x + y = a` and their difference `|x - y|` is as small as possible. If multiple valid pairs exist with the same minimal difference, return the pair where `x ≤ y` is not required—instead, follow the rule: if the remainder of `a` divided by 3 is 1, the first number is one larger than the second; if the remainder is 2, the second is one larger than the first; if the remainder is 0, both numbers are equal. The function must handle large values up to 10^18 and must not rely on floating-point arithmetic; use integer division and remainder operations only.
*/
#include <utility>

// Returns a pair (x, y) such that x + y = a and |x - y| is minimal.
// If remainder is 1, x is one larger; if remainder is 2, y is one larger.
std::pair<long long, long long> splitIntoTwoParts(long long a) {
    long long base = a / 3;
    long long rem = a % 3;
    if (rem == 0) {
        return {base, base};
    } else if (rem == 1) {
        return {base + 1, base};
    } else {  // rem == 2
        return {base, base + 1};
    }
}
#include <cassert>
#include <utility>

// Function declaration (must match the solution exactly)
std::pair<long long, long long> splitIntoTwoParts(long long a);

int main() {
    // Basic cases
    assert(splitIntoTwoParts(3) == std::make_pair(1LL, 1LL));
    assert(splitIntoTwoParts(4) == std::make_pair(2LL, 1LL));
    assert(splitIntoTwoParts(5) == std::make_pair(1LL, 2LL));
    assert(splitIntoTwoParts(6) == std::make_pair(2LL, 2LL));
    assert(splitIntoTwoParts(7) == std::make_pair(3LL, 2LL));
    assert(splitIntoTwoParts(8) == std::make_pair(2LL, 3LL));
    
    // Larger values
    assert(splitIntoTwoParts(1) == std::make_pair(1LL, 0LL));
    assert(splitIntoTwoParts(2) == std::make_pair(0LL, 1LL));
    assert(splitIntoTwoParts(1000000000000000000LL) == std::make_pair(333333333333333333LL, 333333333333333334LL));
    
    // Edge with large remainder 1
    assert(splitIntoTwoParts(1000000000000000001LL) == std::make_pair(333333333333333334LL, 333333333333333333LL));
    
    return 0;
}
// The problem essentially asks to split `a` into two roughly equal parts while giving priority to the exact ratio derived from dividing `a` by 3. The core idea is to compute `base = a / 3` (integer division) and then adjust based on the remainder `rem = a % 3`. When `rem == 0`, `(base, base)` sums to `a` and difference is 0. When `rem == 1`, the smallest possible absolute difference is 1, achieved by `(base+1, base)` because `(base+1) + base = 2*base+1 = a` (since `a = 3*base+1`). When `rem == 2`, similarly `(base, base+1)` gives difference 1 and sum `2*base+1 = a` (since `a = 3*base+2`). Edge case: `a` is always positive per the problem statement, so no negative or zero handling needed, but the code works for zero as well (returns `(0,0)`). Time complexity is O(1) and space complexity is O(1). No overflow occurs because the maximum value of each part is at most roughly `a/3 + 1`, which fits in `long long` for `a ≤ 10^18`.

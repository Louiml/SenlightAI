Write a C++ function that, given an integer `n` (the number of levels in a game) and an integer `k` (the experience points gained per level), returns the total experience points needed to reach level `n` from level 1, assuming experience requirements increase linearly such that the total experience from level 1 to level `n` equals `1 + (n - 1) * k`. The function must be named `totalExperience` and accept two long long integers, returning a `long long`. The inputs `n` and `k` will always be positive (≥1), and the result is guaranteed to fit within a 64-bit signed integer. The function must be efficient and not use loops or recursion.
#include <cassert>

int main() {
    // Basic cases
    assert(totalExperience(1, 10) == 1);        // Level 1 needs only 1 XP
    assert(totalExperience(2, 5) == 6);         // 1 + (2-1)*5 = 6
    assert(totalExperience(5, 3) == 13);        // 1 + 4*3 = 13
    // Larger numbers
    assert(totalExperience(1000000000, 1000000000) == 999999999000000001LL);
    // Minimum k
    assert(totalExperience(10, 1) == 10);       // 1 + 9*1 = 10
    // Single level
    assert(totalExperience(1, 123456) == 1);    // Always 1 at level 1
    // Symmetry check (n-1)*k vs k*(n-1)
    assert(totalExperience(7, 8) == totalExperience(8, 7) + 1 ? totalExperience(7, 8) : totalExperience(7, 8));
    // This is a sanity check: 1+(6*8)=49, 1+(7*7)=50, so not equal, but we just verify the first.
    assert(totalExperience(7, 8) == 49);
    // Ensure no negative or zero results for positive inputs
    assert(totalExperience(3, 2) == 5);
    assert(totalExperience(100, 1) == 100);
    return 0;
}
#include <cstdint>

// Returns total experience points needed to reach level n from level 1,
// given k experience points are gained per level (formula: 1 + (n-1)*k).
long long totalExperience(long long n, long long k) {
    return 1 + (n - 1) * k;
}
// The problem is a direct application of a linear formula. The description gives the total experience needed to reach level `n` as `1 + (n - 1) * k`. This is a simple arithmetic expression that can be evaluated in constant time. There are no special edge cases beyond ensuring that `n` and `k` are positive integers, but since the input constraints guarantee this, no validation is needed. The multiplication `(n - 1) * k` could overflow if both numbers are near the maximum 64-bit range, but the problem statement guarantees the final result fits in 64 bits, so we can safely compute it. The time complexity is O(1) and the auxiliary space complexity is O(1).

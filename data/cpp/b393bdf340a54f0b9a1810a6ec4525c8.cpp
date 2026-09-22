// Write a C++ function `bool canFormSequence(int n, int a, int b, int s)` that determines whether there exists an arithmetic progression of length `n` where the first term is `a`, the last term is `b`, and the sum of all terms equals exactly `s`. The function should return `true` if such a progression exists, and `false` otherwise. Note that all terms must be integers, and the progression must be valid (i.e., it must have a constant integer common difference). The constraints are: `1 <= n <= 1000`, and `a`, `b`, and `s` are integers within the range `[-10^9, 10^9]`. The function must handle edge cases where `n == 1` (only one term, so `a` must equal `b` and `s` must equal `a`), `n == 2` (two terms, so `s` must equal `a + b` and no constraint on common difference other than it being `b - a`), and `n > 2` (where a common difference `d` is determined by `d = (b - a) / (n - 1)` if divisible, and the sum is computed using the formula `s_expected = n * (a + b) / 2`, which must equal `s`). Validate that `(b - a) % (n - 1) == 0` for `n > 1` to ensure an integer common difference, and that the sum formula is consistent. If any condition fails, return `false`; otherwise return `true`.

The problem reduces to checking whether an arithmetic progression with given first term `a`, last term `b`, length `n`, and sum `s` can exist. For an arithmetic progression, the common difference `d` is `(b - a) / (n - 1)` when `n > 1`. This must be an integer, so we require `(b - a) % (n - 1) == 0`. For any arithmetic progression with `n` terms, the sum is `n * (first + last) / 2`, which is an integer only if `n * (a + b)` is even. However, since `d` is integer, the sum is automatically an integer, and the formula `n * (a + b) / 2` must be exactly equal to `s`. Edge cases: when `n == 1`, the progression has only the first term, so `a` must equal `b` and `s` must equal `a`. When `n == 2`, the sum is simply `a + b` (since `d = b - a` is always integer), so the only condition is `s == a + b`. For `n > 2`, we check the divisibility of `b - a` by `n - 1` and the sum equality. The algorithm runs in constant time `O(1)` and uses `O(1)` auxiliary space. The key insight is to avoid constructing the progression and only use mathematical conditions.

#include <cstdlib>  // for std::abs (not needed here, but included for completeness)

// Check if an arithmetic progression of length n with first term a, last term b,
// and total sum s exists. Returns true if such a progression exists.
bool canFormSequence(int n, int a, int b, int s) {
    if (n == 1) {
        // Only one term: it must be both first and last, and sum must equal it.
        return (a == b) && (s == a);
    }
    if (n == 2) {
        // Two terms: sum is simply a + b.
        return s == (a + b);
    }
    // For n > 2: the common difference must be an integer.
    // The difference (b - a) must be divisible by (n - 1).
    if ((b - a) % (n - 1) != 0) {
        return false;
    }
    // The sum of an arithmetic progression is n * (a + b) / 2.
    // Since d is integer, the sum is integer; check equality with s.
    long long expectedSum = static_cast<long long>(n) * (static_cast<long long>(a) + static_cast<long long>(b)) / 2;
    return expectedSum == static_cast<long long>(s);
}

#include <cassert>

int main() {
    // Single term cases
    assert(canFormSequence(1, 5, 5, 5) == true);
    assert(canFormSequence(1, 5, 6, 5) == false);
    assert(canFormSequence(1, -3, -3, -3) == true);
    assert(canFormSequence(1, 7, 7, 8) == false);

    // Two term cases
    assert(canFormSequence(2, 1, 3, 4) == true);
    assert(canFormSequence(2, 1, 3, 5) == false);
    assert(canFormSequence(2, -2, 2, 0) == true);
    assert(canFormSequence(2, -2, 2, -1) == false);

    // More than two terms, valid progression
    assert(canFormSequence(3, 1, 3, 6) == true);     // 1,2,3 sum=6
    assert(canFormSequence(4, 1, 4, 10) == true);    // 1,2,3,4 sum=10
    assert(canFormSequence(4, 2, 8, 20) == true);    // 2,4,6,8 sum=20
    assert(canFormSequence(5, 10, 2, 30) == true);   // 10,8,6,4,2 sum=30

    // More than two terms, invalid due to non-integer common difference
    assert(canFormSequence(4, 1, 4, 9) == false);    // d not integer
    assert(canFormSequence(3, 1, 4, 6) == false);    // d=1.5, non-integer
    assert(canFormSequence(3, 1, 3, 7) == false);    // sum mismatch
    assert(canFormSequence(4, 2, 8, 21) == false);   // sum mismatch

    // Large numbers (use long long internally)
    assert(canFormSequence(3, 1000000000, 1000000000, 3000000000LL) == true); // all same, sum=3e9
    assert(canFormSequence(3, -1000000000, -1000000000, -3000000000LL) == true);
    assert(canFormSequence(3, 1000000000, -1000000000, 0) == true); // 1e9,0,-1e9 sum=0
    assert(canFormSequence(2, 1000000000, -1000000000, 0) == true);

    return 0;
}

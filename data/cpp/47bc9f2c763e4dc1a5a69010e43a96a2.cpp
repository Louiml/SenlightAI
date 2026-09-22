// Write a C++ function `countLuckySequences(int n)` that takes a non-negative integer `n` and returns the total number of distinct strings of length exactly `n` that can be formed using only the digits `'8'` and `'7'`, where every prefix of the string (from length 1 up to length `n`) is also considered valid. In other words, count all possible strings of length exactly `n` over the alphabet `{7, 8}` such that the recursive generation rule (starting from a single character, and at each step appending either `8` or `7` to the right) produces exactly `n`-length strings. The function should return the total count as a `long long`. For example, for `n = 1`, there are 2 strings: `"8"` and `"7"`. For `n = 2`, there are 4 strings: `"88"`, `"87"`, `"78"`, `"77"`. The function must handle values of `n` up to such that the result fits in a `long long` (i.e., `n` up to about 62). Do not use recursion or explicit generation; instead, derive a closed-form formula based on the observation that the number of strings of length exactly `n` is \(2^n\), and the total number of strings of lengths from 1 to `n` is \(2^{1}+2^{2}+\dots+2^{n}=2^{n+1}-2\). The function should return that total sum (i.e., for a given `n`, return \(2^{n+1} - 2\)). Edge case: for `n = 0`, the sum of lengths from 1 to 0 is 0, so return 0.

// The problem reduces to counting all valid strings of lengths from 1 to `n` that can be formed using only digits `7` and `8`, where the generation rule is that at each step you append either `8` or `7` to the end of any existing string. This forms a complete binary tree of depth `n`, where each node at depth `d` (starting depth 1) represents a string of length `d`. The total number of nodes in this tree (i.e., the total number of strings of lengths 1 through `n`) is the sum of a geometric series: \(2^1 + 2^2 + \dots + 2^n = 2^{n+1} - 2\). This formula works because at each level `k` (from 1 to `n`), there are exactly `2^k` strings. The maximum `n` allowed is limited by the `long long` range; since `2^{63}` overflows `long long`, we must ensure `n+1` is at most 62 or use a safe check. For `n` up to 62, the result fits in `long long`. The time complexity is O(1) because we just compute a power of two using bit shifting or `(1LL << (n+1))` (but carefully avoid overflow when `n+1` reaches 63; use `(1LL << (n+1)) - 2` only if `n+1 <= 62`). Alternatively, use `pow(2, n+1)` but avoid floating-point inaccuracies; use integer exponentiation. Edge case: `n=0` returns 0. Another edge case: if `n` is large (e.g., 62), the result is at most `2^63 - 2`, which can overflow signed `long long` if `2^63` exceeds the max (which it does). So we either restrict `n` to 61 or return modulo something, but the task says "result fits in long long", so we assume `n <= 61` or handle overflow by using `unsigned long long` or checking. For simplicity in the solution, we compute using `long long` and note that for `n=61`, `2^62 - 2` fits, and for `n=62`, `2^63 - 2` overflows signed `long long` (max ~9.22e18, `2^63` is 9.22e18, so `2^63 - 2` is actually less than max? Actually `2^63` is 9223372036854775808, which is one more than max signed long long (9223372036854775807), so `2^63 - 2` = 9223372036854775806, which fits! Wait, `2^63` is 9223372036854775808, but `2^63 - 2` = 9223372036854775806, which is less than max (which is 9223372036854775807). So actually `2^63 - 2` fits in signed long long. So `n` up to 62 is fine. But `2^63 - 2` is valid. So we can safely shift for `n+1 <= 63`. However, `1LL << 63` would overflow because shifting into the sign bit is undefined behavior. So we need to be careful. For `n=62`, `n+1=63`, `1LL << 63` is UB. So we can use a helper that computes `2^(k)` safely up to `k=62` using powers of two via multiplication or using `(1LL << (k-1)) * 2`. Simpler: use a loop or use `pow(2, n+1)` and cast to `long long` but that might lose precision for large values? Actually `pow` returns double, which can represent up to 2^53 exactly. For `n+1 > 53`, double may lose precision. So better to use integer exponentiation with a loop or a precomputed table. We'll implement a helper `power2` that returns `2^k` for `k <= 63` as `long long` using a bit shift but with a check: if `k == 63`, we know `2^63` cannot be represented as signed `long long`, but `2^63 - 2` can fit as we said? Wait, `2^63` itself is 9223372036854775808, which is out of range for signed `long long` (max 9223372036854775807). But we don't need `2^63`; we need `2^(n+1) - 2`; for `n=62`, that is `2^63 - 2` which is 9223372036854775806, which is within range. So we can compute `2^(n+1)` as `(1ULL << (n+1))` and cast to signed? But `1ULL << 63` is valid for unsigned long long, giving 9223372036854775808, then subtract 2 to get 9223372036854775806, which fits in signed. So we can use `unsigned long long` for the power and then cast to `long long` after subtraction. Simpler: use `long long result = (1LL << (n+1))` only for `n+1 <= 62`; for `n+1 == 63`, use `(1LL << 62) * 2` but that multiplication overflows signed? Actually `(1LL << 62)` is positive, multiply by 2 gives `2^63` which overflows signed. So better to use `unsigned long long` internally. In the reference solution, we'll handle by using `unsigned long long` for the power and then subtract 2 and cast to `long long`, but note that for `n=62`, result is still within signed range. For `n=63`, it would overflow, but we don't need that. So we'll check `if (n > 62) return -1` or just document that `n <= 62` is assumed. Time complexity O(1), space O(1).

#include <cstdint>

// Count the total number of distinct strings of lengths 1..n over alphabet {'7','8'}.
// Equivalent to 2^(n+1) - 2 for n >= 1, and 0 for n == 0.
long long countLuckySequences(int n) {
    if (n < 0) return -1;  // invalid input
    if (n == 0) return 0;

    // For n up to 62, 2^(n+1) - 2 fits in unsigned long long and also in signed long long.
    // Use unsigned long long to avoid signed overflow from shifting.
    unsigned long long power = 1ULL << (n + 1);  // 2^(n+1)
    unsigned long long total = power - 2;        // 2^(n+1) - 2
    // Ensure the result fits in signed long long: max is 2^63 - 1, but 2^(n+1)-2 is at most 2^63-2 for n=62.
    return static_cast<long long>(total);
}

#include <cassert>

int main() {
    // Small cases
    assert(countLuckySequences(0) == 0);
    assert(countLuckySequences(1) == 2);   // {"7","8"}
    assert(countLuckySequences(2) == 6);   // 2+4
    assert(countLuckySequences(3) == 14);  // 2+4+8

    // Larger case, validate formula
    assert(countLuckySequences(10) == (1LL << 11) - 2);  // 2046
    assert(countLuckySequences(20) == (1LL << 21) - 2);

    // Maximum safe: n = 62
    assert(countLuckySequences(62) == 9223372036854775806LL);

    // Negative input returns -1
    assert(countLuckySequences(-1) == -1);
}

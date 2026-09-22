You are given two positive integers `a` and `b` (where `1 ≤ a, b ≤ 10^18`). Write a function `long long countPairs(long long a, long long b)` that returns the number of pairs `(x, y)` such that `1 ≤ x ≤ a` and `1 ≤ y ≤ b`, and the decimal representation of `y` consists only of the digit `9` (i.e., `y ∈ {9, 99, 999, 9999, ...}`). The count is simply `a` multiplied by the number of such `y` values that are less than or equal to `b`. For example, if `a = 3` and `b = 100`, the valid `y` values are `9`, `99` (two values), so the result is `3 * 2 = 6`. If `b = 8`, there are no valid `y` values, so the result is `0`. The function must handle large inputs and must not use floating-point arithmetic.

// The problem reduces to counting how many repunit-like numbers consisting solely of the digit `9` are ≤ `b`. These numbers are: 9, 99, 999, ... Each such number is of the form `10^k - 1` for `k = 1, 2, 3, ...`. We can iterate starting from `p = 9`, and while `p ≤ b`, increment a counter and update `p = p * 10 + 9`. This works because `p` grows very quickly (exponentially), so the loop runs at most about 18 times for `b ≤ 10^18`. Then the answer is simply `a * cnt`. Edge cases: `b < 9` yields `cnt = 0`, so answer is `0`. Also, when `b` is exactly a repdigit (e.g., `b = 99`), the loop correctly includes it since the condition is `≤`. Time complexity: O(log₁₀ b) ≈ O(18) per call, which is constant in practice. Space complexity: O(1).

#include <cstdint>

// Count pairs (x, y) where 1 ≤ x ≤ a, 1 ≤ y ≤ b, and y is of the form 99...9.
// Returns a * (number of repdigit-9 values ≤ b).
long long countPairs(long long a, long long b) {
    long long cnt = 0;
    long long p = 9;

    // p takes values 9, 99, 999, ... while p ≤ b
    while (p <= b) {
        ++cnt;
        p = p * 10 + 9;  // next repdigit-9: 9 -> 99 -> 999 ...
    }

    return a * cnt;
}

#include <cassert>

int main() {
    // Basic cases
    assert(countPairs(1, 9) == 1);       // y=9
    assert(countPairs(1, 8) == 0);       // no valid y
    assert(countPairs(3, 100) == 6);     // y=9,99 → 2*3
    assert(countPairs(5, 99) == 10);     // y=9,99 → 2*5
    assert(countPairs(2, 999) == 6);     // y=9,99,999 → 3*2
    assert(countPairs(10, 9) == 10);     // y=9 → 1*10
    assert(countPairs(0, 100) == 0);     // a=0 → 0 pairs (even though b has values)
    assert(countPairs(1, 999999999999999999LL) == 18); // b = 10^18-1 → all 18 values from 9 to 999...9 (18 nines)
    assert(countPairs(7, 999999999999999999LL) == 126); // 18 * 7
    assert(countPairs(123456789, 123456788) == 0); // b < 9
    return 0;
}

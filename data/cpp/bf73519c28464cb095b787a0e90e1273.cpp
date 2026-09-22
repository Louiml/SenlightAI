// Write a C++ function `long long countNumbersWithAtMostTwoOnes(long long L, long long R)` that counts how many integers in the inclusive range `[L, R]` have at most two non-zero decimal digits in their standard decimal representation (i.e., a number can have any number of zeros, but the count of digits from 1–9 is ≤ 2). For example, 100, 1200, and 5 are valid, while 123 and 1010 (since it has two 1 digits and a 0, but the count of non‑zero digits is 2) are valid only if the total non‑zero digit count is ≤ 2; 123 has three non‑zero digits and is invalid. The function must handle up to 64‑bit unsigned range endpoints (0 ≤ L ≤ R ≤ 1e18) and return a `long long` result. The implementation must be efficient and deterministic.

// The problem is a classic digit DP (dynamic programming on digits). The core idea: count numbers from 0 to X that satisfy the condition, then compute `countUpTo(R) - countUpTo(L-1)`. For `countUpTo(X)`, if X < 0 return 0. Otherwise, extract the decimal digits of X into an array (least significant first). Define a recursive function `dp(pos, usedNonZero, tight)`: `pos` is the current digit index from most significant to least (or vice versa), `usedNonZero` is how many non‑zero digits have been placed (0,1,2), and `tight` indicates whether the prefix already matches X's prefix (if so, we cannot exceed the current digit of X). At each position, iterate possible digits `d` from 0 to `limit` (where limit = 9 if tight is false, otherwise X's digit at that position). The new `usedNonZero` becomes `usedNonZero + (d != 0 ? 1 : 0)`. If `usedNonZero > 2`, prune (return 0). When all positions are processed (base case), return 1 because we built a valid number. Use memoization with dimensions [20][3][2] since position count ≤ 19 (for 1e18), usedNonZero ≤ 2, and tight is boolean. Important edge cases: X = 0 (should return 1 because zero has zero non‑zero digits), and the subtraction L-1 when L=0 (handle by returning 0 for negative). Time complexity: O(number_of_digits * 3 * 2 * 10) ≈ O(19*60) per call, so O(1) for each range. Space complexity: O(19*3*2) for memoization.

#include <cstdint>
#include <cstring>
#include <vector>

// Count numbers from 0 to X (inclusive) that have at most two non-zero digit digits.
// Uses digit DP with memoization. X is assumed to be non-negative.
long long countUpTo(long long X) {
    if (X < 0) return 0;  // For L-1 when L=0

    // Extract decimal digits (most significant first) for DP traversal.
    std::vector<int> digits;
    if (X == 0) {
        digits.push_back(0);
    } else {
        long long temp = X;
        while (temp > 0) {
            digits.push_back(temp % 10);
            temp /= 10;
        }
        // digits are in least significant first order; reverse for MSB-first traversal.
        std::reverse(digits.begin(), digits.end());
    }

    int n = static_cast<int>(digits.size());
    // dp[pos][usedNonZero][tight]  -  tight is 0 or 1. Initialize to -1 meaning uncomputed.
    long long dp[20][3][2];
    memset(dp, -1, sizeof(dp));

    // Recursive lambda with memoization.
    std::function<long long(int, int, int)> dfs = [&](int pos, int usedNonZero, int tight) -> long long {
        if (usedNonZero > 2) return 0;  // Already more than two non-zero digits.
        if (pos == n) return 1;         // Built a valid number.

        long long &memo = dp[pos][usedNonZero][tight];
        if (memo != -1) return memo;

        int limit = tight ? digits[pos] : 9;
        long long total = 0;
        for (int d = 0; d <= limit; ++d) {
            int nextTight = tight && (d == limit) ? 1 : 0;
            int nextUsed = usedNonZero + (d != 0 ? 1 : 0);
            total += dfs(pos + 1, nextUsed, nextTight);
        }
        memo = total;
        return memo;
    };

    return dfs(0, 0, 1);
}

// Count numbers in [L, R] inclusive that have at most two non-zero digit digits.
long long countNumbersWithAtMostTwoOnes(long long L, long long R) {
    return countUpTo(R) - countUpTo(L - 1);
}

#include <cassert>

int main() {
    // Basic single-number tests.
    assert(countNumbersWithAtMostTwoOnes(0, 0) == 1);      // 0 is valid.
    assert(countNumbersWithAtMostTwoOnes(1, 1) == 1);      // "1" has one non-zero digit.
    assert(countNumbersWithAtMostTwoOnes(9, 9) == 1);      // "9" has one non-zero digit.
    assert(countNumbersWithAtMostTwoOnes(10, 10) == 1);    // "10" has one non-zero digit.
    assert(countNumbersWithAtMostTwoOnes(11, 11) == 1);    // "11" has two non-zero digits.
    assert(countNumbersWithAtMostTwoOnes(12, 12) == 1);    // "12" has two non-zero digits.
    assert(countNumbersWithAtMostTwoOnes(100, 100) == 1);  // "100" has one non-zero digit.
    assert(countNumbersWithAtMostTwoOnes(101, 101) == 1);  // "101" has two non-zero digits.
    assert(countNumbersWithAtMostTwoOnes(110, 110) == 1);  // "110" has two non-zero digits.
    assert(countNumbersWithAtMostTwoOnes(123, 123) == 0);  // three non-zero digits → invalid.
    assert(countNumbersWithAtMostTwoOnes(999, 999) == 0);  // three non-zero digits → invalid.

    // Range tests.
    // Numbers 0..9: all 10 have exactly one non-zero digit (or zero for 0). All valid.
    assert(countNumbersWithAtMostTwoOnes(0, 9) == 10);
    // 0..99: all numbers from 0 to 99 have 1 or 2 digits, max two non-zero digits. All 100 valid.
    assert(countNumbersWithAtMostTwoOnes(0, 99) == 100);
    // 0..100: 0..99 (100 numbers) + 100 (1) = 101.
    assert(countNumbersWithAtMostTwoOnes(0, 100) == 101);
    // 0..101: 101 + 1 (because 101 has two non-zero digits, but is it counted? yes) → 102.
    // Actually 0..101 includes 0..100 (101 numbers) + 101 (1) = 102.
    assert(countNumbersWithAtMostTwoOnes(0, 101) == 102);
    // 0..110: 0..99 (100) + 100..110: all have at most two non-zero digits (100,101,102,... 110) – that's 11 numbers. Total 111.
    assert(countNumbersWithAtMostTwoOnes(0, 110) == 111);
    // 0..120: 0..99 (100) + 100..120 (21 numbers) – all have 1 or 2 non-zero digits. Total 121.
    assert(countNumbersWithAtMostTwoOnes(0, 120) == 121);
    // 0..199: 0..99 (100) + 100..199 (100 numbers) – but 199 has three non-zero digits? Actually 199 has three digits (1,9,9) → non-zero count = 3 → invalid.
    // So 100..199: all valid except those with three non-zero digits. From 100 to 199, the hundreds digit is 1, tens and units can be any digit 0-9. A number is invalid if both tens and units are non-zero. There are 9*9 = 81 such invalid (tens 1-9, units 1-9). So valid = 100 - 81 = 19? Wait 100..199 has 100 numbers. Invalid are those with both tens and units non-zero: 1-9,1-9 → 81 invalid. Valid = 19. Total valid = 100 (0..99) + 19 = 119.
    assert(countNumbersWithAtMostTwoOnes(0, 199) == 119);
    // Test a large range: 0..999
    // All 0..99 are valid (100). For 100..999: we have 900 numbers. A number is valid if it has at most two non-zero digits among three positions. Negative case: three non-zero digits. There are 9*9*9 = 729 numbers with all three non-zero. So valid = 900 - 729 = 171. Total = 100 + 171 = 271.
    assert(countNumbersWithAtMostTwoOnes(0, 999) == 271);
    // Check L>0: 100..999 count should be 171.
    assert(countNumbersWithAtMostTwoOnes(100, 999) == 171);
    // Edge: L > R should never happen per spec, but test L == R.
    assert(countNumbersWithAtMostTwoOnes(500, 500) == 0);  // three non-zero digits → invalid.
    // Test very large bounds: 1e18 range up to 1e18. 1e18 is "1000000000000000000" → has one non-zero digit → valid. So countUpTo(1e18) = countUpTo(999999999999999999) + 1? We can simply test that result is positive and less than 1e18+1. For robustness, just assert it is > 0.
    long long maxVal = 1000000000000000000LL;
    long long result = countNumbersWithAtMostTwoOnes(0, maxVal);
    assert(result > 0 && result < maxVal); // Just a sanity check.
    return 0;
}

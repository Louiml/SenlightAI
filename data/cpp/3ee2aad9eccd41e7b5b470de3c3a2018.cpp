// Write a C++ function `int countDivisibleSubsets(const std::string& digits, int d)` that, given a string of decimal digits (each character is a digit `'0'`–`'9'`, length between 1 and 10 inclusive) and a positive integer `d` (1 ≤ d ≤ 100), returns the number of **distinct permutations** of the digits that produce a number divisible by `d`. The count must account for duplicate digits — permutations that are identical due to repeated digits are considered the same. The result fits in a 32-bit signed integer. For example, `countDivisibleSubsets("112", 3)` should return 2, because the distinct permutations are 112, 121, 211, and only 111? Actually permutations: 112,121,211. 112 mod 3 = 1, 121 mod 3 = 1, 211 mod 3 = 1 → 0. Wait check: 112/3=37 R1, 121/3=40 R1, 211/3=70 R1 → none divisible by 3? Actually 111? No digits are 1,1,2. So result 0. But for "12" and d=3, permutations: 12 and 21. 12%3=0, 21%3=0 → both → result 2. The function must be efficient for the given constraints.

#include <cassert>
#include <string>

int countDivisibleSubsets(const std::string& digits, int d); // declaration from solution

int main() {
    // Digits "1" with d=1 → only permutation "1" divisible by 1
    assert(countDivisibleSubsets("1", 1) == 1);
    // Digits "1" with d=2 → "1" not divisible by 2
    assert(countDivisibleSubsets("1", 2) == 0);
    // Digits "12" with d=3 → both "12" and "21" divisible by 3 → 2
    assert(countDivisibleSubsets("12", 3) == 2);
    // Digits "112" with d=1 → all distinct permutations: 112,121,211 → 3
    assert(countDivisibleSubsets("112", 1) == 3);
    // Digits "112" with d=3 → check: 112%3=1,121%3=1,211%3=1 → 0
    assert(countDivisibleSubsets("112", 3) == 0);
    // Digits "0" with d=1 → "0" divisible by 1 → 1
    assert(countDivisibleSubsets("0", 1) == 1);
    // Digits "00" with d=10 → only permutation "00" = 0 divisible by 10 → 1 (distinct permutations only one)
    assert(countDivisibleSubsets("00", 10) == 1);
    // Digits "1234" with d=5 → numbers ending in 0 or 5, none, so 0
    assert(countDivisibleSubsets("1234", 5) == 0);
    // Digits "123" with d=6 → test manually: permutations: 123%6=3,132%6=0,213%6=3,231%6=3,312%6=0,321%6=3 → 2
    assert(countDivisibleSubsets("123", 6) == 2);
    // Digits "222" with d=3 → only "222" divisible by 3 (2+2+2=6) → 1
    assert(countDivisibleSubsets("222", 3) == 1);
    return 0;
}

#include <string>
#include <vector>
#include <numeric>

// Returns the number of distinct permutations of digits in `digits` that form a number divisible by `d`.
int countDivisibleSubsets(const std::string& digits, int d) {
    int n = static_cast<int>(digits.size());
    std::vector<int> digit(n);
    int freq[10] = {0};
    for (int i = 0; i < n; ++i) {
        digit[i] = digits[i] - '0';
        ++freq[digit[i]];
    }

    // Precompute factorials up to n (max 10)
    int fact[11];
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i-1] * i;

    // Denominator for distinct permutations
    int denom = 1;
    for (int v : freq) denom *= fact[v];

    int S = 1 << n;
    // dp[mask][rem] = number of ways to build a prefix using the set of positions `mask`, with remainder `rem`
    std::vector<std::vector<int>> dp(S, std::vector<int>(d, 0));
    dp[0][0] = 1;

    for (int mask = 0; mask < S; ++mask) {
        for (int rem = 0; rem < d; ++rem) {
            int ways = dp[mask][rem];
            if (ways == 0) continue;
            for (int k = 0; k < n; ++k) {
                if (mask & (1 << k)) continue;
                int new_mask = mask | (1 << k);
                int new_rem = (rem * 10 + digit[k]) % d;
                dp[new_mask][new_rem] += ways;
            }
        }
    }

    // Total number of ordered permutations that are divisible by d, divided by the multiset correction factor.
    return dp[S-1][0] / denom;
}

// We need to count the number of unique permutations of the digits (treating identical digits as indistinguishable) that yield a number divisible by `d`. Since the length is at most 10, we can use bitmask dynamic programming over subsets of positions. The state is `(mask, mod)` where `mask` indicates which positions have been used so far, and `mod` is the current remainder modulo `d` of the partially formed prefix. Transition: from state `(mask, mod)`, we try to append a digit at an unused position `k`; the new remainder is `(mod * 10 + digit[k]) % d`, and we set the bit. However, this counts each distinct permutation multiple times because identical digits are swapped. To correct, we divide the final count by the product of factorials of the frequency of each digit (0–9) present in the input. This is a classic technique: the number of distinct permutations of a multiset is `n! / (freq[0]! * freq[1]! * ... * freq[9]!)`, and since DP counts ordered assignments, dividing by that product yields the distinct count. Edge cases: `d` can be 1 (all numbers divisible by 1, so result is the number of distinct permutations), and leading zeros are allowed because we are counting numbers that can start with zero (e.g., "0" is a valid number). The DP has `2^n` masks and `d` remainders; for each state we loop over `n` positions, giving O(2^n * n * d) time. With n≤10, this is at most 1024*10*100 = ~1M operations, fine. Space: O(2^n * d) integers. We must compute factorials up to 10. The division by `w` (product of factorials) is exact integer division; since we compute `dp` as integers, the final count is divisible by `w`, as it counts all permutations. We can either compute `dp` and then divide, or use `long long` internally for safety, but the result fits in `int`.

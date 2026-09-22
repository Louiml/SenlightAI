Write a C++ function `countDigitMultiple` that takes an integer `D` (1 ≤ D ≤ 100) and a string `S` consisting of digits (representing a non-negative integer without leading zeros, except possibly "0" itself). The function must return a `long long` value equal to the number of positive integers from 1 to the integer represented by `S` (inclusive) whose digit sum is divisible by `D`. The result may be large, so compute it modulo 1,000,000,007. For example, if `D = 3` and `S = "20"`, the valid numbers are 3, 6, 9, 12, 15, 18 (also 0 would be excluded) → 6 valid numbers, so the answer is 6. The input string `S` can have up to 10,000 digits, and your solution must be efficient in both time and memory for such large lengths.

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// (The solution function definition from goes here, omitted for brevity in test context.)

int main() {
    // Basic examples
    assert(countDigitMultiple(1, "10") == 10); // numbers 1..10 all have digit sum divisible by 1
    assert(countDigitMultiple(2, "10") == 5);  // 2,4,6,8,10 → 5
    assert(countDigitMultiple(3, "20") == 6);  // 3,6,9,12,15,18 → 6
    assert(countDigitMultiple(5, "5") == 1);   // only 5
    assert(countDigitMultiple(5, "4") == 0);   // none from 1..4

    // Test with a large D and a small number
    assert(countDigitMultiple(100, "100") == 0); // only 0 is divisible by 100, but excluded

    // Test with single digit 0: only zero number, so answer 0
    assert(countDigitMultiple(3, "0") == 0);

    // Test with a longer number and D=10 (digit sum divisible by 10)
    // For "19": numbers with sum multiple of 10 from 1..19: only 19 (sum=10) → 1
    assert(countDigitMultiple(10, "19") == 1);

    // Test consistency: for D=1, answer must equal the numeric value of S
    assert(countDigitMultiple(1, "12345") == 12345LL);

    // Test with leading zeros not allowed but string could be "000"? Disallowed, but we test "0" only.
    // Large D with small S: D=7, S=6 → none divisible, expect 0
    assert(countDigitMultiple(7, "6") == 0);

    // Test with D=2 and S="3": only 2 → 1
    assert(countDigitMultiple(2, "3") == 1);

    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1000000007LL;

// Count positive integers from 1 to number represented by S whose digit sum is divisible by D.
// S is a string of decimal digits (no leading zeros unless "0"). D >= 1.
// Returns answer modulo 1e9+7.
ll countDigitMultiple(int D, const string& S) {
    const int N = (int)S.size();
    // dpSmaller[remainder] and dpEqual[remainder] represent counts for states
    // with smaller flag 1 and 0, respectively, at the current processed position.
    vector<ll> dpSmaller(D, 0), dpEqual(D, 0);
    dpEqual[0] = 1; // initial state: 0 digits, equal, remainder 0

    for (char c : S) {
        int limit = c - '0';
        vector<ll> nextSmaller(D, 0), nextEqual(D, 0);

        // Process transitions from equal state (only possible if still matching prefix exactly)
        for (int k = 0; k < D; ++k) {
            // next digit can be 0..limit
            for (int digit = 0; digit <= limit; ++digit) {
                int newRem = (k + digit) % D;
                if (digit < limit) {
                    nextSmaller[newRem] = (nextSmaller[newRem] + dpEqual[k]) % MOD;
                } else {
                    // digit == limit, so still equal
                    nextEqual[newRem] = (nextEqual[newRem] + dpEqual[k]) % MOD;
                }
            }
        }

        // Process transitions from smaller state (any digit 0..9 allowed)
        for (int k = 0; k < D; ++k) {
            for (int digit = 0; digit <= 9; ++digit) {
                int newRem = (k + digit) % D;
                nextSmaller[newRem] = (nextSmaller[newRem] + dpSmaller[k]) % MOD;
            }
        }

        dpSmaller = move(nextSmaller);
        dpEqual = move(nextEqual);
    }

    // Total valid numbers from 0..N inclusive: equal prefix final state plus all smaller states,
    // harvested at remainder 0.
    ll total = (dpEqual[0] + dpSmaller[0]) % MOD;
    // Exclude zero (the number 0 always has sum 0, divisible by any D, and is counted in total).
    ll ans = (total - 1 + MOD) % MOD;
    return ans;
}

// This is a classic digit DP (dynamic programming) problem. We process the digits of `S` from most significant to least significant, maintaining a state `(position, smaller, remainder)` where:
// - `position` is how many digits have been processed (0 to len(S)).
// - `smaller` is a boolean flag: 1 if the prefix already chosen is strictly smaller than the corresponding prefix of `S`, 0 if it matches so far.
// - `remainder` is the current sum of chosen digits modulo `D`.
//
// We initialize `dp[0][0][0] = 1` (meaning no digits processed, not smaller yet, and sum mod D is 0). At each step, for each state `(s, k)`, we try every possible next digit `next` (from 0 to `s ? 9 : S[pos] - '0'`). If we choose a digit less than the bound, the new smaller flag becomes 1; otherwise it remains `s`. The new remainder is `(k + next) % D`. We accumulate counts modulo MOD. 
//
// After processing all digits, the total valid numbers (including zero) is `dp[N][0][0] + dp[N][1][0]` — the sum of all numbers from 0 to the given integer inclusive whose digit sum mod D equals 0. Since we must exclude zero (the problem asks for positive integers from 1 to N), we subtract 1 from the total and take modulo. Edge cases: if `D` is large, the loop over `next` still runs at most 10 times per state, so complexity is `O(N * 2 * D * 10)` = `O(N * D)` which is fine for N=10,000 and D=100 (≈ 2 million operations). Space is `O(D)` if we use a rolling array over position; we can optimize by keeping only two 2D arrays (current and next layer). The modulo arithmetic must be handled carefully to avoid overflow (use `long long` and reduce after each addition).

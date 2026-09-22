Write a C++ function `countHexNumbers` that takes two integers `n` and `sum` (both non-negative) and returns the number of `n`-digit hexadecimal numbers (using digits `0`-`9` and letters `A`-`F` representing values 0–15) whose digit sum equals `sum`. A “number” must not have leading zeros, so the first digit must be from `1` to `15`. Digits may repeat, and order matters (e.g., `1A` and `A1` are distinct). The count may be very large, so return the answer modulo `1'000'000'007`. The function should handle `n == 0` (in which case the only valid empty number has sum `0`, so return `1` if `sum == 0`, otherwise `0`). Optimize using memoization because the same subproblems occur for many digit choices.
#include <cassert>

// Declaration of the function under test
long long countHexNumbers(int n, int sum);

int main() {
    // n=1: single digit from 1..15, sum must equal that digit
    assert(countHexNumbers(1, 5) == 1);   // digit '5'
    assert(countHexNumbers(1, 0) == 0);   // no leading zero
    assert(countHexNumbers(1, 15) == 1);  // digit 'F'
    assert(countHexNumbers(1, 20) == 0);  // impossible sum

    // n=0: only sum 0 is valid
    assert(countHexNumbers(0, 0) == 1);
    assert(countHexNumbers(0, 1) == 0);

    // n=2, sum=1: first digit must be 1, second must be 0 → only "10"
    assert(countHexNumbers(2, 1) == 1);

    // n=2, sum=2: first digit 1, second 1; or first digit 2, second 0 → "11", "20"
    assert(countHexNumbers(2, 2) == 2);

    // n=2, sum=3: possibilities: 12,21,30 → three numbers
    assert(countHexNumbers(2, 3) == 3);

    // n=3, sum=1: only 100 → one number
    assert(countHexNumbers(3, 1) == 1);

    // large but feasible: n=3, sum=45 (max is 15*3=45) → only one number "FFF"
    assert(countHexNumbers(3, 45) == 1);

    // edge: sum too high -> zero
    assert(countHexNumbers(3, 46) == 0);

    // known value: n=2, sum=15: count = sum_{d=1..15} if (15-d)<=15 → all d from 1..15 have (15-d) in 0..14, so 15 ways
    assert(countHexNumbers(2, 15) == 15);

    // modulo check: large n, but keep small enough to be quick; n=10, sum=80 (max 150) — still fast, compare with known recurrence? Use rough count: any leading digit 1-15, rest 0-15. Just ensure it doesn't crash and returns non-negative.
    long long result = countHexNumbers(10, 80);
    assert(result >= 0 && result < MOD);

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

// Memoization cache: key = (digits_left, sum_left, is_first_digit)
map<tuple<int, int, bool>, long long> memo;

long long solveHex(int digits_left, int sum_left, bool is_first_digit) {
    // Base case: no more digits to place
    if (digits_left == 0) {
        return (sum_left == 0) ? 1 : 0;
    }

    // Check memo
    auto key = make_tuple(digits_left, sum_left, is_first_digit);
    if (memo.count(key)) {
        return memo[key];
    }

    long long total = 0;
    // First digit cannot be 0; other digits can be 0 through 15
    int start_digit = is_first_digit ? 1 : 0;
    for (int d = start_digit; d <= 15; ++d) {
        if (d <= sum_left) {
            total = (total + solveHex(digits_left - 1, sum_left - d, false)) % MOD;
        }
    }

    memo[key] = total;
    return total;
}

// Public function: count n-digit hex numbers with exact digit sum
long long countHexNumbers(int n, int sum) {
    memo.clear();
    return solveHex(n, sum, true);
}
// This is a counting problem that can be solved by dynamic programming with memoization. Define a recursive function `f(pos, remainingSum, first)` where `pos` is the number of digits still to place, `remainingSum` is the sum still needed, and `first` is a boolean indicating whether we are placing the very first (most significant) digit. For the first digit, the allowed values are `1` to `15`; for all other positions, the allowed values are `0` to `15`. For each allowed digit `d` not exceeding `remainingSum`, we add the result of `f(pos-1, remainingSum-d, false)`. The base case is `pos == 0`: return `1` if `remainingSum == 0`, else `0`. Use a memoization table keyed by `(pos, remainingSum, first)`. Since `n` may be up to, say, 100 and `sum` up to `15*n`, the number of states is `O(n * sum * 2)`. Each state loops over at most 16 digits, so total time is `O(n * sum * 16)` and space `O(n * sum)`. Edge cases include `n == 0` (handled directly), `sum == 0` with `n > 0` (no valid number because first digit must be at least 1), and cases where `sum > 15 * n` (return 0; the recursion will naturally handle this since no digit choices fit). The modulo arithmetic ensures the result stays within integer bounds.

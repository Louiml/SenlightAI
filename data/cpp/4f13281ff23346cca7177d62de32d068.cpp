Given a positive integer `n` and a vector of distinct digits `arr` (each between 0 and 9 inclusive) that are allowed to appear in a number, write a C++ function that counts how many `n`-digit positive integers can be formed using only digits from `arr`. Leading zeros are not allowed (so the first digit cannot be zero). The returned count should be a non-negative integer that fits in a 32-bit signed integer for all valid inputs (you may assume `1 <= n <= 7` and `1 <= arr.size() <= 10`). For example, if `n = 2` and `arr = {1, 3, 5}`, the valid two-digit numbers are 11, 13, 15, 31, 33, 35, 51, 53, 55 — a total of 9. If `arr` contains 0, like `arr = {0, 2}`, then for `n = 2` the valid numbers are 20, 22 (but not 02 or 00), so the count is 2.

The problem asks for the count of `n`-digit numbers (first digit 1-9) where every digit is chosen from the allowed set `arr`. A direct combinatorial approach works: For the first digit, there are `k` choices if 0 is not in `arr` (since any allowed digit except 0 can be the first digit) or `k-1` choices if 0 is in `arr` (because 0 cannot be the first digit). For each of the remaining `n-1` positions, there are `k` choices (including 0 if present). Thus the total count is `(firstChoices) * (k^(n-1))`. Alternatively, a complementary counting approach: total `n`-digit numbers using any of the 10 digits is `9 * 10^(n-1)` (first digit 1-9, others 0-9). The number of `n`-digit numbers that include at least one digit *not* in `arr` (i.e., using only digits from the complement set) is `(10-k)^n` minus the count of those with leading zero if 0 is in the complement. This is exactly what the provided snippet does. However, for clarity and correctness, the direct product approach is simpler and avoids off-by-one errors. Edge cases: if `arr` has only 0 (i.e., `arr = {0}`), then no `n`-digit number can be formed because the first digit cannot be 0, so the count is 0. If `arr` has all 10 digits, then the count is all `n`-digit numbers: `9*10^(n-1)`. Time complexity is O(n) due to power computation (or O(1) if we use fast exponentiation), and space complexity is O(1). We must handle integer overflow by using `long long` internally and then cast to `int` (since the problem guarantees the result fits in 32-bit).

#include <vector>
#include <algorithm>

// Counts the number of n-digit positive integers (no leading zero) that can be
// formed using only the digits present in the allowed_digits vector.
// allowed_digits must contain distinct digits in [0,9]. n >= 1.
// Returns an int as the result is guaranteed to fit in 32-bit signed int.
int countValidNumbers(int n, const std::vector<int>& allowed_digits) {
    int k = static_cast<int>(allowed_digits.size());
    bool contains_zero = std::find(allowed_digits.begin(), allowed_digits.end(), 0) != allowed_digits.end();

    // Number of choices for the first digit: allowed digits except 0.
    long long first_choices = (contains_zero ? k - 1 : k);
    if (first_choices <= 0) return 0; // no allowed non-zero digit

    // Each of the remaining n-1 positions can be any allowed digit (including 0).
    long long result = first_choices;
    for (int i = 0; i < n - 1; ++i) {
        result *= k;
    }
    return static_cast<int>(result);
}

#include <cassert>
#include <vector>

int main() {
    // Basic example: n=2, digits {1,3,5} -> 9 numbers
    assert(countValidNumbers(2, {1, 3, 5}) == 9);

    // With zero in allowed digits: n=2, digits {0,2} -> 20,22 only (2 numbers)
    assert(countValidNumbers(2, {0, 2}) == 2);

    // Only zero allowed: n=3, digits {0} -> no valid number
    assert(countValidNumbers(3, {0}) == 0);

    // All digits allowed: n=1 -> 9 numbers (1-9)
    assert(countValidNumbers(1, {0,1,2,3,4,5,6,7,8,9}) == 9);

    // n=3, digits {9} -> only 999 (1 number)
    assert(countValidNumbers(3, {9}) == 1);

    // n=2, digits {5,6,7,8,9} -> first digit 5 options, second 5 options -> 25
    assert(countValidNumbers(2, {5,6,7,8,9}) == 25);

    // n=1, digits {0,1,2} -> only 1 and 2 (2 numbers)
    assert(countValidNumbers(1, {0,1,2}) == 2);

    // Large case: n=7, digits {1,2} -> first digit 2 options, each of 6 remaining 2 options -> 2^7=128
    assert(countValidNumbers(7, {1,2}) == 128);

    // n=7, digits {0,1} -> first digit only 1, remaining 6 positions each 2 options -> 2^6=64
    assert(countValidNumbers(7, {0,1}) == 64);

    // n=2, digits {0} (only zero) -> 0
    assert(countValidNumbers(2, {0}) == 0);

    return 0;
}

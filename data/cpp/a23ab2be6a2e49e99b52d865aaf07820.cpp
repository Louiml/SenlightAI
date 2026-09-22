// Write a C++ function that takes an integer `n` and returns the "best possible" negative number obtainable from its absolute value by deleting exactly one digit, following this rule: if `n` is non-negative, return it unchanged; if `n` is negative, remove exactly one digit from its absolute value (i.e., from the unsigned number formed by ignoring the minus sign) such that the resulting number, when multiplied by -1, is **as large as possible** (i.e., the least negative / closest to zero). If the absolute value has only one digit, then after deletion zero digits remain, so the result is `0` (since n is negative, return 0). You must handle the case where the absolute value may have leading zeros after deletion? No—by standard integer representation, no leading zeros; if the absolute value is like `100`, deleting the `1` yields `00` which becomes `0`; deleting a `0` yields `10`; choose the largest resulting absolute value to minimize negativity. Return the final negative integer (or 0) as an `int`. For example, `-123` → absolute value 123 → delete `1` → 23 → -23; delete `2` → 13 → -13; delete `3` → 12 → -12; best is -12 (largest). For `-10`, abs = 10 → delete `1` → 0 → 0; delete `0` → 1 → -1; best is 0 (largest). For `-100`, abs = 100 → delete `1` → 00 → 0; delete `0` → 10 → -10; delete other 0 → 10 → -10; best is 0. For `-5`, abs has one digit → result 0.
The algorithm first checks if the input `n` is non-negative; if so, return `n` as is. For negative input, take the absolute value (as a positive integer). If this absolute value is less than 10 (i.e., has only one digit), then after deleting the only digit, no digits remain, so the number becomes 0, and because we need a negative or zero result, return 0. Otherwise, we generate every possible number formed by removing exactly one digit from the decimal representation of the absolute value. To do this without string conversion, we can use arithmetic: for a number `x` with `d` digits, removing the digit at position `i` (0-indexed from the right, 0 = units place) gives `left * 10^i + right`, where `left = x / (10^(i+1))` and `right = x % (10^i)`. We iterate over all possible positions from 0 to digit count-1, compute each candidate, and track the maximum candidate (since a larger absolute value, when negated, is closer to zero). After finding the maximum candidate, return `-maxCandidate`. Edge cases include when the absolute value is like 100, where removing a digit can result in 0; we still compare and choose the largest. Also, when the absolute value is 0 (but input negative so absolute value >0) — not possible. The time complexity is O(d^2) if counting digit extraction with repeated divisions, but we can compute digit count in O(d) and each candidate in O(1) using precomputed powers, so overall O(d) time where d is the number of digits, and O(1) extra space. Simpler: since d ≤ 9 for a 32-bit int (max abs ~2.1e9 has 10 digits), a straightforward loop with string conversion is also acceptable, but the arithmetic approach is more efficient and self-contained. We'll implement arithmetic to avoid unnecessary includes.
#include <cmath>   // for pow? Not needed; use integer loops
#include <algorithm> // for std::max

// Returns the best possible number after removing one digit from the absolute value.
// If n is non-negative, returns n unchanged. If n is negative, returns the largest negative
// number (closest to zero) obtainable by deleting exactly one digit from |n|.
// If |n| has a single digit, returns 0.
int bestAfterDigitDeletion(int n) {
    if (n >= 0) {
        return n;
    }
    long long absVal = -static_cast<long long>(n); // use long long to avoid overflow on INT_MIN
    if (absVal < 10) {
        return 0; // only one digit, deleting it leaves nothing -> 0
    }

    // Count digits
    int digits = 0;
    long long temp = absVal;
    while (temp > 0) {
        ++digits;
        temp /= 10;
    }

    long long best = -1; // candidate absolute values are non-negative, start below 0

    // Precompute powers of 10 up to digits
    long long power[11]; // enough for 32-bit int (max 10 digits)
    power[0] = 1;
    for (int i = 1; i <= digits; ++i) {
        power[i] = power[i-1] * 10;
    }

    // Try removing each digit position (0 = units, digits-1 = most significant)
    for (int i = 0; i < digits; ++i) {
        long long left = absVal / power[i+1]; // digits to the left of removed digit
        long long right = absVal % power[i];  // digits to the right (lower significance)
        long long candidate = left * power[i] + right;
        if (candidate > best) {
            best = candidate;
        }
    }

    return -static_cast<int>(best);
}
#include <cassert>

int main() {
    // Non-negative numbers unchanged
    assert(bestAfterDigitDeletion(0) == 0);
    assert(bestAfterDigitDeletion(123) == 123);
    assert(bestAfterDigitDeletion(1000) == 1000);

    // Negative numbers with multiple digits
    assert(bestAfterDigitDeletion(-123) == -12);   // remove 3 -> 12
    assert(bestAfterDigitDeletion(-10) == 0);      // remove 1 -> 0, remove 0 -> -1 -> best is 0
    assert(bestAfterDigitDeletion(-100) == 0);     // remove 1 -> 0, remove 0 -> 10 -> -10, best is 0
    assert(bestAfterDigitDeletion(-99) == -9);     // abs=99: remove 9 (any) -> 9
    assert(bestAfterDigitDeletion(-19) == -1);     // abs=19: remove 9 -> 1, remove 1 -> 9 -> best is -1
    assert(bestAfterDigitDeletion(-501) == -51);   // abs=501: remove 5 -> 1, remove 0 -> 51, remove 1 -> 50 -> best is 51
    assert(bestAfterDigitDeletion(-1000) == -100); // abs=1000: remove leading 1 -> 0, remove any 0 -> 100 -> best is -100

    // Single-digit negative numbers -> 0
    assert(bestAfterDigitDeletion(-5) == 0);
    assert(bestAfterDigitDeletion(-9) == 0);

    // Edge case: INT_MIN (absolute value 2147483648, which overflows int, but function uses long long)
    assert(bestAfterDigitDeletion(-2147483647) == -214748364); // remove last digit 7 -> 214748364

    return 0;
}

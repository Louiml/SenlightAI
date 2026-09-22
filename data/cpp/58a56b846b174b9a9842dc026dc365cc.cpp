/*
Write a C++ function that, given two integers `s` (the sum of digits) and `d` (the number of digits), returns the smallest possible positive integer with exactly `d` digits whose digit sum equals `s`. If no such number exists (e.g., because the required sum exceeds the maximum possible digit sum for `d` digits, which is `9*d`, or because `s` is 0 or negative, or `d` is not positive), return the string `"-1"`. The returned number must not have leading zeros, except when `d == 1` and `s == 0` (in which case the number `"0"` is valid). The function should be efficient and handle edge cases properly.
*/
#include <string>
#include <vector>
#include <algorithm>

// Returns the smallest positive integer with exactly 'd' digits whose digit sum equals 's'.
// Returns "-1" if no such number exists.
std::string smallestNumberWithDigitSum(int s, int d) {
    // Validate input: d must be positive, s non-negative, and s within max possible.
    if (d <= 0 || s < 0 || s > 9 * d) {
        return "-1";
    }

    // Special case: single digit with sum 0 is the number "0".
    if (d == 1 && s == 0) {
        return "0";
    }

    // General case: we need the first digit to be at least 1.
    if (s == 0) {
        return "-1"; // No positive number with more than 1 digit can have sum 0.
    }

    // Build digits from most significant to least, placing smallest possible at each step.
    std::string result(d, '0');
    int remaining = s;

    // For the first digit, at least 1, and as small as possible.
    // We try from 1 upward, ensuring the rest can still be filled with max 9.
    for (int firstDigit = 1; firstDigit <= 9; ++firstDigit) {
        int restMax = 9 * (d - 1);
        if (remaining - firstDigit <= restMax && remaining - firstDigit >= 0) {
            result[0] = static_cast<char>('0' + firstDigit);
            remaining -= firstDigit;
            break;
        }
    }

    // Fill the rest from the last digit backward, putting as much as possible.
    for (int pos = d - 1; pos >= 1 && remaining > 0; --pos) {
        int add = std::min(9, remaining);
        result[pos] = static_cast<char>('0' + add);
        remaining -= add;
    }

    // If we couldn't exactly use remaining sum, it's impossible (shouldn't happen after check).
    if (remaining != 0) {
        return "-1";
    }

    return result;
}
#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(smallestNumberWithDigitSum(9, 2) == "18"); // smallest 2-digit with sum 9
    assert(smallestNumberWithDigitSum(10, 2) == "19");
    assert(smallestNumberWithDigitSum(1, 1) == "1");
    assert(smallestNumberWithDigitSum(0, 1) == "0");

    // Edge cases: impossibilities
    assert(smallestNumberWithDigitSum(19, 2) == "-1"); // max sum is 18
    assert(smallestNumberWithDigitSum(0, 3) == "-1"); // leading zero not allowed
    assert(smallestNumberWithDigitSum(-5, 3) == "-1"); // negative sum
    assert(smallestNumberWithDigitSum(10, 0) == "-1"); // zero digits

    // Larger d
    assert(smallestNumberWithDigitSum(1, 3) == "100");
    assert(smallestNumberWithDigitSum(27, 3) == "999");
    assert(smallestNumberWithDigitSum(5, 5) == "10004"); // first digit=1, last gets 4

    // Boundary: max sum
    assert(smallestNumberWithDigitSum(18, 2) == "99");
    assert(smallestNumberWithDigitSum(9*4, 4) == "9999");

    return 0;
}
// The optimal strategy is greedy: we want the smallest number with exactly `d` digits and digit sum `s`. To minimize the number, we should place the smallest possible digit in the most significant position (leftmost), and then fill the remaining positions from right to left with the largest possible digits (up to 9) to consume the remaining sum quickly, while ensuring each position gets at least the minimal digit (0 for all except the first, which must be at least 1 to avoid leading zero). The original code works by initially setting the first digit to 1 and subtracting 1 from the required sum, then iterating from the last digit backward, adding as much as possible (up to 9 minus the current digit) to each position. Edge cases: if `s` is 0 and `d` > 1, no valid number (since the first digit must be at least 1), return "-1". If `s` > `9*d`, return "-1". If `d == 1` and `s == 0`, the number is "0". The algorithm runs in O(d) time and O(d) space for the digit vector.

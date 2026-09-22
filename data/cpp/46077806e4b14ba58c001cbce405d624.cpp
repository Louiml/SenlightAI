/*
Write a C++ function `int countSignificantDigits(const std::string& number)` that takes a string representing a decimal number (which may include a leading sign, a decimal point, and digits, but no exponents) and returns the number of significant digits according to standard rules: leading zeros are never significant; trailing zeros after a decimal point are significant; zeros between significant digits are significant; for numbers without a decimal point, trailing zeros are not significant. The input string may contain a leading '+' or '-' sign (ignored for counting), may have leading/trailing spaces (trim them), and may have a decimal point. The function must handle edge cases like "0", "0.0", "000.1200", "-0.00450", "100", "100.", and ".5".
*/

#include <string>
#include <algorithm>
#include <cctype>

// Count significant digits in a decimal number string (no exponent).
int countSignificantDigits(const std::string& input) {
    // Trim leading and trailing whitespace
    std::string s = input;
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return 0; // empty
    size_t last = s.find_last_not_of(" \t\n\r");
    s = s.substr(first, last - first + 1);

    // Remove optional leading sign
    if (!s.empty() && (s[0] == '+' || s[0] == '-')) {
        s.erase(0, 1);
    }

    bool hasDecimal = (s.find('.') != std::string::npos);
    int count = 0;
    bool seenNonZero = false;

    for (char c : s) {
        if (c == '.') continue;
        if (!isdigit(static_cast<unsigned char>(c))) continue; // ignore any unexpected chars

        if (c != '0') {
            seenNonZero = true;
            ++count;
        } else {
            // Zero digit
            if (seenNonZero) {
                ++count; // zero after a significant digit is significant unless it's trailing integer zero
            }
            // If no decimal point, trailing zeros will be handled by not counting beyond last non-zero
        }
    }

    // If no non-zero digit found, the number is zero -> has one significant digit
    if (!seenNonZero) return 1;

    if (!hasDecimal) {
        // Remove trailing zeros (they are not significant without decimal point)
        // We can adjust by counting from the last non-zero position
        int lastNonZero = -1;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] != '0' && s[i] != '.') lastNonZero = i;
        }
        // Count digits from first non-zero to last non-zero, excluding dots
        int countAdjusted = 0;
        bool started = false;
        for (int i = 0; i <= lastNonZero; ++i) {
            if (s[i] == '.') continue;
            if (s[i] != '0' || started) {
                started = true;
                if (s[i] != '.') ++countAdjusted;
            }
        }
        return countAdjusted == 0 ? 1 : countAdjusted;
    }

    // With decimal point: trailing zeros after decimal are significant, count already correct
    return count;
}

#include <cassert>
#include <string>

// Declaration from solution
int countSignificantDigits(const std::string& input);

int main() {
    // Standard cases
    assert(countSignificantDigits("123") == 3);
    assert(countSignificantDigits("00123") == 3);
    assert(countSignificantDigits("0.00123") == 3);
    assert(countSignificantDigits("123.4500") == 6);
    assert(countSignificantDigits("100") == 1);
    assert(countSignificantDigits("100.") == 3);
    assert(countSignificantDigits("0.000") == 1);
    assert(countSignificantDigits("0") == 1);
    assert(countSignificantDigits("-0.00450") == 3);
    assert(countSignificantDigits("  +120.00  ") == 5);
}

// The solution first trims all whitespace from the input and removes an optional leading '+' or '-' sign (not part of the numeric value for digit counting). Then, we determine whether a decimal point exists. If the decimal point exists, we count all digits from the first non-zero digit (or the first digit after a leading zero run) through the end of the string (excluding the decimal point). This counts leading zeros correctly (they are skipped until the first non-zero digit), and it counts trailing zeros after the decimal point because they appear to the right of the decimal point and are included in the scan. If no decimal point exists, we count digits from the first non-zero digit up to the last non-zero digit (excluding any trailing zeros). Edge cases: if the number is zero (all digits are '0'), the count is 1 because zero itself has one significant digit. For strings like "0.0", the first non-zero digit never appears, so we treat the whole number as zero and return 1. For strings like ".5", the leading decimal point is handled by scanning and counting from the first non-zero digit (which is '5'). Time complexity is O(n) where n is the string length (after trimming), and space complexity is O(1) aside from a copy of the string for trimming.

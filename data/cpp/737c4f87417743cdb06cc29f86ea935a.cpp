Write a standalone C++ function named `stringToInteger` that converts a given C++ string into a 32-bit signed integer, following the behavior of the classic `atoi` function but with explicit overflow handling. The function must skip leading whitespace, handle an optional leading `+` or `-` sign, then parse consecutive digit characters until a non-digit is encountered. If the resulting value (ignoring sign) exceeds the range of a 32-bit signed integer (`INT_MAX` = 2147483647 or `INT_MIN` = -2147483648), the function must return `INT_MAX` for positive overflow or `INT_MIN` for negative overflow. If no valid digits are found, return 0. The input may contain any characters, including letters, spaces, and symbols. The function should be declared with `const` correctness, taking the input string by `const std::string&`, and must not rely on built-in conversion functions like `std::stoi` or `std::atoi`. Provide an implementation that uses `long` to detect overflow but also limits the digit count to avoid undefined behavior with extreme inputs.
#include <cassert>
#include <string>
#include <climits>

int stringToInteger(const std::string& s); // declaration from solution

int main() {
    // Basic cases
    assert(stringToInteger("42") == 42);
    assert(stringToInteger("   -42") == -42);
    assert(stringToInteger("4193 with words") == 4193);

    // Overflow cases
    assert(stringToInteger("2147483648") == INT_MAX);
    assert(stringToInteger("-2147483649") == INT_MIN);
    assert(stringToInteger("9999999999999") == INT_MAX);
    assert(stringToInteger("-9999999999999") == INT_MIN);

    // No digits or invalid
    assert(stringToInteger("words") == 0);
    assert(stringToInteger("") == 0);
    assert(stringToInteger("   +   ") == 0);

    // Sign handling and leading zeros
    assert(stringToInteger("+5") == 5);
    assert(stringToInteger("00012") == 12);

    // Mixed characters after digits
    assert(stringToInteger("123abc") == 123);
    assert(stringToInteger("  -0012a42") == -12);

    // Exact boundary
    assert(stringToInteger("-2147483648") == INT_MIN);
    assert(stringToInteger("2147483647") == INT_MAX);

    return 0;
}
#include <string>
#include <climits>
#include <cctype>

// Convert a string to a 32-bit signed integer, mimicking atoi with overflow handling.
// Skips leading whitespace, accepts an optional sign, parses digits until a non-digit,
// and returns INT_MAX or INT_MIN on overflow, or 0 if no digits are present.
int stringToInteger(const std::string& s) {
    int idx = 0;
    long result = 0;
    int sign = 1;
    int digitCount = 0;

    // Skip leading spaces.
    while (idx < static_cast<int>(s.size()) && s[idx] == ' ') {
        ++idx;
    }

    // Handle optional sign.
    if (idx < static_cast<int>(s.size()) && s[idx] == '+') {
        ++idx;
    } else if (idx < static_cast<int>(s.size()) && s[idx] == '-') {
        sign = -1;
        ++idx;
    }

    // Parse digits.
    while (idx < static_cast<int>(s.size()) && std::isdigit(static_cast<unsigned char>(s[idx]))) {
        result = result * 10 + (s[idx] - '0');
        ++idx;
        ++digitCount;
    }

    // Overflow detection: more than 10 digits or result exceeds INT_MAX.
    if (digitCount > 10 || result > static_cast<long>(INT_MAX)) {
        return (sign == 1) ? INT_MAX : INT_MIN;
    }

    // Apply sign and return.
    return static_cast<int>(sign * result);
}
// The algorithm processes the string in a single left-to-right pass. First, skip all leading space characters (`' '`) by advancing an index. Next, examine the current character: if it is `'+'`, advance and set the sign to positive; if it is `'-'`, advance and set the sign to negative; otherwise, the sign defaults to positive. Then, iterate while the current index is within bounds and the character is a digit (using `std::isdigit` after casting to `unsigned char`). For each digit, multiply the accumulated result by 10 and add the digit value (e.g., `s[idx] - '0'`). After each digit, increment a count. As soon as a non-digit is encountered (or the string ends), stop parsing. After parsing, check for overflow: if the count of digits exceeds 10 or the accumulated `long` value exceeds `INT_MAX` (2147483647), then return `INT_MAX` when sign is positive or `INT_MIN` when sign is negative. Otherwise, return the sign multiplied by the accumulated value. Important edge cases include: empty string, only whitespace, only sign without digits, leading zeros (e.g., `"00001"`), input with more than 10 digits causing overflow, and mixed characters like `"123abc"` where parsing stops at the first non-digit. Time complexity is O(n) where n is the length of the input string, and space complexity is O(1) since only a few integer variables are used.

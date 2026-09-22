// Given a string that represents a single integer (possibly with a leading plus or minus sign and optional leading/trailing whitespace), write a C++ function that converts the string to its numeric value using only arithmetic operations and character digit checks, without using standard library conversion functions like `stoi`, `atoi`, or `istringstream`. The input string is guaranteed to be a valid integer in the range of a 64-bit signed integer (from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807). The function must handle the negative edge case of the minimum 64-bit value where the absolute value exceeds the maximum positive value. Return the parsed `long long` value.
// The core idea is to iterate through the string, ignoring leading whitespace, then read an optional sign (`+` or `-`), then accumulate digits one by one. For each digit character `c`, convert it to its numeric value by subtracting `'0'`, and build the result as `result = result * 10 + digit`. Since we need to handle the minimum 64-bit value (`-9223372036854775808`), we cannot simply negate the absolute value because that would overflow. Instead, we accumulate the absolute value into an unsigned 64-bit integer or use a negative accumulation strategy. A safe approach is to accumulate the magnitude into a `long long` but track negativity separately and check for overflow before multiplying by 10 and adding a digit. Alternatively, use an unsigned 64-bit accumulator and at the end, if negative sign was present and the accumulated value equals 2^63, return `LLONG_MIN`, else return negative of the accumulated value. For positive numbers, the accumulator must not exceed `LLONG_MAX`. The complexity is O(n) time and O(1) auxiliary space, where n is the string length. Edge cases: empty string (not given, but handle gracefully), whitespace-only, plus/minus sign without digits (invalid but handle), and exactly at the boundaries of 64-bit range.
#include <cstdint>
#include <string>
#include <limits>

// Parse a decimal integer string (possibly with leading/trailing whitespace and a sign)
// into a 64-bit signed integer without using standard library conversion functions.
// Assumes the input is a valid integer within the 64-bit signed range.
long long parseInteger(const std::string& input) {
    size_t i = 0;
    const size_t n = input.size();

    // Skip leading whitespace
    while (i < n && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\r' || input[i] == '\f' || input[i] == '\v')) {
        ++i;
    }

    // Handle sign
    bool negative = false;
    if (i < n && (input[i] == '+' || input[i] == '-')) {
        negative = (input[i] == '-');
        ++i;
    }

    // Accumulate digits into an unsigned 64-bit value to safely handle LLONG_MIN
    uint64_t magnitude = 0;
    const uint64_t limit = static_cast<uint64_t>(std::numeric_limits<long long>::max());

    while (i < n && input[i] >= '0' && input[i] <= '9') {
        uint64_t digit = static_cast<uint64_t>((input[i] - '0'));
        // Check for overflow before multiplication and addition
        if (magnitude > (limit - digit) / 10) {
            // If we overflow, only valid scenario is negative and magnitude exactly becomes 2^63
            if (negative && magnitude == (limit + 1 - digit) / 10 && digit == 8 && (limit - digit) / 10 == magnitude - 1) {
                // This check is messy; simpler: use a separate approach below.
                // For clarity, we will just allow overflow for negative LLONG_MIN, but in practice
                // we must handle it carefully. A cleaner way is to track using long long with negative accumulation.
                // Let's implement a robust version using long long with negative accumulation.
                break;
            }
            // For simplicity, we revert to a safe long long method below.
            break;
        }
        magnitude = magnitude * 10 + digit;
        ++i;
    }

    // The above has edge issues; instead, we implement a robust version:
    // Use long long and accumulate as negative to avoid overflow for LLONG_MIN.
    // We re-implement cleanly here.
    long long result = 0;
    i = 0;
    // Skip leading whitespace
    while (i < n && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n' || input[i] == '\r' || input[i] == '\f' || input[i] == '\v')) {
        ++i;
    }
    bool neg = false;
    if (i < n && (input[i] == '+' || input[i] == '-')) {
        neg = (input[i] == '-');
        ++i;
    }
    // Accumulate negative to handle |LLONG_MIN| safely
    while (i < n && input[i] >= '0' && input[i] <= '9') {
        int digit = input[i] - '0';
        // Check overflow in negative direction: if result < (LLONG_MIN + digit) / 10
        if (result < (std::numeric_limits<long long>::min() + digit) / 10) {
            // overflow occurs, but input is guaranteed valid, so we can break; for safety throw or assert
            break;
        }
        result = result * 10 - digit;
        ++i;
    }
    if (!neg) {
        // If not negative, we have accumulated negative; if result == LLONG_MIN it's overflow, but input valid so not case.
        // Convert to positive: -result, but careful if result == LLONG_MIN (should not happen for positive)
        return -result;
    }
    return result;
}
#include <cassert>
#include <string>
#include <limits>

int main() {
    assert(parseInteger("123") == 123);
    assert(parseInteger("-123") == -123);
    assert(parseInteger("+42") == 42);
    assert(parseInteger("  0  ") == 0);
    assert(parseInteger("-0") == 0);
    assert(parseInteger("  -7 ") == -7);
    assert(parseInteger("9223372036854775807") == std::numeric_limits<long long>::max());
    assert(parseInteger("-9223372036854775808") == std::numeric_limits<long long>::min());
    assert(parseInteger("000123") == 123);
    assert(parseInteger("  +0001  ") == 1);
    return 0;
}

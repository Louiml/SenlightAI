/*
Write a C++ function that takes a string containing a single non-negative integer (with no leading zeros, except the number zero itself) and returns the factorial of that integer as a `long long` value. The input string must be validated: it may only contain digits (0-9), must have length at least 1, and must not be empty. If the input is invalid (contains non-digit characters, is empty, or has leading zeros for a multi-digit number), the function should return -1. The function must handle all values where the factorial fits within a 64-bit signed integer; for inputs whose factorial would overflow `long long`, return -1 as well. Do not use any external libraries beyond the C++ standard library.
*/

#include <string>
#include <climits>
#include <cctype>

// Return the factorial of a non-negative integer represented as a string.
// Returns -1 if the input is invalid or if the factorial overflows long long.
long long factorialFromString(const std::string& input) {
    // Validate: non-empty, all digits, no leading zeros (except "0" itself)
    if (input.empty()) {
        return -1;
    }
    if (input.size() > 1 && input[0] == '0') {
        return -1;
    }
    for (char c : input) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return -1;
        }
    }

    // Convert to integer, catching overflow
    long long n;
    try {
        n = std::stoll(input);
    } catch (const std::out_of_range&) {
        return -1;
    }

    // Factorial of 0 and 1 is 1; for larger n compute with overflow check
    long long result = 1;
    for (long long i = 2; i <= n; ++i) {
        if (result > LLONG_MAX / i) {
            return -1; // would overflow
        }
        result *= i;
    }
    return result;
}

#include <cassert>
#include <string>

// Declare the function (since it's in another translation unit, we re-declare here)
long long factorialFromString(const std::string& input);

int main() {
    // Basic valid cases
    assert(factorialFromString("0") == 1);
    assert(factorialFromString("1") == 1);
    assert(factorialFromString("5") == 120);
    assert(factorialFromString("10") == 3628800);

    // Invalid inputs
    assert(factorialFromString("") == -1);          // empty
    assert(factorialFromString("01") == -1);        // leading zero
    assert(factorialFromString("12a") == -1);       // non-digit
    assert(factorialFromString("-3") == -1);        // negative sign

    // Overflow cases (21! > LLONG_MAX)
    assert(factorialFromString("20") == 2432902008176640000LL); // still fits
    assert(factorialFromString("21") == -1);        // overflow
    assert(factorialFromString("100") == -1);       // definitely overflow

    // Large but valid string that fits in long long (but factorial overflows)
    assert(factorialFromString("999999999999999999") == -1); // conversion OK but factorial overflows
    return 0;
}

// The solution must first validate the input string character by character. We check that the string is not empty, that every character is a digit, and that there are no leading zeros unless the string is exactly "0". If any of these checks fail, return -1. We then convert the string to an integer using `std::stoll` (or manual conversion) inside a try-catch block to handle potential overflow during conversion. For the factorial computation, we multiply sequentially from 1 to n. Before each multiplication, we check whether the current result would exceed `LLONG_MAX / i`; if so, we return -1 immediately. This prevents overflow in the multiplication. Edge cases include: input "0" returns 1, invalid strings like "" or "01" or "12a" return -1, and large inputs like "21" (21! = 51090942171709440000 which exceeds 64-bit) return -1. Time complexity is O(n) for the factorial loop, and O(k) for input validation and conversion where k is the string length. Space complexity is O(1) auxiliary.

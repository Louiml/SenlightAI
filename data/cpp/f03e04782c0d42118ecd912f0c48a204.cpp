Write a C++ function named `scientificNotation` that takes a string `s` representing a non-negative decimal number (which may or may not contain a decimal point, leading zeros, trailing zeros, or zero leading digits like "0.00123") and returns a string representing that number normalized in scientific notation. The output must be of the form: a single non-zero digit, optionally followed by a decimal point and one or more digits (if there are more significant digits), then the letter `E`, then the base-10 exponent (an integer, possibly negative). If the number is an integer (no fractional part after normalization), output just the single digit without the `E` part. The input is guaranteed to contain at most one decimal point and at least one digit. Leading zeros before the decimal point and trailing zeros after the decimal point should be removed before constructing the scientific notation. For example, "0.00123" should become "1.23E-3", "123.45" should become "1.2345E2", "100" (which becomes "100.") should become "1E2", and "0.5" should become "5E-1". Note the exponent is the power of 10 that the mantissa (the part before `E`) must be multiplied by to get the original number.
// The function first normalizes the input string. We ensure there is a decimal point by appending one if absent. Then, we remove all leading zeros (but not the last digit if all zeros) and all trailing zeros (both before and after the decimal point), being careful not to remove the decimal point itself. After removing leading zeros, if the string becomes empty or just ".", it means the number is zero; however, the problem guarantees non-negative and at least one digit, so we can assume nonzero (but handle zero gracefully by returning "0" if needed, though not required). Then, find the position of the decimal point. Cases:
//
// - If decimal point is at position 0 (i.e., the number is like ".xxxx" after removing leading zeros), then the number is less than 1. Count the zeros after the decimal point until the first non-zero digit. The exponent `pw` is negative and equals `-(number of zeros + 1)`. Then take the substring starting from that first non-zero digit, remove the decimal point for the mantissa. If the mantissa has length 1, output just that digit and `E pw`; otherwise, output first digit, '.', rest of substring, 'E', pw.
//
// - If decimal point is at position 1 (the mantissa is a single digit before the point), then if there are no digits after the point (i.e., string length is 2), output just that digit. Otherwise output that digit, '.', and everything after the decimal point (but skip the point itself, taking substring from index 2). No exponent needed because the exponent is 0.
//
// - If decimal point is at the end (position equals size-1), the number is an integer. The exponent is `(size-2)` because the integer part has `size-1` digits, so the decimal point shifts left by `size-2` places. Remove trailing zeros from the integer part (the substring before the point). If the remaining integer part is just one digit, output that digit and 'E' and the exponent; otherwise, output first digit, '.', rest, 'E', exponent.
//
// - Otherwise (decimal point is somewhere in the middle), the exponent is the number of digits before the decimal point minus 1 (i.e., `pos-1`). Remove the decimal point, then output first digit, '.', the rest of the digits (everything after the first digit), 'E', exponent.
//
// Time complexity is O(n) where n is the length of the input string, due to substring operations. Space complexity is O(n) for the returned string and temporary substrings.
#include <string>
#include <cassert>

// Convert a decimal string to normalized scientific notation.
// Input: non-negative decimal number, at most one '.', at least one digit.
// Output: mantissa (single digit, optionally with '.' and more digits) 
//         followed by 'E' and an integer exponent, or just the digit if exponent is 0.
std::string scientificNotation(const std::string& s) {
    std::string str = s;

    // Ensure there is a decimal point.
    if (str.find('.') == std::string::npos) {
        str.push_back('.');
    }

    // Remove leading zeros before the decimal point.
    size_t first = 0;
    while (first < str.size() && str[first] == '0') {
        ++first;
    }
    str.erase(0, first);

    // Remove trailing zeros after the decimal point (but keep at least one digit).
    while (str.size() > 1 && str.back() == '0') {
        str.pop_back();
    }

    size_t pos = str.find('.');

    if (pos == 0) { // Number is less than 1, e.g., ".xxxx"
        int pw = -1;
        size_t ptr = 1;
        while (ptr < str.size() && str[ptr] == '0') {
            --pw;
            ++ptr;
        }
        // ptr now points to first non-zero digit (must exist)
        std::string mantissa = str.substr(ptr);
        if (mantissa.size() == 1) {
            return mantissa + "E" + std::to_string(pw);
        } else {
            return mantissa.substr(0,1) + "." + mantissa.substr(1) + "E" + std::to_string(pw);
        }
    } else if (pos == 1) { // Exponent is 0
        if (str.size() == 2) { // Just a single digit and the point
            return str.substr(0,1);
        } else {
            return str.substr(0,1) + "." + str.substr(2);
        }
    } else if (pos == str.size() - 1) { // Integer number
        int pw = static_cast<int>(str.size() - 2);
        std::string integerPart = str.substr(0, str.size() - 1);
        // Remove trailing zeros from integer part (already done, but just in case)
        while (integerPart.size() > 1 && integerPart.back() == '0') {
            integerPart.pop_back();
        }
        if (integerPart.size() == 1) {
            return integerPart + "E" + std::to_string(pw);
        } else {
            return integerPart.substr(0,1) + "." + integerPart.substr(1) + "E" + std::to_string(pw);
        }
    } else { // General case: digits before and after the decimal point
        int pw = static_cast<int>(pos - 1);
        std::string digits = str;
        digits.erase(pos, 1); // remove the dot
        return digits.substr(0,1) + "." + digits.substr(1) + "E" + std::to_string(pw);
    }
}
#include <cassert>
#include <string>
#include "scientificNotation.h" // assuming solution is in a header

int main() {
    assert(scientificNotation("123.45") == "1.2345E2");
    assert(scientificNotation("0.00123") == "1.23E-3");
    assert(scientificNotation("100") == "1E2");
    assert(scientificNotation("0.5") == "5E-1");
    assert(scientificNotation("7") == "7"); // exponent 0, no E
    assert(scientificNotation("0.0001") == "1E-4");
    assert(scientificNotation("123456") == "1.23456E5");
    assert(scientificNotation("0.000001") == "1E-6");
    assert(scientificNotation("101.01") == "1.0101E2");
    assert(scientificNotation("0.0000000001") == "1E-10");
    return 0;
}

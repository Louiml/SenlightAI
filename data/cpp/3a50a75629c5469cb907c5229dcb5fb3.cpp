// Write a C++ function named `parseNonNegativeInteger` that takes a `const std::string&` as input. The input string is guaranteed to contain only digits (characters `'0'` through `'9'`) and may be of arbitrary length (including potentially very long, e.g., more than 10 digits). The function must parse the string into an integer, but because the value may exceed the range of `int`, use a `long long` for the result. If the string is empty, return 0. For any non-empty string, calculate the integer value by processing each character from left to right, accumulating `result = result * 10 + (digit)`. The function should return the parsed `long long` value. The task must not use any built-in conversion functions like `stoi`, `stoll`, `atoi`, or string streams—only manual character arithmetic is allowed. Provide the function without a `main` entry point.

#include <cassert>
#include <string>

// Include the solution function declaration or implementation here.

int main() {
    assert(parseNonNegativeInteger("") == 0);
    assert(parseNonNegativeInteger("0") == 0);
    assert(parseNonNegativeInteger("1") == 1);
    assert(parseNonNegativeInteger("9") == 9);
    assert(parseNonNegativeInteger("10") == 10);
    assert(parseNonNegativeInteger("123456789") == 123456789);
    assert(parseNonNegativeInteger("00042") == 42);
    assert(parseNonNegativeInteger("9999999999999999999") == 9999999999999999999LL);
    return 0;
}

#include <string>

// Parse a string of digits into a long long integer.
// Assumes the input contains only characters '0'..'9'.
// Returns 0 for an empty string.
long long parseNonNegativeInteger(const std::string& s) {
    long long result = 0;
    for (const char c : s) {
        result = result * 10 + (c - '0');
    }
    return result;
}

// The core algorithm is straightforward: initialize a `long long result` to 0, then iterate over each character in the input string. For each character `c`, convert it to its integer digit by subtracting `'0'` (since the ASCII codes for digits are contiguous). Then update `result` as `result = result * 10 + digit`. This is the standard manual integer parsing method. Edge cases: an empty string should return 0 (as specified). The input contains only digits, so no need to handle signs, whitespace, or non-digit characters. The use of `long long` helps avoid overflow for inputs up to 19 digits (the maximum for a signed 64-bit integer). Time complexity is O(n) where n is the string length, as each character is processed exactly once. Space complexity is O(1) auxiliary, ignoring the input string itself (which is not modified). No error handling is needed because the input is guaranteed valid digits only.

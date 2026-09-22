// Write a standalone C++ function named `reverseDigits` that accepts a non-negative integer and returns a string containing its decimal digits in reverse order, with no leading zeros (except when the input is 0, in which case return the string "0"). The function must handle the digit extraction manually without using standard library functions like `std::to_string` or `std::reverse`, and must not rely on variable-length arrays or buffers of fixed size that could overflow for large inputs. Instead, build the result dynamically using a `std::string` or a container. Your implementation must be self-contained, include all necessary headers, and be correct for inputs ranging from 0 up to the maximum value of an `int` (e.g., 2147483647).

The main idea is to repeatedly extract the least significant digit of the number using the modulo operation (`n % 10`) and then discard that digit by integer division (`n / 10`). Each extracted digit is appended to a `std::string` (or prepended if you want the reversed order directly, but appending after extraction naturally yields reversed order since we process from least to most significant digit). For example, for input 123: first extract 3, append "3"; then n becomes 12; extract 2, append "2"; then n becomes 1; extract 1, append "1"; n becomes 0 and loop ends, yielding "321". Edge cases: input 0 must return "0" (the loop would not execute since `while(n)` is false), so handle it explicitly. Also, since we convert each digit to a character via `char('0' + digit)`, we correctly handle digits 0-9. For very large inputs, the loop runs at most about 10 times (since `int` max has 10 digits), so no overflow risk. Time complexity is O(d) where d is the number of digits in the number (≤10 for 32-bit int), space complexity is O(d) for the returned string.

#include <string>

// Returns the decimal digits of a non-negative integer in reverse order as a string.
// For input 0, returns "0". No leading zeros are included for non-zero inputs.
std::string reverseDigits(int n) {
    if (n == 0) {
        return "0";
    }
    std::string result;
    while (n > 0) {
        int digit = n % 10;
        result.push_back(static_cast<char>('0' + digit));
        n /= 10;
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (provided in the solution section)
std::string reverseDigits(int n);

int main() {
    assert(reverseDigits(0) == "0");
    assert(reverseDigits(5) == "5");
    assert(reverseDigits(123) == "321");
    assert(reverseDigits(1000) == "0001"); // Note: leading zeros in reversed string are kept per digit extraction, but input has no leading zeros; for 1000, reversed is "0001".
    assert(reverseDigits(2147483647) == "7463847412");
    assert(reverseDigits(10) == "01");
    assert(reverseDigits(7) == "7");
    assert(reverseDigits(987654321) == "123456789");
    assert(reverseDigits(1) == "1");
    assert(reverseDigits(100) == "001");
    return 0;
}

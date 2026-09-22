// Write a C++ function `std::string convertToOctal(int n)` that takes a non-negative integer `n` and returns its octal (base-8) representation as a string, without any leading zeros (except for the input value `0`, which must return `"0"`). The function must handle any valid `int` input that is non-negative, and must not use any standard library conversion functions like `std::oct`, `sprintf`, or `itoa`. The output should contain only digits `0` through `7`.
#include <cassert>
#include <string>

// Declaration of the function under test (already defined in solution).
std::string convertToOctal(int n);

int main() {
    // Basic positive numbers
    assert(convertToOctal(8) == "10");
    assert(convertToOctal(9) == "11");
    assert(convertToOctal(15) == "17");
    assert(convertToOctal(16) == "20");
    
    // Zero edge case
    assert(convertToOctal(0) == "0");
    
    // Power of 8
    assert(convertToOctal(64) == "100");
    
    // Larger number
    assert(convertToOctal(1234) == "2322");
    
    // Maximum non-negative int (assuming 32-bit int, value 2147483647)
    assert(convertToOctal(2147483647) == "17777777777");
    
    // A number that has all digits 7 in octal
    assert(convertToOctal(7) == "7");
    assert(convertToOctal(63) == "77");
    
    return 0;
}
#include <string>

// Convert a non-negative integer to its octal (base-8) string representation.
// Returns "0" for input 0; otherwise produces digits without any leading zeros.
std::string convertToOctal(int n) {
    if (n == 0) {
        return "0";
    }

    std::string result;
    while (n > 0) {
        int remainder = n % 8;
        char digit = static_cast<char>(remainder + '0');
        result = digit + result;  // prepend to build most significant first
        n /= 8;
    }
    return result;
}
// The algorithm repeatedly divides the input number by 8 and collects the remainders. Since the remainders are produced from least significant to most significant, we build the string by prepending each remainder converted to its character form (`char(remainder + '0')`, where `'0'` is 48 in ASCII). The loop continues while `n > 0`. A special edge case is when the input is exactly `0`; the loop would not run, so we must explicitly return `"0"` in that case. For any positive input, the number of iterations equals the number of digits in the octal representation, which is at most `ceil(log_8(INT_MAX))` ≈ 11 for a 32-bit integer, but for general analysis: if `n` has `k` decimal digits, the number of octal digits is approximately `k * log_10(8)` ≈ `0.903k`. Time complexity is `O(log_8 n)`, equivalently `O(number of digits)`, and space complexity is `O(number of digits)` for the returned string, ignoring the temporary string building overhead. The algorithm is correct for all non-negative integers, including values that happen to be exact powers of 8, because the loop runs until `n` becomes 0, with the final division producing a quotient of zero.

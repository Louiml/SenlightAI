// Write a C++ function named `digitalRoot` that takes a non-negative integer as input and returns its digital root, which is the single-digit value obtained by repeatedly summing the digits of the number until only one digit remains. For example, the digital root of 9875 is 2 because 9+8+7+5=29, 2+9=11, and 1+1=2. The function must handle the input 0 correctly (returning 0), and must work for arbitrarily large integers that may not fit in standard integer types; therefore, the input should be provided as a `std::string` representing the number in decimal notation. The function must not use any additional libraries beyond the standard ones, and must be `const`-correct. You may assume the input string contains only digits and is non-empty.

int main() {
    assert(digitalRoot("0") == "0");
    assert(digitalRoot("5") == "5");
    assert(digitalRoot("18") == "9");
    assert(digitalRoot("27") == "9");
    assert(digitalRoot("9875") == "2");
    assert(digitalRoot("123456789") == "9");
    assert(digitalRoot("000") == "0");
    assert(digitalRoot("999999999999999999999999999999") == "9");
    assert(digitalRoot("100000000000000000000000000000000000000") == "1");
    assert(digitalRoot("999999999999999999999999999999999999999999999") == "9");
    return 0;
}

#include <string>
#include <numeric>
#include <cctype>

// Compute the digital root (single-digit result of repeatedly summing digits)
// of a non-negative integer given as a decimal string.
// The input must contain only digits and be non-empty.
std::string digitalRoot(const std::string& number) {
    if (number == "0") {
        return "0";
    }

    // Sum all digits. Since each digit is <= 9, the sum fits in a long long.
    long long sum = 0;
    for (char c : number) {
        sum += c - '0';
    }

    // Apply the digital root formula for a positive sum.
    long long root = 1 + (sum - 1) % 9;
    return std::to_string(root);
}

// The digital root can be computed directly by repeated summation as described, but an efficient approach uses the mathematical property that the digital root of a positive integer `n` is `1 + (n - 1) % 9`, and for `n = 0` the digital root is 0. However, since the input is given as a string that may be extremely long, we cannot convert it to a standard integer type. Therefore, we compute the sum of all digits first (which fits in a small integer, e.g., a `long long`, because each digit is at most 9 and the string length is limited by memory). Then apply the formula to that sum. Alternatively, we can simulate the repeated summation directly on the string, converting the string to a new string after each pass until length becomes 1. The formula approach is simpler and faster. Edge cases: input "0" should return 0; input with leading zeros like "000" should return 0; input "1" through "9" return that digit. The time complexity is O(n) where n is the number of digits (to compute the sum), and space complexity is O(1) besides the input string and the output string.

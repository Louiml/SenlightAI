Write a standalone C++ function named `decimalToBinaryString` that takes a single non-negative integer as input and returns a string containing its binary representation (with no leading zeros, except for the input `0`, which should return `"0"`). The function must validate that the input is within the range 0 to 255 inclusive; if the input is outside this range, it should return the string `"Error:Give proper integer number"`. Do not rely on any built-in binary conversion functions (e.g., `std::bitset` or `std::to_chars`); implement the conversion manually using repeated division by 2, building the result in reverse order. The function should be `const`-correct for any parameters and should not modify the input.
The solution uses the classic repeated division algorithm: repeatedly divide the input by 2, recording the remainder (0 or 1) until the quotient becomes 0, then reverse the collected digits to form the binary string. Edge cases: if the input is exactly 0, the loop would produce no digits, so it must be handled specially by returning `"0"`. If the input is negative or greater than 255, the function should immediately return the error string without performing any computation. The input value is passed by value, so `const` applies only to local variables (e.g., a loop variable) but not to the parameter itself; we can declare local variables as `const` where appropriate to avoid accidental modification. Time complexity is \(O(\log_2 n)\) for the loop (at most 8 iterations for numbers up to 255), and space complexity is \(O(\log_2 n)\) for the intermediate string (at most 8 characters).
#include <string>
#include <algorithm>

// Converts a non-negative integer (0-255) to its binary string representation.
// Returns error string for out-of-range input or negative values.
std::string decimalToBinaryString(int value) {
    if (value < 0 || value > 255) {
        return "Error:Give proper integer number";
    }
    if (value == 0) {
        return "0";
    }

    std::string binary;
    while (value > 0) {
        const int remainder = value % 2;
        binary.push_back(remainder ? '1' : '0');
        value /= 2;
    }
    std::reverse(binary.begin(), binary.end());
    return binary;
}
#include <cassert>
#include <string>

// Declaration of the function under test (must match the solution).
std::string decimalToBinaryString(int value);

int main() {
    // Basic cases
    assert(decimalToBinaryString(0) == "0");
    assert(decimalToBinaryString(1) == "1");
    assert(decimalToBinaryString(2) == "10");
    assert(decimalToBinaryString(3) == "11");
    assert(decimalToBinaryString(4) == "100");
    assert(decimalToBinaryString(5) == "101");
    assert(decimalToBinaryString(6) == "110");
    assert(decimalToBinaryString(7) == "111");
    assert(decimalToBinaryString(8) == "1000");
    assert(decimalToBinaryString(10) == "1010");

    // Boundary cases
    assert(decimalToBinaryString(255) == "11111111");

    // Invalid inputs
    assert(decimalToBinaryString(-1) == "Error:Give proper integer number");
    assert(decimalToBinaryString(256) == "Error:Give proper integer number");
    assert(decimalToBinaryString(1000) == "Error:Give proper integer number");

    // Additional random check
    assert(decimalToBinaryString(128) == "10000000");
    assert(decimalToBinaryString(169) == "10101001");
    assert(decimalToBinaryString(200) == "11001000");

    return 0;
}

/*
Write a C++ function named `parityString` that takes a single integer parameter and returns the string `"Odd"` if the integer is odd, and `"Even"` if it is even, without using the modulo operator or division. The function must handle negative integers and zero correctly, where zero is considered even. The function should be `const`-correct (i.e., it does not modify its parameter and can be called on const objects) and must be self-contained with all necessary headers included.
*/

#include <string>

// Return "Odd" if n is odd, otherwise "Even" (zero considered even).
std::string parityString(const int n) {
    if (n & 1) {
        return "Odd";
    }
    return "Even";
}

#include <cassert>
#include <string>

int main() {
    assert(parityString(0) == "Even");
    assert(parityString(1) == "Odd");
    assert(parityString(2) == "Even");
    assert(parityString(-3) == "Odd");
    assert(parityString(-4) == "Even");
    assert(parityString(100) == "Even");
    assert(parityString(101) == "Odd");
    assert(parityString(-1) == "Odd");
    assert(parityString(-2) == "Even");
    assert(parityString(2147483647) == "Odd"); // INT_MAX
    return 0;
}

// The core algorithm uses a bitwise AND operation with `1` to determine the least significant bit of the integer. In binary, an odd number always has its least significant bit set to `1` (e.g., 3 = 0b11, -5 = 0b1011...1), while an even number (including zero) has it as `0` (e.g., 2 = 0b10, 0 = 0b0). Therefore, `(n & 1)` yields `1` for odd and `0` for even, regardless of sign, because bitwise operations work on the two's complement representation. This approach avoids modulo and division, making it efficient for all integer ranges. Edge cases: zero returns `"Even"` (since `0 & 1 == 0`), negative odd numbers (e.g., -3) return `"Odd"` (since `-3 & 1 == 1`), and negative even numbers (e.g., -4) return `"Even"`. The time complexity is O(1) constant time, and space complexity is O(1) auxiliary space (only the returned string allocation).

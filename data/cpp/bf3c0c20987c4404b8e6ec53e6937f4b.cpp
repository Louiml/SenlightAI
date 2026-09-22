/*
Write a C++ function named `toBinaryString` that takes a non-negative integer as input and returns its binary representation as a `std::string`, without leading zeros (except for the number 0 itself, which should return `"0"`). The function must be implemented recursively, following the same logic as converting a decimal number to binary by repeatedly dividing by 2 and prepending the remainder. You must not use any built-in binary conversion utilities (e.g., `std::bitset`, `itoa`, or string streams for formatting). The function should handle edge cases such as `0`, `1`, power-of-two values, very large integers (up to at least 2^20), and odd/even numbers correctly. Provide the function definition with appropriate `const` correctness where applicable, and ensure it is self-contained with necessary headers.
*/

#include <string>

// Convert a non-negative integer to its binary string representation (no leading zeros).
std::string toBinaryString(int n) {
    if (n == 0) {
        return "0";
    }
    if (n == 1) {
        return "1";
    }
    // Recursively build the binary representation of n / 2, then append the remainder.
    return toBinaryString(n / 2) + (n % 2 == 0 ? "0" : "1");
}

#include <cassert>
#include <string>

std::string toBinaryString(int n);

int main() {
    // Base cases
    assert(toBinaryString(0) == "0");
    assert(toBinaryString(1) == "1");
    
    // Small numbers
    assert(toBinaryString(2) == "10");
    assert(toBinaryString(3) == "11");
    assert(toBinaryString(4) == "100");
    assert(toBinaryString(5) == "101");
    
    // Power of two
    assert(toBinaryString(512) == "1000000000");
    
    // Arbitrary numbers
    assert(toBinaryString(1697) == "11010100001");
    
    // Large number (2^20 - 1)
    assert(toBinaryString(1048575) == "11111111111111111111");
    
    // Even and odd large
    assert(toBinaryString(1024) == "10000000000");
    assert(toBinaryString(1023) == "1111111111");
    
    return 0;
}

// The core algorithm is a direct recursive implementation of the standard division-by-2 method for binary conversion. Given a number `n`:
// - Base case: If `n == 0`, return `"0"`; if `n == 1`, return `"1"`.
// - Recursive step: For `n > 1`, compute the binary representation of `n / 2` (integer division) and append the remainder (`n % 2`) as a character (`'0'` or `'1'`). This works because the binary representation of `n` is the binary representation of `n / 2` followed by the least significant bit of `n`. Edge cases: `0` and `1` are handled by base cases to avoid infinite recursion and to produce correct output (especially for `0`, which would otherwise require special handling since `0 / 2` is still `0`). Power-of-two numbers work correctly—e.g., `512` yields `"1000000000"`. The recursion depth for an integer `n` is about `log2(n)`, so for `n` up to about 2^20, depth is ~20, which is safe. Time complexity is `O(log n)` because each recursive call reduces `n` by roughly half, and string concatenation is also `O(log n)` overall if we consider building the result. Space complexity is `O(log n)` due to the call stack and the result string length.

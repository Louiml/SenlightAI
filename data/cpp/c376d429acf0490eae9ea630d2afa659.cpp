/*
Write a C++ function that accepts a non-negative integer `N` (where `0 <= N < 2^30`) and returns a 30-character string representing its binary representation, left-padded with leading zeros to exactly 30 bits. The string must have the most significant bit first (i.e., standard binary order), and each character must be either `'0'` or `'1'`. For example, given `N = 5`, the result should be `"000000000000000000000000000101"` (27 zeros followed by `101`). The function should handle edge cases such as `N = 0` (all zeros) and `N = 2^29` (a single `1` followed by 29 zeros). You may assume the input is always within the specified range, so no error handling for negative or overly large values is required.
*/
#include <string>

// Return a 30-character string containing the binary representation of N,
// left-padded with leading zeros. Assumes 0 <= N < 2^30.
std::string toBinary30(int N) {
    std::string result;
    result.reserve(30);
    for (int i = 29; i >= 0; --i) {
        int bit = (N >> i) & 1;                 // Extract bit i
        result.push_back(static_cast<char>('0' + bit));
    }
    return result;
}
#include <cassert>
#include <string>

// Assume toBinary30 is declared above

int main() {
    // Basic small numbers
    assert(toBinary30(0) == "000000000000000000000000000000");
    assert(toBinary30(1) == "000000000000000000000000000001");
    assert(toBinary30(5) == "000000000000000000000000000101");

    // Edge: single 1 at the most significant bit (2^29)
    assert(toBinary30(536870912) == "100000000000000000000000000000");

    // Edge: maximum allowed value (2^30 - 1)
    assert(toBinary30(1073741823) == "111111111111111111111111111111");

    // Number with alternating bits: 0b101010... (10 bits: 0x2AA = 682)
    assert(toBinary30(682) == "000000000000000000000010101010");

    // Large number: 2^29 + 2^0 = 536870913
    assert(toBinary30(536870913) == "100000000000000000000000000001");

    // Random mid-range: 123456789
    assert(toBinary30(123456789) == "00000000000000000111010110110101");

    // Power of two: 2^20
    assert(toBinary30(1048576) == "000000000000010000000000000000");

    // All bits set except the most significant: 2^29 - 1
    assert(toBinary30(536870911) == "011111111111111111111111111111");

    // Repeat a case to ensure no side effects
    assert(toBinary30(5) == "000000000000000000000000000101");

    return 0;
}
// The solution iterates over the 30 bit positions from the most significant (bit 29, corresponding to value 2^29) down to the least significant (bit 0). For each bit position `i`, it extracts the bit value using the bitwise right shift `(N >> i) & 1`, which isolates the bit at that position. The extracted integer `p` (either 0 or 1) is then converted to a character by adding `'0'` (since `'0'` has ASCII value 48) and appended to a result string. This builds the binary string from the most significant bit to the least significant bit. The loop runs exactly 30 times, ensuring the output always has 30 characters, including leading zeros. Edge cases: `N = 0` yields all zeros; `N = 2^29` yields a single `1` at the first position (index 0) followed by 29 zeros; the maximum allowed `N = 2^30 - 1` yields all ones. Time complexity is O(30) = O(1) because the loop runs a fixed number of times. Space complexity is O(30) = O(1) for the output string, excluding the input storage.

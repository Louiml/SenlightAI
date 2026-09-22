// Write a C++ function `baseStringToDecimal` that takes a string `number` representing a non-negative integer in a given base `base` (where `base` is between 2 and 36 inclusive), and returns the integer value in decimal. The input string consists only of digits `0-9` and uppercase letters `A-Z` (for values 10–35), with no leading zeros (except the string `"0"` itself). The function must handle large inputs correctly (i.e., results may exceed typical 32-bit range, but will fit within `long long`). Do not use any built‑in conversion functions; implement the conversion manually. The function should be `const`-correct and efficient.
#include <cassert>

int main() {
    // Basic decimal
    assert(baseStringToDecimal("0", 10) == 0);
    assert(baseStringToDecimal("123", 10) == 123);
    assert(baseStringToDecimal("7", 10) == 7);

    // Binary
    assert(baseStringToDecimal("101", 2) == 5);
    assert(baseStringToDecimal("11111111", 2) == 255);
    assert(baseStringToDecimal("1000", 2) == 8);

    // Hexadecimal (base 16) - uppercase only
    assert(baseStringToDecimal("A", 16) == 10);
    assert(baseStringToDecimal("FF", 16) == 255);
    assert(baseStringToDecimal("1A", 16) == 26);

    // Base 36
    assert(baseStringToDecimal("Z", 36) == 35);
    assert(baseStringToDecimal("10", 36) == 36);
    assert(baseStringToDecimal("2A", 36) == 2*36 + 10); // 82

    // Large value beyond 32-bit but within long long
    assert(baseStringToDecimal("7FFFFFFFFFFFFFFF", 16) == 9223372036854775807LL);
    assert(baseStringToDecimal("111111111111111111111111111111111111", 2) == 68719476735LL); // 2^36 - 1

    // Odd bases
    assert(baseStringToDecimal("202", 3) == 2*9 + 0*3 + 2); // 20
    assert(baseStringToDecimal("B", 12) == 11);
}
#include <string>

// Convert a string representing a number in a given base (2-36) to a decimal long long.
long long baseStringToDecimal(const std::string& number, int base) {
    long long result = 0;
    for (char c : number) {
        int digitValue = 0;
        if (c >= 'A' && c <= 'Z') {
            digitValue = c - 'A' + 10;
        } else { // c is a digit '0'-'9'
            digitValue = c - '0';
        }
        result = result * base + digitValue;
    }
    return result;
}
// The algorithm processes each character of the input string from left to right, maintaining a running decimal value. For each character, we first determine its numeric value: if it is a digit (`'0'` to `'9'`), the value is `c - '0'`; if it is an uppercase letter (`'A'` to `'Z'`), the value is `10 + (c - 'A')`. Then we update the running result as `result = result * base + digitValue`. This is exactly how positional numeral systems work.  
// Edge cases:  
// - The string `"0"` should return `0`.  
// - The base can be as low as 2, so the string will contain only valid characters for that base (input is guaranteed valid).  
// - Values can be up to `long long` range, so we use `long long` for the result to avoid overflow.  
// - The string may be empty? The problem states non-empty, so we can assume at least one character.  
// Time complexity: \(O(n)\) where \(n\) is the length of the string, since we process each character once.  
// Space complexity: \(O(1)\) auxiliary space, not counting input storage.

// Write a C++ function `std::string base12ToBinaryWithSpacing(const std::string& input)` that accepts a non-empty string containing only the characters `'0'`–`'9'`, `'a'`, and `'b'` (representing base-12 digits, where `'a'` = 10 and `'b'` = 11). The function must validate the input: if any character is invalid, return the string `"input error!"`. Otherwise, it must parse the string as a base-12 number (from left to right, most significant digit first), compute its decimal (base-10) integer value, and return a string that (1) lists each digit's decimal value separated by spaces, followed by a newline, (2) then the decimal value on its own line, and (3) finally the binary representation of that decimal value as exactly 32 bits (with leading zeros) grouped into four groups of 8 bits separated by single spaces. The function must not print anything itself; it must return this fully formatted string. For example, for input `"1a"`, the expected return string is `"1 10\n22\n00000000 00000000 00000000 00010110"`.
// The solution requires three main steps: input validation, base-12 to decimal conversion, and decimal to 32-bit binary formatting. First, iterate over each character of the input string; if any character is not a digit `'0'`–`'9'` nor `'a'` nor `'b'`, immediately return `"input error!"`. Assuming valid input, perform a left-to-right scan: for each character, convert it to its numeric value (`'0'`–`'9'` → 0–9, `'a'` → 10, `'b'` → 11), append that numeric value as a string followed by a space (for the first output line), and update the accumulated decimal sum using `sum = sum * 12 + digitValue`. Once the scan completes, trim the trailing space from the digit-list string, append a newline and the decimal sum, then a newline and the binary representation. For the binary representation, use an array of 32 integers initialized to zero, start from the most significant bit (index 31) and fill from right to left by repeatedly taking `sum % 2` and dividing `sum` by 2, stopping when `sum` becomes zero. Then construct a string that iterates over the 32-bit array left to right, appending each bit, and adding a space after every 8th bit. Edge cases include: a single-character input like `"0"` which yields decimal 0 and all 32 bits zero; leading zeros in the input are naturally handled by the conversion (they contribute 0 to the sum but each digit still appears in the digit list); invalid characters return the error string immediately without further processing. Time complexity is O(n + 32) = O(n) for n input length, and space complexity is O(n) for the output string and O(1) extra auxiliary (fixed 32-bit array).
#include <string>
#include <vector>
#include <cctype>

// Converts a valid base-12 digit character to its decimal value.
int charToValue(char c) {
    if (std::isdigit(static_cast<unsigned char>(c))) {
        return c - '0';
    }
    return (c == 'a') ? 10 : 11;
}

// Validates and converts a base-12 string to a formatted output string.
std::string base12ToBinaryWithSpacing(const std::string& input) {
    // Validate input: only digits 0-9, 'a', 'b' allowed.
    for (char ch : input) {
        if (!std::isdigit(static_cast<unsigned char>(ch)) && ch != 'a' && ch != 'b') {
            return "input error!";
        }
    }

    std::string digitList;
    int decimalValue = 0;

    // Convert each digit, build the digit list, accumulate decimal value.
    for (char ch : input) {
        int val = charToValue(ch);
        digitList += std::to_string(val) + " ";
        decimalValue = decimalValue * 12 + val;
    }

    // Remove trailing space from digit list.
    if (!digitList.empty()) {
        digitList.pop_back();
    }

    // Generate 32-bit binary representation.
    std::vector<int> bits(32, 0);
    int temp = decimalValue;
    int index = 31;
    do {
        bits[index] = temp % 2;
        temp /= 2;
        index--;
    } while (temp > 0);

    std::string binaryStr;
    for (int i = 0; i < 32; ++i) {
        binaryStr += (bits[i] ? '1' : '0');
        if ((i + 1) % 8 == 0 && i != 31) {
            binaryStr += ' ';
        }
    }

    return digitList + "\n" + std::to_string(decimalValue) + "\n" + binaryStr;
}
#include <cassert>
#include <string>

// Function declaration (as above) would be here.

int main() {
    // Basic valid input
    assert(base12ToBinaryWithSpacing("1a") == "1 10\n22\n00000000 00000000 00000000 00010110");
    // Single digit zero
    assert(base12ToBinaryWithSpacing("0") == "0\n0\n00000000 00000000 00000000 00000000");
    // Single digit max value
    assert(base12ToBinaryWithSpacing("b") == "11\n11\n00000000 00000000 00000000 00001011");
    // Multi-digit leading zeros
    assert(base12ToBinaryWithSpacing("00b") == "0 0 11\n11\n00000000 00000000 00000000 00001011");
    // All digits including letters
    assert(base12ToBinaryWithSpacing("ab1") == "10 11 1\n1573\n00000000 00000000 00000110 00100101");
    // Invalid character
    assert(base12ToBinaryWithSpacing("12c") == "input error!");
    // Invalid uppercase letter
    assert(base12ToBinaryWithSpacing("A") == "input error!");
    // Empty string (though spec says non-empty, handle gracefully)
    assert(base12ToBinaryWithSpacing("") == "input error!");  // Not specified, defensive test
    // Large value to test multiple binary groups
    assert(base12ToBinaryWithSpacing("bbbb") == "11 11 11 11\n20735\n00000000 00000000 01010000 11111111");
    return 0;
}

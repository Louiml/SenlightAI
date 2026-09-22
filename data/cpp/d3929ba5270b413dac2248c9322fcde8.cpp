/*
Write a C++ function that takes a single string containing a message encoded by shifting each printable ASCII character 7 positions forward in the ASCII table (i.e., each original character `c` was transformed to `c + 7`, with standard `char` arithmetic), and returns the decoded string where each character is shifted back by 7 (i.e., `c - 7`). The input string may contain any printable ASCII characters including spaces, digits, punctuation, and letters; it will never be empty, and it is guaranteed that the encoded characters are all within the printable ASCII range (ASCII 32–126) so that subtracting 7 always yields a valid printable character. No other transformations are applied, and the output must preserve the original order and length.
*/

#include <string>

// Decodes a string encoded by adding 7 to each character's ASCII value.
// Returns the decoded string with each character shifted back by 7.
std::string decodeShift7(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.size());
    for (char ch : encoded) {
        decoded.push_back(static_cast<char>(ch - 7));
    }
    return decoded;
}

#include <cassert>
#include <string>

// Assume the solution function is defined above (included here for completeness).
std::string decodeShift7(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.size());
    for (char ch : encoded) {
        decoded.push_back(static_cast<char>(ch - 7));
    }
    return decoded;
}

int main() {
    // Basic encoding/decoding roundtrip: original "Hello" -> encoded "Olssv"
    assert(decodeShift7("Olssv") == "Hello");

    // Test with spaces and punctuation (encoded equivalents of "Hi!" are "Op'")
    assert(decodeShift7("Op'") == "Hi!");

    // Test with digits and symbols: original "123" -> encoded ":;>"
    assert(decodeShift7(":;>") == "123");

    // Test with a longer sentence: original "C++ is fun" -> encoded "J'' pz mvu"
    assert(decodeShift7("J'' pz mvu") == "C++ is fun");

    // Test with all printable characters (from space +7 to tilde +7, but limited to valid range)
    // Original "AZaz09" -> encoded "Hghg67" (A+7=H, Z+7=g, a+7=h, z+7=g, 0+7=7, 9+7=@ but here we use valid)
    assert(decodeShift7("Hghg67") == "AZaz09");

    // Test with a string containing only spaces (encoded with spaces+7)
    // Original "   " -> encoded "'''" (each space 32+7=39)
    assert(decodeShift7("'''") == "   ");

    // Test with a single character: original "A" -> encoded "H"
    assert(decodeShift7("H") == "A");

    // Test with empty string (though not expected, function returns empty)
    assert(decodeShift7("") == "");

    return 0;
}

// The solution is straightforward: iterate through each character in the input string, subtract 7 from its ASCII value using `char` arithmetic, and build the result string. Since the input is guaranteed to only contain characters that after subtraction remain printable, no special overflow or out-of-range handling is needed. The main algorithm simply loops over the string once, applying the transformation. The key edge case is that spaces (ASCII 32) become ASCII 25 (a non‑printable control character) after subtracting 7, but the task statement guarantees that such cases will not occur; nonetheless, the code works as specified. Time complexity is O(n) where n is the length of the string, and space complexity is O(n) for the resulting string (the input is copied into a new string). The function should be `const`‑correct by taking the input as `const std::string&` and returning a new `std::string`.

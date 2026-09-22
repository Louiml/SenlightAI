Write a C++ function named `decodeWerty` that takes a string `encoded` representing text typed with the keyboard shifted one key to the right (so each character in the input is the key to the right of the intended key on a standard US QWERTY keyboard layout, including digits, punctuation, and uppercase letters only) and returns the decoded string with each character replaced by the key immediately to its left. Spaces remain unchanged. The valid input characters are exactly those from the set: `1234567890-=WERTYUIOP[]\SDFGHJKL;'XCVBNM,.` (note the backslash is included). The function should handle empty strings gracefully, returning an empty string. All input characters are guaranteed to be from the valid set or spaces, so no error handling is needed. Use a mapping approach: for each character in the input, find its position in the "right" sequence and replace it with the corresponding character from the "left" sequence.

#include <cassert>
#include <string>

// Declaration of the function under test (included here for the test)
std::string decodeWerty(const std::string& encoded);

int main() {
    // Basic decoding
    assert(decodeWerty("WERTYU") == "QWERTY");
    assert(decodeWerty("SDFGH") == "ASDFG");
    assert(decodeWerty("XCVBN") == "ZXCVB");
    assert(decodeWerty("12345") == "`1234");
    assert(decodeWerty("O;") == "IP");
    assert(decodeWerty("/") == ".");
    assert(decodeWerty("\\") == "]");

    // Spaces preserved
    assert(decodeWerty("W S X") == "Q A Z");

    // Empty string
    assert(decodeWerty("") == "");

    // Full sentence typed with shift
    std::string typed = "O S G;T R";
    std::string expected = "I AM A E";
    assert(decodeWerty(typed) == expected);

    // All characters from layout mapping
    std::string allRight = "1234567890-=WERTYUIOP[]\\SDFGHJKL;'XCVBNM,./";
    std::string allLeft  = "~1234567890-QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,.";
    assert(decodeWerty(allRight) == allLeft);

    return 0;
}

#include <string>

// Decode a string typed with keyboard shifted one key to the right.
// Maps each character to the key immediately to its left on a US QWERTY layout.
std::string decodeWerty(const std::string& encoded) {
    // Layout when typing one key to the right
    const std::string right = "1234567890-=WERTYUIOP[]\\SDFGHJKL;'XCVBNM,./";
    // Corresponding intended (left) characters
    const std::string left  = "~1234567890-QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,.";

    std::string decoded;
    decoded.reserve(encoded.size());

    for (char ch : encoded) {
        if (ch == ' ') {
            decoded += ' ';
        } else {
            // Find the character in the right layout
            std::size_t pos = right.find(ch);
            if (pos != std::string::npos) {
                decoded += left[pos];
            } else {
                // Should not happen for valid input; keep original as fallback
                decoded += ch;
            }
        }
    }
    return decoded;
}

// The core idea is to create two parallel strings: one representing the keyboard layout when shifted right (the "actual" typed characters) and one representing the intended characters (the "left" counterparts). For each input character, we scan the first string to find its index, then pick the character at that same index from the second string. Since spaces are not in the layout, they are copied directly. We build the output string by iterating through the input once. Edge cases: empty input returns empty output; a character not found should not happen per problem constraints, but if it did, we could skip or copy it, but here we assume valid input. Time complexity is \(O(n \cdot m)\) where \(n\) is the length of the input and \(m\) is the length of the layout string (fixed at 43), so effectively \(O(n)\). Space complexity is \(O(n)\) for the output string.

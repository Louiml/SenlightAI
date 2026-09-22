// Write a C++ function named `isValidIdentifier` that takes a single `std::string` parameter and returns a `bool` indicating whether the string is a valid variable name in a simplified C-like language. The rules are: the string must be non-empty, the first character must be a letter (A-Z, a-z) or an underscore (`_`), and every subsequent character must be a letter, digit (0-9), or underscore. No spaces or other special characters are allowed anywhere in the string. The function should be case-sensitive and must safely handle empty strings (return `false` for empty input).
The solution checks the string character by character. First, if the string is empty, the function returns `false` immediately because an empty string cannot be a valid identifier. Then, it verifies the first character using `std::isalpha` (which returns nonzero for letters) or checks if it equals `'_'`; if neither, it returns `false`. For all remaining characters, it uses `std::isalnum` (which checks for letters or digits) plus the underscore check; if any character fails, it returns `false`. If every character passes, the function returns `true`. Edge cases include: strings starting with a digit, strings containing spaces, empty strings, strings with only a single valid letter/underscore, and strings with underscores in the middle. The time complexity is O(n) where n is the string length, and auxiliary space is O(1) because no extra storage is used beyond a loop index.
#include <string>
#include <cctype>

// Checks if a string is a valid identifier: first char letter or underscore,
// all subsequent chars letter, digit, or underscore. Empty strings are invalid.
bool isValidIdentifier(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    // First character must be a letter or underscore
    if (!std::isalpha(static_cast<unsigned char>(name[0])) && name[0] != '_') {
        return false;
    }
    
    // Remaining characters must be letter, digit, or underscore
    for (std::size_t i = 1; i < name.length(); ++i) {
        const unsigned char ch = static_cast<unsigned char>(name[i]);
        if (!std::isalnum(ch) && ch != '_') {
            return false;
        }
    }
    
    return true;
}
#include <cassert>
#include <string>

int main() {
    assert(isValidIdentifier("abc") == true);
    assert(isValidIdentifier("_var") == true);
    assert(isValidIdentifier("a1_b2") == true);
    assert(isValidIdentifier("A") == true);
    assert(isValidIdentifier("_") == true);
    assert(isValidIdentifier("1abc") == false);
    assert(isValidIdentifier("abc def") == false);
    assert(isValidIdentifier("abc-def") == false);
    assert(isValidIdentifier("") == false);
    assert(isValidIdentifier("abc$") == false);
}

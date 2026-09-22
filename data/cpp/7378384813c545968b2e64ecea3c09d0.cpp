// Write a C++ function named `swapCase` that takes a single non-empty string (which may contain uppercase letters, lowercase letters, digits, spaces, and punctuation) and returns a new string where every alphabetic character has its case inverted: lowercase letters become uppercase and uppercase letters become lowercase. All non-alphabetic characters must remain unchanged. The function should not modify the original input string.
The solution iterates over each character in the input string. For each character, use `std::islower` and `std::toupper` or `std::isupper` and `std::tolower` to convert the case. Since `std::islower` and `std::isupper` expect an `unsigned char` value (or EOF) to avoid undefined behavior for negative `char` values, cast the character to `unsigned char` before passing it to these functions. The result is appended to a new string. The original string remains unchanged because the function takes a `const std::string&` and returns a new string. Edge cases include strings with no alphabetic characters (returns the same string), strings with mixed cases and non-alphabetic symbols (digits, spaces, punctuation are left as-is). Time complexity is O(n) where n is the length of the string, and space complexity is O(n) for the returned string (excluding input storage).
#include <string>
#include <cctype>

// Return a copy of the input with every alphabetic character's case swapped.
// Non-alphabetic characters are unchanged.
std::string swapCase(const std::string& input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::islower(uc)) {
            result.push_back(static_cast<char>(std::toupper(uc)));
        } else if (std::isupper(uc)) {
            result.push_back(static_cast<char>(std::tolower(uc)));
        } else {
            result.push_back(c);
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Forward declaration of the solution function
std::string swapCase(const std::string& input);

int main() {
    assert(swapCase("Hello World") == "hELLO wORLD");
    assert(swapCase("ABC") == "abc");
    assert(swapCase("xyz") == "XYZ");
    assert(swapCase("123 !@#") == "123 !@#");
    assert(swapCase("MiXeD CaSe 123") == "mIxEd cAsE 123");
    assert(swapCase("a") == "A");
    assert(swapCase("Z") == "z");
    assert(swapCase("") == "");
    return 0;
}

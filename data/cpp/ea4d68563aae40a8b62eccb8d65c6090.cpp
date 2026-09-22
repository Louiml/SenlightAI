// Write a C++ function that takes a non-empty string containing a person's full name (consisting of first, middle, and last words separated by single spaces) and returns a new string with the entire name reversed character by character, including the spaces, so that the last character of the original string becomes the first character of the result. The function must preserve the exact characters and spacing of the input. For example, the input `"Freta Yordinia Laura"` should produce `"aruaL ainidroY aterF"`. The function must be named `reverseFullName` and accept the input by const reference.
#include <cassert>
#include <string>

// Declaration of the function under test.
std::string reverseFullName(const std::string& name);

int main() {
    // Basic three-word name.
    assert(reverseFullName("Freta Yordinia Laura") == "aruaL ainidroY aterF");
    // Single-word name.
    assert(reverseFullName("Alice") == "ecilA");
    // Two-word name.
    assert(reverseFullName("John Doe") == "eoD nhoJ");
    // Name with trailing spaces (even though problem says single spaces, test robustness).
    assert(reverseFullName("  A B  ") == "  B A  ");
    // Single character.
    assert(reverseFullName("Z") == "Z");
    // Empty string (not in spec, but function should handle it).
    assert(reverseFullName("") == "");
    // Longer name with mixed case and punctuation.
    assert(reverseFullName("Ada Lovelace") == "ecalevoL adA");
    // Repeated spaces exactly preserved.
    assert(reverseFullName("X  Y") == "Y  X");
    // A name that is a palindrome (e.g., "Anna").
    assert(reverseFullName("Anna") == "annA");
    // All spaces (not typical, but test).
    assert(reverseFullName("   ") == "   ");
    return 0;
}
#include <string>
#include <algorithm>

// Return a string that is the character-by-character reverse of the input.
// The original string is not modified.
std::string reverseFullName(const std::string& name) {
    // Make a copy to avoid modifying the caller's string.
    std::string reversed = name;
    // Reverse the entire copied string.
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}
// The solution leverages the standard library's `std::reverse` algorithm, which reverses a range of characters in-place. Since we need to return a reversed copy without modifying the original, we first create a copy of the input string. Then we apply `std::reverse` on the entire range of the copy (from `begin()` to `end()`). Edge cases include single-word names (e.g., `"Alice"`), inputs with leading/trailing spaces (though the problem states single spaces between words, the algorithm handles general spacing), and empty strings (though the specification says non-empty, the function handles an empty input gracefully by returning an empty string). Time complexity is O(n), where n is the length of the input string, because we copy the string (O(n)) and then reverse it in-place (also O(n)). Space complexity is O(n) because we allocate a new string of the same length as the input.

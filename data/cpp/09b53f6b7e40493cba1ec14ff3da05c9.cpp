Write a C++ function `removeAllDots(const std::string& input)` that takes a string as input and returns a new string containing only the non-dot characters from the original string, preserving their original order. The function should handle empty strings, strings with only dots, and strings with no dots at all. It must not modify the input string and must be `const`-correct.

The solution iterates character by character through the input string using a range-based for loop or an index-based loop. For each character, if it is not equal to `'.'`, we append it to a result string. This preserves relative order because we append as we scan. Edge cases: an empty string returns an empty result; a string of only dots returns an empty string; a string with no dots returns a copy equal to the input. We can optimize by reserving capacity in the result string to avoid repeated reallocations, but this is not necessary for correctness. Time complexity is O(n) where n is the length of the input string, as each character is examined exactly once. Space complexity is O(n) for the result string (excluding input storage), though we could also say O(n) auxiliary space because we build a new string.

#include <string>

// Remove all dot characters from the input string, preserving order.
// Returns a new string containing only non-dot characters.
std::string removeAllDots(const std::string& input) {
    std::string result;
    // Reserve capacity to avoid reallocations (optional optimization)
    result.reserve(input.size());
    for (char ch : input) {
        if (ch != '.') {
            result.push_back(ch);
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Function declaration (or include the header if separate)
std::string removeAllDots(const std::string& input);

int main() {
    // Empty string
    assert(removeAllDots("") == "");
    // Only dots
    assert(removeAllDots("...") == "");
    // No dots
    assert(removeAllDots("hello") == "hello");
    // Mixed with dots at beginning, end, and middle
    assert(removeAllDots(".a.b.c.") == "abc");
    // Consecutive dots
    assert(removeAllDots("h..e..l..l..o") == "hello");
    // Digits and special characters
    assert(removeAllDots("1.2.3.4") == "1234");
    // Dots only at the start
    assert(removeAllDots("...xyz") == "xyz");
    // Dots only at the end
    assert(removeAllDots("xyz...") == "xyz");
    // Single dot in the middle
    assert(removeAllDots("ab.c") == "abc");
    // Single character non-dot
    assert(removeAllDots("x") == "x");
    // Large string with many dots (basic sanity check)
    std::string largeInput(1000, '.');
    assert(removeAllDots(largeInput) == "");
    largeInput[500] = 'Z';
    assert(removeAllDots(largeInput) == "Z");
    
    return 0;
}

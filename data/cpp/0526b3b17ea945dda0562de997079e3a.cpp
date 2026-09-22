/*
Write a C++ function named `reverseString` that takes a non-empty string as input and returns a new string with its characters in reverse order. The function must not modify the original input string and should work for any printable ASCII characters, including spaces and punctuation. The input string may contain leading or trailing spaces, and those should be preserved in their reversed positions. The function should be defined in a header-only style (no `main` function), and must handle edge cases such as a single-character string, strings with only spaces, and strings with repeated characters.
*/
#include <string>

// Returns a new string containing the characters of the input in reverse order.
// The input is not modified; leading/trailing spaces and punctuation are preserved.
std::string reverseString(const std::string& input) {
    std::string result;
    result.reserve(input.size()); // optional optimization to avoid reallocations

    // Iterate from the last character to the first.
    for (std::string::size_type i = input.size(); i > 0; --i) {
        result.push_back(input[i - 1]);
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared here (assuming it is in the same translation unit).
std::string reverseString(const std::string& input);

int main() {
    // Basic reversal
    assert(reverseString("hello") == "olleh");
    // Single character
    assert(reverseString("x") == "x");
    // String with spaces and punctuation
    assert(reverseString("a b c!") == "!c b a");
    // Leading/trailing spaces are preserved
    assert(reverseString("  abc  ") == "  cba  ");
    // All spaces
    assert(reverseString("   ") == "   ");
    // Repeated characters
    assert(reverseString("aaabbb") == "bbbaaa");
    // Palindrome
    assert(reverseString("racecar") == "racecar");
    // Numbers and symbols
    assert(reverseString("123$%^") == "^%$321");
    // Longer string
    assert(reverseString("C++ programming") == "gnimmargorp ++C");
    return 0;
}
// The solution is straightforward: iterate through the input string from its last character down to the first, appending each character to a result string. This can be done using a simple loop with an index, or by using the standard library’s `std::reverse` on a copy. The main algorithm is O(n) time, where n is the string length, because each character is visited exactly once. The space complexity is also O(n) because we need to store the reversed result. Edge cases: an empty string is not allowed per the task, but the implementation can handle it by returning an empty string if it occurs. For a single-character string, the reversal is just the same character. Strings with spaces or punctuation are reversed as-is, since we treat every character as a unit. The function should be `const`-correct by taking a `const std::string&` and returning a `std::string` by value. We avoid modifying the input by constructing a new string.

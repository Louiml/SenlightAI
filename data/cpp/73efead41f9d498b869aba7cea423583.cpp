Write a C++ function named `reverseCharacters` that takes a `const std::string&` and returns a new `std::string` containing the characters of the input string in reverse order. The function must preserve the original string unchanged and work correctly for empty strings, single-character strings, strings with spaces, punctuation, and Unicode-like multibyte sequences (treated as raw bytes). The solution must not use any standard library reverse algorithm or string stream; it must manually construct the reversed result using an index-based loop. The function should be declared with proper `const` correctness and include the necessary `<string>` header. The function should be self-contained and not rely on any external global state.
#include <cassert>
#include <string>

// The solution function is defined above (for brevity, it is assumed to be included here).
// Copy the solution code into this file before running the tests.

int main() {
    // Basic cases
    assert(reverseCharacters("hello") == "olleh");
    assert(reverseCharacters("") == "");
    assert(reverseCharacters("a") == "a");

    // Palindromes
    assert(reverseCharacters("racecar") == "racecar");
    assert(reverseCharacters("12321") == "12321");

    // With spaces and punctuation
    assert(reverseCharacters("hello world!") == "!dlrow olleh");
    assert(reverseCharacters("  a b  ") == "  b a  ");

    // Duplicate characters
    assert(reverseCharacters("aaabbb") == "bbbaaa");

    // Longer sentence with mixed content
    std::string input = "C++ is fun!";
    std::string expected = "!nuf si ++C";
    assert(reverseCharacters(input) == expected);
    assert(input == "C++ is fun!"); // Ensure original is unchanged

    // Numeric string
    assert(reverseCharacters("1234567890") == "0987654321");

    // Special characters
    assert(reverseCharacters("!@#$%^") == "^%$#@!");

    return 0;
}
#include <string>

// Return a new string containing the characters of the input in reverse order.
std::string reverseCharacters(const std::string& input) {
    const std::size_t n = input.size();
    std::string reversed(n, '\0');  // Preallocate and initialize with null characters.

    for (std::size_t i = 0; i < n; ++i) {
        reversed[i] = input[n - i - 1];  // Copy from the end to the beginning.
    }

    return reversed;
}
// The algorithm is straightforward: create a result string of the same length as the input, initialized with zeros (or any placeholder), then iterate from index `0` to `n-1` (where `n` is the length of the input), assigning the character at position `i` of the result to be the character at position `n-i-1` of the input. This effectively copies the last character to the first, the second-to-last to the second, and so on. Edge cases: for an empty input (`n == 0`), the result string is also empty and the loop does not execute; for a single character, the result is the same character. The algorithm uses exactly `n` assignments and no extra space beyond the result string, so time complexity is O(n) and auxiliary space is O(1) (excluding the output string, which is required). The implementation does not modify the input, ensuring const correctness. The loop is safe because index arithmetic `n-i-1` stays within `[0, n-1]` for all `i` in `[0, n-1]`.

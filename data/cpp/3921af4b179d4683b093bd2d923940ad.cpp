// Write a C++ function named `toJadenCase` that takes a non-empty string `s` consisting of lowercase and uppercase English letters and spaces (possibly with leading/trailing spaces or multiple consecutive spaces), and returns a new string where every word is converted to JadenCase format: the first character of each word is uppercase, and all other characters of that word are lowercase. A word is defined as any maximal contiguous sequence of non-space characters. Spaces in the original string must be preserved exactly as they appear (same count and position). The function signature is `std::string toJadenCase(const std::string& s);` and it must be `const`-correct. Your implementation must not use a `main` function; just provide the function and necessary headers.

// The key idea is to process the input string character by character. We always start by converting the first character to uppercase. For every subsequent character at index `i`, we look back at the previous character: if the previous character is a space, then the current character begins a new word, so we convert it to uppercase; otherwise, it is inside a word and should be converted to lowercase. This ensures that all spaces are preserved unchanged. Important edge cases include: strings with leading spaces (e.g., `"  hello"` — here the first character is a space, and our approach would convert it as is; then the first letter after the leading space becomes uppercase correctly because its previous character is a space); strings with multiple consecutive spaces (each space is passed through unchanged); strings that already contain uppercase letters (they are lowercased except when at word start); and strings that are entirely spaces (the loop simply copies them). The algorithm runs in O(n) time where `n` is the string length, using O(n) auxiliary space for the result string (since `std::string` concatenation may allocate). It uses only constant extra memory aside from the output. The implementation uses `std::toupper` and `std::tolower` which are safe for ASCII characters, but note they expect an `int` and return an `int`; casting to `char` is fine for typical inputs.

#include <string>
#include <cctype>

// Convert a string to JadenCase: first letter of each word uppercase, rest lowercase, spaces preserved.
std::string toJadenCase(const std::string& s) {
    if (s.empty()) {
        return s;
    }
    std::string result;
    result.reserve(s.size());

    // First character: always uppercase (even if it's a space, toupper on space is space).
    result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(s[0]))));

    for (std::size_t i = 1; i < s.size(); ++i) {
        if (s[i - 1] == ' ') {
            // Start of a new word: uppercase.
            result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(s[i]))));
        } else {
            // Inside a word: lowercase.
            result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(s[i]))));
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is assumed to be defined above.

int main() {
    assert(toJadenCase("hello world") == "Hello World");
    assert(toJadenCase("a") == "A");
    assert(toJadenCase("HELLO") == "Hello");
    assert(toJadenCase("  multiple   spaces  ") == "  Multiple   Spaces  ");
    assert(toJadenCase("") == "");
    assert(toJadenCase("   ") == "   ");
    assert(toJadenCase("tEsT 123 abc") == "Test 123 Abc");
    assert(toJadenCase("one two") == "One Two");
    assert(toJadenCase("already Mixed CASE") == "Already Mixed Case");
    assert(toJadenCase("x y z") == "X Y Z");
    return 0;
}

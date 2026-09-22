Write a C++ function that takes a non-empty string `word` consisting of lowercase English letters and a character `ch`, and returns a new string where the prefix of `word` up to and including the first occurrence of `ch` is reversed, while the suffix after that first `ch` remains unchanged. If `ch` does not appear in `word`, return the original string unchanged. For example, given `word = "abcdefd"` and `ch = 'd'`, the prefix `"abcd"` is reversed to `"dcba"`, yielding `"dcbaefd"`. The function should handle cases where `ch` appears multiple times (reverse only up to the first occurrence) and where `ch` is the last character (the suffix is empty).
// The solution scans the string from left to right, building the reversed prefix incrementally. For each character `c` at index `i`, prepend `c` to a `reversedPrefix` string (so after processing a few characters, `reversedPrefix` holds the characters seen so far in reverse order). When we encounter `c == ch`, we stop and return `reversedPrefix + word.substr(i + 1)`. If we finish the loop without finding `ch`, we return the original `word`. Edge cases: if `ch` is the first character, the reversed prefix is just that character, so the result equals the original string; if `ch` is not present, we return `word` unchanged. Time complexity is O(n) because we traverse the string once and construct the result via concatenation (each character is copied at most twice). Space complexity is O(n) for the reversed prefix and the final result string, plus O(1) auxiliary space.
#include <string>

// Reverses the prefix of `word` up to and including the first occurrence of `ch`.
// If `ch` is not present, returns the original word.
std::string reversePrefix(const std::string& word, char ch) {
    std::string reversedPrefix;
    const std::size_t n = word.size();

    for (std::size_t i = 0; i < n; ++i) {
        reversedPrefix = word[i] + reversedPrefix;  // build in reverse order
        if (word[i] == ch) {
            return reversedPrefix + word.substr(i + 1);
        }
    }

    // ch not found, return original unchanged
    return word;
}
#include <cassert>
#include <string>

// Forward declaration for the solution function (already defined above).
std::string reversePrefix(const std::string& word, char ch);

int main() {
    // Basic case
    assert(reversePrefix("abcdefd", 'd') == "dcbaefd");
    // ch at the beginning
    assert(reversePrefix("xyxz", 'x') == "xyxz");
    // ch at the end
    assert(reversePrefix("hello", 'o') == "olleh");
    // ch appears multiple times, only first reversed
    assert(reversePrefix("abacaba", 'a') == "abacaba");
    // ch not present
    assert(reversePrefix("abcd", 'z') == "abcd");
    // single character word
    assert(reversePrefix("a", 'a') == "a");
    // single character word, ch not present
    assert(reversePrefix("b", 'a') == "b");
    // longer string, ch in middle
    assert(reversePrefix("abcdefgh", 'e') == "edcbafgh");
    // all characters reversed? (ch at end)
    assert(reversePrefix("abc", 'c') == "cba");
    return 0;
}

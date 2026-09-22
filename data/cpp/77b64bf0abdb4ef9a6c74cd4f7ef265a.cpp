Write a C++ function `reverseWords` that takes a string `s` containing words separated by single or multiple spaces (and possibly leading/trailing spaces) and returns a new string where each word is reversed in place, while the order of the words and the exact spacing pattern (including leading and trailing spaces) are preserved. For example, `"hello world"` becomes `"olleh dlrow"`, and `"  abc   def  "` becomes `"  cba   fed  "`. Assume the input consists only of printable ASCII characters (no tabs or newlines). The function must be efficient, not use additional string storage beyond the output string, and handle empty input correctly.
// The algorithm scans the input string from left to right. For each maximal contiguous block of non-space characters (a word), it records the starting index `start`. It then advances the index `i` until it hits a space or the end of the string. At that point, the word is from `start` to `i-1`. To reverse it, we iterate `p` from `start` to `i-1` and push `s[start + i - 1 - p]` onto the result string—this effectively copies the characters in reverse order. After finishing the word, we skip any consecutive spaces (`while (i < length && s[i] == ' ')`) and for each space we push a space onto the result. This preserves the exact spacing between words, including leading and trailing spaces, because if the string starts with spaces, the first while loop will immediately enter the space-skipping branch (since `i` is at 0 and `s[0]` is a space) and push those spaces onto the result. The loop continues until all characters are processed. Edge cases: empty string returns empty string; a string with only spaces returns the same spaces; single word with no spaces works because after the inner word loop, the space-skipping while sees the end of string and does nothing. Time complexity is O(n) where n is the length of the string, because each character is visited a constant number of times. Space complexity is O(n) for the result string, but no extra auxiliary data structures are used beyond the output and input strings.
#include <string>

/**
 * Reverses each word in the input string while preserving word order
 * and the exact spacing pattern (including leading/trailing spaces).
 *
 * @param s The input string containing words separated by spaces.
 * @return A new string where every maximal block of non-space characters
 *         is reversed, with all spaces kept in their original positions.
 */
std::string reverseWords(const std::string& s) {
    std::string result;
    const std::size_t length = s.length();
    std::size_t i = 0;

    while (i < length) {
        std::size_t start = i;

        // Advance to the end of the current word (or stay if spaces).
        while (i < length && s[i] != ' ') {
            ++i;
        }

        // Copy the current word in reverse order.
        for (std::size_t p = start; p < i; ++p) {
            result.push_back(s[start + i - 1 - p]);
        }

        // Copy all consecutive spaces after the word (or before first word).
        while (i < length && s[i] == ' ') {
            result.push_back(' ');
            ++i;
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the solution function (provided above).
std::string reverseWords(const std::string& s);

int main() {
    // Basic case with single spaces.
    assert(reverseWords("hello world") == "olleh dlrow");

    // Multiple consecutive spaces between words.
    assert(reverseWords("  abc   def  ") == "  cba   fed  ");

    // Leading and trailing spaces.
    assert(reverseWords("   one two   ") == "   eno owt   ");

    // Single word.
    assert(reverseWords("programming") == "gnimmargorp");

    // Single character words.
    assert(reverseWords("a b c") == "a b c");

    // Empty string.
    assert(reverseWords("") == "");

    // Only spaces.
    assert(reverseWords("     ") == "     ");

    // Mixed punctuation, numbers, and letters (no special handling needed).
    assert(reverseWords("ab12 cd34") == "21ba 43dc");

    // Very short words with spaces at ends.
    assert(reverseWords(" x ") == " x ");

    // Long sentence with irregular spacing.
    assert(reverseWords("The quick  brown fox") == "ehT kciuq  nworb xof");

    return 0;
}

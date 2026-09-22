// Write a C++ function that takes a non-empty string `s` (which may contain leading and/or trailing whitespace and consists only of lowercase English letters and spaces) and returns the length of the last word in the string. A word is defined as a maximal contiguous sequence of non-space characters. For example, for the input `"Hello World"` the function must return `5` (the length of `"World"`). The function must correctly handle cases where the string ends with spaces, such as `" fly me to the moon   "` returning `4`, and where the string contains only one word, such as `"a"` returning `1`. You must not use external libraries beyond the standard C++ headers, and your solution should be efficient and avoid unnecessary copies of the input string.
#include <cassert>
#include <string>

// Forward declaration of the function under test.
int lengthOfLastWord(const std::string& s);

int main() {
    // Basic examples
    assert(lengthOfLastWord("Hello World") == 5);
    assert(lengthOfLastWord("a") == 1);
    assert(lengthOfLastWord("aas") == 3);
    // Trailing spaces
    assert(lengthOfLastWord(" fly me to the moon   ") == 4);
    assert(lengthOfLastWord("word   ") == 4);
    // Leading spaces
    assert(lengthOfLastWord("   hello") == 5);
    // Multiple internal spaces
    assert(lengthOfLastWord("one  two   three") == 5);
    // All spaces
    assert(lengthOfLastWord("     ") == 0);
    // Empty string (though task says non-empty, still test)
    assert(lengthOfLastWord("") == 0);
    // Single character with spaces around
    assert(lengthOfLastWord("   x   ") == 1);
    // Long word at the end
    assert(lengthOfLastWord("short length") == 6);
    return 0;
}
#include <string>

// Returns the length of the last word in the given string.
// A word is a maximal contiguous sequence of non-space characters.
// If the string contains no words (all spaces), returns 0.
int lengthOfLastWord(const std::string& s) {
    // Find the position of the last non-space character.
    // If the string is all spaces or empty, find_last_not_of returns npos.
    std::size_t last_non_space = s.find_last_not_of(' ');
    if (last_non_space == std::string::npos) {
        return 0;  // No words exist.
    }

    int count = 0;
    // Iterate backward from the last non-space character until a space or the beginning.
    for (std::size_t i = last_non_space; i != std::string::npos && s[i] != ' '; --i) {
        ++count;
        // Stop when i reaches 0 to avoid underflow when decrementing.
        if (i == 0) {
            break;
        }
    }
    return count;
}
// The core idea is to traverse the string from the end to the beginning, skipping any trailing spaces, and then count characters until either the beginning of the string or the next space is encountered. First, find the index of the last non-space character using `find_last_not_of(' ')`; if no such character exists (i.e., the string is all spaces), the result should be 0. Then, starting from that index, iterate backward one position at a time, incrementing a counter, and stop when either the index goes below 0 or the character at that index is a space. This approach avoids reversing the string or splitting it, and it handles leading/trailing/multiple spaces naturally. Edge cases include: an all-space string (return 0), a single word with no spaces, a string with trailing spaces (the trailing spaces are skipped first), and a string with only one character. Time complexity is O(n) where n is the length of the string, because each character is examined at most twice (once in `find_last_not_of` and once in the counting loop). Space complexity is O(1) auxiliary, aside from the input string itself.

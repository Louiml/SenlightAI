// Write a C++ function named `lengthOfLastWord` that takes a `const std::string&` as input (which may contain letters, digits, or other characters, and may have leading, trailing, or multiple internal spaces) and returns the length of the last word in the string. A "word" is defined as a maximal contiguous sequence of non-space characters, where spaces are ASCII `' '` (0x20). If the string is empty or contains only spaces, the function should return 0. The function must be const-correct, handle any string length (including very large inputs), and must not modify the input string. You may assume the input contains only printable ASCII characters (no tabs or newlines).
#include <cassert>
#include <string>

// Forward declaration of the function under test
int lengthOfLastWord(const std::string& s);

int main() {
    // Basic case with trailing spaces
    assert(lengthOfLastWord("Hello World") == 5);
    assert(lengthOfLastWord("Hello World   ") == 5);
    
    // Empty string and all spaces
    assert(lengthOfLastWord("") == 0);
    assert(lengthOfLastWord("   ") == 0);
    
    // Single word without spaces
    assert(lengthOfLastWord("hello") == 5);
    assert(lengthOfLastWord("a") == 1);
    
    // Multiple internal spaces
    assert(lengthOfLastWord("   fly me   to   the moon  ") == 4);
    
    // Word with digits or punctuation (non-space characters)
    assert(lengthOfLastWord("luffy is still joyboy") == 6);
    assert(lengthOfLastWord("hello123") == 8);
    
    // Leading spaces only
    assert(lengthOfLastWord("   hello") == 5);
    
    // Large word at the end
    assert(lengthOfLastWord("short verylongword") == 13);
    
    return 0;
}
#include <string>

// Returns the length of the last word in the input string.
// A word is a maximal sequence of non-space characters.
// Returns 0 if the string is empty or contains only spaces.
int lengthOfLastWord(const std::string& s) {
    // Find the last non-space character (or npos if none)
    size_t lastNonSpace = s.find_last_not_of(' ');
    if (lastNonSpace == std::string::npos) {
        return 0; // string is empty or all spaces
    }
    
    // Find the last space before that position (or npos if no space exists)
    size_t lastSpaceBefore = s.find_last_of(' ', lastNonSpace);
    
    // If no space found, the word starts at index 0; otherwise start after the space
    if (lastSpaceBefore == std::string::npos) {
        return static_cast<int>(lastNonSpace + 1); // word length from index 0 to lastNonSpace inclusive
    } else {
        return static_cast<int>(lastNonSpace - lastSpaceBefore);
    }
}
// The algorithm first trims trailing spaces by advancing an index from the end of the string backward while the character at that index is a space. If the entire string consists of spaces, the index becomes -1 and we return 0. Otherwise, we have found the end of the last word. We then continue moving the index backward until we hit a space or reach the beginning of the string, counting the characters as we go. Alternatively, we can find the first space to the left of the last non-space character using `rfind` on the substring before the last non-space character. The simpler robust approach: use `find_last_not_of(' ')` to get the index of the last non-space character, then `find_last_of(' ', that_index)` to find the space just before the word (or `npos` if none). The length is `lastNonSpace - lastSpaceBefore` where if the space is not found, we use the position `-1` effectively (i.e., the word starts at index 0). Edge cases: empty string (return 0), all spaces (return 0), single word without spaces (return entire length), word followed by trailing spaces (trim them), and multiple spaces between words. Time complexity is O(n) in the worst case (single scan of the string for `find_last_not_of` and another for `find_last_of`), but both are linear; space complexity is O(1) extra (excluding input).

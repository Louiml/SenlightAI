// Write a C++ function named `ignoreNonAlnumAndCheckPalindrome` that takes a `const std::string&` parameter and returns a `bool` indicating whether the string is a palindrome when considering only alphanumeric characters (letters and digits) and ignoring case differences. Non-alphanumeric characters (punctuation, spaces, symbols) must be completely skipped. The function must handle empty strings (return `true`), strings with only non-alphanumeric characters (return `true`), mixed-case letters, and digits. For example, `"A man, a plan, a canal: Panama"` should return `true`, while `"race a car"` should return `false`. The function must not rely on any external libraries beyond the standard C++ headers, and must use constant auxiliary space (no additional containers for filtered characters).

// The approach uses two pointers, `left` starting at index 0 and `right` starting at `s.size()-1`. While `left < right`, we advance `left` forward past any characters that are not alphanumeric (`std::isalnum`), and similarly advance `right` backward past non-alphanumeric characters. Careful bounds checking ensures we don’t go out of range when the string has many non-alphanumerics. Once both pointers point to alphanumeric characters, we compare their lowercase versions using `std::tolower`. If they differ, return `false` immediately. If they match, move both pointers inward (`left++`, `right--`) and continue. If the loop completes without mismatches, return `true`. Edge cases: empty string (loop never runs, returns true), string with no alphanumerics (both pointers will skip all and eventually `left >= right`, returns true), and single-character strings (loop condition fails, true). Time complexity is O(n) where n is the string length, as each character is visited at most once across both pointers. Space complexity is O(1) beyond the input string itself.

#include <string>
#include <cctype>

// Returns true if the alphanumeric characters of s (ignoring case) form a palindrome.
// Non-alphanumeric characters are skipped. Empty or all-non-alphanumeric strings are true.
bool ignoreNonAlnumAndCheckPalindrome(const std::string& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;
    
    while (left < right) {
        // Skip non-alphanumeric characters from the left
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) {
            ++left;
        }
        // Skip non-alphanumeric characters from the right
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) {
            --right;
        }
        // Compare lowercase versions of characters
        if (std::tolower(static_cast<unsigned char>(s[left])) != 
            std::tolower(static_cast<unsigned char>(s[right]))) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}

#include <cassert>
#include <string>

// Declaration of the function under test
bool ignoreNonAlnumAndCheckPalindrome(const std::string& s);

int main() {
    // Common palindromic phrases with punctuation and spaces
    assert(ignoreNonAlnumAndCheckPalindrome("A man, a plan, a canal: Panama") == true);
    assert(ignoreNonAlnumAndCheckPalindrome("race a car") == false);
    assert(ignoreNonAlnumAndCheckPalindrome("No 'x' in Nixon") == true);
    
    // Digits and mixed case
    assert(ignoreNonAlnumAndCheckPalindrome("0P") == false);
    assert(ignoreNonAlnumAndCheckPalindrome("1a1") == true);
    assert(ignoreNonAlnumAndCheckPalindrome("A1b2B1a") == true);
    
    // Edge cases: empty, only non-alphanumeric, single char
    assert(ignoreNonAlnumAndCheckPalindrome("") == true);
    assert(ignoreNonAlnumAndCheckPalindrome("!!! *** ???") == true);
    assert(ignoreNonAlnumAndCheckPalindrome("z") == true);
    
    // Multiple spaces and punctuation at boundaries
    assert(ignoreNonAlnumAndCheckPalindrome("  Able was I ere I saw Elba  ") == true);
    assert(ignoreNonAlnumAndCheckPalindrome("hello") == false);
    
    return 0;
}

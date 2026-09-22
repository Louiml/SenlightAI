Write a C++ function named `isPalindrome` that takes a `std::string` parameter and returns a `bool` indicating whether the string is a palindrome (reads the same forwards and backwards), ignoring case and considering only alphanumeric characters. The function should treat empty strings and strings with only non-alphanumeric characters as palindromes. Non-alphanumeric characters (spaces, punctuation, symbols) should be ignored entirely. The function must be case-insensitive (e.g., 'A' matches 'a'). Examples: `"A man, a plan, a canal: Panama"` returns `true`; `"race a car"` returns `false`; `"ab_a"` returns `true`.

// The solution iterates through the string using two indices: one starting from the beginning (`left`) and one from the end (`right`). At each step, we skip any non-alphanumeric characters by advancing `left` forward and `right` backward until each points to a valid alphanumeric character (or until the indices cross). Then compare the two characters after converting them to lowercase using `std::tolower`. If they differ, return `false`. If all comparisons pass until the indices meet or cross, the string is a palindrome. Edge cases include: empty string (immediately returns true), string with only non-alphanumeric characters (loop skips past all, returns true), and mixing uppercase/lowercase (handled by `tolower`). Time complexity is O(n) where n is the string length, because each character is visited at most once by either pointer. Space complexity is O(1) since we only use a few integer indices and temporary characters.

#include <string>
#include <cctype>

// Checks if the given string is a palindrome, ignoring case and non-alphanumeric characters.
bool isPalindrome(const std::string& s) {
    int left = 0;
    int right = static_cast<int>(s.length()) - 1;

    while (left < right) {
        // Skip non-alphanumeric characters from the left.
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) {
            ++left;
        }
        // Skip non-alphanumeric characters from the right.
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) {
            --right;
        }
        // Compare characters case-insensitively.
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

int main() {
    assert(isPalindrome("A man, a plan, a canal: Panama") == true);
    assert(isPalindrome("race a car") == false);
    assert(isPalindrome("ab_a") == true);
    assert(isPalindrome("") == true);
    assert(isPalindrome("   ") == true);
    assert(isPalindrome("No 'x' in Nixon") == true);
    assert(isPalindrome("hello") == false);
    assert(isPalindrome("a") == true);
    assert(isPalindrome("Madam, I'm Adam") == true);
    assert(isPalindrome("0P") == false);
    return 0;
}

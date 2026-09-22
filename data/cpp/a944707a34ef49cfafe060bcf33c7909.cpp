Write a C++ function named `isPalindrome` that takes a C-style string (null-terminated character array) as input and returns a boolean value indicating whether the string is a palindrome. A palindrome is a string that reads the same forward and backward, ignoring case sensitivity. For example, "Racecar" and "level" are palindromes, while "hello" and "noon " (with a trailing space) are not. The function must handle empty strings and strings with only whitespace appropriately: an empty string (first character is `'\0'`) is considered a palindrome, while a string consisting solely of spaces or other whitespace (e.g., "   ") is also considered a palindrome (since ignoring case and whitespace, it reads the same forward and backward). However, any string containing at least one non-whitespace character must be checked for palindrome status case-insensitively. The function should use `const` correctly and not modify the input. Do not use any standard library functions other than `strlen` and `tolower` (you may include `<cctype>` and `<cstring>`). Provide the implementation as a standalone free function.
// The main algorithm uses two pointers: one starting at the beginning (index 0) and one at the end (index `strlen(str) - 1`). While the left index is less than the right index, compare the characters after converting both to lowercase using `tolower`. If at any point they differ, return `false`. If the loop completes without mismatches, return `true`. Edge cases: an empty string (length 0) trivially returns `true` because the while loop condition `left < right` fails immediately. A string of only spaces also returns `true` because all characters are equal when lowercased (spaces are unchanged by `tolower`), so the palindrome check passes. However, note that the original code incorrectly checks for `str != " "` which compares addresses, not contents—our improvement avoids that mistake. Time complexity is O(n) where n is the length of the string, because each character is examined at most once. Space complexity is O(1) auxiliary, since only two integer indices are used. The function is `const`-correct because it only reads the input.
#include <cctype>
#include <cstring>

// Returns true if the given null-terminated string is a palindrome,
// ignoring case. Empty strings and strings with only whitespace are palindromes.
bool isPalindrome(const char str[]) {
    if (str == nullptr) {
        return false;
    }
    int left = 0;
    int right = static_cast<int>(strlen(str)) - 1;

    while (left < right) {
        if (std::tolower(static_cast<unsigned char>(str[left])) !=
            std::tolower(static_cast<unsigned char>(str[right]))) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}
#include <cassert>

int main() {
    // Basic palindromes
    assert(isPalindrome("racecar") == true);
    assert(isPalindrome("Racecar") == true);
    assert(isPalindrome("level") == true);
    
    // Non-palindromes
    assert(isPalindrome("hello") == false);
    assert(isPalindrome("noon ") == false); // trailing space makes it not a palindrome
    
    // Edge cases: empty and whitespace-only
    assert(isPalindrome("") == true);
    assert(isPalindrome("   ") == true);
    
    // Single character (always palindrome)
    assert(isPalindrome("A") == true);
    
    // Mixed case and spaces inside
    assert(isPalindrome("A man a plan a canal Panama") == false); // spaces break it, not ignored
    assert(isPalindrome("abccba") == true);
    
    // Longer string
    assert(isPalindrome("tattarrattat") == true);
    
    return 0;
}

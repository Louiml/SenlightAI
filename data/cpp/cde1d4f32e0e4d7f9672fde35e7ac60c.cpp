Write a C++ function `isPalindromeAlphaNumeric` that takes a `std::string` (which may contain uppercase/lowercase letters, digits, spaces, punctuation, and other non-alphanumeric characters) and returns `true` if the string is a palindrome considering only alphanumeric characters and ignoring case, and `false` otherwise. An empty string or a string with no alphanumeric characters should be considered a palindrome. The function must be `const`-correct, handle the full ASCII range (including characters like `'0'`–`'9'`, `'A'`–`'Z'`, `'a'`–`'z'`), and not modify the input string.

#include <cassert>

int main() {
    assert(isPalindromeAlphaNumeric("A man, a plan, a canal: Panama") == true);
    assert(isPalindromeAlphaNumeric("race a car") == false);
    assert(isPalindromeAlphaNumeric("") == true);
    assert(isPalindromeAlphaNumeric("   ") == true);
    assert(isPalindromeAlphaNumeric(".,!?") == true);
    assert(isPalindromeAlphaNumeric("0P") == false);
    assert(isPalindromeAlphaNumeric("12321") == true);
    assert(isPalindromeAlphaNumeric("AbcBa") == true);
    assert(isPalindromeAlphaNumeric("a") == true);
    assert(isPalindromeAlphaNumeric("ab") == false);
}

#include <cctype>
#include <string>

// Returns true if the alphanumeric characters in s form a palindrome, ignoring case.
bool isPalindromeAlphaNumeric(const std::string& s) {
    int i = 0;
    int j = static_cast<int>(s.size()) - 1;

    while (i < j) {
        if (!std::isalnum(static_cast<unsigned char>(s[i]))) {
            ++i;
        } else if (!std::isalnum(static_cast<unsigned char>(s[j]))) {
            --j;
        } else if (std::tolower(static_cast<unsigned char>(s[i])) !=
                   std::tolower(static_cast<unsigned char>(s[j]))) {
            return false;
        } else {
            ++i;
            --j;
        }
    }
    return true;
}

// The solution uses a two-pointer approach with indices `i` starting at the beginning and `j` starting at the end of the string. In each loop iteration, if the character at `i` is not alphanumeric (checked via `std::isalnum`), increment `i`; if the character at `j` is not alphanumeric, decrement `j`; otherwise, compare the lowercase versions of both characters using `std::tolower`. If they differ, return `false`; if they match, move both pointers inward. The loop continues until the pointers meet or cross. This correctly handles multiple consecutive non-alphanumeric characters and ignores case. Edge cases include: empty string (loop doesn't execute, returns `true`), strings with only non-alphanumeric characters (pointers skip everything, eventually meet, returns `true`), mixed-case letters, digits (which are case-insensitive anyway), and strings where the central character is non-alphanumeric. Time complexity is O(n) because each character is visited at most once per pointer, and space complexity is O(1) aside from the input string itself.

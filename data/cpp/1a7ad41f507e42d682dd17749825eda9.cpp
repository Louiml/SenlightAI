Write a C++ function named `isPalindromeIgnoreCase` that takes a non-empty string as input and returns `true` if the string is a palindrome when considering only alphanumeric characters and ignoring case, and `false` otherwise. The input may contain spaces, punctuation, digits, and uppercase/lowercase letters. For example, `"A man, a plan, a canal: Panama"` should return `true`, while `"race a car"` should return `false`. The function must handle empty strings (return `true`), single characters (return `true`), and strings with no alphanumeric characters (return `true`). Use `std::isalnum` and `std::tolower` from `<cctype>` for character classification and normalization. The function should be const-correct and not modify the input string.
The solution uses two pointers: one starting at the beginning (`left`) and one at the end (`right`) of the string. While `left` is less than `right`, we skip any characters that are not alphanumeric by advancing the pointers inward. Once both pointers land on alphanumeric characters, we compare them after converting to lowercase using `std::tolower` (casting to `unsigned char` to avoid undefined behavior for negative `char` values). If they differ, return `false`. If all comparisons pass, return `true`. Edge cases: an empty string or a string with only non-alphanumerics trivially passes because the loop never finds mismatched alphanumerics; a single alphanumeric character also passes. Time complexity is O(n) where n is the string length, since each character is visited at most once by the two pointers. Space complexity is O(1) as only a few integer variables are used, no extra data structures.
#include <string>
#include <cctype>

// Returns true if the input string is a palindrome considering only
// alphanumeric characters and ignoring case. Empty strings and strings
// with no alphanumerics are considered palindromes.
bool isPalindromeIgnoreCase(const std::string& s) {
    int left = 0;
    int right = static_cast<int>(s.size()) - 1;

    while (left < right) {
        // Move left pointer to the next alphanumeric character
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[left]))) {
            ++left;
        }
        // Move right pointer to the previous alphanumeric character
        while (left < right && !std::isalnum(static_cast<unsigned char>(s[right]))) {
            --right;
        }

        if (left >= right) {
            break;
        }

        char leftChar = static_cast<char>(std::tolower(static_cast<unsigned char>(s[left])));
        char rightChar = static_cast<char>(std::tolower(static_cast<unsigned char>(s[right])));

        if (leftChar != rightChar) {
            return false;
        }

        ++left;
        --right;
    }

    return true;
}
#include <cassert>
#include <string>

// Function declaration (placed here for the test)
bool isPalindromeIgnoreCase(const std::string& s);

int main() {
    // Basic palindromes
    assert(isPalindromeIgnoreCase("racecar") == true);
    assert(isPalindromeIgnoreCase("RaceCar") == true);
    // With spaces, punctuation, and mixed case
    assert(isPalindromeIgnoreCase("A man, a plan, a canal: Panama") == true);
    // Not a palindrome
    assert(isPalindromeIgnoreCase("race a car") == false);
    // Digits and case-insensitivity
    assert(isPalindromeIgnoreCase("0P") == false);
    assert(isPalindromeIgnoreCase("A1b2B1a") == true);
    // Edge cases: empty, single char, and only non-alphanumerics
    assert(isPalindromeIgnoreCase("") == true);
    assert(isPalindromeIgnoreCase("a") == true);
    assert(isPalindromeIgnoreCase("!!!") == true);
    // Uppercase letters only
    assert(isPalindromeIgnoreCase("A") == true);
    assert(isPalindromeIgnoreCase("ABBA") == true);
    assert(isPalindromeIgnoreCase("ABAB") == false);
    // Mixed alphanumeric and punctuation
    assert(isPalindromeIgnoreCase("No 'x' in Nixon") == true);
    assert(isPalindromeIgnoreCase("Was it a car or a cat I saw?") == true);
    return 0;
}

Write a C++ function named `isPalindromeIgnoreCase` that takes a null-terminated character array (C-string) and its logical size (not including the null terminator) as parameters, and returns a `bool` indicating whether the string reads the same forward and backward when case differences are ignored. The function must only consider alphabetic characters; all non-alphabetic characters (digits, punctuation, spaces, etc.) should be ignored entirely when comparing characters from the two ends. For example, the string `"A man, a plan, a canal: Panama"` should be considered a palindrome once non-letters are removed and case is normalized. The input array is guaranteed to be non-empty and null-terminated. Your solution must not modify the input array and must work with any valid C-string, including those with uppercase, lowercase, and mixed non-alphabetic characters. Provide a standalone implementation without a `main` function, and then write test assertions in a separate `main` function that directly calls your function with representative inputs.

// The algorithm uses two indices, `left` starting at 0 and `right` starting at `size - 1`, and iterates until they cross. At each step, advance `left` forward while the character at `left` is not alphabetic (using `std::isalpha`), and advance `right` backward while the character at `right` is not alphabetic. Once both indices point to alphabetic characters, compare them case-insensitively by converting both to lowercase using a helper or `std::tolower`. If they differ, return `false`. If they match, move both indices inward by one and continue. If the loop completes without mismatch, return `true`. Edge cases include strings that become empty after ignoring non-letters (should return `true` because an empty sequence is vacuously a palindrome), single alphabetic character, all non-alphabetic characters, and mixed case with punctuation. The time complexity is O(n) where n is the string length, because each character is examined at most twice. Auxiliary space is O(1), aside from the input itself. The function must use `const char*` or `const char[]` to avoid modification and should include `#include <cctype>` and `#include <cstddef>`.

#include <cctype>
#include <cstddef>

// Return true if the C-string str (with logical size size) is a palindrome,
// ignoring case and ignoring all non-alphabetic characters.
bool isPalindromeIgnoreCase(const char str[], std::size_t size) {
    std::size_t left = 0;
    std::size_t right = (size > 0) ? size - 1 : 0;

    while (left < right) {
        // Advance left past non-alphabetic characters
        while (left < right && !std::isalpha(static_cast<unsigned char>(str[left]))) {
            ++left;
        }
        // Advance right past non-alphabetic characters
        while (left < right && !std::isalpha(static_cast<unsigned char>(str[right]))) {
            --right;
        }
        // If we crossed, all remaining characters are non-alphabetic => palindrome
        if (left >= right) {
            break;
        }
        // Compare case-insensitively
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
#include <cstddef>

// Assume the solution function is declared above or included here.
// For the test, we only need the function's implementation available.

int main() {
    // Basic palindrome with same case
    char s1[] = "racecar";
    assert(isPalindromeIgnoreCase(s1, 7) == true);

    // Mixed case palindrome
    char s2[] = "AbBa";
    assert(isPalindromeIgnoreCase(s2, 4) == true);

    // Non-palindrome
    char s3[] = "hello";
    assert(isPalindromeIgnoreCase(s3, 5) == false);

    // Ignoring non-alphabetic characters and case
    char s4[] = "A man, a plan, a canal: Panama";
    assert(isPalindromeIgnoreCase(s4, 30) == true);

    // All non-alphabetic characters -> empty after filtering -> true
    char s5[] = "1234!@#";
    assert(isPalindromeIgnoreCase(s5, 7) == true);

    // Single character
    char s6[] = "Z";
    assert(isPalindromeIgnoreCase(s6, 1) == true);

    // Mixed case and punctuation, non-palindrome after filtering
    char s7[] = "No 'x' in Nixon?";
    assert(isPalindromeIgnoreCase(s7, 17) == true);

    // Edge: crossing with only non-alphabetic at ends
    char s8[] = "?a!";
    assert(isPalindromeIgnoreCase(s8, 3) == true);

    // Non-palindrome with punctuation
    char s9[] = "a,b!c";
    assert(isPalindromeIgnoreCase(s9, 5) == false);

    // Long palindrome with digits and spaces
    char s10[] = "Was it a car or a cat I saw?";
    assert(isPalindromeIgnoreCase(s10, 27) == true);

    return 0;
}

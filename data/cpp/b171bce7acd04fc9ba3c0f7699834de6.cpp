Write a C++ function named `isPalindrome` that takes a `const std::string&` as its only parameter and returns a `bool` indicating whether the input string is a palindrome (reads the same forward and backward, ignoring case). The function must handle empty strings (which should be considered palindromes) and single-character strings (also palindromes). It must treat uppercase and lowercase letters as equivalent (e.g., `"Racecar"` and `"rAcEcAr"` are palindromes), but all other characters (spaces, digits, punctuation) must be compared exactly as they appear. For example, `"A man, a plan, a canal: Panama"` is **not** a palindrome because spaces, commas, and colons differ in reverse order. The function must not modify the input and must use only `const` references. Provide the implementation in a self-contained header-usable format (with `#include <string>` and `#include <cctype>`), but do not include a `main` function.
// The solution uses two indices, one starting at the beginning (`left = 0`) and one at the end (`right = length - 1`). While `left < right`, compare the characters at those positions after converting both to lowercase using `std::tolower` (cast to `unsigned char` to avoid undefined behavior for negative `char` values). If they differ, return `false` immediately. If they match, increment `left` and decrement `right`. Continue until the indices meet or cross, then return `true`. Edge cases: empty string and single-character string naturally return `true` because the loop never executes or exits immediately. Case-insensitivity is handled by the `tolower` conversion; non-alphabetic characters are unchanged by `tolower`, so they must match exactly. The time complexity is \(O(n)\) where \(n\) is the string length, and space complexity is \(O(1)\) since no extra storage is used beyond the indices.
#include <string>
#include <cctype>

// Returns true if the input string is a palindrome ignoring case, false otherwise.
bool isPalindrome(const std::string& text) {
    size_t left = 0;
    size_t right = text.size();

    // If empty, right becomes 0, so loop condition fails immediately.
    while (left < right) {
        char leftChar = static_cast<char>(std::tolower(static_cast<unsigned char>(text[left])));
        char rightChar = static_cast<char>(std::tolower(static_cast<unsigned char>(text[right - 1])));
        if (leftChar != rightChar) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}
#include <cassert>

int main() {
    // Basic cases
    assert(isPalindrome("racecar") == true);
    assert(isPalindrome("hello") == false);
    
    // Case-insensitive
    assert(isPalindrome("Racecar") == true);
    assert(isPalindrome("rAcEcAr") == true);
    
    // Empty and single character
    assert(isPalindrome("") == true);
    assert(isPalindrome("a") == true);
    
    // Non-alphabetic characters are compared exactly
    assert(isPalindrome("12321") == true);
    assert(isPalindrome("12 21") == true);
    assert(isPalindrome("A man a plan a canal Panama") == false); // spaces differ in reverse
    
    // Mixed alphanumeric
    assert(isPalindrome("A1b2B1a") == true);
    assert(isPalindrome("A1b2b1a") == true);
    assert(isPalindrome("abc") == false);
    
    // Longer odd/even palindromes
    assert(isPalindrome("Never odd or even") == false); // spaces reversed differ
    assert(isPalindrome("Neveroddoreven") == true);
    
    return 0;
}

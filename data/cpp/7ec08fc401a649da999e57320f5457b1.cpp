Write a C++ function named `isPalindromeIgnoreNonAlnum` that takes a `const std::string&` parameter and returns a `bool` indicating whether the input string is a palindrome when considering only alphanumeric characters and ignoring case. The function should ignore all non-alphanumeric characters (e.g., spaces, punctuation, symbols) and treat uppercase and lowercase letters as equivalent. The function must handle empty strings (which are trivially palindromes), strings with only non-alphanumeric characters, and strings with mixed alphanumeric and symbolic content. Provide a clean implementation with proper `const` correctness, using only standard library facilities.
// The main algorithm processes the input string in two stages. First, build a filtered string that contains only alphanumeric characters, converting each to lowercase using `std::tolower` (with `unsigned char` cast to avoid undefined behavior for negative `char` values). Then, compare the filtered string with its reverse using `std::equal` or by creating a reversed copy and comparing. Since we only need a boolean result, we can avoid an extra copy by using two iterators: one from the beginning and one from the end of the filtered string, checking equality pairwise until they meet. This gives an \(O(n)\) time complexity and \(O(n)\) auxiliary space for the filtered string (which is necessary because we must store the filtered content). Edge cases: empty input returns `true`; input with no alphanumeric characters produces an empty filtered string, which is a palindrome; case differences are normalized; Unicode characters outside the standard ASCII alphanumeric set are generally not considered alphanumeric by `std::isalnum` when using the default C locale, so only ASCII letters and digits are processed, which is acceptable for typical tasks. Time complexity is linear in the length of the input string; space complexity is linear in the length of the filtered string (which is at most the input length).
#include <string>
#include <cctype>

// Returns true if the input string is a palindrome considering only alphanumeric
// characters and ignoring case. Non-alphanumeric characters are ignored.
bool isPalindromeIgnoreNonAlnum(const std::string& str) {
    std::string filtered;
    filtered.reserve(str.size());
    
    for (char ch : str) {
        unsigned char c = static_cast<unsigned char>(ch);
        if (std::isalnum(c)) {
            filtered.push_back(static_cast<char>(std::tolower(c)));
        }
    }
    
    // Compare first and last, then move inward
    size_t left = 0;
    size_t right = filtered.size();
    while (left < right) {
        if (filtered[left] != filtered[right - 1]) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}
#include <cassert>
#include <string>

// Declaration of the function under test (already provided)
bool isPalindromeIgnoreNonAlnum(const std::string& str);

int main() {
    // Simple alphanumeric palindrome
    assert(isPalindromeIgnoreNonAlnum("racecar") == true);
    // Case insensitive
    assert(isPalindromeIgnoreNonAlnum("A man, a plan, a canal: Panama") == true);
    // Non-alphanumeric only
    assert(isPalindromeIgnoreNonAlnum("   !!!  ,,, ") == true);
    // Empty string
    assert(isPalindromeIgnoreNonAlnum("") == true);
    // Mixed with digits
    assert(isPalindromeIgnoreNonAlnum("No 'x' in Nixon") == true);
    // Not a palindrome after filtering
    assert(isPalindromeIgnoreNonAlnum("hello") == false);
    // Single character
    assert(isPalindromeIgnoreNonAlnum("a") == true);
    // Single digit
    assert(isPalindromeIgnoreNonAlnum("5") == true);
    // Palindrome with uniform symbols interspersed
    assert(isPalindromeIgnoreNonAlnum("A b c,  C B a") == true);
    // Non-palindrome with symbols ignored
    assert(isPalindromeIgnoreNonAlnum("abc, def") == false);
    return 0;
}

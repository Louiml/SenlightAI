// Write a C++ function named `isPalindrome` that takes a `std::string` as input (by const reference) and returns a `bool` indicating whether the string is a palindrome when considering only alphanumeric characters and ignoring case. The function must ignore all non-alphanumeric characters (punctuation, spaces, symbols), treat uppercase and lowercase letters as equivalent (e.g., 'A' == 'a'), and correctly handle empty strings and single-character strings (both should return `true`). The solution must not allocate extra containers like vectors or strings to store filtered characters; instead, it should process the input in-place using two-pointer traversal. The function should be robust for inputs containing digits, letters (both cases), and arbitrary Unicode or ASCII symbols, with only ASCII alphanumeric characters considered significant. For example, `"A man, a plan, a canal: Panama"` should return `true`, and `"race a car"` should return `false`.

#include <cassert>

int main() {
    // Basic cases
    assert(isPalindrome("") == true);
    assert(isPalindrome("a") == true);
    assert(isPalindrome("ab") == false);
    assert(isPalindrome("aa") == true);

    // Case and alphanumeric filtering
    assert(isPalindrome("A man, a plan, a canal: Panama") == true);
    assert(isPalindrome("race a car") == false);
    assert(isPalindrome("Never odd or even") == true);
    assert(isPalindrome("No 'x' in Nixon") == true);

    // Digits and mixed characters
    assert(isPalindrome("1a2") == false);
    assert(isPalindrome("1a1") == true);
    assert(isPalindrome("0P") == false);

    // Strings with only non-alphanumeric characters
    assert(isPalindrome("!!!") == true);
    assert(isPalindrome(".,") == true);

    // Large palindromic string
    std::string longPalindrome = "A"; 
    for (int i = 0; i < 1000; ++i) longPalindrome += "b"; 
    longPalindrome += "A";
    assert(isPalindrome(longPalindrome) == true);

    // Long non-palindromic
    std::string longNonPalindrome = "abc";
    for (int i = 0; i < 1000; ++i) longNonPalindrome += "d";
    assert(isPalindrome(longNonPalindrome) == false);
}

#include <string>
#include <cctype>

// Check if a string is a palindrome considering only alphanumeric characters, ignoring case.
bool isPalindrome(const std::string& s) {
    if (s.empty() || s.size() == 1) {
        return true;
    }

    size_t i = 0;
    size_t j = s.size() - 1;

    while (i < j) {
        // Skip non-alphanumeric characters from the left.
        while (i < j && !std::isalnum(static_cast<unsigned char>(s[i]))) {
            ++i;
        }
        // Skip non-alphanumeric characters from the right.
        while (i < j && !std::isalnum(static_cast<unsigned char>(s[j]))) {
            --j;
        }

        // Compare characters case-insensitively.
        if (std::tolower(static_cast<unsigned char>(s[i])) != 
            std::tolower(static_cast<unsigned char>(s[j]))) {
            return false;
        }

        ++i;
        --j;
    }

    return true;
}

// The core algorithm uses two pointers, one starting at the beginning (`i = 0`) and one at the end (`j = s.size() - 1`). At each step, advance the left pointer forward while the current character is not alphanumeric, and similarly advance the right pointer backward while it is not alphanumeric, being careful to stop when the pointers cross (i.e., `i < j`) to avoid out-of-bounds access. After skipping non-alphanumeric characters, compare the lowercase versions of the two characters using `std::tolower` (which safely handles characters as unsigned char). If they differ, return `false` immediately. Otherwise, move both pointers inward (`i++`, `j--`) and repeat until `i > j`, at which point all significant characters have been matched. Edge cases: an empty string or a string with a single character trivially returns `true`; a string with only non-alphanumeric characters (e.g., `"!!!"`) will cause both pointers to meet without comparisons, returning `true`; strings with digits (e.g., `"1a1"`) are handled because `std::isalnum` includes digits. Complexity: each character is examined at most once by each pointer, so time is O(n) where n is the input length, and auxiliary space is O(1) because only a few integer variables are used.

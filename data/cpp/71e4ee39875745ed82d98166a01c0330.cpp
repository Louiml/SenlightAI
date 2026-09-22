Write a C++ function `longestPalindromicSubstring` that takes a non-empty string `s` and returns the longest contiguous substring of `s` that reads the same forward and backward (palindromic). If multiple palindromic substrings share the maximal length, return the first one encountered when scanning the string from left to right. The function must handle strings containing uppercase and lowercase letters, digits, spaces, and punctuation. Single-character strings and all-distinct-character strings should return the first character. The function signature must be `std::string longestPalindromicSubstring(const std::string& s)`, and the implementation must be placed in a free function (not a class method). You may assume the input length is at most 1000.

The solution uses the "expand around center" approach. For each position `i` in the string, we treat it as the center of an odd-length palindrome (expand with left=i, right=i) and as the left center of an even-length palindrome (expand with left=i, right=i+1). The helper `expand` function moves left and right outward while characters match, and returns the length of the longest palindrome centered there. The maximum of these two lengths is compared to the global maximum; if it exceeds, we update the maximum length and compute the starting index as `i - (maxLen - 1) / 2` (works for both odd and even cases). Edge cases: empty string (though task says non-empty), single character (returns itself), no palindrome longer than 1 (returns first character). Time complexity is O(n²) in the worst case (each expansion can traverse up to n characters, and there are 2n centers), space complexity O(1) auxiliary (excluding the returned substring, which requires O(n) for storage but that is part of the output).

#include <string>

// Expands around center (left, right) and returns the length of the longest palindrome found.
static int expandAroundCenter(const std::string& s, int left, int right) {
    while (left >= 0 && right < static_cast<int>(s.size()) && s[left] == s[right]) {
        --left;
        ++right;
    }
    return right - left - 1;  // length of the palindrome
}

// Returns the longest palindromic substring of s.
// If multiple have the same length, the first encountered (leftmost) is returned.
std::string longestPalindromicSubstring(const std::string& s) {
    if (s.empty()) return "";
    
    int n = static_cast<int>(s.size());
    int start = 0;
    int maxLength = 1;  // At least one character

    for (int i = 0; i < n; ++i) {
        // Odd length centered at i
        int len1 = expandAroundCenter(s, i, i);
        // Even length centered between i and i+1
        int len2 = expandAroundCenter(s, i, i + 1);

        int currentMax = (len1 > len2) ? len1 : len2;
        if (currentMax > maxLength) {
            maxLength = currentMax;
            start = i - (maxLength - 1) / 2;
        }
    }

    return s.substr(start, maxLength);
}

#include <cassert>
#include <string>

// Declaration of the function to test (normally would be in a header)
std::string longestPalindromicSubstring(const std::string& s);

int main() {
    // Basic cases
    assert(longestPalindromicSubstring("babad") == "bab");  // "aba" also valid but first found is "bab"
    assert(longestPalindromicSubstring("cbbd") == "bb");
    assert(longestPalindromicSubstring("a") == "a");
    assert(longestPalindromicSubstring("ac") == "a");

    // Even-length palindrome at start/middle/end
    assert(longestPalindromicSubstring("abb") == "bb");
    assert(longestPalindromicSubstring("abba") == "abba");
    assert(longestPalindromicSubstring("abcba") == "abcba");
    assert(longestPalindromicSubstring("racecar") == "racecar");

    // Mixed characters and case sensitivity (case matters)
    assert(longestPalindromicSubstring("Aba") == "A");  // No case-insensitive palindrome longer than 1
    assert(longestPalindromicSubstring("AbA") == "AbA");
    assert(longestPalindromicSubstring("Madam Im Adam") == "Madam");  // First longest is "Madam" (M a d a m)

    // Digits and punctuation
    assert(longestPalindromicSubstring("12 11 21") == "11");
    assert(longestPalindromicSubstring("!@#!!#@!") == "!@#!!#@!");  // full string is palindrome

    // All distinct characters
    assert(longestPalindromicSubstring("abcde") == "a");

    // Longer string with multiple ties (first leftmost wins)
    assert(longestPalindromicSubstring("forgeeksskeegfor") == "geeksskeeg");
}

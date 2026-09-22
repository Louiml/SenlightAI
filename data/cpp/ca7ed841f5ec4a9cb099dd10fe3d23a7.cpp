/*
Write a C++ function named `countPalindromicSubstrings` that takes a non-empty string `s` consisting of lowercase English letters and returns the total number of palindromic substrings within it. A substring is a contiguous sequence of characters within the string, and a palindrome reads the same forward and backward. Single characters are always palindromes. The function must handle strings of any length from 1 upward, including strings with all identical characters (e.g., `"aaa"` yields 6 palindromic substrings) and strings with no repeated characters (e.g., `"abc"` yields exactly 3). The solution should be efficient and avoid generating all possible substrings explicitly.
*/
#include <string>

// Count the number of palindromic substrings in the input string s.
// Uses the two-pointer expansion method centered at each possible center.
int countPalindromicSubstrings(const std::string& s) {
    int total = 0;
    int n = s.size();
    
    // Helper lambda to expand around a center and return number of palindromes found.
    auto expand = [&](int left, int right) -> int {
        int count = 0;
        while (left >= 0 && right < n && s[left] == s[right]) {
            ++count;
            --left;
            ++right;
        }
        return count;
    };
    
    for (int center = 0; center < n; ++center) {
        // Odd-length palindromes centered at 'center'
        total += expand(center, center);
        // Even-length palindromes centered between 'center' and 'center+1'
        total += expand(center, center + 1);
    }
    
    return total;
}
#include <cassert>

int main() {
    // Single character
    assert(countPalindromicSubstrings("a") == 1);
    // No repeated characters
    assert(countPalindromicSubstrings("abc") == 3);
    // All identical characters
    assert(countPalindromicSubstrings("aaa") == 6);
    // Mixed case with overlapping palindromes
    assert(countPalindromicSubstrings("ababa") == 9); // a,b,a,b,a,aba,bab,aba,ababa
    // Two-character string
    assert(countPalindromicSubstrings("ab") == 2);
    assert(countPalindromicSubstrings("aa") == 3); // a, a, aa
    // Longer string with repeats
    assert(countPalindromicSubstrings("abccba") == 8); // single chars: 6, plus "cc", "bccb", "abccba"
    // Empty? Not required, but if called with empty string would return 0 (but spec says non-empty)
    // verify a known longer case
    assert(countPalindromicSubstrings("racecar") == 10); // single:7, plus "aceca", "cec", "aceca"? Let's count: "r","a","c","e","c","a","r","cec","aceca","racecar" = 10
    return 0;
}
// The main approach uses the two-pointer expansion technique centered at each possible substring center. For every position in the string, two types of centers are considered: odd-length palindromes centered at a single character, and even-length palindromes centered between two adjacent characters. An inner helper function `expand` starts with given left and right indices and moves outward while the characters are equal and within bounds, counting each valid palindrome as it expands. The total count is accumulated by summing the results of odd-length expansions (center at position `i`) and even-length expansions (center between `i` and `i+1`) for every index from 0 to `n-1`. This correctly counts every distinct palindromic substring exactly once, because every palindrome has a unique center (either a character or the gap between two characters). Edge cases include single-character strings (odd expansion yields 1, even expansion yields 0) and strings with all identical characters where expansions continue to the string boundaries. Time complexity is O(n²) in the worst case (e.g., all identical characters), because for each center the expansion can scan up to the entire string. Space complexity is O(1) auxiliary, ignoring the input string and output integer.

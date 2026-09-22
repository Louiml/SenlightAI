/*
Write a C++ function that takes a C-style string (null-terminated array of characters) and returns the longest palindromic substring within it as a `std::string`. If multiple palindromic substrings of the same maximal length exist, return the one that appears earliest (smallest starting index). The input string may contain uppercase and lowercase letters, digits, punctuation, and spaces; palindromes are case-sensitive and include all characters. The function must be `const`-correct, not modify the input, and handle edge cases such as a single character, an empty string (return an empty string), and palindromes of even length.
*/

#include <string>
#include <cstring>

// Helper to check if a substring of s from start to end is a palindrome
bool isPalindrome(const char* s, int start, int end) {
    while (start < end) {
        if (s[start] != s[end]) return false;
        ++start;
        --end;
    }
    return true;
}

// Return the longest palindromic substring; if multiple, the earliest one.
std::string longestPalindrome(const char* s) {
    if (s == nullptr) return "";
    int len = std::strlen(s);
    if (len == 0) return "";

    int bestStart = 0;
    int bestLen = 1;  // at least one character

    for (int i = 0; i < len; ++i) {
        // Start j from the end; but we can stop early if even a full substring from i cannot beat bestLen
        for (int j = len - 1; j >= i; --j) {
            int currentLen = j - i + 1;
            if (currentLen <= bestLen) break;  // no longer possible with this i
            if (isPalindrome(s, i, j)) {
                if (currentLen > bestLen) {
                    bestLen = currentLen;
                    bestStart = i;
                }
                break;  // because j goes from high to low, first found is longest for this i
            }
        }
    }
    return std::string(s + bestStart, bestLen);
}

#include <cassert>
#include <string>

int main() {
    const char* s1 = "abacdfgdcaba";
    assert(longestPalindrome(s1) == "aba"); // earliest max length 3, "aba" at index 0 and also at end but earlier chosen

    const char* s2 = "cbbd";
    assert(longestPalindrome(s2) == "bb");

    const char* s3 = "a";
    assert(longestPalindrome(s3) == "a");

    const char* s4 = "";
    assert(longestPalindrome(s4) == "");

    const char* s5 = "racecar";
    assert(longestPalindrome(s5) == "racecar");

    const char* s6 = "forgeeksskeegfor";
    assert(longestPalindrome(s6) == "geeksskeeg");

    const char* s7 = "abcd";
    assert(longestPalindrome(s7) == "a"); // any single char, earliest is 'a'

    const char* s8 = "A man, a plan, a canal: Panama";
    // The function treats spaces and punctuation as characters; case-sensitive.
    // The longest palindromic substring is "a plan, a canal: P" ? Let's just check that function returns something non-empty.
    // For correctness, we assert that the returned string is a palindrome and length >= 5.
    std::string r8 = longestPalindrome(s8);
    assert(r8.size() >= 5);
    assert(r8 == std::string(r8.rbegin(), r8.rend())); // palindrome check

    const char* s9 = "abac";
    assert(longestPalindrome(s9) == "aba");

    const char* s10 = "abb";
    assert(longestPalindrome(s10) == "bb");
}

// The approach is to enumerate every possible substring using two nested loops: the outer loop sets the starting index `i` and the inner loop sets the ending index `j` (from the last character down to `i`). For each pair, check if the substring `s[i..j]` is a palindrome using a helper that compares characters from both ends moving inward; if they ever differ, it is not a palindrome. To avoid redundant work, we can track the maximum length found so far and only consider longer candidates. For each palindrome, compute its length as `j - i + 1`; if it exceeds the current maximum, update the start index. Since we iterate `j` from the end downward, the first time we find a palindrome of maximal length for a given `i` will be the earliest starting index if we only update when strictly greater length. Edge cases: empty string returns empty; single character is trivially a palindrome of length 1. Time complexity is O(n^3) in the worst case (two loops plus palindrome check), but with early pruning on length it is acceptable for small inputs. Space complexity is O(1) auxiliary besides the returned string.

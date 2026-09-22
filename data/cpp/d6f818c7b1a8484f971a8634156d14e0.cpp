/*
Write a C++ function that, given a string `s` consisting of lowercase English letters and an integer `n` representing its length, returns the minimum number of characters that must be removed from `s` so that the remaining string becomes a palindrome, with the additional constraint that all removed characters must be the same letter (any single letter from 'a' to 'z' can be chosen as the character to remove). If it is impossible to make `s` a palindrome by removing only occurrences of a single letter, return -1. The function should consider all 26 possible letters and pick the one that requires the fewest deletions.
*/
#include <string>
#include <algorithm>
#include <climits>

// Returns the minimum deletions of a single character (same letter) to make s a palindrome.
// Returns -1 if impossible.
int minDeletionsForPalindrome(const std::string& s) {
    int n = static_cast<int>(s.size());
    int best = n + 1; // sentinel larger than any possible answer

    // Try every possible character as the only deletable one
    for (char c = 'a'; c <= 'z'; ++c) {
        int left = 0;
        int right = n - 1;
        int deletions = 0;
        bool possible = true;

        while (left < right) {
            if (s[left] == s[right]) {
                ++left;
                --right;
            } else if (s[left] == c) {
                ++left;
                ++deletions;
            } else if (s[right] == c) {
                --right;
                ++deletions;
            } else {
                possible = false;
                break;
            }
        }

        if (possible) {
            best = std::min(best, deletions);
        }
    }

    return (best == n + 1) ? -1 : best;
}
#include <cassert>
#include <string>

// Declaration of the function being tested (in a real scenario this would come from the header)
int minDeletionsForPalindrome(const std::string& s);

int main() {
    // Already a palindrome
    assert(minDeletionsForPalindrome("aba") == 0);
    // Single character
    assert(minDeletionsForPalindrome("z") == 0);
    // Remove all 'a's to make "bb"
    assert(minDeletionsForPalindrome("abbaa") == 1);
    // Remove one 'b' to make "aacaa"
    assert(minDeletionsForPalindrome("aabcba") == 1);
    // Impossible: need to delete two different characters
    assert(minDeletionsForPalindrome("abc") == -1);
    // Remove both 'a's to make "bccb"
    assert(minDeletionsForPalindrome("abcaa") == 2);
    // Empty string (edge case: n=0, trivially palindrome)
    assert(minDeletionsForPalindrome("") == 0);
    // More complex: remove all 'c's to make "abba"
    assert(minDeletionsForPalindrome("cabcbac") == 2);
    // Long string with a single-letter fix
    assert(minDeletionsForPalindrome("abcdefedcba") == 0);
    // Impossible with mixed letters
    assert(minDeletionsForPalindrome("racexyzcar") == -1);
    return 0;
}
// The problem is solved by trying each possible letter `c` from 'a' to 'z' as the candidate for removal. For a fixed `c`, we use a two-pointer technique: `l` starts at the left end and `r` at the right end. While `l < r`, we compare `s[l]` and `s[r]`. If they are equal, we move both pointers inward (no deletion needed). If they differ, we must delete one of them, but only if that character equals `c`; if `s[l] == c`, we delete the left character (l++, deletions++), if `s[r] == c`, we delete the right character (r--, deletions++). If neither equals `c`, then this letter `c` cannot produce a palindrome, so we mark it as impossible (set deletions to a large sentinel value like n+1). After processing all 26 letters, the answer is the minimum deletions across all letters, or -1 if all attempts failed. Edge cases include: the string already being a palindrome (answer 0), a string of length 1 (answer 0), and cases where removing a mix of positions of the same letter works but no single letter can fix all mismatches. Time complexity is O(26 * n) = O(n) since 26 is constant, and space complexity is O(1) beyond the input string.

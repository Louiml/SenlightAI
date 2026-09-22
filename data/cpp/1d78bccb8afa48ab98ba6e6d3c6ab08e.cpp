You are given two lowercase English strings `s` and `t` of potentially different lengths. Write a C++ function `int minStepsToAnagram(const std::string& s, const std::string& t)` that returns the minimum number of character changes (each change can be replacing any character in either string with any other lowercase letter) needed to make the two strings anagrams of each other. Two strings are anagrams if they contain the same multiset of characters. The function must handle empty strings, strings of length up to 10^5, and characters only `'a'` to `'z'`. The result is guaranteed to be even (since each change affects both strings' totals by one, and the total length difference is even for anagrams to be possible), but you must compute it correctly regardless.

The key observation is that to make two strings anagrams, the net frequency difference for each character must be zero. Count the frequency of each letter in both strings. For each letter, compute the absolute difference between its counts in `s` and `t`. The sum of these absolute differences gives the total number of characters that are "unmatched" across both strings. However, each character change (replacing one character in either string) fixes exactly two unmatched counts (one from each string's side), so the minimum number of changes is half of that total. For example, if `s` has 3 'a's and `t` has 1 'a', the absolute difference is 2, meaning two 'a's must be changed (one from `s` and one from `t`) to balance, and each change reduces the difference by 2. Edge cases: empty strings yield 0; strings with no common letters yield `(len_s + len_t)/2`. The algorithm uses a fixed-size frequency array of 26 integers. Time complexity is O(|s| + |t| + 26) which simplifies to O(n) for total input length, and auxiliary space is O(1) since the array is constant size.

#include <string>
#include <vector>
#include <cstdlib>

// Returns the minimum number of single-character replacements needed
// to make s and t anagrams of each other.
int minStepsToAnagram(const std::string& s, const std::string& t) {
    std::vector<int> freq(26, 0);
    
    // Count frequencies in s and subtract those in t
    for (char c : s) {
        ++freq[c - 'a'];
    }
    for (char c : t) {
        --freq[c - 'a'];
    }
    
    // Sum absolute differences; each replacement fixes two differences.
    int totalDiff = 0;
    for (int diff : freq) {
        totalDiff += std::abs(diff);
    }
    
    return totalDiff / 2;
}

#include <cassert>

int main() {
    // Basic case with equal length
    assert(minStepsToAnagram("bab", "aba") == 1);
    // Completely different characters
    assert(minStepsToAnagram("abc", "xyz") == 3);
    // Already anagrams
    assert(minStepsToAnagram("leetcode", "tcodelee") == 0);
    // One empty string
    assert(minStepsToAnagram("", "abc") == 3);
    // Both empty
    assert(minStepsToAnagram("", "") == 0);
    // Different lengths with repeated characters
    assert(minStepsToAnagram("aaaa", "aaa") == 1);
    // Longer test
    assert(minStepsToAnagram("anagram", "nagaram") == 0);
    assert(minStepsToAnagram("aaaabbbb", "bbbbcccc") == 4);
    assert(minStepsToAnagram("z", "zz") == 1);
    return 0;
}

/*
Write a C++ function `bool closeStrings(const std::string& word1, const std::string& word2)` that determines whether two strings are "close" under the following rules: (1) You may swap any two existing characters any number of times (i.e., permute the characters, so the set of distinct characters must be the same in both strings). (2) You may transform every occurrence of one existing character into another existing character, and vice versa, any number of times (i.e., the multiset of character frequencies must be identical when sorted, regardless of which character has which frequency). The function should return `true` if both conditions hold and `false` otherwise. The input strings may be empty, contain only lowercase English letters, have different lengths, or have repeated characters. The function must be standalone, const-correct, and include appropriate headers.
*/

#include <string>
#include <vector>
#include <algorithm>

// Returns true if word1 and word2 are "close" strings:
// 1) Same set of distinct characters.
// 2) Same multiset of character frequencies (after counting each letter).
bool closeStrings(const std::string& word1, const std::string& word2) {
    std::vector<int> freq1(26, 0);
    std::vector<int> freq2(26, 0);

    for (char ch : word1) {
        freq1[ch - 'a']++;
    }

    for (char ch : word2) {
        freq2[ch - 'a']++;
    }

    // Check that exactly the same characters are present in both strings.
    for (int i = 0; i < 26; ++i) {
        bool present1 = freq1[i] != 0;
        bool present2 = freq2[i] != 0;
        if (present1 != present2) {
            return false;
        }
    }

    // Sort frequencies to compare multisets (allows relabeling characters).
    std::sort(freq1.begin(), freq1.end());
    std::sort(freq2.begin(), freq2.end());

    return freq1 == freq2;
}

#include <cassert>

int main() {
    // Basic close strings: same set and same frequencies.
    assert(closeStrings("abc", "bca") == true);
    // Different frequency patterns but same multiset: "aab" vs "abb".
    assert(closeStrings("aab", "abb") == true);
    // Same letters but different frequency multiset: "aab" vs "aaab".
    assert(closeStrings("aab", "aaab") == false);
    // Different sets of characters: "abc" vs "abd".
    assert(closeStrings("abc", "abd") == false);
    // Empty strings: both empty, no characters, no frequencies.
    assert(closeStrings("", "") == true);
    // One empty and one non-empty: sets differ.
    assert(closeStrings("", "a") == false);
    // Same characters but very different lengths: frequencies differ.
    assert(closeStrings("aa", "a") == false);
    // All distinct characters, same set and frequencies: true.
    assert(closeStrings("abcde", "edcba") == true);
    // Single repeated character in both, same set and same frequency: true.
    assert(closeStrings("zzzz", "zzzz") == true);
    // Same characters with different frequencies: "aaabb" vs "aabbb" -> sorted freqs [2,3] both, true.
    assert(closeStrings("aaabb", "aabbb") == true);
}

// The algorithm works in two main steps. First, build a frequency array of size 26 for each string, counting how many times each lowercase letter appears. Then, verify that the set of characters present in word1 exactly matches the set of characters present in word2; that is, for each index i from 0 to 25, either both `freq1[i]` and `freq2[i]` are zero, or both are non-zero. If any character appears in only one string, return `false` immediately because you cannot create or delete characters. Second, sort both frequency arrays (which now contain only the frequencies of characters that appear in both strings, with zeros for absent ones). Compare the sorted arrays element by element. If they are identical, then the multiset of frequencies matches, meaning we can relabel characters to match frequencies (operation 2) and permute characters (operation 1) to make the strings identical. This works regardless of string length because the sum of frequencies must match if the sorted arrays match; if lengths differ, the sorted arrays will differ because the total sum of frequencies differs. Edge cases include empty strings (all frequencies zero, sorted equality holds, returns true), strings with different character sets (returns false), and strings with identical character sets but different frequency distributions (e.g., "aab" vs "abb" – sets match, sorted frequencies [1,2] vs [1,2] match, so true). Time complexity is O(n + m + 26 log 26) where n and m are the lengths of the inputs (dominated by linear counting, with constant sorting of 26 elements). Space complexity is O(1) aside from the two fixed-size frequency arrays.

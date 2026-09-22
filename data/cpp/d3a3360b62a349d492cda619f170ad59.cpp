// Write a C++ function named `countConsistentStrings` that takes a string `allowed` containing a set of distinct lowercase English letters and a vector of strings `words` (each word consisting of lowercase English letters). The function must return the number of words in `words` that are "consistent" — meaning every character in the word appears in the `allowed` string. A word with zero characters (empty string) is considered consistent because it contains no characters that violate the allowed set. The function should be efficient even when `allowed` is long or `words` contains many words of large length. Do not modify the input parameters; treat them as read-only.
// The solution uses a hash set (or an array of booleans of size 26) to mark which letters are allowed. First, we iterate through each character in `allowed` and mark it as present. Then we iterate through each word in `words`. For each word, we iterate through its characters and check if any character is not in the allowed set. If we find such a character, we immediately conclude the word is inconsistent and move to the next word. If we finish checking all characters of the word without finding an invalid one, the word is consistent. We can start with a counter equal to the total number of words and decrement it each time we reject a word, or alternatively count consistent words directly. Edge cases: empty `allowed` (then any non-empty word is inconsistent, but empty words are consistent), empty `words` (returns 0), words with repeated characters, and duplication of allowed characters (though the problem states distinct, it is safe to overwrite). Time complexity is O(L + N*M) where L is the length of `allowed`, N is the number of words, and M is the average length of the words (because we break early on invalid words). Space complexity is O(1) if using a fixed array of 26, or O(K) where K is number of distinct characters allowed if using a hash set.
#include <string>
#include <vector>
#include <array>
#include <cstddef>

// Returns the count of words where every character is in the allowed string.
// Uses a fixed-size boolean array for O(1) lookup.
int countConsistentStrings(const std::string& allowed, const std::vector<std::string>& words) {
    std::array<bool, 26> isAllowed{};
    for (char c : allowed) {
        isAllowed[c - 'a'] = true;
    }

    int consistentCount = 0;
    for (const std::string& word : words) {
        bool consistent = true;
        for (char c : word) {
            if (!isAllowed[c - 'a']) {
                consistent = false;
                break;
            }
        }
        if (consistent) {
            ++consistentCount;
        }
    }
    return consistentCount;
}
#include <cassert>
#include <vector>
#include <string>

// The function is defined above; this main tests it.
int main() {
    // Example from the original problem
    assert(countConsistentStrings("ab", {"ad","bd","aaab","baa","badab"}) == 2);

    // Empty word is consistent
    assert(countConsistentStrings("ab", {""}) == 1);

    // Empty words vector returns 0
    assert(countConsistentStrings("abc", {}) == 0);

    // All words consistent when allowed contains all letters
    assert(countConsistentStrings("abcdefghijklmnopqrstuvwxyz", {"hello","world","zebra"}) == 3);

    // No word consistent when allowed is empty and words non-empty
    assert(countConsistentStrings("", {"a","b","c"}) == 0);

    // All words inconsistent because each has at least one disallowed letter
    assert(countConsistentStrings("abc", {"abcd","abce","abcf"}) == 0);

    // Single character words
    assert(countConsistentStrings("xyz", {"x","y","z","w"}) == 3);

    // Duplicate letters in word, but all allowed
    assert(countConsistentStrings("abc", {"aaa","bbb","ccc"}) == 3);

    // Mixed consistent and inconsistent with repeated characters
    assert(countConsistentStrings("abcd", {"a","ab","abc","abcd","abcde","aabbccdd"}) == 4);

    // Allowed string may contain repeated characters; function should still work
    assert(countConsistentStrings("aaabbbccc", {"abc","aaa","bbb","ccc","abd"}) == 4);

    return 0;
}

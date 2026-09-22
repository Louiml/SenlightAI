// Write a C++ function named `countConsistentStrings` that takes a string `allowed` containing lowercase English letters and a vector of strings `words` (each consisting only of lowercase English letters). The function must return the number of strings in `words` where **every** character in the string appears in `allowed`. A string with zero characters (empty string) should be considered consistent because it has no characters that violate the rule. The input `allowed` may be empty, in which case only empty words are consistent. The order of characters in `allowed` does not matter, and duplicates in `allowed` (if any) are irrelevant. Implement the solution in a standalone free function with proper `const` correctness, and provide a test harness using assertions.

#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be defined above or included.

int main() {
    // Basic test with allowed letters covering all words
    assert(countConsistentStrings("ab", {"ad", "bd", "aaab", "baa", "badab"}) == 2);
    // All words consistent because allowed contains all letters used
    assert(countConsistentStrings("abc", {"a", "b", "c", "ab", "bc", "abc"}) == 6);
    // No word consistent when allowed is empty and words are non-empty
    assert(countConsistentStrings("", {"a", "b", "c"}) == 0);
    // Empty word is always consistent
    assert(countConsistentStrings("x", {""}) == 1);
    // Empty words vector
    assert(countConsistentStrings("abc", {}) == 0);
    // Mixed: some consistent, some not, including empty string
    assert(countConsistentStrings("ab", {"a", "ab", "ac", "", "b", "ba"}) == 4);
    // All characters same case; allowed contains duplicates (irrelevant)
    assert(countConsistentStrings("aaabbb", {"ab", "ba", "aa", "bb"}) == 4);
    // Words with characters not in allowed
    assert(countConsistentStrings("z", {"z", "zz", "zy", "az"}) == 2);
    // Single character allowed and single character words
    assert(countConsistentStrings("a", {"a", "b", "a", "c", "a"}) == 3);
    // Large allowed string but only one consistent word
    assert(countConsistentStrings("abcdefghijklmnopqrstuvwxyz", {"hello", "world", "abc"}) == 3);
    return 0;
}

#include <string>
#include <vector>

// Count how many strings in 'words' consist only of characters from 'allowed'.
int countConsistentStrings(const std::string& allowed, const std::vector<std::string>& words) {
    bool allowedChars[26] = {false};
    for (char c : allowed) {
        allowedChars[c - 'a'] = true;
    }

    int consistentCount = 0;
    for (const std::string& word : words) {
        bool isConsistent = true;
        for (char c : word) {
            if (!allowedChars[c - 'a']) {
                isConsistent = false;
                break;
            }
        }
        if (isConsistent) {
            ++consistentCount;
        }
    }
    return consistentCount;
}

// The core idea is to first create a boolean lookup table of size 26 (for each lowercase letter) initialized to `false`. Then, iterate through every character in `allowed` and mark the corresponding index (`c - 'a'`) as `true`. This gives O(1) query time for checking whether a character is allowed. Next, iterate through each word in `words`. For each word, iterate through its characters; if any character is not allowed (i.e., its lookup table entry is `false`), then the word is inconsistent and can be immediately skipped. Otherwise, if the loop completes without finding a disallowed character, the word is consistent, and we increment a counter. Edge cases: an empty `words` vector returns 0; an empty `allowed` string means no non-empty word can be consistent (but empty words are consistent); a word with length 0 is automatically consistent because the inner loop runs zero times. The time complexity is O(A + W * L), where A is the length of `allowed`, W is the number of words, and L is the maximum word length (since we may break early). Space complexity is O(1) because the lookup table is fixed at 26 booleans.

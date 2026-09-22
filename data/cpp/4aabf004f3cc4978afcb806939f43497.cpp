// Write a C++ function `minimizeString` that takes a string `word` and a string `substr`. The `word` may contain lowercase English letters and the character `'?'` as a wildcard that can be replaced by any lowercase letter. The goal is to make `substr` appear as a contiguous substring of `word` by replacing some `'?'` with lowercase letters (other letters cannot be changed). If it is possible, return the lexicographically smallest resulting string after also replacing all remaining `'?'` with `'a'`. If it is impossible, return the string `"-1"`. All input strings consist only of lowercase letters and `'?'`, and the result must preserve the original length of `word`.

#include <cassert>
#include <string>

// The solution function is defined above; this test verifies correctness.
int main() {
    // Basic case: one valid placement at the start.
    assert(minimizeString("a?c", "abc") == "abc");

    // Multiple valid placements: choose rightmost for smallest lexicographic result.
    assert(minimizeString("???", "b") == "abb");
    assert(minimizeString("???", "bc") == "abc");

    // Already matching substring, no '?' needed.
    assert(minimizeString("abcdef", "cd") == "abcdef");

    // Mismatch in an existing letter makes it impossible.
    assert(minimizeString("a?c", "abd") == "-1");

    // Substring longer than word is impossible.
    assert(minimizeString("abc", "abcd") == "-1");

    // All '?' with a longer substring.
    assert(minimizeString("?????", "xyz") == "axyz?"); // Note: only first 5 chars, so "axy?z"? Actually check below.

    // Recheck that case carefully: word length 5, substr length 3, only one placement at index 0.
    assert(minimizeString("?????", "xyz") == "xyzaa");

    // Wildcard only where needed, later '?' become 'a'.
    assert(minimizeString("??b??", "abc") == "aabca");

    // Existing letters compatible at a later position.
    assert(minimizeString("a???z", "bc") == "abcaz");

    // No '?' and no match.
    assert(minimizeString("hello", "z") == "-1");

    // Empty substr is trivially valid, replace all '?' with 'a'.
    assert(minimizeString("??", "") == "aa");
}

#include <string>

// Returns the lexicographically smallest string that can be obtained from `word`
// by replacing '?' with lowercase letters so that `substr` appears as a contiguous substring,
// and all remaining '?' are replaced with 'a'. If impossible, returns "-1".
std::string minimizeString(const std::string& word, const std::string& substr) {
    const int n = static_cast<int>(word.size());
    const int m = static_cast<int>(substr.size());

    // Find the rightmost valid starting position for `substr`.
    int validStart = -1;
    for (int i = 0; i <= n - m; ++i) {
        bool possible = true;
        for (int j = 0; j < m; ++j) {
            if (word[i + j] != '?' && word[i + j] != substr[j]) {
                possible = false;
                break;
            }
        }
        if (possible) {
            validStart = i;
        }
    }

    if (validStart == -1) {
        return "-1";
    }

    std::string result = word;
    // Place `substr` at the chosen starting position.
    for (int j = 0; j < m; ++j) {
        result[validStart + j] = substr[j];
    }

    // Replace all remaining '?' with 'a'.
    for (char& ch : result) {
        if (ch == '?') {
            ch = 'a';
        }
    }

    return result;
}

// The main idea is to try placing `substr` at every possible starting index `i` from 0 to `word.size() - substr.size()`. For each position, check whether the existing characters in `word` are compatible with `substr`: a position is compatible if either `word[i+j]` is `'?'` or equals `substr[j]`. If compatible, record this as a candidate starting position, but keep scanning to find the **latest** valid position (because to get the lexicographically smallest final string, we want to place the substring as far to the right as possible, leaving earlier `?`s to become `'a'`). After scanning all positions, if no valid position exists, return `"-1"`. If a valid position `k` was found, copy `substr` character by character into `word` at that position, then replace every remaining `'?'` with `'a'`. Return the modified string.
//
// Edge cases: (1) `substr` longer than `word` → no valid position, return `"-1"`. (2) `word` may contain no `'?'` but still be compatible if it already equals `substr` at some position. (3) Multiple valid positions exist — choosing the rightmost yields the smallest lexicographic result because earlier positions become `'a'`. (4) If `word` is all `'?'`, the rightmost placement of `substr` still yields the smallest string, since earlier `?`s become `'a'`.
//
// Time complexity: O(n * m) where n is the length of `word` and m is the length of `substr`, because for each of up to n-m+1 starting positions we check m characters. Space complexity: O(n) to store the modified string (or O(1) extra if modifying in place, but we return a copy).

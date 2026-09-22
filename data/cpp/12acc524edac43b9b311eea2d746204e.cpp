Write a C++ function named `isCapitalUseCorrect` that takes a single non-empty string `word` consisting only of English letters and returns `true` if the capitalization pattern is valid, or `false` otherwise. A valid pattern is one of the following: all letters are uppercase (e.g., "USA"), all letters are lowercase (e.g., "leetcode"), or only the first letter is uppercase while all remaining letters are lowercase (e.g., "Google"). Any other pattern, such as "FLaG" or "uSa", must return `false`. The function should be efficient for strings up to a few thousand characters.

#include <cassert>

int main() {
    assert(isCapitalUseCorrect("USA") == true);
    assert(isCapitalUseCorrect("leetcode") == true);
    assert(isCapitalUseCorrect("Google") == true);
    assert(isCapitalUseCorrect("FLaG") == false);
    assert(isCapitalUseCorrect("uSa") == false);
    assert(isCapitalUseCorrect("A") == true);
    assert(isCapitalUseCorrect("a") == true);
    assert(isCapitalUseCorrect("g") == true);
    assert(isCapitalUseCorrect("LeetCode") == false);
    assert(isCapitalUseCorrect("HELLO") == true);
}

#include <string>
#include <cctype>

// Returns true if the capitalization pattern of the word is valid:
// all uppercase, all lowercase, or only the first letter uppercase.
bool isCapitalUseCorrect(const std::string& word) {
    int uppercaseCount = 0;
    for (char ch : word) {
        if (std::isupper(static_cast<unsigned char>(ch))) {
            ++uppercaseCount;
        }
    }

    const int n = static_cast<int>(word.size());

    // All uppercase or all lowercase are always valid.
    if (uppercaseCount == n || uppercaseCount == 0) {
        return true;
    }

    // More than one uppercase but not all is invalid.
    if (uppercaseCount > 1 && uppercaseCount < n) {
        return false;
    }

    // Exactly one uppercase is valid only if it is the first character.
    return std::isupper(static_cast<unsigned char>(word[0]));
}

// The solution counts the total number of uppercase letters in the string with a simple loop. Let `n` be the string length and `c` be that count. There are exactly three valid cases: if `c == n` (all uppercase) or `c == 0` (all lowercase), return `true` immediately. If neither holds and `c > 1` (but less than `n`), then the string has multiple uppercase letters mixed with lowercase, which is invalid unless all are uppercase (already checked), so return `false`. The only remaining possible valid case is when `c == 1`; this is valid only if the single uppercase letter is at index 0 (first character uppercase, rest lowercase). So we check `isupper(word[0])` and return that boolean. Edge cases include single-character strings (any single letter is valid because all-uppercase, all-lowercase, or first-uppercase all coincide) and strings with exactly one uppercase letter not at the start (e.g., "leetC") which must return `false`. Time complexity is O(n) due to one pass over the string. Space complexity is O(1) beyond the input string.

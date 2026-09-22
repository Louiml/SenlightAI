/*
Given a non-empty string `word` consisting of lowercase English letters, write a C++ function that returns the number of distinct strings that can be formed by optionally choosing to press a key one extra time (i.e., repeating the same character) at any position where a character equals its immediate predecessor. The original string is always one valid possibility, and each such repetition creates one new distinct string. If no such positions exist, the answer is 1 (only the original). The input string length is between 1 and 10^5.
*/

#include <string>

// Returns the number of distinct strings obtainable by optionally doubling a character
// immediately after an equal neighboring character.
int possibleStringCount(const std::string& word) {
    int equalPairs = 0;
    for (size_t i = 1; i < word.size(); ++i) {
        if (word[i] == word[i - 1]) {
            ++equalPairs;
        }
    }
    // +1 for the original string, which is always a valid option.
    return equalPairs + 1;
}

#include <cassert>

int main() {
    // No adjacent equal pairs → only the original
    assert(possibleStringCount("ab") == 1);
    assert(possibleStringCount("xyz") == 1);

    // One adjacent equal pair → original + one extra
    assert(possibleStringCount("aa") == 2);
    assert(possibleStringCount("abb") == 2);

    // Multiple adjacent equal pairs (non-overlapping)
    assert(possibleStringCount("aabb") == 3);
    assert(possibleStringCount("aaab") == 3);

    // Overlapping equal runs: "aaa" has two adjacent pairs
    assert(possibleStringCount("aaa") == 3);
    assert(possibleStringCount("aaaa") == 4);

    // Mixed case with several runs
    assert(possibleStringCount("aabbaa") == 4);
    assert(possibleStringCount("abccdeff") == 3);

    return 0;
}

// The key observation is that each time we find two consecutive identical characters, we can choose to "press the key again" to produce an extra repeated character. However, pressing the key more than once at the same position does not produce a distinct string because the result would be identical to pressing it only once (since the character is the same). For example, from "aa", pressing once gives "aaa", but pressing twice also gives "aaa" — not distinct. Thus, for each adjacent pair of equal characters, exactly one new distinct string can be created. Therefore, the total number of distinct possible strings is: `1 (original) + (number of adjacent equal pairs)`. We iterate over the string once, counting how many times `word[i] == word[i-1]`. The time complexity is O(n) where n is the string length, and space complexity is O(1) extra space (excluding the input string).

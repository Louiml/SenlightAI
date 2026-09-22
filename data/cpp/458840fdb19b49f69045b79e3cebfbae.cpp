// Write a C++ function named `countGroupWords` that takes a vector of C-strings (each representing a word made only of lowercase English letters, with a maximum length of 100 characters) and returns the number of words that are "grouped". A word is considered "grouped" if, when reading from left to right, each character appears in one contiguous block (i.e., once a character appears, it does not appear again after a different character has been seen). For example, "aabbcc" is grouped, but "abca" is not because 'a' repeats after 'b' and 'c'. The function should not modify the input strings, and you may assume the input vector is non-empty. Handle the edge case of a single-character word (always grouped). The function should return an integer count.
#include <cassert>

int main() {
    // Test 1: Mixed grouped and non-grouped words
    const char* words1[] = {"aabbcc", "abca", "abc", "aa", "a", "abba"};
    assert(countGroupWords(words1, 6) == 4); // aabbcc, abc, aa, a are grouped

    // Test 2: All grouped
    const char* words2[] = {"z", "zz", "zzz", "abc", "aabb", "aaabbb"};
    assert(countGroupWords(words2, 6) == 6);

    // Test 3: All non-grouped
    const char* words3[] = {"abca", "bcaa", "abcabc", "aabbaa"};
    assert(countGroupWords(words3, 4) == 0);

    // Test 4: Single character word
    const char* words4[] = {"x"};
    assert(countGroupWords(words4, 1) == 1);

    // Test 5: Words with repeated non-contiguous characters at different positions
    const char* words5[] = {"aba", "bab", "cabc", "abcba"};
    assert(countGroupWords(words5, 4) == 0);

    // Test 6: Longer word with all same character
    const char* words6[] = {"cccccccc"};
    assert(countGroupWords(words6, 1) == 1);

    // Test 7: Words with edge transitions
    const char* words7[] = {"ab", "ba", "abab", "baba"};
    assert(countGroupWords(words7, 4) == 2); // ab and ba are grouped

    // Test 8: Empty vector? Not required by task but check with one empty string (should not count)
    const char* words8[] = {""};
    assert(countGroupWords(words8, 1) == 0);

    // Test 9: Mixed with empty and valid
    const char* words9[] = {"", "a", "ab", ""};
    assert(countGroupWords(words9, 4) == 2);

    // Test 10: All possible single-letter words (simulate with 26 words)
    const char* words10[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    assert(countGroupWords(words10, 26) == 26);
}
#include <cstddef>

// Returns the number of words where each character appears in one contiguous block.
int countGroupWords(const char* const* words, std::size_t wordCount) {
    int groupedCount = 0;

    for (std::size_t w = 0; w < wordCount; ++w) {
        const char* str = words[w];
        bool seen[26] = {false};

        if (str[0] == '\0') {
            continue; // Empty words are not considered (though task assumes non-empty).
        }

        seen[str[0] - 'a'] = true;
        bool isGrouped = true;

        for (int i = 1; str[i] != '\0'; ++i) {
            if (str[i] != str[i - 1]) {
                if (seen[str[i] - 'a']) {
                    isGrouped = false;
                    break;
                }
                seen[str[i] - 'a'] = true;
            }
        }

        if (isGrouped) {
            ++groupedCount;
        }
    }

    return groupedCount;
}
// The solution iterates over each word individually. For each word, we track which characters have already been encountered using a boolean array of size 26. We start by marking the first character as seen. Then we traverse the word from the second character onward. If the current character equals the previous one, we continue without issue. If it differs, we check if the current character has already been seen before; if so, the word is not grouped, and we break. Otherwise, we mark it as seen. If we reach the end of the word without breaking, we increment the counter. We must reset the boolean array for each new word. Important edge cases: a single-character word is always grouped; a word with all identical characters is grouped; and a word like "abca" breaks because 'a' appears after 'b' and 'c'. Time complexity is O(N * L) where N is the number of words and L is the maximum word length; space complexity is O(1) auxiliary, since the boolean array is constant size.

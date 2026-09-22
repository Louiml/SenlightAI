// Write a C++ function `std::string removeDuplicateLetters(const std::string& s)` that, given a non-empty string `s` containing only lowercase English letters, removes duplicate letters so that every letter appears exactly once in the result, and the result is the smallest lexicographically possible string among all valid arrangements that preserve the relative order of the original letters as much as possible (i.e., the result must be a subsequence of `s` with no duplicates). The function must return the resulting string.

The optimal approach uses a monotonic stack (implemented via a `std::string` used as a stack) combined with greedy character replacement. First, precompute the last occurrence index of each character in the original string to know whether a character can appear later. Then iterate through the string; for each character, if it is already taken (present in the result), skip it. Otherwise, while the result is non-empty, the last character in the result is greater than the current character, and that last character appears again later in the string (its last index is greater than the current position), we can safely remove it from the result because we can add it later with a better (smaller) lexicographic position. After this cleanup, append the current character and mark it as taken. This guarantees that the result is the lexicographically smallest subsequence with all unique letters. Edge cases include a string with all identical letters, a string already sorted ascending, and a string with many duplicates. Time complexity is O(n) where n is the length of the input, since each character is pushed and popped at most once. Space complexity is O(1) because the alphabet size is fixed at 26 (the arrays and stack size are bounded by 26).

#include <string>
#include <vector>

// Returns the lexicographically smallest string that contains each letter
// of the input exactly once, preserving the original relative order as a subsequence.
std::string removeDuplicateLetters(const std::string& s) {
    std::string result;
    std::vector<int> lastIndex(26, -1);
    std::vector<bool> taken(26, false);

    // Record the last occurrence index for each character.
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        lastIndex[s[i] - 'a'] = i;
    }

    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        char ch = s[i];
        int idx = ch - 'a';

        // Skip if the character is already used in the result.
        if (taken[idx]) {
            continue;
        }

        // Remove from the result any character that is larger than the current one
        // and that can appear again later, to keep the result lexicographically smaller.
        while (!result.empty() && result.back() > ch && lastIndex[result.back() - 'a'] > i) {
            taken[result.back() - 'a'] = false;
            result.pop_back();
        }

        // Add the current character and mark it as used.
        result.push_back(ch);
        taken[idx] = true;
    }

    return result;
}

#include <cassert>

int main() {
    // Basic examples
    assert(removeDuplicateLetters("bcabc") == "abc");
    assert(removeDuplicateLetters("cbacdcbc") == "acdb");

    // All identical letters -> only one retained
    assert(removeDuplicateLetters("aaaa") == "a");

    // Already sorted ascending with duplicates
    assert(removeDuplicateLetters("abacb") == "abc");

    // Already sorted descending with duplicates
    assert(removeDuplicateLetters("zyxwzyx") == "wxyz");

    // Single character
    assert(removeDuplicateLetters("z") == "z");

    // Two characters with reverse order
    assert(removeDuplicateLetters("ba") == "ab");

    // Long string with many duplicates
    assert(removeDuplicateLetters("edcbaedcba") == "abcde");

    // Testing with a case where greedy removal matters
    assert(removeDuplicateLetters("bbac") == "bac");

    // Empty string is not expected per task, but if given, return empty
    // This is just a safety check (not specified, so we omit it).

    return 0;
}

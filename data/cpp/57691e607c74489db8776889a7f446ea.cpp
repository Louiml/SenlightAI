// Write a C++ function that takes two strings, `text` and `pattern`, representing a DNA sequence (containing only uppercase letters A, C, G, T) and a pattern to search for, respectively. The function must return the 0-based starting index of the first occurrence of `pattern` in `text` using the Bad Character Heuristic of the Boyer-Moore algorithm, or `-1` if the pattern is not found. Handle edge cases where the pattern is empty, longer than the text, or the text/pattern contains characters outside the DNA alphabet (the function should still search literally, treating any character as a valid symbol). Ensure the preprocessing step builds a shift table for each character in the pattern except the last one, where the shift equals `(pattern length - 1 - index)`. During the search, if a mismatch occurs at a text character, shift by its precomputed value if present, otherwise shift by the pattern length. The function must not modify its input strings and should use `const` references. Provide a concise implementation with appropriate comments.

The solution uses the Boyer-Moore Bad Character Heuristic, which preprocesses the pattern to build an `unordered_map<char, int>` where each character in the pattern (excluding the last) maps to the number of positions the pattern can safely shift when that character is mismatched. The search starts from the end of the pattern aligned with the text at index `m-1` and compares characters from right to left. If all characters match, the starting index is returned. If a mismatch occurs, the bad character is the text character at the current position; if it appears in the map, shift by its stored value, otherwise shift by `m`. Edge cases: an empty pattern should return `0` (conventionally, empty pattern matches at position 0), a pattern longer than text returns `-1`, and characters not in the DNA alphabet are treated as ordinary characters with no special handling. Time complexity is O(n*m) worst-case but O(n) average for large alphabets; space complexity is O(m) for the map (bounded by the number of distinct characters in the pattern, at most m).

#include <string>
#include <unordered_map>

// Builds the bad-character shift table for Boyer-Moore search.
std::unordered_map<char, int> buildShiftTable(const std::string& pattern) {
    const int m = static_cast<int>(pattern.size());
    std::unordered_map<char, int> table;
    // For each character except the last, store the shift distance.
    for (int i = 0; i < m - 1; ++i) {
        table[pattern[i]] = m - 1 - i;
    }
    return table;
}

// Returns the 0-based index of the first occurrence of pattern in text, or -1 if not found.
// Uses the Bad Character Heuristic of the Boyer-Moore algorithm.
int findPatternBoyerMoore(const std::string& text, const std::string& pattern) {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());

    // Edge case: empty pattern matches at position 0.
    if (m == 0) return 0;
    // Edge case: pattern longer than text.
    if (m > n) return -1;

    const std::unordered_map<char, int> shift = buildShiftTable(pattern);
    int i = m - 1; // Align pattern's last character with text index m-1.

    while (i < n) {
        int k = 0;
        // Compare pattern from right to left.
        while (k < m && pattern[m - 1 - k] == text[i - k]) {
            ++k;
        }
        if (k == m) {
            return i - m + 1; // All characters matched.
        }
        // Determine shift based on the bad character.
        const char bad_char = text[i];
        const auto it = shift.find(bad_char);
        if (it != shift.end()) {
            i += it->second;
        } else {
            i += m; // Character not in pattern – shift by pattern length.
        }
    }
    return -1; // Not found.
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic DNA search.
    assert(findPatternBoyerMoore("ATCGATCGTACGATCG", "TACG") == 9);
    assert(findPatternBoyerMoore("AAAAB", "AAAB") == 1);
    assert(findPatternBoyerMoore("GATTACA", "GATTACA") == 0);

    // Pattern not found.
    assert(findPatternBoyerMoore("ATCG", "CGTA") == -1);

    // Edge cases: empty pattern, pattern longer than text, single character.
    assert(findPatternBoyerMoore("ACGT", "") == 0);
    assert(findPatternBoyerMoore("AC", "ACG") == -1);
    assert(findPatternBoyerMoore("A", "A") == 0);
    assert(findPatternBoyerMoore("A", "C") == -1);

    // Characters outside DNA alphabet still work literally.
    assert(findPatternBoyerMoore("XYZXYZ", "ZXY") == 2);
    assert(findPatternBoyerMoore("abcde", "cd") == 2);

    // Pattern with repeated characters.
    assert(findPatternBoyerMoore("AABAA", "ABA") == 2);
    assert(findPatternBoyerMoore("TTTTTT", "TT") == 0);

    return 0;
}

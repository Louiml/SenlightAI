Write a C++ function `int minMismatchOverAlignment(const std::string& shortStr, const std::string& longStr)` that, given two strings consisting only of lowercase letters and spaces (where spaces act as wildcards that can match any character), returns the minimum number of mismatched characters when the shorter string is aligned anywhere along the longer string. The shorter string is guaranteed to be non-empty and its length is less than or equal to the length of the longer string. A character in the short string is a mismatch if it is not a space and it differs from the character directly below it in the aligned position of the long string. You must slide the short string across every possible starting position in the long string (starting indices from 0 to `longLen - shortLen` inclusive) and compute the total mismatches for each alignment, returning the smallest total found. Spaces in the short string always match any character and are never counted as mismatches. Spaces in the long string are just regular characters (they only match if the short character is also a space, but since short spaces are wildcards, a short space over a long space is not a mismatch; a non-space short character over a long space is a mismatch).

#include <cassert>
#include <string>

// Function declaration from the solution (would be in the same translation unit)
int minMismatchOverAlignment(const std::string& shortStr, const std::string& longStr);

int main() {
    // Basic exact match
    assert(minMismatchOverAlignment("abc", "abcdef") == 0);
    // No match possible (all positions differ)
    assert(minMismatchOverAlignment("abc", "zzzabc") == 3);
    // Wildcard spaces ignore long string characters
    assert(minMismatchOverAlignment("a c", "axc") == 0);
    // Only one matching alignment at the end
    assert(minMismatchOverAlignment("xy", "zxy") == 0);
    // All spaces are wildcards, always zero
    assert(minMismatchOverAlignment("   ", "anything here") == 0);
    // Equal length strings, only one alignment
    assert(minMismatchOverAlignment("abc", "abd") == 1);
    // Longer short string than long? Not allowed, but test valid case with long containing spaces
    assert(minMismatchOverAlignment("ab ", " a b") == 1); // best alignment: "ab " over "a b" -> positions: a vs a, b vs space (mismatch), space vs b (wildcard) => 1
    // Sliding finds best middle alignment
    assert(minMismatchOverAlignment("cat", "concatenate") == 0); // "cat" aligns at index 5? Actually "cat" is in "concatenate" -> positions 5-7 = "cat", yes
    // Mixed case where best is not at start or end
    assert(minMismatchOverAlignment("ab", "baab") == 0); // "ab" at index 2
    // Verify no overflow with large strings (sanity check)
    assert(minMismatchOverAlignment("a", "bbbbbbbbbb") == 1); // one mismatch regardless

    return 0;
}

#include <string>
#include <algorithm>

// Returns the minimum number of mismatches when aligning shortStr anywhere over longStr.
// Spaces in shortStr are wildcards and never count as mismatches.
int minMismatchOverAlignment(const std::string& shortStr, const std::string& longStr) {
    const int shortLen = static_cast<int>(shortStr.size());
    const int longLen = static_cast<int>(longStr.size());

    int best = longLen; // Upper bound: all characters could mismatch in some alignment

    for (int start = 0; start <= longLen - shortLen; ++start) {
        int currentMismatches = 0;
        for (int j = 0; j < shortLen; ++j) {
            const char s = shortStr[j];
            if (s == ' ') {
                continue; // wildcard always matches
            }
            if (s != longStr[start + j]) {
                ++currentMismatches;
            }
        }
        best = std::min(best, currentMismatches);
    }

    return best;
}

// The core algorithm is a sliding window comparison. For each possible starting index `i` in the longer string (from 0 to `longLen - shortLen` inclusive), iterate over every character position `j` in the shorter string. For each pair `(shortStr[j], longStr[i + j])`, count a mismatch if `shortStr[j]` is not a space and it is not equal to `longStr[i + j]`. Keep a running sum of mismatches for the current alignment and update a global minimum after each alignment. Edge cases: if the shorter string is all spaces, the result is always 0 because every alignment has zero mismatches. If the strings are equal in length, there is exactly one alignment. The total number of comparisons is proportional to `(longLen - shortLen + 1) * shortLen`, which in the worst case when `shortLen` is about half of `longLen`, is O(n²) where n is the length of the longer string. Space complexity is O(1) extra, aside from input strings.

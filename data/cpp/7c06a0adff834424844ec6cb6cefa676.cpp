Write a C++ function that takes two non-empty strings, `text` and `pattern`, and returns an integer count of how many times `pattern` appears as a substring in `text`, allowing overlapping occurrences. The function must be case-sensitive and must not modify the input strings. Use a single-pass Z-algorithm approach for efficiency. The function should be named `countPatternOccurrences` and should have the signature `int countPatternOccurrences(const std::string& text, const std::string& pattern)`. The input strings may contain any printable ASCII characters, including spaces (but the function should not consider any delimiters—the entire string is searched as-is). The pattern length will be at least 1, and the text length may be less than, equal to, or greater than the pattern length. Overlapping matches must be counted (e.g., in "aaa" with pattern "aa", the result is 2 occurrences).

The solution uses the Z-algorithm, which computes for each position in a combined string the length of the longest substring starting at that position that matches the prefix of the string. We concatenate the pattern, a unique delimiter (e.g., `$` that cannot appear in the pattern or text), and the text. Then we compute the Z-array of this combined string. For every index in the portion corresponding to the text (i.e., after the delimiter), if the Z-value equals the pattern length, it means a full match of the pattern starts at that position in the text. Because the Z-array algorithm naturally handles overlaps (it extends matches as far as possible without shifting the window prematurely), overlapping occurrences are counted correctly. Edge cases: if the text is shorter than the pattern, the loop over text positions still executes but no Z-value will equal the pattern length, returning 0. If the pattern is longer than the text, same result. The delimiter must be a character not in either string; we can use a sentinel like `char(0)` or `$` assuming inputs do not contain it, but to be safe, we can choose a separator like `'\n'` or a non-printable character. The time complexity is O(|pattern| + |text|) because the Z-algorithm is linear, and the space complexity is O(|pattern| + |text|) for the combined string and Z-array. The implementation avoids any copying of the original strings.

#include <string>
#include <vector>

// Compute the Z-array for a given string.
// Z[i] = length of the longest substring starting at position i
// that matches the prefix of the string.
std::vector<int> computeZ(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> z(n, 0);
    int left = 0, right = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= right) {
            // Use previously computed value, but bound by remaining right segment.
            int k = i - left;
            if (z[k] < right - i + 1) {
                z[i] = z[k];
                continue;
            }
            // Otherwise start comparing from right+1.
            left = i;
        }
        // Naive comparison from current position.
        while (right + 1 < n && s[right + 1] == s[right + 1 - left]) {
            ++right;
        }
        z[i] = right - left + 1;
        if (i > right) {
            left = i;
            right = i - 1; // Will be incremented in comparison
        }
    }
    return z;
}

// Count occurrences of pattern in text using the Z-algorithm.
// Overlapping matches are counted.
int countPatternOccurrences(const std::string& text, const std::string& pattern) {
    // Choose a delimiter guaranteed not to be in pattern or text.
    // Use a character unlikely to appear: '\0' is safe as C++ strings can hold it.
    const char delimiter = '\0';
    std::string combined = pattern + delimiter + text;
    int patternLen = static_cast<int>(pattern.size());
    int textLen = static_cast<int>(text.size());

    std::vector<int> z = computeZ(combined);

    int count = 0;
    // Iterate over positions in the text part (after pattern + delimiter).
    for (int i = patternLen + 1; i < patternLen + 1 + textLen; ++i) {
        if (z[i] == patternLen) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

int main() {
    // Basic cases
    assert(countPatternOccurrences("hello world", "world") == 1);
    assert(countPatternOccurrences("hello world", "hello") == 1);
    assert(countPatternOccurrences("abcabc", "abc") == 2);
    
    // Overlapping matches
    assert(countPatternOccurrences("aaa", "aa") == 2);
    assert(countPatternOccurrences("aaaa", "aa") == 3);
    
    // Pattern longer than text
    assert(countPatternOccurrences("abc", "abcd") == 0);
    
    // Empty text (though task says non-empty, but test edge)
    assert(countPatternOccurrences("", "a") == 0);
    
    // Single character
    assert(countPatternOccurrences("a", "a") == 1);
    assert(countPatternOccurrences("a", "b") == 0);
    
    // Spaces and punctuation
    assert(countPatternOccurrences("a b a b", "a b") == 2);
    assert(countPatternOccurrences("x y z", " ") == 2);  // spaces as pattern
    
    // Pattern equal to text
    assert(countPatternOccurrences("pattern", "pattern") == 1);
    
    // Many non-overlapping
    assert(countPatternOccurrences("abababab", "ab") == 4);
}

/*
Write a standalone C++ function named `runLengthEncode` that takes a non-empty `std::string` as input, which consists only of lowercase English letters, and returns a new string that represents the run-length encoding of the input. The encoding must follow this precise format: for each maximal sequence of consecutive identical characters, append the count of characters followed immediately by the character itself. For example, `"aaabbcccaab"` becomes `"3a2b3c2a1b"`. The function must handle counts greater than 9 correctly (e.g., a run of 12 `'x'` characters should produce `"12x"`), must process input of length 1 correctly (e.g., `"a"` → `"1a"`), and must not modify the original string. The function should be efficient for very long strings, preserving the original order of characters.
*/

#include <string>

// Perform run-length encoding on a non-empty lowercase string.
// Returns a string with count followed by character for each run.
std::string runLengthEncode(const std::string& input) {
    if (input.empty()) {
        return "";
    }

    std::string encoded;
    int runLength = 1;

    for (std::size_t i = 1; i < input.size(); ++i) {
        if (input[i] == input[i - 1]) {
            ++runLength;
        } else {
            encoded += std::to_string(runLength) + input[i - 1];
            runLength = 1;
        }
    }

    // Append the last run
    encoded += std::to_string(runLength) + input.back();
    return encoded;
}

#include <cassert>
#include <string>

// Function declaration (solution provided above)
std::string runLengthEncode(const std::string& input);

int main() {
    // Basic example
    assert(runLengthEncode("aaabbcccaab") == "3a2b3c2a1b");
    // Single character
    assert(runLengthEncode("a") == "1a");
    // All same characters
    assert(runLengthEncode("zzzz") == "4z");
    // No adjacent duplicates
    assert(runLengthEncode("abc") == "1a1b1c");
    // Count greater than 9
    assert(runLengthEncode("xxxxxxxxxxxx") == "12x");
    // Mixed long runs and short runs
    assert(runLengthEncode("ppppppqqqqqqqqqq") == "6p10q");
    // Two-character string with different chars
    assert(runLengthEncode("ab") == "1a1b");
    // Two-character string with same chars
    assert(runLengthEncode("cc") == "2c");
    // Longer string with pattern
    assert(runLengthEncode("aaabbbaa") == "3a3b2a");
    // Edge case: run of 10 identical characters
    assert(runLengthEncode("dddddddddd") == "10d");

    return 0;
}

// The solution iterates through the input string exactly once, tracking the current character and its run length. Starting with the first character, set a count to 1, then for each subsequent character, compare it to the previous one. If it matches, increment the count; otherwise, append the current count (converted to its decimal string representation) followed by the previous character to the result string, then reset the count to 1 for the new character. After the loop, append the final count and character. Key edge cases: (1) a single-character string—the loop does not run, and the final append handles it; (2) counts ≥10—converting the integer count to a string via `std::to_string` correctly produces multi-digit numbers without needing manual digit extraction; (3) input with all identical characters—the loop just increments count, and the final append writes the total; (4) the original string is never altered because we only read from it and build a new string. Time complexity is O(n) where n is the length of the input, since we traverse the string once and each appending operation takes amortized constant time. Space complexity is O(n) for the output string, which is necessary to store the encoded result (the worst case is when no adjacent characters match, e.g., "abcdef", producing "1a1b1c1d1e1f").

// Write a C++ function that simulates a simplified text-editing comparison. The function receives two vectors of strings: `typedLines` and `referenceLines`. Each line in `typedLines` represents what a user typed, where the character `'<'` acts as a backspace that deletes the previously typed character (if any; if the current line is empty, a backspace has no effect). The function must process each line independently to produce a "final typed line." It then compares each processed typed line with the corresponding reference line (pairwise by index) and counts the number of matching characters at the same positions (only up to the length of the shorter line). The function should return an integer representing the total match count across all lines. The input vectors may have different lengths; if `typedLines` has more lines than `referenceLines`, ignore the extra typed lines; if it has fewer, only compare the available pairs. Lines may be empty, and the character `'<'` may appear anywhere in a line (including at the very start). The vectors are read-only inputs, and the function should not modify them. For example, if a typed line is `"ab<cd"`, the backspace removes `'b'`, resulting in `"acd"`. If the reference line is `"acx"`, then comparing `acd` vs `acx` yields a match count of 2 (positions 0 and 1). The total count across all pairs is returned.

// The core algorithm processes each typed line from left to right, building a new string `t`. For each character: if it is `'<'`, we pop the last character from `t` (if `t` is not empty); otherwise, we append the character to `t`. This simulates the backspace behavior exactly. After processing a typed line, we compare it with the corresponding reference line (if it exists) by iterating over indices from `0` to `min(processedLine.length(), referenceLine.length())-1`, and increment a global counter whenever the characters at that index are equal. Edge cases include empty typed lines (the loop does nothing, `t` remains empty), leading backspaces (pop on empty string does nothing), and lines with multiple consecutive backspaces (each removes one character). If `typedLines` has more lines than `referenceLines`, we simply skip processing extra lines (or process them but ignore the count) — the function should only count up to the number of pairs available, which is `min(typedLines.size(), referenceLines.size())`. For `n` total characters across all typed lines and `m` total characters in compared reference lines, the time complexity is `O(n + m)` because each character is visited once for processing and once for comparison. Space complexity is `O(L)` where `L` is the length of the longest processed line, since we build a temporary string for each line (worst case if no backspaces, the temporary string length equals the line length). If we consider the output (the total count), no extra space is needed beyond the temporary string.

#include <string>
#include <vector>
#include <algorithm>

// Returns the total number of matching characters between processed typed lines
// and reference lines, processing backspace characters '<' in typed lines.
// Lines are compared pairwise by index, up to the shorter of the two vectors.
int countTypingMatches(const std::vector<std::string>& typedLines,
                       const std::vector<std::string>& referenceLines) {
    int totalMatches = 0;
    const size_t pairs = std::min(typedLines.size(), referenceLines.size());

    for (size_t idx = 0; idx < pairs; ++idx) {
        const std::string& typed = typedLines[idx];
        const std::string& reference = referenceLines[idx];

        // Process backspaces in the typed line
        std::string processed;
        processed.reserve(typed.size());
        for (char ch : typed) {
            if (ch == '<') {
                if (!processed.empty()) {
                    processed.pop_back();
                }
            } else {
                processed.push_back(ch);
            }
        }

        // Count matching characters up to the length of the shorter string
        const size_t compareLen = std::min(processed.size(), reference.size());
        for (size_t i = 0; i < compareLen; ++i) {
            if (processed[i] == reference[i]) {
                ++totalMatches;
            }
        }
    }
    return totalMatches;
}

#include <cassert>
#include <string>
#include <vector>

// Include the solution function here or in a header.

int main() {
    // Basic case
    std::vector<std::string> typed1 = {"ab<cd"};
    std::vector<std::string> ref1 = {"acx"};
    assert(countTypingMatches(typed1, ref1) == 2);

    // Leading backspace does nothing
    std::vector<std::string> typed2 = {"<abc"};
    std::vector<std::string> ref2 = {"abc"};
    assert(countTypingMatches(typed2, ref2) == 3);

    // Multiple backspaces that remove all characters
    std::vector<std::string> typed3 = {"ab<<", "x"};
    std::vector<std::string> ref3 = {"", "y"};
    assert(countTypingMatches(typed3, ref3) == 0);

    // Empty lines and different vector lengths
    std::vector<std::string> typed4 = {"", "a", "b"};
    std::vector<std::string> ref4 = {"", "a"};
    assert(countTypingMatches(typed4, ref4) == 1); // first pair empty, second pair "a" vs "a"

    // Reference longer than typed, typed longer than reference
    std::vector<std::string> typed5 = {"abc"};
    std::vector<std::string> ref5 = {"abcd"};
    assert(countTypingMatches(typed5, ref5) == 3);

    std::vector<std::string> typed6 = {"abcd"};
    std::vector<std::string> ref6 = {"abc"};
    assert(countTypingMatches(typed6, ref6) == 3);

    // No pairs because one vector is empty
    std::vector<std::string> typed7 = {"a"};
    std::vector<std::string> ref7 = {};
    assert(countTypingMatches(typed7, ref7) == 0);

    std::vector<std::string> typed8 = {};
    std::vector<std::string> ref8 = {"a"};
    assert(countTypingMatches(typed8, ref8) == 0);

    // Backspace at beginning and then normal chars
    std::vector<std::string> typed9 = {"<<hello"};
    std::vector<std::string> ref9 = {"hello"};
    assert(countTypingMatches(typed9, ref9) == 5);

    // Mixed backspaces and matching with spaces
    std::vector<std::string> typed10 = {"a< b", "c d"};
    std::vector<std::string> ref10 = {" b", "c d"};
    // typed10[0] -> "a< b" -> 'a', '<' removes 'a', then space, 'b' -> " b" matches " b" => 2
    // typed10[1] -> "c d" matches "c d" => 3
    assert(countTypingMatches(typed10, ref10) == 5);

    return 0;
}

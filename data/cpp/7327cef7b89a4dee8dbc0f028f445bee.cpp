// Write a C++ function that takes a vector of strings representing words and an integer `maxWidth` (where each word's length is at most `maxWidth` and `maxWidth >= 1`), and returns a vector of strings representing the fully justified text. Each line must have exactly `maxWidth` characters. For lines with multiple words, spaces are distributed as evenly as possible between words, with extra spaces added from left to right. The last line is left-justified with all extra spaces appended at the end. If a line contains only one word, pad it with trailing spaces. Preserve the original word order and do not change any word's characters. The function should handle empty word vectors and cases where a single word fills the entire width.
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Single word, padded
    std::vector<std::string> words1 = {"hello"};
    std::vector<std::string> expected1 = {"hello     "};
    assert(fullJustify(words1, 10) == expected1);

    // Empty vector
    std::vector<std::string> words2;
    assert(fullJustify(words2, 5).empty());

    // Standard example
    std::vector<std::string> words3 = {"This", "is", "an", "example", "of", "text", "justification."};
    std::vector<std::string> expected3 = {"This    is    an", "example  of text", "justification.  "};
    assert(fullJustify(words3, 16) == expected3);

    // Word exactly fits width
    std::vector<std::string> words4 = {"exact"};
    std::vector<std::string> expected4 = {"exact"};
    assert(fullJustify(words4, 5) == expected4);

    // Two words with uneven spaces
    std::vector<std::string> words5 = {"a", "b", "c"};
    std::vector<std::string> expected5 = {"a   b   c"};
    assert(fullJustify(words5, 9) == expected5);

    // Last line left-justified
    std::vector<std::string> words6 = {"one", "two", "three"};
    std::vector<std::string> expected6 = {"one   two", "three    "};
    assert(fullJustify(words6, 8) == expected6);

    // All words fit on one line, not last line handling
    std::vector<std::string> words7 = {"word"};
    std::vector<std::string> expected7 = {"word"};
    assert(fullJustify(words7, 4) == expected7);

    // Multiple lines, last line has extra spaces
    std::vector<std::string> words8 = {"a", "b", "c", "d"};
    std::vector<std::string> expected8 = {"a b", "c d"};
    assert(fullJustify(words8, 3) == expected8);

    // Edge: maxWidth = 1, single letters each on own line
    std::vector<std::string> words9 = {"x", "y"};
    std::vector<std::string> expected9 = {"x", "y"};
    assert(fullJustify(words9, 1) == expected9);

    return 0;
}
#include <string>
#include <vector>

// Fully justify text given words and a target line width.
std::vector<std::string> fullJustify(const std::vector<std::string>& words, int maxWidth) {
    std::vector<std::string> result;
    std::vector<std::string> currentLine;
    int currentLength = 0;
    int wordCount = words.size();

    for (const std::string& word : words) {
        // Check if adding this word would exceed maxWidth (with at least one space before it if not first)
        if (!currentLine.empty() && currentLength + 1 + static_cast<int>(word.size()) > maxWidth) {
            // Justify the current line
            std::string line = "";
            int gapCount = static_cast<int>(currentLine.size()) - 1;
            int totalSpaces = maxWidth - currentLength;
            if (gapCount == 0) {
                // Single word case: pad with all spaces on the right
                line = currentLine[0] + std::string(totalSpaces, ' ');
            } else {
                int baseSpaces = totalSpaces / gapCount;
                int extraSpaces = totalSpaces % gapCount;
                for (int i = 0; i < static_cast<int>(currentLine.size()); ++i) {
                    line += currentLine[i];
                    if (i < gapCount) {
                        line += std::string(baseSpaces, ' ');
                        if (i < extraSpaces) {
                            line += ' ';
                        }
                    }
                }
            }
            result.push_back(line);
            currentLine.clear();
            currentLength = 0;
        }
        currentLine.push_back(word);
        currentLength += static_cast<int>(word.size());
    }

    // Handle the last line: left-justified with single spaces, then trailing spaces
    if (!currentLine.empty()) {
        std::string line = "";
        int totalWords = static_cast<int>(currentLine.size());
        int lineLength = currentLength + (totalWords - 1); // spaces between words
        for (int i = 0; i < totalWords; ++i) {
            if (i > 0) {
                line += ' ';
            }
            line += currentLine[i];
        }
        line += std::string(maxWidth - lineLength, ' ');
        result.push_back(line);
    }

    return result;
}
// The core algorithm processes words sequentially, grouping them into lines. For each word, we check if adding it to the current temporary line (including at least one space between each word) would exceed `maxWidth`. If it would, we first justify the current temporary line. The justification logic depends on the number of words in the line: if there's only one word, pad with spaces to the right; otherwise, compute the total spaces needed (`maxWidth - totalWordLengths`), divide evenly among gaps between words (`totalSpaces / (wordCount - 1)`), and distribute the remainder (`totalSpaces % (wordCount - 1)`) one extra space per gap from left to right. After justifying, we reset the temporary line and start a new one with the current word. After processing all words, the final temporary line (which is the last line) is left-justified: join words with a single space, then append trailing spaces to reach `maxWidth`. Edge cases include an empty word vector (return empty vector), a line with a single word, and the last line (which never gets fully justified). Time complexity is O(total characters), since each word is visited once and each output line's construction takes time proportional to its length. Space complexity is O(total output size), which is O(total characters plus number of lines times `maxWidth`).

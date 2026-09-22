/*
Write a C++ function named `reverseWordsInEachLine` that takes a vector of strings, where each string is a line of text containing words separated by single spaces, and returns a new vector of strings where the order of words in each line is reversed (i.e., the first word becomes last, the last becomes first). The function must preserve the exact spacing between words (exactly one space) and must not modify the input vector. Lines may be empty (zero words), in which case the output for that line must be an empty string. The original line ordering must be maintained. Assume each line contains only lowercase letters and spaces, with no leading or trailing spaces, except possibly an entire empty line.
*/
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Reverses the order of words in each line of the input vector.
// Each line contains lowercase letters separated by exactly one space,
// or is empty. Returns a new vector with the same line ordering.
std::vector<std::string> reverseWordsInEachLine(const std::vector<std::string>& lines) {
    std::vector<std::string> result;
    result.reserve(lines.size());

    for (const std::string& line : lines) {
        if (line.empty()) {
            result.push_back("");
            continue;
        }

        std::istringstream iss(line);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }

        std::reverse(words.begin(), words.end());

        std::ostringstream oss;
        for (size_t i = 0; i < words.size(); ++i) {
            if (i > 0) {
                oss << ' ';
            }
            oss << words[i];
        }
        result.push_back(oss.str());
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
int main() {
    // Test 1: normal multiple words
    std::vector<std::string> input1 = {"hello world", "one two three"};
    std::vector<std::string> expected1 = {"world hello", "three two one"};
    assert(reverseWordsInEachLine(input1) == expected1);

    // Test 2: single word per line
    std::vector<std::string> input2 = {"alpha", "beta"};
    std::vector<std::string> expected2 = {"alpha", "beta"};
    assert(reverseWordsInEachLine(input2) == expected2);

    // Test 3: empty lines preserved
    std::vector<std::string> input3 = {"", "abc", ""};
    std::vector<std::string> expected3 = {"", "abc", ""};
    assert(reverseWordsInEachLine(input3) == expected3);

    // Test 4: line with only one space? No, but test line with two words
    std::vector<std::string> input4 = {"a b", "x"};
    std::vector<std::string> expected4 = {"b a", "x"};
    assert(reverseWordsInEachLine(input4) == expected4);

    // Test 5: longer lines and ensure original not modified
    std::vector<std::string> input5 = {"the quick brown fox", "jumps over"};
    std::vector<std::string> copy5 = input5; // copy to verify non-modification by reference after calling
    std::vector<std::string> expected5 = {"fox brown quick the", "over jumps"};
    auto output5 = reverseWordsInEachLine(input5);
    assert(output5 == expected5);
    assert(input5 == copy5); // verify no modification

    // Test 6: words that are all same
    std::vector<std::string> input6 = {"go go go", "stop"};
    std::vector<std::string> expected6 = {"go go go", "stop"};
    assert(reverseWordsInEachLine(input6) == expected6);
}
// The main algorithm processes each line independently. For each line, we split it into words using a string stream (which handles the single-space separation). We then collect the words into a temporary vector, reverse that vector using `std::reverse`, and join the reversed words back with a single space using a `std::ostringstream`. Edge cases include empty lines (where we push back an empty string), lines with a single word (reversal yields the same word), and multiple words with exactly one space between them (which we handle inherently by tokenizing with `>>`). Time complexity is O(total number of characters) since each character is read and written once; space complexity is O(max line length) for the temporary word vector.

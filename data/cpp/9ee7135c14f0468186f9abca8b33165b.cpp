Write a C++ function named `longestWordBeforePeriod` that reads words from standard input one at a time (using `std::cin >> word`), where the input ends when a word ends with a period (`.`). For the final word, remove the trailing period before considering its length. Among all words entered (including the final one after stripping its period), the function must return the first word that has the maximum length. If multiple words tie for the maximum length, the earliest one encountered should be returned. Words consist only of lowercase and uppercase letters and may be up to 19 characters long (before the period, which is not counted). The input is guaranteed to contain at least one word, and the final word will always end with a period. The function should return a `std::string` containing the selected word (without any period). For example, if the input is `cat dog elephant mouse.`, the output should be `elephant`; if the input is `ab abc abcd xyz.`, the output should be `abcd`.
#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

// The function under test is declared here (or included from a header).
std::string longestWordBeforePeriod();

// Helper to simulate input stream with the given string.
std::string runWithInput(const std::string& input) {
    std::istringstream iss(input);
    // Save and replace cin buffer
    auto old_buf = std::cin.rdbuf(iss.rdbuf());
    std::string result = longestWordBeforePeriod();
    std::cin.rdbuf(old_buf);
    return result;
}

int main() {
    assert(runWithInput("cat dog elephant mouse.") == "elephant");
    assert(runWithInput("ab abc abcd xyz.") == "abcd");
    assert(runWithInput("hi.") == "hi");
    assert(runWithInput("a bb ccc dd ee.") == "ccc");
    assert(runWithInput("same same same.") == "same");
    assert(runWithInput("longer than all.") == "longer");  // "than" length 4, "longer" length 6
    assert(runWithInput("short long longest.") == "longest");
    assert(runWithInput("A zz AAA.") == "AAA");  // case sensitive, AAA length 3
    assert(runWithInput("x yy zzz.") == "zzz");
    assert(runWithInput("one two three.") == "three");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <string>

// Reads words from standard input until one ends with '.', removes that period,
// and returns the first word with the maximum length (ties broken by earliest occurrence).
std::string longestWordBeforePeriod() {
    std::string word;
    std::string best;
    size_t maxLen = 0;
    bool done = false;

    while (!done && std::cin >> word) {
        // Check if the word ends with a period.
        if (!word.empty() && word.back() == '.') {
            // Remove the trailing period.
            word.pop_back();
            done = true;
        }

        // Update the best word if this one is strictly longer.
        if (word.length() > maxLen) {
            maxLen = word.length();
            best = word;
        }
    }

    return best;
}
// The solution reads words in a loop. For each word, we first check if the last character is a period. If so, we remove the period by taking a substring of the word without the last character, and set a flag to terminate the loop after processing this word. Otherwise, we keep the word as is. The length of the (possibly trimmed) word is then compared to the current maximum length. If it is strictly greater, we update the maximum length and store this word as the candidate. Because we update only on strict greater, the first occurrence of the maximum is retained. Edge cases: a word that is just a single period would become an empty string after stripping, but the problem guarantees at least one word and the final word ends with a period, so a word like `.` is possible? The statement says "non-empty string" for words, but the final word could be a single character plus period, e.g., `a.` becomes `a`. If the input were just `.`, that would be an empty word, which is not allowed, so we can assume the stripped final word has at least one character. Time complexity is O(total length of all input words) because we iterate through each word's characters to check for period and to copy for the return value. Space complexity is O(max word length) for the stored candidate, plus the input words themselves. Using `std::string` and `std::cin` is straightforward.

/*
Write a C++ function that accepts an array of strings, where each string is a word of arbitrary case, and returns a vector of strings indicating whether each input word is exactly "YES" when case-insensitively compared. The output for each input must be the string "YES" if the word equals "YES" ignoring case, otherwise "NO". Preserve the original order of inputs. The function should handle empty strings, mixed-case inputs like "yEs", "YeS", and non-alphabetic characters (e.g., "YES!"), correctly returning "NO" for those that do not exactly match "YES" after case conversion.
*/
#include <string>
#include <vector>
#include <cctype>

// Returns a vector where each element is "YES" if the corresponding input
// string equals "YES" ignoring case, otherwise "NO".
std::vector<std::string> checkYesWords(const std::vector<std::string>& words) {
    std::vector<std::string> result;
    result.reserve(words.size());

    for (const std::string& word : words) {
        std::string upper = word;
        for (char& c : upper) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        if (upper == "YES") {
            result.push_back("YES");
        } else {
            result.push_back("NO");
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic cases
    std::vector<std::string> input1 = {"YES", "yes", "Yes", "yEs", "yeS", "YEs", "yES"};
    std::vector<std::string> expected1 = {"YES", "YES", "YES", "YES", "YES", "YES", "YES"};
    assert(checkYesWords(input1) == expected1);

    // Incorrect words
    std::vector<std::string> input2 = {"NO", "no", "yess", "ye", "Y", "ES", "Y E S", "YES "};
    std::vector<std::string> expected2 = {"NO", "NO", "NO", "NO", "NO", "NO", "NO", "NO"};
    assert(checkYesWords(input2) == expected2);

    // Mixed with punctuation and empty string
    std::vector<std::string> input3 = {"yes!", "YES?", "Yes.", ""};
    std::vector<std::string> expected3 = {"NO", "NO", "NO", "NO"};
    assert(checkYesWords(input3) == expected3);

    // Single element
    std::vector<std::string> input4 = {"YE S"};
    assert(checkYesWords(input4) == std::vector<std::string>{"NO"});

    // Already uppercase and exact match
    std::vector<std::string> input5 = {"YES"};
    assert(checkYesWords(input5) == std::vector<std::string>{"YES"});

    // Large mixed set
    std::vector<std::string> input6 = {"yes", "NO", "yEs", "YE", "YES", "yesno"};
    std::vector<std::string> expected6 = {"YES", "NO", "YES", "NO", "YES", "NO"};
    assert(checkYesWords(input6) == expected6);

    // Empty input vector
    std::vector<std::string> input7;
    assert(checkYesWords(input7).empty());

    // All lower-case "yes" with trailing newline (treated as part of string)
    std::vector<std::string> input8 = {"yes\n"};
    assert(checkYesWords(input8) == std::vector<std::string>{"NO"});
}
// The solution is straightforward: for each input string, convert every character to uppercase using the standard `std::toupper` function (casting to `unsigned char` to avoid undefined behavior for negative char values). Then compare the converted string with the literal "YES". If equal, append "YES" to the result; otherwise append "NO". Edge cases include an empty string (which becomes an empty uppercase string and thus is not "YES"), strings with punctuation or spaces (which become uppercase but still differ from "YES"), and strings that are already "YES" or "YES" with extra whitespace (which after conversion differ because of the extra characters). The algorithm processes each character exactly once, so for a total of \(n\) characters across all strings, time complexity is \(O(n)\). Space complexity is \(O(m)\) for the output vector and the temporary uppercase strings, where \(m\) is the total number of output characters (proportional to the input size). No special memory is needed beyond copies of the inputs.

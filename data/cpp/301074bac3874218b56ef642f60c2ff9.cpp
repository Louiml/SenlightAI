Write a C++ function that takes a vector of strings and processes each string according to the following rule: if a string's length is greater than 10 characters, replace it with its first character, the number of characters between the first and last (i.e., length minus 2), and its last character, all concatenated without spaces (e.g., "internationalization" becomes "i18n"). If the string's length is 10 or fewer, keep it unchanged. The function should return a new vector of strings containing the processed versions, preserving the original order. The input vector may contain any number of strings (including empty), and each string may consist of printable ASCII characters (no spaces). Assume the input is valid and does not need validation.
The main algorithm iterates over each string in the input vector. For each string, check its size. If the size is greater than 10, construct a new string that starts with the first character, then converts the value (size - 2) to a string using `std::to_string`, then appends the last character. This uses the fact that Python-style abbreviations like `a-2` in the snippet are actually meant to represent `size-2` as a number, not literal arithmetic characters. If the size is not greater than 10, copy the original string unchanged. Edge cases: an empty string has size 0, which is ≤10, so it is unchanged. A string of length exactly 10 is unchanged. A string of length 11 becomes `first char + 9 + last char` (e.g., "abcdefghijk" → "a9k"). Strings with non-alphanumeric characters are handled fine because we only access by index. Complexity: O(total number of characters across all strings), because we iterate each string once and for long strings we build a new string of length at most that string's length. Auxiliary space: O(total length of output strings), which is at most the input size, so linear.
#include <string>
#include <vector>

// Process each string: abbreviate if longer than 10 characters.
std::vector<std::string> abbreviateLongStrings(const std::vector<std::string>& input) {
    std::vector<std::string> result;
    result.reserve(input.size());
    for (const auto& str : input) {
        if (str.size() > 10) {
            result.push_back(str.front() + std::to_string(str.size() - 2) + str.back());
        } else {
            result.push_back(str);
        }
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Use the solution function
std::vector<std::string> abbreviateLongStrings(const std::vector<std::string>& input);

int main() {
    // Test basic abbreviation
    assert(abbreviateLongStrings({"internationalization"}) == std::vector<std::string>{"i18n"});
    // Test mixed lengths
    assert(abbreviateLongStrings({"hello", "supercalifragilistic"}) == 
           std::vector<std::string>{"hello", "s18c"});
    // Test length exactly 10
    assert(abbreviateLongStrings({"abcdefghij"}) == std::vector<std::string>{"abcdefghij"});
    // Test length 11
    assert(abbreviateLongStrings({"abcdefghijk"}) == std::vector<std::string>{"a9k"});
    // Test empty string
    assert(abbreviateLongStrings({""}) == std::vector<std::string>{""});
    // Test single-character strings
    assert(abbreviateLongStrings({"a", "z"}) == std::vector<std::string>{"a", "z"});
    // Test non-alphanumeric characters
    assert(abbreviateLongStrings({"@#$%^&*()!"}) == std::vector<std::string>{"@9!"});
    // Test multiple long strings and order
    assert(abbreviateLongStrings({"aaaaaaaaaaa", "bb", "ccccccccccc"}) == 
           std::vector<std::string>{"a9a", "bb", "c9c"});
    // Test empty vector
    assert(abbreviateLongStrings({}).empty());
    // Test all long with special characters
    assert(abbreviateLongStrings({"ABCDEFGHIJKLMNOP", "xyz"}) == 
           std::vector<std::string>{"A14P", "xyz"});
    return 0;
}

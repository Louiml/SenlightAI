Write a standalone C++ function named `findLongestCommonPrefix` that, given a vector of strings, returns the longest common prefix string. If the vector is empty, return an empty string. If the vector contains only one string, return that entire string. The function must handle strings of arbitrary length and content, including strings that share no common prefix (in which case an empty string is returned). The solution must use iterative character-by-character comparison across all strings, and must not rely on sorting or any external libraries beyond standard headers.

The core approach is to first check if the vector is empty (return empty string) or has one element (return that element). Otherwise, take the first string as an initial prefix candidate. Then, for each subsequent string, repeatedly shorten the candidate prefix while the current string does not start with it. A more efficient direct character-wise approach is: iterate over character indices from 0 up to the length of the first string; for each index, compare that character against the same index in every other string. If any string is shorter or has a mismatched character at that position, stop and return the prefix built so far (characters 0 through index-1). This is O(S * L) where S is the number of strings and L is the length of the shortest string, since we stop at the first mismatch. Time complexity is O(S * L) in the worst case (all strings equal long). Space complexity is O(1) auxiliary, excluding the storage for the returned prefix string. Edge cases include empty input, single string, one empty string among others (immediately returns empty), and all strings identical.

#include <string>
#include <vector>

// Returns the longest common prefix among all strings in the vector.
// Returns an empty string if the vector is empty or if no common prefix exists.
std::string findLongestCommonPrefix(const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return "";
    }
    if (strings.size() == 1) {
        return strings[0];
    }

    // Use the first string as the reference for length.
    const std::string& first = strings[0];
    std::string prefix;

    for (std::size_t i = 0; i < first.size(); ++i) {
        char currentChar = first[i];
        // Check this character against all other strings.
        for (std::size_t j = 1; j < strings.size(); ++j) {
            // If another string is shorter or has a different character at this position,
            // we've reached the end of the common prefix.
            if (i >= strings[j].size() || strings[j][i] != currentChar) {
                return prefix;
            }
        }
        // All strings match at this position, so append to prefix.
        prefix.push_back(currentChar);
    }

    return prefix;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above (in a real setup). Here we provide a main for testing.
int main() {
    // Standard multiple strings with common prefix
    std::vector<std::string> test1 = {"flower", "flow", "flight"};
    assert(findLongestCommonPrefix(test1) == "fl");

    // No common prefix
    std::vector<std::string> test2 = {"dog", "racecar", "car"};
    assert(findLongestCommonPrefix(test2) == "");

    // Single string
    std::vector<std::string> test3 = {"single"};
    assert(findLongestCommonPrefix(test3) == "single");

    // Empty vector
    std::vector<std::string> test4;
    assert(findLongestCommonPrefix(test4) == "");

    // All identical strings
    std::vector<std::string> test5 = {"same", "same", "same"};
    assert(findLongestCommonPrefix(test5) == "same");

    // One empty string among others
    std::vector<std::string> test6 = {"", "nonempty", "another"};
    assert(findLongestCommonPrefix(test6) == "");

    // Different lengths with prefix that is entire shorter string
    std::vector<std::string> test7 = {"abc", "abcdef", "abcd"};
    assert(findLongestCommonPrefix(test7) == "abc");

    // Strings with common prefix but then one has a null-character difference? Not needed, standard.

    // Prefix ends exactly at the end of the first string
    std::vector<std::string> test8 = {"ab", "abc", "ab"};
    assert(findLongestCommonPrefix(test8) == "ab");

    // Long strings with repeated characters
    std::vector<std::string> test9 = {"aaaa", "aaaa", "aaab"};
    assert(findLongestCommonPrefix(test9) == "aaa");

    // Single character common prefix
    std::vector<std::string> test10 = {"a", "ab", "ac"};
    assert(findLongestCommonPrefix(test10) == "a");

    return 0;
}

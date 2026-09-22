/*
Write a C++ function `std::string longestCommonPrefix(const std::vector<std::string>& strings)` that, given a vector of strings (which may be empty), returns the longest common prefix shared by all strings. If the vector is empty, return an empty string. If no common prefix exists (e.g., one string starts with a different character than another), return an empty string. The function must be `const`-correct and not modify the input. The strings may contain any ASCII printable characters, including digits, punctuation, and spaces. The function should handle cases where one string is a prefix of another (e.g., `{"abc", "abcd"}` should return `"abc"`).
*/
#include <string>
#include <vector>

// Returns the longest common prefix of all strings in the input vector.
// If the vector is empty or no common prefix exists, returns an empty string.
std::string longestCommonPrefix(const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return {};
    }

    std::string prefix;
    const std::string& first = strings[0];

    for (std::size_t i = 0; i < first.size(); ++i) {
        const char current = first[i];

        // Compare with the same position in all other strings.
        for (std::size_t j = 1; j < strings.size(); ++j) {
            // If another string is shorter or has a different character, stop.
            if (i >= strings[j].size() || strings[j][i] != current) {
                return prefix;
            }
        }

        // The character matches in all strings, so extend the prefix.
        prefix.push_back(current);
    }

    return prefix;
}
#include <cassert>
#include <string>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic case with common prefix
    assert(longestCommonPrefix({"flower", "flow", "flight"}) == "fl");

    // No common prefix
    assert(longestCommonPrefix({"dog", "racecar", "car"}) == "");

    // Empty vector
    assert(longestCommonPrefix({}) == "");

    // Single string
    assert(longestCommonPrefix({"alone"}) == "alone");

    // One string is a prefix of another
    assert(longestCommonPrefix({"interspecies", "interstellar", "interstate"}) == "inters");

    // All identical strings
    assert(longestCommonPrefix({"same", "same", "same"}) == "same");

    // Shortest string limits the prefix
    assert(longestCommonPrefix({"abc", "abd", "ab"}) == "ab");

    // Strings with spaces and punctuation
    assert(longestCommonPrefix({"a b", "a c", "a d"}) == "a ");

    // Empty string in the vector
    assert(longestCommonPrefix({"", "anything"}) == "");

    // Case sensitivity matters
    assert(longestCommonPrefix({"Prefix", "prefix"}) == "");

    // Only one character common
    assert(longestCommonPrefix({"apple", "apricot", "april"}) == "ap");

    return 0;
}
// The solution iterates character-by-character over the first string, treating it as the candidate prefix. For each position `i`, we take the character from `strings[0][i]` and compare it against the character at the same position `i` in every other string. If any string is shorter than `i` or has a different character at that position, we stop and return the prefix accumulated so far. Otherwise, we append the character to the prefix. This works because the common prefix cannot extend beyond the shortest string, and if a mismatch is found at some position, no longer prefix is possible. Edge cases: empty vector returns empty string; a vector with a single string returns that string itself (the loop over `j` runs zero times, so it accumulates all characters); strings with no common first character return empty immediately; one string being a prefix of another is naturally handled because the shorter string will cause the index `i` to exceed its length, terminating the loop. Time complexity is \(O(S)\) where \(S\) is the total number of characters across all strings (in the worst case, all strings are identical and we examine every character of every string). Space complexity is \(O(1)\) auxiliary, excluding the returned prefix string itself.

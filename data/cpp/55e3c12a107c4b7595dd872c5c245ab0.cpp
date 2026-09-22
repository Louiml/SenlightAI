// Write a C++ function `size_t longestCommonSuffixLength(const std::vector<std::string>& strings)` that takes a vector of strings and returns the length of the longest common suffix shared by all strings in the vector. A common suffix is a sequence of characters that appears at the end of every string. If the vector is empty, return 0. If any string is empty, the common suffix length is 0 (since an empty string has no suffix characters). All characters are compared case-sensitively. For example, for strings `{"prefix-alpha", "suffix-alpha", "alpha"}`, the common suffix is `"alpha"` (length 5), but for `{"hello", "yellow", "mellow"}`, the common suffix is `"ello"` (length 4). The function must be const-correct, use only standard library facilities, and assume the input is a valid `std::vector<std::string>`.

The approach is to compare characters from the end of each string simultaneously. We initialize the suffix length counter to 0, then repeatedly check the character at position `len` from the end of each string. For iteration `i` (starting at 0), we compare `strings[j][strings[j].size() - 1 - i]` for all `j`. If all strings have at least `i+1` length and all those characters are equal, we increment the counter and continue; otherwise, we stop. Edge cases: empty vector returns 0; any empty string immediately returns 0 because no character can be compared (checking `size() > len` fails for the first iteration). Time complexity is O(n * L) where `n` is the number of strings and `L` is the length of the longest common suffix, since we compare each string character by character. Space complexity is O(1) auxiliary.

#include <vector>
#include <string>

// Returns the length of the longest common suffix shared by all strings in the vector.
// Returns 0 for an empty vector or if any string is empty.
size_t longestCommonSuffixLength(const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return 0;
    }
    // Check if any string is empty immediately.
    for (const auto& s : strings) {
        if (s.empty()) {
            return 0;
        }
    }

    size_t suffixLen = 0;
    while (true) {
        // Check if all strings have at least suffixLen + 1 characters.
        for (const auto& s : strings) {
            if (s.size() <= suffixLen) {
                return suffixLen;
            }
        }
        // Compare the character at position suffixLen from the end of each string.
        char expected = strings.front()[strings.front().size() - 1 - suffixLen];
        for (const auto& s : strings) {
            if (s[s.size() - 1 - suffixLen] != expected) {
                return suffixLen;
            }
        }
        ++suffixLen;
    }
}

#include <cassert>
#include <string>
#include <vector>

// Function declaration (definition would be above)
size_t longestCommonSuffixLength(const std::vector<std::string>& strings);

int main() {
    // Basic cases
    assert(longestCommonSuffixLength({"hello", "yellow", "mellow"}) == 4);
    assert(longestCommonSuffixLength({"alpha", "beta", "gamma"}) == 1); // only 'a'
    assert(longestCommonSuffixLength({"same", "same", "same"}) == 4);
    assert(longestCommonSuffixLength({"prefix", "suffix"}) == 2); // "ix"

    // Edge cases
    assert(longestCommonSuffixLength({}) == 0);
    assert(longestCommonSuffixLength({"single"}) == 6); // entire string
    assert(longestCommonSuffixLength({"", "abc"}) == 0);
    assert(longestCommonSuffixLength({"abc", ""}) == 0);
    assert(longestCommonSuffixLength({"a", "ab", "abc"}) == 0); // last char differs
    assert(longestCommonSuffixLength({"xyz", "wxyz", "vwxyz"}) == 2); // "yz"

    return 0;
}

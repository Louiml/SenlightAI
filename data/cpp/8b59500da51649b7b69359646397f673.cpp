Write a C++ function `std::string mergeAlternately(const std::string& word1, const std::string& word2)` that merges two input strings by alternating their characters, starting with the first character of `word1`. If one string is longer than the other, append the remaining characters of the longer string at the end. The function must handle empty strings (if both are empty, return an empty string; if one is empty, return the other unchanged) and must not modify the inputs. Use only standard library facilities.

// The solution iterates over indices from 0 up to the maximum of the two string lengths. At each index `i`, if `i` is within the bounds of `word1`, append `word1[i]` to the result; then, if `i` is within the bounds of `word2`, append `word2[i]`. This naturally interleaves characters while ensuring that when one string is exhausted, only the remaining characters of the longer string are appended. Edge cases: both empty returns an empty string; one empty returns the other string unchanged; strings of unequal length handle the tail correctly. Time complexity is O(n + m), where `n` and `m` are the lengths of the two strings, because each character is visited once. Space complexity is O(n + m) for the result string, excluding the input strings.

#include <string>

// Merge two strings by alternating characters, starting with word1.
std::string mergeAlternately(const std::string& word1, const std::string& word2) {
    std::string result;
    const size_t maxLen = (word1.length() > word2.length()) ? word1.length() : word2.length();
    for (size_t i = 0; i < maxLen; ++i) {
        if (i < word1.length()) {
            result += word1[i];
        }
        if (i < word2.length()) {
            result += word2[i];
        }
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration (the free function is defined above in the solution section).
std::string mergeAlternately(const std::string& word1, const std::string& word2);

int main() {
    // Both strings equal length
    assert(mergeAlternately("abc", "pqr") == "apbqcr");
    // word1 longer than word2
    assert(mergeAlternately("ab", "pqrs") == "apbqrs");
    // word2 longer than word1
    assert(mergeAlternately("abcd", "pq") == "apbqcd");
    // Both empty
    assert(mergeAlternately("", "") == "");
    // One empty (word1 non-empty)
    assert(mergeAlternately("abc", "") == "abc");
    // One empty (word2 non-empty)
    assert(mergeAlternately("", "xyz") == "xyz");
    // Single characters
    assert(mergeAlternately("a", "b") == "ab");
    // Longer strings with mixed lengths
    assert(mergeAlternately("hello", "world") == "hweolrllod");
    // Strings with identical characters repeated
    assert(mergeAlternately("aaa", "bb") == "ababa");
    return 0;
}

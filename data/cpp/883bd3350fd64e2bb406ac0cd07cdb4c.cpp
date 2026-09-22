/*
Write a C++ function that takes two strings, `word1` and `word2`, and returns a new string formed by merging the characters of the two input strings alternately. The merge starts with the first character of `word1`, then the first character of `word2`, then the second character of `word1`, then the second character of `word2`, and so on. When one string is exhausted, the remaining characters of the other string are appended directly to the end of the result. If both strings are empty, return an empty string. The function should be named `mergeAlternately` and should accept the two input strings as `const std::string&` parameters, returning the merged result by value. Ensure the function works with strings of arbitrary length, including where one string is much longer than the other, and handles empty strings correctly.
*/
#include <string>

// Merge two strings alternately, starting with the first character of word1,
// then the first character of word2, and so on. Append the remaining suffix
// of the longer string after the shorter one is exhausted.
std::string mergeAlternately(const std::string& word1, const std::string& word2) {
    std::string result;
    const size_t len1 = word1.length();
    const size_t len2 = word2.length();
    const size_t maxLength = (len1 > len2) ? len1 : len2;

    for (size_t i = 0; i < maxLength; ++i) {
        if (i < len1) {
            result += word1[i];
        }
        if (i < len2) {
            result += word2[i];
        }
    }

    return result;
}
#include <cassert>
#include <string>

// Declaration of the function to test (provided by the solution)
std::string mergeAlternately(const std::string& word1, const std::string& word2);

int main() {
    // Both strings equal length
    assert(mergeAlternately("abc", "pqr") == "apbqcr");
    // First string longer
    assert(mergeAlternately("abcd", "pq") == "apbqcd");
    // Second string longer
    assert(mergeAlternately("ab", "pqrs") == "apbqrs");
    // Empty first string
    assert(mergeAlternately("", "xyz") == "xyz");
    // Empty second string
    assert(mergeAlternately("hello", "") == "hello");
    // Both empty
    assert(mergeAlternately("", "") == "");
    // Single characters
    assert(mergeAlternately("a", "b") == "ab");
    // Mixed case and spaces
    assert(mergeAlternately("Ab", "cD") == "AcbD");
    // Longer strings with repeated characters
    assert(mergeAlternately("aaa", "bbbb") == "abababb");
    return 0;
}
// The main algorithm uses a single loop that iterates up to the maximum length of the two input strings. At each iteration index `i`, if `i` is still within the bounds of `word1`, the character `word1[i]` is appended to a result string; similarly, if `i` is within the bounds of `word2`, `word2[i]` is appended. This ensures that characters are interleaved while indices are valid for the shorter string, then the longer string's remaining suffix is appended after the shorter one is exhausted. 
// Edge cases to consider: both strings empty (loop runs zero times, returns an empty string); one string empty (the loop appends only the nonzero string's characters in order); strings of equal length (perfect alternation); a very long string paired with a very short one (the loop appends the shorter string's characters at the start, then the remaining suffix of the longer one). 
// Time complexity is \(O(\max(\text{len1}, \text{len2}))\) because each character of both strings is processed exactly once. Space complexity is \(O(\max(\text{len1}, \text{len2}))\) for the result string, excluding the input storage. No additional auxiliary data structures are needed beyond the loop counter.

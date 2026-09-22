/*
Write a standalone C++ function `commonAnagramSubsequence` that takes a positive integer `n` and a vector of `n` strings (all consisting of lowercase English letters) and returns a string containing the longest multiset-intersection of all strings, sorted in ascending order. In other words, for each character from 'a' to 'z', the returned string must include that character exactly `min(count_in_string_1, count_in_string_2, ..., count_in_string_n)` times. If the result is empty, return an empty string. The function must handle `n = 1` by returning that single string sorted, and must work for arbitrary order of input strings. The time complexity should be O(total characters + alphabet size * n) or better, and you must not use brute-force pairwise comparisons.
*/
#include <string>
#include <vector>
#include <algorithm>

// Returns the longest multiset-intersection of all strings, sorted ascending.
std::string commonAnagramSubsequence(const std::vector<std::string>& strings) {
    if (strings.empty()) {
        return "";
    }

    // Global frequency counts initialized from first string
    std::vector<int> common(26, 0);
    for (char c : strings[0]) {
        common[c - 'a']++;
    }

    // For each remaining string, intersect frequencies
    for (size_t i = 1; i < strings.size(); ++i) {
        std::vector<int> current(26, 0);
        for (char c : strings[i]) {
            current[c - 'a']++;
        }
        for (int j = 0; j < 26; ++j) {
            common[j] = std::min(common[j], current[j]);
        }
    }

    // Build result string
    std::string result;
    for (int i = 0; i < 26; ++i) {
        result.append(common[i], static_cast<char>('a' + i));
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Solution function declaration
std::string commonAnagramSubsequence(const std::vector<std::string>& strings);

int main() {
    // Basic case
    assert(commonAnagramSubsequence({"ab", "abc", "abz"}) == "ab");
    // Single string returns sorted
    assert(commonAnagramSubsequence({"cba"}) == "abc");
    // Empty result
    assert(commonAnagramSubsequence({"abc", "def"}) == "");
    // Duplicate counts
    assert(commonAnagramSubsequence({"aabb", "ab", "aaab"}) == "ab");
    // All same character
    assert(commonAnagramSubsequence({"zzz", "zz", "zzzz"}) == "zz");
    // No common characters but same multiset
    assert(commonAnagramSubsequence({"abc", "bca", "cab"}) == "abc");
    // Large repeated characters
    assert(commonAnagramSubsequence({"aaaa", "aa", "aaa"}) == "aa");
    // Empty string in input
    assert(commonAnagramSubsequence({"abc", "", "bca"}) == "");
    // Multiple strings with identical content
    assert(commonAnagramSubsequence({"xyz", "xyz", "xyz"}) == "xyz");
    // Mixed lengths
    assert(commonAnagramSubsequence({"apple", "ppale", "pleap"}) == "appel");
    return 0;
}
// The correct approach is to compute the frequency of each character (from 'a' to 'z') in every string independently, then take the minimum frequency across all strings for each character. Initialize a global frequency array of size 26 with all zeros for the "current common" state. For the first string, fill this array with its counts. For each subsequent string, compute its own frequency array, then update the global array by setting `global[i] = min(global[i], current[i])`. After processing all strings, build the result string by appending each character `i` exactly `global[i]` times, in ascending order of `i`. Edge cases: if any string is empty, the result is empty. If `n = 1`, the result is simply the sorted first string. Duplicate characters are handled naturally by counts. Time complexity is O(26 * n + total characters) because we iterate over each string once to count (O(len)) and then do 26 comparisons per string. Space complexity is O(26) for the count arrays plus the length of the returned string, which is O(total minimum counts).

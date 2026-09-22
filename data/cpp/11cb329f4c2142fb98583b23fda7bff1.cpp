// Write a C++ function named `longestCommonPrefix` that accepts a vector of strings and returns the longest common prefix shared by all strings in the vector as a string. If the vector is empty, the function must return an empty string. The function should determine the prefix by only comparing the lexicographically smallest and largest strings after sorting the input vector, since these two strings bound all possible common prefixes. The function must not modify the original vector and should handle vectors with a single element, identical strings, and strings where no common prefix exists beyond an empty string.
// The solution leverages sorting to simplify the problem. After sorting the vector of strings lexicographically, the first and last strings are the most dissimilar pair in terms of alphabetical order. Any common prefix shared by all strings must also be shared by these two extreme strings, and conversely, if the first and last strings share a prefix of length `k`, then every string between them in sorted order must share that same prefix. Therefore, comparing only `strs[0]` and `strs[n-1]` character-by-character from the start is sufficient. The loop runs until the shorter of the two strings ends or a mismatch occurs; each matching character is appended to the result. Edge cases: an empty vector returns `""`; a vector with one string returns that string itself; if the first characters differ, the loop breaks immediately and returns `""`. Sorting has average time complexity O(n log n) where n is the number of strings, but each comparison during sorting may involve comparing characters up to the length of the longest string, so the actual cost is O(m n log n) where m is the average string length. Space complexity is O(1) beyond the input and output storage, as no dynamic allocation is used except for the returned string.
#include <string>
#include <vector>
#include <algorithm>

// Returns the longest common prefix of all strings in the input vector.
// Returns an empty string if the vector is empty.
std::string longestCommonPrefix(std::vector<std::string>& strs) {
    if (strs.empty()) {
        return "";
    }

    std::sort(strs.begin(), strs.end());

    const std::string& first = strs.front();
    const std::string& last = strs.back();

    std::string prefix;
    prefix.reserve(first.size());

    for (std::size_t i = 0; i < first.size() && i < last.size(); ++i) {
        if (first[i] == last[i]) {
            prefix.push_back(first[i]);
        } else {
            break;
        }
    }

    return prefix;
}
#include <cassert>
#include <string>
#include <vector>

// Function declaration (must match the solution definition)
std::string longestCommonPrefix(std::vector<std::string>& strs);

int main() {
    {
        std::vector<std::string> v = {"flower", "flow", "flight"};
        assert(longestCommonPrefix(v) == "fl");
    }
    {
        std::vector<std::string> v = {"dog", "racecar", "car"};
        assert(longestCommonPrefix(v) == "");
    }
    {
        std::vector<std::string> v = {};
        assert(longestCommonPrefix(v) == "");
    }
    {
        std::vector<std::string> v = {"single"};
        assert(longestCommonPrefix(v) == "single");
    }
    {
        std::vector<std::string> v = {"same", "same", "same"};
        assert(longestCommonPrefix(v) == "same");
    }
    {
        std::vector<std::string> v = {"ab", "abc", "abd"};
        assert(longestCommonPrefix(v) == "ab");
    }
    {
        std::vector<std::string> v = {"a", "b", "c"};
        assert(longestCommonPrefix(v) == "");
    }
    {
        std::vector<std::string> v = {"prefix", "pre", "preheat"};
        assert(longestCommonPrefix(v) == "pre");
    }
}

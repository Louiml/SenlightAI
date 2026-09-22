Write a C++ function `wordBreakAll(const std::vector<std::string>& dictionary, const std::string& s)` that returns all possible ways to segment the string `s` into a space-separated sequence of dictionary words, without reordering the characters. Each complete segmentation must be returned as a single string with words separated by single spaces, and no trailing space should be present. If no valid segmentation exists, return an empty vector. The order of results does not need to be sorted, but all distinct segmentations must appear exactly once. For example, with dictionary {"cat","cats","and","sand","dog"} and input "catsanddog", the function should return {"cats and dog", "cat sand dog"}.
The problem is a classic word-break with backtracking. The key is to recursively explore prefixes of the remaining string: at each position `i` in the string, try every substring `s[i..j]`; if that substring is in the dictionary, add it to the current temporary segmentation and recurse on the remaining part starting at `j+1`. When the entire string is consumed (`i >= s.size()`), we have a complete segmentation; we must remove the trailing space from the temporary string before adding it to the answer list. To speed up dictionary lookups, store all words in an unordered_set. Important edge cases: empty string (should return an empty vector, since no segmentation exists unless dictionary contains empty string, which is not allowed), words that overlap in ways that produce multiple segmentations (e.g., "catsanddog"), and dictionary words that are prefixes of other words. The recursion depth is bounded by the length of the string. Time complexity is O(2^n) in the worst case (e.g., dictionary = {"a","aa","aaa",...} and string = "aaa...a"), but with a set lookup it’s exponential in the number of possible segmentations; typical cases are much faster. Space complexity is O(n) for the recursion stack plus storage for the answer vector, which can be O(2^n) in the worst case.
#include <vector>
#include <string>
#include <unordered_set>

// Return all valid space-separated segmentations of s using words from dictionary.
std::vector<std::string> wordBreakAll(const std::vector<std::string>& dictionary, const std::string& s) {
    std::unordered_set<std::string> dict(dictionary.begin(), dictionary.end());
    std::vector<std::string> result;

    // Recursive helper: process from index i, current segmented prefix in current.
    std::function<void(int, std::string)> dfs = [&](int i, std::string current) {
        if (i >= static_cast<int>(s.size())) {
            if (!current.empty()) {
                // Remove trailing space before storing.
                current.pop_back();
                result.push_back(current);
            }
            return;
        }

        std::string prefix;
        for (int j = i; j < static_cast<int>(s.size()); ++j) {
            prefix += s[j];
            if (dict.find(prefix) != dict.end()) {
                dfs(j + 1, current + prefix + " ");
            }
        }
    };

    if (!s.empty()) {
        dfs(0, "");
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// The function is assumed to be included from above.

int main() {
    // Example from the problem statement.
    {
        std::vector<std::string> dict = {"cat", "cats", "and", "sand", "dog"};
        std::vector<std::string> result = wordBreakAll(dict, "catsanddog");
        std::sort(result.begin(), result.end());
        std::vector<std::string> expected = {"cat sand dog", "cats and dog"};
        assert(result == expected);
    }

    // No segmentation possible.
    {
        std::vector<std::string> dict = {"cat", "dog"};
        std::vector<std::string> result = wordBreakAll(dict, "catdoggo");
        assert(result.empty());
    }

    // Multiple overlapping ways.
    {
        std::vector<std::string> dict = {"a", "aa", "aaa"};
        std::vector<std::string> result = wordBreakAll(dict, "aaaa");
        std::sort(result.begin(), result.end());
        std::vector<std::string> expected = {"a a a a", "a a aa", "a aa a", "aa a a", "aa aa", "aaa a"};
        // Ensure all are present (compare sorted).
        assert(result == expected);
    }

    // Single word segmentation.
    {
        std::vector<std::string> dict = {"hello", "world"};
        std::vector<std::string> result = wordBreakAll(dict, "hello");
        std::vector<std::string> expected = {"hello"};
        assert(result == expected);
    }

    // Empty string returns empty vector (no sentences).
    {
        std::vector<std::string> dict = {"a"};
        std::vector<std::string> result = wordBreakAll(dict, "");
        assert(result.empty());
    }

    // Words that share prefixes but not full matches.
    {
        std::vector<std::string> dict = {"pine", "apple", "pineapple"};
        std::vector<std::string> result = wordBreakAll(dict, "pineapple");
        std::sort(result.begin(), result.end());
        std::vector<std::string> expected = {"pine apple", "pineapple"};
        assert(result == expected);
    }

    // Duplicate words in dictionary shouldn't cause duplicate results.
    {
        std::vector<std::string> dict = {"cat", "cat", "dog"};
        std::vector<std::string> result = wordBreakAll(dict, "catdog");
        std::vector<std::string> expected = {"cat dog"};
        assert(result == expected);
    }

    // Long string with no solution but large dictionary.
    {
        std::vector<std::string> dict = {"ab", "bc", "cd"};
        std::vector<std::string> result = wordBreakAll(dict, "abc");
        assert(result.empty());
    }

    // Exactly one solution from two-word string.
    {
        std::vector<std::string> dict = {"a", "b"};
        std::vector<std::string> result = wordBreakAll(dict, "ab");
        std::vector<std::string> expected = {"a b"};
        assert(result == expected);
    }

    // Multiple solutions from mixed lengths.
    {
        std::vector<std::string> dict = {"code", "cod", "e", "x"};
        std::vector<std::string> result = wordBreakAll(dict, "codex");
        std::sort(result.begin(), result.end());
        std::vector<std::string> expected = {"cod e x", "code x"};
        assert(result == expected);
    }

    return 0;
}

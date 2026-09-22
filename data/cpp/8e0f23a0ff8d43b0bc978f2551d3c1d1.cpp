Write a C++ function that takes a non-empty string `s` and a vector of strings `wordDict`, and returns a vector of all possible complete sentences that can be formed by segmenting `s` into dictionary words, preserving the original order of characters. Each sentence is a single string where words are separated by exactly one space. The dictionary words may appear multiple times in `s`, and the same word may be used more than once in a sentence. If no segmentation is possible, return an empty vector. The input string contains only lowercase English letters, and dictionary words are non-empty and consist of lowercase letters.

// The solution uses dynamic programming with a table `dp` where `dp[i]` stores a vector of all valid sentence strings that can be formed from the prefix `s[0..i-1]` (i.e., the first `i` characters). The base case is `dp[0]` containing a single empty string, representing that an empty prefix can be segmented in exactly one way (with no words). For each index `i` from 1 to `n`, we iterate over every possible split point `j` from 0 to `i-1`. We check whether the substring `s[j..i-1]` (call it `suffix`) exists in the dictionary (stored in an unordered_set for O(1) average lookup). If it does, we take every sentence already computed in `dp[j]` and append `suffix` to it, adding a space before `suffix` if the sentence is non-empty. This correctly preserves the original order because we are building from left to right. At the end, `dp[n]` contains all complete sentences for the entire string. Edge cases include: (1) dictionary words that are substrings of other dictionary words—this does not affect correctness because we check all split positions; (2) overlapping splits that produce identical sentences—we do not deduplicate, but if the task expects unique sentences, we could add a deduplication step; however, the typical problem from LeetCode expects all sentences, including duplicates? In the original problem, duplicates are not produced because each split position yields a distinct combination, so duplicates only occur if the same sentence can be formed in multiple ways—which is possible, but the original solution does not filter them. To be safe, the reference solution will not deduplicate but will match the exact behavior of the given snippet. Time complexity: For each `i` (n+1 iterations) and each `j` (up to n), we do a substring extraction (O(n) worst-case per substring) and a dictionary lookup O(1) average, plus we append to potentially many sentences in `dp[j]`. In the worst case, the number of sentences can be exponential (e.g., when s is "aaaa" and dict is ["a","aa"]), so the total time can be O(n^2 * L * S) where L is average sentence length and S is number of sentences, but the standard analysis is O(n^3) for time if we ignore the output size, because substring creation is O(n) and there are O(n^2) (i,j) pairs. Space is O(n * total output size) for the dp table.

#include <vector>
#include <string>
#include <unordered_set>

// Returns all possible sentences formed by segmenting s into words from wordDict.
// Each sentence is a single string with words separated by single spaces.
std::vector<std::string> wordBreak(const std::string& s, const std::vector<std::string>& wordDict) {
    const int n = s.length();
    std::unordered_set<std::string> wordSet(wordDict.begin(), wordDict.end());
    
    // dp[i] holds all valid sentences for prefix s[0..i-1]
    std::vector<std::vector<std::string>> dp(n + 1);
    dp[0].push_back("");  // empty prefix corresponds to an empty sentence
    
    for (int i = 1; i <= n; ++i) {
        std::vector<std::string> sentences;
        for (int j = 0; j < i; ++j) {
            const std::string suffix = s.substr(j, i - j);
            if (wordSet.find(suffix) != wordSet.end()) {
                // Append suffix to all sentences from dp[j]
                for (const std::string& prefixSentence : dp[j]) {
                    if (prefixSentence.empty()) {
                        sentences.push_back(suffix);
                    } else {
                        sentences.push_back(prefixSentence + " " + suffix);
                    }
                }
            }
        }
        dp[i] = std::move(sentences);
    }
    
    return dp[n];
}

#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be included above.)

int main() {
    // Basic example from the problem
    std::string s1 = "catsanddog";
    std::vector<std::string> dict1 = {"cat", "cats", "and", "sand", "dog"};
    std::vector<std::string> result1 = wordBreak(s1, dict1);
    std::vector<std::string> expected1 = {"cats and dog", "cat sand dog"};
    assert(result1 == expected1);

    // No possible segmentation
    std::string s2 = "pineapplepenapple";
    std::vector<std::string> dict2 = {"apple", "pen", "applepen", "pine", "pineapple"};
    std::vector<std::string> result2 = wordBreak(s2, dict2);
    std::vector<std::string> expected2 = {"pine apple pen apple", "pineapple pen apple", "pine applepen apple"};
    assert(result2 == expected2);

    // String that cannot be broken
    std::string s3 = "catsandog";
    std::vector<std::string> dict3 = {"cats", "dog", "sand", "and", "cat"};
    std::vector<std::string> result3 = wordBreak(s3, dict3);
    assert(result3.empty());

    // Single character
    std::string s4 = "a";
    std::vector<std::string> dict4 = {"a"};
    std::vector<std::string> result4 = wordBreak(s4, dict4);
    assert(result4 == std::vector<std::string>{"a"});

    // Overlapping dictionary words, multiple segmentations
    std::string s5 = "aaaa";
    std::vector<std::string> dict5 = {"a", "aa"};
    std::vector<std::string> result5 = wordBreak(s5, dict5);
    // Expected: all combinations of "a" and "aa" summing to 4 characters
    std::vector<std::string> expected5 = {"a a a a", "a a aa", "a aa a", "aa a a", "aa aa"};
    assert(result5 == expected5);

    // Empty dictionary but non-empty string -> no segmentation
    std::string s6 = "abc";
    std::vector<std::string> dict6 = {};
    std::vector<std::string> result6 = wordBreak(s6, dict6);
    assert(result6.empty());

    return 0;
}

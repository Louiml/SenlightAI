Write a C++ function `longestWordChain(std::vector<std::string>& words)` that, given a list of distinct words, returns the length of the longest possible word chain. A word chain is a sequence of words where each word is formed by adding exactly one character to the previous word (anywhere in the string), and the next word must be exactly one character longer than the previous. The chain can start from any word in the list, and words must be used in the given list (no external words). Words are case‑sensitive and consist only of lowercase English letters. The input vector may be empty, in which case return 0; if it has one word, return 1. Duplicate words are not present, but words can have lengths from 1 to 100. The order of words in the input does not matter; you may sort them internally. The function should be efficient for up to, say, 1000 words.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above; include its header or definition.
int main() {
    // Basic chain: a → ba → bca → bdca (length 4)
    std::vector<std::string> words1 = {"a", "bca", "ba", "b", "bdca", "bda"};
    assert(longestWordChain(words1) == 4);

    // Empty input
    std::vector<std::string> words2;
    assert(longestWordChain(words2) == 0);

    // Single word
    std::vector<std::string> words3 = {"abc"};
    assert(longestWordChain(words3) == 1);

    // No chain possible (different lengths but no single-char insertion relationships)
    std::vector<std::string> words4 = {"abc", "def", "ghi"};
    assert(longestWordChain(words4) == 1);

    // Reverse order input (must sort internally)
    std::vector<std::string> words5 = {"bdca", "bda", "ba", "a", "bca"};
    assert(longestWordChain(words5) == 4);

    // Chain with branching: "x" → "xy" and "x" → "xy" → "xyz", best is 3
    std::vector<std::string> words6 = {"x", "xy", "xyz", "xz"};
    assert(longestWordChain(words6) == 3);

    // Chain where predecessor appears after in original order (sorting fixes)
    std::vector<std::string> words7 = {"ab", "a", "abc", "b"};
    assert(longestWordChain(words7) == 3); // "a" → "ab" → "abc"

    // Longer chain: "c" → "ca" → "cba" → "dcba" → "edcba" (5)
    std::vector<std::string> words8 = {"c", "ca", "cba", "dcba", "edcba", "edc", "dc"};
    assert(longestWordChain(words8) == 5);

    // Words with same length but different content: no chain among them
    std::vector<std::string> words9 = {"ab", "cd", "ef"};
    assert(longestWordChain(words9) == 1);

    // Chain where best path is not the first found: "a" → "ab" → "abc" or "a" → "ac" → "abc"
    std::vector<std::string> words10 = {"a", "ac", "ab", "abc"};
    assert(longestWordChain(words10) == 3);
}
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Return the length of the longest word chain from the given list.
// A chain is a sequence where each word is formed by adding one character
// to the previous word. The order of insertion does not matter.
int longestWordChain(std::vector<std::string>& words) {
    if (words.empty()) {
        return 0;
    }

    // Sort words by length ascending.
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.size() < b.size();
              });

    // dp[word] = best chain length ending at that word.
    std::unordered_map<std::string, int> dp;
    int result = 1;

    for (const std::string& word : words) {
        dp[word] = 1;  // at least the word itself
        // Try removing one character at each position.
        for (size_t i = 0; i < word.size(); ++i) {
            std::string predecessor = word.substr(0, i) + word.substr(i + 1);
            auto it = dp.find(predecessor);
            if (it != dp.end()) {
                dp[word] = std::max(dp[word], it->second + 1);
            }
        }
        result = std::max(result, dp[word]);
    }

    return result;
}
// The solution is analogous to finding the longest increasing subsequence (LIS) but on strings, using dynamic programming. First, sort the words by their length in ascending order (using a custom comparator). For each word in this sorted order, initialize its chain length to 1 (the word alone). Then, for every possible single-character removal from the current word (i.e., every index i from 0 to word.size()-1), form a predecessor string by removing character at that position. If that predecessor exists in the dictionary (which we maintain as a hash map from word to its best chain length), then we can extend the chain: new length = dp[predecessor] + 1. Update dp[current] with the maximum of its current value and this new length. Keep a global maximum `res` across all words. The result is `res`. Edge cases: empty input → return 0; a word with no predecessor in the dictionary → chain length 1; multiple predecessors → take the best (longest) one. Sorting ensures that when we process a word, all possible shorter predecessors (by exactly 1 char) have already been processed because any predecessor is shorter. Time complexity: O(n log n) for sorting, and for each word of average length L, we do O(L) removals, each removal requires O(L) to construct the predecessor string (substring concatenation), giving O(n * L^2) total, but since max length is small (100), this is acceptable. Space: O(n * L) for the DP map and O(n) for sorting.

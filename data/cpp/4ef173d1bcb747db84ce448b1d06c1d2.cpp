/*
Write a C++ function `longestStringChain` that takes a vector of strings `words` and returns the length of the longest word chain, where each word in the chain can be formed from the previous word by inserting exactly one character anywhere (not necessarily at the end), while preserving the relative order of the other characters. That is, word `a` is a predecessor of word `b` if `a` can be obtained by deleting exactly one character from `b` (so `len(b) == len(a) + 1` and `a` is a subsequence of `b` with only one missing character). The function must handle duplicate words, words of arbitrary case (though case matters, so "Cat" and "cat" are different), and an empty input vector (which should return 0). Words can be in any order in the input; the chain can use them in any order. For example, given `["a","b","ba","bca","bda","bdca"]`, the longest chain is length 4: `"a" -> "ba" -> "bda" -> "bdca"` (or similar). The function must be efficient for up to 1000 words with lengths up to 16.
*/
#include <vector>
#include <string>
#include <algorithm>

// Returns true if word 'shortWord' is a predecessor of 'longWord',
// i.e., we can insert exactly one character into shortWord to get longWord.
// Both words must be non-empty, and length(longWord) == length(shortWord) + 1.
bool isPredecessor(const std::string& shortWord, const std::string& longWord) {
    if (longWord.size() != shortWord.size() + 1) {
        return false;
    }
    size_t i = 0; // index for shortWord
    size_t j = 0; // index for longWord
    bool skipped = false;
    while (i < shortWord.size() && j < longWord.size()) {
        if (shortWord[i] == longWord[j]) {
            ++i;
            ++j;
        } else {
            if (skipped) {
                return false; // more than one difference
            }
            skipped = true;
            ++j; // skip this character in longWord
        }
    }
    // If we reach here, all shortWord characters matched.
    // If we haven't skipped yet, the extra character is at the end.
    return true;
}

// Returns the length of the longest word chain.
// A chain is a sequence of words where each next word is obtained by inserting
// exactly one character into the previous word (order preserved).
// Time: O(n^2 * L) where n = words.size(), L = max word length.
// Space: O(n) for the DP array.
int longestStringChain(const std::vector<std::string>& words) {
    if (words.empty()) return 0;
    
    // Sort by length to ensure we process predecessors first.
    std::vector<std::string> sortedWords = words;
    std::sort(sortedWords.begin(), sortedWords.end(),
              [](const std::string& a, const std::string& b) {
                  return a.size() < b.size();
              });
    
    int n = sortedWords.size();
    std::vector<int> dp(n, 1); // dp[i] = longest chain ending at sortedWords[i]
    
    int maxChain = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (sortedWords[j].size() + 1 == sortedWords[i].size() &&
                isPredecessor(sortedWords[j], sortedWords[i])) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        maxChain = std::max(maxChain, dp[i]);
    }
    return maxChain;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above. Here we test it.

int main() {
    // Empty vector
    assert(longestStringChain({}) == 0);
    
    // Single word
    assert(longestStringChain({"a"}) == 1);
    
    // Two words where one is predecessor of the other
    assert(longestStringChain({"a", "ab"}) == 2);
    
    // Example from LeetCode
    std::vector<std::string> words1 = {"a","b","ba","bca","bda","bdca"};
    assert(longestStringChain(words1) == 4);
    
    // Duplicate words cannot be in a chain together
    assert(longestStringChain({"x", "x"}) == 1);
    
    // Words of different lengths but not predecessor
    assert(longestStringChain({"abc", "ab"}) == 1);
    
    // Chain requiring character insertion in the middle
    assert(longestStringChain({"ab", "acb", "acdb"}) == 3);
    
    // Case sensitivity matters
    assert(longestStringChain({"a", "A", "ab", "Ab"}) == 2);
    
    // Words in unsorted order
    assert(longestStringChain({"bc", "b", "c", "abc", "a"}) == 3); // e.g., "a"->"ab"? no "ab" not present; "a"->"abc"? no; actually chain: "b"->"bc"->? no; but "a"->? none. So max is 2? Let's check: "b"->"bc" (2), "a"->? "ab"? no. So answer should be 2. Wait, we have "abc" of len 3 and "bc" len 2 and "b" len1. "bc" is predecessor of "abc"? insert 'a' at start, yes. And "b" -> "bc" (insert 'c'), yes. So chain "b" -> "bc" -> "abc" length 3. But we don't have "a", so that's fine. The test vector has "a" but no "ab", so "a" alone is 1. So longest is 3.
    assert(longestStringChain({"bc", "b", "c", "abc", "a"}) == 3);
    
    // All same length
    assert(longestStringChain({"abc", "def", "ghi"}) == 1);
    
    // Large chain
    std::vector<std::string> words2 = {"a", "ab", "abc", "abcd", "abcde"};
    assert(longestStringChain(words2) == 5);
    
    return 0;
}
// The problem is a classic longest path problem on a directed acyclic graph (DAG) where each word is a node, and an edge from word `a` to word `b` exists if `a` can become `b` by inserting a single character (i.e., `is_predecessor(a, b)`). Since all edges go from shorter to longer words (because the length increases by exactly 1), the graph is acyclic. A straightforward approach is to sort the words by length in ascending order, then use dynamic programming: `dp[i] = 1 + max(dp[j])` for all `j < i` such that `words[j]` is a predecessor of `words[i]`. To check the predecessor condition efficiently, for each pair we can use a two-pointer comparison: if lengths differ by 1, skip at most one character in the longer word. This is `O(L)` where `L` is the max length (here ≤16). The total complexity is `O(n^2 * L)` time and `O(n)` space. An important edge case is when there are duplicate words: since we need to insert exactly one character, duplicates cannot be predecessors of each other (same length). Also, if the input is empty, return 0. We can optimize by grouping words by length, but for the given constraints, the simple nested loop is fine. We must include all necessary headers (`<vector>`, `<string>`, `<algorithm>`). The function signature should take a `const std::vector<std::string>&` to respect const-correctness, and it should return an `int`.

Write a C++ function `longestChainLength` that takes a vector of lowercase strings and returns the length of the longest word chain. A word chain is a sequence of words where each word is formed by inserting exactly one character into the previous word (order preserved), and the next word is one character longer. For example, `"a" → "ab" → "abc"` is a valid chain. Words can be reused, and the input may contain duplicate words. The function should handle empty vectors (return 0) and vectors with a single word (return 1). The order of the input vector is arbitrary, but the chain must respect letter order (e.g., `"ba"` is not a predecessor of `"bra"` because inserting `'r'` after `'b'` would give `"bra"`, but inserting `'r'` anywhere must preserve the original sequence of letters). Ensure the solution is efficient for up to 1000 words with lengths up to 16.

#include <cassert>
#include <vector>
#include <string>

int main() {
    assert(longestChainLength({}) == 0);
    assert(longestChainLength({"a"}) == 1);
    assert(longestChainLength({"a", "b"}) == 1);
    assert(longestChainLength({"a", "ab", "acb"}) == 3);  // a -> ab -> acb
    assert(longestChainLength({"xbc", "pc", "xb", "cx", "pcxbc"}) == 4);  // xb -> xbc -> pcxbc
    assert(longestChainLength({"a", "b", "ba", "bca", "bda", "bdca"}) == 4);  // a -> ba -> bda -> bdca
    assert(longestChainLength({"abcd", "abc", "ab", "a"}) == 4);  // reverse order still works
    assert(longestChainLength({"a", "a", "aa"}) == 2);  // a -> aa (duplicate "a" cannot chain)
    assert(longestChainLength({"ab", "a", "abc", "abcd"}) == 4);  // a -> ab -> abc -> abcd
    return 0;
}

#include <vector>
#include <string>
#include <algorithm>

// Check if s1 is a subsequence of s2 (order preserved, not necessarily contiguous)
static bool isSubsequence(const std::string& s1, const std::string& s2) {
    size_t j = 0;
    for (size_t i = 0; i < s2.size(); ++i) {
        if (j < s1.size() && s2[i] == s1[j]) {
            ++j;
        }
    }
    return j == s1.size();
}

// Return the length of the longest word chain in the given vector.
int longestChainLength(std::vector<std::string> words) {
    if (words.empty()) {
        return 0;
    }
    // Sort by length ascending (stable not required)
    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
        return a.size() < b.size();
    });

    const int n = static_cast<int>(words.size());
    std::vector<int> dp(n, 1);
    int maxChain = 1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (words[i].size() == words[j].size() + 1 && isSubsequence(words[j], words[i])) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        maxChain = std::max(maxChain, dp[i]);
    }
    return maxChain;
}

// Sort the words by length (shortest first) using a custom comparator. Then, for each word, compare it only with previous shorter words. For a previous word to be a valid predecessor: its length must be exactly one less than the current word's length, and the current word must contain the previous word as a subsequence (i.e., all characters of the previous word appear in order in the current word). Maintain a DP array where `dp[i]` is the longest chain ending at word `i`. Initialize each `dp[i] = 1`. For each pair `(i, j)` with `j < i`, if conditions hold, update `dp[i] = max(dp[i], dp[j] + 1)`. Track the global maximum. Edge cases: empty input (return 0), single word (return 1), duplicate words (they may chain if they are identical? No, because length must increase, so duplicates cannot be predecessors; but they are sorted by length and can appear in any order, still length condition fails). The subsequence check is O(L) for each pair, and total pairs are O(n^2). Sorting is O(n log n). Overall time O(n^2 * L) where L is max word length, space O(n).

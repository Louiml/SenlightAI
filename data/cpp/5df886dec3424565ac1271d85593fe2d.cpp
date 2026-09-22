Write a C++ function `std::string longestCompleteString(const std::vector<std::string>& words)` that, given a vector of lowercase strings, returns the longest string such that every prefix of that string (including the string itself) appears exactly once in the input vector. If multiple strings have the same maximum length, return the lexicographically smallest among them. If no string satisfies the condition (i.e., every non-empty prefix appears in the list), return the string `"None"`. Note that a single-character string is always valid if it appears in the list, since its only prefix is itself. The input list may contain duplicates, and duplicates do not affect validity beyond their presence. The function must handle up to \(10^5\) strings with total length up to \(10^5\), so ensure an efficient solution.
The core idea is to use a Trie (prefix tree) to store all words. After inserting all words, we mark the end of each word by setting a boolean flag at the corresponding node. To check if a word is "complete", we traverse its characters from the root; at each step, we must ensure that the current node exists and that the node’s flag is `true` (meaning the prefix formed so far is a complete word present in the list). If at any step the flag is `false` or a child is missing, the word fails. We then iterate over all original words (not deduplicated, but duplicates won’t change the result) and keep the best candidate according to length and lexicographic order. Edge cases: empty input (should return `"None"`), single-character words, and duplicate strings (they don’t affect correctness). Time complexity: inserting all words takes \(O(\text{total length})\), and checking each word takes \(O(\text{word length})\), so overall \(O(\text{total length})\). Space complexity is \(O(\text{total length})\) for the Trie nodes (with 26 pointers each) plus storage for the words.
#include <string>
#include <vector>
#include <memory>

struct TrieNode {
    std::unique_ptr<TrieNode> children[26];
    bool isEnd = false;
};

class Trie {
private:
    std::unique_ptr<TrieNode> root;
public:
    Trie() : root(std::make_unique<TrieNode>()) {}

    void insert(const std::string& word) {
        TrieNode* node = root.get();
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->children[idx]) {
                node->children[idx] = std::make_unique<TrieNode>();
            }
            node = node->children[idx].get();
        }
        node->isEnd = true;
    }

    bool isComplete(const std::string& word) const {
        TrieNode* node = root.get();
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx].get();
            if (!node->isEnd) return false;
        }
        return true;
    }
};

// Returns the longest complete string, lexicographically smallest on ties.
std::string longestCompleteString(const std::vector<std::string>& words) {
    if (words.empty()) return "None";

    Trie trie;
    for (const auto& w : words) {
        trie.insert(w);
    }

    std::string best = "";
    for (const auto& w : words) {
        if (!trie.isComplete(w)) continue;
        if (w.size() > best.size() || (w.size() == best.size() && w < best)) {
            best = w;
        }
    }
    return best.empty() ? "None" : best;
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Example from problem statement
    assert(longestCompleteString({"ab", "abc", "a", "bp"}) == "abc");
    // Sample 1
    assert(longestCompleteString({"n", "ni", "nin", "ninj", "ninja", "ninga"}) == "ninja");
    // Sample 2
    assert(longestCompleteString({"ab", "bc"}) == "None");
    // Sample 3 (test case 1)
    assert(longestCompleteString({"g", "a", "ak", "szhkb", "hy"}) == "ak");
    // Sample 4 (test case 2)
    assert(longestCompleteString({"kez", "vfj", "vfjq", "vfjqo"}) == "None");
    // Single character
    assert(longestCompleteString({"x"}) == "x");
    // Empty input
    assert(longestCompleteString({}) == "None");
    // Duplicate strings
    assert(longestCompleteString({"a", "a", "ab", "abc"}) == "abc");
    // Lexicographic tie
    assert(longestCompleteString({"ab", "ac", "a"}) == "ab");
    return 0;
}

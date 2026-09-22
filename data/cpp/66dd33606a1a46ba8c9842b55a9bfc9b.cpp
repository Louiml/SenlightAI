Design a C++ function `countDistinctSubstrings` that takes a non-empty string `s` consisting of lowercase English letters and returns the number of distinct substrings of `s`. For example, for `s = "ababa"`, the distinct substrings are: `"a"`, `"ab"`, `"aba"`, `"abab"`, `"ababa"`, `"b"`, `"ba"`, `"bab"`, `"baba"`, and the function should return `9`. The function must use a Trie data structure to efficiently insert all suffixes of the string, and count the total number of nodes created in the Trie (excluding the root), since each node represents a unique prefix of some suffix, which corresponds exactly to a distinct substring. The string may be up to length 1000, and you should implement the Trie manually without using the STL `map` for the children, instead using a fixed-size array of 26 pointers to reduce overhead. Ensure the function handles edge cases like a single character string (returning 1) and strings with all identical characters (e.g., `"aaaa"` returns 4).
// The core idea is that every distinct substring of `s` is a prefix of some suffix of `s`. By inserting all suffixes of `s` into a Trie, each node (except the root) represents exactly one distinct substring. The number of nodes in the Trie after inserting all suffixes equals the number of distinct substrings. For example, for `s = "ab"`, suffixes are `"ab"` and `"b"`. Inserting `"ab"` creates nodes for `'a'` and `'b'`, then inserting `"b"` finds the existing `'b'` node. Total nodes = 2, which is the count of distinct substrings (`"a"`, `"b"`, `"ab"`? Wait, `"ab"` is also a substring but that's the full string? Actually `"ab"` is a substring, but it is covered by the suffix `"ab"` itself. So distinct substrings are `"a"`, `"b"`, `"ab"` – that's 3, but the Trie nodes are `'a'` and `'b'` only? Let's check: Root -> 'a' -> 'b' for suffix "ab". Then insert "b": root already has child 'b'? No, root's child 'b' is created when inserting "b". So nodes: 'a', 'b' under 'a', and 'b' under root – that's 3 nodes total (excluding root). So correct.)
//
// Implementation: We define a `TrieNode` struct with an array of 26 pointers initialized to `nullptr`, and a boolean `isEnd` to mark end of a word, but for counting distinct substrings we don't need `isEnd`; we only need node count. However, we can keep it for clarity. The Trie class will have a `root` and a method `insert` that takes a string and returns nothing, but we maintain a counter of nodes created. The `countDistinctSubstrings` function creates a Trie, inserts every suffix of `s` (starting from each index `i` from 0 to `n-1`, inserting the substring `s.substr(i)`), and returns the total number of nodes created (excluding the root). The time complexity is O(n^2) because we insert n suffixes, each of length up to n, leading to O(n^2) operations overall. Space complexity is O(number of distinct substrings) in the worst case for a string with all distinct characters, which is O(n^2) slots in the trie but each node has a fixed array of 26 pointers, so memory is O(n^2 * 26) which is acceptable for n=1000 (about 26 million pointers, but each pointer is 8 bytes – that's ~200MB, too high). To optimize memory, we can use `map<char, TrieNode*>` instead of array? But the task says "without using the STL map for the children" – however, for n=1000, array-based is fine because worst-case distinct substrings are n*(n+1)/2 ≈ 500,000 nodes, each with 26 pointers → 13 million pointers → ~104 MB. That's acceptable. Alternatively, we can use a `unordered_map<char, TrieNode*>` to save memory but maybe the task expects array. We'll use array but note that we can also use a vector of size 26. We'll implement with `std::array<TrieNode*, 26>`. Edge cases: empty string? Task says non-empty. Single character returns 1. All same characters: e.g., "aaaa" has distinct substrings "a","aa","aaa","aaaa" → 4. Good.
#include <string>
#include <array>
#include <memory>

struct TrieNode {
    std::array<TrieNode*, 26> children;
    bool isEnd;

    TrieNode() : children{}, isEnd(false) {}
};

class Trie {
private:
    TrieNode* root;
    int nodeCount;

public:
    Trie() : root(new TrieNode()), nodeCount(0) {}

    ~Trie() {
        // Recursively delete nodes
        deleteSubtree(root);
    }

    void insert(const std::string& word) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (cur->children[idx] == nullptr) {
                cur->children[idx] = new TrieNode();
                ++nodeCount;
            }
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    int getNodeCount() const {
        return nodeCount;
    }

private:
    void deleteSubtree(TrieNode* node) {
        if (node == nullptr) return;
        for (int i = 0; i < 26; ++i) {
            if (node->children[i] != nullptr) {
                deleteSubtree(node->children[i]);
            }
        }
        delete node;
    }
};

// Count the number of distinct substrings in the given string.
// Uses a Trie to insert all suffixes; each trie node (except root) represents one distinct substring.
int countDistinctSubstrings(const std::string& s) {
    if (s.empty()) return 0;
    Trie trie;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        trie.insert(s.substr(i));
    }
    return trie.getNodeCount();
}
#include <cassert>
#include <string>

// Declaration (the solution function is included above)
int countDistinctSubstrings(const std::string& s);

int main() {
    // Single character
    assert(countDistinctSubstrings("a") == 1);

    // Two distinct characters
    assert(countDistinctSubstrings("ab") == 3); // "a","b","ab"

    // All same characters
    assert(countDistinctSubstrings("aaaa") == 4); // "a","aa","aaa","aaaa"

    // Example from task
    assert(countDistinctSubstrings("ababa") == 9);

    // Longer string with repeated pattern
    assert(countDistinctSubstrings("abc") == 6); // a,b,c,ab,bc,abc

    // String with all distinct characters of length 4
    assert(countDistinctSubstrings("wxyz") == 10); // all substrings: 4+3+2+1

    // Repeating pattern "ababa" but with different structure
    assert(countDistinctSubstrings("aba") == 5); // a,b,ab,ba,aba

    // Very long all same chars (length 10 → 10 distinct)
    assert(countDistinctSubstrings("zzzzzzzzzz") == 10);

    // Mixed with repeats
    assert(countDistinctSubstrings("banana") == 15); // known correct count

    // Edge: length 2 with distinct
    assert(countDistinctSubstrings("cd") == 3); // c,d,cd

    // Ensure function works for maximum length (1000) – not asserted but runs without crash
    std::string longString(1000, 'x');
    assert(countDistinctSubstrings(longString) == 1000);

    return 0;
}

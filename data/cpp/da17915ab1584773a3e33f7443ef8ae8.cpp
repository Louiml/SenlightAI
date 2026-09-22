Write a C++ function that takes a vector of strings as input, where each string represents a word, and builds a memory-efficient trie (prefix tree) to store all the words. The function should return the number of distinct complete words that are prefixes of some longer word in the input. For example, if the input contains {"cat", "cats", "dog", "dogs", "do"}, the distinct complete words that are prefixes of longer words are "cat" (prefix of "cats") and "do" (prefix of "dog" and "dogs"), so the result is 2. The function should handle empty input, duplicate words (count each distinct word once), and words that are not prefixes of any other word (these should not be counted). The trie must be built using a map from characters to child nodes and a boolean flag indicating whether the current node marks the end of a complete word.

// The solution builds a trie from the given words. Each node contains a `std::map<char, Node*>` for children and a `bool isEnd` flag to mark the end of a word. Insert all words into the trie, but when a duplicate word is inserted, the `isEnd` flag remains true but we don't double-count. After building, traverse the trie to find all nodes where `isEnd == true` and the node has at least one child (meaning the word is a prefix of longer words). For each such node, increment a counter. Edge cases: empty vector returns 0; a word that is a prefix of another but appears multiple times counts once; a word that is shorter than another and also a prefix counts. Time complexity is O(total characters in all words) for insertion and O(total nodes) for traversal. Space complexity is O(total characters in all distinct words) due to the trie structure.

#include <map>
#include <vector>
#include <string>

struct TrieNode {
    std::map<char, TrieNode*> children;
    bool isEnd;
    TrieNode() : isEnd(false) {}
};

// Count distinct complete words that are prefixes of some longer word.
int countPrefixWords(const std::vector<std::string>& words) {
    TrieNode* root = new TrieNode();
    
    // Insert all words into the trie.
    for (const std::string& word : words) {
        TrieNode* curr = root;
        for (char c : word) {
            if (curr->children.find(c) == curr->children.end()) {
                curr->children[c] = new TrieNode();
            }
            curr = curr->children[c];
        }
        curr->isEnd = true;
    }
    
    // Traverse trie to count prefix words.
    int count = 0;
    std::function<void(const TrieNode*)> dfs = [&](const TrieNode* node) {
        if (node->isEnd && !node->children.empty()) {
            ++count;
        }
        for (const auto& pair : node->children) {
            dfs(pair.second);
        }
    };
    dfs(root);
    
    // Clean up memory.
    std::function<void(TrieNode*)> deleteTrie = [&](TrieNode* node) {
        for (auto& pair : node->children) {
            deleteTrie(pair.second);
        }
        delete node;
    };
    deleteTrie(root);
    
    return count;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case with mixed prefixes.
    assert(countPrefixWords({"cat", "cats", "dog", "dogs", "do"}) == 2);
    
    // Empty input.
    assert(countPrefixWords({}) == 0);
    
    // All words are prefixes of each other.
    assert(countPrefixWords({"a", "ab", "abc"}) == 2); // "a" and "ab"
    
    // Duplicate words.
    assert(countPrefixWords({"test", "test", "testing", "test"}) == 1); // "test"
    
    // No word is a prefix of another.
    assert(countPrefixWords({"apple", "banana", "cherry"}) == 0);
    
    // Single word is not a prefix of anything.
    assert(countPrefixWords({"hello"}) == 0);
    
    // Same prefix repeated in different words.
    assert(countPrefixWords({"pre", "prefix", "prelude", "prepare"}) == 1); // "pre"
    
    // Word that is prefix of multiple longer words.
    assert(countPrefixWords({"run", "running", "runner", "runs"}) == 1); // "run"
    
    // Case sensitivity matters.
    assert(countPrefixWords({"Cat", "cat", "cats"}) == 1); // "cat" only
    
    // Longer words that are also prefixes.
    assert(countPrefixWords({"abc", "abcd", "abcde", "abcdef"}) == 3); // "abc", "abcd", "abcde"
    
    // All identical words.
    assert(countPrefixWords({"same", "same", "same"}) == 0);
    
    return 0;
}

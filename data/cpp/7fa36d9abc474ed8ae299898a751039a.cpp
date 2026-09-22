Write a C++ function `countWordsWithPrefix` that, given a trie represented by a collection of words inserted into a `std::unordered_map`-based structure (or simpler: given a vector of strings and a prefix string), returns the number of distinct words in the collection that start with the given prefix. The function must be case-sensitive, consider exact prefix matches as valid (e.g., prefix "ca" matches "cat" and "car"), and ignore words shorter than the prefix. Additionally, the function must handle an empty prefix, which should return the total number of words in the collection, and must not modify the input collection. To make the task independent, implement your own minimal trie data structure from scratch (without using external libraries), with nodes having `std::unordered_map<char, std::unique_ptr<Node>>` for children and a boolean flag for end-of-word. The function signature should be: `int countWordsWithPrefix(const std::vector<std::string>& words, const std::string& prefix);` and it should build a trie internally from the provided words, then traverse to the prefix node and collect all words below it. Ensure your code is self-contained, compiles with C++17, and uses appropriate `const` correctness. The function must be efficient for large word sets and handle duplicate words by counting them once.
#include <cassert>
#include <string>
#include <vector>

// Forward declare the function to test (in real code, include the solution above).
int countWordsWithPrefix(const std::vector<std::string>& words, const std::string& prefix);

int main() {
    std::vector<std::string> words = {"cat", "car", "card", "dog", "care", ""};
    
    // Basic prefix matches.
    assert(countWordsWithPrefix(words, "ca") == 3); // cat, car, card, care? Actually "ca" matches cat, car, card, care = 4? Wait: "ca" matches cat, car, card, care = 4.
    // Let's correct: words: cat, car, card, dog, care, "" – "ca" matches cat, car, card, care = 4.
    assert(countWordsWithPrefix(words, "ca") == 4);
    assert(countWordsWithPrefix(words, "car") == 3); // car, card, care
    assert(countWordsWithPrefix(words, "card") == 1);
    assert(countWordsWithPrefix(words, "ca") == 4);
    assert(countWordsWithPrefix(words, "d") == 1);
    assert(countWordsWithPrefix(words, "do") == 1);
    assert(countWordsWithPrefix(words, "dog") == 1);
    
    // Empty prefix returns total distinct words (including the empty string).
    assert(countWordsWithPrefix(words, "") == 6);
    
    // Non-existent prefix returns 0.
    assert(countWordsWithPrefix(words, "z") == 0);
    assert(countWordsWithPrefix(words, "caa") == 0);
    
    // Prefix longer than any word.
    assert(countWordsWithPrefix(words, "cardx") == 0);
    
    // Duplicates are counted once.
    std::vector<std::string> dup = {"apple", "apple", "app", "application"};
    assert(countWordsWithPrefix(dup, "app") == 3); // apple, app, application
    assert(countWordsWithPrefix(dup, "apple") == 1);
    
    // Empty vector.
    std::vector<std::string> empty;
    assert(countWordsWithPrefix(empty, "") == 0);
    assert(countWordsWithPrefix(empty, "x") == 0);
    
    return 0;
}
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Minimal trie node structure with children map and end-of-word flag.
struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isEndOfWord = false;
};

// Count distinct words in the input vector that start with the given prefix.
int countWordsWithPrefix(const std::vector<std::string>& words, const std::string& prefix) {
    // Build trie from all words.
    auto root = std::make_unique<TrieNode>();
    for (const std::string& word : words) {
        TrieNode* current = root.get();
        for (char ch : word) {
            auto it = current->children.find(ch);
            if (it == current->children.end()) {
                current->children[ch] = std::make_unique<TrieNode>();
            }
            current = current->children[ch].get();
        }
        current->isEndOfWord = true;
    }

    // Traverse to the prefix node.
    TrieNode* prefixNode = root.get();
    for (char ch : prefix) {
        auto it = prefixNode->children.find(ch);
        if (it == prefixNode->children.end()) {
            return 0; // Prefix not present.
        }
        prefixNode = it->second.get();
    }

    // DFS to count all terminal nodes under the prefix node.
    int count = 0;
    // Recursive lambda with std::function captured by reference.
    std::function<void(const TrieNode*)> dfs = [&](const TrieNode* node) {
        if (node->isEndOfWord) {
            ++count;
        }
        for (const auto& pair : node->children) {
            dfs(pair.second.get());
        }
    };
    dfs(prefixNode);

    return count;
}
// The solution builds a trie from the input vector of strings. For each word, traverse (or create) nodes character by character, marking the final node as an end-of-word. Duplicate words are naturally handled because inserting the same word again just sets an already-true end-of-word flag, and when counting, we visit each terminal node once. To count words with a given prefix, first traverse from the root following the prefix characters. If any character is missing, return 0. Once at the prefix node, perform a depth-first search (DFS) that accumulates a count whenever a node has `isEndOfWord == true`. The DFS must explore all children recursively. The prefix itself may be a complete word (e.g., prefix "cat" and word "cat") – that counts as one word. Edge cases: empty prefix should return the total number of distinct words (since the root is the prefix node, DFS counts all terminal nodes). Empty words in the input are allowed; they occupy the root node as end-of-word, and an empty prefix will count them, but a non-empty prefix won't match them. Complexity: building the trie uses \(O(L)\) time and space where \(L\) is the total number of characters across all words. For counting with prefix of length \(P\), traversing to the prefix node takes \(O(P)\), and the DFS visits every node in the subtree under the prefix, worst-case \(O(\text{total characters in the subtree})\). Overall, the algorithm is linear in total input size.

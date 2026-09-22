// Given a `Trie` class that supports insertion, exact word search, and prefix checking, implement the `countWordsEqual` method. This method should count how many words in the trie are exactly equal to the given `word` parameter. The trie may contain duplicate insertions of the same word, and each insertion counts as a separate occurrence. The method should handle empty strings and words not present in the trie. All words consist of lowercase English letters ('a'–'z'). The solution must be written as a standalone C++ function that operates on a `Trie` object (you may redefine the trie structure as needed in your solution) and returns an integer count.

// The core idea is to traverse the trie following the characters of the given word. If at any point a character link is missing, the word is not present, so the count is 0. If all characters are found, we reach the node representing the end of the word. The count of exact matches equals how many times this word has been inserted. To track that, each `Node` should store a frequency counter (e.g., `int endCount`) that increments each time `insert` reaches that node and sets `flag = true`. Then `countWordsEqual` traverses the trie; if traversal fails, return 0; otherwise return the `endCount` at the final node. Edge cases: empty word (which would have `endCount` at the root if inserted), duplicate insertions, and words that are prefixes of other words (the prefix node may have `endCount` > 0 but the longer word may not exist — that's fine). Time complexity is O(L) where L = word length, space is O(1) extra. The trie itself uses O(total characters inserted * alphabet size) space.

#include <string>

struct TrieNode {
    TrieNode* links[26] = {};
    int endCount = 0;
};

// Count how many times the given word appears in the trie.
int countWordsEqual(const std::string& word, TrieNode* root) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (node->links[idx] == nullptr) {
            return 0;
        }
        node = node->links[idx];
    }
    return node->endCount;
}

#include <cassert>
#include <string>

// Reuse the TrieNode struct and countWordsEqual function from above.
// (In a real test you would include them; here we duplicate for clarity.)
struct TrieNode {
    TrieNode* links[26] = {};
    int endCount = 0;
};

int countWordsEqual(const std::string& word, TrieNode* root) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (node->links[idx] == nullptr) return 0;
        node = node->links[idx];
    }
    return node->endCount;
}

// Helper to insert a word into the trie (for testing).
void insert(TrieNode* root, const std::string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (node->links[idx] == nullptr) {
            node->links[idx] = new TrieNode();
        }
        node = node->links[idx];
    }
    node->endCount++;
}

int main() {
    TrieNode* root = new TrieNode();
    insert(root, "apple");
    insert(root, "apple");
    insert(root, "app");
    insert(root, "apricot");
    insert(root, "");

    assert(countWordsEqual("apple", root) == 2);
    assert(countWordsEqual("app", root) == 1);
    assert(countWordsEqual("apricot", root) == 1);
    assert(countWordsEqual("ap", root) == 0);
    assert(countWordsEqual("banana", root) == 0);
    assert(countWordsEqual("", root) == 1);
    assert(countWordsEqual("appl", root) == 0); // 'appl' is prefix but not a full word
    assert(countWordsEqual("App", root) == 0); // case-sensitive, lowercase only

    // Cleanup (not exhaustive but fine for test)
    delete root;
    return 0;
}

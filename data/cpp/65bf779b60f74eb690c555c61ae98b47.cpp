Given an `m x n` board of lowercase English letters and a list of words, write a C++ function `findWords` that returns all words from the list that can be formed by sequentially traversing adjacent cells (horizontally or vertically, not diagonally) on the board. Each cell can be used only once per word, and words must not be duplicated in the output (order does not matter). The function should take a `std::vector<std::vector<char>>& board` and a `std::vector<std::string>& words` as parameters and return a `std::vector<std::string>` of matching words. Assume the board is non‑empty (at least 1×1), and words are non‑empty lowercase strings. Your solution must be efficient for boards up to 12×12 and up to 100 words of length up to 10. The output vector should contain each matching word exactly once, in any order.
// The core challenge is to avoid redundant prefix searches. We build a Trie (prefix tree) from all given words. Each Trie node has an array of 26 children pointers (for letters 'a'–'z') and an integer `ref` that stores the index of the word if the node marks the end of a word, otherwise `-1`. Inserting all words into the Trie allows us to prune paths that cannot lead to any word. Then we perform a Depth‑First Search (DFS) starting from each cell on the board. During DFS, we traverse the board using the current Trie node: at a cell, we check if the current character leads to a child in the Trie; if not, we backtrack. If we reach a node with `ref != -1`, we add `words[ref]` to the answer and set `ref = -1` to avoid duplicates. We mark visited cells with a temporary sentinel `'#'` to prevent reusing cells in the same path, and restore them after backtracking. We also prune when the board cell is `'#'` or out of bounds. Edge cases: single‑letter words, words that are substrings of longer words, duplicate words in the input list (we want only one occurrence in output), and cases where the same word can be formed by multiple paths (handled by the `ref` reset). Time complexity: For each starting cell, the worst‑case DFS explores all cells in the board, but the Trie prunes invalid prefixes. In the worst case (e.g., all words share long strings), it is `O(m * n * 4^L)` where `L` is the maximum word length, but with the Trie it is typically much better. Space complexity: The Trie has `O(total letters in all words)` nodes, plus `O(m*n)` for the board and recursion stack (depth ≤ L). The answer vector is `O(number of found words)`.
#include <vector>
#include <string>
#include <functional>

class TrieNode {
public:
    std::vector<TrieNode*> children;
    int ref;  // index of word, -1 if none

    TrieNode() : children(26, nullptr), ref(-1) {}
};

// Insert a word into the Trie and store its index at the end node.
void insertWord(TrieNode* root, const std::string& word, int idx) {
    TrieNode* node = root;
    for (char c : word) {
        int ch = c - 'a';
        if (!node->children[ch]) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }
    node->ref = idx;
}

// Find all words from the list that can be formed on the board.
std::vector<std::string> findWords(std::vector<std::vector<char>>& board,
                                   std::vector<std::string>& words) {
    if (board.empty() || board[0].empty() || words.empty()) return {};

    TrieNode* root = new TrieNode();
    for (size_t i = 0; i < words.size(); ++i) {
        insertWord(root, words[i], static_cast<int>(i));
    }

    std::vector<std::string> ans;
    int m = static_cast<int>(board.size());
    int n = static_cast<int>(board[0].size());

    // Depth‑first search from (i, j) with current trie node.
    std::function<void(TrieNode*, int, int)> dfs = [&](TrieNode* node, int i, int j) {
        int idx = board[i][j] - 'a';
        if (!node->children[idx]) return;

        TrieNode* next = node->children[idx];
        if (next->ref != -1) {
            ans.push_back(words[next->ref]);
            next->ref = -1;  // avoid duplicates
        }

        char saved = board[i][j];
        board[i][j] = '#';  // mark visited

        const int dirs[5] = {-1, 0, 1, 0, -1};
        for (int k = 0; k < 4; ++k) {
            int x = i + dirs[k];
            int y = j + dirs[k + 1];
            if (x >= 0 && x < m && y >= 0 && y < n && board[x][y] != '#') {
                dfs(next, x, y);
            }
        }

        board[i][j] = saved;  // restore
    };

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            dfs(root, i, j);
        }
    }

    return ans;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (assume included from above).

int main() {
    // Example 1: basic case
    std::vector<std::vector<char>> board1 = {
        {'o','a','a','n'},
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };
    std::vector<std::string> words1 = {"oath","pea","eat","rain"};
    std::vector<std::string> result1 = findWords(board1, words1);
    std::sort(result1.begin(), result1.end());
    std::vector<std::string> expected1 = {"eat","oath"};
    assert(result1 == expected1);

    // Example 2: single letter word
    std::vector<std::vector<char>> board2 = {{'a'}};
    std::vector<std::string> words2 = {"a"};
    assert(findWords(board2, words2) == std::vector<std::string>{"a"});

    // Example 3: word not present
    std::vector<std::vector<char>> board3 = {{'a','b'},{'c','d'}};
    std::vector<std::string> words3 = {"ab","cd","ac","db","abc"};
    std::vector<std::string> result3 = findWords(board3, words3);
    std::sort(result3.begin(), result3.end());
    std::vector<std::string> expected3 = {"ab","ac","cd","db"};
    assert(result3 == expected3);

    // Example 4: duplicate words in input list
    std::vector<std::vector<char>> board4 = {{'a','b'},{'b','a'}};
    std::vector<std::string> words4 = {"ab","ab","ba"};
    std::vector<std::string> result4 = findWords(board4, words4);
    std::sort(result4.begin(), result4.end());
    std::vector<std::string> expected4 = {"ab","ba"};
    assert(result4 == expected4);

    // Example 5: word is substring of another
    std::vector<std::vector<char>> board5 = {{'a','b','c'}};
    std::vector<std::string> words5 = {"a","ab","abc"};
    std::vector<std::string> result5 = findWords(board5, words5);
    std::sort(result5.begin(), result5.end());
    std::vector<std::string> expected5 = {"a","ab","abc"};
    assert(result5 == expected5);

    // Example 6: empty board? Not allowed by spec, but test empty words list
    std::vector<std::vector<char>> board6 = {{'a'}};
    std::vector<std::string> words6 = {};
    assert(findWords(board6, words6).empty());

    // Example 7: multiple paths but same word
    std::vector<std::vector<char>> board7 = {{'a','a'},{'a','a'}};
    std::vector<std::string> words7 = {"aaa"};
    std::vector<std::string> result7 = findWords(board7, words7);
    assert(result7.size() == 1 && result7[0] == "aaa");

    // Example 8: no word is found
    std::vector<std::vector<char>> board8 = {{'x','y'},{'z','w'}};
    std::vector<std::string> words8 = {"abc","def"};
    assert(findWords(board8, words8).empty());

    return 0;
}

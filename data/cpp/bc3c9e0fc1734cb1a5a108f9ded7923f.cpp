Write a C++ function `vector<string> findWordsInGrid(const vector<vector<char>>& board, const vector<string>& words)` that, given a 2D grid of lowercase letters and a list of target words, returns all words from the list that can be formed by sequentially adjacent cells (horizontally or vertically neighboring, no diagonal moves). Each cell may be used at most once per word, and the same word may appear once in the result even if there are multiple occurrences on the board. The grid is guaranteed to be rectangular with at least one row and one column. The function must handle duplicate words in the input list, empty words, and cases where no words are found.
The problem is a classic word search on a grid, best solved using a trie (prefix tree) to efficiently prune the DFS. First, insert all target words into the trie. Then perform a depth-first search from every cell in the board, carrying the current trie node corresponding to the prefix built so far. At each step, if the current cell is out of bounds, already visited (marked with a sentinel like `'0'`), or has no corresponding child in the trie, backtrack. Otherwise, append the character to the current word, and if the current trie node marks a word ending, add the word to the result set (deduplicate using a set or in-place check). Then mark the cell as visited, recurse on all four neighbors, and unmark the cell to allow reuse in other paths. Critical edge cases: (1) empty words in the input list—since the trie root has `finish = true` after inserting `""`, the DFS from a cell will never hit the empty string because we always advance the trie by at least one character; however, the empty word should be included if present. To handle this correctly, check for empty words before DFS and add them to the result if present. (2) Duplicate words in input—the trie insertion naturally handles duplicates (same path), but we must avoid duplicating results, so use a `std::set` for found words. (3) A word may be a prefix of another, so we must continue DFS after finding a word, not stop. Time complexity: Let `R` and `C` be grid dimensions, `W` total number of characters in all words, and `L` be the maximum word length. Building the trie takes `O(W)`. The DFS worst-case explores all paths of length up to `L` from each cell, but with trie pruning. In the worst case (all cells same letter, all words long), it is `O(R*C*4^L)` but typically much less; with trie and visited marking, each cell is visited at most once per path. Space complexity: `O(W)` for the trie, plus `O(L)` recursion stack, plus `O(R*C)` for board modifications in place.
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
using namespace std;

// Trie node for efficient prefix matching
struct TrieNode {
    TrieNode* children[26] = {nullptr};
    bool isEnd = false;
};

// Insert a word into the trie
void insertWord(TrieNode* root, const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!node->children[idx]) {
            node->children[idx] = new TrieNode();
        }
        node = node->children[idx];
    }
    node->isEnd = true;
}

// Depth-first search from (r, c) to collect matching words
void dfs(const vector<vector<char>>& board, int r, int c, TrieNode* node,
         string& current, set<string>& found) {
    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size()) return;
    if (board[r][c] == '\0') return; // visited sentinel

    int idx = board[r][c] - 'a';
    if (!node->children[idx]) return;

    node = node->children[idx];
    current.push_back(board[r][c]);

    if (node->isEnd) {
        found.insert(current);
    }

    char original = board[r][c];
    board[r][c] = '\0';  // mark visited

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    for (int dir = 0; dir < 4; ++dir) {
        dfs(board, r + dr[dir], c + dc[dir], node, current, found);
    }

    board[r][c] = original; // backtrack
    current.pop_back();
}

// Main function: return all words found in the grid
vector<string> findWordsInGrid(const vector<vector<char>>& board, const vector<string>& words) {
    TrieNode* root = new TrieNode();
    for (const string& w : words) {
        if (!w.empty()) {
            insertWord(root, w);
        }
    }

    set<string> foundSet;
    // Handle empty word if present in the input
    for (const string& w : words) {
        if (w.empty()) {
            foundSet.insert("");
        }
    }

    string current = "";
    int rows = board.size();
    int cols = board[0].size();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            dfs(board, r, c, root, current, foundSet);
        }
    }

    vector<string> result(foundSet.begin(), foundSet.end());

    // Clean up trie memory
    // (omitted for brevity, but in production you'd delete recursively)
    return result;
}
#include <cassert>
#include <vector>
#include <string>
using namespace std;

// Include the solution function here (or link it)

int main() {
    // Test 1: basic
    vector<vector<char>> board1 = {{'o','a','a','n'}, {'e','t','a','e'}, {'i','h','k','r'}, {'i','f','l','v'}};
    vector<string> words1 = {"oath","pea","eat","rain"};
    vector<string> result1 = findWordsInGrid(board1, words1);
    assert(result1.size() == 2);
    assert(find(result1.begin(), result1.end(), "oath") != result1.end());
    assert(find(result1.begin(), result1.end(), "eat") != result1.end());

    // Test 2: empty word in input
    vector<vector<char>> board2 = {{'a','b'}};
    vector<string> words2 = {"", "a"};
    vector<string> result2 = findWordsInGrid(board2, words2);
    assert(result2.size() == 2);
    assert(find(result2.begin(), result2.end(), "") != result2.end());
    assert(find(result2.begin(), result2.end(), "a") != result2.end());

    // Test 3: duplicate words
    vector<vector<char>> board3 = {{'a','b'}, {'c','d'}};
    vector<string> words3 = {"ab", "ab", "cd"};
    vector<string> result3 = findWordsInGrid(board3, words3);
    assert(result3.size() == 2);
    assert(find(result3.begin(), result3.end(), "ab") != result3.end());
    assert(find(result3.begin(), result3.end(), "cd") != result3.end());

    // Test 4: word not present
    vector<vector<char>> board4 = {{'a','b'}};
    vector<string> words4 = {"ac"};
    vector<string> result4 = findWordsInGrid(board4, words4);
    assert(result4.empty());

    // Test 5: single cell
    vector<vector<char>> board5 = {{'x'}};
    vector<string> words5 = {"x", "y"};
    vector<string> result5 = findWordsInGrid(board5, words5);
    assert(result5.size() == 1);
    assert(result5[0] == "x");

    // Test 6: word requires non-linear path (snake)
    vector<vector<char>> board6 = {{'a','b'}, {'d','c'}};
    vector<string> words6 = {"abcd"};
    vector<string> result6 = findWordsInGrid(board6, words6);
    assert(result6.size() == 1);
    assert(result6[0] == "abcd");

    // Test 7: all cells same letter, multiple words
    vector<vector<char>> board7 = {{'a','a'}, {'a','a'}};
    vector<string> words7 = {"a", "aa", "aaa", "aaaa"};
    vector<string> result7 = findWordsInGrid(board7, words7);
    assert(result7.size() == 4);
    assert(find(result7.begin(), result7.end(), "a") != result7.end());
    assert(find(result7.begin(), result7.end(), "aa") != result7.end());
    assert(find(result7.begin(), result7.end(), "aaa") != result7.end());
    assert(find(result7.begin(), result7.end(), "aaaa") != result7.end());

    // Test 8: no words
    vector<vector<char>> board8 = {{'a','b'}};
    vector<string> words8 = {};
    vector<string> result8 = findWordsInGrid(board8, words8);
    assert(result8.empty());

    // Test 9: one letter grid, empty word only
    vector<vector<char>> board9 = {{'z'}};
    vector<string> words9 = {""};
    vector<string> result9 = findWordsInGrid(board9, words9);
    assert(result9.size() == 1);
    assert(result9[0] == "");

    // Test 10: large grid but no matching words
    vector<vector<char>> board10 = {{'a','b','c'}, {'d','e','f'}, {'g','h','i'}};
    vector<string> words10 = {"xyz"};
    vector<string> result10 = findWordsInGrid(board10, words10);
    assert(result10.empty());

    return 0;
}

// Write a standalone C++ function that, given a 2D character grid (board) of size `m` rows by `n` columns, a list of dictionary words (as a vector of strings), and the maximum possible word length to consider (which is the length of the longest dictionary word), returns all dictionary words that can be formed by traversing adjacent cells (horizontally, vertically, or diagonally, in any of the 8 directions) without reusing any cell in a single word. The search must start from any cell, and words can be formed in any direction (including backwards). The returned list should preserve the original order of the dictionary input. If no word is found, return an empty vector. Assume the grid contains only lowercase letters, and all dictionary words also consist of lowercase letters. The function signature must be: `std::vector<std::string> findWords(const std::vector<std::string>& dictionary, const std::vector<std::vector<char>>& board)`. Inside, you may compute the maximum word length from the dictionary, but you should not rely on any global state.

#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be included above.)

int main() {
    // Test 1: Simple 3x3 board with a few words.
    std::vector<std::vector<char>> board1 = {
        {'a','b','c'},
        {'d','e','f'},
        {'g','h','i'}
    };
    std::vector<std::string> dict1 = {"abc", "cfi", "adg", "beh", "ab", "z"};
    std::vector<std::string> res1 = findWords(dict1, board1);
    // 'abc' (index 0), 'cfi' (index 1), 'adg' (index 2), 'beh' (index 3), 'ab' (index 4)
    // 'z' not present.
    std::vector<std::string> expected1 = {"abc", "cfi", "adg", "beh", "ab"};
    assert(res1 == expected1);

    // Test 2: No words found.
    std::vector<std::vector<char>> board2 = {{'x','y'}, {'z','w'}};
    std::vector<std::string> dict2 = {"cat", "dog"};
    std::vector<std::string> res2 = findWords(dict2, board2);
    assert(res2.empty());

    // Test 3: Single character word.
    std::vector<std::vector<char>> board3 = {{'a'}};
    std::vector<std::string> dict3 = {"a", "aa"};
    std::vector<std::string> res3 = findWords(dict3, board3);
    std::vector<std::string> expected3 = {"a"};
    assert(res3 == expected3);

    // Test 4: Empty dictionary.
    std::vector<std::string> res4 = findWords({}, board1);
    assert(res4.empty());

    // Test 5: Empty board.
    std::vector<std::vector<char>> board5;
    std::vector<std::string> res5 = findWords({"abc"}, board5);
    assert(res5.empty());

    // Test 6: Duplicate dictionary words appear once.
    std::vector<std::vector<char>> board6 = {{'c','a','t'}};
    std::vector<std::string> dict6 = {"cat", "cat"};
    std::vector<std::string> res6 = findWords(dict6, board6);
    std::vector<std::string> expected6 = {"cat"};
    assert(res6 == expected6);

    // Test 7: Word formed in reverse diagonal.
    std::vector<std::vector<char>> board7 = {
        {'r','a','c'},
        {'e','t','f'},
        {'d','g','h'}
    };
    std::vector<std::string> dict7 = {"cat", "car", "red", "ate"};
    std::vector<std::string> res7 = findWords(dict7, board7);
    // 'car' from (0,1) start? Actually 'c' at (0,2) -> 'a' at (1,1) -> 'r' at (2,0) is diagonal back. Also 'red' from (0,1) 'r' -> (1,0) 'e' -> (2,0) 'd'? No, (2,0) is 'd' so 'red' from (0,1) 'r' -> (1,0) 'e' -> (2,1) 'g'? Not 'd'. Let's check words present: 'cat' not present (no t adjacent to a? Actually a at (0,1), t at (1,1), c at (0,2) -> 'cat' would be c-a-t with path (0,2)->(1,1)->(1,2)? (1,2) is 'f' so no. 'car': c(0,2)->a(1,1)->r(2,0) yes. 'red': r(0,1)->e(1,0)->d(2,0) yes. 'ate': a(0,1)->t(1,1)->e(1,0) yes. So expect "car","red","ate". Order by dictionary: car(index1), red(index2), ate(index3). So expected = {"car","red","ate"}.
    std::vector<std::string> expected7 = {"car", "red", "ate"};
    assert(res7 == expected7);

    return 0;
}

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// DFS helper to find words on the board.
void dfsHelper(const std::vector<std::vector<char>>& board,
               int r, int c,
               const std::unordered_set<std::string>& dictSet,
               int maxLen,
               std::string& current,
               std::vector<std::vector<bool>>& visited,
               std::unordered_set<std::string>& foundWords) {
    // Prune if current length exceeds max possible dictionary word length.
    if (current.size() >= maxLen) {
        return;
    }

    current.push_back(board[r][c]);
    visited[r][c] = true;

    // Check if current string is a dictionary word.
    if (dictSet.find(current) != dictSet.end()) {
        foundWords.insert(current);
    }

    // Explore all 8 adjacent directions.
    static const int dirs[8][2] = {{1,0},{0,-1},{0,1},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
    int m = static_cast<int>(board.size());
    int n = static_cast<int>(board[0].size());

    for (int d = 0; d < 8; ++d) {
        int nr = r + dirs[d][0];
        int nc = c + dirs[d][1];
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && !visited[nr][nc]) {
            dfsHelper(board, nr, nc, dictSet, maxLen, current, visited, foundWords);
        }
    }

    // Backtrack.
    current.pop_back();
    visited[r][c] = false;
}

// Main function to find all dictionary words present on the board.
std::vector<std::string> findWords(const std::vector<std::string>& dictionary,
                                   const std::vector<std::vector<char>>& board) {
    std::vector<std::string> result;
    if (board.empty() || board[0].empty() || dictionary.empty()) {
        return result;
    }

    // Compute the maximum word length to prune DFS.
    int maxLen = 0;
    for (const auto& word : dictionary) {
        maxLen = std::max(maxLen, static_cast<int>(word.size()));
    }

    // Store dictionary words in a set for O(1) lookup.
    std::unordered_set<std::string> dictSet(dictionary.begin(), dictionary.end());

    // To preserve original order, map each word to its index.
    std::unordered_map<std::string, int> wordIndex;
    for (int i = 0; i < static_cast<int>(dictionary.size()); ++i) {
        // If duplicate words exist, keep the first occurrence's index.
        if (wordIndex.find(dictionary[i]) == wordIndex.end()) {
            wordIndex[dictionary[i]] = i;
        }
    }

    int m = static_cast<int>(board.size());
    int n = static_cast<int>(board[0].size());

    std::vector<std::vector<bool>> visited(m, std::vector<bool>(n, false));
    std::unordered_set<std::string> foundWords;

    // Start DFS from every cell.
    std::string current;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            dfsHelper(board, i, j, dictSet, maxLen, current, visited, foundWords);
        }
    }

    // Sort found words by their original dictionary order.
    std::vector<std::string> found(foundWords.begin(), foundWords.end());
    std::sort(found.begin(), found.end(),
              [&wordIndex](const std::string& a, const std::string& b) {
                  return wordIndex[a] < wordIndex[b];
              });

    return found;
}

// The core algorithm is a depth-first search (DFS) from each cell on the board. For each starting cell (i,j), we perform DFS that expands in all 8 directions, building a candidate string character by character. We stop the DFS when the current string length reaches the maximum dictionary word length (since no longer word can be in the dictionary). At each step, we check if the current string exists in the dictionary; if yes, we record it. To avoid revisiting cells, we maintain a boolean visited matrix that is toggled on entry and off on exit (backtracking). To efficiently check dictionary membership, we use an unordered_set of all dictionary words. However, to preserve the output order as given in the input dictionary, we also maintain an unordered_map from word to its original index, then sort the found words by that index at the end (or use a set). Edge cases: The board may have 0 rows or 0 columns, in which case return empty. The dictionary may be empty, return empty. A single-character word must be found if that character appears anywhere on the board. Duplicate words in the dictionary should be output once. The same word might be formed from different starting positions or paths; we must ensure it is output only once (use a set of found words). Time complexity: For each of the m*n cells, we start a DFS that in the worst case explores all possible paths of length up to L (the maximum word length). The branching factor is 8, so the number of paths from a single cell is O(8^L), but since we prune by length and visited cells, it is bounded by that. With dictionary lookup O(1) using a hash set, total time is O(m*n*8^L) in the worst case, but typically much less due to visited restrictions. Space complexity is O(m*n) for the visited matrix, plus O(total dictionary characters) for the hash set, and O(L) for the recursion stack depth.

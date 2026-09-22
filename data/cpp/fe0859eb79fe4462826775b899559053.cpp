// Write a C++ function `bool containsWord(const std::vector<std::vector<char>>& board, const std::string& word)` that determines whether the given word can be constructed by traversing adjacent cells (horizontally or vertically) in a rectangular 2D board of characters. Each cell can be used at most once in a single path, and paths may start at any cell. The board may be empty (0 rows or 0 columns), and the word may be empty. The function must handle edge cases such as a word longer than the number of cells, or a board with no characters. Return `true` if the word exists, `false` otherwise.
The solution uses depth-first search (DFS) with backtracking. For each cell in the board, attempt to match the first character of the word; if it matches, recursively try each of the four directions (up, down, left, right) to match the next character, while marking the current cell as visited to prevent reuse. If the recursion reaches the end of the word (index == word.length()), the word has been found. If a path fails (out-of-bounds, mismatched character, or already visited), backtrack by unmarking the cell. Important edge cases: empty board (return false unless word is empty), empty word (return true immediately), word longer than total cells (DFS will fail naturally), and single-character word (check each cell). Time complexity is O(M * N * 4^L) in the worst case, where M and N are board dimensions and L is word length, since each starting cell may explore up to 4 branches per character. Space complexity is O(M * N) for the visited matrix plus O(L) recursion stack depth.
#include <vector>
#include <string>
#include <functional>

// Determine if 'word' can be found in 'board' by moving to adjacent cells (up/down/left/right)
// without reusing any cell more than once in a single path.
bool containsWord(const std::vector<std::vector<char>>& board, const std::string& word) {
    if (word.empty()) return true;
    if (board.empty() || board[0].empty()) return false;

    const int rows = static_cast<int>(board.size());
    const int cols = static_cast<int>(board[0].size());

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Depth-first search from cell (r, c) attempting to match word starting at index 'idx'.
    std::function<bool(int, int, int)> dfs = [&](int r, int c, int idx) -> bool {
        if (idx == static_cast<int>(word.size())) return true;
        if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
        if (visited[r][c] || board[r][c] != word[idx]) return false;

        visited[r][c] = true;
        bool found = dfs(r + 1, c, idx + 1) ||
                     dfs(r - 1, c, idx + 1) ||
                     dfs(r, c + 1, idx + 1) ||
                     dfs(r, c - 1, idx + 1);
        visited[r][c] = false;  // backtrack
        return found;
    };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (dfs(r, c, 0)) return true;
        }
    }
    return false;
}
#include <cassert>
#include <vector>
#include <string>

// Assume containsWord is defined above.

int main() {
    std::vector<std::vector<char>> board1 = {
        {'A','B','C','E'},
        {'S','F','C','S'},
        {'A','D','E','E'}
    };

    assert(containsWord(board1, "ABCCED") == true);
    assert(containsWord(board1, "SEE") == true);
    assert(containsWord(board1, "ABCB") == false);
    assert(containsWord(board1, "A") == true);
    assert(containsWord(board1, "Z") == false);
    assert(containsWord(board1, "") == true);

    // Single-row board
    std::vector<std::vector<char>> board2 = {{'a'}};
    assert(containsWord(board2, "a") == true);
    assert(containsWord(board2, "aa") == false);

    // Empty board
    std::vector<std::vector<char>> board3 = {};
    assert(containsWord(board3, "x") == false);
    assert(containsWord(board3, "") == true);

    // Board with zero-width columns (but rows exist)
    std::vector<std::vector<char>> board4 = {{}, {}};
    assert(containsWord(board4, "a") == false);
    assert(containsWord(board4, "") == true);

    // Path that revisits would be needed but not allowed
    std::vector<std::vector<char>> board5 = {
        {'A','B'},
        {'C','D'}
    };
    assert(containsWord(board5, "ABD") == true);
    assert(containsWord(board5, "ABC") == false);  // would need to revisit a cell

    return 0;
}

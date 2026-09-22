// Write a C++ function `bool wordExists(const std::vector<std::vector<char>>& board, const std::string& word)` that determines whether a given word can be constructed from sequentially adjacent cells in a rectangular grid of characters (`board`). Adjacent cells are horizontally or vertically neighboring (not diagonal). The same cell may not be used more than once per construction. The function must handle an empty board, an empty word (return true immediately), and non-rectangular boards gracefully (assume all rows have equal length; if the board is empty, return false unless the word is also empty). Return `true` if the word exists, otherwise `false`.

The solution uses depth-first search (DFS) with backtracking. For each cell in the board, attempt to start a search for the word: if the current character matches the first character of the remaining word, mark the cell as visited (e.g., replace it with a sentinel like `*`), recursively search in all four directions for the next character, and after the recursive calls, restore the cell's original character (backtrack) so other paths can reuse it. The base cases: if the entire word has been matched (wordIndex == word.length()) return true; if the current position is out of bounds or the character does not match, return false. Important edge cases: empty word returns true regardless of board (since no characters needed); empty board and non-empty word returns false; board with only one cell; duplicate characters; and cycles—the visited marking prevents infinite loops and reuse. The time complexity is \(O(R \times C \times 4^L)\) in the worst case where R and C are board dimensions and L is the word length, because from each starting cell we may explore up to 4 branches per character. Space complexity is \(O(L)\) for the recursion stack, plus no extra auxiliary storage beyond the board copy (the board is passed by const reference and we create a local mutable copy if needed, or use a separate visited array to keep the original const). For simplicity and correctness with `const` input, we copy the board into a local mutable vector in the solution.

#include <vector>
#include <string>

// Determines if the given word can be formed by sequentially adjacent cells
// (up, down, left, right) in the board without reusing any cell.
bool wordExists(const std::vector<std::vector<char>>& board, const std::string& word) {
    if (board.empty() || board[0].empty()) {
        return word.empty();
    }
    if (word.empty()) {
        return true;
    }
    int rows = static_cast<int>(board.size());
    int cols = static_cast<int>(board[0].size());

    // Mutable copy to allow marking visited cells.
    std::vector<std::vector<char>> grid = board;

    // Depth-first search with backtracking.
    std::function<bool(int, int, int)> dfs = [&](int r, int c, int idx) -> bool {
        if (idx == static_cast<int>(word.size())) {
            return true;
        }
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] != word[idx]) {
            return false;
        }
        // Temporarily mark as visited.
        char original = grid[r][c];
        grid[r][c] = '*';
        // Explore all four directions.
        bool found = dfs(r + 1, c, idx + 1) ||
                     dfs(r - 1, c, idx + 1) ||
                     dfs(r, c + 1, idx + 1) ||
                     dfs(r, c - 1, idx + 1);
        // Restore original character (backtrack).
        grid[r][c] = original;
        return found;
    };

    // Try every cell as a starting point.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (dfs(i, j, 0)) {
                return true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>
#include <string>
#include <functional>

// Include the solution function here (or link) - for standalone compilation, paste the solution above.

int main() {
    // Basic case: word found horizontally.
    std::vector<std::vector<char>> board1 = {{'A','B','C','E'}, {'S','F','C','S'}, {'A','D','E','E'}};
    assert(wordExists(board1, "ABCCED") == true);

    // Basic case: word found vertically.
    assert(wordExists(board1, "SEE") == true);

    // Word not present due to reuse restriction.
    assert(wordExists(board1, "ABCB") == false);

    // Single character board matching word.
    std::vector<std::vector<char>> board2 = {{'X'}};
    assert(wordExists(board2, "X") == true);

    // Single character board not matching word.
    assert(wordExists(board2, "Y") == false);

    // Empty word always true.
    assert(wordExists(board2, "") == true);

    // Empty board with non-empty word false.
    std::vector<std::vector<char>> board3;
    assert(wordExists(board3, "A") == false);

    // Empty board with empty word true.
    assert(wordExists(board3, "") == true);

    // Word requiring snake path.
    std::vector<std::vector<char>> board4 = {{'a','b','c'}, {'d','e','f'}, {'g','h','i'}};
    assert(wordExists(board4, "abcfed") == true);
    assert(wordExists(board4, "abcfedz") == false);

    // All same characters but insufficient due to adjacency.
    std::vector<std::vector<char>> board5 = {{'a','a'}, {'a','a'}};
    assert(wordExists(board5, "aaaa") == true);
    assert(wordExists(board5, "aaaaa") == false);

    return 0;
}

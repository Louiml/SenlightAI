Write a C++ function `bool solveSudoku(std::vector<std::vector<char>>& board)` that takes a 9×9 Sudoku board represented as a vector of vectors of characters (`'.'` for empty cells, digits `'1'`–`'9'` for filled cells) and modifies the board in-place to fill all empty cells with digits so that every row, column, and 3×3 subgrid contains each digit exactly once. The input is guaranteed to have at least one valid solution, and the function should return `true` if a solution was found and the board is filled, and `false` if it fails (which should not occur for valid inputs). Use a backtracking search that always picks the empty cell with the fewest possible candidate digits to improve efficiency.

// The solution uses a classic backtracking approach with constraint propagation and a heuristic for variable ordering. First, initialize three sets of possible digits per row, column, and 3×3 block (initially all digits '1'..'9'). For every pre-filled cell, remove that digit from its corresponding row, column, and block sets. Also maintain a list of all empty cells. In the recursive `dfs` function, pick the empty cell with the smallest number of candidate digits (computed as the intersection of the row, column, and block sets). For each candidate digit in that cell, place it, update the sets, and recurse; if the recursion succeeds, return true; otherwise undo the placement and try the next candidate. If no candidates work, backtrack. The algorithm terminates because each move reduces the number of empty cells. Time complexity is exponential in the worst case but typically very fast due to the MRV heuristic; space complexity is O(1) auxiliary beyond the board and the fixed 9×9 arrays of sets.

#include <vector>
#include <set>
#include <algorithm>
#include <stack>

bool solveSudoku(std::vector<std::vector<char>>& board) {
    std::vector<std::set<char>> rows(9), cols(9), blocks(9);
    std::set<char> all_digits;
    for (char c = '1'; c <= '9'; ++c) {
        all_digits.insert(c);
    }
    for (int i = 0; i < 9; ++i) {
        rows[i] = all_digits;
        cols[i] = all_digits;
        blocks[i] = all_digits;
    }
    std::vector<std::pair<int,int>> empty_cells;
    
    auto update = [&](int r, int c, char val) {
        rows[r].erase(val);
        cols[c].erase(val);
        blocks[3*(r/3) + c/3].erase(val);
    };
    
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == '.') {
                empty_cells.emplace_back(i, j);
            } else {
                update(i, j, board[i][j]);
            }
        }
    }
    
    auto intersection = [](const std::set<char>& a, const std::set<char>& b) {
        std::set<char> result;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                              std::inserter(result, result.begin()));
        return result;
    };
    
    std::function<bool()> dfs = [&]() -> bool {
        if (empty_cells.empty()) return true;
        
        // Find the empty cell with the minimum number of candidates
        int best_index = -1;
        int best_count = 10;
        std::set<char> best_choices;
        for (int idx = 0; idx < (int)empty_cells.size(); ++idx) {
            int r = empty_cells[idx].first;
            int c = empty_cells[idx].second;
            auto choices = intersection(intersection(rows[r], cols[c]), 
                                        blocks[3*(r/3) + c/3]);
            if ((int)choices.size() < best_count) {
                best_count = choices.size();
                best_index = idx;
                best_choices = choices;
                if (best_count == 1) break; // cannot do better
            }
        }
        
        if (best_count == 0) return false; // no possible digits
        
        int r = empty_cells[best_index].first;
        int c = empty_cells[best_index].second;
        // Remove this cell from the list
        std::swap(empty_cells[best_index], empty_cells.back());
        empty_cells.pop_back();
        
        for (char val : best_choices) {
            board[r][c] = val;
            update(r, c, val);
            if (dfs()) return true;
            // Undo
            rows[r].insert(val);
            cols[c].insert(val);
            blocks[3*(r/3) + c/3].insert(val);
        }
        
        board[r][c] = '.';
        empty_cells.push_back({r, c});
        return false;
    };
    
    return dfs();
}

#include <cassert>
#include <vector>

int main() {
    // Valid simple puzzle
    std::vector<std::vector<char>> board1 = {
        {'5','3','.','.','7','.','.','.','.'},
        {'6','.','.','1','9','5','.','.','.'},
        {'.','9','8','.','.','.','.','6','.'},
        {'8','.','.','.','6','.','.','.','3'},
        {'4','.','.','8','.','3','.','.','1'},
        {'7','.','.','.','2','.','.','.','6'},
        {'.','6','.','.','.','.','2','8','.'},
        {'.','.','.','4','1','9','.','.','5'},
        {'.','.','.','.','8','.','.','7','9'}
    };
    assert(solveSudoku(board1));
    // Check all rows have digits 1-9
    for (int i = 0; i < 9; ++i) {
        std::vector<int> cnt(10,0);
        for (int j = 0; j < 9; ++j) cnt[board1[i][j]-'0']++;
        for (int d = 1; d <= 9; ++d) assert(cnt[d]==1);
    }
    // Check all columns
    for (int j = 0; j < 9; ++j) {
        std::vector<int> cnt(10,0);
        for (int i = 0; i < 9; ++i) cnt[board1[i][j]-'0']++;
        for (int d = 1; d <= 9; ++d) assert(cnt[d]==1);
    }
    // Check all 3x3 blocks
    for (int br = 0; br < 3; ++br) {
        for (int bc = 0; bc < 3; ++bc) {
            std::vector<int> cnt(10,0);
            for (int i = br*3; i < br*3+3; ++i)
                for (int j = bc*3; j < bc*3+3; ++j) cnt[board1[i][j]-'0']++;
            for (int d = 1; d <= 9; ++d) assert(cnt[d]==1);
        }
    }
    
    // Already solved board
    std::vector<std::vector<char>> board2 = {
        {'1','2','3','4','5','6','7','8','9'},
        {'4','5','6','7','8','9','1','2','3'},
        {'7','8','9','1','2','3','4','5','6'},
        {'2','3','1','5','6','4','8','9','7'},
        {'5','6','4','8','9','7','2','3','1'},
        {'8','9','7','2','3','1','5','6','4'},
        {'3','1','2','6','4','5','9','7','8'},
        {'6','4','5','9','7','8','3','1','2'},
        {'9','7','8','3','1','2','6','4','5'}
    };
    assert(solveSudoku(board2));
    
    // Highly constrained puzzle (only one empty cell)
    std::vector<std::vector<char>> board3 = {
        {'1','2','3','4','5','6','7','8','9'},
        {'4','5','6','7','8','9','1','2','3'},
        {'7','8','9','1','2','3','4','5','6'},
        {'2','3','1','5','6','4','8','9','7'},
        {'5','6','4','8','9','7','2','3','1'},
        {'8','9','7','2','3','1','5','6','4'},
        {'3','1','2','6','4','5','9','7','8'},
        {'6','4','5','9','7','8','3','1','2'},
        {'9','7','8','3','1','2','6','4','.'}
    };
    assert(solveSudoku(board3));
    assert(board3[8][8] == '5');
    
    // Empty board (but need valid solution exists)
    std::vector<std::vector<char>> board4(9, std::vector<char>(9, '.'));
    assert(solveSudoku(board4));
    // Check validity of board4
    for (int i = 0; i < 9; ++i) {
        std::vector<int> cnt(10,0);
        for (int j = 0; j < 9; ++j) cnt[board4[i][j]-'0']++;
        for (int d = 1; d <= 9; ++d) assert(cnt[d]==1);
    }
    
    return 0;
}

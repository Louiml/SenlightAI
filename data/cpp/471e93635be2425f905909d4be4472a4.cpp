Write a C++ function `removeEmptyRowsAndColumns` that takes a rectangular grid of characters (represented as a `vector<vector<char>>`) and returns a new grid with all rows and columns that consist entirely of the character `'.'` removed. The input grid may contain any printable ASCII characters, and the dimensions can be from 1×1 up to 100×100. The function should preserve the relative order of the remaining rows and columns, and if the entire grid consists only of `'.'`, the result should be an empty grid (size 0×0). You may assume the input is always rectangular (all rows have the same length).

// The algorithm has two main passes. First, identify which rows are entirely `'.'` by scanning each row and checking if every character is `'.'`. Similarly, identify which columns are entirely `'.'` by scanning each column. A row or column is marked for removal if all its characters are `'.'`. Then, construct the output grid by iterating only over rows and columns that are not marked for removal, copying the corresponding characters from the original grid. Edge cases include: an entirely `'.'` grid (returns empty), a grid with no `'.'` at all (returns identical copy), and single-row or single-column grids. The time complexity is O(H×W) for scanning and building, and the auxiliary space is O(H+W) for the boolean flags plus the output grid, which is O(H×W) in the worst case when nothing is removed.

#include <vector>
#include <string>

// Remove all rows and columns that consist entirely of the character '.'.
std::vector<std::vector<char>> removeEmptyRowsAndColumns(const std::vector<std::vector<char>>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return {};
    }

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // Determine which rows are entirely '.'.
    std::vector<bool> row_keep(rows, false);
    for (int i = 0; i < rows; ++i) {
        bool all_dot = true;
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != '.') {
                all_dot = false;
                break;
            }
        }
        row_keep[i] = !all_dot;
    }

    // Determine which columns are entirely '.'.
    std::vector<bool> col_keep(cols, false);
    for (int j = 0; j < cols; ++j) {
        bool all_dot = true;
        for (int i = 0; i < rows; ++i) {
            if (grid[i][j] != '.') {
                all_dot = false;
                break;
            }
        }
        col_keep[j] = !all_dot;
    }

    // Build the result grid.
    std::vector<std::vector<char>> result;
    for (int i = 0; i < rows; ++i) {
        if (!row_keep[i]) {
            continue;
        }
        std::vector<char> row;
        for (int j = 0; j < cols; ++j) {
            if (col_keep[j]) {
                row.push_back(grid[i][j]);
            }
        }
        result.push_back(row);
    }

    return result;
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (include header or paste it here).

int main() {
    // Test 1: Basic removal of an all-dot row and column.
    std::vector<std::vector<char>> grid1 = {
        {'a', '.', 'b'},
        {'.', '.', '.'},
        {'c', '.', 'd'}
    };
    std::vector<std::vector<char>> expected1 = {
        {'a', 'b'},
        {'c', 'd'}
    };
    assert(removeEmptyRowsAndColumns(grid1) == expected1);

    // Test 2: Entire grid is '.' -> empty result.
    std::vector<std::vector<char>> grid2 = {
        {'.', '.'},
        {'.', '.'}
    };
    assert(removeEmptyRowsAndColumns(grid2).empty());

    // Test 3: No '.' anywhere -> identical grid.
    std::vector<std::vector<char>> grid3 = {
        {'x', 'y'},
        {'z', 'w'}
    };
    assert(removeEmptyRowsAndColumns(grid3) == grid3);

    // Test 4: Only one row with a dot column.
    std::vector<std::vector<char>> grid4 = {
        {'p', '.', 'q'}
    };
    std::vector<std::vector<char>> expected4 = {
        {'p', 'q'}
    };
    assert(removeEmptyRowsAndColumns(grid4) == expected4);

    // Test 5: Only one column with a dot row.
    std::vector<std::vector<char>> grid5 = {
        {'r'},
        {'.'},
        {'s'}
    };
    std::vector<std::vector<char>> expected5 = {
        {'r'},
        {'s'}
    };
    assert(removeEmptyRowsAndColumns(grid5) == expected5);

    // Test 6: Mixed removal with multiple rows/columns kept.
    std::vector<std::vector<char>> grid6 = {
        {'.', 'a', '.'},
        {'.', 'b', '.'},
        {'.', '.', '.'}
    };
    std::vector<std::vector<char>> expected6 = {
        {'a'},
        {'b'}
    };
    assert(removeEmptyRowsAndColumns(grid6) == expected6);

    // Test 7: All rows and all columns are kept even if some dots exist.
    std::vector<std::vector<char>> grid7 = {
        {'1', '.', '2'},
        {'.', '3', '.'},
        {'4', '.', '5'}
    };
    std::vector<std::vector<char>> expected7 = grid7; // nothing is entirely dot
    assert(removeEmptyRowsAndColumns(grid7) == expected7);

    // Test 8: Single cell with '.' -> empty grid.
    std::vector<std::vector<char>> grid8 = {{'.'}};
    assert(removeEmptyRowsAndColumns(grid8).empty());

    // Test 9: 1x1 with non-dot -> same grid.
    std::vector<std::vector<char>> grid9 = {{'#'}};
    assert(removeEmptyRowsAndColumns(grid9) == grid9);

    // Test 10: Large-ish grid.
    std::vector<std::vector<char>> grid10 = {
        {'.', '.', '.', '.', '.'},
        {'.', 'A', '.', 'B', '.'},
        {'.', '.', '.', '.', '.'},
        {'.', 'C', '.', 'D', '.'},
        {'.', '.', '.', '.', '.'}
    };
    std::vector<std::vector<char>> expected10 = {
        {'A', 'B'},
        {'C', 'D'}
    };
    assert(removeEmptyRowsAndColumns(grid10) == expected10);

    return 0;
}

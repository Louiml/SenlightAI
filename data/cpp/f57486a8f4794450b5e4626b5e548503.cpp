Write a C++ function `int count_legal_directions(int piece_position, int board_size, const std::vector<int>& occupied_cells)` that takes a 1D-indexed position of a piece on a square board, the board size (number of cells per side), and a list of occupied cell indices (excluding the piece itself), and returns the number of distinct straight-line directions (horizontal, vertical, and both diagonals) in which the piece can legally move at least one cell without hitting an occupied cell or leaving the board. A direction is considered legal if the piece can move along that line (starting from its current position) for at least one step into an empty cell, considering that occupied cells block further movement in that direction. The board uses 0-based indexing (row-major order: index = row * board_size + column). The input list may be empty, unsorted, and may contain valid indices only. The function must not modify the input vector.
// The core idea is to check each of the eight possible movement directions from the piece’s current row and column. For each direction, we step one cell at a time away from the piece until we either exit the board or hit an occupied cell. If we take at least one valid step (i.e., the first step lands on an in-bounds, unoccupied cell), then that direction is legal. To avoid duplicating work, we group opposite directions into four lines (left-right, up-down, and two diagonals). For each line, we compute whether moving in the positive direction or negative direction yields any valid step. Since the occupied list may be unsorted, we convert it into a `std::unordered_set<int>` for O(1) membership tests. Edge cases include: the piece at a corner (some directions immediately go out of bounds), occupied cells adjacent to the piece (blocking that direction), an empty occupied list (all in-bounds directions are legal), and the board size being 1 (no movement possible). Time complexity is O(board_size + number_of_occupied_cells) because for each of 4 lines we step at most board_size cells, and building the set is O(number_of_occupied_cells). Space complexity is O(number_of_occupied_cells) for the set.
#include <vector>
#include <unordered_set>
#include <algorithm>

// Count the number of legal movement directions (lines) from a piece position.
// piece_position: 0-based index (row * board_size + column)
// board_size: number of cells per side (square board)
// occupied_cells: list of occupied indices (excluding the piece itself)
// Returns number of distinct lines (horizontal, vertical, two diagonals)
// along which the piece can move at least one step into an empty cell.
int count_legal_directions(int piece_position, int board_size, const std::vector<int>& occupied_cells) {
    if (board_size <= 1) {
        return 0;
    }

    int row = piece_position / board_size;
    int col = piece_position % board_size;

    // Store occupied cells for O(1) lookup
    std::unordered_set<int> occupied(occupied_cells.begin(), occupied_cells.end());

    // Helper lambda to test if a cell is empty and in bounds
    auto is_free = [&](int r, int c) -> bool {
        if (r < 0 || r >= board_size || c < 0 || c >= board_size) {
            return false;
        }
        int idx = r * board_size + c;
        return occupied.find(idx) == occupied.end();
    };

    int legal_count = 0;

    // Horizontal line: left and right
    bool legal_left = false;
    for (int c = col - 1; c >= 0; --c) {
        if (is_free(row, c)) {
            legal_left = true;
        } else {
            break;
        }
    }
    bool legal_right = false;
    for (int c = col + 1; c < board_size; ++c) {
        if (is_free(row, c)) {
            legal_right = true;
        } else {
            break;
        }
    }
    if (legal_left || legal_right) {
        ++legal_count;
    }

    // Vertical line: up and down
    bool legal_up = false;
    for (int r = row - 1; r >= 0; --r) {
        if (is_free(r, col)) {
            legal_up = true;
        } else {
            break;
        }
    }
    bool legal_down = false;
    for (int r = row + 1; r < board_size; ++r) {
        if (is_free(r, col)) {
            legal_down = true;
        } else {
            break;
        }
    }
    if (legal_up || legal_down) {
        ++legal_count;
    }

    // Main diagonal (top-left to bottom-right)
    bool legal_tl = false;
    for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
        if (is_free(r, c)) {
            legal_tl = true;
        } else {
            break;
        }
    }
    bool legal_br = false;
    for (int r = row + 1, c = col + 1; r < board_size && c < board_size; ++r, ++c) {
        if (is_free(r, c)) {
            legal_br = true;
        } else {
            break;
        }
    }
    if (legal_tl || legal_br) {
        ++legal_count;
    }

    // Anti-diagonal (top-right to bottom-left)
    bool legal_tr = false;
    for (int r = row - 1, c = col + 1; r >= 0 && c < board_size; --r, ++c) {
        if (is_free(r, c)) {
            legal_tr = true;
        } else {
            break;
        }
    }
    bool legal_bl = false;
    for (int r = row + 1, c = col - 1; r < board_size && c >= 0; ++r, --c) {
        if (is_free(r, c)) {
            legal_bl = true;
        } else {
            break;
        }
    }
    if (legal_tr || legal_bl) {
        ++legal_count;
    }

    return legal_count;
}
#include <cassert>
#include <vector>

// Forward declaration of the function under test (already included above)
int count_legal_directions(int piece_position, int board_size, const std::vector<int>& occupied_cells);

int main() {
    // Empty board, piece in center of 3x3 (index 4) -> all 4 lines legal
    assert(count_legal_directions(4, 3, {}) == 4);

    // 3x3, piece at corner (0), no obstacles -> left/up/up-left are out of bounds, right, down, down-right legal -> 3 lines
    assert(count_legal_directions(0, 3, {}) == 3);

    // 3x3, piece at center (4), block all four immediate neighbors -> no movement
    assert(count_legal_directions(4, 3, {1, 3, 5, 7}) == 0);

    // 3x3, piece at center, block only right neighbor (5) -> horizontal blocked, vertical, both diagonals legal
    assert(count_legal_directions(4, 3, {5}) == 3);

    // 5x5, piece at index 12 (row2,col2), block cells (2,1) and (2,3) -> horizontal blocked, vertical legal, diagonals legal
    assert(count_legal_directions(12, 5, {11, 13}) == 3);

    // 5x5, piece at index 0 (top-left), block (1,1) and (0,1) -> right blocked, down blocked, down-right blocked -> 0
    assert(count_legal_directions(0, 5, {1, 6}) == 0);

    // 1x1 board, piece at 0, no occupied -> no moves
    assert(count_legal_directions(0, 1, {}) == 0);

    // 2x2 board, piece at 0, no obstacles -> right, down, down-right legal -> 3 (left/up/up-left out of bounds)
    assert(count_legal_directions(0, 2, {}) == 3);

    // 4x4 board, piece at 5 (row1,col1), occupied at 0 (blocks up-left short?), but up-left still has (0,0) blocked -> up-left illegal, other directions check
    // Occupied: 0,4,6,9 -> 0 blocks up-left, 4 blocks up, 6 blocks right, 9 blocks down-right; left and down and up-right and down-left are free
    assert(count_legal_directions(5, 4, {0, 4, 6, 9}) == 4); // horizontal: left legal, right blocked -> legal; vertical: up blocked, down legal -> legal; main diagonal: up-left blocked, down-right blocked -> illegal; anti-diagonal: up-right legal, down-left legal -> legal.

    // Ensure function does not break with unsorted duplicate occupied lists
    assert(count_legal_directions(4, 3, {5, 1, 5, 3, 7}) == 0);

    return 0;
}

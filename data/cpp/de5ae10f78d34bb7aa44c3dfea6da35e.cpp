// Write a C++ function that simulates a simplified minesweeper board. The function should take two 11x11 character grids (`out` for the player-visible grid and `in` for the hidden mine layout) plus a row and column coordinate (both between 1 and 9 inclusive). If the cell in `in` contains a mine (character '1'), return 0 to indicate game over. If the cell in `out` is already revealed (not '*'), return 2 to indicate an invalid move. Otherwise, count how many mines are in the 3x3 neighborhood around (x,y) (including the cell itself), set `out[x][y]` to that count as a character digit ('0' to '9'), and return 2 to indicate a valid move. Assume the hidden grid `in` is pre-filled with '0' and '1' characters, and the outer border (row 0, row 10, column 0, column 10) can be safely read as containing '0' (no mines) to avoid bounds checking. The function must be named `sweep` and must not call any other functions, print to console, or modify `in`.

The solution is straightforward and follows directly from the original code’s logic. First, check if the cell at (x,y) in `out` is still unrevealed (`'*'`). If not, return 2 for an invalid move. If it is unrevealed, check the hidden grid’s cell: if it equals `'1'`, return 0 for hitting a mine. Otherwise, initialize a counter to zero and scan a 3x3 block from row x-1 to x+1 and column y-1 to y+1, incrementing the counter whenever `in[i][j] == '1'`. Since the coordinates are guaranteed to be in the valid 1..9 range and the outer border of `in` is filled with '0' (as set by the `start` function), no bounds checking is necessary. Finally, write the counter converted to a character (`count + '0'`) into `out[x][y]` and return 2. Edge cases: the cell may be on the border (e.g., x=1 or y=9), but the border cells are all '0', so the neighborhood scan remains safe. Duplicate mines in the neighborhood are counted correctly because each `'1'` increments the counter. Time complexity is O(1) as only a fixed 3x3 area is examined. Space complexity is O(1) as no extra data structures are used.

#include <cstddef>

// Reveal a cell on a simplified 9x9 minesweeper board.
// out: player-visible grid (initially '*', later digits or 'M')
// in: hidden mine layout ('0' safe, '1' mine), with border filled with '0'
// x,y: row and column in [1,9]
// Returns 0 if a mine is hit, 2 for a valid reveal or invalid move.
int sweep(char out[11][11], const char in[11][11], int x, int y) {
    if (out[x][y] == '*') {
        if (in[x][y] == '1') {
            return 0; // mine hit
        }
        int count = 0;
        for (int i = x - 1; i <= x + 1; ++i) {
            for (int j = y - 1; j <= y + 1; ++j) {
                if (in[i][j] == '1') {
                    ++count;
                }
            }
        }
        out[x][y] = static_cast<char>(count + '0');
        return 2; // valid move
    }
    return 2; // already revealed or marked
}

#include <cassert>

int main() {
    // Test 1: empty board (no mines) - revealing center shows '0'
    char out1[11][11] = {};
    char in1[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out1[i][j] = '*';
            in1[i][j] = '0';
        }
    int r1 = sweep(out1, in1, 5, 5);
    assert(r1 == 2);
    assert(out1[5][5] == '0');

    // Test 2: mine at (2,2), reveal (2,2) - game over
    char out2[11][11] = {};
    char in2[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out2[i][j] = '*';
            in2[i][j] = '0';
        }
    in2[2][2] = '1';
    int r2 = sweep(out2, in2, 2, 2);
    assert(r2 == 0);

    // Test 3: mine at (4,4), reveal (3,3) - counts neighbor mine
    char out3[11][11] = {};
    char in3[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out3[i][j] = '*';
            in3[i][j] = '0';
        }
    in3[4][4] = '1';
    int r3 = sweep(out3, in3, 3, 3);
    assert(r3 == 2);
    assert(out3[3][3] == '1');

    // Test 4: multiple mines in neighborhood
    char out4[11][11] = {};
    char in4[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out4[i][j] = '*';
            in4[i][j] = '0';
        }
    in4[1][1] = '1';
    in4[1][2] = '1';
    in4[2][1] = '1';
    int r4 = sweep(out4, in4, 2, 2);
    assert(r4 == 2);
    assert(out4[2][2] == '3');

    // Test 5: already revealed cell returns 2 without changing
    char out5[11][11] = {};
    char in5[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out5[i][j] = '*';
            in5[i][j] = '0';
        }
    out5[5][5] = '4';
    int r5 = sweep(out5, in5, 5, 5);
    assert(r5 == 2);
    assert(out5[5][5] == '4');

    // Test 6: border cell with mine outside 9x9 (should still count correctly, mine at 0,0 is outside valid area)
    char out6[11][11] = {};
    char in6[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out6[i][j] = '*';
            in6[i][j] = '0';
        }
    in6[0][0] = '1'; // border mine, not counted as neighbor for (1,1)
    int r6 = sweep(out6, in6, 1, 1);
    assert(r6 == 2);
    assert(out6[1][1] == '0');

    // Test 7: mine at corner, reveal adjacent corner
    char out7[11][11] = {};
    char in7[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out7[i][j] = '*';
            in7[i][j] = '0';
        }
    in7[9][9] = '1';
    int r7 = sweep(out7, in7, 9, 8);
    assert(r7 == 2);
    assert(out7[9][8] == '1');

    // Test 8: reveals '9' when surrounded by mines
    char out8[11][11] = {};
    char in8[11][11] = {};
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j) {
            out8[i][j] = '*';
            in8[i][j] = '0';
        }
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= 3; ++j)
            if (i != 2 || j != 2) in8[i][j] = '1'; // all neighbors mine
    int r8 = sweep(out8, in8, 2, 2);
    assert(r8 == 2);
    assert(out8[2][2] == '8'); // 8 surrounding cells

    return 0;
}

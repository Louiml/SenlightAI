Write a C++ function `int countStickers(int rows, int cols, const std::vector<std::string>& grid, const std::string& instructions)` that simulates a robot navigating a rectangular grid. The grid consists of cells that are either open (`.`), a wall (`#`), a sticker (`*`), or a robot start position (`N`, `S`, `L`, `O`), where `N`=north, `S`=south, `L`=east (leste), `O`=west (oeste). The robot begins facing the direction indicated by its starting letter, and that cell becomes open. The instructions string contains `D` (turn right/clockwise 90°), `E` (turn left/counterclockwise 90°), and `F` (move forward one cell in the current direction). When moving forward, the robot cannot pass through walls; if the next cell is a wall or outside the grid, the robot remains in place (it does not wrap around). If the next cell contains a sticker, the robot collects it (incrementing the count), that cell becomes open, and the robot moves onto it. The function should return the total number of stickers collected after processing all instructions. You may assume the grid is rectangular, contains exactly one robot starting letter, and all strings have lengths matching `rows` and `cols` respectively. The main challenge is to correctly handle turning and movement with boundary and wall checks.
// The solution simulates the robot state using three variables: row index, column index, and direction (0=north, 1=east, 2=south, 3=west). First, search the grid to locate the starting position and reset that cell to `.`, while setting the initial direction based on the letter. Then iterate through each instruction. For `D` and `E`, update the direction modulo 4, making sure negative values wrap to 3 (e.g., `(dir + 3) % 4` for left turn). For `F`, compute the candidate next cell based on the current direction. If the candidate is out of bounds, clamp it to the last valid index (though this is equivalent to staying in place because any out-of-bounds move is not allowed; the clamping ensures we check the edge cell as a wall, but since the robot is at the edge, the clamped cell is the current cell itself, which is always open, so the robot stays). Actually, better to explicitly check bounds: if new row/col is within [0, rows-1] and [0, cols-1] and the cell is not `#`, then move; if it is a sticker, increment count and set cell to `.`. Important edge case: turning does not change position; repeated `F` into walls does nothing; stickers are collected only once. Time complexity is O(rows*cols + len(instructions)) for scanning the grid and processing each instruction, space O(1) extra (excluding input storage).
#include <vector>
#include <string>

// Simulate the robot and return the number of stickers collected.
int countStickers(int rows, int cols, const std::vector<std::string>& grid, const std::string& instructions) {
    // Make a mutable copy of the grid
    std::vector<std::string> g = grid;
    
    int r = -1, c = -1, dir = 0;
    // Find the robot start
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            char ch = g[i][j];
            if (ch == 'N' || ch == 'S' || ch == 'L' || ch == 'O') {
                r = i; c = j;
                if (ch == 'N') dir = 0;      // north
                else if (ch == 'L') dir = 1; // east
                else if (ch == 'S') dir = 2; // south
                else if (ch == 'O') dir = 3; // west
                g[i][j] = '.'; // starting cell becomes open
                i = rows; j = cols; // break out of loops
            }
        }
    }
    
    int stickers = 0;
    // Direction vectors: N, E, S, W
    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};
    
    for (char inst : instructions) {
        if (inst == 'D') {
            dir = (dir + 1) % 4; // right turn
        } else if (inst == 'E') {
            dir = (dir + 3) % 4; // left turn (equivalent to -1 mod 4)
        } else if (inst == 'F') {
            int nr = r + dr[dir];
            int nc = c + dc[dir];
            // Check if the target is inside the grid
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (g[nr][nc] != '#') {
                    // Move is allowed
                    r = nr; c = nc;
                    if (g[r][c] == '*') {
                        ++stickers;
                        g[r][c] = '.';
                    }
                }
            }
            // If outside or wall, robot stays in place
        }
    }
    return stickers;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration from solution (include here for testing)
int countStickers(int rows, int cols, const std::vector<std::string>& grid, const std::string& instructions);

int main() {
    // Basic movement with stickers
    std::vector<std::string> grid1 = {
        "N.*",
        ".#.",
        "..."
    };
    assert(countStickers(3, 3, grid1, "FDFF") == 1);

    // Turning left and moving
    std::vector<std::string> grid2 = {
        "S*.",
        ".*.",
        "..."
    };
    assert(countStickers(3, 3, grid2, "EEF") == 1);

    // Wall blocks movement
    std::vector<std::string> grid3 = {
        "N#*",
        ".#.",
        "..."
    };
    assert(countStickers(3, 3, grid3, "FF") == 0);

    // Multiple stickers collected
    std::vector<std::string> grid4 = {
        "N*.*",
        "....",
        ".*.."
    };
    assert(countStickers(3, 4, grid4, "FFDDDDFF") == 2);

    // Boundary stops movement, no wrap
    std::vector<std::string> grid5 = {
        "N..",
        "...",
        "..."
    };
    assert(countStickers(3, 3, grid5, "FF") == 0);

    // Sticker collected only once
    std::vector<std::string> grid6 = {
        "N*.",
        "..."
    };
    assert(countStickers(2, 3, grid6, "FDF") == 1);

    // No instructions, count remains 0
    std::vector<std::string> grid7 = {
        "N*",
        ".."
    };
    assert(countStickers(2, 2, grid7, "") == 0);

    // All turns no movement
    std::vector<std::string> grid8 = {
        "O*",
        ".."
    };
    assert(countStickers(2, 2, grid8, "DEDE") == 0);
}

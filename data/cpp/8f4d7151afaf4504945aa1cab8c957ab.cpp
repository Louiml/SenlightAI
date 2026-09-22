/*
Write a C++ function `countValidPaths` that takes a rectangular grid of characters (`N` rows, `M` columns), a start cell marked with `'S'`, an end cell marked with `'E'`, walls marked with `'#'`, and a string of digit commands (each digit `0`–`3`). The digits represent four directions, but the mapping from digit to direction is not fixed: the function must consider all 24 possible permutations of the four directions `{up, down, left, right}`. For each permutation, simulate the robot starting at `'S'` and following the command string in order. The robot fails if it ever moves out of bounds or into a wall; it succeeds immediately upon reaching `'E'` before finishing all commands. The function must return the number of permutations for which the simulation reaches the end cell. The grid dimensions and command string are provided as parameters; assume dimensions are at most 60×60 and the command string length is at least 1. The grid contains exactly one `'S'` and one `'E'`, and no command string causes a successful path without hitting a wall or boundary unless it reaches `'E'`. The task is to implement the simulation and counting logic without using any global variables or external libraries beyond the C++ standard library.
*/
#include <vector>
#include <string>
#include <algorithm>

// Count how many permutations of the four directions {up, down, left, right}
// allow the robot to reach 'E' when following the command string from 'S'.
int countValidPaths(const std::vector<std::string>& grid, const std::string& commands) {
    const int N = static_cast<int>(grid.size());
    const int M = static_cast<int>(grid[0].size());

    // Direction vectors: {right, left, down, up} as per original dx/dy.
    // We'll use indices 0=up, 1=down, 2=left, 3=right for clarity.
    const int dr[4] = {-1, 1, 0, 0}; // up, down, left, right
    const int dc[4] = {0, 0, -1, 1};

    int start_r = -1, start_c = -1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'S') {
                start_r = i;
                start_c = j;
                break;
            }
        }
        if (start_r != -1) break;
    }

    // We will consider permutations of digits 0..3 mapping to direction indices.
    int order[4] = {0, 1, 2, 3};
    int valid_count = 0;

    do {
        int r = start_r;
        int c = start_c;
        bool success = false;

        for (char ch : commands) {
            int dir = order[ch - '0'];
            r += dr[dir];
            c += dc[dir];

            if (r < 0 || r >= N || c < 0 || c >= M || grid[r][c] == '#') {
                success = false;
                break;
            }
            if (grid[r][c] == 'E') {
                success = true;
                break;
            }
        }

        if (success) {
            ++valid_count;
        }
    } while (std::next_permutation(order, order + 4));

    return valid_count;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function declaration here (copy it or #include the code).
int countValidPaths(const std::vector<std::string>& grid, const std::string& commands);

int main() {
    // Test 1: Simple straight line to the right. Only one direction mapping works.
    std::vector<std::string> grid1 = {
        "S.E"
    };
    std::string cmd1 = "0"; // Need to map '0' to 'right' (direction index 3 in our function).
    // All permutations: '0' maps to {up,down,left,right}; only right works.
    assert(countValidPaths(grid1, cmd1) == 1);

    // Test 2: Two-step path: right then down.
    std::vector<std::string> grid2 = {
        "S..",
        "..E"
    };
    std::string cmd2 = "01"; // Need '0'->right and '1'->down.
    // Permutations where order[0]=right and order[1]=down: those two are fixed relative, but order[2], order[3] can be any of remaining 2! = 2.
    // There are 2 valid permutations out of 24.
    assert(countValidPaths(grid2, cmd2) == 2);

    // Test 3: Blocked by wall immediately: no permutation works.
    std::vector<std::string> grid3 = {
        "S#E"
    };
    std::string cmd3 = "0";
    // Any movement from S goes out or into #, so 0 valid.
    assert(countValidPaths(grid3, cmd3) == 0);

    // Test 4: Longer path with all four directions used.
    std::vector<std::string> grid4 = {
        "S..",
        ".#.",
        "..E"
    };
    std::string cmd4 = "00332211"; // Need right,right,down,down,left,left,up,up? Actually let's craft: from (0,0) to (2,2) via (0,1),(0,2),(1,2),(2,2) uses right,right,down,down.
    // Use "0011" to go right,right,down,down.
    std::string cmd4b = "0011";
    // For each permutation, digits must map: '0'->right, '1'->down. There are 2 permutations for the other two directions.
    assert(countValidPaths(grid4, cmd4b) == 2);

    // Test 5: Path that reaches E before consuming all commands.
    std::vector<std::string> grid5 = {
        "SEX" // X is just an open cell
    };
    std::string cmd5 = "000"; // Only need first '0' to be right.
    // The robot reaches E on first command; remaining commands ignored.
    // Only one mapping for '0'->right works, so 1 valid.
    assert(countValidPaths(grid5, cmd5) == 1);

    // Test 6: Start next to end but first command wrong direction.
    std::vector<std::string> grid6 = {
        "S#E"
    };
    std::string cmd6 = "1"; // '1' can never go right because left/up/down all fail.
    assert(countValidPaths(grid6, cmd6) == 0);

    // Test 7: All 24 permutations valid if commands always lead to E via any path? Not trivial, but check a simple 1x3 with cmd="0" as before.
    // Already tested.

    // Test 8: Empty command not allowed by spec, but verify no crash if it were? Not needed.

    return 0;
}
// The solution enumerates all permutations of the four movement directions `{up, down, left, right}` using `std::next_permutation` on an array of indices `{0,1,2,3}`. For each permutation, we simulate the robot: first locate the start position `(r,c)` by scanning the grid. Then iterate over each character in the command string. For each command digit `d`, apply the movement vector corresponding to `order[d]`, where `order` maps the digit to a direction index. Update the position; if it becomes out of bounds or lands on a wall, the permutation fails immediately. If it lands on `'E'`, the permutation succeeds and we count it. If the loop finishes without reaching `'E'`, the permutation fails. Edge cases include: a command may cause failure early, the robot may never reach the end even if it avoids walls, and multiple permutations may yield the same path but must each be counted separately. The time complexity is \(O(24 \times (N \cdot M + L))\), where \(L\) is the command length, since for each permutation we scan the grid once (O(N*M)) and simulate each command (O(L)). Space complexity is O(1) beyond the input storage, since we only use a few integers and the permutation array.

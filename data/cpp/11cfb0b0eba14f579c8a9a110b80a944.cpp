/*
Write a C++ function `solveAkari` that takes a rectangular grid of integers representing an Akari (Light Up) puzzle board and returns a completed solution grid. The input grid uses the following encoding: cells with values `-1` (black wall), `0` to `4` (black wall with a number indicating exactly how many adjacent orthogonal cells contain lights), `-2` (empty white cell), and `5` (light placed on a white cell). The function must place lights (`5`) on some `-2` cells such that: (1) every numbered black cell has exactly that many orthogonally adjacent lights; (2) no two lights shine on each other (i.e., no two lights are in the same row or column with only white cells between them); (3) every white cell (originally `-2` or `5`) is illuminated by at least one light from the same row or column without a black wall blocking. The solution should be found via backtracking with constraint propagation; if multiple solutions exist, return any valid one. The input is guaranteed to be solvable. The function should return a vector of vectors of the same dimensions, where `-1` and numbered walls remain unchanged, and every `-2` cell is either left as `-2` (if it was already illuminated and not chosen as a light) or set to `5`. For simplicity, you may leave unlit but illuminated cells as `-2`; however, the checker in the provided snippet considers `-2` as a cell that must not be completely unlit. In your solution, ensure that every `-2` cell is illuminated, but you may leave them as `-2` or set them to `5` as appropriate. The function signature is `std::vector<std::vector<int>> solveAkari(const std::vector<std::vector<int>>& grid)`. Your implementation must be self-contained, efficient for grids up to 10×10, and use standard C++17 features.
*/

#include <vector>
#include <functional>

// Solves an Akari puzzle. Returns a valid solution grid.
// Encoding: -1 = wall, 0..4 = numbered wall, -2 = empty, 5 = light.
std::vector<std::vector<int>> solveAkari(const std::vector<std::vector<int>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<int>> board = grid;

    // Directions: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    // Check if a cell is inside board
    auto inside = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };

    // Check if a cell is a wall (black cell with number or -1)
    auto isWall = [&](int r, int c) {
        return board[r][c] >= -1 && board[r][c] <= 4;
    };

    // Check if placing a light at (r,c) would cause two lights to shine on each other
    auto lightConflicts = [&](int r, int c) {
        // Check four directions until a wall
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            while (inside(nr, nc) && !isWall(nr, nc)) {
                if (board[nr][nc] == 5) return true; // another light
                nr += dr[d];
                nc += dc[d];
            }
        }
        return false;
    };

    // Check if a numbered wall is violated (too many lights already or impossible to satisfy)
    auto wallViolated = [&](int r, int c, int num) {
        int placed = 0, empty = 0;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (inside(nr, nc) && !isWall(nr, nc)) {
                if (board[nr][nc] == 5) placed++;
                else if (board[nr][nc] == -2) empty++;
            }
        }
        if (placed > num) return true;
        if (placed + empty < num) return true; // not enough cells to place required lights
        return false;
    };

    // Check if a white cell is illuminated (by any light in same row/col without wall)
    auto isIlluminated = [&](int r, int c) {
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            while (inside(nr, nc) && !isWall(nr, nc)) {
                if (board[nr][nc] == 5) return true;
                nr += dr[d];
                nc += dc[d];
            }
        }
        return false;
    };

    // Global validity check for complete solution
    auto isValidComplete = [&]() {
        // Check all numbered walls have correct adjacent lights
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c] >= 0 && board[r][c] <= 4) {
                    int cnt = 0;
                    for (int d = 0; d < 4; ++d) {
                        int nr = r + dr[d], nc = c + dc[d];
                        if (inside(nr, nc) && board[nr][nc] == 5) cnt++;
                    }
                    if (cnt != board[r][c]) return false;
                }
            }
        }
        // Check no two lights shine on each other (already ensured during placement, but double-check)
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c] == 5) {
                    for (int d = 0; d < 4; ++d) {
                        int nr = r + dr[d], nc = c + dc[d];
                        while (inside(nr, nc) && !isWall(nr, nc)) {
                            if (board[nr][nc] == 5) return false;
                            nr += dr[d];
                            nc += dc[d];
                        }
                    }
                }
            }
        }
        // Check every white cell is illuminated
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c] == -2) {
                    if (!isIlluminated(r, c)) return false;
                }
            }
        }
        return true;
    };

    // Backtracking search
    std::function<bool()> backtrack = [&]() -> bool {
        // Find an empty cell that is not illuminated yet (priority) or any empty cell
        int bestR = -1, bestC = -1;
        bool foundUnlit = false;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c] == -2) {
                    if (!foundUnlit && !isIlluminated(r, c)) {
                        bestR = r; bestC = c;
                        foundUnlit = true;
                        break;
                    } else if (!foundUnlit) {
                        bestR = r; bestC = c; // fallback to any empty
                    }
                }
            }
            if (foundUnlit) break;
        }
        // If no empty cell left, check if complete
        if (bestR == -1) {
            return isValidComplete();
        }

        // Try placing a light at (bestR,bestC)
        if (!lightConflicts(bestR, bestC)) {
            // Check if placing a light violates any adjacent numbered wall
            bool wouldViolate = false;
            for (int d = 0; d < 4; ++d) {
                int nr = bestR + dr[d], nc = bestC + dc[d];
                if (inside(nr, nc) && board[nr][nc] >= 0 && board[nr][nc] <= 4) {
                    if (wallViolated(nr, nc, board[nr][nc])) {
                        wouldViolate = true;
                        break;
                    }
                }
            }
            if (!wouldViolate) {
                board[bestR][bestC] = 5;
                // Also check all numbered walls after placement
                bool anyWallViolated = false;
                for (int r = 0; r < rows && !anyWallViolated; ++r) {
                    for (int c = 0; c < cols && !anyWallViolated; ++c) {
                        if (board[r][c] >= 0 && board[r][c] <= 4) {
                            if (wallViolated(r, c, board[r][c])) {
                                anyWallViolated = true;
                            }
                        }
                    }
                }
                if (!anyWallViolated && backtrack()) return true;
                board[bestR][bestC] = -2;
            }
        }

        // Try leaving it empty (no light here)
        // But if it's unlit and no other option, leaving empty might still work if illuminated later
        // We already know it might be illuminated from elsewhere, so just recurse
        board[bestR][bestC] = -2; // already -2, but explicit
        return backtrack();
    };

    // Pre-check: if any numbered wall already violated in initial board, return empty (though guaranteed solvable)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c] >= 0 && board[r][c] <= 4) {
                if (wallViolated(r, c, board[r][c])) {
                    return std::vector<std::vector<int>>(); // invalid input
                }
            }
        }
    }

    backtrack();
    return board;
}

#include <cassert>
#include <vector>
#include <iostream>

// Test helper: verify a solution using the provided checker logic (simplified)
bool checkSolution(const std::vector<std::vector<int>>& g, const std::vector<std::vector<int>>& ans) {
    int n = g.size(), m = g[0].size();
    if (n != ans.size() || m != ans[0].size()) return false;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] >= -1 && g[i][j] <= 4) {
                if (ans[i][j] != g[i][j]) return false;
            } else {
                if (ans[i][j] >= -1 && ans[i][j] <= 4) return false;
            }
        }
    }
    int ps[4][2] = {-1,0,1,0,0,-1,0,1};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (ans[i][j] >= 0 && ans[i][j] <= 4) {
                int cnt = 0;
                for (int k = 0; k < 4; ++k) {
                    int dx = i+ps[k][0], dy = j+ps[k][1];
                    if (dx>=0 && dx<n && dy>=0 && dy<m && ans[dx][dy]==5) cnt++;
                }
                if (cnt != ans[i][j]) return false;
            } else if (ans[i][j] == 5) {
                for (int k = i+1; k < n; ++k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) return false;
                }
                for (int k = j+1; k < m; ++k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) return false;
                }
            } else if (ans[i][j] == -2) {
                bool lit = false;
                for (int dir = 0; dir < 4; ++dir) {
                    int dx = i+ps[dir][0], dy = j+ps[dir][1];
                    while (dx>=0 && dx<n && dy>=0 && dy<m) {
                        if (ans[dx][dy] >= -1 && ans[dx][dy] <= 4) break;
                        if (ans[dx][dy] == 5) lit = true;
                        dx += ps[dir][0]; dy += ps[dir][1];
                    }
                }
                if (!lit) return false;
            }
        }
    }
    return true;
}

int main() {
    // Test 1: Simple 2x2 with one numbered wall
    std::vector<std::vector<int>> g1 = {{1, -2}, {-2, -2}};
    auto sol1 = solveAkari(g1);
    assert(checkSolution(g1, sol1));

    // Test 2: 3x3 puzzle from known example (walls at corners, etc.)
    std::vector<std::vector<int>> g2 = {
        {-1, -2, -1},
        {-2, 0, -2},
        {-1, -2, -1}
    };
    auto sol2 = solveAkari(g2);
    assert(checkSolution(g2, sol2));

    // Test 3: Single empty cell with no walls? Not solvable, but we skip; test with a wall block
    std::vector<std::vector<int>> g3 = {{0, -2, -2}};
    auto sol3 = solveAkari(g3);
    assert(checkSolution(g3, sol3));

    // Test 4: 4x4 with some constraints
    std::vector<std::vector<int>> g4 = {
        {-1, -2, -2, -1},
        {-2, 1, -2, -2},
        {-2, -2, -2, 2},
        {-1, -2, -2, -1}
    };
    auto sol4 = solveAkari(g4);
    assert(checkSolution(g4, sol4));

    // Test 5: All empty 1x3 with one wall at end? Not solvable, skip. Use small solvable.
    std::vector<std::vector<int>> g5 = {
        {-1, 0, -1}
    };
    auto sol5 = solveAkari(g5);
    assert(checkSolution(g5, sol5));

    // Test 6: 2x3 with wall numbers
    std::vector<std::vector<int>> g6 = {
        {0, -2, 1},
        {-2, -2, -2}
    };
    auto sol6 = solveAkari(g6);
    assert(checkSolution(g6, sol6));

    // Test 7: 5x5 moderately complex
    std::vector<std::vector<int>> g7 = {
        {-1, 1, -2, 0, -1},
        {-2, -2, -2, -2, -2},
        {1, -2, -1, -2, 1},
        {-2, -2, -2, -2, -2},
        {-1, 0, -2, 1, -1}
    };
    auto sol7 = solveAkari(g7);
    assert(checkSolution(g7, sol7));

    // Test 8: 1x1 with no walls? Not solvable because must illuminate, skip.
    // Test 8: 2x2 with two walls and two empties
    std::vector<std::vector<int>> g8 = {
        {-1, -2},
        {-2, -1}
    };
    auto sol8 = solveAkari(g8);
    assert(checkSolution(g8, sol8));

    // Test 9: 3x3 with zero walls
    std::vector<std::vector<int>> g9 = {
        {0, -2, 0},
        {-2, -2, -2},
        {0, -2, 0}
    };
    auto sol9 = solveAkari(g9);
    assert(checkSolution(g9, sol9));

    // Test 10: 4x4 with many numbers
    std::vector<std::vector<int>> g10 = {
        {1, -2, -2, 1},
        {-2, -2, -2, -2},
        {-2, -2, -2, -2},
        {1, -2, -2, 1}
    };
    auto sol10 = solveAkari(g10);
    assert(checkSolution(g10, sol10));

    std::cout << "All tests passed!\n";
    return 0;
}

// The problem is a classic constraint satisfaction problem solved with backtracking. Represent the board as a 2D vector. For each empty cell (`-2`), we try placing a light (`5`) or leaving it empty. To reduce branching, use constraint propagation: whenever we place a light, we mark all cells in the four orthogonal directions as "illuminated" until hitting a wall. Also, for numbered walls, we can compute how many lights are already adjacent; if it equals the number, we mark the remaining adjacent empty cells as "cannot place light" (i.e., they must remain empty). If a numbered wall has too few possible adjacent cells to meet its requirement, backtrack. Additionally, a common optimization: if a white cell is not illuminated and all its possible light positions are blocked, it must get a light from some direction; we can use heuristics like "forced moves" but a simple loop over cells is sufficient for small boards. For each empty cell, before placing a light, verify that placing a light does not cause two lights to shine on each other (check same row/column until wall) and that it doesn't violate any adjacent numbered wall's max count. After placing a light, recursively solve the rest. The base case is when all cells have been decided; then verify all constraints globally (lights shine, numbers satisfied, every white cell illuminated). Since we place lights incrementally, we can prune early. Time complexity is exponential in the worst case but acceptable for 10×10 with good pruning; space complexity is O(NM) for the board and recursion depth O(NM). Edge cases: walls with numbers 0 require no adjacent lights, so we must ensure we don't place lights next to them; cells at borders; ensuring illumination check stops at walls. We also need to handle the case where a cell is already illuminated but has no light; we can leave it as `-2` but it must be illuminated. The backtracking will find a valid assignment.

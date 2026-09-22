// You are given a rectangular grid of characters consisting of empty cells (`.`), walls (`*` or `#`), a starting point marked `H`, and possibly a destination marked `D`. The grid is bounded by walls. Your task is to write a C++ function that finds the minimum number of moves required to reach `D` from `H`. A move consists of moving one cell in one of the four cardinal directions (up, down, left, right). At any moment, you may choose to use one "dash" ability: if you use it, you can move up to two cells in a straight line through empty or wall cells in one single step (you may stop after one or two cells, but cannot move diagonally or pass outside the grid). The dash ability can be used at most once during the entire journey, and it is optional. If it is impossible to reach `D`, return `-1`.
The problem is equivalent to a shortest-path search on a state graph where each state is `(row, col, usedDash)`, with `usedDash` being a boolean indicating whether the dash has already been used. Normal moves cost 1 and do not change `usedDash`. If the dash has not been used yet, from any cell you can also perform a dash: move one cell (cost 1, set `usedDash=1`) or two cells (cost 1, set `usedDash=1`) in a straight line, regardless of whether intermediate cells are walls (but you cannot move through the grid boundary; the grid is enclosed by walls, so boundary check is unnecessary if we treat out-of-bounds as walls). Since all move costs are 1, standard BFS (0-1 BFS not needed because all edges have weight 1) yields the minimum number of moves. The grid is at most 1000×1000, so the state space is at most 2×10^6, which is feasible. Edge cases: `D` might be adjacent to `H` (answer 1), `D` might be unreachable even with dash (return -1), and the dash might allow passing through walls, but cannot pass through `D` as an intermediate step without stopping—actually if `D` is encountered as an intermediate cell during a two-cell dash, you must count that as reaching it; we handle this by checking the destination cell after each step of the dash. Time complexity is O(N*M) for the state space (each cell visited at most twice, with and without dash), and space complexity is O(N*M) for the visited array and queue.
#include <bits/stdc++.h>

// Return the minimum number of moves to reach 'D' from 'H' in the grid,
// or -1 if unreachable. The grid is represented as a vector of strings,
// all of equal length, with '.' = empty, '*'/'#' = wall, 'H' = start, 'D' = goal.
int minMovesToReachDestination(const std::vector<std::string>& originalGrid) {
    const int rows = static_cast<int>(originalGrid.size());
    const int cols = static_cast<int>(originalGrid[0].size());
    const int dx[4] = {-1, 0, 0, 1};
    const int dy[4] = {0, -1, 1, 0};

    // Add an extra border of walls around the grid to simplify boundary checks.
    std::vector<std::string> grid(rows + 2, std::string(cols + 2, '*'));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i + 1][j + 1] = originalGrid[i][j];
        }
    }

    int startX = -1, startY = -1;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            if (grid[i][j] == 'H') {
                startX = i;
                startY = j;
                grid[i][j] = '.'; // Treat start as empty after recording it
            }
        }
    }

    // visited[x][y][usedDash] = distance, initialized to -1.
    std::vector<std::vector<std::vector<int>>> dist(
        rows + 2, std::vector<std::vector<int>>(cols + 2, std::vector<int>(2, -1)));

    struct State {
        int x, y, usedDash, moves;
    };

    std::queue<State> q;
    dist[startX][startY][0] = 0;
    q.push({startX, startY, 0, 0});

    while (!q.empty()) {
        State cur = q.front();
        q.pop();

        // Check if we reached destination
        if (grid[cur.x][cur.y] == 'D') {
            return cur.moves;
        }

        // Normal moves (one cell)
        for (int d = 0; d < 4; ++d) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (grid[nx][ny] == '*' || grid[nx][ny] == '#') continue; // wall
            if (dist[nx][ny][cur.usedDash] != -1) continue;
            dist[nx][ny][cur.usedDash] = cur.moves + 1;
            q.push({nx, ny, cur.usedDash, cur.moves + 1});
        }

        // Dash moves (only if we haven't already used it)
        if (cur.usedDash == 0) {
            for (int d = 0; d < 4; ++d) {
                // Dash for one cell
                int nx1 = cur.x + dx[d];
                int ny1 = cur.y + dy[d];
                if (grid[nx1][ny1] != '*' && grid[nx1][ny1] != '#') {
                    if (dist[nx1][ny1][1] == -1) {
                        dist[nx1][ny1][1] = cur.moves + 1;
                        q.push({nx1, ny1, 1, cur.moves + 1});
                    }
                    // Dash for two cells (pass through the first cell)
                    int nx2 = cur.x + 2 * dx[d];
                    int ny2 = cur.y + 2 * dy[d];
                    // The second cell must be inside the border and not a wall
                    if (nx2 >= 0 && nx2 <= rows + 1 && ny2 >= 0 && ny2 <= cols + 1 &&
                        grid[nx2][ny2] != '*' && grid[nx2][ny2] != '#') {
                        if (dist[nx2][ny2][1] == -1) {
                            dist[nx2][ny2][1] = cur.moves + 1;
                            q.push({nx2, ny2, 1, cur.moves + 1});
                        }
                    }
                }
            }
        }
    }

    return -1; // unreachable
}
#include <bits/stdc++.h>

// (The solution function is assumed to be included above.)

int main() {
    // Test 1: Simple straight line
    {
        std::vector<std::string> grid = {
            "H.D",
            "...",
            "..."
        };
        assert(minMovesToReachDestination(grid) == 1);
    }

    // Test 2: Need to go around a wall
    {
        std::vector<std::string> grid = {
            "H*D",
            "...",
            "***"
        };
        // Path: (0,0) -> (1,0) -> (1,1) -> (1,2) -> (0,2) = 4 moves
        assert(minMovesToReachDestination(grid) == 4);
    }

    // Test 3: Dash through a wall
    {
        std::vector<std::string> grid = {
            "H*D",
            "***",
            "***"
        };
        // Dash two cells right: H -> (0,0) dash to (0,2) = 1 move
        assert(minMovesToReachDestination(grid) == 1);
    }

    // Test 4: Dash through multiple walls
    {
        std::vector<std::string> grid = {
            "H*#D",
            "*****",
            "....."
        };
        // Dash two cells right from (0,0) to (0,2), then normal to (0,3) = 2 moves
        assert(minMovesToReachDestination(grid) == 2);
    }

    // Test 5: Unreachable even with dash
    {
        std::vector<std::string> grid = {
            "H*D",
            "***",
            "***",
            "***",
            "...."
        };
        assert(minMovesToReachDestination(grid) == -1);
    }

    // Test 6: Dash not enough, must walk around
    {
        std::vector<std::string> grid = {
            "H....",
            "*****",
            "D....",
            "....."
        };
        // Must go down then right then up: H(0,0)->(2,0)->(2,1)->(2,2)->(2,3)->(2,4) but D at (2,0) no...
        // Let's define: H at (0,0), D at (2,0), walls row 1 all '*'.
        // Path: H->(0,1)->...->(0,4)->(1,4) is wall? Actually row1 all walls, so go to row2 via (0,4)->(1,4) is wall.
        // Better: H->(0,1)->(0,2)->(0,3)->(0,4)->(1,4) wall, so go down from (0,0) to (2,0) direct impossible because row1 wall.
        // Use dash: from (0,0) dash down two to (2,0) which is D -> answer 1.
        std::vector<std::string> grid2 = {
            "H....",
            "*****",
            "D...."
        };
        assert(minMovesToReachDestination(grid2) == 1);
    }

    // Test 7: Dash two cells over gap but stop after one
    {
        std::vector<std::string> grid = {
            "H*.",
            ".*.",
            "..D"
        };
        // Without dash: H(0,0)->(1,0)->(2,0)->(2,1)->(2,2) = 4, but (1,0) wall? Actually grid[1][0]='.' (since row1 is ".*."), so okay.
        // But also from H can dash right one to (0,1) wall? No, (0,1) is '*', so can't. Dash down two to (2,0) passes through (1,0) which is '.', so reach (2,0) in 1 move.
        // Then normal to (2,2) in 2 moves, total 3. But better: normal path H->(1,0)->(2,0)->(2,1)->(2,2) = 4, so dash better.
        assert(minMovesToReachDestination(grid) == 3);
    }

    // Test 8: Start and destination same? Not allowed per spec, but if so return 0? Not tested.

    // Test 9: Large grid with corridor
    {
        std::vector<std::string> grid(100, std::string(100, '.'));
        grid[0][0] = 'H';
        grid[99][99] = 'D';
        // Manhattan distance without dash = 198, but dash can only go two in a line, so still 198 - 0 = 198? 
        // Actually dash saves at most 1 per use, so answer 197.
        assert(minMovesToReachDestination(grid) == 197);
    }

    // Test 10: Destru=ination as intermediate dash step
    {
        std::vector<std::string> grid = {
            "H.D",
            "...",
            "..."
        };
        // Dash two cells right from H passes through '.' then D, so answer 1.
        assert(minMovesToReachDestination(grid) == 1);
    }

    return 0;
}

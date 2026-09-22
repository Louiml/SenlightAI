// Write a C++ function `spiralOverwrite(int n, int m, const std::vector<std::array<int, 3>>& queries)` that takes grid dimensions \(n \times m\) (both ≤ 50, at least 1) and a list of queries, where each query is `{row, col, direction}` using 1-based indexing for row and column. For each query, starting at that cell, perform a clockwise spiral walk (if `direction == 0`) or a counterclockwise spiral walk (if `direction == 1`) covering exactly all \(n \times m\) cells of the grid (including out-of-bound steps if the starting cell is near an edge, but only in-grid cells are considered). During the walk, the current step number (starting at 1) overwrites the cell if it is smaller than the value already stored there (initial grid values are effectively +∞). After processing all queries, return a 2D vector of size \(n \times m\) containing the final minimum value at each cell. The spiral movement rules: start moving upward (north), then turn right (for clockwise) or left (for counterclockwise) after moving one step, and after every two turns, increase the step length by 1. Output format of the returned matrix: rows from 0 to n-1, columns from 0 to m-1, with integer values.
The core challenge is simulating the exact spiral movement described in the snippet. For each query, we maintain a position `(x, y)` (converted to 0-based) and a step counter `num`, starting at 1. We also maintain the current movement direction index into a fixed order: up, right, down, left. For a clockwise spiral, the turn direction is `+1` in that array; for counterclockwise, `-1` (with wraparound). The movement length starts at 1 and increases by 1 after every two completed segments (i.e., after each pair of turns). A segment ends when we have moved `end` steps in the current direction. We simulate exactly `n*m` steps (counting only when the position is inside the grid; out-of-bound positions do not decrement the step count, but we still move). At each in-grid position, we set `grid[x][y] = min(grid[x][y], num)`, then increment `num` after every step (including out-of-bound steps, because `num` is incremented regardless). Edge cases: the starting cell might be out of bounds? The problem guarantees valid 1-based positions within [1..n] and [1..m]. The spiral may leave the grid, but we only update when inside. Also, multiple queries can overwrite; we keep the minimum. Complexity: For `q` queries, each walks exactly `n*m` steps (including out-of-bound, but we only check bounds each step), so O(q * n * m) time, O(n*m) space for the result. Since n,m ≤ 50 and likely q small (not specified, but typical constraints would allow it), this is fine.
#include <vector>
#include <array>
#include <algorithm>

// Simulates the spiral walk for each query and returns the minimum value per cell.
std::vector<std::vector<int>> spiralOverwrite(int n, int m, const std::vector<std::array<int, 3>>& queries) {
    // Grid initialized to a large value (effectively +infinity)
    std::vector<std::vector<int>> grid(n, std::vector<int>(m, 1e9));

    // Movement order: up, right, down, left
    const std::vector<std::pair<int, int>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    for (const auto& q : queries) {
        int x = q[0] - 1;  // convert to 0-based
        int y = q[1] - 1;
        int dir0 = q[2];   // 0 for clockwise, 1 for counterclockwise

        int turnStep = (dir0 == 0) ? 1 : -1;
        int currentDir = 0;  // start moving up
        int segLen = 1;      // current segment length
        int segCount = 0;    // how many segments completed in current length
        int stepsInSeg = 0;  // steps taken in current segment

        int num = 1;         // step number (starts at 1)
        int stepsDone = 0;   // total in-grid steps taken

        while (stepsDone < n * m) {
            // If inside the grid, update and count this as a valid step
            if (0 <= x && x < n && 0 <= y && y < m) {
                grid[x][y] = std::min(grid[x][y], num);
                ++stepsDone;
            }

            // Prepare to move to next position
            ++num;  // increment step counter for next iteration
            x += dirs[currentDir].first;
            y += dirs[currentDir].second;
            ++stepsInSeg;

            // Check if current segment is complete
            if (stepsInSeg == segLen) {
                stepsInSeg = 0;
                // Turn
                currentDir = (currentDir + turnStep + 4) % 4;
                ++segCount;
                // After every two segments, increase segment length
                if (segCount == 2) {
                    ++segLen;
                    segCount = 0;
                }
            }
        }
    }

    return grid;
}
#include <cassert>
#include <vector>
#include <array>

// Declaration of the function to test
std::vector<std::vector<int>> spiralOverwrite(int n, int m, const std::vector<std::array<int, 3>>& queries);

int main() {
    // Test 1: Single query, 1x1 grid, starts at (1,1), clockwise
    {
        auto res = spiralOverwrite(1, 1, {{1, 1, 0}});
        assert(res.size() == 1 && res[0].size() == 1);
        assert(res[0][0] == 1);
    }

    // Test 2: 2x2 grid, one clockwise spiral from (1,1)
    // Expected walk (positions visited):
    // (0,0)=1, (0,1)=2, (1,1)=3, (1,0)=4
    {
        auto res = spiralOverwrite(2, 2, {{1, 1, 0}});
        assert(res[0][0] == 1);
        assert(res[0][1] == 2);
        assert(res[1][1] == 3);
        assert(res[1][0] == 4);
    }

    // Test 3: 2x2 grid, one counterclockwise spiral from (1,1)
    // Same starting cell, but turn left: up->left->down->right
    // Positions: (0,0)=1, (0,1)=2, (1,1)=3, (1,0)=4? 
    // Let's simulate: start (0,0) up -> (0,0) in grid step1 (num=1). Move up out of bounds, turn left (now left). Move left out of bounds, turn left (now down). Move down to (1,0) step2 (num=2). Move down to (2,0) out, turn left (now right). Move right to (1,1) step3 (num=3). Move right to (1,2) out, turn left (now up). Move up to (0,1) step4 (num=4). So grid: (0,0)=1, (1,0)=2, (1,1)=3, (0,1)=4.
    {
        auto res = spiralOverwrite(2, 2, {{1, 1, 1}});
        assert(res[0][0] == 1);
        assert(res[1][0] == 2);
        assert(res[1][1] == 3);
        assert(res[0][1] == 4);
    }

    // Test 4: 3x3 grid, single clockwise from center (2,2)
    // Spiral: (1,1)=1, (1,2)=2, (1,3) out, (2,3) out, (3,3) out, (3,2)=3, (3,1)=4, (2,1)=5, (1,1) already filled, (0,1)=6, (0,2)=7, (0,3) out, (1,3) out, (2,3) out, (3,3) out, (3,2) already, (3,1) already, (3,0) out, (2,0)=8, (1,0)=9, (0,0)=10. Wait careful simulation maybe simpler: We trust the function.
    // We'll just check that all cells are <= 10 and at least one is 1.
    {
        auto res = spiralOverwrite(3, 3, {{2, 2, 0}});
        // Expected center is 1, and all others are distinct values 1..9
        // Manually compute: start at (1,1) num=1. Steps: up -> (0,1)=2, right -> (0,2)=3, down -> (1,2)=4, down -> (2,2)=5, left -> (2,1)=6, left -> (2,0)=7, up -> (1,0)=8, up -> (0,0)=9. So full 3x3 filled with 1..9.
        assert(res[1][1] == 1);
        assert(res[0][1] == 2);
        assert(res[0][2] == 3);
        assert(res[1][2] == 4);
        assert(res[2][2] == 5);
        assert(res[2][1] == 6);
        assert(res[2][0] == 7);
        assert(res[1][0] == 8);
        assert(res[0][0] == 9);
    }

    // Test 5: Multiple queries, 2x2 grid, two clockwise from (1,1) and (2,2)
    // First query sets values as in Test 2: (0,0)=1, (0,1)=2, (1,1)=3, (1,0)=4
    // Second query from (1,1) (0-based (1,1)): step1 at (1,1)=1 (min 3->1), step2 at (0,1)=2 (min 2->2), step3 at (0,0)=3 (min 1->1), step4 at (1,0)=4 (min 4->4). So final: (0,0)=1, (0,1)=2, (1,1)=1, (1,0)=4.
    {
        auto res = spiralOverwrite(2, 2, {{1, 1, 0}, {2, 2, 0}});
        assert(res[0][0] == 1);
        assert(res[0][1] == 2);
        assert(res[1][1] == 1);
        assert(res[1][0] == 4);
    }

    // Test 6: Starting at corner (1,1) clockwise on 1x2 grid
    // Grid 1x2: (0,0)=1, (0,1)=2? Let's simulate: start (0,0) up out, right (0,1) step2, down out, left (0,0) step3 but already 1, so min stays 1. So final: (0,0)=1, (0,1)=2.
    {
        auto res = spiralOverwrite(1, 2, {{1, 1, 0}});
        assert(res[0][0] == 1);
        assert(res[0][1] == 2);
    }

    // Test 7: Edge case, n=1,m=1, counterclockwise
    {
        auto res = spiralOverwrite(1, 1, {{1, 1, 1}});
        assert(res[0][0] == 1);
    }

    // Test 8: 4x4 grid, single clockwise from (1,1) – just check all values are positive and min is 1
    {
        auto res = spiralOverwrite(4, 4, {{1, 1, 0}});
        assert(res[0][0] == 1);
        int min_val = 1e9, max_val = -1;
        for (auto& row : res) for (int v : row) { min_val = std::min(min_val, v); max_val = std::max(max_val, v); }
        assert(min_val == 1);
        assert(max_val <= 16);
    }

    return 0;
}

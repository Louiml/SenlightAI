/*
Write a C++ function `int rottenOranges(vector<vector<int>>& grid)` that simulates the spread of rot in a grid of oranges. Each cell is `0` (empty), `1` (fresh orange), or `2` (rotten orange). In each minute, any fresh orange that is 4-directionally adjacent (up, down, left, right) to a rotten orange becomes rotten. The process continues until no more fresh oranges can be infected. The function must return the total number of minutes required for all fresh oranges to rot, or `-1` if it is impossible (some fresh oranges remain unreachable). The grid can be up to 10x10 in size and will always contain at least one cell. Assume no invalid values; all entries are exactly `0`, `1`, or `2`. Handle the case where there are no fresh oranges initially by returning `0`. The grid is provided as a non-const reference because you are allowed to modify it if needed, but your implementation must not depend on mutating the input; use a separate visited structure as in the source.
*/
#include <vector>
#include <queue>
#include <algorithm>

// Returns the minimum minutes to rot all fresh oranges, or -1 if impossible.
int rottenOranges(std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());

    // visited[v] = true if the cell has been reached by rot (either originally rotten or infected later)
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    // queue holds pairs of (time, coordinate)
    std::queue<std::pair<int, std::pair<int, int>>> q;

    // Seed BFS with all initially rotten oranges.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 2) {
                visited[i][j] = true;
                q.push({0, {i, j}});
            }
        }
    }

    int maxTime = 0;
    const int dr[4] = {0, 1, 0, -1};
    const int dc[4] = {1, 0, -1, 0};

    // BFS to simulate rot spreading level by level.
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        int time = current.first;
        int r = current.second.first;
        int c = current.second.second;

        maxTime = std::max(maxTime, time);

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            // Check bounds, not visited, and fresh orange.
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && !visited[nr][nc] && grid[nr][nc] == 1) {
                visited[nr][nc] = true;
                q.push({time + 1, {nr, nc}});
            }
        }
    }

    // Check if any fresh orange remains unvisited.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1 && !visited[i][j]) {
                return -1;
            }
        }
    }

    return maxTime;
}
#include <cassert>
#include <vector>

// The function signature matches the task.
int rottenOranges(std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Simple case, all rot in 2 minutes.
    std::vector<std::vector<int>> grid1 = {
        {2, 1, 1},
        {1, 1, 0},
        {0, 1, 1}
    };
    assert(rottenOranges(grid1) == 4);

    // Test 2: No fresh oranges, answer 0.
    std::vector<std::vector<int>> grid2 = {
        {0, 2, 0},
        {0, 0, 2}
    };
    assert(rottenOranges(grid2) == 0);

    // Test 3: Impossible because an isolated fresh orange.
    std::vector<std::vector<int>> grid3 = {
        {2, 1, 0},
        {1, 0, 1},
        {0, 1, 1}
    };
    assert(rottenOranges(grid3) == -1);

    // Test 4: Already all rotten, answer 0.
    std::vector<std::vector<int>> grid4 = {
        {2, 2},
        {2, 2}
    };
    assert(rottenOranges(grid4) == 0);

    // Test 5: Single fresh orange adjacent to one rotten, takes 1 minute.
    std::vector<std::vector<int>> grid5 = {
        {2, 1}
    };
    assert(rottenOranges(grid5) == 1);

    // Test 6: Single fresh orange with no rotten, impossible.
    std::vector<std::vector<int>> grid6 = {
        {1}
    };
    assert(rottenOranges(grid6) == -1);

    // Test 7: Long line of fresh oranges, takes 4 minutes.
    std::vector<std::vector<int>> grid7 = {
        {2, 1, 1, 1, 1}
    };
    assert(rottenOranges(grid7) == 4);

    // Test 8: Empty cells block spread, impossible.
    std::vector<std::vector<int>> grid8 = {
        {2, 0, 1},
        {0, 0, 0},
        {1, 0, 2}
    };
    assert(rottenOranges(grid8) == -1);

    // Test 9: Multiple sources spread quickly.
    std::vector<std::vector<int>> grid9 = {
        {2, 1, 1, 2},
        {1, 1, 1, 1},
        {2, 1, 1, 2}
    };
    assert(rottenOranges(grid9) == 1);

    // Test 10: Single rotten and one fresh not adjacent (diagonal) – impossible.
    std::vector<std::vector<int>> grid10 = {
        {2, 0, 1},
        {0, 0, 0},
        {0, 0, 0}
    };
    assert(rottenOranges(grid10) == -1);

    return 0;
}
// The solution uses a multi-source BFS starting from all initially rotten oranges simultaneously. Each rotten orange is pushed into a queue with a time stamp of `0`. During BFS, when a rotten orange is popped, we record its time as the current maximum. Then we explore its four neighbors; any fresh orange (`grid[nr][nc] == 1`) that is not yet visited becomes rotten and is pushed with time `+1`. The visited array prevents reprocessing and also marks every cell that has been reached by rot. After BFS completes, we scan the grid for any fresh orange that was never visited; if any exist, return `-1`. Otherwise, return the maximum time recorded (which is the last minute anything rotted). Key edge cases: grid with no fresh oranges (answer is `0` because BFS processes only rotten oranges, max time stays `0`, and the final scan finds no fresh oranges); grid with isolated fresh oranges not adjacent to any rot (BFS never visits them, so final scan returns `-1`); multiple rotten sources (BFS handles them together in level order, giving correct minimal times). Time complexity is O(n*m) because each cell is pushed and popped at most once. Space complexity is O(n*m) for the queue and visited matrix.

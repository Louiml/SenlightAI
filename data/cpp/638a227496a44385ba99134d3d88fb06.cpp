/*
You are given an R × C grid where each cell is one of three characters: `'.'` (open water), `'X'` (ice block), or `'L'` (a swan's location). There are exactly two `'L'` cells, representing the two swans. Initially, all `'.'` and `'L'` cells are water, and all `'X'` cells are ice. Each day, all ice cells that are adjacent (up, down, left, right) to any current water cell melt and become water. After each day of melting, the swans are free to move through water and `'L'` cells (but not through ice) in any number of steps. Write a C++ function `int daysUntilSwanMeet(int R, int C, const std::vector<std::string>& grid)` that returns the minimum number of days required for the two swans to reach each other. During a day, the swans can move only after the ice melts for that day; the process repeats daily until they can meet. It is guaranteed that the grid is valid and that at least one swan can eventually reach the other.
*/
#include <vector>
#include <queue>
#include <string>
#include <utility>

// Returns the minimum number of days until the two swans can meet.
int daysUntilSwanMeet(int R, int C, const std::vector<std::string>& grid) {
    // Direction vectors: up, right, down, left
    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};

    std::vector<std::vector<char>> graph(R, std::vector<char>(C));
    std::vector<std::vector<bool>> waterVisited(R, std::vector<bool>(C, false));
    std::vector<std::vector<bool>> swanVisited(R, std::vector<bool>(C, false));

    std::queue<std::pair<int,int>> waterQ, waterTempQ;
    std::queue<std::pair<int,int>> swanQ, swanTempQ;

    int swanStartR = -1, swanStartC = -1;

    // Initialize graph and queues
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            graph[i][j] = grid[i][j];
            if (grid[i][j] == 'L') {
                if (swanStartR == -1) {
                    swanStartR = i;
                    swanStartC = j;
                }
                // Water cells include swan locations
                waterVisited[i][j] = true;
                waterQ.push({i, j});
            } else if (grid[i][j] == '.') {
                waterVisited[i][j] = true;
                waterQ.push({i, j});
            }
        }
    }

    // Start BFS from the first swan
    swanQ.push({swanStartR, swanStartC});
    swanVisited[swanStartR][swanStartC] = true;

    int day = 0;

    while (true) {
        // Try to move the swan today
        bool met = false;
        while (!swanQ.empty()) {
            auto [cr, cc] = swanQ.front();
            swanQ.pop();

            for (int d = 0; d < 4; ++d) {
                int nr = cr + dr[d];
                int nc = cc + dc[d];

                if (nr < 0 || nr >= R || nc < 0 || nc >= C || swanVisited[nr][nc]) continue;

                swanVisited[nr][nc] = true;

                if (graph[nr][nc] == '.') {
                    swanQ.push({nr, nc});
                } else if (graph[nr][nc] == 'X') {
                    swanTempQ.push({nr, nc}); // will be reachable after melting
                } else if (graph[nr][nc] == 'L') {
                    return day; // met the other swan
                }
            }
        }

        if (met) return day;

        // Melting phase: expand water to adjacent ice
        while (!waterQ.empty()) {
            auto [cr, cc] = waterQ.front();
            waterQ.pop();

            for (int d = 0; d < 4; ++d) {
                int nr = cr + dr[d];
                int nc = cc + dc[d];

                if (nr < 0 || nr >= R || nc < 0 || nc >= C || waterVisited[nr][nc]) continue;

                if (graph[nr][nc] == 'X') {
                    waterVisited[nr][nc] = true;
                    waterTempQ.push({nr, nc});
                    graph[nr][nc] = '.';
                }
            }
        }

        // Swap queues for next day
        swanQ = swanTempQ;
        waterQ = waterTempQ;
        swanTempQ = std::queue<std::pair<int,int>>();
        waterTempQ = std::queue<std::pair<int,int>>();
        ++day;
    }

    // Should never reach here
    return -1;
}
#include <cassert>
#include <vector>
#include <string>

// Declare the function to test (assumes the solution is provided before this main)
int daysUntilSwanMeet(int R, int C, const std::vector<std::string>& grid);

int main() {
    // Case 1: Adjacent swans, day 0
    {
        std::vector<std::string> grid = {"L.", ".L"};
        assert(daysUntilSwanMeet(2, 2, grid) == 0);
    }

    // Case 2: One ice block between swans
    {
        std::vector<std::string> grid = {"L.X", "...", "..L"};
        assert(daysUntilSwanMeet(3, 3, grid) == 1);
    }

    // Case 3: Swans separated by a 2x2 block of ice
    {
        std::vector<std::string> grid = {"LXX", "XXX", "XXL"};
        assert(daysUntilSwanMeet(3, 3, grid) == 2);
    }

    // Case 4: Swans far apart, all water
    {
        std::vector<std::string> grid = {"L....", ".....", "....L"};
        assert(daysUntilSwanMeet(3, 5, grid) == 0);
    }

    // Case 5: Multiple layers of ice
    {
        std::vector<std::string> grid = {"LXXXX", "XXXXX", "XXXXL"};
        assert(daysUntilSwanMeet(3, 5, grid) == 2);
    }

    // Case 6: Single row with ice between
    {
        std::vector<std::string> grid = {"L.X.L"};
        assert(daysUntilSwanMeet(1, 5, grid) == 1);
    }

    // Case 7: Swans at corners, large open water
    {
        std::vector<std::string> grid = {"L....", ".....", ".....", "....L"};
        assert(daysUntilSwanMeet(4, 5, grid) == 0);
    }

    // Case 8: Snake-like path with ice
    {
        std::vector<std::string> grid = {"L.X..", ".X.X.", ".X.X.", "..X.L"};
        assert(daysUntilSwanMeet(4, 5, grid) == 0); // path exists
    }
    
    // Case 9: Fully blocked except one corridor
    {
        std::vector<std::string> grid = {"LXX", "X.X", "XXL"};
        assert(daysUntilSwanMeet(3, 3, grid) == 1);
    }

    // Case 10: Larger grid requiring 3 days
    {
        std::vector<std::string> grid = {
            "LXXXX",
            "XXXXX",
            "XXXXX",
            "XXXXX",
            "XXXXL"
        };
        assert(daysUntilSwanMeet(5, 5, grid) == 2);
    }

    return 0;
}
// We simulate the process day by day using two BFS queues: one for water spreading (ice melting) and one for swan movement. First, initialize a water queue with all initial water and swan cells (marking them visited), and initialize a swan queue with the first swan's location (marking visited_swan). Each day, we first attempt to move the swan: we pop from the swan queue and explore neighbors; if a neighbor is water or a swan cell, we add it to the current swan queue; if it’s ice, we add it to a temporary swan queue (to be explored next day); if it’s the other swan, we return the current day count. If the swan cannot meet yet, we let the water melt: pop from the water queue and for each neighbor that is ice, turn it to water and add it to a temporary water queue. After melting, we replace the active water and swan queues with their temporary equivalents and increment the day counter. The algorithm terminates when the swans meet. Edge cases: the swans might be adjacent from the start (day 0); the grid may have only one ice cell; the swans may be blocked but melting proceeds until they connect. Time complexity: each cell is processed at most once in each BFS (water and swan), so O(R*C) per phase, and each day processes only the frontier cells, leading to O(R*C) total over all days. Space complexity: O(R*C) for queues and visited arrays.

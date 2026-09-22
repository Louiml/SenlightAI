// Write a C++ function `std::vector<std::string> generateMarkedGrid(const std::vector<std::vector<int>>& grid)` that takes a 2D grid of integers (with possible negative values representing obstacles, zero for empty cells, and positive integers representing sources with a "strength" value). The function must return a vector of strings of the same dimensions, where each cell is marked as `'Y'` if it is reachable from at least one source within its strength distance, `'N'` if it is empty and not reachable, and `'B'` if it is an obstacle. Reachability is defined via 4-directional movement (up/down/left/right), and from a source at `(r,c)` with strength `s`, all cells within Manhattan distance `≤ s` are reachable (movement through obstacles is blocked). If multiple sources, their effects combine. The grid dimensions are at least 1×1 and at most 1000×1000. The function should handle up to 10 test cases in a single call? No—the function handles one grid per call. Implement an efficient solution using a priority queue (max-heap) to simulate a multi-source wavefront, processing cells in decreasing order of remaining reachable distance. Ensure the function is `const`-correct and uses no global state.
#include <bits/stdc++.h>
using namespace std;

// Assume generateMarkedGrid definition above

int main() {
    // Test 1: simple single source
    vector<vector<int>> g1 = {{0, 2, 0}, {0, 0, 0}, {0, 0, 0}};
    vector<string> r1 = generateMarkedGrid(g1);
    assert(r1[0][0] == 'N');
    assert(r1[0][1] == 'Y');
    assert(r1[0][2] == 'N');
    assert(r1[1][0] == 'N');
    assert(r1[1][1] == 'Y');
    assert(r1[1][2] == 'N');
    assert(r1[2][0] == 'N');
    assert(r1[2][1] == 'N');
    assert(r1[2][2] == 'N');

    // Test 2: obstacle blocks
    vector<vector<int>> g2 = {{0, 0, 2}, {0, -1, 0}, {0, 0, 0}};
    vector<string> r2 = generateMarkedGrid(g2);
    assert(r2[0][0] == 'N');
    assert(r2[0][1] == 'Y');
    assert(r2[0][2] == 'Y');
    assert(r2[1][0] == 'N');
    assert(r2[1][1] == 'B');
    assert(r2[1][2] == 'Y');
    assert(r2[2][0] == 'N');
    assert(r2[2][1] == 'N');
    assert(r2[2][2] == 'Y');

    // Test 3: multiple sources overlap
    vector<vector<int>> g3 = {{1, 0, 0}, {0, 0, 1}};
    vector<string> r3 = generateMarkedGrid(g3);
    assert(r3[0][0] == 'Y');
    assert(r3[0][1] == 'Y');
    assert(r3[0][2] == 'N');
    assert(r3[1][0] == 'Y');
    assert(r3[1][1] == 'Y');
    assert(r3[1][2] == 'Y');

    // Test 4: all obstacles
    vector<vector<int>> g4 = {{-1, -1}, {-1, -1}};
    vector<string> r4 = generateMarkedGrid(g4);
    assert(r4[0][0] == 'B');
    assert(r4[0][1] == 'B');
    assert(r4[1][0] == 'B');
    assert(r4[1][1] == 'B');

    // Test 5: single cell empty
    vector<vector<int>> g5 = {{0}};
    vector<string> r5 = generateMarkedGrid(g5);
    assert(r5[0][0] == 'N');

    // Test 6: single cell source
    vector<vector<int>> g6 = {{5}};
    vector<string> r6 = generateMarkedGrid(g6);
    assert(r6[0][0] == 'Y');

    // Test 7: diagonal not reachable
    vector<vector<int>> g7 = {{1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    vector<string> r7 = generateMarkedGrid(g7);
    assert(r7[2][2] == 'N');

    // Test 8: large strength covers all non-obstacle
    vector<vector<int>> g8 = {{0, 0, 0}, {0, 5, 0}, {0, 0, 0}};
    vector<string> r8 = generateMarkedGrid(g8);
    for (const auto& row : r8)
        for (char ch : row)
            assert(ch == 'Y');

    cout << "All tests passed!\n";
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Returns a grid marked with 'B' for obstacles, 'Y' for reachable, 'N' for unreachable.
vector<string> generateMarkedGrid(const vector<vector<int>>& gridIn) {
    const int n = (int)gridIn.size();
    const int m = (int)gridIn[0].size();

    // Metadata per cell
    vector<vector<int>> value(n, vector<int>(m, 0));
    vector<vector<int>> depth(n, vector<int>(m, -1)); // -1 means unset
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<bool>> valid(n, vector<bool>(m, false));

    // Max-heap: (remaining distance, row, col)
    priority_queue<pair<int, pair<int,int>>> pq;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            value[i][j] = gridIn[i][j];
            if (gridIn[i][j] > 0) {
                depth[i][j] = gridIn[i][j];
                valid[i][j] = true;
                pq.push({depth[i][j], {i,j}});
            }
        }
    }

    // Directions: up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        int curDepth = top.first;
        int r = top.second.first;
        int c = top.second.second;

        if (visited[r][c]) continue;
        visited[r][c] = true;
        valid[r][c] = true;

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && !visited[nr][nc] && value[nr][nc] != -1) {
                int newDepth = curDepth - 1;
                if (newDepth > depth[nr][nc]) {
                    depth[nr][nc] = newDepth;
                    valid[nr][nc] = true;
                    if (newDepth > 0) {
                        pq.push({newDepth, {nr, nc}});
                    }
                }
            }
        }
    }

    vector<string> result(n, string(m, 'N'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (value[i][j] == -1) result[i][j] = 'B';
            else if (valid[i][j]) result[i][j] = 'Y';
            else result[i][j] = 'N';
        }
    }
    return result;
}
// The solution models each positive cell as a source with an initial "remaining reachable distance" equal to its value. We use a max-heap priority queue storing pairs `(remaining distance, cell coordinates)`. We initialize the queue with all sources, set their `depth` to their strength, and mark them as valid. Then we pop the cell with the largest remaining distance. If already visited, skip; otherwise mark it visited and valid. For each of its 4 orthogonal neighbors (not diagonals), if the neighbor is not an obstacle and not yet visited, we compute the potential new remaining distance as `current_remaining - 1`. If this new distance is greater than the neighbor's current recorded depth (or if neighbor has never been set), we update the neighbor's depth and push it onto the queue. Also, if the new distance is exactly 0, we still mark the neighbor as valid but do not push it (since no further propagation needed). This ensures that any cell that is within reachable range (including exactly at the boundary) gets marked `'Y'`. Since the heap processes larger distances first, we guarantee that each cell is processed in optimal order, and once visited we do not revisit, so total time is O(NM log(NM)) where N,M are grid dimensions, due to each cell being pushed/popped at most once. Space complexity is O(NM) for storing grid metadata, visited flags, and the heap. Edge cases include cells that have zero strength, single-cell grids, obstacles at sources (but sources are positive so not obstacles), and multiple overlapping sources where the stronger one dominates.

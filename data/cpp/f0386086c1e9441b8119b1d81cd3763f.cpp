Write a C++ function `int minimumStepsToEscape(vector<vector<int>>& grid, int k)` that computes the minimum number of moves required for a person to travel from the top-left cell `(0,0)` to the bottom-right cell `(m-1, n-1)` of an `m x n` binary grid, where `0` represents an open cell and `1` represents a wall. In one move, the person can move up, down, left, or right by one cell. The person can remove (pass through) at most `k` walls in total. If reaching the destination is impossible, the function must return `-1`. The grid dimensions will be between 1 and 40 in each direction, and `k` will be non-negative. The function must handle cases where the start and destination are the same (return 0), where walls are abundant, and where multiple paths exist with different wall-removal needs.

#include <cassert>
#include <vector>
using namespace std;

int main() {
    // Test 1: Simple open grid
    vector<vector<int>> grid1 = {{0,0,0},{0,0,0},{0,0,0}};
    assert(minimumStepsToEscape(grid1, 0) == 4);

    // Test 2: Wall blocking but enough k
    vector<vector<int>> grid2 = {{0,1,0},{1,1,0},{0,0,0}};
    assert(minimumStepsToEscape(grid2, 2) == 4);

    // Test 3: Not enough k to break walls
    vector<vector<int>> grid3 = {{0,1,0},{1,1,0},{0,0,0}};
    assert(minimumStepsToEscape(grid3, 1) == -1);

    // Test 4: Single cell
    vector<vector<int>> grid4 = {{0}};
    assert(minimumStepsToEscape(grid4, 5) == 0);

    // Test 5: Need to break exactly k walls
    vector<vector<int>> grid5 = {{0,1},{1,0}};
    assert(minimumStepsToEscape(grid5, 1) == 2);

    // Test 6: k larger than walls, same as open
    vector<vector<int>> grid6 = {{0,1},{1,0}};
    assert(minimumStepsToEscape(grid6, 10) == 2);

    // Test 7: Start blocked but k allows
    vector<vector<int>> grid7 = {{1,0},{0,0}};
    assert(minimumStepsToEscape(grid7, 1) == 2);

    // Test 8: Impossible even with huge k (no path because of shape? Actually with enough k always possible on connected grid)
    // But test with k=0 and walls forming a barrier
    vector<vector<int>> grid8 = {{0,1,0},{0,1,0},{0,1,0}};
    assert(minimumStepsToEscape(grid8, 0) == -1);
    assert(minimumStepsToEscape(grid8, 1) == 4);

    return 0;
}

#include <vector>
#include <queue>
#include <climits>
#include <cstring>

using namespace std;

// Returns the minimum steps to go from (0,0) to (m-1,n-1)
// in a grid where 1 is a wall, at most k walls can be broken.
int minimumStepsToEscape(vector<vector<int>>& grid, int k) {
    int m = grid.size();
    int n = grid[0].size();
    
    // Early exit for single-cell grid
    if (m == 1 && n == 1) return 0;
    
    // visited[i][j] stores the maximum remaining walls we had when first reaching (i,j)
    // We initialize to -1 to denote unvisited.
    int visited[40][40];
    memset(visited, -1, sizeof(visited));
    
    // Queue stores (remaining_walls, (x, y))
    queue<pair<int, pair<int, int>>> q;
    q.push({k, {0, 0}});
    visited[0][0] = k;
    
    // Directions: right, down, up, left
    int dx[4] = {0, 1, -1, 0};
    int dy[4] = {1, 0, 0, -1};
    
    int steps = 0;
    
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            auto current = q.front();
            q.pop();
            int remaining = current.first;
            int x = current.second.first;
            int y = current.second.second;
            
            // Since visited[x][y] is the max remaining seen, if current remaining <= visited[x][y],
            // this state is redundant and we skip it. But we have already updated visited at push time,
            // so this check ensures we don't process a state that was superseded.
            if (visited[x][y] != -1 && visited[x][y] > remaining) {
                continue;
            }
            
            for (int dir = 0; dir < 4; ++dir) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                
                if (nx < 0 || ny < 0 || nx >= m || ny >= n) continue;
                
                // If we reached the destination
                if (nx == m - 1 && ny == n - 1) {
                    return steps + 1;
                }
                
                // Calculate remaining walls after moving
                int newRemaining = remaining;
                if (grid[nx][ny] == 1) {
                    if (remaining == 0) continue; // cannot break a wall
                    newRemaining = remaining - 1;
                }
                
                // Only push if we have a better remaining value for this cell
                if (visited[nx][ny] < newRemaining) {
                    visited[nx][ny] = newRemaining;
                    q.push({newRemaining, {nx, ny}});
                }
            }
        }
        steps++;
    }
    
    return -1;
}

// The problem is a shortest-path search on a grid where the state includes the current cell and the number of walls still available to break. We use BFS because all moves have equal weight (1 step). Instead of storing visited cells as simple booleans, we store the maximum remaining walls we had when visiting each cell. This is because if we revisit a cell with fewer or equal remaining walls than a previous visit, the new state is strictly suboptimal—it cannot lead to a better answer, since we have fewer resources and have taken at least as many steps. Thus, we only push a new state when the current remaining walls exceed the best recorded for that cell. We process the grid level by level, incrementing the move counter each BFS layer. When we reach the destination, we return the current move count. If the BFS queue empties without reaching the destination, we return `-1`. Important edge cases include a 1x1 grid (returns 0 immediately), walls that require using all `k`, and cases where `k` is larger than the number of walls, making pathfinding trivial. The algorithm runs in O(m * n * k) time in the worst case because each cell can be processed up to k+1 times (once for each distinct remaining-walls value), and uses O(m * n) space for the visited array plus the queue size, which is also O(m * n * k) in the worst case.

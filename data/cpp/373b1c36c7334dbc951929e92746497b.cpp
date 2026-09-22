Given a rectangular city grid represented as a vector of strings, where each character is either `'.'` (an empty cell) or `'#'` (an obstacle), write a C++ function `countHamiltonianPaths` that returns the number of distinct paths that start on any border empty cell (top row, bottom row, left column, or right column), visit every empty cell exactly once (including the starting cell), and end on any border empty cell. Obstacles must never be visited. The path can move only up, down, left, or right to adjacent cells. Since the grid can be up to 4×4 (so up to 16 empty cells), the result may exceed 32-bit integers, so return a `long long`. If no such path exists, return 0.
// The problem is a Hamiltonian path counting problem on a grid with obstacles. A straightforward recursive backtracking would be exponential in the number of empty cells, but with at most 16 empty cells, we can use a bitmask dynamic programming approach. Let `H` and `W` be grid dimensions, and `N = H*W` be the total cell count. For each cell, we assign a bit index `idx = y*W + x`. We maintain a DP table `dp[mask][cell]` where `mask` is a bitmask of visited cells (including obstacles set to 1 initially so they are never visited), and `cell` is the index of the last visited cell. The value stores the number of paths that visit exactly the cells in `mask` and end at `cell`. We initialize by setting `dp[mask_obstacles | (1<<start)][start] = 1` for every border empty cell. Then we iterate masks in increasing order. For each state, we try to move to an adjacent unvisited empty cell, adding the count to the new state. At the end, we sum `dp[all_cells_mask][end]` for all border empty cells `end`. Obstacles are pre-set in the mask so transitions never go into them. The answer is the sum over all border endings. Time complexity is O(2^N * N) for states times up to 4 transitions, which is at most 2^16 * 16 * 4 ≈ 4 million operations. Space complexity is O(2^N * N) for the DP table, which for N=16 is 16 * 65536 * 8 bytes ≈ 8 MB. Edge cases: no empty cells ⇒ return 0; a single empty cell at border ⇒ count 1; obstacles making the graph disconnected ⇒ DP naturally handles it; grids larger than 4×4 are not part of the constraints, but the algorithm generalizes as long as 2^N fits in memory.
#include <bits/stdc++.h>
using namespace std;

// Count Hamiltonian paths on a grid with obstacles, starting and ending on border empty cells.
// city: vector of strings, each char '.' empty or '#' obstacle.
// Returns the number of distinct paths as a long long.
long long countHamiltonianPaths(const vector<string>& city) {
    int H = (int)city.size();
    if (H == 0) return 0;
    int W = (int)city[0].size();
    int N = H * W;
    if (N > 20) return 0; // safety check for memory

    // dp[mask][pos] = number of ways to visit exactly cells in mask, ending at pos.
    // We use a 2D vector of long long to avoid huge stack.
    vector<vector<long long>> dp(1 << N, vector<long long>(N, 0));
    
    // Build initial mask with obstacles set to visited (so we never step on them).
    int obstacleMask = 0;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (city[y][x] == '#') {
                obstacleMask |= (1 << (y * W + x));
            }
        }
    }
    
    // Initialize: start from any border empty cell.
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (city[y][x] == '.') {
                int idx = y * W + x;
                // Check if border
                if (y == 0 || y == H - 1 || x == 0 || x == W - 1) {
                    int mask = obstacleMask | (1 << idx);
                    dp[mask][idx] = 1;
                }
            }
        }
    }
    
    // The final mask should have all empty cells visited. Compute it.
    int allEmptyMask = obstacleMask;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (city[y][x] == '.') {
                allEmptyMask |= (1 << (y * W + x));
            }
        }
    }
    
    // DP transitions
    for (int mask = 0; mask < (1 << N); ++mask) {
        for (int pos = 0; pos < N; ++pos) {
            if (dp[mask][pos] == 0) continue;
            int y = pos / W;
            int x = pos % W;
            // Directions: up, down, left, right
            const int dy[4] = {-1, 1, 0, 0};
            const int dx[4] = {0, 0, -1, 1};
            for (int d = 0; d < 4; ++d) {
                int ny = y + dy[d];
                int nx = x + dx[d];
                if (ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
                int nidx = ny * W + nx;
                // Check if cell is empty and not visited yet
                if (city[ny][nx] != '.') continue;
                if (mask & (1 << nidx)) continue; // already visited
                int nmask = mask | (1 << nidx);
                dp[nmask][nidx] += dp[mask][pos];
            }
        }
    }
    
    // Sum all paths that end on any border cell and have visited all empty cells.
    long long result = 0;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (city[y][x] == '.') {
                int idx = y * W + x;
                if (y == 0 || y == H - 1 || x == 0 || x == W - 1) {
                    result += dp[allEmptyMask][idx];
                }
            }
        }
    }
    return result;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Include or paste the solution function above here.

int main() {
    // Test case 0 from original snippet
    vector<string> city0 = {"....", ".##.", "...."};
    assert(countHamiltonianPaths(city0) == 20LL);
    
    // Test case 1
    vector<string> city1 = {"....", ".###", "...."};
    assert(countHamiltonianPaths(city1) == 2LL);
    
    // Test case 2
    vector<string> city2 = {"....", "####", "...."};
    assert(countHamiltonianPaths(city2) == 0LL);
    
    // Test case 3
    vector<string> city3 = {"....", "....", "...."};
    assert(countHamiltonianPaths(city3) == 80LL);
    
    // Single cell empty on border
    vector<string> city4 = {"."};
    assert(countHamiltonianPaths(city4) == 1LL);
    
    // Single cell obstacle
    vector<string> city5 = {"#"};
    assert(countHamiltonianPaths(city5) == 0LL);
    
    // 2x2 all empty: Hamiltonian paths starting and ending on any border cell.
    // All cells are border. Count all Hamiltonian paths (cycles or paths).
    // For 2x2, there are 8 Hamiltonian paths: each cell can start, then visit the other three in a path.
    // Let's verify manually: from any start, there are 2 neighbors, each leads to unique full path. 4 starts * 2 = 8.
    vector<string> city6 = {"..", ".."};
    assert(countHamiltonianPaths(city6) == 8LL);
    
    // 3x3 with center obstacle, all others empty. Border cells: 8 cells. Count expected.
    vector<string> city7 = {"...", ".#.", "..."};
    // No Hamiltonian path possible because the center obstacle disconnects? Actually edges around center are still connected along the perimeter.
    // Let's compute manually: This forms a cycle of 8 cells (the outer ring). Hamiltonian paths starting and ending anywhere on the ring.
    // For a cycle of 8 nodes, number of Hamiltonian paths is 8 * 7 = 56? No, Hamiltonian path on a cycle: pick start and end, must be different and not adjacent? Actually the paths must visit all 8 nodes exactly once.
    // In a cycle C8, any Hamiltonian path is a contiguous segment, but to visit all nodes it must be the whole cycle without one edge. So choose which edge to break (8 choices) and direction (2) -> 16 paths.
    // But start and end must be border, which all are. So expected 16.
    assert(countHamiltonianPaths(city7) == 16LL);
    
    // All obstacles, no empty
    vector<string> city8 = {"###", "###", "###"};
    assert(countHamiltonianPaths(city8) == 0LL);
    
    cout << "All tests passed!" << endl;
    return 0;
}

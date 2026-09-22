/*
Write a C++ function that takes a grid of characters (representing rooms in a building) with dimensions `n` rows and `m` columns, along with two integers `q` (initial noise level multiplier) and `p` (threshold). The grid contains uppercase letters `A`–`Z` as noise sources and `'*'` as walls (impassable). Each noise source at cell `(i,j)` with letter `ch` generates an initial noise level of `(ch - 'A' + 1) * q`. This noise propagates to all reachable cells (not through walls) in a BFS manner: starting from the source, each neighboring cell (up/down/left/right, if not a wall) receives half the noise of the cell it was reached from (integer division, i.e., `currentNoise / 2`). Propagation stops when the current noise level becomes 0. Multiple sources can contribute to the same cell; their contributions are summed. The function should return the number of cells whose total accumulated noise level is **greater than** `p`. All positions outside the grid or blocked by `'*'` are not considered. The grid is at most 300×300, and all values fit in a 64-bit signed integer.
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Function to count cells with total noise > p in an n x m grid.
// grid: vector of strings, each char is '.' (passable), '*' (wall), or 'A'..'Z' (source).
// q: initial multiplier, p: threshold.
long long countNoisyCells(const vector<string>& grid, ll q, ll p) {
    int n = (int)grid.size();
    if (n == 0) return 0;
    int m = (int)grid[0].size();
    vector<vector<ll>> ans(n, vector<ll>(m, 0));
    vector<vector<int>> mark(n, vector<int>(m, 0));
    int stage = 0;
    struct Cell { int r, c; ll noise; };
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char ch = grid[i][j];
            if (ch >= 'A' && ch <= 'Z') {
                ++stage;
                ll initial = (ll)(ch - 'A' + 1) * q;
                queue<Cell> bfs;
                bfs.push({i, j, initial});
                mark[i][j] = stage;
                while (!bfs.empty()) {
                    auto [r, c, noise] = bfs.front();
                    bfs.pop();
                    ans[r][c] += noise;
                    if (noise / 2 == 0) continue;
                    ll nextNoise = noise / 2;
                    // Up
                    if (r > 0 && grid[r-1][c] != '*' && mark[r-1][c] != stage) {
                        mark[r-1][c] = stage;
                        bfs.push({r-1, c, nextNoise});
                    }
                    // Down
                    if (r+1 < n && grid[r+1][c] != '*' && mark[r+1][c] != stage) {
                        mark[r+1][c] = stage;
                        bfs.push({r+1, c, nextNoise});
                    }
                    // Left
                    if (c > 0 && grid[r][c-1] != '*' && mark[r][c-1] != stage) {
                        mark[r][c-1] = stage;
                        bfs.push({r, c-1, nextNoise});
                    }
                    // Right
                    if (c+1 < m && grid[r][c+1] != '*' && mark[r][c+1] != stage) {
                        mark[r][c+1] = stage;
                        bfs.push({r, c+1, nextNoise});
                    }
                }
            }
        }
    }
    ll count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (ans[i][j] > p) ++count;
        }
    }
    return count;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

long long countNoisyCells(const vector<string>& grid, long long q, long long p);

int main() {
    // Single source A with q=1, p=0: cell itself gets 1 > 0 => 1
    assert(countNoisyCells({"A"}, 1, 0) == 1);
    // Single source A with q=1, p=1: cell gets 1, not >1 => 0
    assert(countNoisyCells({"A"}, 1, 1) == 0);
    // Propagation: A (q=1) in 1x3 grid, p=0: source 1, neighbor 0 (1/2=0) so only 1 cell
    assert(countNoisyCells({"A.."}, 1, 0) == 1);
    // Larger q: q=2, A gives 2, neighbor gives 1, next neighbor gives 0 => 2 cells >0
    assert(countNoisyCells({"A.."}, 2, 0) == 2);
    // Wall blocks: A..*.. with q=2, p=0 => 2 cells on left side, none on right
    assert(countNoisyCells({"A..*.."}, 2, 0) == 2);
    // Two sources: A and B in separate cells, q=1, p=0 => each source contributes, no overlap
    assert(countNoisyCells({"A.B"}, 1, 0) == 2);
    // Overlap: A and A adjacent, q=1, p=0 => each source contributes to itself, neighbor from one source gets 0? Actually q=1 gives source=1, neighbor=0, so only 2 cells
    assert(countNoisyCells({"AA"}, 1, 0) == 2);
    // With q=2: source=2, neighbor=1, each source affects the other, so both cells get 2+1=3 >0 => 2 cells
    assert(countNoisyCells({"AA"}, 2, 0) == 2);
    // Threshold p=2 with q=2 for "AA": each cell gets 3 >2 => 2
    assert(countNoisyCells({"AA"}, 2, 2) == 2);
    // Threshold p=3 with q=2 for "AA": each cell gets 3, not >3 => 0
    assert(countNoisyCells({"AA"}, 2, 3) == 0);
    // Multi-grid: 
    // A.
    // .B   q=1, p=1: A gives 1 at (0,0), B gives 1 at (1,1), neighbors get 0 => cells with >1 are 0
    assert(countNoisyCells({"A.", ".B"}, 1, 1) == 0);
    // 3x3 with wall in middle, q=1, p=0: A in corner, only its own cell >0 unless neighbors get 0; so 1
    assert(countNoisyCells({"A..", ".*.", "..."}, 1, 0) == 1);

    cout << "All tests passed!" << endl;
    return 0;
}

// The problem is a multi-source BFS with decaying noise. For each uppercase letter in the grid, run a breadth-first search starting from that cell with initial noise `(letter - 'A' + 1) * q`. Use a queue storing `{row, col, currentNoise}`. For each popped cell, add `currentNoise` to an accumulator array `ans` (initialized to 0). If `currentNoise / 2 != 0`, then for each of the four orthogonal neighbors that is within bounds and not a wall `'*'`, and that has not already been visited by this BFS (use a `mark` array with a unique stage identifier per source to avoid revisiting cells in the same propagation), push the neighbor with noise `currentNoise / 2`. Since each source uses its own BFS, cells can be visited multiple times across different sources, which is correct because noise sums. Important edge cases: walls block propagation entirely; sources may be adjacent and their propagations overlap; noise can be large (use `long long`); if `p` is negative, the number of cells with sum > p could be large, but the logic still holds. Time complexity: For each of up to `n*m` sources, BFS visits each reachable non-wall cell once, so worst-case O(n*m * n*m) = O(300^4) ≈ 8.1e9 which might be borderline, but typical constraints in such problems are smaller or sources are sparse; we assume the test data fits. Space complexity O(n*m) for `ans` and `mark`. We present an efficient implementation that reuses a single `mark` array with a monotonically increasing stage counter.

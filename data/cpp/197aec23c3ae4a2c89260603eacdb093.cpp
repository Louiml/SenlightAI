// You are given an \(n \times n\) grid where each cell can either be unassigned (value `-1`) or assigned one of four directions: `'L'`, `'R'`, `'U'`, `'D'`. A cell is considered **good** if it points outside the grid (e.g., a left arrow in the leftmost column) or points toward an already good neighboring cell. Initially, all cells are unassigned, so every cell is good (since an unassigned cell can be considered to have a direction that leads to a good neighbor; in particular, treat it as good immediately). You are given a sequence of \(q\) updates. Each update specifies a cell \((r, c)\) and a direction character `t`, and **sets** that cell's direction to `t` (overwriting any previous assignment). After all \(q\) updates have been applied, you must process the updates in reverse order, and before undoing each update you need to compute the number of **bad** cells (cells that are not good) in the current grid. Then you undo the update by setting that cell back to unassigned (`-1`), and recompute the good status of affected cells. Your task is to write a C++ function that takes \(n\), the list of updates (as tuples of row, column, direction), and returns a vector of \(q\) integers: the count of bad cells just before each reverse-step (i.e., the state after all updates except the last one, then after all but the last two, etc., down to the initial state after zero undos). The grid is 1-indexed; rows and columns are from 1 to \(n\). The function must efficiently maintain the set of good cells using DFS/BFS propagation, and return the answers in the original order of the updates (i.e., the first element corresponds to the state before the last update is undone, which is the state after all updates have been applied).
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Include the provided solution function here (or paste above)

int main() {
    // Test 1: simple 2x2, one update 'L' at (1,1)
    {
        int n = 2;
        vector<tuple<int,int,char>> updates = {{1,1,'L'}};
        vector<int> res = computeBadCounts(n, updates);
        // After all updates: grid has (1,1) pointing left (out of bounds) -> good; others unassigned -> good. Bad=0.
        // Then undo update: all unassigned -> good. Bad=0.
        assert(res.size() == 1 && res[0] == 0);
    }
    // Test 2: 2x2, update (1,1)='R', (1,2)='L' → they point to each other, none points outside → both bad? Actually (1,1) points to (1,2) which is not good yet, so bad. (1,2) points to (1,1) not good. So bad=2. Then undo last update: (1,2) unassigned → good, (1,1) points to (1,2) which is good → good. Bad=0.
    {
        int n = 2;
        vector<tuple<int,int,char>> updates = {{1,1,'R'}, {1,2,'L'}};
        vector<int> res = computeBadCounts(n, updates);
        // State after both updates: (1,1) R, (1,2) L → bad=2
        // Then undo (1,2) L → now (1,2) unassigned, (1,1) R points to good neighbor → good, bad=0
        assert(res.size() == 2 && res[0] == 2 && res[1] == 0);
    }
    // Test 3: 1x1 grid, one update 'D'
    {
        int n = 1;
        vector<tuple<int,int,char>> updates = {{1,1,'D'}};
        vector<int> res = computeBadCounts(n, updates);
        // Only cell points down (out of bounds) → good. Bad=0. Undo → unassigned good. Bad=0.
        assert(res.size() == 1 && res[0] == 0);
    }
    // Test 4: 3x3, a cycle of 4 cells pointing in a loop (bad), one outside pointing in good
    {
        int n = 3;
        vector<tuple<int,int,char>> updates = {
            {2,2,'D'}, // points down to (3,2) which will be unassigned → good
            {3,2,'D'}, // points out of bounds → good
            {1,1,'R'}, {1,2,'R'}, {2,2,'U'}, {2,1,'D'}? // Actually let's craft a small cycle
        };
        // Simpler: 2x2 cycle: (1,1)->R, (1,2)->D, (2,2)->L, (2,1)->U creates a cycle, all bad.
        n = 2;
        updates = {{1,1,'R'}, {1,2,'D'}, {2,2,'L'}, {2,1,'U'}};
        vector<int> res = computeBadCounts(n, updates);
        // After all updates: all 4 bad. Then undo last update (2,1) becomes unassigned → good, and then (1,1) points to (1,2) which is still bad? Actually (1,1) points to (1,2) which is bad, but (1,1) could also point left to outside? No, it's fixed to R. So (1,1) still bad. (1,2) points to (2,2) bad. So after undoing (2,1), only (2,1) good, others bad → bad=3. Then undo (2,2) → now (1,2) points to (2,2) which is unassigned → good, so (1,2) good, and (1,1) points to (1,2) good → good. Bad=0? Let's compute expected: 
        // After all 4 updates: all 4 bad → res[0]=4
        // Undo (2,1) → (2,1) unassigned good. Now (1,1) R->(1,2) bad; (1,2) D->(2,2) bad; (2,2) L->(2,1) good → so (2,2) good, then (1,2) D->(2,2) good → (1,2) good, then (1,1) R->(1,2) good → (1,1) good. So all good except? Actually after undoing (2,1), we have (1,1) R, (1,2) D, (2,2) L, and (2,1) unassigned. Let's compute good: (2,2) points to (2,1) unassigned → good. Then (1,2) points to (2,2) good → good. Then (1,1) points to (1,2) good → good. So all good → bad=0! So res[1]=0. Then undo (2,2) → all unassigned except (1,1) R, (1,2) D, and (2,1) unassigned. (1,2) points down to (2,2) unassigned → good, (1,1) points to (1,2) good → good, (2,1) unassigned good. So bad=0. etc. So final res = {4,0,0,0}. We'll just test first two.
        assert(res.size() == 4 && res[0] == 4 && res[1] == 0);
    }
    return 0;
}
#include <vector>
#include <tuple>
#include <cstring>
#include <algorithm>

// Function to compute bad cell counts when processing updates in reverse.
// n: grid size (1-indexed rows/cols)
// updates: vector of (row, col, direction) where direction is 'L','R','U','D'
// Returns vector of bad counts: before undoing each update (from last to first reversed)
std::vector<int> computeBadCounts(int n, const std::vector<std::tuple<int,int,char>>& updates) {
    const int MAX = 1005;
    static int dir[MAX][MAX]; // -1 unassigned, else index for 'L','R','U','D'
    static bool good[MAX][MAX];
    static int di[4] = {0, 0, -1, 1};
    static int dj[4] = {-1, 1, 0, 0};
    static const char allDirs[4] = {'L','R','U','D'};

    // reset grids
    std::memset(dir, -1, sizeof(dir));
    std::memset(good, 0, sizeof(good));

    // helper lambdas
    auto inside = [n](int i, int j) -> bool {
        return i >= 1 && i <= n && j >= 1 && j <= n;
    };

    // checks if cell (i,j) is good; precondition: inside(i,j)
    auto isGood = [&](int i, int j) -> bool {
        for (int d = 0; d < 4; d++) {
            if (dir[i][j] != -1 && d != dir[i][j]) continue;
            int ni = i + di[d];
            int nj = j + dj[d];
            if (!inside(ni, nj) || good[ni][nj]) {
                return true;
            }
        }
        return false;
    };

    // DFS to mark good cells from (i,j) if possible
    int curGood = 0;
    std::function<void(int,int)> dfs = [&](int i, int j) {
        if (!inside(i,j) || good[i][j] || !isGood(i,j)) return;
        good[i][j] = true;
        curGood++;
        for (int d = 0; d < 4; d++) {
            dfs(i + di[d], j + dj[d]);
        }
    };

    // apply all initial updates
    for (auto& [r, c, t] : updates) {
        int idx = std::find(allDirs, allDirs+4, t) - allDirs;
        dir[r][c] = idx;
    }

    // initial good computation
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            dfs(i, j);
        }
    }

    std::vector<int> result;
    // process updates in reverse
    for (auto it = updates.rbegin(); it != updates.rend(); ++it) {
        int r, c; char t;
        std::tie(r, c, t) = *it;
        // record bad count before undoing this update
        result.push_back(n * n - curGood);
        // undo: set to unassigned
        dir[r][c] = -1;
        // if it becomes good (it will because unassigned is good) propagate
        dfs(r, c);
    }
    // result is in reverse order; we need to reverse it back
    std::reverse(result.begin(), result.end());
    return result;
}
// The key insight is that a cell is good if it has at least one direction (or a possible direction among its four) that either goes out of bounds or leads to an already good cell. When a cell is unassigned (`-1`), we consider it good immediately because we can choose a direction that satisfies the condition (e.g., point outward if possible, or toward any neighbor that is good). To handle updates efficiently, we process them in reverse: initially after all updates are applied, we compute the good cells using a DFS from every cell that is good by definition (unassigned or pointing outside). Then we reverse the update list. For each reversed update, we first record the current number of bad cells (`n*n - curGood`), then set that cell to unassigned (`-1`), and call a DFS from that cell to mark new good cells if it becomes good (since unassigned makes it good and it may propagate to neighbors that point to it). This works because when we remove a direction, cells only become more likely to be good, never less. Edge cases: the grid may be size 1; updates may target the same cell multiple times; the direction character mapping must be correct. Time complexity: Each cell is added to the good set at most once, and each DFS edge is traversed at most once per cell, so total \(O(n^2 + q \cdot \text{constant})\) average, but in worst case a single DFS can visit many cells, but since each cell is marked good at most once across all DFS calls, the total work is \(O(n^2 + q)\) amortized. Space complexity: \(O(n^2)\) for the `good` and `dir` grids.

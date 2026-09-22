Write a C++ function that, given a grid of characters with cells either `.` (passable) or `#` (wall), along with a movement string `s` containing only `L`, `R`, `U`, `D`, and the original grid dimensions `h` and `c`, returns the number of starting positions `(i,j)` on the grid such that when a robot starts at `(i,j)` and executes the full movement string in order, every cell the robot visits (including the starting cell, after each move, and the final cell) is passable (`.`). The grid uses 0-based indexing, and the robot cannot move outside the grid at any step. The movement string length `n` is between 1 and 100, and `h` and `c` are each between 1 and 500. The grid rows are given as strings of length `c` each, containing only `.` and `#`.
The key observation is that the set of visited cells relative to the starting position is the same for every possible start, because the movement pattern is fixed. We first precompute the cumulative displacement after each move by simulating the movement string once from coordinates `(0,0)`. Let `x[k]` and `y[k]` be the row and column offset after the first `k` moves (with `x[0]=y[0]=0`). For a starting position `(i,j)`, the robot visits cells `(i + x[k], j + y[k])` for `k=0..n`. Thus, for each candidate `(i,j)`, we need to check that for all `k`, the cell `(i + x[k], j + y[k])` is within bounds and is `.`. Naively this is `O(h*c*n)` which is up to `500*500*100 = 25 million`, acceptable, but we can optimize by first computing a 2D boolean prefix sum (or simple grid check) to speed up each check, but for constraints it is fine to do direct checks. Edge cases include: starting positions where the first cell is `#` or out of bounds (though we only iterate over in-bounds starts), and movements that cause the robot to leave the grid during the path — those must be excluded. The total displacement `(x[n], y[n])` is also useful: only starting cells `(i,j)` such that `i + x[n]`, `j + y[n]` are within grid bounds can possibly be valid, but even then intermediate steps might go out of bounds. The simplest robust solution is to precompute all offsets and then for each in-bounds start, check all steps. Time complexity is `O(h*c*n)` in the worst case, but since `n <= 100` and grid up to `250000`, worst-case operations are ~25 million, fine in C++. Space complexity is `O(n)` for the offset arrays, plus we may store the grid as vector of strings.
#include <string>
#include <vector>

// Counts valid starting positions for the robot given a grid and movement string.
// grid is a vector of strings, each string length c, characters '.' or '#'.
// s is movement string of length n with 'L','R','U','D'.
// Returns the number of starting cells (i,j) such that the robot never hits '#' or goes outside.
int countValidStarts(const std::vector<std::string>& grid, const std::string& s) {
    const int h = static_cast<int>(grid.size());
    const int c = static_cast<int>(grid[0].size());
    const int n = static_cast<int>(s.size());

    // Precompute cumulative row and column offsets after each move.
    std::vector<int> rowOffsets(n + 1, 0);
    std::vector<int> colOffsets(n + 1, 0);
    for (int k = 0; k < n; ++k) {
        int dr = 0, dc = 0;
        if (s[k] == 'L') dc = -1;
        else if (s[k] == 'R') dc = 1;
        else if (s[k] == 'U') dr = -1;
        else if (s[k] == 'D') dr = 1;
        rowOffsets[k + 1] = rowOffsets[k] + dr;
        colOffsets[k + 1] = colOffsets[k] + dc;
    }

    int answer = 0;
    // Iterate over all possible starting cells (i,j).
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < c; ++j) {
            // Quick rejection: the final cell after full movement must be inside.
            int fi = i + rowOffsets[n];
            int fj = j + colOffsets[n];
            if (fi < 0 || fi >= h || fj < 0 || fj >= c) continue;

            // Simulate all steps.
            bool valid = true;
            for (int k = 0; k <= n; ++k) {
                int ni = i + rowOffsets[k];
                int nj = j + colOffsets[k];
                if (ni < 0 || ni >= h || nj < 0 || nj >= c || grid[ni][nj] != '.') {
                    valid = false;
                    break;
                }
            }
            if (valid) ++answer;
        }
    }
    return answer;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function here (or link it).
// For testing, we copy the function definition.
int countValidStarts(const std::vector<std::string>& grid, const std::string& s);

int main() {
    // Test 1: Single cell, no movement (empty string not allowed, but we test with "L" that goes out)
    {
        std::vector<std::string> g = {"."};
        assert(countValidStarts(g, "L") == 0); // moves out of grid
        assert(countValidStarts(g, "R") == 0);
        assert(countValidStarts(g, "RDD") == 0);
    }
    // Test 2: 1x3 grid, movement "R" — valid starts at col 0 and col 1, but col 2 goes out
    {
        std::vector<std::string> g = {"..."};
        assert(countValidStarts(g, "R") == 2); // starts at (0,0) and (0,1)
    }
    // Test 3: 2x2 grid, movement "DR" — starts at (0,0) works: (0,0)->(1,0)->(1,1) all '.'
    {
        std::vector<std::string> g = {"..", ".."};
        assert(countValidStarts(g, "DR") == 1); // only (0,0)
    }
    // Test 4: Grid with a wall blocking
    {
        std::vector<std::string> g = {".#", ".."};
        // Movement "R" — start (0,0) -> (0,1) which is '#', invalid; start (0,1) -> out; start (1,0)-> (1,1) valid; start (1,1)->out
        assert(countValidStarts(g, "R") == 1); // only (1,0)
    }
    // Test 5: Longer movement, simple loop
    {
        std::vector<std::string> g = {"....", "....", "...."};
        // Movement "RDLU" returns to start, all cells must be '.' — all 12 starts valid? Actually all cells passable, so all 12.
        assert(countValidStarts(g, "RDLU") == 12);
    }
    // Test 6: Movement with U from top row goes out
    {
        std::vector<std::string> g = {"...", "...", "..."};
        // "U" — only starts in row 2 are valid (3 starts)
        assert(countValidStarts(g, "U") == 3);
    }
    // Test 7: Wall in the middle of path
    {
        std::vector<std::string> g = {"..#", "...", "..."};
        // Movement "DR" — starts at (0,0): (0,0)->(1,0)->(1,1) good; (0,1): (0,1)->(1,1)->(1,2) good; (0,2) is '#' so not valid; (1,0): (1,0)->(2,0)->(2,1) good; etc. Let's manually count: (0,0) ✓, (0,1) ✓, (1,0) ✓, (1,1) ✓, (2,0) ✓, (2,1) ✓. Others out or start on '#'? (0,2) no, (1,2) -> (2,2) -> out? (1,2) start -> (2,2) good? actually (1,2) '.' then (2,2) '.' then out after? DR: start (1,2)->(2,2)->(2,3) out so invalid. So total 6.
        assert(countValidStarts(g, "DR") == 6);
    }
    // Test 8: Grid 1x5, movement "RL" — net zero but visits start and after L and after R back; all positions valid if all '.'? Actually "RL": from start (0,0): visits (0,0)->(0,1)->(0,0) all '.' so valid; (0,1): visits (0,2),(0,1) valid; (0,2): (0,3),(0,2) valid; (0,3): (0,4),(0,3) valid; (0,4): (0,5) out invalid. So 4 valid.
    {
        std::vector<std::string> g = {"....."};
        assert(countValidStarts(g, "RL") == 4);
    }
    // Test 9: Movement length 1 with single cell grid and "D" 
    {
        std::vector<std::string> g = {"."};
        assert(countValidStarts(g, "D") == 0);
    }
    // Test 10: Larger grid, ensure no crash and correct count
    {
        std::vector<std::string> g(5, std::string(5, '.'));
        // Movement "RRDDLLUU" returns to start but visits all 8 neighbors? Actually path: (0,0)->(0,1)->(0,2)->(1,2)->(2,2)->(2,1)->(2,0)->(1,0)->(0,0). All must be '.' — all cells are '.', so any start that stays within bounds is valid. The movement total displacement is (0,0), so we need all intermediate cells in bounds for each start. Since grid is 5x5, starts at any cell (i,j) such that path stays in bounds. The path reaches offsets: (0,0),(0,1),(0,2),(1,2),(2,2),(2,1),(2,0),(1,0),(0,0). So valid starts are those (i,j) with i+0 in [0,4], i+2 in [0,4], j+2 in [0,4], etc. So i must be 0..2, j must be 0..2 → 9 starts.
        assert(countValidStarts(g, "RRDDLLUU") == 9);
    }
    return 0;
}

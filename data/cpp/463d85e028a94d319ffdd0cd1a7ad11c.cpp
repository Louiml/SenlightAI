Write a C++ function `long long illuminatedPairsSum(int h, int w, const std::vector<std::string>& grid)` that, given a grid of `h` rows and `w` columns where each cell is either `'.'` (empty) or `'#'` (blocked), computes the sum over all empty cells of `2^(k-1)`, where `k` is the number of empty cells that lie in the same contiguous horizontal or vertical segment as that cell (including itself). More precisely, for each empty cell, define `cnt = horizontalRun + verticalRun - 1`, where `horizontalRun` is the number of consecutive empty cells in the same row including the cell, and `verticalRun` is the number of consecutive empty cells in the same column including the cell. Then `cnt` is the number of distinct empty cells reachable from that cell by moving only horizontally or only vertically without passing through a `'#'`. For each empty cell, add `2^(totalEmpty - cnt)` modulo `1'000'000'007` to the answer. The total number of empty cells is `totalEmpty`. Return the final sum modulo `1'000'000'007`. The grid dimensions are positive and at most 2000×2000. The grid contains only `'.'` and `'#'`. The function must be efficient for large grids.

// The key is to precompute for every empty cell both the length of its contiguous horizontal run of `'.'` and the length of its contiguous vertical run of `'.'`. Instead of scanning each run for every cell (which would be O(h²w²)), we traverse each row once, and for each maximal horizontal segment of consecutive `'.'`, we assign its length to every cell in that segment. Similarly, for each maximal vertical segment, assign its length to every cell. For a cell, `cnt = horizontalLen + verticalLen - 1` because the cell itself is counted twice. This gives exactly the size of the union of its horizontal and vertical reachable empty cells. Then we need to compute `2^(totalEmpty - cnt)` for each cell. Precompute powers of two up to `totalEmpty` and also prefix sums of powers of two in reverse order: let `pow2[i] = 2^i mod MOD`, and `pow2sum[i] = sum_{j=0}^{i-1} pow2[totalEmpty - j]` so that `pow2sum[cnt] = sum_{j=0}^{cnt-1} 2^(totalEmpty - j)`. Then each cell adds `pow2sum[cnt]` to the answer. This works because `totalEmpty - cnt` ranges from `0` to `totalEmpty-1`, and the sum of `2^(totalEmpty - j)` for `j` from 0 to cnt-1 is exactly what we need. Edge cases: cells that are blocked contribute zero. Grid can be all blocked (then answer 0). Grid can be all empty, then for each cell cnt is its row length + column length -1. Complexity: two O(hw) passes to fill horizontal and vertical run lengths, plus one O(hw) pass to accumulate the answer, so O(hw) time and O(hw) extra space for the run-length arrays and powers. Memory is O(hw) for the arrays.

#include <vector>
#include <string>

const long long MOD = 1000000007LL;

// Given a grid of '.' and '#', compute the sum over all empty cells of
// 2^(totalEmpty - cnt), where cnt = horizontalRun + verticalRun - 1.
// Returns the result modulo 1'000'000'007.
long long illuminatedPairsSum(int h, int w, const std::vector<std::string>& grid) {
    // Count total empty cells.
    long long totalEmpty = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == '.') ++totalEmpty;
        }
    }

    // Precompute powers of two: pow2[i] = 2^i mod MOD.
    std::vector<long long> pow2(totalEmpty + 1, 1);
    for (int i = 1; i <= totalEmpty; ++i) {
        pow2[i] = (pow2[i - 1] * 2) % MOD;
    }

    // pow2sum[i] = sum_{j=0}^{i-1} pow2[totalEmpty - j]
    // so pow2sum[0] = 0, pow2sum[1] = pow2[totalEmpty], etc.
    std::vector<long long> pow2sum(totalEmpty + 1, 0);
    for (int i = 1; i <= totalEmpty; ++i) {
        pow2sum[i] = (pow2sum[i - 1] + pow2[totalEmpty - i + 1]) % MOD; // careful index
    }
    // Correction: pow2sum[i] should be sum_{j=0}^{i-1} pow2[totalEmpty - j].
    // So pow2sum[1] = pow2[totalEmpty], pow2sum[2] = pow2[totalEmpty] + pow2[totalEmpty-1], ...
    // The loop above is wrong: i ranges 1..totalEmpty, but we want index totalEmpty - (i-1)
    // Let's redo correctly:
    pow2sum[0] = 0;
    for (int i = 1; i <= totalEmpty; ++i) {
        pow2sum[i] = (pow2sum[i - 1] + pow2[totalEmpty - (i - 1)]) % MOD;
    }

    // horizontalLen[i][j] = length of horizontal run of '.' containing cell (i,j)
    std::vector<std::vector<int> > horizontalLen(h, std::vector<int>(w, 0));
    for (int i = 0; i < h; ++i) {
        int j = 0;
        while (j < w) {
            if (grid[i][j] == '#') {
                ++j;
                continue;
            }
            int start = j;
            while (j < w && grid[i][j] == '.') ++j;
            int runLength = j - start;
            for (int k = start; k < j; ++k) {
                horizontalLen[i][k] = runLength;
            }
        }
    }

    // verticalLen[i][j] = length of vertical run of '.' containing cell (i,j)
    std::vector<std::vector<int> > verticalLen(h, std::vector<int>(w, 0));
    for (int j = 0; j < w; ++j) {
        int i = 0;
        while (i < h) {
            if (grid[i][j] == '#') {
                ++i;
                continue;
            }
            int start = i;
            while (i < h && grid[i][j] == '.') ++i;
            int runLength = i - start;
            for (int k = start; k < i; ++k) {
                verticalLen[k][j] = runLength;
            }
        }
    }

    long long answer = 0;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j] == '.') {
                int cnt = horizontalLen[i][j] + verticalLen[i][j] - 1;
                answer = (answer + pow2sum[cnt]) % MOD;
            }
        }
    }

    return answer;
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the solution function (to be included from the solution header).
long long illuminatedPairsSum(int h, int w, const std::vector<std::string>& grid);

int main() {
    // Test 1: All blocked -> answer 0
    {
        std::vector<std::string> grid = {"###", "###"};
        assert(illuminatedPairsSum(2, 3, grid) == 0);
    }

    // Test 2: Single empty cell -> totalEmpty=1, cnt=1+1-1=1, sum = 2^(1-1)=1
    {
        std::vector<std::string> grid = {".", "#"};
        assert(illuminatedPairsSum(2, 1, grid) == 1);
    }

    // Test 3: Two empty cells in same row, separated by a wall? Actually ".#." -> left and right separate
    // Row ".#.": left cell has horizontal run=1, vertical run=1, cnt=1 -> 2^(2-1)=2. Right cell similar -> 2. total 4.
    {
        std::vector<std::string> grid = {".#."};
        assert(illuminatedPairsSum(1, 3, grid) == 4);
    }

    // Test 4: 2x2 all empty
    // totalEmpty=4. For each cell, horizontal=2, vertical=2, cnt=3 -> 2^(4-3)=2 each -> sum=8.
    {
        std::vector<std::string> grid = {"..", ".."};
        assert(illuminatedPairsSum(2, 2, grid) == 8);
    }

    // Test 5: 1x4 all empty -> each cell horizontal=4, vertical=1, cnt=4, totalEmpty=4, contribution=2^0=1 each -> sum=4
    {
        std::vector<std::string> grid = {"...."};
        assert(illuminatedPairsSum(1, 4, grid) == 4);
    }

    // Test 6: 4x1 all empty -> each cell vertical=4, horizontal=1, cnt=4 -> sum=4
    {
        std::vector<std::string> grid = {".", ".", ".", "."};
        assert(illuminatedPairsSum(4, 1, grid) == 4);
    }

    // Test 7: Mixed grid with a plus shape
    // Grid:
    // .#.
    // ...
    // .#.
    // totalEmpty=7, but each cell's cnt varies. Let's compute manually:
    // Cell (0,0): horizontal run=1, vertical run=3 (col 0 has rows 0,1,2) -> cnt=3 => 2^(7-3)=16
    // Cell (0,2): horizontal=1, vertical=3 -> cnt=3 => 16
    // Cell (1,0): horizontal=3 (row1 all dots), vertical=3 -> cnt=5 => 2^(2)=4
    // Cell (1,1): horizontal=3, vertical=1 (only itself) -> cnt=3 => 16
    // Cell (1,2): horizontal=3, vertical=3 -> cnt=5 => 4
    // Cell (2,0): horizontal=1, vertical=3 -> cnt=3 => 16
    // Cell (2,2): horizontal=1, vertical=3 -> cnt=3 => 16
    // Sum = 16+16+4+16+4+16+16 = 88
    {
        std::vector<std::string> grid = {".#.", "...", ".#."};
        assert(illuminatedPairsSum(3, 3, grid) == 88);
    }

    // Test 8: Large all-empty 3x3 -> each cell: horizontal=3, vertical=3, cnt=5, totalEmpty=9, contribution=2^(9-5)=16 each, 9 cells -> 144
    {
        std::vector<std::string> grid = {"...", "...", "..."};
        assert(illuminatedPairsSum(3, 3, grid) == 144);
    }

    // Test 9: Single row ".#." with totalEmpty=2 -> already tested as Test 3, but recheck with bigger
    // Not needed, but ensure it works.

    // Test 10: Check modulo for worst case: full 2000x2000 empty, but we just check a smaller case
    // 1x1 empty -> 1
    {
        std::vector<std::string> grid = {"."};
        assert(illuminatedPairsSum(1, 1, grid) == 1);
    }

    return 0;
}

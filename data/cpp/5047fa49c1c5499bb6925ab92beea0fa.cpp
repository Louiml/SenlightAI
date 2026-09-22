/*
Implement a C++ function `generateGrid(int n)` that returns a `pair<vector<vector<long long>>, vector<vector<int>>>`. The first element is an n x n grid of 64-bit integers, the second is an n x n grid of ints (0 or 1). The grids are constructed as follows: Number rows and columns from 1 to n. For each diagonal with sum `s = i + j` (where `i` is row, `j` is column), consider the cells on that diagonal ordered from smallest row to largest row. For the `t`-th cell on that diagonal (t starting from 0), set `value = 1LL << (s - 2 - (t % 2))` if `s - 2 - (t % 2) >= 0`, otherwise `value = 0`. Set `dirValue = 1 - (t % 2)` (so 1 for even t, 0 for odd t). Store these in the 0-indexed grids: `values[i-1][j-1] = value` and `dir[i-1][j-1] = dirValue` for each cell `(i,j)`. Return the two grids.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Generate the value and direction grids as described.
// Returns {values, dir} where both are n x n (0-indexed).
std::pair<std::vector<std::vector<long long>>, std::vector<std::vector<int>>>
generateGrid(int n) {
    std::vector<std::vector<long long>> values(n, std::vector<long long>(n, 0));
    std::vector<std::vector<int>> dir(n, std::vector<int>(n, 0));
    
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            int s = (r + 1) + (c + 1);  // 1-indexed diagonal sum
            // 0-indexed start row on this diagonal
            int start = std::max(0, s - n - 1);
            int t = (r + 1) - (start + 1); // position along diagonal, 0-based
            int shift = (s - 2) - (t % 2);
            if (shift >= 0) {
                values[r][c] = 1LL << shift;
            } else {
                values[r][c] = 0;
            }
            dir[r][c] = 1 - (t % 2);
        }
    }
    return {values, dir};
}
#include <cassert>
#include <vector>
#include <utility>

// The function is assumed to be declared above.

int main() {
    // n = 1
    auto [v1, d1] = generateGrid(1);
    assert(v1.size() == 1 && v1[0].size() == 1);
    assert(v1[0][0] == 1LL);
    assert(d1[0][0] == 1);

    // n = 2
    auto [v2, d2] = generateGrid(2);
    // Expected values (0-indexed):
    // (0,0): s=2, t=0 -> shift 0, value 1
    // (0,1): s=3, t=0 -> shift 1, value 2
    // (1,0): s=3, t=1 -> shift 0, value 1
    // (1,1): s=4, t=0 -> shift 2, value 4
    assert(v2[0][0] == 1LL);
    assert(v2[0][1] == 2LL);
    assert(v2[1][0] == 1LL);
    assert(v2[1][1] == 4LL);
    // dir: t even ->1, t odd ->0
    assert(d2[0][0] == 1);
    assert(d2[0][1] == 1);
    assert(d2[1][0] == 0);
    assert(d2[1][1] == 1);

    // n = 3
    auto [v3, d3] = generateGrid(3);
    // Check a few known cells from the snippet's algorithm
    // (0,0): s=2 -> 1<<0=1, dir 1
    // (0,2): s=4, t=0 -> 1<<2=4, dir 1
    // (1,1): s=4, t=1 (since start row for s=4 is max(0,4-3-1=0) start=0, r=1 -> t=1) -> shift = 4-2-1=1 -> 2, dir 0
    // (2,0): s=4, t=2 -> shift = 4-2-0=2 -> 4, dir 1
    // (2,2): s=6, start = max(0,6-3-1=2) start=2, t=0 -> shift=4 -> 16, dir 1
    assert(v3[0][0] == 1LL);
    assert(v3[0][2] == 4LL);
    assert(v3[1][1] == 2LL);
    assert(v3[2][0] == 4LL);
    assert(v3[2][2] == 16LL);
    assert(d3[0][0] == 1);
    assert(d3[0][2] == 1);
    assert(d3[1][1] == 0);
    assert(d3[2][0] == 1);
    assert(d3[2][2] == 1);

    // n = 5, check that all values are powers of two and last diagonal has largest.
    auto [v5, d5] = generateGrid(5);
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            long long val = v5[r][c];
            assert(val > 0);
            assert((val & (val - 1)) == 0); // power of two
            assert(d5[r][c] == 0 || d5[r][c] == 1);
        }
    }
    assert(v5[4][4] == (1LL << 8)); // s=10, shift 8

    return 0;
}
// The construction iterates over all anti-diagonals. A cell `(r,c)` (0-indexed) lies on diagonal `s = (r+1)+(c+1) = r+c+2`. The starting row on that diagonal is `max(1, s-n)` in 1-indexed, so in 0-indexed it's `start = max(0, s-n-1)`. The position `t = (r+1) - (start+1) = r - start`. Then the value shift is `s - 2 - (t%2)`, which is always non-negative because for the last cell on a diagonal, `t` is at most `min(n, s-1) - max(1, s-n)`, and the minimum shift occurs when `t` is odd and equal to the length-1; for smallest diagonal s=2, length=1, t=0 even, shift=0; for s=3, length=2, t=1 odd, shift=3-2-1=0; for general, the minimum shift is 0. So no overflow issues for n up to 30 (max shift 58). The algorithm runs in O(n^2) time and O(n^2) space, which is optimal since the output itself is O(n^2). Edge cases: n=1 yields a single cell with value 1 and dir 1. No special handling needed beyond ensuring shifts are non-negative (they always are for valid n≥1). The direction grid uses 0 for "odd" positions and 1 for "even" positions, which matches the snippet's `fl` flag.

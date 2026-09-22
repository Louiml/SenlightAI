// Write a C++ function `long long optimizeTransformation(int n, int m, int k, const std::vector<std::string>& grid, const std::vector<std::array<int,4>>& rects, const std::string& letters)` that, given an `n x m` grid of lowercase letters (indices 1-based internally), a list of `k` rectangle operations each defined by top-left `(x1, y1)` and bottom-right `(x2, y2)` (1-based input, inclusive) and a target lowercase letter, computes the minimum possible total cost after applying at most one operation globally. Initially, the cost is the sum over all cells of `|current_letter - original_letter|` where `current_letter` is the original letter. For each operation, you may choose to change all letters inside that rectangle to the operation's target letter exactly once, replacing the original letters there (i.e., the operation overwrites the original values). The final cost is evaluated after optionally applying exactly one of the `k` operations (or none, but the operation must be applied). The function must return the minimum achievable total cost. Input constraints: `1 <= n,m <= 1000`, `1 <= k <= 300000`, rectangle coordinates are valid and inclusive, letters are lowercase `a`-`z`. The function should be efficient enough for the given limits.
// We need to handle up to 300k rectangle updates and 1e6 cells. A naive per-operation O(n*m) is impossible. We use difference arrays (2D prefix sums) to accumulate, for each cell, how many operations include it (`times`). Then, for each cell, the number of operations that do **not** cover it is `k - times[i][j]`; those non-covering operations will still have the original letter at that cell. The covering operations would replace that cell's letter with their target letter. However, we are allowed to apply exactly one operation, so we need to compute the incremental cost delta for each operation and take the minimum.
//
// The key insight: compute the base cost for all cells as if no operation is applied (original letters). Then, for each operation, we need to compute the new cost if that operation is applied. For a cell inside the operation's rectangle, its contribution changes from `|orig - orig|` (0) to `|target - orig|` (since it's overwritten). For cells outside the rectangle, the contribution remains `|orig - orig|` = 0. Thus the total cost for an operation is simply the sum over all cells inside the rectangle of `|target_letter - original_letter|`. So the minimum over all operations is the minimum of that sum over all rectangles.
//
// But careful: The problem statement says "apply exactly one operation" – so we must apply one, not none. So the answer is `min_{i} sum_{cell in rect_i} |target_i - original_cell|`.
//
// We can precompute, for each letter `z` (0..25), a 2D prefix sum `cnt[z][i][j]` that gives the number of cells with original letter `z` in the top-left `i x j` submatrix. Then the sum inside a rectangle `(x1,y1)-(x2,y2)` for a target letter `t` is `sum_{z=0..25} |t - z| * count_z_in_rect`. Using the 2D prefix sums, each rectangle query is O(26) = O(1) constant. So total time O(n*m*26 + k). Space O(26*n*m) = about 26*1e6*8 bytes ~ 208MB, which is large but acceptable with careful memory (use `long long`). However, the original snippet uses `long long cnt[26][maxn][maxn]` with `maxn=1123` which fits. We can use `std::vector<std::vector<std::array<long long,26>>>` but that's overhead; better to use a flat array or `vector<vector<array<long long,26>>>`. We'll implement with a static 3D vector.
//
// Edge cases: n,m up to 1000, k large, rectangle coordinates inclusive. Need to use 1-based indexing internally. Use prefix sums over the grid directly.
//
// Correctness: Since each operation overwrites the entire rectangle, the cost after applying operation `i` is exactly the sum of `|target_i - original|` for all cells in that rectangle, because outside the rectangle original letters remain and cost 0. So the minimum over all operations is the answer.
//
// Time complexity: O(26*n*m + 26*k) = O(26*(n*m+k)) which is about 26*1e6 + 26*3e5 = 26e6 + 7.8e6 = ~34e6 operations, fine. Space: O(26*n*m) long longs = 26*1e6*8 = 208 MB, may be borderline but acceptable in typical C++ memory limits (256 MB). We can reduce by using `int` for counts? But counts can be up to n*m = 1e6, fits in int. However, sums of `|t-z|*count` can be up to 25*1e6 = 25e6, fits in long long. We'll store `int` counts and compute with `long long`.
//
// Implementation details: read grid, build 2D prefix sum for each letter. The reference solution will define a function that takes n,m,k, grid as vector<string> (1-indexed? We'll store 0-indexed but handle coordinates as given), and rectangles as vector of struct {int x1,y1,x2,y2; char letter;}. Convert letters to index 0..25. Build prefix sum `pref[26][n+1][m+1]` where `pref[z][i][j]` = count of letter z in submatrix rows 1..i, cols 1..j. Then for each rectangle, compute sum = Σ_z |t-z|*(rect_sum). The `rect_sum` is `pref[z][x2][y2] - pref[z][x1-1][y2] - pref[z][x2][y1-1] + pref[z][x1-1][y1-1]`. Take minimum over all rectangles.
//
// We must be careful about input format: given `rects` as vector of four ints (x1,y1,x2,y2) and string of letters (one per operation). The function signature uses `std::vector<std::array<int,4>>` plus `std::string` of letters.
//
// Now, the solution code will be a free function without main.
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <climits>

// Compute the minimum total cost after applying exactly one rectangle operation.
// grid: n rows, each string length m, letters 'a'..'z'
// rects: each element is {x1,y1,x2,y2} with 1-based inclusive coordinates (top-left, bottom-right)
// letters: string of length k, each char is the target letter for corresponding operation
// Returns the minimum possible total cost (long long).
long long optimizeTransformation(int n, int m, int k,
                                 const std::vector<std::string>& grid,
                                 const std::vector<std::array<int,4>>& rects,
                                 const std::string& letters) {
    // Build 2D prefix sums per letter (1-indexed dimensions)
    // pref[z][i][j] = number of letter z in submatrix rows 1..i, cols 1..j
    std::vector<std::vector<std::array<int,26>>> pref(n+1, std::vector<std::array<int,26>>(m+1));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int ch = grid[i-1][j-1] - 'a';
            // copy previous: pref[i][j] = pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1]
            for (int z = 0; z < 26; ++z) {
                pref[i][j][z] = pref[i-1][j][z] + pref[i][j-1][z] - pref[i-1][j-1][z];
            }
            pref[i][j][ch]++; // add current cell
        }
    }

    // Helper lambda to get count of letter z in rectangle (x1,y1)-(x2,y2) inclusive, 1-based
    auto rectCount = [&](int z, int x1, int y1, int x2, int y2) -> int {
        if (x1 > x2 || y1 > y2) return 0;
        return pref[x2][y2][z] - pref[x1-1][y2][z] - pref[x2][y1-1][z] + pref[x1-1][y1-1][z];
    };

    long long best = LLONG_MAX;
    for (int idx = 0; idx < k; ++idx) {
        int x1 = rects[idx][0], y1 = rects[idx][1];
        int x2 = rects[idx][2], y2 = rects[idx][3];
        int target = letters[idx] - 'a';
        long long cost = 0;
        for (int z = 0; z < 26; ++z) {
            int cnt = rectCount(z, x1, y1, x2, y2);
            if (cnt) {
                cost += 1LL * cnt * std::abs(target - z);
            }
        }
        if (cost < best) best = cost;
    }
    return best;
}
#include <cassert>
#include <vector>
#include <string>
#include <array>
#include <climits>

// The solution function is declared above (included here for completeness)
// We assume the function is defined in the same translation unit.

int main() {
    // Example 1: 1x1 grid, one operation covering the only cell
    {
        std::vector<std::string> grid = {"a"};
        std::vector<std::array<int,4>> rects = {{1,1,1,1}};
        std::string letters = "b";
        assert(optimizeTransformation(1,1,1,grid,rects,letters) == 1);
    }
    // Example 2: 2x2 grid, one operation covering top-left cell
    {
        std::vector<std::string> grid = {"ab", "cd"};
        std::vector<std::array<int,4>> rects = {{1,1,1,1}};
        std::string letters = "a";
        // Cell (1,1) is 'a', target 'a' cost 0, others untouched (cost 0)
        assert(optimizeTransformation(2,2,1,grid,rects,letters) == 0);
    }
    // Example 3: 2x2 grid, one operation covering all cells, target 'z'
    {
        std::vector<std::string> grid = {"ab", "cd"};
        std::vector<std::array<int,4>> rects = {{1,1,2,2}};
        std::string letters = "z";
        // costs: |25-0| + |25-1| + |25-2| + |25-3| = 25+24+23+22 = 94
        assert(optimizeTransformation(2,2,1,grid,rects,letters) == 94);
    }
    // Example 4: Multiple operations, choose the best
    {
        std::vector<std::string> grid = {"aa", "aa"};
        std::vector<std::array<int,4>> rects = {{1,1,1,1}, {2,2,2,2}};
        std::string letters = "b", "c"; // actually we need a vector? The function expects letters as string, so we use "bc"
        std::string letters_str = "bc";
        // Operation 1: cover (1,1) target 'b' -> cost |1| = 1
        // Operation 2: cover (2,2) target 'c' -> cost |2| = 2
        // Minimum is 1
        assert(optimizeTransformation(2,2,2,grid,rects,letters_str) == 1);
    }
    // Example 5: Large rectangle, ensure sum correct
    {
        std::vector<std::string> grid = {"abc", "def", "ghi"};
        std::vector<std::array<int,4>> rects = {{1,1,3,3}};
        std::string letters = "a";
        // All cells changed to 'a': costs = |0-0|+|0-1|+|0-2| + |0-3|+|0-4|+|0-5| + |0-6|+|0-7|+|0-8| = 0+1+2+3+4+5+6+7+8 = 36
        assert(optimizeTransformation(3,3,1,grid,rects,letters) == 36);
    }
    // Example 6: Edge case with k=0? But problem says k>=1, but we can test with k=1 anyway
    // Example 7: Single cell, multiple operations, same target
    {
        std::vector<std::string> grid = {"x"};
        std::vector<std::array<int,4>> rects = {{1,1,1,1}, {1,1,1,1}};
        std::string letters = "a", "b"; // use "ab"
        std::string letters_str = "ab";
        // Operation 1 target 'a' cost |0-23|=23, operation2 target 'b' cost |1-23|=22, min 22
        assert(optimizeTransformation(1,1,2,grid,rects,letters_str) == 22);
    }

    return 0;
}

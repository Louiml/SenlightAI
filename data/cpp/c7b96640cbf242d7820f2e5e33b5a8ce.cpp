Write a C++ function that, given an `n x m` grid of lowercase letters (where `n >= m`) and an `m x m` pattern grid, finds the first occurrence (top-left corner) of the pattern within the main grid. The search proceeds column by column from left to right; within each column, it checks rows from top to bottom. Use a rolling hash to precompute column-wise hashes for the pattern and row-wise hashes for the main grid to quickly filter candidate positions, then verify exact matches character-by-character to avoid hash collisions. Return the 1-based coordinates `{row, column}` of the first match, or `{-1, -1}` if no match exists. The function signature is `std::pair<int,int> findPattern(const std::vector<std::string>& grid, const std::vector<std::string>& pattern)`.

The solution uses a two-level rolling hash. First, for each row of the main grid, compute a hash of that row (using base 127 and modulus 1e9+7). Then compute prefix hashes for each row so that any contiguous segment of length `m` in a row can be hashed in O(1). For the pattern, compute hashes for each column (since the pattern is `m x m`, process each column as a string of length `m`) and also store prefix hashes for vertical segments from row 1 to row `n`. Then for each candidate top-left column `j` in the main grid (from 1 to `n-m+1`), compute the combined hash of the pattern's columns shifted vertically to match position `j`: for each row `i` of the pattern, get the horizontal hash of the main grid's row `i` segment from `j` to `j+m-1`, and combine these row hashes into a single value (again using base multiplication) to get the "vertical hash" of the main grid region starting at row 1. Compare this with the pattern's vertical hash (computed similarly from its columns). If they match, perform a character-by-character verification to eliminate false positives due to hash collisions. If verification passes, return the coordinates. The loop breaks on the first match, scanning columns in increasing order and within each column rows from top to bottom. Edge cases: if `m > n`, return `{-1,-1}`; if pattern is empty, return `{1,1}` (if grid is non-empty). Hash collisions are handled by the exact check. Time complexity: O(n*m + (n-m+1)^2 * m) in the worst case due to the verification step, but typically much faster because of hash filtering. Space complexity: O(n*m) for storing hashes.

#include <vector>
#include <string>
#include <utility>
#include <cstdint>

// Rolling hash parameters
constexpr int64_t kBase = 127;
constexpr int64_t kMod = 1000000007;

// Return first (row, col) 1-based index where pattern appears in grid.
// If not found, return {-1, -1}.
std::pair<int, int> findPattern(const std::vector<std::string>& grid,
                                const std::vector<std::string>& pattern) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0) {
        return (n > 0) ? std::make_pair(1, 1) : std::make_pair(-1, -1);
    }
    if (n < m) {
        return {-1, -1};
    }

    // Precompute powers of base up to n (max needed length)
    std::vector<int64_t> pow(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        pow[i] = (pow[i - 1] * kBase) % kMod;
    }

    // Horizontal prefix hashes for each row of grid: gridHash[row][col] = hash of grid[row][0..col-1]
    std::vector<std::vector<int64_t>> gridHash(n, std::vector<int64_t>(n + 1, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int64_t val = (j < static_cast<int>(grid[i].size())) ? grid[i][j] : 0;
            gridHash[i][j + 1] = (gridHash[i][j] * kBase + val) % kMod;
        }
    }

    // Vertical prefix hashes for pattern columns: patternColHash[col][row] = hash of pattern[0..row-1][col]
    std::vector<std::vector<int64_t>> patternColHash(m, std::vector<int64_t>(m + 1, 0));
    for (int col = 0; col < m; ++col) {
        for (int row = 0; row < m; ++row) {
            int64_t val = (row < static_cast<int>(pattern.size()) && col < static_cast<int>(pattern[row].size()))
                          ? pattern[row][col] : 0;
            patternColHash[col][row + 1] = (patternColHash[col][row] * kBase + val) % kMod;
        }
    }

    // Compute overall hash of the pattern (combining its columns vertically)
    int64_t patternHash = 0;
    for (int col = 0; col < m; ++col) {
        int64_t colHash = patternColHash[col][m]; // hash of entire column
        patternHash = (patternHash * kBase + colHash) % kMod;
    }

    // Search for each possible top-left column j (1-indexed in task description, but use 0-indexed internally)
    for (int startCol = 0; startCol <= n - m; ++startCol) {
        // Compute the combined hash for the region starting at row 0 and columns startCol..startCol+m-1
        int64_t regionHash = 0;
        for (int row = 0; row < m; ++row) {
            int64_t rowSegment = (gridHash[row][startCol + m] - gridHash[row][startCol] * pow[m] % kMod + kMod) % kMod;
            regionHash = (regionHash * kBase + rowSegment) % kMod;
        }
        if (regionHash != patternHash) {
            continue; // hash mismatch, skip
        }
        // Now check rows from top to bottom for this column
        for (int startRow = 0; startRow <= n - m; ++startRow) {
            // Compute hash of this specific candidate region
            int64_t candidateHash = 0;
            for (int rowOffset = 0; rowOffset < m; ++rowOffset) {
                int64_t rowSegment = (gridHash[startRow + rowOffset][startCol + m] - gridHash[startRow + rowOffset][startCol] * pow[m] % kMod + kMod) % kMod;
                candidateHash = (candidateHash * kBase + rowSegment) % kMod;
            }
            if (candidateHash != patternHash) {
                continue;
            }
            // Verify exactly
            bool ok = true;
            for (int i = 0; i < m && ok; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (grid[startRow + i][startCol + j] != pattern[i][j]) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) {
                return {startRow + 1, startCol + 1}; // 1-based
            }
        }
    }
    return {-1, -1};
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Assume findPattern is defined as above (include the solution code here in actual compilation)
// For brevity, we include the function in the test file by including the solution.

int main() {
    // Basic case where pattern matches at top-left
    {
        std::vector<std::string> grid = {"ab", "cd"};
        std::vector<std::string> pattern = {"ab", "cd"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(1, 1));
    }
    // Pattern appears in middle
    {
        std::vector<std::string> grid = {"xxxx", "xabx", "xcdx", "xxxx"};
        std::vector<std::string> pattern = {"ab", "cd"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(2, 2));
    }
    // Pattern does not exist
    {
        std::vector<std::string> grid = {"abc", "def", "ghi"};
        std::vector<std::string> pattern = {"zz", "zz"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(-1, -1));
    }
    // Pattern larger than grid
    {
        std::vector<std::string> grid = {"ab"};
        std::vector<std::string> pattern = {"abc"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(-1, -1));
    }
    // Pattern of size 1
    {
        std::vector<std::string> grid = {"a", "b"};
        std::vector<std::string> pattern = {"b"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(2, 1));
    }
    // Multiple occurrences, first found by column then row
    {
        std::vector<std::string> grid = {"aab", "aab", "aaa"};
        std::vector<std::string> pattern = {"a", "a"};
        auto res = findPattern(grid, pattern);
        // columns: 1 has rows 1-2, 2 has rows 1-2, 3 has rows 2-3? Actually pattern 2x1, check
        // grid 3x3, pattern 2x1 => columns 1,2,3; check column 1: rows 1-2 match
        assert(res == std::make_pair(1, 1));
    }
    // Grid has larger width than height? But n>=m guaranteed by problem? We handle n<m, but here n=m
    {
        std::vector<std::string> grid = {"abcd", "efgh", "ijkl", "mnop"};
        std::vector<std::string> pattern = {"fg", "jk"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(2, 2));
    }
    // Test with potential hash collision (very unlikely but we force exact check)
    {
        std::vector<std::string> grid = {"aa", "aa"};
        std::vector<std::string> pattern = {"ab", "aa"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(-1, -1));
    }
    // Empty pattern returns (1,1) if grid nonempty
    {
        std::vector<std::string> grid = {"x"};
        std::vector<std::string> pattern = {};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(1, 1));
    }
    // Pattern found at bottom-right corner
    {
        std::vector<std::string> grid = {"aaa", "aaa", "abb"};
        std::vector<std::string> pattern = {"bb"};
        auto res = findPattern(grid, pattern);
        assert(res == std::make_pair(3, 2));
    }
    return 0;
}

Write a standalone C++ function named `computeEdgeDensityProfile` that, given a 2D grid of positive integer run-length values (representing, for each pixel, the minimum distance to an edge in the x or y direction), computes a horizontal edge-density profile and returns it as a `std::vector<float>`. The profile must assign to each column index `i` the sum of reciprocals of all run-lengths in that column (i.e., for each row, add `1.0f / run_length` to the column's accumulator). After filling all columns, normalize the entire profile so that the sum of all elements equals `1.0f`. If the grid is empty (zero rows or zero columns), return an empty vector. If all run-lengths are zero or the total sum of reciprocals is zero, return a vector of zeros of the appropriate width. The function must be `const`-correct and avoid modifying the input. The input is a `std::vector<std::vector<int>>` where the outer vector represents rows and the inner vector represents columns; all inner vectors have the same size. The function should handle edge cases where run-length values may be zero by treating a zero as `1` (i.e., `run = max(run, 1)` before taking the reciprocal).

// The solution iterates over every cell in the 2D grid exactly once. For each cell at row `r` and column `c`, it reads the integer run-length value. If that value is zero, it replaces it with `1` to avoid division by zero. It then adds `1.0f / run` to an accumulator for column `c`. After processing all cells, the profile vector (of length equal to the number of columns) contains the raw sums. If the sum of all entries in the profile is greater than zero, each entry is divided by that total so that the profile sums to `1.0`. If the total is zero (which can only happen if the grid had no columns, or all cells were somehow non-positive with an empty grid, but note that after replacing zeros with 1, the raw sums will always be positive as long as the grid has at least one column and one row), the profile is left as zeros. Time complexity is `O(rows × columns)` because each cell is visited once. Space complexity is `O(columns)` for the profile vector, plus the input itself which is not copied. The normalization step takes `O(columns)` additional time. The function is robust to empty grids and grids with all-zero values because it clamps zero to one, so the total sum is always positive when at least one cell exists. The implementation uses `std::vector<float>` and a simple loop structure, ensuring correctness and clarity.

#include <vector>
#include <algorithm> // for std::max

// Compute the edge-density profile from a 2D grid of run-lengths.
// Each cell contributes 1.0f / max(run, 1) to its column's sum.
// The profile is normalized to sum to 1.0f.
std::vector<float> computeEdgeDensityProfile(const std::vector<std::vector<int>>& runlengths) {
    if (runlengths.empty()) {
        return {};
    }
    const size_t rows = runlengths.size();
    const size_t cols = runlengths[0].size();
    if (cols == 0) {
        return {};
    }
    
    std::vector<float> profile(cols, 0.0f);
    double total = 0.0;
    
    for (size_t r = 0; r < rows; ++r) {
        // Defensive: ensure every row has the same length as the first row.
        if (runlengths[r].size() != cols) {
            // If malformed, we could throw, but for a simple task we return empty.
            return {};
        }
        for (size_t c = 0; c < cols; ++c) {
            int run = runlengths[r][c];
            if (run < 0) {
                run = 1; // negative values should not happen, but clamp for safety
            } else if (run == 0) {
                run = 1;
            }
            float density = 1.0f / static_cast<float>(run);
            profile[c] += density;
            total += density;
        }
    }
    
    if (total > 0.0) {
        for (size_t c = 0; c < cols; ++c) {
            profile[c] /= static_cast<float>(total);
        }
    }
    
    return profile;
}

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or paste it above main).

int main() {
    // Example 1: simple 2x2 grid all run = 2.
    // Each cell contributes 0.5, so each column sum = 1.0, total = 2.0.
    // After normalization each column becomes 0.5.
    {
        std::vector<std::vector<int>> grid = {{2, 2}, {2, 2}};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.size() == 2);
        assert(std::fabs(profile[0] - 0.5f) < 1e-5);
        assert(std::fabs(profile[1] - 0.5f) < 1e-5);
    }

    // Example 2: grid with zeros, zeros are treated as 1.
    // Grid: [ [0, 4], [4, 4] ]
    // Cell (0,0) = 0 -> 1 -> density 1.0
    // Cell (0,1) = 4 -> density 0.25
    // Cell (1,0) = 4 -> density 0.25
    // Cell (1,1) = 4 -> density 0.25
    // Column sums: col0 = 1.25, col1 = 0.5, total = 1.75
    // Normalized: col0 = 1.25/1.75 = 0.7142857, col1 = 0.5/1.75 = 0.2857143
    {
        std::vector<std::vector<int>> grid = {{0, 4}, {4, 4}};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.size() == 2);
        assert(std::fabs(profile[0] - 0.7142857f) < 1e-5);
        assert(std::fabs(profile[1] - 0.2857143f) < 1e-5);
    }

    // Example 3: single row, single column.
    {
        std::vector<std::vector<int>> grid = {{5}};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.size() == 1);
        assert(std::fabs(profile[0] - 1.0f) < 1e-5);
    }

    // Example 4: all zeros -> all ones after clamping.
    // Each cell density = 1.0, so for a 2x3 grid each column sum = 2.0, total = 6.0, normalized each = 1/3.
    {
        std::vector<std::vector<int>> grid = {{0, 0, 0}, {0, 0, 0}};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.size() == 3);
        for (float v : profile) {
            assert(std::fabs(v - (1.0f/3.0f)) < 1e-5);
        }
    }

    // Example 5: empty grid -> empty profile.
    {
        std::vector<std::vector<int>> grid = {};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.empty());
    }

    // Example 6: grid with rows of length 0 -> empty profile.
    {
        std::vector<std::vector<int>> grid = {{}, {}};
        auto profile = computeEdgeDensityProfile(grid);
        assert(profile.empty());
    }

    // Example 7: profile sums to 1.0 after normalization for a larger grid.
    {
        std::vector<std::vector<int>> grid(3, std::vector<int>(4, 1));
        auto profile = computeEdgeDensityProfile(grid);
        float sum = 0.0f;
        for (float v : profile) sum += v;
        assert(std::fabs(sum - 1.0f) < 1e-5);
        // Each column has 3 cells of reciprocal 1.0, so raw sum per col = 3.0, total = 12.0, normalized = 0.25 each.
        assert(std::fabs(profile[0] - 0.25f) < 1e-5);
        assert(std::fabs(profile[3] - 0.25f) < 1e-5);
    }

    // Example 8: negative values are clamped to 1 (defensive).
    {
        std::vector<std::vector<int>> grid = {{-5, 1}, {2, -1}};
        // Treat -5 as 1, 1 as 1, 2 as 2, -1 as 1.
        // Densities: [1.0, 1.0], [0.5, 1.0] -> col sums: col0=1.5, col1=2.0, total=3.5.
        auto profile = computeEdgeDensityProfile(grid);
        assert(std::fabs(profile[0] - (1.5f/3.5f)) < 1e-5);
        assert(std::fabs(profile[1] - (2.0f/3.5f)) < 1e-5);
    }

    return 0;
}

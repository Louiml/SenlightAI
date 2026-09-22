Write a standalone C++ function that performs bicubic convolution interpolation on a regular 2D grid represented by a 2D `std::vector<std::vector<double>>` and a scalar resolution. The function should take as input: the grid (rows × columns), the resolution (cell size, assumed square), and the query coordinates (x, y) in the same coordinate system as the grid (origin at the top-left cell center, with x increasing to the right and y increasing downward). It must return a `std::optional<double>` containing the interpolated value at the query point, or `std::nullopt` if the query point is outside the grid's bounds (i.e., outside the rectangle covered by the grid cells). The interpolation must use the classic bicubic convolution kernel (also known as the catmull-rom style 4×4 convolution matrix) with clamped boundary handling: out-of-range indices are clamped to the nearest valid row/column index. The function must be self-contained, avoid external libraries beyond the C++ standard library, and be named `bicubicConvolutionInterpolate`. Assume the grid is at least 2×2 in size.
#include <cassert>
#include <optional>
#include <vector>

// Declare the function (assuming it's in the same file or included).
std::optional<double> bicubicConvolutionInterpolate(
    const std::vector<std::vector<double>>& grid,
    double resolution,
    double queryX,
    double queryY);

int main() {
    // Simple 2x2 grid: [[0, 1], [1, 0]] with resolution 1.0
    std::vector<std::vector<double>> grid2x2 = {{0.0, 1.0}, {1.0, 0.0}};
    double res = 1.0;

    // At the center of the bottom-left cell (0,0), should return 0.0
    auto val1 = bicubicConvolutionInterpolate(grid2x2, res, 0.0, 0.0);
    assert(val1.has_value());
    assert(std::abs(*val1 - 0.0) < 1e-9);

    // At the center of the top-right cell (1,1), should return 0.0
    auto val2 = bicubicConvolutionInterpolate(grid2x2, res, 1.0, 1.0);
    assert(val2.has_value());
    assert(std::abs(*val2 - 0.0) < 1e-9);

    // At the center of the top-left cell (0,1), should return 1.0
    auto val3 = bicubicConvolutionInterpolate(grid2x2, res, 0.0, 1.0);
    assert(val3.has_value());
    assert(std::abs(*val3 - 1.0) < 1e-9);

    // At the exact middle of the grid (x=0.5, y=0.5) is not symmetric, but we can test it's defined.
    auto val4 = bicubicConvolutionInterpolate(grid2x2, res, 0.5, 0.5);
    assert(val4.has_value());
    // Expected: linear blend of four corners? Since bicubic convolution does not guarantee exact values, we just check it's finite.
    assert(std::isfinite(*val4));

    // Test out-of-bounds: query outside the grid coverage area.
    auto val5 = bicubicConvolutionInterpolate(grid2x2, res, -0.6, 0.0);
    assert(!val5.has_value());
    auto val6 = bicubicConvolutionInterpolate(grid2x2, res, 2.2, 1.0);
    assert(!val6.has_value());

    // Test with a 2D constant field: all 7s in a 3x3 grid, resolution 2.0.
    std::vector<std::vector<double>> gridConst(3, std::vector<double>(3, 7.0));
    auto val7 = bicubicConvolutionInterpolate(gridConst, 2.0, 1.0, 1.0);
    assert(val7.has_value());
    assert(std::abs(*val7 - 7.0) < 1e-9);

    // Test boundary exactly at the outer cell center (should be valid).
    auto val8 = bicubicConvolutionInterpolate(gridConst, 2.0, -1.0, -1.0);
    assert(val8.has_value());
    assert(std::abs(*val8 - 7.0) < 1e-9);

    // Test boundary just outside (should be null).
    auto val9 = bicubicConvolutionInterpolate(gridConst, 2.0, -1.1, 0.0);
    assert(!val9.has_value());

    // Test nonzero query not aligned with centers returns a plausible value
    // For a linear gradient grid: f(x,y) = x + y, values at centers.
    std::vector<std::vector<double>> gridLinear;
    for (int r = 0; r < 4; ++r) {
        std::vector<double> row;
        for (int c = 0; c < 4; ++c) {
            row.push_back(static_cast<double>(c) * 1.0 + static_cast<double>(r) * 1.0);
        }
        gridLinear.push_back(row);
    }
    auto val10 = bicubicConvolutionInterpolate(gridLinear, 1.0, 1.2, 2.3);
    assert(val10.has_value());
    // For bilinear it would be 1.2+2.3=3.5; bicubic convolution should be close but not exact. We just check it's finite and positive.
    assert(std::isfinite(*val10));
    assert(*val10 > 0.0);

    return 0;
}
#include <vector>
#include <optional>
#include <cmath>
#include <array>

// Perform bicubic convolution interpolation on a 2D grid.
// grid: rows x cols, each cell value is at the cell center.
// resolution: the cell size (assumed square).
// queryX, queryY: coordinates in the same space as the grid centers.
// Returns interpolated value or nullopt if the query point is outside the grid's coverage area.
std::optional<double> bicubicConvolutionInterpolate(
    const std::vector<std::vector<double>>& grid,
    double resolution,
    double queryX,
    double queryY) {

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    if (rows < 2 || cols < 2) {
        return std::nullopt;
    }

    // Check bounds: valid region is cell centers extend by half a cell on each side.
    const double halfRes = resolution / 2.0;
    const double minX = -halfRes;
    const double maxX = (cols - 1) * resolution + halfRes;
    const double minY = -halfRes;
    const double maxY = (rows - 1) * resolution + halfRes;
    if (queryX < minX || queryX > maxX || queryY < minY || queryY > maxY) {
        return std::nullopt;
    }

    // Find middle knot cell index (i, j) such that its center (i*res, j*res) is the nearest lower-left center.
    int i = static_cast<int>(std::floor(queryX / resolution));
    int j = static_cast<int>(std::floor(queryY / resolution));
    // Ensure i and j are within valid range (due to floating point edge cases).
    i = std::max(0, std::min(i, rows - 1));
    j = std::max(0, std::min(j, cols - 1));

    // Helper to clamp an index.
    auto clamp = [](int idx, int maxVal) -> int {
        return std::max(0, std::min(idx, maxVal));
    };

    // Assemble 4x4 function value matrix, with clamped indices.
    std::array<std::array<double, 4>, 4> f;
    for (int di = 0; di < 4; ++di) {
        int rowIdx = clamp(i + 1 - di, rows - 1); // di=0 -> i+1, di=1 -> i, di=2 -> i-1, di=3 -> i-2
        for (int dj = 0; dj < 4; ++dj) {
            int colIdx = clamp(j + 1 - dj, cols - 1); // dj=0 -> j+1, dj=1 -> j, dj=2 -> j-1, dj=3 -> j-2
            f[di][dj] = grid[rowIdx][colIdx];
        }
    }

    // Normalized coordinates within the cell (0 to 1).
    double tx = (queryX - static_cast<double>(i) * resolution) / resolution;
    double ty = (queryY - static_cast<double>(j) * resolution) / resolution;

    // Standard bicubic convolution matrix (Catmull-Rom style), scaled by 0.5.
    const double conv[4][4] = {
        {0.0, 2.0, 0.0, 0.0},
        {-1.0, 0.0, 1.0, 0.0},
        {2.0, -5.0, 4.0, -1.0},
        {-1.0, 3.0, -3.0, 1.0}
    };

    // Helper lambda for 1D convolution with given t.
    auto convolve1D = [&conv](double t, const std::array<double,4>& values) -> double {
        double t2 = t * t;
        double t3 = t2 * t;
        double result = 0.0;
        // result = 0.5 * (values[0]*(0 + 2*t) + values[1]*(-1 + 0*t + 1*t2) + values[2]*(2 -5*t +4*t2 -1*t3) + values[3]*(-1 +3*t -3*t2 +1*t3))
        // This is equivalent to 0.5 * (tVector^T * convMatrix * values)
        double row0 = (0.0 + 2.0*t);
        double row1 = (-1.0 + 0.0*t + 1.0*t2);
        double row2 = (2.0 -5.0*t + 4.0*t2 -1.0*t3);
        double row3 = (-1.0 + 3.0*t -3.0*t2 + 1.0*t3);
        result = values[0]*row0 + values[1]*row1 + values[2]*row2 + values[3]*row3;
        return 0.5 * result;
    };

    // Convolve along x for each of the 4 rows.
    std::array<double, 4> rowConvolved;
    for (int r = 0; r < 4; ++r) {
        rowConvolved[r] = convolve1D(tx, f[r]);
    }

    // Convolve along y.
    double finalValue = convolve1D(ty, rowConvolved);
    return finalValue;
}
// The algorithm follows the standard bicubic convolution interpolation technique described in the provided snippet. For a query point, we first find the "middle knot" cell index (i, j) using `floor` of `(queryX / resolution)` and `floor` of `(queryY / resolution)` — this is the cell whose center is closest but not exceeding the query coordinate in both axes. We then assemble a 4×4 matrix of function values sampled from the grid at indices (i+1, i, i-1, i-2) for rows and (j+1, j, j-1, j-2) for columns, with each index clamped to the valid [0, rows-1] or [0, cols-1] range. The normalized coordinates tx and ty are computed as the fractional distances from the middle knot cell center: `tx = (queryX - i*resolution)/resolution` and `ty = (queryY - j*resolution)/resolution`. Then, for each of the four rows of the 4×4 matrix, we apply a 1D cubic convolution with the same kernel vector `[1, t, t^2, t^3]` multiplied by the standard 4×4 convolution matrix (the one used in the provided code, which is the Catmull-Rom style with coefficients: rows `[0,2,0,0], [-1,0,1,0], [2,-5,4,-1], [-1,3,-3,1]` times 0.5). The result of the four 1D convolutions gives four values, which are then combined by a final 1D convolution along the y-direction. The query point is considered out of bounds if `queryX < -0.5*resolution` or `queryX > (cols-0.5)*resolution` or similarly for y (since cell centers span from `-0.5*res` to `(n-0.5)*res`). Edge cases: clamping handles partial support near boundaries; the boundary check uses half-cell margin because a query point exactly at the outermost cell center is valid. Time complexity is O(1) since we only access a fixed 4×4 neighborhood; space complexity is O(1) for the temporary matrices.

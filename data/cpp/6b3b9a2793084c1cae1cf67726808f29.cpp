// Write a C++ function that, given an integer width and height (both positive, at least 1), returns a `std::vector<std::vector<int>>` representing a synthetic image where each pixel value is computed using a simple Mandelbrot-like iteration. For each pixel at coordinate (x, y), normalize x to range [-2.0, 1.0] and y to range [-1.5, 1.5] based on width and height, then iterate the recurrence Z = Z² + C (where Z starts at 0, C is the normalized complex coordinate) up to 100 iterations. If the magnitude of Z exceeds 2.0, stop early and record the number of iterations performed; otherwise record 0. The output value for that pixel is the recorded iteration count (0–100) as an integer. The function must be `const`-correct, use no external libraries beyond the standard library, and be named `mandelbrotIterationCounts`.
The solution iterates over every pixel in row-major order. For each pixel, we map the integer coordinates to a normalized complex plane: `x_norm = (x / (width - 1)) * 3.0 - 2.0` and `y_norm = (y / (height - 1)) * 3.0 - 1.5` (ensuring the full range is used even for width=1). Then we run the Mandelbrot iteration: initialize `zx = 0.0`, `zy = 0.0`. For up to 100 iterations, update `zx_next = zx*zx - zy*zy + x_norm`, `zy_next = 2*zx*zy + y_norm`, then assign `zx = zx_next`, `zy = zy_next`. If `zx*zx + zy*zy > 4.0`, break and store the current iteration index (starting from 1). If the loop completes all 100 iterations without escaping, store 0. Important edge cases: width or height of 1 (avoids division by zero), coordinates at the exact boundary (normalization still yields valid finite values), and potential precision issues with `double`—we use `double` for all intermediate calculations. Time complexity is O(width * height * maxIterations) with maxIterations=100, so O(100 * W * H) effectively. Space complexity is O(W * H) for the returned vector.
#include <vector>
#include <cmath>

// Compute Mandelbrot iteration counts for a synthetic image of given dimensions.
// Each pixel maps to a complex coordinate, and the return value is the iteration count
// (0 if point is in the set, otherwise 1..100).
std::vector<std::vector<int>> mandelbrotIterationCounts(int width, int height) {
    const int MAX_ITER = 100;
    // Handle zero/negative dimensions defensively, though caller should ensure positive.
    if (width <= 0 || height <= 0) return {};

    std::vector<std::vector<int>> result(height, std::vector<int>(width, 0));

    for (int y = 0; y < height; ++y) {
        // Normalize y so that y=0 maps to -1.5 and y=height-1 maps to 1.5.
        double y_norm = (height == 1) ? -1.5 : -1.5 + (3.0 * y) / (height - 1);
        for (int x = 0; x < width; ++x) {
            // Normalize x so that x=0 maps to -2.0 and x=width-1 maps to 1.0.
            double x_norm = (width == 1) ? -2.0 : -2.0 + (3.0 * x) / (width - 1);

            double zx = 0.0;
            double zy = 0.0;
            int iter = 0;
            for (; iter < MAX_ITER; ++iter) {
                double zx2 = zx * zx;
                double zy2 = zy * zy;
                if (zx2 + zy2 > 4.0) break;
                double zx_next = zx2 - zy2 + x_norm;
                double zy_next = 2.0 * zx * zy + y_norm;
                zx = zx_next;
                zy = zy_next;
            }
            if (iter == MAX_ITER) {
                result[y][x] = 0; // Point is in the set (or never escaped).
            } else {
                result[y][x] = iter + 1; // iter is the iteration where it escaped, store count 1..100.
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Simple 1x1 test: center of the set should be 0 iterations.
    auto img1 = mandelbrotIterationCounts(1, 1);
    assert(img1.size() == 1);
    assert(img1[0].size() == 1);
    // For a 1x1 image, x_norm = -2.0, y_norm = -1.5, which is outside, so >0 iterations.
    assert(img1[0][0] > 0 && img1[0][0] <= 100);

    // Test a 2x2: exact bounds produce valid values.
    auto img2 = mandelbrotIterationCounts(2, 2);
    assert(img2.size() == 2);
    for (int y = 0; y < 2; ++y) {
        assert(img2[y].size() == 2);
        for (int x = 0; x < 2; ++x) {
            assert(img2[y][x] >= 0 && img2[y][x] <= 100);
        }
    }

    // Test a 3x3: all pixels are valid integers.
    auto img3 = mandelbrotIterationCounts(3, 3);
    for (const auto& row : img3) {
        for (int val : row) {
            assert(val >= 0 && val <= 100);
        }
    }

    // Test known point: (width=200,height=200) and coordinate (100,100) should be inside set.
    // Normalize: x=100/199*3-2 ≈ -0.4925, y=100/199*3-1.5 ≈ 0.0075. This is near the cardioid, likely inside.
    auto img_large = mandelbrotIterationCounts(200, 200);
    // The center of a 200x200 image: x=100, y=100.
    int centerVal = img_large[100][100];
    assert(centerVal == 0); // Should be in the set (escapes only after many iterations).

    // Test a point known to escape quickly: (width=200,height=200), coordinate x=0,y=0 → (-2.0,-1.5) escapes after a few iterations.
    int cornerVal = img_large[0][0];
    assert(cornerVal > 0 && cornerVal <= 100);

    // Test that all values in the whole grid are within [0,100].
    for (const auto& row : img_large) {
        for (int val : row) {
            assert(val >= 0 && val <= 100);
        }
    }

    // Test asymmetric dimensions to ensure normalization works for non-square.
    auto img_rect = mandelbrotIterationCounts(4, 7);
    assert(img_rect.size() == 7);
    assert(img_rect[0].size() == 4);

    return 0;
}

Write a C++ function `countMandelbrotPixels` that takes three integer arguments: image height, image width (both positive), and a maximum iteration count (positive), and returns the number of points in the complex plane that belong to the Mandelbrot set (i.e., do not escape to infinity within the given iteration limit). The image maps the complex plane from x = -2.5 to 1.0 and y = -1.0 to 1.0, with pixels evenly spaced. For each pixel (row r, column c), the complex coordinate is: real = -2.5 + c * (3.5 / width), imag = 1.0 - r * (2.0 / height). A point is in the Mandelbrot set if, starting with z = 0, after repeatedly applying z = z^2 + c for up to max_iterations, the squared magnitude of z remains ≤ 4. Count only those points that never exceed this bound. Use `double` for all floating-point arithmetic. Handle edge cases where the image dimensions are zero or negative by returning 0, and ensure no division by zero occurs.
// The algorithm iterates over every pixel in the height × width grid. For each pixel, it computes the corresponding complex coordinate (real and imaginary parts) using linear interpolation across the specified bounding box. Then it simulates the Mandelbrot recurrence: start z_real = 0.0, z_imag = 0.0, and for up to max_iterations steps update: next_real = z_real² - z_imag² + c_real, next_imag = 2*z_real*z_imag + c_imag; then set z_real = next_real, z_imag = next_imag. After each update, check if z_real² + z_imag² > 4.0. If it exceeds, the point escapes and is not counted; break out of the inner loop. If the loop completes without exceeding, the pixel is inside the set and counted. Important edge cases: (1) If width or height ≤ 0, return 0 to avoid division by zero; (2) for large max_iterations, the computation is slow but correct; (3) floating-point precision is adequate for typical sizes; (4) the complex coordinate mapping ensures the full region is covered. Time complexity is O(height × width × max_iterations) because each pixel runs up to max_iterations steps. Space complexity is O(1) beyond the input parameters, as only a few local variables are used. The function is const-correct and self-contained.
#include <vector>
#include <cmath>

// Counts the number of pixels inside the Mandelbrot set for given image dimensions.
// Returns 0 if either dimension is non-positive.
int countMandelbrotPixels(int height, int width, int max_iterations) {
    if (height <= 0 || width <= 0 || max_iterations <= 0) return 0;

    const double x_min = -2.5;
    const double x_max = 1.0;
    const double y_min = -1.0;
    const double y_max = 1.0;

    const double x_step = (x_max - x_min) / width;
    const double y_step = (y_max - y_min) / height;

    int inside_count = 0;

    for (int r = 0; r < height; ++r) {
        double c_imag = y_max - r * y_step;  // top to bottom
        for (int c = 0; c < width; ++c) {
            double c_real = x_min + c * x_step;

            double z_real = 0.0;
            double z_imag = 0.0;
            bool escaped = false;

            for (int iter = 0; iter < max_iterations; ++iter) {
                double z_real_sq = z_real * z_real;
                double z_imag_sq = z_imag * z_imag;
                if (z_real_sq + z_imag_sq > 4.0) {
                    escaped = true;
                    break;
                }
                double next_real = z_real_sq - z_imag_sq + c_real;
                double next_imag = 2.0 * z_real * z_imag + c_imag;
                z_real = next_real;
                z_imag = next_imag;
            }

            if (!escaped) {
                ++inside_count;
            }
        }
    }

    return inside_count;
}
#include <cassert>
#include <cstdlib>

// Forward declaration of the solution function.
int countMandelbrotPixels(int height, int width, int max_iterations);

int main() {
    // Zero or negative dimensions should return 0.
    assert(countMandelbrotPixels(0, 100, 100) == 0);
    assert(countMandelbrotPixels(100, 0, 100) == 0);
    assert(countMandelbrotPixels(0, 0, 100) == 0);
    assert(countMandelbrotPixels(100, 100, 0) == 0);

    // Single pixel at c = 0 (inside) with enough iterations should count 1.
    assert(countMandelbrotPixels(1, 1, 100) == 1);

    // A 2x2 image: corners are (-2.5, 1.0), (-2.5, -1.0), (1.0, 1.0), (1.0, -1.0)
    // All far outside, so with any iterations, count should be 0.
    assert(countMandelbrotPixels(2, 2, 50) == 0);

    // A 3x3 with low iterations: only center pixel (c=0) might be inside.
    // But with low iterations, even center may escape; let's test high iterations.
    // Center pixel for odd dims is at (row 1, col 1) => c = 0, always inside.
    assert(countMandelbrotPixels(3, 3, 1000) == 1);

    // Check that increasing width increases count, as more points near inside region.
    int count_small = countMandelbrotPixels(50, 50, 100);
    int count_large = countMandelbrotPixels(50, 100, 100);
    assert(count_large >= count_small);

    // For very low max_iterations, some boundary points may be incorrectly classified as inside,
    // but at least count should be positive for a reasonable image size.
    assert(countMandelbrotPixels(100, 100, 10) > 0);

    return 0;
}

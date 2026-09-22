// Write a standalone C++ function named `generateMandelbrotAscii` that takes an output stream reference, width, height, real-axis bounds (`x0`, `x1`), imaginary-axis bounds (`y0`, `y1`), and a maximum iteration count. The function must print an ASCII representation of the Mandelbrot set to the given stream. For each pixel, compute the complex number `c = (x0 + (x / width) * (x1 - x0)) + i*(y0 + (y / height) * (y1 - y0))`, then iterate `z = z*z + c` starting from `z = 0` until `|z| > 2` or the iteration limit is reached. If the iteration count equals the limit, print `'#'`; otherwise print `'.'`. Each row must end with a newline. The function must handle zero or negative width/height gracefully by doing nothing (no output). Use `const` for all parameters that are not modified. The function must be self-contained with only standard headers (`<iostream>`, `<complex>`).

// The core algorithm is the standard escape-time method for the Mandelbrot set. For every pixel coordinate `(x, y)`, we map the pixel index to the complex plane using linear interpolation. The iteration loop repeatedly applies `z = z*z + c` and checks if the magnitude exceeds 2.0 (the known escape radius). If the loop completes without exceeding the radius within the maximum iterations, the point is considered inside the set. The output uses a simple binary representation: `'#'` for inside, `'.'` for outside. Edge cases include: (1) width or height <= 0 — return immediately to avoid division by zero or infinite loops; (2) iteration count <= 0 — treat as no iterations, so every point is outside (printed as `'.'`); (3) very large dimensions may cause slow output, but the algorithm remains linear in `width * height * iter`. Time complexity is `O(width * height * iter)` in the worst case, and space complexity is `O(1)` beyond the output stream. The function must be `const`-correct, meaning all parameters except the stream are passed by value (or `const` reference if needed), and the function itself does not modify any passed-in data.

#include <iostream>
#include <complex>

// Print an ASCII Mandelbrot set to the given output stream.
// Parameters: os - output stream, width/height - image dimensions in characters,
// x0/x1/y0/y1 - complex plane bounds, iter - maximum iterations per point.
void generateMandelbrotAscii(std::ostream& os, const int width, const int height,
                             const double x0, const double x1,
                             const double y0, const double y1,
                             const int iter) {
    // Guard: if dimensions are non-positive, do nothing.
    if (width <= 0 || height <= 0) {
        return;
    }

    // If iteration limit is non-positive, every point escapes immediately.
    const int maxIter = (iter > 0) ? iter : 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Map pixel to complex coordinate
            const double real = x0 + (static_cast<double>(x) / width) * (x1 - x0);
            const double imag = y0 + (static_cast<double>(y) / height) * (y1 - y0);
            const std::complex<double> c(real, imag);

            // Mandelbrot iteration
            std::complex<double> z(0.0, 0.0);
            int n = 0;
            while (std::abs(z) <= 2.0 && n < maxIter) {
                z = z * z + c;
                ++n;
            }

            // Output character
            os << (n == maxIter ? '#' : '.');
        }
        os << '\n';
    }
}

#include <cassert>
#include <sstream>

int main() {
    // Test case 1: Single iteration, all points escape -> all dots
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 2, 1, -2.0, 1.0, -1.0, 1.0, 1);
        assert(oss.str() == "..\n");
    }

    // Test case 2: Large iteration, known inside point at c=0 -> '#' at center-ish
    {
        std::ostringstream oss;
        // Use width=3, height=1, range [-1,1] -> middle x maps to 0
        generateMandelbrotAscii(oss, 3, 1, -1.0, 1.0, -1.0, 1.0, 1000);
        // x=0 -> real=-1, x=1 -> real=0, x=2 -> real=1
        // c=0 is inside, others outside
        assert(oss.str() == ".#.\n");
    }

    // Test case 3: Zero width -> no output
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 0, 10, -2.0, 1.0, -1.0, 1.0, 100);
        assert(oss.str().empty());
    }

    // Test case 4: Negative height -> no output
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 10, -1, -2.0, 1.0, -1.0, 1.0, 100);
        assert(oss.str().empty());
    }

    // Test case 5: Zero iteration -> all dots
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 2, 1, -2.0, 1.0, -1.0, 1.0, 0);
        assert(oss.str() == "..\n");
    }

    // Test case 6: Explicit escaping point: c=2 -> one iteration
    {
        std::ostringstream oss;
        // width=1, height=1, x0=x1=2, y0=y1=0 -> exactly c=2
        generateMandelbrotAscii(oss, 1, 1, 2.0, 2.0, 0.0, 0.0, 10);
        assert(oss.str() == ".\n");
    }

    // Test case 7: Explicit inside point: c=0 with many iterations -> '#'
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 1, 1, 0.0, 0.0, 0.0, 0.0, 100);
        assert(oss.str() == "#\n");
    }

    // Test case 8: Check row structure with multiple rows
    {
        std::ostringstream oss;
        generateMandelbrotAscii(oss, 2, 2, -2.0, 1.0, -1.0, 1.0, 1);
        // All escape -> all dots, 2 rows of 2 dots
        assert(oss.str() == "..\n..\n");
    }

    return 0;
}

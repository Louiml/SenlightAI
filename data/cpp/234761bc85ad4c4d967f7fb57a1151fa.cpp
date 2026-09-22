// Given two grayscale images represented as 2D grids of floating-point pixel intensities, write a C++ function that computes the horizontal and vertical displacement fields (optical flow) between them using a simplified Horn-Schunck-like method. The function should take the first image, second image, a smoothing weight (alpha), and the number of iterations as inputs, and return two displacement matrices (u for horizontal motion, v for vertical motion) of the same dimensions as the input images. The algorithm should estimate per-pixel displacement by minimizing a combined brightness constancy and smoothness objective, using iterative relaxation. Displacements should be zero at the image boundaries, and the function must handle rectangular images of arbitrary dimensions. Assume both input images have identical dimensions and contain only valid (finite) pixel values.

// The solution uses an iterative Gauss-Seidel relaxation approach inspired by the Horn-Schunck method. First, compute the spatial gradients (Ix, Iy) using central differences (with forward/backward differences at boundaries) and the temporal gradient (It) as the difference between the second and first images. Then, for each iteration, update the displacement fields u and v at each interior pixel using the locally averaged neighboring displacements and the brightness constancy error: 
// u_new = u_avg - (Ix * (Ix * u_avg + Iy * v_avg + It)) / (alpha^2 + Ix^2 + Iy^2)
// v_new = v_avg - (Iy * (Ix * u_avg + Iy * v_avg + It)) / (alpha^2 + Ix^2 + Iy^2)
// where u_avg and v_avg are the averages of the four-connected neighbor displacements (left, right, top, bottom), treating boundary pixels as having zero displacement or using only existing neighbors. This is a standard fixed-point iteration that converges to a smooth flow field. Edge cases include division by near-zero denominators (guard by adding a small epsilon), and ensuring that boundary pixels retain zero displacement (either by not updating them or by clamping). Time complexity is O(iterations × width × height) for the iterative updates, with O(width × height) auxiliary space for the displacement matrices and gradient arrays. The algorithm is straightforward to implement without external libraries, using std::vector for storage.

#include <vector>
#include <cmath>
#include <algorithm>

/**
 * Compute optical flow between two grayscale images using a Horn-Schunck-style
 * iterative relaxation method.
 *
 * @param im1 First image (width x height), row-major.
 * @param im2 Second image (width x height), same dimensions as im1.
 * @param alpha Smoothness weight (higher = smoother flow).
 * @param iterations Number of relaxation iterations to perform.
 * @param u Output horizontal displacement field (width x height).
 * @param v Output vertical displacement field (width x height).
 */
void computeOpticalFlow(
    const std::vector<std::vector<double>>& im1,
    const std::vector<std::vector<double>>& im2,
    double alpha,
    int iterations,
    std::vector<std::vector<double>>& u,
    std::vector<std::vector<double>>& v)
{
    const int height = static_cast<int>(im1.size());
    const int width = (height > 0) ? static_cast<int>(im1[0].size()) : 0;
    if (width == 0 || height == 0) return;

    // Initialize displacement fields to zero.
    u.assign(height, std::vector<double>(width, 0.0));
    v.assign(height, std::vector<double>(width, 0.0));

    // Compute image gradients: Ix, Iy, It.
    std::vector<std::vector<double>> Ix(height, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> Iy(height, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> It(height, std::vector<double>(width, 0.0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Temporal gradient: difference between images.
            It[y][x] = im2[y][x] - im1[y][x];

            // Horizontal gradient: central differences, one-sided at borders.
            if (x == 0) {
                Ix[y][x] = im1[y][1] - im1[y][0];
            } else if (x == width - 1) {
                Ix[y][x] = im1[y][x] - im1[y][x - 1];
            } else {
                Ix[y][x] = 0.5 * (im1[y][x + 1] - im1[y][x - 1]);
            }

            // Vertical gradient: central differences, one-sided at borders.
            if (y == 0) {
                Iy[y][x] = im1[1][x] - im1[0][x];
            } else if (y == height - 1) {
                Iy[y][x] = im1[y][x] - im1[y - 1][x];
            } else {
                Iy[y][x] = 0.5 * (im1[y + 1][x] - im1[y - 1][x]);
            }
        }
    }

    const double alpha2 = alpha * alpha;
    const double eps = 1e-10; // To avoid division by zero.

    // Iterative relaxation (Gauss-Seidel style).
    for (int iter = 0; iter < iterations; ++iter) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Only update interior pixels; boundary pixels remain zero displacement.
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                    continue;
                }

                // Average displacement of four-connected neighbors.
                double u_avg = 0.25 * (u[y][x - 1] + u[y][x + 1] + u[y - 1][x] + u[y + 1][x]);
                double v_avg = 0.25 * (v[y][x - 1] + v[y][x + 1] + v[y - 1][x] + v[y + 1][x]);

                // Brightness constancy error at current estimate.
                double rho = Ix[y][x] * u_avg + Iy[y][x] * v_avg + It[y][x];
                double denom = alpha2 + Ix[y][x] * Ix[y][x] + Iy[y][x] * Iy[y][x] + eps;

                // Update displacement.
                u[y][x] = u_avg - Ix[y][x] * rho / denom;
                v[y][x] = v_avg - Iy[y][x] * rho / denom;
            }
        }
    }
}

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Test 1: Static images (no motion) -> zero flow.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 2.0, 3.0},
                                                {4.0, 5.0, 6.0},
                                                {7.0, 8.0, 9.0}};
        std::vector<std::vector<double>> im2 = im1;
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 1.0, 10, u, v);
        for (size_t y = 0; y < u.size(); ++y)
            for (size_t x = 0; x < u[y].size(); ++x) {
                assert(std::abs(u[y][x]) < 1e-6);
                assert(std::abs(v[y][x]) < 1e-6);
            }
    }

    // Test 2: Pure horizontal translation by 1 pixel.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 2.0, 3.0},
                                                {1.0, 2.0, 3.0},
                                                {1.0, 2.0, 3.0}};
        std::vector<std::vector<double>> im2 = {{2.0, 3.0, 4.0},
                                                {2.0, 3.0, 4.0},
                                                {2.0, 3.0, 4.0}};
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 0.01, 100, u, v);
        // Interior pixels should have positive horizontal displacement.
        assert(u[1][1] > 0.5);
        assert(std::abs(v[1][1]) < 0.5);
        // Boundary pixels must remain zero.
        assert(u[0][0] == 0.0 && v[0][0] == 0.0);
    }

    // Test 3: Pure vertical translation by 1 pixel.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 1.0, 1.0},
                                                {2.0, 2.0, 2.0},
                                                {3.0, 3.0, 3.0}};
        std::vector<std::vector<double>> im2 = {{2.0, 2.0, 2.0},
                                                {3.0, 3.0, 3.0},
                                                {4.0, 4.0, 4.0}};
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 0.01, 100, u, v);
        assert(v[1][1] > 0.5);
        assert(std::abs(u[1][1]) < 0.5);
    }

    // Test 4: Single-pixel image (no interior) -> all zero.
    {
        std::vector<std::vector<double>> im1 = {{5.0}};
        std::vector<std::vector<double>> im2 = {{5.0}};
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 1.0, 5, u, v);
        assert(u.size() == 1 && u[0].size() == 1);
        assert(u[0][0] == 0.0 && v[0][0] == 0.0);
    }

    // Test 5: Strong smoothness (large alpha) yields near-zero flow even with noise.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 1.0, 1.0},
                                                {1.0, 1.0, 1.0},
                                                {1.0, 1.0, 1.0}};
        std::vector<std::vector<double>> im2 = {{1.0, 2.0, 1.0},
                                                {1.0, 2.0, 1.0},
                                                {1.0, 2.0, 1.0}};
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 100.0, 50, u, v);
        // With high alpha, flow should be tiny.
        for (size_t y = 0; y < u.size(); ++y)
            for (size_t x = 0; x < u[y].size(); ++x) {
                assert(std::abs(u[y][x]) < 0.1);
                assert(std::abs(v[y][x]) < 0.1);
            }
    }

    // Test 6: Non-square image dimensions.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 2.0, 3.0, 4.0},
                                                {2.0, 3.0, 4.0, 5.0}};
        std::vector<std::vector<double>> im2 = im1;
        std::vector<std::vector<double>> u, v;
        computeOpticalFlow(im1, im2, 1.0, 5, u, v);
        assert(u.size() == 2 && u[0].size() == 4);
        for (size_t y = 0; y < u.size(); ++y)
            for (size_t x = 0; x < u[y].size(); ++x)
                assert(std::abs(u[y][x]) < 1e-6);
    }

    // Test 7: Convergence with more iterations reduces error.
    {
        std::vector<std::vector<double>> im1 = {{1.0, 2.0, 1.0},
                                                {2.0, 3.0, 2.0},
                                                {1.0, 2.0, 1.0}};
        std::vector<std::vector<double>> im2 = {{2.0, 3.0, 2.0},
                                                {3.0, 4.0, 3.0},
                                                {2.0, 3.0, 2.0}};
        std::vector<std::vector<double>> u1, v1, u2, v2;
        computeOpticalFlow(im1, im2, 0.5, 10, u1, v1);
        computeOpticalFlow(im1, im2, 0.5, 200, u2, v2);
        // More iterations should give a more refined (usually larger) displacement.
        assert(std::abs(u2[1][1]) >= std::abs(u1[1][1]) - 0.1);
    }

    return 0;
}

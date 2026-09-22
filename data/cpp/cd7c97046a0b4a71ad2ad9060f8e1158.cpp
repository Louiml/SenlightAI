// Write a C++ function `klt_track_point` that implements one iteration of the Lucas-Kanade optical flow update for a single point, given two grayscale image patches (old and new), spatial gradient values, and the accumulated spatial gradient matrix. The function should take the old patch values, new patch values, the horizontal gradient patch values, the vertical gradient patch values, the spatial gradient matrix components (A11, A12, A22 as floats), and the determinant of that matrix as inputs. It should compute the temporal gradient vector b (from the difference between new and old patches weighted by the gradients), then compute the motion vector (delta_x, delta_y) as A^{-1} * (-b). Return both delta values in a `std::pair<float, float>`. The patches are provided as `std::vector<int>` of equal length, where each element is an already scaled (fixed-point) pixel value. The gradient matrix values are float (already scaled to normal floating point), and the determinant is provided (must be non-zero). Use the formula: `b1 = sum((new[i] - old[i]) * gx[i])`, `b2 = sum((new[i] - old[i]) * gy[i])`, then `delta_x = (A12 * b2 - A22 * b1) / determinant`, `delta_y = (A12 * b1 - A11 * b2) / determinant`. Handle the case where the determinant is zero or negative by returning `(0.0f, 0.0f)`. No other input validation is required; assume all vectors are the same size.

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Declaration of the function under test (as provided in the solution).
std::pair<float, float> klt_track_point(
    const std::vector<int>& old_patch,
    const std::vector<int>& new_patch,
    const std::vector<int>& gx_patch,
    const std::vector<int>& gy_patch,
    float A11,
    float A12,
    float A22,
    float determinant);

int main() {
    // Test 1: Simple uniform gradient with zero temporal difference -> no motion.
    {
        std::vector<int> old_patch = {10, 20, 30, 40};
        std::vector<int> new_patch = {10, 20, 30, 40};  // no change
        std::vector<int> gx_patch  = {1, 1, 1, 1};
        std::vector<int> gy_patch  = {0, 0, 0, 0};
        float A11 = 4.0f, A12 = 0.0f, A22 = 0.0f;
        float det = A11 * A22 - A12 * A12;  // 0, but we'll pass a positive det for test
        // Override det to a valid positive value to test no motion
        det = 1.0f;
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, A11, A12, A22, det);
        assert(std::fabs(result.first - 0.0f) < 1e-5f);
        assert(std::fabs(result.second - 0.0f) < 1e-5f);
    }

    // Test 2: Determinant zero -> returns zero motion.
    {
        std::vector<int> old_patch = {1, 2, 3, 4};
        std::vector<int> new_patch = {2, 3, 4, 5};
        std::vector<int> gx_patch  = {1, 1, 1, 1};
        std::vector<int> gy_patch  = {1, 1, 1, 1};
        float A11 = 4.0f, A12 = 4.0f, A22 = 4.0f;
        float det = A11 * A22 - A12 * A12;  // = 0
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, A11, A12, A22, det);
        assert(result.first == 0.0f && result.second == 0.0f);
    }

    // Test 3: Known simple case: A = identity (A11=1, A12=0, A22=1), b = (-2, -4) => delta = (2, 4)
    {
        // Construct patches such that sum((new-old)*gx) = -2, sum((new-old)*gy) = -4
        std::vector<int> old_patch = {0, 1, 2, 3};
        std::vector<int> new_patch = {0, 1, 2, 3};  // start with zero diff, will adjust below
        std::vector<int> gx_patch  = {1, 1, -1, -1};  // sum of gx = 0, but we will set diff accordingly
        std::vector<int> gy_patch  = {1, -1, 1, -1};

        // Set new values so that diff array = {-1, -1, 1, 1} for b1 = -1*1 + -1*1 + 1*(-1) + 1*(-1) = -4? Let's compute properly.
        // We want b1 = sum(diff * gx) = -2. Let diff = { -1, -1, 1, 1 }.
        // b1 = (-1)*1 + (-1)*1 + 1*(-1) + 1*(-1) = -1 -1 -1 -1 = -4, not -2.
        // Let's choose diff = { -1, 1, -1, 1 } and gx = {1, -1, 1, -1} -> b1 = -1*1 + 1*(-1) + (-1)*1 + 1*(-1) = -1-1-1-1 = -4, still not -2.
        // Simpler: Use single-pixel patch? But we require equal sizes. Use size 1.
        // Let's redo with size 1.
    }

    // Test 3 (revised): Single pixel patch.
    {
        std::vector<int> old_patch = {10};
        std::vector<int> new_patch = {12};  // diff = 2
        std::vector<int> gx_patch  = {1};   // b1 = 2*1 = 2
        std::vector<int> gy_patch  = {-1};  // b2 = 2*(-1) = -2
        float A11 = 1.0f, A12 = 0.0f, A22 = 1.0f;
        float det = 1.0f;
        // delta_x = (A12*b2 - A22*b1)/det = (0*(-2) - 1*2)/1 = -2
        // delta_y = (A12*b1 - A11*b2)/det = (0*2 - 1*(-2))/1 = 2
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, A11, A12, A22, det);
        assert(std::fabs(result.first - (-2.0f)) < 1e-5f);
        assert(std::fabs(result.second - 2.0f) < 1e-5f);
    }

    // Test 4: Mismatched patch sizes -> returns zero.
    {
        std::vector<int> old_patch = {1, 2, 3};
        std::vector<int> new_patch = {1, 2, 3, 4};
        std::vector<int> gx_patch  = {1, 1, 1};
        std::vector<int> gy_patch  = {1, 1, 1};
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, 1.0f, 0.0f, 1.0f, 1.0f);
        assert(result.first == 0.0f && result.second == 0.0f);
    }

    // Test 5: Empty patch -> returns zero (determinant check not triggered but loops don't run).
    {
        std::vector<int> empty;
        auto result = klt_track_point(empty, empty, empty, empty, 1.0f, 0.0f, 1.0f, 1.0f);
        assert(result.first == 0.0f && result.second == 0.0f);
    }

    // Test 6: Non-zero motion with larger patch, verify formula manually.
    {
        // Use 4 pixels. Let diff = {1, -2, 3, -4}, gx = {1, 1, 1, 1}, gy = {1, -1, 1, -1}
        // b1 = 1*1 + (-2)*1 + 3*1 + (-4)*1 = -2
        // b2 = 1*1 + (-2)*(-1) + 3*1 + (-4)*(-1) = 1 + 2 + 3 + 4 = 10
        // Let A11=2, A12=0.5, A22=3, det = 2*3 - 0.25 = 5.75
        // delta_x = (A12*b2 - A22*b1)/det = (0.5*10 - 3*(-2))/5.75 = (5 + 6)/5.75 = 11/5.75 ≈ 1.913043
        // delta_y = (A12*b1 - A11*b2)/det = (0.5*(-2) - 2*10)/5.75 = (-1 - 20)/5.75 = -21/5.75 ≈ -3.652174
        std::vector<int> old_patch = {0, 0, 0, 0};
        std::vector<int> new_patch = {1, -2, 3, -4};  // diff = new-old = these values
        std::vector<int> gx_patch  = {1, 1, 1, 1};
        std::vector<int> gy_patch  = {1, -1, 1, -1};
        float A11 = 2.0f, A12 = 0.5f, A22 = 3.0f;
        float det = A11 * A22 - A12 * A12;  // 2*3 - 0.25 = 5.75
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, A11, A12, A22, det);
        float expected_x = (A12 * 10.0f - A22 * (-2.0f)) / det;  // (0.5*10 + 6)/5.75
        float expected_y = (A12 * (-2.0f) - A11 * 10.0f) / det;  // (-1 - 20)/5.75
        assert(std::fabs(result.first - expected_x) < 1e-4f);
        assert(std::fabs(result.second - expected_y) < 1e-4f);
    }

    // Test 7: Negative determinant also returns zero.
    {
        std::vector<int> old_patch = {1};
        std::vector<int> new_patch = {2};
        std::vector<int> gx_patch  = {1};
        std::vector<int> gy_patch  = {1};
        auto result = klt_track_point(old_patch, new_patch, gx_patch, gy_patch, 1.0f, 2.0f, 1.0f, -1.0f);
        assert(result.first == 0.0f && result.second == 0.0f);
    }

    return 0;
}

#include <vector>
#include <utility>
#include <cstddef>

// Perform one Lucas-Kanade optical flow update step for a single point.
// old_patch, new_patch, gx_patch, gy_patch are fixed-point pixel values (as ints).
// A11, A12, A22 are the accumulated spatial gradient matrix components (floats).
// determinant is the determinant of the spatial gradient matrix.
// Returns a pair (delta_x, delta_y) representing the motion update.
// If the determinant is not positive, returns (0.0f, 0.0f).
std::pair<float, float> klt_track_point(
    const std::vector<int>& old_patch,
    const std::vector<int>& new_patch,
    const std::vector<int>& gx_patch,
    const std::vector<int>& gy_patch,
    float A11,
    float A12,
    float A22,
    float determinant)
{
    // Validate that all patches have the same size.
    const std::size_t size = old_patch.size();
    if (new_patch.size() != size || gx_patch.size() != size || gy_patch.size() != size) {
        return {0.0f, 0.0f};
    }

    // If the determinant is not positive, the matrix is singular or ill-conditioned.
    if (determinant <= 0.0f) {
        return {0.0f, 0.0f};
    }

    // Compute the temporal gradient vector b = (b1, b2).
    float b1 = 0.0f;
    float b2 = 0.0f;
    for (std::size_t i = 0; i < size; ++i) {
        const float diff = static_cast<float>(new_patch[i] - old_patch[i]);
        b1 += diff * static_cast<float>(gx_patch[i]);
        b2 += diff * static_cast<float>(gy_patch[i]);
    }

    // Solve A * delta = -b for delta using the 2x2 inverse formula.
    const float delta_x = (A12 * b2 - A22 * b1) / determinant;
    const float delta_y = (A12 * b1 - A11 * b2) / determinant;

    return {delta_x, delta_y};
}

// The core of the Lucas-Kanade method is solving a 2x2 linear system for the optical flow increment. Given the spatial gradient matrix \(A = \begin{bmatrix} A_{11} & A_{12} \\ A_{12} & A_{22} \end{bmatrix}\) (symmetric) and the temporal gradient vector \(b = \begin{bmatrix} b_1 \\ b_2 \end{bmatrix}\) computed as the sum of the pixel-wise difference between the new and old image patches multiplied by the respective horizontal and vertical gradients, the motion update is \( \Delta = -A^{-1} b \). For a 2x2 matrix, the inverse is \( \frac{1}{\det(A)} \begin{bmatrix} A_{22} & -A_{12} \\ -A_{12} & A_{11} \end{bmatrix} \), so \( \Delta_x = \frac{A_{12} b_2 - A_{22} b_1}{\det(A)} \) and \( \Delta_y = \frac{A_{12} b_1 - A_{11} b_2}{\det(A)} \). The function must compute the two sums `b1` and `b2` by iterating over the vectors once. Edge cases: if the determinant is zero or negative (which indicates a singular or ill-conditioned matrix, often due to lack of texture), the update is invalid, so return zero motion. Also, if any vector is empty, the sums are zero, leading to zero delta (but determinant check handles it). Time complexity is O(n), where n is the patch size (number of pixels per patch). Space complexity is O(1) auxiliary, as we only use a few scalar accumulators.

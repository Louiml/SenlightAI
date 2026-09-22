Write a C++ function `dominantEigenpair` that, given a non-diagonal square matrix represented as a `std::vector<std::vector<double>>` (or a 2D array if you prefer, but the function signature must accept a vector of vectors), an initial eigenvector guess as a `std::vector<double>`, and a tolerance `double eps`, computes the dominant eigenvalue (largest absolute value among eigenvalues) and its corresponding eigenvector using the power iteration method. The function must return a `std::pair<double, std::vector<double>>` containing the eigenvalue and the normalized eigenvector. The input is guaranteed to be a square matrix of order at least 2, with a valid initial guess (non-zero). The matrix may have negative entries, and the dominant eigenvalue may be negative; the function should return the actual eigenvalue (with its sign), not its absolute value. The eigenvector should be scaled so that its largest element (in absolute value) is 1. The iteration should stop when the change in the absolute value of the eigenvalue estimate between successive iterations is less than `eps`. Note that the original snippet mistakenly used `abs(c[0])` and then picked the largest absolute value, which gives the magnitude of the dominant eigenvalue; you must recover the sign by tracking the sign of the largest component before taking the absolute value.

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function here (or in a header). For this test, we assume it's defined above.

int main() {
    // Test 1: Simple diagonal matrix, dominant eigenvalue 5
    std::vector<std::vector<double>> A1 = {{5, 0}, {0, 3}};
    std::vector<double> b1 = {1, 1};
    auto res1 = dominantEigenpair(A1, b1, 1e-6);
    assert(std::fabs(res1.first - 5.0) < 1e-5);
    assert(std::fabs(res1.second[0] - 1.0) < 1e-5);
    assert(std::fabs(res1.second[1]) < 1e-5);

    // Test 2: Dominant eigenvalue negative
    std::vector<std::vector<double>> A2 = {{-4, 1}, {1, 2}};
    // Eigenvalues: -4.236, 2.236, dominant is -4.236 (largest magnitude)
    std::vector<double> b2 = {1, 1};
    auto res2 = dominantEigenpair(A2, b2, 1e-6);
    assert(std::fabs(res2.first + 4.2360679) < 1e-3); // -4.236...
    // Check that eigenvector is proportional to [1, -0.236] or so
    // Our convergence gives largest component = 1, so check ratio
    double ratio = res2.second[1] / res2.second[0];
    assert(std::fabs(ratio - (-0.2360679)) < 1e-3);

    // Test 3: 3x3 matrix with dominant eigenvalue 10
    std::vector<std::vector<double>> A3 = {{10, 2, 3}, {0, 1, 0}, {0, 0, 1}};
    std::vector<double> b3 = {1, 1, 1};
    auto res3 = dominantEigenpair(A3, b3, 1e-7);
    assert(std::fabs(res3.first - 10.0) < 1e-5);
    assert(std::fabs(res3.second[0] - 1.0) < 1e-5);
    assert(std::fabs(res3.second[1]) < 1e-4);
    assert(std::fabs(res3.second[2]) < 1e-4);

    // Test 4: Matrix with dominant eigenvalue 2 (non-identity)
    std::vector<std::vector<double>> A4 = {{2, 1}, {0, 1}};
    std::vector<double> b4 = {0.5, 0.5};
    auto res4 = dominantEigenpair(A4, b4, 1e-8);
    assert(std::fabs(res4.first - 2.0) < 1e-5);
    assert(std::fabs(res4.second[0] - 1.0) < 1e-5);
    assert(std::fabs(res4.second[1]) < 1e-4);

    // Test 5: Larger matrix, ensure convergence and normalization
    std::vector<std::vector<double>> A5 = {{3, 1, 0, 0}, {0, 3, 0, 0}, {0, 0, 2, 0}, {0, 0, 0, 1}};
    std::vector<double> b5 = {1, 0, 1, 1};
    auto res5 = dominantEigenpair(A5, b5, 1e-6);
    // Dominant eigenvalue is 3 (largest magnitude). However the initial guess has component along eigenvector [1,0,0,0] and [0,0,1,0], etc. It should converge to 3.
    assert(std::fabs(res5.first - 3.0) < 1e-4);
    // Eigenvector should have non-zero first component (since b5 has 1 there)
    assert(std::fabs(res5.second[0] - 1.0) < 1e-4);
    assert(std::fabs(res5.second[1]) < 1e-4);
    assert(std::fabs(res5.second[2]) < 1e-4);
    assert(std::fabs(res5.second[3]) < 1e-4);

    // Test 6: Negative dominant eigenvalue with symmetric matrix
    std::vector<std::vector<double>> A6 = {{0, 1}, {1, 0}}; // eigenvalues 1 and -1, dominant magnitude is 1 (both same magnitude), but power iteration may not converge to a unique eigenvalue. We skip this as it's not guaranteed.

    // Test 7: Dominant eigenvalue with mixed signs in matrix
    std::vector<std::vector<double>> A7 = {{2, -1}, {-1, 2}}; // eigenvalues 3 and 1
    std::vector<double> b7 = {1, 1};
    auto res7 = dominantEigenpair(A7, b7, 1e-6);
    assert(std::fabs(res7.first - 3.0) < 1e-4);
    // Eigenvector for eigenvalue 3 is [1, -1] normalized to largest absolute component 1
    assert(std::fabs(res7.second[0] - 1.0) < 1e-4);
    assert(std::fabs(res7.second[1] + 1.0) < 1e-4);

    // Test 8: Check called with high tolerance gives fewer iterations (just ensure it works)
    auto res8 = dominantEigenpair(A1, b1, 1.0); // coarse tolerance
    assert(std::fabs(res8.first - 5.0) < 1.0); // sanity

    // Test 9: Ensure that the function handles a 2x2 matrix with an eigenvalue very close in magnitude
    // Use a nearly diagonal matrix
    std::vector<std::vector<double>> A9 = {{10, 0.1}, {0.1, 9.9}};
    std::vector<double> b9 = {1, 1};
    auto res9 = dominantEigenpair(A9, b9, 1e-6);
    assert(std::fabs(res9.first - 10.0) < 1e-2); // dominant is ~10.005

    return 0;
}

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

// Compute the dominant eigenvalue and corresponding eigenvector of a square matrix
// using power iteration. Returns {eigenvalue, eigenvector}.
// The matrix is square, order n >= 2. The initial guess must be non-zero.
// The eigenvector is normalized so that its largest magnitude component is 1.
std::pair<double, std::vector<double>> dominantEigenpair(
    const std::vector<std::vector<double>>& a,
    std::vector<double> b,
    double eps
) {
    const int n = static_cast<int>(a.size());
    std::vector<double> c(n);

    double lambda = 0.0;
    double lambda_prev = 0.0;

    // Initialize lambda from the initial guess using the first component (any non-zero)
    // We need an initial estimate; we'll just start with 0 and let the loop update.
    // Use do-while as in the snippet to ensure at least one iteration.
    do {
        lambda_prev = lambda;

        // Compute c = A * b
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += a[i][j] * b[j];
            }
            c[i] = sum;
        }

        // Find the element of c with the largest absolute value, preserving sign
        double max_abs = std::fabs(c[0]);
        double lambda_sign = c[0]; // this will hold the signed max abs value
        for (int i = 1; i < n; ++i) {
            double abs_val = std::fabs(c[i]);
            if (abs_val > max_abs) {
                max_abs = abs_val;
                lambda_sign = c[i];
            }
        }

        lambda = lambda_sign; // could be negative

        // Divide c by lambda to get the new eigenvector b
        // Since lambda is guaranteed non-zero (dominant eigenvalue of a non-singular matrix)
        for (int i = 0; i < n; ++i) {
            b[i] = c[i] / lambda;
        }

    } while (std::fabs(lambda - lambda_prev) >= eps);

    // If the loop converges, return the final eigenvalue and eigenvector
    return {lambda, b};
}

// The solution uses the power iteration method: starting from an initial guess vector `b`, repeatedly compute `c = A * b`. Then find the element of `c` with the largest absolute value; that value (with its sign) is the eigenvalue estimate `lambda`, and the new eigenvector is `c / lambda`. The loop continues until the difference in the absolute value of successive `lambda` estimates is less than `eps`. Important edge cases: (1) The matrix may have a dominant eigenvalue that is negative; the sign must be preserved by taking the largest absolute component with its original sign, not just the absolute value. (2) The initial guess must not be orthogonal to the dominant eigenvector; the task guarantees a valid guess. (3) If the matrix has repeated eigenvalues or complex dominant eigenvalues, the method may not converge; but the task assumes a real dominant eigenvalue with distinct absolute value. (4) Division by zero: if the largest absolute component is zero (which should not happen with a valid initial guess and non-singular dominant eigenvector), handle gracefully, but the task assumes valid inputs. Time complexity is `O(iterations * n^2)` for each matrix-vector multiplication; space complexity is `O(n^2)` for storing the matrix, plus `O(n)` for vectors. The number of iterations depends on the ratio of the two largest eigenvalue magnitudes; typically a few dozen to hundreds.

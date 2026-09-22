Write a C++ function named `transformAndMultiply` that takes a 3x3 matrix of doubles as input. The function should first scale the matrix so that all its elements are shifted to be positive and fall into a larger numerical range: add 1.2 to every element, then multiply the entire matrix by 50. After that, multiply the transformed matrix by the column vector `(1, 2, 3)^T` and return the resulting 3-element vector. The input matrix should not be modified; the function should work with `const` references. Use the Eigen library for matrix and vector operations. Ensure that the function is robust to any 3x3 matrix of doubles (including matrices with NaN or infinity, which will propagate naturally).
The solution uses the Eigen library's `MatrixXd` for dense double matrices and `VectorXd` for vectors. The transformation is applied element-wise using the `array()` method (or by directly adding a constant matrix) and then scaling by 50. This follows the same pattern as the snippet. The key steps: clone the input matrix (since it's const), add 1.2 to each element, multiply by 50, then compute the matrix-vector product with `v << 1, 2, 3`. Edge cases: because we only use arithmetic operations, NaN and infinity propagate correctly, and no special handling is needed. The function takes a `const MatrixXd&` to avoid copying and guarantee the input isn't modified. Time complexity: \(O(1)\) since the matrix size is fixed at 3x3. Space complexity: \(O(1)\) for temporary matrices and vectors. The solution is straightforward and uses Eigen's optimized operators.
#include <Eigen/Dense>

// Transforms a 3x3 matrix by adding 1.2 to each element, scaling by 50,
// then multiplies the result by vector (1,2,3)^T. Input is not modified.
Eigen::VectorXd transformAndMultiply(const Eigen::MatrixXd& input) {
    // Validate size to ensure 3x3; Eigen would assert otherwise, but we keep it explicit.
    assert(input.rows() == 3 && input.cols() == 3);

    // Step 1: Add 1.2 to every element.
    // Use array() for element-wise addition, then convert back to matrix.
    Eigen::MatrixXd shifted = (input.array() + 1.2).matrix();

    // Step 2: Scale by 50.
    shifted *= 50.0;

    // Step 3: Define the column vector (1,2,3)^T.
    Eigen::VectorXd v(3);
    v << 1.0, 2.0, 3.0;

    // Return the matrix-vector product.
    return shifted * v;
}
#include <Eigen/Dense>
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple known matrix; compute by hand.
    Eigen::MatrixXd m1(3,3);
    m1 << 0, 0, 0,
          0, 0, 0,
          0, 0, 0;
    Eigen::VectorXd r1 = transformAndMultiply(m1);
    // After shift: all 1.2, *50 -> 60 each. Multiply by (1,2,3) => each row: 60*1 + 60*2 + 60*3 = 360.
    Eigen::VectorXd expected1(3);
    expected1 << 360, 360, 360;
    assert(r1.isApprox(expected1, 1e-12));

    // Test 2: Identity matrix.
    Eigen::MatrixXd m2(3,3);
    m2 << 1, 0, 0,
          0, 1, 0,
          0, 0, 1;
    Eigen::VectorXd r2 = transformAndMultiply(m2);
    // Shift: 2.2 for diagonal, 1.2 for off-diagonal => *50 => 110 and 60. Multiply by (1,2,3):
    // Row1: 110*1 + 60*2 + 60*3 = 110+120+180 = 410
    // Row2: 60*1 + 110*2 + 60*3 = 60+220+180 = 460
    // Row3: 60*1 + 60*2 + 110*3 = 60+120+330 = 510
    Eigen::VectorXd expected2(3);
    expected2 << 410, 460, 510;
    assert(r2.isApprox(expected2, 1e-12));

    // Test 3: Negative values.
    Eigen::MatrixXd m3(3,3);
    m3 << -1.2, -1.2, -1.2,
          -1.2, -1.2, -1.2,
          -1.2, -1.2, -1.2;
    Eigen::VectorXd r3 = transformAndMultiply(m3);
    // Shifted all 0, *50 = 0. Result all 0.
    Eigen::VectorXd expected3 = Eigen::VectorXd::Zero(3);
    assert(r3.isApprox(expected3, 1e-12));

    // Test 4: Matrix with large values.
    Eigen::MatrixXd m4(3,3);
    m4 << 1e6, 2e6, 3e6,
          4e6, 5e6, 6e6,
          7e6, 8e6, 9e6;
    Eigen::VectorXd r4 = transformAndMultiply(m4);
    // Hand compute first element: ( (1e6+1.2)*50)*1 + ((2e6+1.2)*50)*2 + ((3e6+1.2)*50)*3
    // = 50*((1e6+1.2) + 2*(2e6+1.2) + 3*(3e6+1.2)) = 50*(1e6 +4e6+9e6 + (1.2+2.4+3.6)) = 50*(14e6 + 7.2) = 700e6 + 360.
    // We'll just check approximate against a direct manual calculation for first element.
    double val1 = 50 * ( (1e6+1.2)*1 + (2e6+1.2)*2 + (3e6+1.2)*3 );
    assert(std::abs(r4[0] - val1) < 1e-6);

    // Test 5: Ensure input matrix is not modified.
    Eigen::MatrixXd m5(3,3);
    m5 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    Eigen::MatrixXd m5_copy = m5;
    transformAndMultiply(m5);
    assert(m5 == m5_copy);

    // Test 6: NaN handling (optional, but check function doesn't crash).
    Eigen::MatrixXd m6(3,3);
    m6 << std::nan(""), 0, 0, 0, 0, 0, 0, 0, 0;
    Eigen::VectorXd r6 = transformAndMultiply(m6);
    assert(std::isnan(r6[0]));
    // Other components should be finite.
    assert(!std::isnan(r6[1]) && !std::isnan(r6[2]));

    return 0;
}

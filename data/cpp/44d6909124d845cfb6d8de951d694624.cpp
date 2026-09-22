// Write a standalone C++ function named `applyRampToMatrix` that takes a constant reference to an `Eigen::MatrixXd` and returns a new `Eigen::MatrixXd` of the same dimensions, where each element of the output is the result of applying the ramp function (also called ReLU) to the corresponding input element: `f(x) = x` if `x > 0`, otherwise `0`. The function must not modify the input matrix, must handle empty matrices (0×0) gracefully, and must work for matrices with arbitrary positive dimensions, including those with negative, zero, and positive values. The solution should use Eigen’s coefficient-wise operations or a manual loop, but must not rely on external libraries other than Eigen Core. Ensure the function is `const`-correct and returns the result by value.
// The core solution is straightforward: for each element in the input matrix, compute the ramp value and place it in the output matrix. The simplest approach is to use Eigen’s `unaryExpr` with a static function or lambda, which internally iterates over all coefficients. Alternatively, one can loop over rows and columns using `.coeff(row, col)` or use the `array()` view to apply a scalar lambda. The ramp function is piecewise: if the value is strictly greater than zero, return the value; otherwise return zero. Edge cases include zero (must return zero), negative values (must return zero), and large magnitudes (no special handling beyond normal arithmetic). Empty matrices: if `rows()` or `cols()` is zero, the function should return a matrix of the same dimensions, which Eigen naturally does when creating a result of the same size and applying an empty operation. The time complexity is O(rows * cols) because every element is visited exactly once. The space complexity is O(rows * cols) for the returned matrix, plus constant auxiliary space for the loop or unary expression. The solution must preserve the input matrix unchanged (pass by const reference) and return by value.
#include <Eigen/Core>

// Helper function for coefficient-wise ramp transform.
inline double ramp(double x) {
    return x > 0.0 ? x : 0.0;
}

// Apply the ramp function (ReLU) to every element of the input matrix.
// Returns a new matrix of the same dimensions; the input is not modified.
Eigen::MatrixXd applyRampToMatrix(const Eigen::MatrixXd& input) {
    // Use unaryExpr to apply the ramp function to each coefficient.
    return input.unaryExpr(&ramp);
}
#include <Eigen/Core>
#include <cassert>

// The solution function is declared here (in real code, it would be in a header).
inline double ramp(double x) {
    return x > 0.0 ? x : 0.0;
}

Eigen::MatrixXd applyRampToMatrix(const Eigen::MatrixXd& input) {
    return input.unaryExpr(&ramp);
}

int main() {
    // Test 1: Positive and negative values.
    Eigen::MatrixXd m1(2, 2);
    m1 << -1.0, 2.5,
           0.0, -0.1;
    Eigen::MatrixXd r1 = applyRampToMatrix(m1);
    assert(r1.rows() == 2 && r1.cols() == 2);
    assert(r1(0, 0) == 0.0);
    assert(r1(0, 1) == 2.5);
    assert(r1(1, 0) == 0.0);
    assert(r1(1, 1) == 0.0);

    // Test 2: All positive.
    Eigen::MatrixXd m2(1, 3);
    m2 << 0.5, 1.0, 100.0;
    Eigen::MatrixXd r2 = applyRampToMatrix(m2);
    assert(r2(0, 0) == 0.5);
    assert(r2(0, 1) == 1.0);
    assert(r2(0, 2) == 100.0);

    // Test 3: All negative and zero.
    Eigen::MatrixXd m3(3, 1);
    m3 << -5.0, 0.0, -0.001;
    Eigen::MatrixXd r3 = applyRampToMatrix(m3);
    assert(r3(0, 0) == 0.0);
    assert(r3(1, 0) == 0.0);
    assert(r3(2, 0) == 0.0);

    // Test 4: Empty matrix (0x0).
    Eigen::MatrixXd m4;
    Eigen::MatrixXd r4 = applyRampToMatrix(m4);
    assert(r4.rows() == 0 && r4.cols() == 0);

    // Test 5: Non-square matrix with mixed values.
    Eigen::MatrixXd m5(2, 3);
    m5 << -3.0, 0.0, 4.0,
           2.0, -2.0, 0.0;
    Eigen::MatrixXd r5 = applyRampToMatrix(m5);
    assert(r5.rows() == 2 && r5.cols() == 3);
    assert(r5(0, 0) == 0.0);
    assert(r5(0, 1) == 0.0);
    assert(r5(0, 2) == 4.0);
    assert(r5(1, 0) == 2.0);
    assert(r5(1, 1) == 0.0);
    assert(r5(1, 2) == 0.0);

    return 0;
}

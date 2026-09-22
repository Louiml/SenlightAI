Write a C++ function that takes an `Eigen::MatrixXd` (a dynamic-size matrix of doubles) and two scalar bounds, `lower` and `upper`, and returns a new matrix of the same dimensions where every element is clamped to the interval `[lower, upper]`. That is, any element less than `lower` becomes `lower`, any element greater than `upper` becomes `upper`, and elements already within the interval remain unchanged. The function must preserve the original matrix (do not modify the input). Use `Eigen`’s `unaryExpr` with a custom functor to apply the clamping element‑wise. Assume `lower <= upper`; if `lower > upper`, the behavior is unspecified (you may simply let the clamping logic produce a result, but you do not need to handle it specially). The function should be efficient and avoid copying the matrix more than necessary.

The solution uses Eigen’s `MatrixBase::unaryExpr`, which applies a callable object to each coefficient and returns a new matrix of the same shape. The functor is a small struct (or lambda) that captures the two bounds by value and returns `std::clamp(x, lower, upper)`. Because `unaryExpr` already creates the output matrix, we simply return its result. The main edge case is an empty matrix (0×0 or 0×n), which is handled naturally because `unaryExpr` on an empty matrix returns an empty matrix without errors. Also, because the input is `const MatrixXd&`, we never modify the original. Time complexity is O(N) where N is the number of elements, and space complexity is O(N) for the returned matrix (plus a tiny constant for the functor). For performance, `Eigen` vectorizes `unaryExpr` where possible, though that is an implementation detail.

#include <Eigen/Core>
#include <algorithm>

// Functor to clamp a scalar value to [lower, upper].
struct ScalarClamp {
    double lower;
    double upper;

    double operator()(double x) const {
        return std::clamp(x, lower, upper);
    }
};

// Returns a new matrix where each element of 'input' is clamped to [lower, upper].
// The input matrix is not modified. Assumes lower <= upper.
Eigen::MatrixXd clampMatrix(const Eigen::MatrixXd& input, double lower, double upper) {
    return input.unaryExpr(ScalarClamp{lower, upper});
}

#include <Eigen/Core>
#include <cassert>

// Assume clampMatrix is declared above (in an actual test, include the header).
Eigen::MatrixXd clampMatrix(const Eigen::MatrixXd& input, double lower, double upper);

int main() {
    // Test 1: Basic clamping with positive matrix
    Eigen::MatrixXd m1(2, 2);
    m1 << 1.0, 2.0,
          3.0, 4.0;
    Eigen::MatrixXd result1 = clampMatrix(m1, 1.5, 3.5);
    assert(result1(0,0) == 1.5);
    assert(result1(0,1) == 2.0);
    assert(result1(1,0) == 3.0);
    assert(result1(1,1) == 3.5);

    // Test 2: Clamping with negative values and mixed signs
    Eigen::MatrixXd m2(1, 4);
    m2 << -2.0, -0.5, 0.5, 2.0;
    Eigen::MatrixXd result2 = clampMatrix(m2, -1.0, 1.0);
    assert(result2(0,0) == -1.0);
    assert(result2(0,1) == -0.5);
    assert(result2(0,2) == 0.5);
    assert(result2(0,3) == 1.0);

    // Test 3: Values exactly at bounds are unchanged
    Eigen::MatrixXd m3(1, 2);
    m3 << -1.0, 1.0;
    Eigen::MatrixXd result3 = clampMatrix(m3, -1.0, 1.0);
    assert(result3(0,0) == -1.0);
    assert(result3(0,1) == 1.0);

    // Test 4: Empty matrix (0x0)
    Eigen::MatrixXd m4(0, 0);
    Eigen::MatrixXd result4 = clampMatrix(m4, -5.0, 5.0);
    assert(result4.rows() == 0);
    assert(result4.cols() == 0);

    // Test 5: Non-square matrix
    Eigen::MatrixXd m5(3, 1);
    m5 << -10.0, 0.0, 10.0;
    Eigen::MatrixXd result5 = clampMatrix(m5, -1.0, 1.0);
    assert(result5(0,0) == -1.0);
    assert(result5(1,0) == 0.0);
    assert(result5(2,0) == 1.0);

    // Test 6: All values below lower bound
    Eigen::MatrixXd m6(1, 3);
    m6 << 0.1, 0.2, 0.3;
    Eigen::MatrixXd result6 = clampMatrix(m6, 0.5, 1.0);
    assert(result6(0,0) == 0.5);
    assert(result6(0,1) == 0.5);
    assert(result6(0,2) == 0.5);

    // Test 7: All values above upper bound
    Eigen::MatrixXd m7(2, 2);
    m7 << 5.0, 6.0,
          7.0, 8.0;
    Eigen::MatrixXd result7 = clampMatrix(m7, 0.0, 1.0);
    assert(result7(0,0) == 1.0);
    assert(result7(1,1) == 1.0);

    // Test 8: Negative bounds
    Eigen::MatrixXd m8(1, 2);
    m8 << -3.0, 3.0;
    Eigen::MatrixXd result8 = clampMatrix(m8, -2.0, 2.0);
    assert(result8(0,0) == -2.0);
    assert(result8(0,1) == 2.0);

    // Test 9: Input matrix not modified
    Eigen::MatrixXd m9(1, 3);
    m9 << -1.0, 0.0, 1.0;
    Eigen::MatrixXd original = m9;
    Eigen::MatrixXd result9 = clampMatrix(m9, -0.5, 0.5);
    assert(m9 == original); // input unchanged

    // Test 10: Large matrix (spot‑check)
    Eigen::MatrixXd m10 = Eigen::MatrixXd::Random(4, 5);
    Eigen::MatrixXd result10 = clampMatrix(m10, -0.2, 0.2);
    for (int i = 0; i < m10.rows(); ++i) {
        for (int j = 0; j < m10.cols(); ++j) {
            assert(result10(i,j) >= -0.2 && result10(i,j) <= 0.2);
            // If original was within bounds, it must remain unchanged
            if (m10(i,j) >= -0.2 && m10(i,j) <= 0.2) {
                assert(result10(i,j) == m10(i,j));
            }
        }
    }

    return 0;
}

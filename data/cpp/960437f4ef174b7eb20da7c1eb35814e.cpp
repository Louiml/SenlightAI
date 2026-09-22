/*
Write a C++ function that takes a `double` value `entry` and returns an `Eigen::MatrixXd` representing an \(8 \times 8\) matrix built from that single value. The matrix entries are defined by the following rule: elements on the main diagonal are equal to \( \text{entry} \), elements on the superdiagonal (just above the main diagonal) are equal to \( \text{entry} \times 2 \), and all other elements are zero. The function must ensure that the matrix has exactly 8 rows and 8 columns, and the returned matrix must be a valid Eigen matrix with the specified values stored at the correct positions.
*/
#include <Eigen/Dense>

// Build an 8x8 matrix where the main diagonal is 'entry',
// the superdiagonal is 'entry * 2', and all other entries are 0.
Eigen::MatrixXd buildSpecialMatrix(double entry) {
    Eigen::MatrixXd mat = Eigen::MatrixXd::Zero(8, 8); // all zeros

    // Main diagonal: positions (i, i)
    for (int i = 0; i < 8; ++i) {
        mat(i, i) = entry;
    }

    // Superdiagonal: positions (i, i+1), which exists for i = 0..6
    for (int i = 0; i < 7; ++i) {
        mat(i, i + 1) = entry * 2.0;
    }

    return mat;
}
#include <cassert>
#include <cmath>
#include <Eigen/Dense>

int main() {
    // Test with a positive entry
    Eigen::MatrixXd m1 = buildSpecialMatrix(3.0);
    assert(m1.rows() == 8 && m1.cols() == 8);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            double expected = 0.0;
            if (i == j) expected = 3.0;
            else if (j == i + 1) expected = 6.0;
            assert(std::abs(m1(i, j) - expected) < 1e-12);
        }
    }

    // Test with zero
    Eigen::MatrixXd m2 = buildSpecialMatrix(0.0);
    assert(m2.rows() == 8 && m2.cols() == 8);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            assert(m2(i, j) == 0.0);
        }
    }

    // Test with negative entry
    Eigen::MatrixXd m3 = buildSpecialMatrix(-2.5);
    assert(m3.rows() == 8 && m3.cols() == 8);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            double expected = 0.0;
            if (i == j) expected = -2.5;
            else if (j == i + 1) expected = -5.0;
            assert(std::abs(m3(i, j) - expected) < 1e-12);
        }
    }

    // Spot-check specific positions
    assert(buildSpecialMatrix(1.0)(0, 0) == 1.0);
    assert(buildSpecialMatrix(1.0)(0, 1) == 2.0);
    assert(buildSpecialMatrix(1.0)(7, 7) == 1.0);
    assert(buildSpecialMatrix(1.0)(7, 6) == 0.0); // superdiagonal does not exist below main
    assert(buildSpecialMatrix(1.0)(1, 2) == 2.0);

    return 0;
}
// The solution is straightforward: create an \(8 \times 8\) matrix initialized to all zeros using `MatrixXd::Zero(8,8)`. Then iterate over diagonal indices from 0 to 7 and set `matrix(i,i) = entry`. For the superdiagonal, iterate indices from 0 to 6 and set `matrix(i, i+1) = entry * 2`. Edge cases to consider: if `entry` is zero or negative, the rules still apply without special handling; the matrix size is fixed and never changes. The time complexity is \(O(1)\) because the matrix dimensions are constant (64 elements, with only 15 assignments), and space complexity is \(O(1)\) for the matrix itself (plus Eigen's internal storage of fixed size). `const` correctness is applied by passing the scalar by value (or by const reference if desired) and returning a matrix by value.

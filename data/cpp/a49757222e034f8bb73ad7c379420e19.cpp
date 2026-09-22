Write a C++ function that accepts a fixed-size 3×3 matrix of double-precision floating-point values (represented as `Eigen::Matrix3d`) and returns a `std::vector<double>` containing the product of each row of the matrix, in row order (row 0 first, then row 1, then row 2). The function must compute the product of all three elements in each row, handling negative values correctly, and should be `const`-correct (i.e., it must not modify the input matrix). The function should be named `rowProducts`. You may assume the matrix is already initialized; no input parsing is required. The returned vector must have exactly three elements, each equal to the product of the corresponding row's elements.

// The solution uses Eigen's built-in `rowwise().prod()` method, which computes the product of each row of a matrix and returns a column vector of length equal to the number of rows. For a 3×3 matrix, this yields a vector of 3 values. The main algorithm is trivial: call `matrix.rowwise().prod()`, which internally iterates over each row and multiplies its elements together. Edge cases include rows containing zeros (product becomes zero), negative numbers (product sign follows the rule of signs), and values large or small enough to cause overflow or underflow — these are not handled specially and follow standard IEEE 754 double behavior. Since the matrix has a fixed size of 3×3, the time complexity is constant: exactly 3 rows × 3 multiplications each = 9 multiplications. Space complexity is O(1) auxiliary space for the computation, plus O(3) for the returned vector, which is constant. The implementation should be `const` because we do not modify the input matrix. The result must be converted to a `std::vector<double>` for convenient return.

#include <vector>
#include <Eigen/Dense>

// Compute the product of each row of a 3x3 matrix, returning a vector of three doubles.
// The i-th element of the returned vector is the product of the i-th row's elements.
std::vector<double> rowProducts(const Eigen::Matrix3d& matrix) {
    // rowwise().prod() returns a column vector with each row's product.
    Eigen::Vector3d products = matrix.rowwise().prod();
    // Convert Eigen vector to std::vector<double>.
    return std::vector<double>(products.data(), products.data() + products.size());
}

#include <cassert>
#include <vector>
#include <Eigen/Dense>

// The solution function is declared here (or included from above).
std::vector<double> rowProducts(const Eigen::Matrix3d& matrix);

int main() {
    // Test 1: Simple positive values
    Eigen::Matrix3d m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    std::vector<double> r1 = rowProducts(m1);
    assert(r1.size() == 3);
    assert(r1[0] == 1 * 2 * 3);  // 6
    assert(r1[1] == 4 * 5 * 6);  // 120
    assert(r1[2] == 7 * 8 * 9);  // 504

    // Test 2: Negative and zero values
    Eigen::Matrix3d m2;
    m2 << -1, 2, -3,
          0, 5, -2,
          4, -4, 4;
    std::vector<double> r2 = rowProducts(m2);
    assert(r2[0] == (-1) * 2 * (-3)); // 6
    assert(r2[1] == 0 * 5 * (-2));    // 0
    assert(r2[2] == 4 * (-4) * 4);    // -64

    // Test 3: All ones
    Eigen::Matrix3d m3 = Eigen::Matrix3d::Ones();
    std::vector<double> r3 = rowProducts(m3);
    for (double v : r3) {
        assert(v == 1.0);
    }

    // Test 4: Identity matrix (products all zero because off-diagonal zeros)
    Eigen::Matrix3d m4 = Eigen::Matrix3d::Identity();
    std::vector<double> r4 = rowProducts(m4);
    assert(r4[0] == 1.0 * 0.0 * 0.0); // 0
    assert(r4[1] == 0.0 * 1.0 * 0.0); // 0
    assert(r4[2] == 0.0 * 0.0 * 1.0); // 0

    // Test 5: All zeros
    Eigen::Matrix3d m5 = Eigen::Matrix3d::Zero();
    std::vector<double> r5 = rowProducts(m5);
    for (double v : r5) {
        assert(v == 0.0);
    }

    return 0;
}

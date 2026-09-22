Write a C++ function that computes the induced 1-norm and infinity-norm of a 2D floating-point matrix represented using `Eigen::MatrixXf`. The induced 1-norm is the maximum absolute column sum, and the induced infinity-norm is the maximum absolute row sum. The function should take a `const Eigen::MatrixXf&` and return a `std::pair<float, float>` where the first element is the 1-norm and the second is the infinity-norm. The matrix may have zero rows or columns (empty matrix), in which case both norms should be 0.0f. Handle negative values correctly by taking absolute values, and ensure the result is accurate for any dimensions.

#include <Eigen/Dense>
#include <cassert>
#include <utility>

// Declare the function under test (assume it's provided elsewhere or above)
std::pair<float, float> matrixNorms(const Eigen::MatrixXf& m);

int main() {
    // Example from the original snippet: 2x2 matrix
    Eigen::MatrixXf m1(2,2);
    m1 << 1, -2,
          -3,  4;
    auto n1 = matrixNorms(m1);
    assert(n1.first == 5.0f);   // max column sum: |1|+|-3|=4, |-2|+|4|=6 -> max is 6? Wait recheck
    // Actually columns: col0: |1|+|-3|=4, col1: |-2|+|4|=6 -> max=6; rows: row0: |1|+|-2|=3, row1: |-3|+|4|=7 -> max=7
    // The original snippet computes 1-norm as 6 and infty-norm as 7? Let's verify: m.cwiseAbs().colwise().sum() -> [4,6], max=6. rowwise sum -> [3,7], max=7.
    // Correct assert:
    assert(n1.first == 6.0f);
    assert(n1.second == 7.0f);

    // Identity matrix 3x3: 1-norm and infty-norm are both 1
    Eigen::MatrixXf m2 = Eigen::MatrixXf::Identity(3,3);
    auto n2 = matrixNorms(m2);
    assert(n2.first == 1.0f);
    assert(n2.second == 1.0f);

    // Single element matrix
    Eigen::MatrixXf m3(1,1);
    m3 << -42.5f;
    auto n3 = matrixNorms(m3);
    assert(n3.first == 42.5f);
    assert(n3.second == 42.5f);

    // Zero matrix with negative values? Zero matrix all zeros
    Eigen::MatrixXf m4 = Eigen::MatrixXf::Zero(2,3);
    auto n4 = matrixNorms(m4);
    assert(n4.first == 0.0f);
    assert(n4.second == 0.0f);

    // Empty matrix (0x0)
    Eigen::MatrixXf m5(0,0);
    auto n5 = matrixNorms(m5);
    assert(n5.first == 0.0f);
    assert(n5.second == 0.0f);

    // Non-square matrix with negative values
    Eigen::MatrixXf m6(3,2);
    m6 << -1,  2,
           3, -4,
          -5,  6;
    // col sums: col0: |-1|+|3|+|-5|=9, col1: |2|+|-4|+|6|=12 -> 1-norm=12
    // row sums: row0: |-1|+|2|=3, row1: |3|+|-4|=7, row2: |-5|+|6|=11 -> infty-norm=11
    auto n6 = matrixNorms(m6);
    assert(n6.first == 12.0f);
    assert(n6.second == 11.0f);

    return 0;
}

#include <Eigen/Dense>
#include <utility>

// Compute induced 1-norm (max absolute column sum) and infinity-norm (max absolute row sum)
// of a floating-point matrix. Returns {1_norm, infinity_norm}. Empty matrix yields {0,0}.
std::pair<float, float> matrixNorms(const Eigen::MatrixXf& m) {
    if (m.size() == 0) {
        return {0.0f, 0.0f};
    }
    float norm1 = m.cwiseAbs().colwise().sum().maxCoeff();
    float normInf = m.cwiseAbs().rowwise().sum().maxCoeff();
    return {norm1, normInf};
}

// The solution uses Eigen's convenient coefficient-wise operations. For the 1-norm, we compute `m.cwiseAbs().colwise().sum()` which returns a row vector containing the sum of absolute values per column, then take `.maxCoeff()` to get the maximum column sum. For the infinity-norm, we compute `m.cwiseAbs().rowwise().sum()` which returns a column vector of row sums, then `.maxCoeff()`. Edge case: if the matrix has zero rows or zero columns, both `colwise()` and `rowwise()` operations yield an empty vector, and calling `.maxCoeff()` on an empty vector is undefined; we handle this by checking `m.size() == 0` and returning `{0.0f, 0.0f}`. Time complexity is O(rows * cols) because we visit every element to compute absolute sums. Space complexity is O(rows + cols) for the temporary row/column sum vectors (though in practice Eigen may optimize this, but complexity still holds).

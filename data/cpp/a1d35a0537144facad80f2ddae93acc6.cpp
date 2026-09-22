// Write a C++ function `columnWiseStats` that takes a constant reference to an Eigen `Matrix3d` and returns a `std::pair<Eigen::Vector3d, Eigen::Vector3d>` where the first element is a vector containing the sum of each column and the second element is a vector containing the maximum absolute value of each column. The function must not modify the input matrix. The sums should be the arithmetic sum of each column (floating-point), and the maximum absolute values should be the largest absolute value (using `std::abs` semantics) among the elements in each column. If the matrix contains NaN or infinity values, the behavior is undefined per Eigen's standard operations, so you may assume finite inputs. The function must be `const`-correct and efficient, avoiding unnecessary copies.

// The solution leverages Eigen's built-in block and coefficient-wise operations to compute column-wise reductions directly, mirroring the original code snippet. For each column, the sum is obtained by calling `.colwise().sum()`, which returns a `RowVector3d`; this is then converted to a `Vector3d` for the return type. For the maximum absolute values, we first apply `cwiseAbs()` to create a matrix of absolute values, then use `.colwise().maxCoeff()` to get the maximum in each column, again converting to a `Vector3d`. No manual loops are needed, so the implementation is concise and uses Eigen's optimized SIMD operations where possible. Edge cases: The matrix is always 3x3, so no empty-matrix concerns. If all elements in a column are zero, the maximum absolute value is zero, which is handled naturally. Time complexity is O(9) = O(1) since the size is fixed; space complexity is O(1) for the temporary row vectors. The function returns by value, which is efficient due to move semantics.

#include <Eigen/Core>
#include <utility>

// Given a 3x3 matrix, return a pair containing:
// - first: Vector3d where each component is the sum of the corresponding column
// - second: Vector3d where each component is the maximum absolute value in the corresponding column
std::pair<Eigen::Vector3d, Eigen::Vector3d> columnWiseStats(const Eigen::Matrix3d& m) {
    // Compute column-wise sums (returns RowVector3d) and convert to Vector3d
    Eigen::Vector3d sums = m.colwise().sum().transpose();
    
    // Compute maximum absolute value per column after applying cwiseAbs
    Eigen::Vector3d maxAbs = m.cwiseAbs().colwise().maxCoeff().transpose();
    
    return {sums, maxAbs};
}

#include <Eigen/Core>
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Identity matrix
    Eigen::Matrix3d m1 = Eigen::Matrix3d::Identity();
    auto result1 = columnWiseStats(m1);
    Eigen::Vector3d expectedSums1(1.0, 1.0, 1.0);
    Eigen::Vector3d expectedMaxAbs1(1.0, 1.0, 1.0);
    assert(result1.first.isApprox(expectedSums1, 1e-12));
    assert(result1.second.isApprox(expectedMaxAbs1, 1e-12));

    // Test 2: Matrix with negative numbers
    Eigen::Matrix3d m2;
    m2 << -1.0, 2.0, -3.0,
           4.0, -5.0, 6.0,
          -7.0, 8.0, -9.0;
    auto result2 = columnWiseStats(m2);
    // Column sums: col0 = -1+4-7 = -4; col1 = 2-5+8 = 5; col2 = -3+6-9 = -6
    Eigen::Vector3d expectedSums2(-4.0, 5.0, -6.0);
    // Column max abs: col0 = max(|-1|,|4|,|-7|) = 7; col1 = max(|2|,|-5|,|8|) = 8; col2 = max(|-3|,|6|,|-9|) = 9
    Eigen::Vector3d expectedMaxAbs2(7.0, 8.0, 9.0);
    assert(result2.first.isApprox(expectedSums2, 1e-12));
    assert(result2.second.isApprox(expectedMaxAbs2, 1e-12));

    // Test 3: All zeros
    Eigen::Matrix3d m3 = Eigen::Matrix3d::Zero();
    auto result3 = columnWiseStats(m3);
    Eigen::Vector3d expectedSums3(0.0, 0.0, 0.0);
    Eigen::Vector3d expectedMaxAbs3(0.0, 0.0, 0.0);
    assert(result3.first.isApprox(expectedSums3, 1e-12));
    assert(result3.second.isApprox(expectedMaxAbs3, 1e-12));

    // Test 4: Matrix with fractional values
    Eigen::Matrix3d m4;
    m4 << 0.5, -1.5, 2.5,
         -0.5, 3.5, -4.5,
          1.5, -2.5, 0.5;
    auto result4 = columnWiseStats(m4);
    // Column sums: col0 = 0.5-0.5+1.5 = 1.5; col1 = -1.5+3.5-2.5 = -0.5; col2 = 2.5-4.5+0.5 = -1.5
    Eigen::Vector3d expectedSums4(1.5, -0.5, -1.5);
    // Column max abs: col0 = max(0.5,0.5,1.5)=1.5; col1 = max(1.5,3.5,2.5)=3.5; col2 = max(2.5,4.5,0.5)=4.5
    Eigen::Vector3d expectedMaxAbs4(1.5, 3.5, 4.5);
    assert(result4.first.isApprox(expectedSums4, 1e-12));
    assert(result4.second.isApprox(expectedMaxAbs4, 1e-12));

    // Test 5: Negative values only
    Eigen::Matrix3d m5;
    m5 << -1.0, -2.0, -3.0,
          -4.0, -5.0, -6.0,
          -7.0, -8.0, -9.0;
    auto result5 = columnWiseStats(m5);
    // Column sums: col0 = -12, col1 = -15, col2 = -18
    Eigen::Vector3d expectedSums5(-12.0, -15.0, -18.0);
    // Column max abs: all are 7,8,9
    Eigen::Vector3d expectedMaxAbs5(7.0, 8.0, 9.0);
    assert(result5.first.isApprox(expectedSums5, 1e-12));
    assert(result5.second.isApprox(expectedMaxAbs5, 1e-12));

    return 0;
}

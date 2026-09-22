// Write a C++ function named `computeMatrixProducts` that takes a 2x2 matrix of doubles (representable via `Eigen::Matrix2d`), two 2D vectors of doubles (`Eigen::Vector2d`), and returns a `std::vector<double>` containing, in this order: (1) the value at row 0, column 0 of the matrix multiplied by itself (`mat * mat`), (2) the dot product of the first vector with the result of `mat * second_vector` (i.e., `u.transpose() * (mat * v)`), (3) the value at row 0, column 0 of the outer product `u * v.transpose()`, and (4) the scalar value of `u.transpose() * (mat * v)` divided by the sum of the absolute values of the matrix entries (if the sum is non-zero, else return 0 for that entry). The function must not modify the input matrix or vectors, must use `const` references for all inputs, and must return an `std::vector<double>` of exactly 4 elements. The final returned vector should be ordered as listed. Assume all inputs are valid (no NaN or infinity).

// The solution uses Eigen’s linear algebra operators. First, compute `mat * mat` to get a `Matrix2d`, extract its (0,0) coefficient. Second, compute `mat * v` (a `Vector2d`), then take the dot product with `u` via `u.dot(temp)` or `u.transpose() * temp` (yielding a scalar). Third, compute the outer product `u * v.transpose()` (a `Matrix2d`), extract its (0,0) coefficient. Fourth, compute the same dot product as in step 2, and the sum of absolute values of `mat`’s four entries; if the sum is not zero, divide the dot product by it, else return 0. Edge cases: if the matrix sum of absolute values is zero (i.e., all entries are zero), the fourth entry becomes 0 to avoid division by zero. The function is `const`-correct since inputs are taken as `const&` and no mutation occurs. Time complexity is O(1) because matrix dimensions are fixed at 2x2. Space complexity is O(1) for temporary Eigen objects, plus the returned vector of size 4.

#include <vector>
#include <Eigen/Dense>

// Compute several matrix/vector products and return a vector of four doubles.
std::vector<double> computeMatrixProducts(
    const Eigen::Matrix2d& mat,
    const Eigen::Vector2d& u,
    const Eigen::Vector2d& v) {
    
    // 1. Extract (0,0) of mat * mat
    Eigen::Matrix2d matSquared = mat * mat;
    double first = matSquared(0,0);
    
    // 2. Dot product u^T * (mat * v)
    Eigen::Vector2d matTimesV = mat * v;          // mat * v
    double dotProduct = u.transpose() * matTimesV; // scalar
    
    // 3. Extract (0,0) of outer product u * v^T
    Eigen::Matrix2d outer = u * v.transpose();
    double third = outer(0,0);
    
    // 4. Dot product divided by sum of absolute values of mat entries
    double sumAbs = mat.cwiseAbs().sum(); // sum of |entries|
    double fourth = (sumAbs != 0.0) ? (dotProduct / sumAbs) : 0.0;
    
    // Return in specified order
    return {first, dotProduct, third, fourth};
}

#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

// (Declaration of computeMatrixProducts from solution is assumed here)

int main() {
    // Test case 1: mat = [[1,2],[3,4]], u=(-1,1), v=(2,0)
    Eigen::Matrix2d mat1;
    mat1 << 1, 2, 3, 4;
    Eigen::Vector2d u1(-1, 1), v1(2, 0);
    std::vector<double> r1 = computeMatrixProducts(mat1, u1, v1);
    // mat*mat = [[7,10],[15,22]], (0,0)=7
    assert(r1[0] == 7.0);
    // mat*v = (2,6), u·(2,6) = -2 +6 = 4
    assert(r1[1] == 4.0);
    // outer = [[-2,0],[2,0]], (0,0)=-2
    assert(r1[2] == -2.0);
    // sumAbs = 1+2+3+4=10, 4/10=0.4
    assert(std::fabs(r1[3] - 0.4) < 1e-12);

    // Test case 2: zero matrix, arbitrary vectors
    Eigen::Matrix2d mat2 = Eigen::Matrix2d::Zero();
    Eigen::Vector2d u2(3, 4), v2(-1, 2);
    std::vector<double> r2 = computeMatrixProducts(mat2, u2, v2);
    assert(r2[0] == 0.0); // (0,0) of zero matrix
    assert(r2[1] == 0.0); // dot product with zero vector result
    assert(r2[2] == -3.0); // (0,0) of outer = 3 * -1
    assert(r2[3] == 0.0); // sumAbs=0 -> returns 0

    // Test case 3: negative values and negative determinants
    Eigen::Matrix2d mat3;
    mat3 << -2, 0, 0, -3;
    Eigen::Vector2d u3(1, -1), v3(0, 5);
    std::vector<double> r3 = computeMatrixProducts(mat3, u3, v3);
    // mat*mat = [[4,0],[0,9]], (0,0)=4
    assert(r3[0] == 4.0);
    // mat*v = (0, -15), u·(0,-15) = 15
    assert(r3[1] == 15.0);
    // outer = [[0,5],[0,-5]], (0,0)=0
    assert(r3[2] == 0.0);
    // sumAbs = 2+0+0+3=5, 15/5=3
    assert(r3[3] == 3.0);
}

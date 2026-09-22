/*
Write a C++ function that takes a fixed-size 4x4 integer matrix (represented as `Eigen::Matrix<int, 4, 4>` or `Eigen::Array44i`) and modifies it in-place so that the top-left 2x2 block is set to zero, the bottom-right 2x2 block is set to all negative ones, and the other two 2x2 blocks (top-right and bottom-left) are left unchanged. The function should return a boolean indicating whether the operation was performed successfully (which is always true for fixed-size matrices). Additionally, write a helper function to print the matrix to standard output for debugging purposes. Use Eigen library constructs like `topLeftCorner<2,2>()`, `bottomRightCorner<2,2>()`, etc., and apply proper `const` correctness where appropriate. The function must not modify the original matrix if it is passed as const; instead, it should return a new modified copy in that case. Provide two overloaded versions: one that modifies in-place (non-const reference) and one that returns a modified copy (const reference input).
*/

#include <Eigen/Dense>
#include <iostream>

// In-place modification: set top-left 2x2 to zero, bottom-right 2x2 to -1.
void applyBlockModification(Eigen::Matrix<int, 4, 4>& mat) {
    mat.topLeftCorner<2,2>().setZero();
    mat.bottomRightCorner<2,2>().setConstant(-1);
}

// Const-correct version: return a modified copy.
Eigen::Matrix<int, 4, 4> applyBlockModification(const Eigen::Matrix<int, 4, 4>& mat) {
    Eigen::Matrix<int, 4, 4> result = mat;  // copy
    result.topLeftCorner<2,2>().setZero();
    result.bottomRightCorner<2,2>().setConstant(-1);
    return result;
}

// Helper to print a 4x4 matrix.
void printMatrix(const Eigen::Matrix<int, 4, 4>& mat) {
    std::cout << mat << std::endl;
}

#include <cassert>
#include <Eigen/Dense>
#include <iostream>

// Declare the functions (they are defined above in the solution, but for the test we include the solution's declarations)
void applyBlockModification(Eigen::Matrix<int, 4, 4>& mat);
Eigen::Matrix<int, 4, 4> applyBlockModification(const Eigen::Matrix<int, 4, 4>& mat);
void printMatrix(const Eigen::Matrix<int, 4, 4>& mat);

int main() {
    // Test 1: In-place modification
    Eigen::Matrix<int, 4, 4> a;
    a << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12,
         13, 14, 15, 16;
    applyBlockModification(a);
    // Expected after modification: top-left 2x2 = 0, bottom-right 2x2 = -1, others unchanged
    Eigen::Matrix<int, 4, 4> expected1;
    expected1 << 0, 0, 3, 4,
                 0, 0, 7, 8,
                 9, 10, -1, -1,
                 13, 14, -1, -1;
    assert((a.array() == expected1.array()).all());

    // Test 2: Const version returns a modified copy, original unchanged
    Eigen::Matrix<int, 4, 4> b;
    b << 10, 20, 30, 40,
         50, 60, 70, 80,
         90, 100, 110, 120,
         130, 140, 150, 160;
    Eigen::Matrix<int, 4, 4> original_b = b;
    Eigen::Matrix<int, 4, 4> result = applyBlockModification(b);
    // Original unchanged?
    assert((b.array() == original_b.array()).all());
    // Result has modifications?
    Eigen::Matrix<int, 4, 4> expected2;
    expected2 << 0, 0, 30, 40,
                 0, 0, 70, 80,
                 90, 100, -1, -1,
                 130, 140, -1, -1;
    assert((result.array() == expected2.array()).all());

    // Test 3: Check that other 2x2 blocks (top-right and bottom-left) are untouched after in-place
    Eigen::Matrix<int, 4, 4> c;
    c.setRandom(); // random values, but we can check specific positions
    c << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12,
         13, 14, 15, 16;
    Eigen::Matrix<int, 4, 4> before = c;
    applyBlockModification(c);
    // Top-right (rows 0-1, cols 2-3) unchanged
    assert(c(0,2) == before(0,2) && c(0,3) == before(0,3) && c(1,2) == before(1,2) && c(1,3) == before(1,3));
    // Bottom-left (rows 2-3, cols 0-1) unchanged
    assert(c(2,0) == before(2,0) && c(2,1) == before(2,1) && c(3,0) == before(3,0) && c(3,1) == before(3,1));
    // Top-left all zero
    assert(c(0,0)==0 && c(0,1)==0 && c(1,0)==0 && c(1,1)==0);
    // Bottom-right all -1
    assert(c(2,2)==-1 && c(2,3)==-1 && c(3,2)==-1 && c(3,3)==-1);

    // Test 4: Print output (optional, but call to ensure no compile issues)
    printMatrix(c);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The key is to leverage Eigen's block operations on fixed-size matrices. For the in-place version, take a non-const reference to a 4x4 integer matrix (or array), access the top-left 2x2 block via `topLeftCorner<2,2>()` and set it to zero with `.setZero()`, access the bottom-right 2x2 block via `bottomRightCorner<2,2>()` and set it to -1 with `.setConstant(-1)`. The other blocks are untouched. For the const-correct version, copy the input matrix into a local variable (since Eigen matrices are copyable), apply the same modifications to the copy, and return it. Edge cases: none, since the size is fixed at 4x4. Time complexity is O(1) because the matrix size is constant and we only modify a fixed number of elements (8 elements total out of 16). Space complexity is O(1) for the in-place version, O(16) for the copy version (constant). Need to include `<Eigen/Dense>` and `<iostream>` for printing. For the assertion test, compare the modified matrix element-by-element or use Eigen's `operator==` which returns a coefficient-wise boolean matrix; we can use `.all()` to check all true.

Write a C++ function named `matrixCombinationResult` that takes three floating-point matrices as parameters: two input matrices `A` and `B` (each of size 2x2), and returns a 2x2 matrix `result` computed as follows. First, compute an element‑wise (coefficient‑wise) sum of matrix `A` with the scalar constant 4 (i.e., add 4 to every element). Then perform an element‑wise product (Hadamard product) of that intermediate matrix and matrix `B`. Finally, multiply the resulting matrix (on the left) by the original matrix `A` using standard matrix multiplication. The function must use the Eigen library for all matrix operations and must be const‑correct (accepting input matrices by const reference). The task is purely computational — no user input, output, or error handling is required. Ensure the function is self‑contained with all necessary headers.
#include <Eigen/Dense>
#include <cassert>

int main() {
    Eigen::MatrixXf A(2,2), B(2,2);
    A << 1, 2,
         3, 4;
    B << 5, 6,
         7, 8;

    Eigen::MatrixXf expected1(2,2);
    // (A+4) = [[5,6],[7,8]], element-wise with B -> [[25,36],[49,64]], times A -> 
    expected1 << 25*1 + 36*3, 25*2 + 36*4,
                 49*1 + 64*3, 49*2 + 64*4;
    // expected1 = [[133, 194], [241, 354]]
    expected1 << 133, 194,
                 241, 354;
    assert(matrixCombinationResult(A, B) == expected1);

    // Test with identity B (element-wise product should be (A+4))
    Eigen::MatrixXf I(2,2);
    I << 1, 1,
         1, 1;  // all ones, element-wise product leaves (A+4) unchanged
    Eigen::MatrixXf expected2 = (A.array() + 4).matrix() * A;
    assert(matrixCombinationResult(A, I) == expected2);

    // Test with zero B
    Eigen::MatrixXf Z(2,2);
    Z << 0, 0,
         0, 0;
    Eigen::MatrixXf zero(2,2);
    zero << 0, 0, 0, 0;
    assert(matrixCombinationResult(A, Z) == zero);

    // Additional check with different values
    Eigen::MatrixXf C(2,2), D(2,2);
    C << 2, -1,
         0, 3;
    D << 1, 4,
         -2, 5;
    // (C+4) = [[6,3],[4,7]], element-wise with D -> [[6,-12],[-8,35]], times C
    // result = [[6*2+(-12)*0, 6*(-1)+(-12)*3], [(-8)*2+35*0, (-8)*(-1)+35*3]]
    // = [[12, -42], [-16, 113]]
    Eigen::MatrixXf expected3(2,2);
    expected3 << 12, -42,
                 -16, 113;
    assert(matrixCombinationResult(C, D) == expected3);
}
#include <Eigen/Dense>

// Compute (A + 4) .* B * A, where .* is element-wise product.
Eigen::MatrixXf matrixCombinationResult(const Eigen::MatrixXf& A, const Eigen::MatrixXf& B) {
    // Add 4 to every element of A (element-wise)
    Eigen::MatrixXf A_plus_4 = (A.array() + 4).matrix();
    // Element-wise product with B
    Eigen::MatrixXf elementwise_product = (A_plus_4.array() * B.array()).matrix();
    // Standard matrix multiplication: result = elementwise_product * A
    return elementwise_product * A;
}
// The solution begins by creating a copy of `A` and adding the constant 4 to each element using the `.array()` interface and then converting back to a matrix via `.matrix()`. Next, the element‑wise product with `B` is obtained by using `.array() * B.array()` and converting the result back to a matrix. This Hadamard product is then multiplied on the right by the original matrix `A` using the `*` operator (which performs standard matrix multiplication for Eigen matrices). Since all inputs are 2x2, the sizes are compatible: the intermediate matrix is 2x2, and multiplying it by `A` (2x2) yields a 2x2 result. The main edge case is ensuring the scalar addition is performed element‑wise and not as a matrix‑scalar multiplication (which would scale the whole matrix) — using `.array()` correctly handles element‑wise semantics. The time complexity is O(1) because the matrices are fixed at 2x2, but for an n×n matrix it would be O(n²) for element‑wise operations and O(n³) for the final matrix multiplication. Space complexity is O(n²) for storing intermediate matrices, but here it is constant.

/*
Write a C++ function with the following signature:  
`template <typename DerivedA, typename DerivedB> void tileMatrix(const Eigen::MatrixBase<DerivedA>& A, int rows, int cols, Eigen::PlainObjectBase<DerivedB>& B)`.  
The function should tile (repeat) the input matrix `A` horizontally `cols` times and vertically `rows` times, storing the result in `B`. For example, if `A` is a 2×3 matrix, `rows=2`, and `cols=3`, then `B` must be a 4×9 matrix where each 2×3 block equals `A`. The function must work for any Eigen dense matrix type (e.g., `Eigen::MatrixXd`, `Eigen::VectorXd`, fixed-size matrices). It must assert that `rows > 0` and `cols > 0`, resize `B` appropriately, and fill it using block assignments. The function should be `const`-correct (i.e., `A` is read-only) and the output `B` must be fully overwritten. Do not use any external libraries other than Eigen (include `<Eigen/Dense>`).
*/

#include <Eigen/Dense>
#include <cassert>

/**
 * @brief Tiles a matrix by repeating it `rows` times vertically and `cols` times horizontally.
 *
 * @param A Input matrix (read-only).
 * @param rows Number of vertical repetitions (must be > 0).
 * @param cols Number of horizontal repetitions (must be > 0).
 * @param B Output matrix (resized and filled with tiled copies of A).
 */
template <typename DerivedA, typename DerivedB>
void tileMatrix(
    const Eigen::MatrixBase<DerivedA>& A,
    int rows,
    int cols,
    Eigen::PlainObjectBase<DerivedB>& B)
{
    // Validate repetition factors
    assert(rows > 0);
    assert(cols > 0);

    // Resize output to accommodate all tiles
    B.resize(rows * A.rows(), cols * A.cols());

    // Copy the input matrix into each tile position
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            B.block(i * A.rows(), j * A.cols(), A.rows(), A.cols()) = A;
        }
    }
}

#include <Eigen/Dense>
#include <cassert>

// Declaration of the solution function (assuming it's in the same translation unit)
template <typename DerivedA, typename DerivedB>
void tileMatrix(
    const Eigen::MatrixBase<DerivedA>& A,
    int rows,
    int cols,
    Eigen::PlainObjectBase<DerivedB>& B);

int main()
{
    // Test 1: 2x2 matrix, tile 2x2 -> 4x4
    Eigen::MatrixXd A1(2, 2);
    A1 << 1, 2,
          3, 4;
    Eigen::MatrixXd B1;
    tileMatrix(A1, 2, 2, B1);
    Eigen::MatrixXd expected1(4, 4);
    expected1 << 1, 2, 1, 2,
                 3, 4, 3, 4,
                 1, 2, 1, 2,
                 3, 4, 3, 4;
    assert(B1 == expected1);

    // Test 2: 1x3 row vector, tile 3x1 -> 3x3
    Eigen::Matrix<double, 1, 3> A2;
    A2 << 5, 6, 7;
    Eigen::MatrixXd B2;
    tileMatrix(A2, 3, 1, B2);
    Eigen::MatrixXd expected2(3, 3);
    expected2 << 5, 6, 7,
                 5, 6, 7,
                 5, 6, 7;
    assert(B2 == expected2);

    // Test 3: 3x1 column vector, tile 1x3 -> 3x3
    Eigen::Vector3d A3(8, 9, 10);
    Eigen::MatrixXd B3;
    tileMatrix(A3, 1, 3, B3);
    Eigen::MatrixXd expected3(3, 3);
    expected3 << 8, 8, 8,
                 9, 9, 9,
                10, 10, 10;
    assert(B3 == expected3);

    // Test 4: 1x1 matrix, tile 2x2 -> 2x2
    Eigen::Matrix<double, 1, 1> A4;
    A4 << 42;
    Eigen::Matrix<double, 2, 2> B4;
    tileMatrix(A4, 2, 2, B4);
    Eigen::Matrix<double, 2, 2> expected4;
    expected4 << 42, 42,
                 42, 42;
    assert(B4 == expected4);

    // Test 5: 2x3 matrix, tile 2x2 -> 4x6
    Eigen::MatrixXd A5(2, 3);
    A5 << 1, 2, 3,
          4, 5, 6;
    Eigen::MatrixXd B5;
    tileMatrix(A5, 2, 2, B5);
    Eigen::MatrixXd expected5(4, 6);
    expected5 << 1, 2, 3, 1, 2, 3,
                 4, 5, 6, 4, 5, 6,
                 1, 2, 3, 1, 2, 3,
                 4, 5, 6, 4, 5, 6;
    assert(B5 == expected5);

    // Test 6: Fixed-size matrix input, dynamic output (ensure no compile errors)
    Eigen::Matrix2d A6;
    A6 << 1, 0,
          0, 1;
    Eigen::MatrixXd B6;
    tileMatrix(A6, 1, 1, B6);
    assert(B6 == A6);

    // Test 7: Empty A (0 rows or columns) is not allowed by Eigen sizing, but we can test a 0x0? Skip.
    // Test 8: Large tile counts (e.g., 10x10 of a 2x2)
    Eigen::MatrixXd B8;
    tileMatrix(A1, 10, 10, B8);
    assert(B8.rows() == 20 && B8.cols() == 20);
    assert(B8.block(0, 0, 2, 2) == A1);
    assert(B8.block(18, 18, 2, 2) == A1);

    // Test 9: Verify const correctness (input can be const)
    const Eigen::MatrixXd C = A1;
    Eigen::MatrixXd B9;
    tileMatrix(C, 2, 1, B9);
    Eigen::MatrixXd expected9(4, 2);
    expected9 << 1, 2,
                 3, 4,
                 1, 2,
                 3, 4;
    assert(B9 == expected9);

    // Test 10: Matrix with negative values
    Eigen::MatrixXd A10(2, 2);
    A10 << -1, -2,
           -3, -4;
    Eigen::MatrixXd B10;
    tileMatrix(A10, 2, 1, B10);
    Eigen::MatrixXd expected10(4, 2);
    expected10 << -1, -2,
                  -3, -4,
                  -1, -2,
                  -3, -4;
    assert(B10 == expected10);

    return 0;
}

// The solution replicates the functionality of MATLAB's `repmat`. The main algorithm is straightforward: first, assert that the repetition factors `rows` and `cols` are positive integers. Then resize the output matrix `B` to have dimensions `(rows * A.rows()) × (cols * A.cols())`. After resizing, iterate over each tile position using nested loops: the outer loop over `i` from 0 to `rows-1` handles vertical placement, and the inner loop over `j` from 0 to `cols-1` handles horizontal placement. For each tile, assign `A` to the corresponding block in `B` using `B.block(i * A.rows(), j * A.cols(), A.rows(), A.cols()) = A;`. This works because `Eigen::PlainObjectBase` supports block assignment and automatic resizing has already made the target block valid. Edge cases: `rows` or `cols` equal to 1 is allowed (produces a single row or column of tiles), and `A` may be a vector (e.g., `Eigen::VectorXd`) or a matrix of any size, including dynamic or fixed-size. The function must handle any type that satisfies `Eigen::MatrixBase<DerivedA>` and `Eigen::PlainObjectBase<DerivedB>`; note that `DerivedB` is typically a matrix type with storage (not a base class). Time complexity is `O(rows * cols * A.size())` because we copy each element of `A` exactly `rows * cols` times. Space complexity is `O(rows * cols * A.size())` for the output matrix `B`, plus negligible auxiliary space.

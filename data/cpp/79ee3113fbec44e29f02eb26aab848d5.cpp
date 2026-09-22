Given a non-empty square matrix of floating-point values (with dimensions at least 1×1), write a C++ function `findMinMaxLocations` that takes a constant reference to an `Eigen::MatrixXf` and returns a struct `MinMaxResult` containing the minimum value, its row and column indices, the maximum value, and its row and column indices. If multiple occurrences of the minimum or maximum exist, return the first occurrence when scanning the matrix in row-major order (i.e., top-to-bottom, left-to-right). The function must not modify the input matrix, must work for any matrix size, and must be declared `const`-correct.
#include <cassert>
#include <Eigen/Dense>

// Include the solution function here or via header.

int main() {
    // 2x2 matrix from the original snippet.
    Eigen::MatrixXf m1(2, 2);
    m1 << 1, 2,
          3, 4;
    MinMaxResult r1 = findMinMaxLocations(m1);
    assert(r1.minValue == 1.0f && r1.minRow == 0 && r1.minCol == 0);
    assert(r1.maxValue == 4.0f && r1.maxRow == 1 && r1.maxCol == 1);

    // Single element matrix.
    Eigen::MatrixXf m2(1, 1);
    m2 << 7.5f;
    MinMaxResult r2 = findMinMaxLocations(m2);
    assert(r2.minValue == 7.5f && r2.minRow == 0 && r2.minCol == 0);
    assert(r2.maxValue == 7.5f && r2.maxRow == 0 && r2.maxCol == 0);

    // All equal values — first occurrence for both is (0,0).
    Eigen::MatrixXf m3(2, 3);
    m3 << 5.0f, 5.0f, 5.0f,
          5.0f, 5.0f, 5.0f;
    MinMaxResult r3 = findMinMaxLocations(m3);
    assert(r3.minValue == 5.0f && r3.minRow == 0 && r3.minCol == 0);
    assert(r3.maxValue == 5.0f && r3.maxRow == 0 && r3.maxCol == 0);

    // Negative values.
    Eigen::MatrixXf m4(2, 2);
    m4 << -3.0f, -1.0f,
          -2.0f, -4.0f;
    MinMaxResult r4 = findMinMaxLocations(m4);
    assert(r4.minValue == -4.0f && r4.minRow == 1 && r4.minCol == 1);
    assert(r4.maxValue == -1.0f && r4.maxRow == 0 && r4.maxCol == 1);

    // Duplicate min and max - first occurrence in row-major order.
    Eigen::MatrixXf m5(3, 1);
    m5 << 2.0f,
          -9.0f,
          2.0f;
    MinMaxResult r5 = findMinMaxLocations(m5);
    assert(r5.minValue == -9.0f && r5.minRow == 1 && r5.minCol == 0);
    assert(r5.maxValue == 2.0f && r5.maxRow == 0 && r5.maxCol == 0);

    // Non-contiguous values with mixed negatives and positives.
    Eigen::MatrixXf m6(2, 3);
    m6 << 0.5f, -2.25f, 3.0f,
          1.0f, -2.25f, 3.0f;
    MinMaxResult r6 = findMinMaxLocations(m6);
    assert(r6.minValue == -2.25f && r6.minRow == 0 && r6.minCol == 1);
    assert(r6.maxValue == 3.0f && r6.maxRow == 0 && r6.maxCol == 2);

    return 0;
}
#include <Eigen/Dense>

struct MinMaxResult {
    float minValue;
    int minRow;
    int minCol;
    float maxValue;
    int maxRow;
    int maxCol;
};

// Finds the first occurrence (row-major) of the minimum and maximum
// values in the given matrix and returns them along with their positions.
MinMaxResult findMinMaxLocations(const Eigen::MatrixXf& m) {
    MinMaxResult result;

    // Use Eigen's built-in coefficient search functions.
    // They output the row and column indices of the first occurrence
    // when scanning the matrix row by row.
    Eigen::Index minRow, minCol, maxRow, maxCol;

    result.minValue = m.minCoeff(&minRow, &minCol);
    result.maxValue = m.maxCoeff(&maxRow, &maxCol);

    // Cast Eigen::Index (typically ptrdiff_t) to int for simplicity.
    result.minRow = static_cast<int>(minRow);
    result.minCol = static_cast<int>(minCol);
    result.maxRow = static_cast<int>(maxRow);
    result.maxCol = static_cast<int>(maxCol);

    return result;
}
// The solution uses Eigen's built-in `minCoeff` and `maxCoeff` member functions, which already accept output parameters for the row and column indices of the first occurrence in row-major order. The algorithm is straightforward: call `minCoeff(&minRow, &minCol)` and `maxCoeff(&maxRow, &maxCol)` on the matrix, store the values and indices into a struct, and return it. Edge cases: the matrix is guaranteed non-empty, so no need to handle empty inputs. For single-element matrices, both min and max are that element, and indices are (0,0). The time complexity is O(rows × cols), because Eigen’s coefficient search scans each element once. The space complexity is O(1) beyond the input matrix, storing only the result struct and index variables.

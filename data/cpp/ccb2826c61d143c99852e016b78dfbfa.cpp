Write a C++ function that performs partial pivoting on a square matrix and its corresponding right-hand side vector for a system of linear equations \(Ax = b\). The function should accept a mutable matrix \(A\) (as `std::vector<std::vector<double>>`) and a mutable vector \(b\) (as `std::vector<double>`), and modify them in place so that the largest absolute value in each pivot column is moved to the diagonal position before elimination. For each column index \(i\) (from 0 to \(n-2\)), find the row \(j \geq i\) with the maximum absolute value of \(A[j][i]\). If that row differs from \(i\), swap the entire row \(i\) with row \(j\) in \(A\), and swap the corresponding entries in \(b\). If the matrix is singular (the maximum absolute pivot value is zero), throw a `std::runtime_error` indicating a singular matrix. The function must handle matrices of size \(n \times n\) for any \(n \geq 1\). It should be `const`-correct where appropriate, and must not allocate unnecessary temporary storage beyond a few local scalars or arrays.
#include <cassert>
#include <vector>
#include <stdexcept>

// Include the solution function declaration here (already defined above).

int main() {
    // Test 1: Simple 3x3 system, no pivoting needed.
    {
        std::vector<std::vector<double>> A = {{4.0, 0.0, 0.0},
                                              {0.0, 3.0, 0.0},
                                              {0.0, 0.0, 2.0}};
        std::vector<double> b = {1.0, 2.0, 3.0};
        partialPivot(A, b);
        assert(A[0][0] == 4.0 && A[1][1] == 3.0 && A[2][2] == 2.0);
        assert(b == std::vector<double>({1.0, 2.0, 3.0}));
    }

    // Test 2: Matrix that requires row swaps to bring largest elements to diagonal.
    {
        std::vector<std::vector<double>> A = {{1.0, 2.0, 3.0},
                                              {10.0, 1.0, 0.0},
                                              {5.0, 0.0, 1.0}};
        std::vector<double> b = {1.0, 2.0, 3.0};
        partialPivot(A, b);
        // After pivoting, the first column's largest absolute value (10) is in row 0.
        assert(A[0][0] == 10.0);
        assert(A[1][1] == 1.0); // Row 1 was original row 0, but column 1 may be 2?
        // Verify the exact swapped matrix:
        // Original rows: R0=[1,2,3], R1=[10,1,0], R2=[5,0,1]
        // Swap rows 0 and 1 because |10| > |1|. After swap:
        // A = [[10,1,0], [1,2,3], [5,0,1]], b = [2,1,3]
        std::vector<std::vector<double>> expectedA = {{10.0, 1.0, 0.0},
                                                      {1.0, 2.0, 3.0},
                                                      {5.0, 0.0, 1.0}};
        std::vector<double> expectedB = {2.0, 1.0, 3.0};
        assert(A == expectedA);
        assert(b == expectedB);
    }

    // Test 3: 2x2 matrix with negative values.
    {
        std::vector<std::vector<double>> A = {{-2.0, 1.0},
                                              {3.0, 4.0}};
        std::vector<double> b = {5.0, -6.0};
        partialPivot(A, b);
        // Row 1 has |3| > |-2|, so swap rows.
        std::vector<std::vector<double>> expectedA = {{3.0, 4.0},
                                                      {-2.0, 1.0}};
        std::vector<double> expectedB = {-6.0, 5.0};
        assert(A == expectedA);
        assert(b == expectedB);
    }

    // Test 4: 1x1 system (no pivoting needed, pivot is the only element).
    {
        std::vector<std::vector<double>> A = {{7.0}};
        std::vector<double> b = {42.0};
        partialPivot(A, b);
        assert(A[0][0] == 7.0);
        assert(b[0] == 42.0);
    }

    // Test 5: Singular matrix should throw.
    {
        std::vector<std::vector<double>> A = {{1.0, 2.0},
                                              {2.0, 4.0}};
        std::vector<double> b = {1.0, 2.0};
        bool threw = false;
        try {
            partialPivot(A, b);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 6: Singular with a zero pivot on last column after swaps.
    {
        std::vector<std::vector<double>> A = {{0.0, 1.0},
                                              {0.0, 2.0}};
        std::vector<double> b = {1.0, 2.0};
        // First column max abs is 0 (both zero), throw immediately.
        bool threw = false;
        try {
            partialPivot(A, b);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}
#include <vector>
#include <stdexcept>
#include <cmath>

// Perform partial pivoting on matrix A and vector b in place.
// A must be square: number of rows == number of columns == b.size().
// Throws std::runtime_error if the matrix is singular.
void partialPivot(std::vector<std::vector<double>>& A, std::vector<double>& b) {
    const int n = static_cast<int>(A.size());
    if (n == 0) return;

    for (int i = 0; i < n - 1; ++i) {
        // Find the row with the largest absolute value in column i, from row i down.
        int pivotRow = i;
        double maxAbs = std::fabs(A[i][i]);
        for (int j = i + 1; j < n; ++j) {
            double absVal = std::fabs(A[j][i]);
            if (absVal > maxAbs) {
                maxAbs = absVal;
                pivotRow = j;
            }
        }

        // Check for singular matrix (zero pivot).
        if (maxAbs == 0.0) {
            throw std::runtime_error("Singular matrix detected during pivoting.");
        }

        // Swap rows if necessary.
        if (pivotRow != i) {
            // Swap row i and pivotRow in A.
            for (int k = 0; k < n; ++k) {
                std::swap(A[i][k], A[pivotRow][k]);
            }
            // Swap corresponding entries in b.
            std::swap(b[i], b[pivotRow]);
        }
    }

    // For the last column (n-1), we already handled all previous columns,
    // but we must ensure the final pivot is not zero.
    if (n > 0 && A[n-1][n-1] == 0.0) {
        throw std::runtime_error("Singular matrix detected during pivoting.");
    }
}
// The solution is straightforward Gaussian elimination partial pivoting. For each pivot column \(i\), scan rows \(i\) through \(n-1\) to find the row index with the largest absolute value in that column. If the maximum is zero (or very small, but we'll check exact zero for simplicity), the matrix is singular and we throw an exception. If the pivot row is not the current row, swap the entire rows in \(A\) and the corresponding elements in \(b\). This ensures numerical stability by reducing round-off errors during elimination. The algorithm runs in \(O(n^2)\) time because scanning each column from row \(i\) to \(n-1\) across all columns yields a geometric series. Space complexity is \(O(1)\) auxiliary, since only a few local variables (scalars and a small array for row swapping) are used. Edge cases include \(n=1\) (no pivoting needed, but the singular check must still be applied), matrices where the only candidate pivot in a column is zero (throw), and matrices with negative values (use `std::fabs` or manual absolute value). The implementation should use `double` for floating-point precision, though the original snippet used `float`.

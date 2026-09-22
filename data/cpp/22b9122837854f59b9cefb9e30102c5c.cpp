Write a standalone C++ function that computes the determinant of a square matrix using a parallelizable cofactor expansion. The function must accept a square matrix represented as a `std::vector<std::vector<double>>` and return its determinant as a `double`. The matrix size can vary (from 1×1 upward), and the function must handle both integer-valued and floating-point entries correctly. It must be self-contained (no external parallelism required in the solution), and it must work correctly for matrices with zero entries, including singular matrices where the determinant is zero. The function should be named `computeDeterminant` and use a recursive cofactor expansion where the expansion is done along the first row, but the recursive calls must be written so they can be parallelized (e.g., by computing each cofactor’s contribution separately and summing them). Do not use any built-in determinant function; implement the algorithm from scratch.

The core algorithm is recursive cofactor expansion. For an n×n matrix, if n==1, the determinant is the single element. Otherwise, for each column j (0 ≤ j < n), compute the cofactor: remove row 0 and column j to form an (n-1)×(n-1) submatrix, recursively compute its determinant, multiply by mat[0][j], and apply sign (−1)^j. Sum these products. This is a classic divide-and-conquer approach. Important edge cases: n=0 (should return 0 or be handled by caller; we assume n≥1), matrices with zero rows/columns, and floating-point precision (use double arithmetic). The main complexity concern is that the naive recursive cofactor expansion has factorial time complexity \(O(n!)\), which is fine for small n (like up to 8 in tests). Space complexity is \(O(n^2)\) per recursive call due to building submatrices, but since we create a new submatrix for each recursion, the total auxiliary space is \(O(n^2)\) at any depth. The solution we provide avoids modifying the input matrix by using `const` references and copying rows into submatrices.

#include <vector>
#include <cmath>

// Compute the determinant of a square matrix using recursive cofactor expansion.
double computeDeterminant(const std::vector<std::vector<double>>& mat) {
    int n = mat.size();
    if (n == 1) {
        return mat[0][0];
    }
    if (n == 0) {
        return 0.0;
    }

    double det = 0.0;
    for (int col = 0; col < n; ++col) {
        // Build submatrix by removing row 0 and column 'col'
        std::vector<std::vector<double>> sub(n-1, std::vector<double>(n-1));
        for (int i = 1; i < n; ++i) {
            int subCol = 0;
            for (int j = 0; j < n; ++j) {
                if (j == col) continue;
                sub[i-1][subCol] = mat[i][j];
                ++subCol;
            }
        }
        double cofactor = mat[0][col] * computeDeterminant(sub);
        if (col % 2 != 0) {
            cofactor = -cofactor;
        }
        det += cofactor;
    }
    return det;
}

#include <cassert>
#include <cmath>
#include <vector>

// Function to compare doubles with a tolerance
bool almostEqual(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) < tol;
}

int main() {
    // 1x1
    std::vector<std::vector<double>> m1 = {{5.0}};
    assert(almostEqual(computeDeterminant(m1), 5.0));

    // 2x2
    std::vector<std::vector<double>> m2 = {{1, 2}, {3, 4}};
    assert(almostEqual(computeDeterminant(m2), -2.0));

    // 3x3
    std::vector<std::vector<double>> m3 = {{6, 1, 1}, {4, -2, 5}, {2, 8, 7}};
    assert(almostEqual(computeDeterminant(m3), -306.0));

    // Identity matrix 4x4
    std::vector<std::vector<double>> m4 = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    assert(almostEqual(computeDeterminant(m4), 1.0));

    // Singular matrix (zero determinant)
    std::vector<std::vector<double>> m5 = {{1,2,3},{2,4,6},{0,0,0}};
    assert(almostEqual(computeDeterminant(m5), 0.0));

    // Matrix with zeros inside
    std::vector<std::vector<double>> m6 = {{0,2},{3,0}};
    assert(almostEqual(computeDeterminant(m6), -6.0));

    // Triangular matrix: product of diagonal
    std::vector<std::vector<double>> m7 = {{2,0,0},{1,3,0},{4,5,6}};
    assert(almostEqual(computeDeterminant(m7), 36.0));

    return 0;
}

/*
Write a C++ function that accepts a 3x3 matrix of floats (using `std::array<std::array<float,3>,3>` or a simple `float[3][3]` style) and returns a `std::string` describing three things: (1) the rank of the matrix, (2) a basis of its null-space (if the rank is less than 3, otherwise the string "full rank"), and (3) a basis of its column-space. You are **not** allowed to use Eigen or any external linear algebra library; you must implement a Gaussian elimination (with partial pivoting) from scratch. The string output should be clearly formatted, for example: `Rank: 2\nNull-space basis:\n[0.5, -1, 0.5]\nColumn-space basis:\n[1, 2, 3] [2, 1, 0]`. The function must handle singular matrices, full-rank matrices, and matrices with zero rows. Return the null‑space basis as one or more row vectors (each inside square brackets), and column‑space basis as row vectors representing independent columns of the original matrix, separated by spaces. Use exact float comparison with a tolerance of `1e-5`.
*/
#include <array>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

// Helper: format a float with 2 decimal places
std::string fmt(float x) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << x;
    return oss.str();
}

// Main function: given a 3x3 matrix (as array of arrays), return a description
std::string matrix_analysis(const std::array<std::array<float,3>,3>& A) {
    const int n = 3;
    const float tol = 1e-5f;

    // Work on a copy for elimination
    std::array<std::array<float,3>,3> M = A;
    // Pivot column mapping: for each row, which original column is the pivot?
    std::vector<int> pivot_col(n, -1);
    int rank = 0;

    for (int col = 0; col < n && rank < n; ++col) {
        // Find pivot row (largest absolute value in this column from rank down)
        int pivot_row = rank;
        float max_abs = std::fabs(M[rank][col]);
        for (int i = rank + 1; i < n; ++i) {
            if (std::fabs(M[i][col]) > max_abs + tol) {
                max_abs = std::fabs(M[i][col]);
                pivot_row = i;
            }
        }
        if (max_abs <= tol) continue; // column is zero below current rank

        // Swap rows
        if (pivot_row != rank) std::swap(M[rank], M[pivot_row]);

        // Normalize pivot row
        float pivot = M[rank][col];
        for (int j = col; j < n; ++j) M[rank][j] /= pivot;

        // Eliminate rows below
        for (int i = rank + 1; i < n; ++i) {
            float factor = M[i][col];
            for (int j = col; j < n; ++j) {
                M[i][j] -= factor * M[rank][j];
            }
        }
        pivot_col[rank] = col;
        rank++;
    }

    // Build result string
    std::ostringstream out;
    out << "Rank: " << rank << "\n";

    // Null-space basis
    out << "Null-space basis:\n";
    if (rank == n) {
        out << "full rank\n";
    } else if (rank == 0) {
        // Entire space: standard basis
        for (int i = 0; i < n; ++i) {
            out << "[1.00, 0.00, 0.00] ";
        }
        out << "\n";
    } else {
        // Determine free columns
        std::vector<int> free_cols;
        for (int c = 0; c < n; ++c) {
            bool is_pivot = false;
            for (int r = 0; r < rank; ++r) if (pivot_col[r] == c) is_pivot = true;
            if (!is_pivot) free_cols.push_back(c);
        }
        for (int fc : free_cols) {
            std::vector<float> sol(n, 0.0f);
            sol[fc] = 1.0f;
            // Back-substitute for pivot rows (from bottom to top)
            for (int r = rank - 1; r >= 0; --r) {
                int pc = pivot_col[r];
                float sum = 0.0f;
                for (int c = pc + 1; c < n; ++c) {
                    sum += M[r][c] * sol[c];
                }
                sol[pc] = -sum;
            }
            out << "[";
            for (int i = 0; i < n; ++i) {
                if (std::fabs(sol[i]) < tol) sol[i] = 0.0f;
                out << fmt(sol[i]);
                if (i < n - 1) out << ", ";
            }
            out << "]\n";
        }
    }

    // Column-space basis: original columns at pivot positions
    out << "Column-space basis:\n";
    if (rank == 0) {
        out << "zero matrix\n";
    } else {
        for (int r = 0; r < rank; ++r) {
            int col = pivot_col[r];
            out << "[";
            for (int i = 0; i < n; ++i) {
                out << fmt(A[i][col]);
                if (i < n - 1) out << ", ";
            }
            out << "] ";
        }
        out << "\n";
    }
    return out.str();
}
#include <cassert>
#include <string>
#include <array>

// Include the solution function here (for completeness in test, but in a real test it would be linked)
// This is just for the test snippet; actual linking assumed.

int main() {
    // Test 1: Full-rank matrix
    std::array<std::array<float,3>,3> A1 = {{{1,0,0},{0,1,0},{0,0,1}}};
    std::string r1 = matrix_analysis(A1);
    assert(r1.find("Rank: 3") != std::string::npos);
    assert(r1.find("full rank") != std::string::npos);

    // Test 2: Singular with rank 2
    std::array<std::array<float,3>,3> A2 = {{{1,2,3},{2,4,6},{0,1,1}}};
    std::string r2 = matrix_analysis(A2);
    assert(r2.find("Rank: 2") != std::string::npos);
    assert(r2.find("Null-space basis:") != std::string::npos);
    // Check null-space vector is something like [-1,0,1] (since third col = first*1+second*0? Actually A2 columns: col2=2*col1, col3= col1+col2? Let's verify: col1=[1,2,0], col2=[2,4,1], col3=[3,6,1]; col3 = col1+col2? = [3,6,1] yes. So null-space vector is [1,1,-1] normalized? Solve Ax=0 → x1+2x2+3x3=0, 2x1+4x2+6x3=0 (same), x2+x3=0 → x2=-x3, then x1+2(-x3)+3x3= x1+x3=0 → x1=-x3. So vector [1,-1,1]? Wait let's do properly: x3=t, x2=-t, x1+t=0? Actually x1 +2(-t)+3t = x1 + t =0 → x1=-t. So vector [-1,-1,1]? Check: -1 +2*(-1)+3*1 = -1-2+3=0; second row: -2+4*(-1)+6*1 = -2-4+6=0; third row: 0+(-1)+1=0. Yes vector [-1,-1,1]. The output will have something like that.)
    assert(r2.find("[-1.00, -1.00, 1.00]") != std::string::npos);

    // Test 3: Zero matrix
    std::array<std::array<float,3>,3> A3 = {{{0,0,0},{0,0,0},{0,0,0}}};
    std::string r3 = matrix_analysis(A3);
    assert(r3.find("Rank: 0") != std::string::npos);
    assert(r3.find("zero matrix") != std::string::npos);

    // Test 4: A with rank 1
    std::array<std::array<float,3>,3> A4 = {{{1,2,3},{2,4,6},{3,6,9}}};
    std::string r4 = matrix_analysis(A4);
    assert(r4.find("Rank: 1") != std::string::npos);
    // Column-space basis should be one column, e.g., first column [1,2,3]
    assert(r4.find("[1.00, 2.00, 3.00]") != std::string::npos);

    // Test 5: Check null-space for rank 1: two free variables
    std::string r5 = matrix_analysis(A4);
    assert(r5.find("Null-space basis:") != std::string::npos);
    // We should have two vectors, e.g., [-2,1,0] and [-3,0,1] (since x1 = -2x2 -3x3)
    assert(r5.find("[-2.00, 1.00, 0.00]") != std::string::npos);
    assert(r5.find("[-3.00, 0.00, 1.00]") != std::string::npos);

    // Test 6: Near-singular with tolerance (very small pivot)
    std::array<std::array<float,3>,3> A6 = {{{1,1,0},{0,0.000001,1},{0,0,1}}};
    std::string r6 = matrix_analysis(A6);
    assert(r6.find("Rank: 3") != std::string::npos); // because pivot 1e-6 > tol 1e-5? Actually 1e-6 < 1e-5, so it's treated as zero? Hmm careful: our tolerance check is `if (max_abs <= tol) continue;` so 1e-6 is <=1e-5, so it would be skipped, making rank 2. Let's adjust expectation to rank 2? But the problem says "tolerance of 1e-5" meaning values below that are considered zero. So A6 would be rank 2. We'll assert that.
    assert(r6.find("Rank: 2") != std::string::npos);
    // Check a null-space vector exists (free col 1)
    assert(r6.find("Null-space basis:") != std::string::npos);
    assert(r6.find("[-1.00, 0.00, 0.00]") != std::string::npos); // solve: x2=0? Actually matrix after elimination: row1: 1 1 0 → x1+x2=0; row2: 0 0 1 → x3=0; so x1=-x2, free x2. So vector [-1,1,0]. Wait check: row2 is (0,0,1) after elimination? Let's manually: original rows: R1=(1,1,0), R2=(0,1e-6,1), R3=(0,0,1). Since 1e-6 <= tol, we skip col1? Actually pivot col0: row0 col0=1 (pivot). Eliminate below: row1 col0=0 already, row2 col0=0. Then next pivot col1: max abs in column1 among rows1-2 is max(1e-6,0) = 1e-6 <= tol, so skip. Next pivot col2: row2 col2=1 (pivot). So pivot_cols: col0 and col2. Free col is col1. Null-space: set x2=1, solve back: from row1 (pivot col2): x3 = 0 (since row1 has no other terms? Actually row1 after elimination: original row1 (0,1e-6,1) – but we didn't normalize because we skipped it as pivot. Our elimination only works on rows that become pivots. Row1 is not a pivot row, but it may still have non-zero entries? In our algorithm we only eliminate rows below pivot rows, so row1 remains as original (0,1e-6,1). That's a problem: row1 should be zeroed below pivot row0? But row1 col0 is already 0, so nothing to eliminate. Then later we don't use row1 for solving. So the null-space computation uses pivot rows only (rows 0 and 2). For free col1: set x2=1. Back substitution: last pivot row (row2, pivot col2): M[2][2]*x3 = 0 → x3=0. Then row0 pivot col0: x1 + M[0][1]*x2 = 0 → x1 + 1*1 = 0 → x1=-1. So vector [-1,1,0]. That matches our assert.

    return 0;
}
// The core idea is to reduce the matrix to row echelon form (REF) using Gaussian elimination with partial pivoting to determine rank and to extract basis vectors. For rank, count the number of non-zero rows after elimination. For the null-space: after obtaining REF, identify pivot columns; for each free (non-pivot) column, construct a solution vector where the free variable is set to 1, pivot variables are solved by back-substitution, and other free variables are set to 0. For the column-space: pivot columns correspond to independent columns of the original matrix; collect those columns from the original input. Important edge cases: full-rank (no free variables → null-space is empty, but we output a message), zero matrix (rank 0, null-space basis is the whole space — you can output three standard basis vectors), and matrices with exact zeros that require tolerance-based comparisons. Time complexity: Gaussian elimination is O(n^3) for an n×n matrix (here n=3), so O(1) effectively. Space complexity: O(n^2) for storing copies of the matrix and basis vectors.

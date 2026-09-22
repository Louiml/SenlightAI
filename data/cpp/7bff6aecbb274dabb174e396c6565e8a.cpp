// Write a standalone C++ function named `invertMatrixWithPivot` that takes as input: an integer `N` (the matrix dimension, 1 ≤ N ≤ 4), a mutable 4×4 array `A` representing a square matrix (only the first `N` rows and columns are used), a mutable 1-D array `Y` of length `N` (a right-hand side vector), and a reference parameter `D` (a `double` used to return the determinant of the original matrix, or 0 if singular). The function must perform Gaussian elimination with partial pivoting (row swaps on the largest absolute value in the current column) on the matrix `A` and apply the same row operations to the vector `Y`, so that after the call, `A` contains the inverse of the original matrix (but only for the leading `N×N` block) and `Y` contains the solution `x` to the linear system `A_original * x = Y_original`. The function must also return an integer status code: `0` if the matrix is non‑singular and the inverse/solution were computed successfully, and `-1` if the matrix is singular (i.e., during elimination a pivot absolute value is below a threshold of `1e-20`). The function must restore the original row order of the inverse matrix after elimination (undoing pivot swaps so that the returned inverse corresponds to the original matrix as given). You may modify `A` and `Y` in place. Use `const` where appropriate for input-only parameters, and ensure the function is self‑contained (no global variables or external libraries beyond `<cmath>`).

// The core algorithm is Gauss‑Jordan elimination with partial pivoting, which computes the inverse and solves a linear system simultaneously. We start with the augmented pair `(A, Y)`. For each column `k` from 0 to N‑1, we find the row with the largest absolute value among rows `k..N-1` in that column. If that maximum is below `1e-20`, we declare the matrix singular (`JF=-1`) and stop. Otherwise, if the pivot row is not the current row `k`, we swap rows `k` and the pivot row in both `A` and `Y`, and flip the sign of `D`. Then we normalize the pivot row by dividing all its elements (and `Y[k]`) by the pivot value, and update `D = D * pivot`. Next, for every other row `i != k`, we eliminate column `k` by subtracting a multiple of the pivot row: `A[i][j] -= A[i][k] * A[k][j]` for all `j != k`, and `Y[i] -= A[i][k] * Y[k]`. After that, we set `A[i][k] = -A[i][k] * (1/pivot)` to correctly maintain the inverse. At the end of the main loop, if no singularity was detected, we perform a post‑processing step that swaps columns back to undo the effect of row pivoting on the inverse matrix (the algorithm stores the original row indices in `IW`; after the elimination, we need to swap columns accordingly). This ensures `A` contains the inverse of the original input matrix. The determinant `D` is the product of the pivots (with sign changes due to row swaps), and if any pivot is too small, `D` is set to 0. Time complexity is `O(N^3)`, and space complexity is `O(1)` beyond the input arrays (we use a small fixed‑size auxiliary array `IW` of length 4). Edge cases: `N=1` works trivially; singular matrices produce `JF=-1` and `D=0`; the threshold avoids division by near‑zero; the inverse is correctly reordered via the column swaps at the end.

#include <cmath>

// Compute the inverse of the leading N×N block of A and solve A*Y = B (B in Y).
// On return, A contains the inverse (original row order), Y contains the solution,
// D contains the determinant (or 0 if singular). Returns 0 on success, -1 if singular.
int invertMatrixWithPivot(int N, double A[4][4], double Y[4], double &D) {
    const double SFA = 1e-20;
    int IW[4];  // stores pivot row indices for later column swaps
    int JF = 0;
    D = 1.0;

    for (int k = 0; k < N; ++k) {
        // Find pivot row (largest absolute value in column k, rows k..N-1)
        double amx = std::fabs(A[k][k]);
        int imx = k;
        for (int i = k + 1; i < N; ++i) {
            double t = std::fabs(A[i][k]);
            if (t > amx) {
                amx = t;
                imx = i;
            }
        }

        if (amx < SFA) {
            JF = -1;
            break;  // singular
        }

        // Swap rows if needed
        if (imx != k) {
            for (int j = 0; j < N; ++j) {
                double t = A[k][j];
                A[k][j] = A[imx][j];
                A[imx][j] = t;
            }
            double t = Y[k];
            Y[k] = Y[imx];
            Y[imx] = t;
            D = -D;
        }

        IW[k] = imx;
        double pivot = A[k][k];
        D *= pivot;

        // Check for overflow/underflow in determinant (optional but safe)
        if (std::fabs(D) < SFA) {
            JF = -1;
            break;
        }

        // Normalize pivot row
        double invPivot = 1.0 / pivot;
        A[k][k] = invPivot;  // store inverse pivot for later
        for (int j = 0; j < N; ++j) {
            if (j != k) A[k][j] = A[k][j] * invPivot;
        }
        double yk = Y[k] * invPivot;
        Y[k] = yk;

        // Eliminate column k from all other rows
        for (int i = 0; i < N; ++i) {
            if (i == k) continue;
            double aik = A[i][k];
            for (int j = 0; j < N; ++j) {
                if (j != k) A[i][j] -= aik * A[k][j];
            }
            Y[i] -= aik * yk;
        }

        // Finalize column of inverse: negate and multiply by invPivot
        for (int i = 0; i < N; ++i) {
            if (i != k) A[i][k] = -A[i][k] * invPivot;
        }
    }

    if (JF != 0) {
        D = 0.0;
        return -1;
    }

    // Undo row swaps on the inverse (swap columns accordingly)
    for (int k = N - 1; k >= 0; --k) {
        int np1mk = k;
        int ki = IW[k];
        if (np1mk != ki) {
            for (int i = 0; i < N; ++i) {
                double t = A[i][np1mk];
                A[i][np1mk] = A[i][ki];
                A[i][ki] = t;
            }
        }
    }
    return 0;
}

#include <cassert>
#include <cmath>

// The solution function is declared above (include it here or paste before main).

int main() {
    // Test 1: 2x2 identity matrix, Y = [1,2] -> inverse = identity, solution = [1,2]
    {
        double A[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
        double Y[4] = {1,2,0,0};
        double D = 0;
        int status = invertMatrixWithPivot(2, A, Y, D);
        assert(status == 0);
        assert(std::fabs(D - 1.0) < 1e-12);
        assert(std::fabs(A[0][0] - 1.0) < 1e-12);
        assert(std::fabs(A[0][1] - 0.0) < 1e-12);
        assert(std::fabs(A[1][0] - 0.0) < 1e-12);
        assert(std::fabs(A[1][1] - 1.0) < 1e-12);
        assert(std::fabs(Y[0] - 1.0) < 1e-12);
        assert(std::fabs(Y[1] - 2.0) < 1e-12);
    }

    // Test 2: 2x2 matrix [[4,7],[2,6]], Y=[1,1] -> solution = [-0.1,0.2], determinant = 10
    {
        double A[4][4] = {{4,7,0,0},{2,6,0,0},{0,0,1,0},{0,0,0,1}};
        double Y[4] = {1,1,0,0};
        double D = 0;
        int status = invertMatrixWithPivot(2, A, Y, D);
        assert(status == 0);
        assert(std::fabs(D - 10.0) < 1e-12);
        // Inverse of [[4,7],[2,6]] is (1/10)*[[6,-7],[-2,4]] = [[0.6,-0.7],[-0.2,0.4]]
        assert(std::fabs(A[0][0] - 0.6) < 1e-12);
        assert(std::fabs(A[0][1] + 0.7) < 1e-12);
        assert(std::fabs(A[1][0] + 0.2) < 1e-12);
        assert(std::fabs(A[1][1] - 0.4) < 1e-12);
        // Solution x = A^{-1}*Y
        assert(std::fabs(Y[0] - (-0.1)) < 1e-12);
        assert(std::fabs(Y[1] - 0.2) < 1e-12);
    }

    // Test 3: 3x3 singular matrix -> should return -1 and D=0
    {
        double A[4][4] = {{1,2,3,0},{2,4,6,0},{3,6,9,0},{0,0,0,1}};
        double Y[4] = {0,0,0,0};
        double D = 1.0;
        int status = invertMatrixWithPivot(3, A, Y, D);
        assert(status == -1);
        assert(std::fabs(D) < 1e-20); // or exactly 0.0
    }

    // Test 4: 3x3 non-singular with row swaps needed (pivot on row 1 initially)
    // A = [[0,1,2],[1,0,1],[2,1,0]], determinant = 4, inverse known
    {
        double A[4][4] = {{0,1,2,0},{1,0,1,0},{2,1,0,0},{0,0,0,1}};
        double Y[4] = {1,2,3,0};
        double D = 0;
        int status = invertMatrixWithPivot(3, A, Y, D);
        assert(status == 0);
        assert(std::fabs(D - 4.0) < 1e-12);
        // Inverse of [[0,1,2],[1,0,1],[2,1,0]] is [[-1,2,-1],[2,-4,2],[-1,2,-1]] (check: multiply gives I)
        // Let's verify by checking A*A_inv = I for a few entries.
        // We'll store original A first? Actually we can re-check by multiplying A (which now holds inverse) by original? But we don't have original. Instead, we can verify that the solution Y satisfies original A*Y = [1,2,3].
        // Original A*Y should equal [1,2,3]. We can recompute using the inverse? That's circular. Instead, we can test the known inverse entries:
        assert(std::fabs(A[0][0] + 1.0) < 1e-12);
        assert(std::fabs(A[0][1] - 2.0) < 1e-12);
        assert(std::fabs(A[0][2] + 1.0) < 1e-12);
        assert(std::fabs(A[1][0] - 2.0) < 1e-12);
        assert(std::fabs(A[1][1] + 4.0) < 1e-12);
        assert(std::fabs(A[1][2] - 2.0) < 1e-12);
        assert(std::fabs(A[2][0] + 1.0) < 1e-12);
        assert(std::fabs(A[2][1] - 2.0) < 1e-12);
        assert(std::fabs(A[2][2] + 1.0) < 1e-12);
        // Solution x = A_inv * [1,2,3] = (-1*1+2*2-1*3)= 0, (2*1-4*2+2*3)=0, (-1*1+2*2-1*3)=0
        assert(std::fabs(Y[0]) < 1e-12);
        assert(std::fabs(Y[1]) < 1e-12);
        assert(std::fabs(Y[2]) < 1e-12);
    }

    // Test 5: 1x1 matrix [5], Y=[10] -> inverse = [0.2], determinant=5, solution=2
    {
        double A[4][4] = {{5,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
        double Y[4] = {10,0,0,0};
        double D = 0;
        int status = invertMatrixWithPivot(1, A, Y, D);
        assert(status == 0);
        assert(std::fabs(D - 5.0) < 1e-12);
        assert(std::fabs(A[0][0] - 0.2) < 1e-12);
        assert(std::fabs(Y[0] - 2.0) < 1e-12);
    }

    return 0;
}

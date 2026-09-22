/*
Write a standalone C++ function `applyLeftGivens` that applies a sequence of elementary Givens rotations to the left of a rectangular matrix (i.e., it pre-multiplies the matrix by a product of rotation matrices), operating on a specified contiguous submatrix defined by row indices `m1` to `m2` (inclusive, 0-based) and column indices `n1` to `n2` (inclusive, 0-based). The rotations are given as two vectors `c` and `s` of length `m2 - m1`, where the `k`-th rotation (0-based) acts on rows `m1 + k` and `m1 + k + 1`. If the boolean parameter `isForward` is `true`, apply the rotations in order from the top pair of rows downward; if `false`, apply them in reverse order from the bottom pair upward. Each rotation transforms two rows of the submatrix as follows: for each column index `col` in `[n1, n2]`, the new values are `row_i' = c_k * row_i - s_k * row_{i+1}` and `row_{i+1}' = s_k * row_i + c_k * row_{i+1}` (using old values). The function must handle the case where the submatrix is empty (m1 > m2 or n1 > n2) by doing nothing, and must correctly process the special case where `n1 == n2` (single column). The function should modify the matrix in place and may use a temporary vector for workspace. The code must not use any external libraries; use `std::vector<std::vector<double>>` for the matrix and `std::vector<double>` for coefficient and workspace arrays. The function signature should be: `void applyLeftGivens(bool isForward, int m1, int m2, int n1, int n2, const std::vector<double>& c, const std::vector<double>& s, std::vector<std::vector<double>>& A, std::vector<double>& work)`. Ensure the function is robust to coefficient vectors that may contain values equal to 1.0 and 0.0 (identity rotations) and skips unnecessary work in such cases.
*/

#include <vector>
#include <cmath>

// Apply a sequence of Givens rotations from the left to a submatrix of A.
// Each rotation k (k = 0..m2-m1-1) acts on rows (m1+k) and (m1+k+1) with coefficients c[k], s[k].
// For each column col in [n1, n2]:
//   new_row_i   = c[k] * old_row_i   - s[k] * old_row_{i+1}
//   new_row_{i+1}= s[k] * old_row_i   + c[k] * old_row_{i+1}
// If isForward is true, apply rotations k=0,1,... in order; otherwise apply k=m2-m1-1,...,0.
void applyLeftGivens(bool isForward,
                     int m1, int m2,
                     int n1, int n2,
                     const std::vector<double>& c,
                     const std::vector<double>& s,
                     std::vector<std::vector<double>>& A,
                     std::vector<double>& work) {
    if (m1 > m2 || n1 > n2) return;

    int numRot = m2 - m1;  // number of rotations
    if (numRot <= 0) return;

    int colCount = n2 - n1 + 1;

    // Ensure work has enough space; we will use indices [0..colCount-1]
    if ((int)work.size() < colCount) {
        work.resize(colCount);
    }

    auto processRotation = [&](int j, int k) {
        double ctemp = c[k];
        double stemp = s[k];
        if (ctemp == 1.0 && stemp == 0.0) return; // identity rotation

        if (n1 != n2) {
            // Copy old row j into work
            for (int col = n1; col <= n2; ++col) {
                int idx = col - n1;
                work[idx] = A[j][col];
            }
            // Update row j and row j+1
            for (int col = n1; col <= n2; ++col) {
                int idx = col - n1;
                double old_row_j = work[idx];
                double old_row_jp1 = A[j+1][col];
                A[j][col] = ctemp * old_row_j - stemp * old_row_jp1;
                A[j+1][col] = stemp * old_row_j + ctemp * old_row_jp1;
            }
        } else {
            // Single column case
            double old_row_j = A[j][n1];
            double old_row_jp1 = A[j+1][n1];
            A[j][n1] = ctemp * old_row_j - stemp * old_row_jp1;
            A[j+1][n1] = stemp * old_row_j + ctemp * old_row_jp1;
        }
    };

    if (isForward) {
        for (int j = m1; j < m2; ++j) {
            int k = j - m1;
            processRotation(j, k);
        }
    } else {
        for (int j = m2 - 1; j >= m1; --j) {
            int k = j - m1;
            processRotation(j, k);
        }
    }
}

#include <cassert>
#include <vector>
#include <cmath>

// Include the solution function here (or assume it's declared above)

int main() {
    // Test 1: Basic forward rotation on a 2x2 matrix, single rotation with c=0.6, s=0.8
    {
        std::vector<std::vector<double>> A = {{1.0, 2.0}, {3.0, 4.0}};
        std::vector<double> c = {0.6, 0.0}; // only first rotation used
        std::vector<double> s = {0.8, 0.0};
        std::vector<double> work;
        applyLeftGivens(true, 0, 1, 0, 1, c, s, A, work);
        // new row0 = 0.6*[1,2] - 0.8*[3,4] = [0.6-2.4, 1.2-3.2] = [-1.8, -2.0]
        // new row1 = 0.8*[1,2] + 0.6*[3,4] = [0.8+1.8, 1.6+2.4] = [2.6, 4.0]
        assert(std::fabs(A[0][0] - (-1.8)) < 1e-9);
        assert(std::fabs(A[0][1] - (-2.0)) < 1e-9);
        assert(std::fabs(A[1][0] - 2.6) < 1e-9);
        assert(std::fabs(A[1][1] - 4.0) < 1e-9);
    }

    // Test 2: Forward with identity rotation (c=1, s=0) should not change anything
    {
        std::vector<std::vector<double>> A = {{5.0, 6.0}, {7.0, 8.0}, {9.0, 10.0}};
        std::vector<double> c = {1.0, 1.0};
        std::vector<double> s = {0.0, 0.0};
        std::vector<double> work;
        applyLeftGivens(true, 0, 2, 0, 1, c, s, A, work);
        assert(A[0][0] == 5.0 && A[0][1] == 6.0);
        assert(A[1][0] == 7.0 && A[1][1] == 8.0);
        assert(A[2][0] == 9.0 && A[2][1] == 10.0);
    }

    // Test 3: Reverse order on 3 rows, with two rotations, check submatrix only (columns 1-2, rows 1-2)
    {
        std::vector<std::vector<double>> A = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0}
        };
        // Only rows 1 and 2 (m1=1, m2=2) are rotated, columns 1 to 2 (n1=1, n2=2)
        std::vector<double> c = {0.0, 1.0}; // index 0 is ignored because only one rotation for rows 1..2
        std::vector<double> s = {0.0, 0.0};
        std::vector<double> work;
        // force a rotation on rows 1,2 with c=0.8, s=0.6
        c[0] = 0.8; s[0] = 0.6;
        applyLeftGivens(false, 1, 2, 1, 2, c, s, A, work);
        // old row1 = [5,6], old row2 = [8,9]
        // new row1 = 0.8*[5,6] - 0.6*[8,9] = [4.0-4.8, 4.8-5.4] = [-0.8, -0.6]
        // new row2 = 0.6*[5,6] + 0.8*[8,9] = [3.0+6.4, 3.6+7.2] = [9.4, 10.8]
        assert(std::fabs(A[1][1] - (-0.8)) < 1e-9);
        assert(std::fabs(A[1][2] - (-0.6)) < 1e-9);
        assert(std::fabs(A[2][1] - 9.4) < 1e-9);
        assert(std::fabs(A[2][2] - 10.8) < 1e-9);
        // Other elements unchanged
        assert(A[0][0] == 1.0 && A[0][1] == 2.0 && A[0][2] == 3.0);
        assert(A[1][0] == 4.0 && A[2][0] == 7.0);
    }

    // Test 4: Empty submatrix (m1 > m2) should do nothing
    {
        std::vector<std::vector<double>> A = {{1.0}};
        std::vector<double> c = {1.0};
        std::vector<double> s = {0.0};
        std::vector<double> work;
        applyLeftGivens(true, 2, 1, 0, 0, c, s, A, work);
        assert(A[0][0] == 1.0);
    }

    // Test 5: Single column case with n1==n2
    {
        std::vector<std::vector<double>> A = {{1.0}, {2.0}};
        std::vector<double> c = {0.0}; // will be replaced
        std::vector<double> s = {0.0};
        c[0] = 0.8; s[0] = 0.6;
        std::vector<double> work;
        applyLeftGivens(true, 0, 1, 0, 0, c, s, A, work);
        // new row0 = 0.8*1 - 0.6*2 = 0.8-1.2 = -0.4
        // new row1 = 0.6*1 + 0.8*2 = 0.6+1.6 = 2.2
        assert(std::fabs(A[0][0] - (-0.4)) < 1e-9);
        assert(std::fabs(A[1][0] - 2.2) < 1e-9);
    }

    // Test 6: Forward with multiple rotations on a 4x4 matrix, verify a known result by manual computation
    {
        std::vector<std::vector<double>> A = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
        };
        std::vector<double> c = {0.6, 0.8, 1.0}; // first two rotations non-trivial
        std::vector<double> s = {0.8, -0.6, 0.0};
        std::vector<double> work;
        applyLeftGivens(true, 0, 3, 0, 3, c, s, A, work);
        // Compute expected by separately applying rotations:
        // Rotation 1 (rows 0,1): c=0.6, s=0.8
        // old row0=[1,2,3,4], old row1=[5,6,7,8]
        // new row0 = 0.6*row0 - 0.8*row1 = [0.6-4, 1.2-4.8, 1.8-5.6, 2.4-6.4] = [-3.4, -3.6, -3.8, -4.0]
        // new row1 = 0.8*row0 + 0.6*row1 = [0.8+3, 1.6+3.6, 2.4+4.2, 3.2+4.8] = [3.8, 5.2, 6.6, 8.0]
        // Then rotation 2 (rows 1,2): c=0.8, s=-0.6
        // current row1 = [3.8,5.2,6.6,8.0], row2 = [9,10,11,12]
        // new row1 = 0.8*row1 - (-0.6)*row2 = 0.8*row1 + 0.6*row2 = [3.04+5.4, 4.16+6, 5.28+6.6, 6.4+7.2] = [8.44, 10.16, 11.88, 13.6]
        // new row2 = (-0.6)*row1 + 0.8*row2 = -0.6*row1 + 0.8*row2 = [-2.28+7.2, -3.12+8, -3.96+8.8, -4.8+9.6] = [4.92, 4.88, 4.84, 4.8]
        // Rotation 3 is identity, no change.
        std::vector<std::vector<double>> expected = {
            {-3.4, -3.6, -3.8, -4.0},
            {8.44, 10.16, 11.88, 13.6},
            {4.92, 4.88, 4.84, 4.8},
            {13,14,15,16}
        };
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                assert(std::fabs(A[i][j] - expected[i][j]) < 1e-9);
            }
        }
    }

    return 0;
}

// The core algorithm is straightforward: iterate over the rows from `m1` to `m2-1` in either forward or reverse order depending on `isForward`. For each rotation index `k = j - m1` (where `j` is the current row), if the rotation is non-trivial (i.e., `c[k] != 1.0 || s[k] != 0.0`), we need to update rows `j` and `j+1` across the column range `[n1, n2]`. To avoid overwriting values before they are used, we must compute the new values using the original values. The key trick is to copy the values of row `j+1` into a temporary workspace vector (size sufficient to hold the column range, but we can use the provided `work` vector which is assumed to be at least `n2 - n1 + 1` in size), scaled by `c[k]`. Then subtract from it the values of row `j` scaled by `s[k]`. That gives the new values for row `j+1`? Wait, let's derive carefully. From the formula: `row_j_new = c * row_j_old - s * row_{j+1}_old` and `row_{j+1}_new = s * row_j_old + c * row_{j+1}_old`. So we need both old rows. If we copy `row_{j+1}_old` into `work` first, then we can compute `row_j_new` directly and store it in `row_j` (overwriting old `row_j`), but to compute `row_{j+1}_new` we need old `row_j`. So we can save old `row_j` into `work` first instead, or compute both with careful ordering: First, copy `row_j` into `work` (or `work` hold old row_j). Then compute `row_j_new = c * old_row_j - s * row_{j+1}` and store in `row_j`. Then compute `row_{j+1}_new = s * old_row_j + c * row_{j+1}` and store in `row_{j+1}`. Since `row_{j+1}` hasn't been modified yet, we can use it. Alternatively, we can copy `row_{j+1}` into `work`, then compute and store `row_j_new = c * row_j - s * work`, then compute `row_{j+1}_new = s * row_j_old + c * work` but `row_j` has been overwritten, so we need old `row_j` too. Thus the safer approach is to copy row `j` into `work` first (since we will overwrite it), then compute both new rows using `row_j_old` from `work` and `row_{j+1}` from the matrix (unchanged). In the given snippet, they copy `a(jp1, n1)` into `work` scaled by `ctemp`, then subtract `a(j, n1)` scaled by `stemp`, giving `work = ctemp * a(j+1) - stemp * a(j)`. That is actually `row_{j+1}_new`? Let's check: `row_{j+1}_new = s * a(j) + c * a(j+1)`. They have `work = c * a(j+1) - s * a(j)`. That's not the same. Actually they then do: `a(j) = c * a(j) + s * a(j+1)` (they do `vmul(a(j), c); vadd(a(j), a(j+1), s)`), so `a(j)_new = c*a(j)_old + s*a(j+1)_old`? Wait, the original comment says "Form P*A" and the formula is likely `[ c  s; -s  c ]` times `[a(j); a(j+1)]`. That yields `a(j)_new = c*a(j) + s*a(j+1)`, `a(j+1)_new = -s*a(j) + c*a(j+1)`. But the problem description says "row_i' = c_k * row_i - s_k * row_{i+1}" and "row_{i+1}' = s_k * row_i + c_k * row_{i+1}". That is a different sign convention. The snippet uses `[c s; -s c]` (since it does `vsub(work, a(j), stemp)` giving `c*a(j+1)-s*a(j)`, then `a(j) = c*a(j)+s*a(j+1)`, and `a(j+1) = work = c*a(j+1)-s*a(j)`). So the snippet's convention is `a(j)_new = c*a(j)+s*a(j+1)`, `a(j+1)_new = -s*a(j)+c*a(j+1)`. However, the task specification explicitly defines the transformation as `row_i' = c_k * row_i - s_k * row_{i+1}` and `row_{i+1}' = s_k * row_i + c_k * row_{i+1}`. I will implement according to the task spec, not the snippet's exact sign, but the algorithm is analogous. For correctness, I'll follow the spec: compute `temp = row_j` (old), `row_j_new = c * temp - s * row_{j+1}`, `row_{j+1}_new = s * temp + c * row_{j+1}`. Use `work` vector to hold the old row_j values for the common case (n1 != n2). For the single-column case, just use a scalar `double temp`. The order of iteration matters: forward goes from `j=m1` to `m2-1`; reverse from `j=m2-1` down to `m1`. The time complexity is O((m2-m1) * (n2-n1+1)) because each rotation touches each element in the two rows and the column range. Space complexity is O(n2-n1+1) for the workspace (or O(1) if we ignore it). Edge cases: empty submatrix, identity rotations (skip), single column, single row (no rotations), and ensure that the `work` vector is large enough (we can assume it is passed with appropriate size, but we should still only use indices from n1 to n2 relative to the start of work? Since the spec says "work" is a vector of size at least n2-n1+1, we can treat `work` as a temporary array indexed from 0 to n2-n1, but we can also just use a local `std::vector<double>` inside the function for clarity, but the signature requires a `work` parameter, so we should use it to avoid extra allocation. We'll copy old row_j values into `work[0..(n2-n1)]` by iterating columns. The matrix is `std::vector<std::vector<double>>` with row-major indexing; we must ensure the matrix has at least `m2+1` rows and `n2+1` columns (assumed valid).

Write a standalone C++ function `keepRowsByLabelMembership` that takes a dense matrix `F` (with fixed or dynamic columns), a dense vector `L` containing a list of labels/indices, and a boolean `exclusive`. The function must return a new matrix `LF` containing only those rows of `F` where, if `exclusive` is `true`, every entry in the row appears somewhere in `L`, or if `exclusive` is `false`, at least one entry in the row appears in `L`. The input matrix `F` and vector `L` can contain `int` values (or any type supporting `==`). The output `LF` must preserve the original row order and have the same number of columns as `F`. The function must not modify its inputs, and must handle empty `F`, empty `L` (where `exclusive=true` yields no rows, `exclusive=false` yields no rows because no entry can match), and rows with repeated values. Provide the implementation as a free function template that works with any Eigen-like dense matrix, but for this task you may assume `Eigen::MatrixXi` and `Eigen::VectorXi` for simplicity (or provide a templated version). The function should resize `LF` appropriately.

// The solution iterates over each row of `F` and for each row, checks every entry to determine whether it exists in `L`. A nested loop over `L` performs the membership test; we can use `std::find` for clarity but a manual loop is fine. For each row, maintain two booleans: `all` (true if every entry found) and `any` (false initially; true if at least one entry found). After checking all entries, the row is selected if `(exclusive ? all : any)`. Count the selected rows to preallocate `LF` with the correct size, then copy the selected rows in order. Important edge cases: empty `L` – then no entry can ever be found, so `any` stays false and `all` starts true but becomes false after the first missing entry; selecting `exclusive=false` yields zero rows; `exclusive=true` yields zero rows as well. Empty `F` – the function should handle `rows()==0` gracefully by setting `LF` to a 0×cols matrix. If `F` has zero columns (unlikely), every row vacuously satisfies `exclusive=true` because `all` remains true, but `exclusive=false` would yield false since `any` is false; this is consistent with the definition. Time complexity is O(R * C * |L|), where R = F.rows(), C = F.cols(), and |L| = L.size(). Space complexity is O(R) for the boolean vector plus O(R*C) for the output matrix (which is the required output size). The implementation uses `const` references for inputs, and returns the result by value for clarity and safety.

#include <Eigen/Dense>
#include <vector>

// Keep rows of F where every entry (exclusive=true) or at least one entry (exclusive=false)
// appears in L. Returns a new matrix with only the selected rows.
template <typename MatF, typename VecL>
MatF keepRowsByLabelMembership(const MatF& F, const VecL& L, bool exclusive) {
    const int rows = F.rows();
    const int cols = F.cols();

    std::vector<bool> keep(rows, false);
    int num_keep = 0;

    // First pass: determine which rows to keep.
    for (int i = 0; i < rows; ++i) {
        bool all = true;
        bool any = false;
        for (int j = 0; j < cols; ++j) {
            bool found = false;
            const auto& value = F(i, j);
            for (int l = 0; l < L.size(); ++l) {
                if (value == L(l)) {
                    found = true;
                    break;
                }
            }
            any = any || found;
            all = all && found;
        }
        keep[i] = exclusive ? all : any;
        if (keep[i]) ++num_keep;
    }

    // Second pass: copy the selected rows.
    MatF LF(num_keep, cols);
    int write_idx = 0;
    for (int i = 0; i < rows; ++i) {
        if (keep[i]) {
            LF.row(write_idx) = F.row(i);
            ++write_idx;
        }
    }
    return LF;
}

#include <cassert>
#include <Eigen/Dense>

int main() {
    Eigen::MatrixXi F(5, 3);
    F << 1, 2, 3,
         4, 5, 6,
         1, 1, 2,
         7, 8, 9,
         2, 3, 4;
    Eigen::VectorXi L(3);
    L << 1, 2, 3;

    // Exclusive: only rows where every entry is in {1,2,3}
    Eigen::MatrixXi LF_excl = keepRowsByLabelMembership(F, L, true);
    assert(LF_excl.rows() == 1);
    assert(LF_excl.cols() == 3);
    assert(LF_excl(0, 0) == 1 && LF_excl(0, 1) == 1 && LF_excl(0, 2) == 2);

    // Non-exclusive: rows with at least one entry in {1,2,3}
    Eigen::MatrixXi LF_any = keepRowsByLabelMembership(F, L, false);
    assert(LF_any.rows() == 3);
    assert(LF_any.row(0).isApprox(Eigen::RowVector3i(1, 2, 3)));
    assert(LF_any.row(1).isApprox(Eigen::RowVector3i(1, 1, 2)));
    assert(LF_any.row(2).isApprox(Eigen::RowVector3i(2, 3, 4)));

    // Empty L: no rows kept in both modes
    Eigen::VectorXi L_empty(0);
    assert(keepRowsByLabelMembership(F, L_empty, true).rows() == 0);
    assert(keepRowsByLabelMembership(F, L_empty, false).rows() == 0);

    // Empty F: result has 0 rows but same columns
    Eigen::MatrixXi F_empty(0, 3);
    Eigen::MatrixXi LF_e = keepRowsByLabelMembership(F_empty, L, true);
    assert(LF_e.rows() == 0 && LF_e.cols() == 3);

    // All entries match when exclusive
    Eigen::MatrixXi F_all(2, 2);
    F_all << 1, 2, 3, 1;
    Eigen::VectorXi L_all(3);
    L_all << 1, 2, 3;
    assert(keepRowsByLabelMembership(F_all, L_all, true).rows() == 2);
    assert(keepRowsByLabelMembership(F_all, L_all, false).rows() == 2);
}

// Write a C++ function `adjlst subtract_identity(const adjlsts& matrix, size_t row_id)` that takes a weighted adjacency list representation of a square matrix (where `adjlsts` is a vector of `adjlst`, and `adjlst` is a vector of `pair<size_t, double>` representing (column, weight) entries) and returns a new adjacency list for the row `row_id` of the matrix after subtracting the identity matrix from it. Specifically, for each non-zero entry `(col, val)` in row `row_id` of the original matrix, the result should contain the entry `(col, val)` if `row_id != col`, and the entry `(col, val - 1)` if `row_id == col`, omitting any entries that become zero. If the diagonal entry was originally absent (i.e., had implied value 0), then the result must contain the entry `(row_id, -1)` to represent the `-1` introduced by subtracting the identity. The function must handle empty rows, rows with only diagonal entries that cancel to zero, and rows where the diagonal becomes zero after subtraction (in which case that entry must be omitted). The complexity should be linear in the number of entries in the input row.

The task is a direct adaptation of the `subtract_id` helper function from the snippet, but simplified to always subtract the identity (no negation parameter and no column gap offset). The algorithm iterates over all entries in the given row of the adjacency list. For each entry, check if the column index equals the row index (diagonal). If not diagonal, copy the entry with unchanged weight. If diagonal, compute `val - 1` and emit it only if non-zero. Additionally, track whether a diagonal entry was seen. If no diagonal entry was seen at the end, this means the original matrix had a `0` on the diagonal, so after identity subtraction the result must include `(row_id, -1)`. Edge cases: an empty input row leads to just `(row_id, -1)`; a diagonal entry with value `1` yields zero and is omitted; if a diagonal entry exists with value different than 1, it is emitted with `val - 1`. The time complexity is O(degree of row) and space complexity O(degree of row) for the output, not counting the input storage.

#include <vector>
#include <utility>

using adjlst = std::vector<std::pair<size_t, double>>;
using adjlsts = std::vector<adjlst>;

// Subtract the identity matrix from row row_id of a square matrix
// given in sparse adjacency list format. Returns the resulting row.
adjlst subtract_identity(const adjlsts& A, size_t row_id) {
    adjlst result;
    bool diag_found = false;

    for (const auto& [col, val] : A.at(row_id)) {
        if (row_id != col) {
            // Off-diagonal entries unchanged (val is non-zero by construction)
            result.emplace_back(col, val);
        } else {
            // Diagonal: val becomes val - 1; emit only if non-zero
            double new_val = val - 1.0;
            if (new_val != 0.0) {
                result.emplace_back(col, new_val);
            }
            diag_found = true;
        }
    }

    if (!diag_found) {
        // Original diagonal was 0 => after subtracting identity it becomes -1
        result.emplace_back(row_id, -1.0);
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

using adjlst = std::vector<std::pair<size_t, double>>;
using adjlsts = std::vector<adjlst>;

// declare the function under test (already provided in solution)
adjlst subtract_identity(const adjlsts& A, size_t row_id);

int main() {
    // Case 1: Row with both diagonal and off-diagonal entries
    adjlsts A1 = {{{0, 3.0}, {1, 2.0}}, {{0, -1.0}, {1, 1.0}}};
    adjlst r1 = subtract_identity(A1, 0);
    assert(r1.size() == 2);
    assert(r1[0] == std::make_pair(1, 2.0));
    assert(r1[1] == std::make_pair(0, 2.0)); // 3 - 1 = 2

    // Case 2: Diagonal exactly 1 cancels to zero, remains with off-diagonal
    adjlsts A2 = {{{0, 1.0}, {1, 5.0}}, {}};
    adjlst r2 = subtract_identity(A2, 0);
    assert(r2.size() == 1);
    assert(r2[0] == std::make_pair(1, 5.0));

    // Case 3: Empty row -> only -1 diagonal entry
    adjlsts A3 = {{}, {}};
    adjlst r3 = subtract_identity(A3, 0);
    assert(r3.size() == 1);
    assert(r3[0] == std::make_pair(0, -1.0));

    // Case 4: Row with only diagonal value 0 (explicitly stored as 0? assume stored)
    adjlsts A4 = {{{0, 0.0}}, {}};  // Note: 0 stored explicitly but should be treated as present
    adjlst r4 = subtract_identity(A4, 1);
    // For row 1, no entries: diagonal missing => -1
    assert(r4.size() == 1);
    assert(r4[0] == std::make_pair(1, -1.0));

    // Case 5: Diagonal negative value, off-diagonal negative
    adjlsts A5 = {{{0, -2.0}, {1, -3.0}}, {}};
    adjlst r5 = subtract_identity(A5, 0);
    assert(r5.size() == 2);
    assert(r5[0] == std::make_pair(1, -3.0));
    assert(r5[1] == std::make_pair(0, -3.0)); // -2 -1 = -3

    // Case 6: All entries zero after subtraction except off-diagonal
    adjlsts A6 = {{{0, 1.0}, {1, 0.0}}, {}};
    adjlst r6 = subtract_identity(A6, 0);
    // diagonal 1->0 removed, off-diagonal 0 should be omitted (but given as 0? per spec we keep non-zero only)
    // Actually input may have 0 entries? Assume they are absent. But just test with present zero:
    // Here col1 has 0, which is not non-zero, but we should keep it? The spec says "non-zero entries" in input.
    // To be safe, we assume input only contains non-zero entries. So test:
    adjlsts A6b = {{{0, 1.0}, {1, 4.0}}, {}};
    adjlst r6b = subtract_identity(A6b, 0);
    assert(r6b.size() == 1);
    assert(r6b[0] == std::make_pair(1, 4.0));

    // Case 7: Large row, ensure no crash
    adjlsts A7 = {{{0, 2.0}, {1, 3.0}, {2, 4.0}, {3, 5.0}}, {}, {}, {}};
    adjlst r7 = subtract_identity(A7, 0);
    assert(r7.size() == 4);
    assert(r7[0] == std::make_pair(1, 3.0));
    assert(r7[1] == std::make_pair(2, 4.0));
    assert(r7[2] == std::make_pair(3, 5.0));
    assert(r7[3] == std::make_pair(0, 1.0)); // 2 - 1 = 1

    return 0;
}

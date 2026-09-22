/*
Write a C++ function `stableSortByColumns` that takes a vector of vectors of strings (representing table rows, where each row has the same number of columns), a vector of column indices (0-based) specifying the sort key order (the first index is the primary key, the second is the secondary key, etc.), and a vector of booleans indicating for each corresponding column whether sorting should be ascending (`true`) or descending (`false`). The function should return a new vector of vectors that is a stable sort of the input rows according to the given column priorities and directions. If the sort key columns are empty, return the input unchanged. If any column index is out of range for any row, throw an `std::out_of_range` exception. The comparison must be lexicographic (string comparison) on the key tuple formed by the specified columns, respecting the direction for each column. The sort must be stable—rows with equal keys (considering all specified columns and directions) must retain their original relative order. You may use `std::stable_sort` with a custom comparator. The function should be `const`-correct and handle empty input gracefully.
*/

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

// Stable sorts rows by a sequence of column indices and directions.
// Returns a new vector of rows; does not modify input.
// Throws std::out_of_range if any column index is invalid for any row.
std::vector<std::vector<std::string>> stableSortByColumns(
    const std::vector<std::vector<std::string>>& rows,
    const std::vector<size_t>& columnIndices,
    const std::vector<bool>& ascendingFlags) {

    // Validate input consistency
    if (columnIndices.size() != ascendingFlags.size()) {
        throw std::invalid_argument("columnIndices and ascendingFlags sizes must match");
    }
    if (columnIndices.empty() || rows.empty()) {
        return rows;
    }

    // Validate column indices for all rows
    for (const auto& row : rows) {
        for (size_t col : columnIndices) {
            if (col >= row.size()) {
                throw std::out_of_range("Column index out of range");
            }
        }
    }

    // Make a copy of rows to sort
    std::vector<std::vector<std::string>> sortedRows = rows;

    // Comparator for stable sort
    auto comparator = [&](const std::vector<std::string>& a,
                          const std::vector<std::string>& b) -> bool {
        for (size_t idx = 0; idx < columnIndices.size(); ++idx) {
            size_t col = columnIndices[idx];
            const std::string& aVal = a[col];
            const std::string& bVal = b[col];
            if (aVal != bVal) {
                if (ascendingFlags[idx]) {
                    return aVal < bVal;
                } else {
                    return aVal > bVal;
                }
            }
        }
        return false; // equal on all keys: stable sort maintains original order
    };

    std::stable_sort(sortedRows.begin(), sortedRows.end(), comparator);
    return sortedRows;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is above; test it here.
// (In a real compile, the solution code must be included or placed before this main.)
int main() {
    // Basic ascending sort by column 0
    std::vector<std::vector<std::string>> rows = {
        {"b", "2"}, {"a", "1"}, {"c", "3"}, {"a", "0"}
    };
    auto result = stableSortByColumns(rows, {0}, {true});
    assert((result == std::vector<std::vector<std::string>>{
        {"a", "1"}, {"a", "0"}, {"b", "2"}, {"c", "3"}
    }));

    // Stable sort: equal primary key retains original order
    rows = {{"x", "1"}, {"x", "0"}, {"x", "2"}};
    result = stableSortByColumns(rows, {0}, {true});
    assert(result == rows); // all equal, unchanged, stable

    // Multi-column sort: primary col 0 ascending, secondary col 1 descending
    rows = {
        {"a", "1"}, {"b", "2"}, {"a", "3"}, {"b", "1"}, {"a", "2"}
    };
    result = stableSortByColumns(rows, {0, 1}, {true, false});
    assert((result == std::vector<std::vector<std::string>>{
        {"a", "3"}, {"a", "2"}, {"a", "1"},
        {"b", "2"}, {"b", "1"}
    }));

    // Empty keys returns input unchanged
    rows = {{"z"}, {"y"}};
    result = stableSortByColumns(rows, {}, {});
    assert(result == rows);

    // Empty rows returns empty
    assert(stableSortByColumns({}, {0}, {true}).empty());

    // Validate exception on out-of-range column
    bool threw = false;
    try {
        rows = {{"a", "b"}};
        auto r = stableSortByColumns(rows, {2}, {true});
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // Single row
    rows = {{"only"}};
    result = stableSortByColumns(rows, {0}, {false});
    assert(result == rows);
}

// The solution uses the standard library's `std::stable_sort` to guarantee stability. The comparator captures the column indices and direction flags by reference and compares two rows element-wise over the key columns. For each column, it compares the corresponding strings using `<` (for ascending) or `>` (for descending); if the two values are equal, it moves to the next column. If all specified columns are equal, the comparator returns `false` for both directions, which is correct for stability because `std::stable_sort` preserves the original order of equivalent elements. Before sorting, we validate all column indices are within bounds for every row; if not, throw `std::out_of_range`. If the key columns are empty or the rows vector is empty, we return a copy of the input unchanged. Time complexity is `O(m * n log n)` where `n` is the number of rows and `m` is the number of key columns (each comparison can take up to `m` string comparisons, each string comparison potentially `O(k)` for string length `k`). Space complexity is `O(1)` auxiliary beyond the copy, though `std::stable_sort` may allocate temporary storage in practice.

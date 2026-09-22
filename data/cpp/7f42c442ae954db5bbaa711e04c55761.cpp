Write a C++ function named `countRowSums` that takes a constant reference to a 2D integer vector (where all rows have the same length) and returns a `std::vector<int>` containing the sum of each row, in the same order as the rows. The input is guaranteed to be non-empty (at least one row and one column per row). The function must handle negative values and large sums (use `long long` internally to avoid overflow, but return `std::vector<long long>` to be safe). You do not need to validate the rectangular shape; assume all rows are equal length. This task is inspired by iterating over a fixed-size 2D array as shown in the snippet, but generalized to dynamic dimensions.

The solution iterates over each row (outer loop) and for each row, iterates over each element (inner loop), accumulating the sum into a `long long` accumulator initialized to zero for that row. After the inner loop finishes, push the accumulator into the result vector. Edge cases: a single row or single column works fine because loops handle them naturally; negative values are handled because addition works correctly with signed integers; empty input is not allowed per task, but if it were, we could return an empty vector. Time complexity is O(rows × columns) where rows = number of rows and columns = number of columns per row, because each element is visited exactly once. Space complexity is O(rows) for the result vector, not counting input storage.

#include <vector>

// Compute the sum of each row of a rectangular 2D vector.
// Input: rows of equal length, non-empty.
// Returns: vector of 'long long' sums, one per row in order.
std::vector<long long> countRowSums(const std::vector<std::vector<int>>& grid) {
    std::vector<long long> result;
    result.reserve(grid.size());

    for (const auto& row : grid) {
        long long sum = 0;
        for (int value : row) {
            sum += value;
        }
        result.push_back(sum);
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above (included here implicitly).
// Tests for countRowSums.
int main() {
    // Basic rectangular case from the snippet style.
    std::vector<std::vector<int>> grid1 = {{10, 20}, {30, 40}, {50, 60}};
    assert(countRowSums(grid1) == std::vector<long long>({30, 70, 110}));

    // Single row, single column.
    std::vector<std::vector<int>> grid2 = {{-5}};
    assert(countRowSums(grid2) == std::vector<long long>({-5}));

    // Single row, multiple columns.
    std::vector<std::vector<int>> grid3 = {{1, 2, 3, 4}};
    assert(countRowSums(grid3) == std::vector<long long>({10}));

    // Multiple rows, single column.
    std::vector<std::vector<int>> grid4 = {{7}, {-2}, {0}};
    assert(countRowSums(grid4) == std::vector<long long>({7, -2, 0}));

    // Negative and zero values mixed.
    std::vector<std::vector<int>> grid5 = {{-1, -2}, {3, -4}, {0, 0}};
    assert(countRowSums(grid5) == std::vector<long long>({-3, -1, 0}));

    // Large numbers to check long long.
    std::vector<std::vector<int>> grid6 = {{1000000000, 1000000000}, {2000000000, 2000000000}};
    assert(countRowSums(grid6) == std::vector<long long>({2000000000LL, 4000000000LL}));

    return 0;
}

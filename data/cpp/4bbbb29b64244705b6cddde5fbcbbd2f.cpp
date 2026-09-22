Write a C++ function `extractRange` that takes three parameters: a positive integer `n` (the size of an imaginary square matrix indexed from 1 to n both rows and columns), and two zero-based indices `left` and `right` into that matrix when read in row-major order (i.e., element at position `(row, col)` has index `(row-1)*n + (col-1)`). The function must return a `std::vector<int>` containing the values `max(row, col)` for every element whose linear index is in the inclusive range `[left, right]` (i.e., starting at `left` and ending at `right`). The input guarantees `left <= right` and that `right` is a valid index (0 ≤ left ≤ right < n*n). Your function must not simulate the full matrix (which would be too large for large `n`); instead, it must compute the values directly from the indices. For example, with `n=3`, the row-major order values are [1,2,3,2,2,3,3,3,3]; calling `extractRange(3, 3, 7)` should return [2,3,3,3,3] because the linear indices 3..7 correspond to (2,1),(2,2),(2,3),(3,1),(3,2).
The key observation is that for a position `(row, col)` (1-based), the value is `max(row, col)`. The linear index `idx` maps to `row = idx / n + 1` and `col = idx % n + 1` (using integer division and modulo). To avoid iterating over all elements from `left` to `right` if that range is huge (though in practice we must iterate over the output size, which is `right - left + 1`), we can start from the `left` index and advance row by row, but simpler is to iterate from `left` to `right` directly because the output length is the number of elements requested, which is unavoidable. However, note the original snippet uses recursion and appends `max(y,x)` each step; for large ranges this could cause stack overflow. A better iterative solution is to compute each element directly. The main algorithm: compute the starting row and column from `left`, then for each index `i` from 0 to `right - left`, update the current row and column, push `max(row,col)`, and move to the next column, wrapping to next row when column exceeds `n`. Edge cases: when the range covers multiple rows, ensure column resets to 1 after each row; when `n=1` only one element exists; and if `left` and `right` are large, avoid recursion. Time complexity is O(k) where k = right-left+1 (the output size), and space complexity is O(k) for the output vector, plus O(1) extra.
#include <vector>
#include <algorithm>

// Returns the values max(row, col) for linear indices [left, right] in row-major order of an n x n matrix.
std::vector<int> extractRange(int n, long long left, long long right) {
    std::vector<int> result;
    result.reserve(static_cast<size_t>(right - left + 1));

    // Compute starting row and column (1-based).
    int row = static_cast<int>(left / n + 1);
    int col = static_cast<int>(left % n + 1);

    long long count = right - left + 1;
    for (long long i = 0; i < count; ++i) {
        result.push_back(std::max(row, col));
        // Advance to next position.
        if (col < n) {
            ++col;
        } else {
            ++row;
            col = 1;
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Assume extractRange is available from the solution.

int main() {
    // Example from the prompt.
    assert(extractRange(3, 3LL, 7LL) == std::vector<int>({2,3,3,3,3}));

    // Full matrix for n=1.
    assert(extractRange(1, 0LL, 0LL) == std::vector<int>({1}));

    // First three elements of n=2 matrix: values [1,2,2].
    assert(extractRange(2, 0LL, 2LL) == std::vector<int>({1,2,2}));

    // All elements of n=2: [1,2,2,2].
    assert(extractRange(2, 0LL, 3LL) == std::vector<int>({1,2,2,2}));

    // Range starting in the middle of a row.
    // n=4: row-major values: [1,2,3,4,2,2,3,4,3,3,3,4,4,4,4,4].
    // Indices 5..10 -> (2,2),(2,3),(2,4),(3,1),(3,2),(3,3) => [2,3,4,3,3,3].
    assert(extractRange(4, 5LL, 10LL) == std::vector<int>({2,3,4,3,3,3}));

    // Single element at the end of a large matrix.
    // n=1000, last index = 999999 -> row=1000, col=1000 => value=1000.
    assert(extractRange(1000, 999999LL, 999999LL) == std::vector<int>({1000}));

    // Large range that spans multiple rows, check first few and last few.
    std::vector<int> full = extractRange(5, 0LL, 24LL);
    // Expected full matrix for n=5: rows:
    // Row1: 1,2,3,4,5
    // Row2: 2,2,3,4,5
    // Row3: 3,3,3,4,5
    // Row4: 4,4,4,4,5
    // Row5: 5,5,5,5,5
    std::vector<int> expected = {1,2,3,4,5,2,2,3,4,5,3,3,3,4,5,4,4,4,4,5,5,5,5,5,5};
    assert(full == expected);

    // Range that starts at a row boundary.
    // n=3, index 3 is start of row 2 (since index = (row-1)*n + col-1, index 3 -> row=2,col=1).
    // So indices 3..5 give values [2,3,3].
    assert(extractRange(3, 3LL, 5LL) == std::vector<int>({2,3,3}));
}

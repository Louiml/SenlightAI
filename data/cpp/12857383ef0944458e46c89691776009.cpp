// Write a C++ function `bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target)` that determines whether an integer `target` exists in a strictly sorted matrix. The matrix is sorted such that each row is sorted in ascending order from left to right, and the first element of each row is greater than the last element of the previous row. This means that if you flatten the matrix, you get a fully sorted sequence. The function should return `true` if the target is found, and `false` otherwise. The matrix is guaranteed to be non-empty (at least one row and one column). Your solution must not use the standard library’s binary search on a flattened view; instead, implement a two‑step binary search: first, locate the possible row using the row boundaries (first and last elements), then search within that row. Handle edge cases such as a `1×1` matrix, a target smaller than the smallest element, and a target larger than the largest element.
// The solution works in two phases. First, binary search over the rows to find the single row that could contain the target: for each mid‑row, compare the target with the row’s first element (`matrix[midRow][0]`) and last element (`matrix[midRow][colN-1]`). If the target lies within that inclusive range, proceed to the second phase; if the target is greater than the last element of the mid‑row, move the search to rows below (`startRow = midRow + 1`); if the target is less than the first element, move to rows above (`endRow = midRow - 1`). Once a candidate row is identified, perform a standard binary search on that row. The algorithm’s time complexity is \(O(\log m + \log n)\) because each binary search halves its search space; equivalently, \(O(\log (m \cdot n))\). Space complexity is \(O(1)\) since only a few integer indices are used. Key edge cases: (1) The matrix has a single row: the row‑finding loop will immediately find that row. (2) The target is outside the overall range: the row‑finding loop will terminate with `startRow > endRow` and return `false`. (3) Duplicate values do not appear (strictly sorted), but the binary search handles equality correctly. (4) The row search uses a standard binary search that avoids integer overflow by computing `mid = start + (end - start) / 2`.
#include <vector>

// Searches for a target value in a strictly sorted matrix where each row is
// sorted and the first element of each row is greater than the last of the previous.
bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
    int rowCount = matrix.size();
    int colCount = matrix[0].size();

    // Step 1: Binary search over rows to find the candidate row.
    int startRow = 0;
    int endRow = rowCount - 1;
    int candidateRow = -1;

    while (startRow <= endRow) {
        int midRow = startRow + (endRow - startRow) / 2;
        if (matrix[midRow][0] <= target && target <= matrix[midRow][colCount - 1]) {
            candidateRow = midRow;
            break;
        } else if (matrix[midRow][colCount - 1] < target) {
            // Target is larger than the last element of this row -> search rows below.
            startRow = midRow + 1;
        } else {
            // Target is smaller than the first element of this row -> search rows above.
            endRow = midRow - 1;
        }
    }

    if (candidateRow == -1) {
        return false;
    }

    // Step 2: Binary search within the candidate row.
    int left = 0;
    int right = colCount - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (matrix[candidateRow][mid] == target) {
            return true;
        } else if (matrix[candidateRow][mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return false;
}
#include <cassert>
#include <vector>

// The solution function is declared above (or included via header).
// Place the solution code here or link it.

int main() {
    // Basic 2x2 matrix
    std::vector<std::vector<int>> m1 = {{1, 3}, {5, 7}};
    assert(searchMatrix(m1, 3) == true);
    assert(searchMatrix(m1, 5) == true);
    assert(searchMatrix(m1, 4) == false);

    // 3x3 matrix
    std::vector<std::vector<int>> m2 = {{1, 4, 7}, {10, 12, 15}, {20, 22, 28}};
    assert(searchMatrix(m2, 1) == true);
    assert(searchMatrix(m2, 28) == true);
    assert(searchMatrix(m2, 12) == true);
    assert(searchMatrix(m2, 0) == false);
    assert(searchMatrix(m2, 30) == false);
    assert(searchMatrix(m2, 11) == false);

    // 1x1 matrix
    std::vector<std::vector<int>> m3 = {{5}};
    assert(searchMatrix(m3, 5) == true);
    assert(searchMatrix(m3, 4) == false);
    assert(searchMatrix(m3, 6) == false);

    // Single row matrix
    std::vector<std::vector<int>> m4 = {{-2, -1, 0, 2}};
    assert(searchMatrix(m4, -2) == true);
    assert(searchMatrix(m4, 2) == true);
    assert(searchMatrix(m4, 0) == true);
    assert(searchMatrix(m4, 1) == false);

    // Single column matrix
    std::vector<std::vector<int>> m5 = {{-5}, {0}, {7}};
    assert(searchMatrix(m5, -5) == true);
    assert(searchMatrix(m5, 7) == true);
    assert(searchMatrix(m5, 3) == false);

    // Negative numbers and boundary values
    std::vector<std::vector<int>> m6 = {{-10, -5}, {-3, -1}, {2, 4}};
    assert(searchMatrix(m6, -10) == true);
    assert(searchMatrix(m6, -1) == true);
    assert(searchMatrix(m6, 2) == true);
    assert(searchMatrix(m6, -4) == false);

    return 0;
}

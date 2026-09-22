// Write a C++ function `searchMatrix` that takes a 2D integer vector `matrix` (where each row is sorted in non-decreasing order, but rows are not necessarily globally sorted) and an integer `target`. The function should return `true` if `target` exists anywhere in the matrix, and `false` otherwise. You must solve the problem by using binary search on each row individually. The matrix may be empty (no rows), may contain rows of different lengths, and may contain duplicate values. Handle edge cases gracefully, such as an empty matrix or empty rows.
// The straightforward approach is to iterate over every row of the matrix, and for each row, use `std::binary_search` (or a manual binary search) to check if the target is present. Since each row is sorted independently, binary search on each row is correct and efficient. Important edge cases include: the matrix itself being empty (return `false`), a row being empty (binary search on an empty range returns `false`), and the target being smaller than all elements or larger than all elements in a row (binary search handles that normally). The time complexity is `O(R * log C)` in the worst case, where `R` is the number of rows and `C` is the average or maximum row length, because we perform binary search on each row. The space complexity is `O(1)` auxiliary, not counting the matrix storage.
#include <vector>
#include <algorithm>

// Return true if target exists in any row of the matrix; each row is sorted.
bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
    for (const auto& row : matrix) {
        if (std::binary_search(row.begin(), row.end(), target)) {
            return true;
        }
    }
    return false;
}
#include <cassert>
#include <vector>

// The solution function declaration is assumed to be available from the included header.
bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target);

int main() {
    // Standard case: target present in a row.
    std::vector<std::vector<int>> m1 = {{1, 3, 5}, {7, 9}, {10, 12, 14}};
    assert(searchMatrix(m1, 9) == true);
    assert(searchMatrix(m1, 4) == false);

    // Empty matrix.
    std::vector<std::vector<int>> m2;
    assert(searchMatrix(m2, 0) == false);

    // Matrix with empty rows.
    std::vector<std::vector<int>> m3 = {{}, {2, 4}, {}};
    assert(searchMatrix(m3, 2) == true);
    assert(searchMatrix(m3, 5) == false);

    // Duplicate values across and within rows.
    std::vector<std::vector<int>> m4 = {{5, 5, 5}, {5, 6}};
    assert(searchMatrix(m4, 5) == true);
    assert(searchMatrix(m4, 6) == true);

    // Single row, target at beginning and end.
    std::vector<std::vector<int>> m5 = {{-3, -1, 0, 2}};
    assert(searchMatrix(m5, -3) == true);
    assert(searchMatrix(m5, 2) == true);
    assert(searchMatrix(m5, 1) == false);

    // Negative numbers only, target not present.
    std::vector<std::vector<int>> m6 = {{-8, -4}, {-2, -1}};
    assert(searchMatrix(m6, -5) == false);
    assert(searchMatrix(m6, -4) == true);

    return 0;
}

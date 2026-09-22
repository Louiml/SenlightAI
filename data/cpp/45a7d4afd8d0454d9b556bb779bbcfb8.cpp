// Write a C++ function that determines whether a given integer `x` exists within a 2D matrix where each row is individually sorted in non-decreasing order. The matrix is represented as a `std::vector<std::vector<int>>` and may contain duplicate values. The function must return `true` if `x` is present in any row, and `false` otherwise. Assume the matrix has at least one row, but individual rows may be empty. The function signature should be `bool searchRowMatrix(const std::vector<std::vector<int>>& mat, int x)`, and it must work efficiently by exploiting the fact that each row is sorted.
#include <cassert>
#include <vector>

// The solution function (provided above) is assumed to be included.
// For completeness, declare it here or place the full implementation.
bool searchRowMatrix(const std::vector<std::vector<int>>& mat, int x);

int main() {
    // Test 1: Basic presence in a row.
    std::vector<std::vector<int>> mat1 = {{1, 3, 5}, {2, 4, 6}};
    assert(searchRowMatrix(mat1, 4) == true);

    // Test 2: Absence in all rows.
    assert(searchRowMatrix(mat1, 7) == false);

    // Test 3: Target equal to the smallest element in a row.
    assert(searchRowMatrix(mat1, 1) == true);

    // Test 4: Target equal to the largest element in a row.
    assert(searchRowMatrix(mat1, 6) == true);

    // Test 5: Duplicate values across rows.
    std::vector<std::vector<int>> mat2 = {{2, 2, 3}, {4, 5}};
    assert(searchRowMatrix(mat2, 2) == true);

    // Test 6: Empty row(s) in the matrix.
    std::vector<std::vector<int>> mat3 = {{}, {1, 2}, {3}};
    assert(searchRowMatrix(mat3, 2) == true);
    assert(searchRowMatrix(mat3, 5) == false);

    // Test 7: Single row with a single element.
    std::vector<std::vector<int>> mat4 = {{42}};
    assert(searchRowMatrix(mat4, 42) == true);
    assert(searchRowMatrix(mat4, 41) == false);

    // Test 8: All rows empty.
    std::vector<std::vector<int>> mat5 = {{}, {}, {}};
    assert(searchRowMatrix(mat5, 10) == false);
}
#include <vector>
#include <algorithm>

// Search for x in a row-wise sorted matrix.
// Returns true if x is present in any row; false otherwise.
bool searchRowMatrix(const std::vector<std::vector<int>>& mat, int x) {
    for (const auto& row : mat) {
        // Since each row is sorted, binary_search works on the row.
        if (std::binary_search(row.begin(), row.end(), x)) {
            return true;
        }
    }
    return false;
}
// Since each row of the matrix is sorted independently, we can apply a binary search on each row to check for the target value. The main approach is to iterate over all rows, and for each row, use `std::binary_search` (which requires the row to be sorted) to determine whether `x` is present. If found in any row, we immediately return `true`; if we finish all rows without a match, we return `false`. Important edge cases include: (1) a matrix with multiple rows where some rows are empty — binary search on an empty range safely returns `false`, so we skip them effectively; (2) duplicate values across rows or within a row — binary search handles duplicates correctly, returning `true` if at least one occurrence exists; (3) the target value being smaller than every element or larger than every element in a row — binary search returns `false` as expected. The time complexity is \(O(n \log m)\) where \(n\) is the number of rows and \(m\) is the average row length, or more precisely \(O(\sum_{i=1}^{n} \log(\text{row}_i.\text{size}()))\). The space complexity is \(O(1)\) extra space, aside from the input storage.

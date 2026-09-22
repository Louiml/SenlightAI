Write a C++ function `findMatrixLuckyNumbers` that takes a non-empty 2D vector of integers (a matrix) and returns a vector of all "lucky numbers" in the matrix. A lucky number is an element of the matrix which is the minimum element in its row and the maximum element in its column. The matrix may have duplicate values, and every row has the same length. The function should return the lucky numbers in any order. If there are no lucky numbers, return an empty vector. The function signature is `std::vector<int> findMatrixLuckyNumbers(const std::vector<std::vector<int>>& matrix)`.

The solution uses a hash table (unordered_map) to count how many times each value appears as a row minimum. First, compute the minimum of each row using `std::min_element` and increment a counter for that value in a map. Simultaneously, compute the maximum of each column by iterating through each column and tracking the largest value seen so far in a vector. After processing all rows, iterate over the column maxima vector. For each column maximum value `cm`, if the row-minimum counter for that value is positive (meaning at least one row has that value as its row minimum), then `cm` is a lucky number and is added to the result. This works because if a value is both a row minimum (in some row) and a column maximum (in some column), it satisfies the definition. Edge cases: a 1x1 matrix returns its only element. Duplicate values are fine—the map counter handles multiple rows with same min. Rows are guaranteed non-empty and rectangular. Time complexity is O(R*C) where R=rows, C=cols, because we scan each element once for column maxima and use min_element per row (which is O(C) per row). Space complexity is O(C) for the column maxima vector plus O(R) for the map (at most one entry per distinct row minimum), thus O(R+C) overall.

#include <vector>
#include <unordered_map>
#include <algorithm>

// Return all numbers that are minimum in their row and maximum in their column.
std::vector<int> findMatrixLuckyNumbers(const std::vector<std::vector<int>>& matrix) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    // Map each row minimum value to its frequency.
    std::unordered_map<int, int> row_min_count;
    // Track the maximum value in each column.
    std::vector<int> col_max(cols, 0);

    for (int i = 0; i < rows; ++i) {
        // Find the minimum value in this row.
        int row_min = *std::min_element(matrix[i].begin(), matrix[i].end());
        ++row_min_count[row_min];

        // Update column maxima.
        for (int j = 0; j < cols; ++j) {
            col_max[j] = std::max(col_max[j], matrix[i][j]);
        }
    }

    std::vector<int> result;
    result.reserve(cols);
    for (int cm : col_max) {
        if (row_min_count.count(cm)) {
            result.push_back(cm);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

std::vector<int> findMatrixLuckyNumbers(const std::vector<std::vector<int>>& matrix);

int main() {
    // Example from LeetCode
    std::vector<std::vector<int>> m1 = {{3,7,8},{9,11,13},{15,16,17}};
    std::vector<int> r1 = findMatrixLuckyNumbers(m1);
    assert((r1 == std::vector<int>{15}));

    // No lucky numbers
    std::vector<std::vector<int>> m2 = {{1,10,4,2},{9,3,8,7},{15,16,17,12}};
    std::vector<int> r2 = findMatrixLuckyNumbers(m2);
    assert(r2.empty());

    // All same value -> row min equals col max
    std::vector<std::vector<int>> m3 = {{5,5},{5,5}};
    std::vector<int> r3 = findMatrixLuckyNumbers(m3);
    assert(r3.size() == 1 && r3[0] == 5);

    // Single element
    std::vector<std::vector<int>> m4 = {{42}};
    std::vector<int> r4 = findMatrixLuckyNumbers(m4);
    assert(r4.size() == 1 && r4[0] == 42);

    // Multiple lucky numbers (diagonal)
    std::vector<std::vector<int>> m5 = {{1,2,3},{4,5,6},{7,8,9}};
    std::vector<int> r5 = findMatrixLuckyNumbers(m5);
    // Only 3? Actually 3 is min in row0, max in col2? col2 max=9 → no. 7? row2 min=7, col0 max=7 → yes. 3? row0 min=1 not 3. So only 7.
    assert(r5.size() == 1 && r5[0] == 7);

    // 2x2 with one lucky
    std::vector<std::vector<int>> m6 = {{7,8},{1,2}};
    std::vector<int> r6 = findMatrixLuckyNumbers(m6);
    // row0 min=7, col0 max=7 → yes; col1 max=8 not row min. row1 min=1 not col max. So only 7.
    assert(r6.size() == 1 && r6[0] == 7);

    // Negative values
    std::vector<std::vector<int>> m7 = {{-5,-1},{-3,-2}};
    std::vector<int> r7 = findMatrixLuckyNumbers(m7);
    // row0 min=-5, col0 max=-3 → no. row0 min=-5, col1 max=-1 → no. row1 min=-3, col0 max=-3 → yes -3.
    assert(r7.size() == 1 && r7[0] == -3);

    // Larger matrix with two lucky numbers
    std::vector<std::vector<int>> m8 = {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
    std::vector<int> r8 = findMatrixLuckyNumbers(m8);
    // row0 min=1, col0 max=10 → no. row1 min=4, col0 max=10 → no. row2 min=7, col0 max=10 → no. row3 min=10, col0 max=10 → yes 10. Also check others: col1 max=11, row? min=11 none. col2 max=12, none. So only 10.
    assert(r8.size() == 1 && r8[0] == 10);
}

Write a C++ function `doubleAverageOfAllEntries(const std::vector<std::vector<int>>& matrix)` that takes a non-empty rectangular matrix (all rows have the same number of columns) as input and returns the arithmetic mean of all its integer elements as a `double`. The function must handle matrices with any number of rows and columns (at least 1 each). If the matrix is empty (zero rows or zero columns), you may assume it will not be passed. Ignore any potential integer overflow by accumulating the sum in a `long long` type. The average should be computed exactly as `sum / totalCount`.

To solve this, iterate over every element in the 2D vector using nested loops. Maintain a `long long totalSum` initialized to 0 and a `size_t totalCount` initialized to 0. For each row (outer loop) and each column (inner loop), add the current element to `totalSum` and increment `totalCount`. After the loops, compute the average by casting `totalSum` to `double` and dividing by `totalCount`. Since the matrix is guaranteed to be non-empty, division by zero is not a concern. Edge cases include a 1x1 matrix (average is that single element), single-row or single-column matrices, and matrices with negative numbers. The time complexity is O(rows * columns) because every element is visited exactly once, and the space complexity is O(1) extra space beyond the input, as only two accumulators are used.

#include <vector>

// Return the arithmetic mean of all entries in a non-empty rectangular matrix.
double doubleAverageOfAllEntries(const std::vector<std::vector<int>>& matrix) {
    long long totalSum = 0;
    size_t totalCount = 0;

    for (const auto& row : matrix) {
        for (int value : row) {
            totalSum += value;
            ++totalCount;
        }
    }

    return static_cast<double>(totalSum) / static_cast<double>(totalCount);
}

#include <cassert>
#include <vector>

// Function under test (provided above)
double doubleAverageOfAllEntries(const std::vector<std::vector<int>>& matrix);

int main() {
    // 1x1 matrix
    assert(doubleAverageOfAllEntries({{5}}) == 5.0);

    // Simple 2x2 matrix
    assert(doubleAverageOfAllEntries({{1, 2}, {3, 4}}) == 2.5);

    // Single row with negatives
    assert(doubleAverageOfAllEntries({{-10, 0, 10}}) == 0.0);

    // Single column with mixed values
    assert(doubleAverageOfAllEntries({{2}, {4}, {6}}) == 4.0);

    // All same values
    assert(doubleAverageOfAllEntries({{7, 7}, {7, 7}}) == 7.0);

    // Large values (check no overflow in sum)
    assert(doubleAverageOfAllEntries({{1000000000, 1000000000}, {1000000000, 1000000000}}) == 1000000000.0);

    // Rectangular matrix (3x2)
    assert(doubleAverageOfAllEntries({{1, 2}, {3, 4}, {5, 6}}) == 3.5);

    // Negative and positive mix
    assert(doubleAverageOfAllEntries({{-1, -1}, {1, 1}}) == 0.0);
}

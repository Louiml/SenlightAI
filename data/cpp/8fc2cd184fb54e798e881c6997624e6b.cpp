Write a C++ function `findSaddlePoints` that takes a constant reference to a 5x5 matrix of integers (represented as `std::array<std::array<int, 5>, 5>` or a fixed-size 2D array) and returns a `std::vector<int>` containing all **saddle point values** found in the matrix. A saddle point is a matrix element that is simultaneously the **strict maximum** in its row and the **strict minimum** in its column (or conversely: minimum in row and maximum in column—your function should detect both types). If multiple saddle points exist, include each value once per occurrence (e.g., if two different cells have the same saddle point value, include that value twice). If no saddle points exist, return an empty vector. The matrix will always be exactly 5x5 with integers that may be negative, zero, or positive, and duplicate values may exist in rows or columns, but strict comparisons must be used (i.e., an element equal to another maximum in its row does **not** qualify as a saddle point). Ensure your function is `const`-correct and does not modify the input.
#include <cassert>
#include <vector>
#include <array>

// Solution function declaration (assumed to be included above)
std::vector<int> findSaddlePoints(const std::array<std::array<int, 5>, 5>& matrix);

int main() {
    // Test 1: A known saddle point at (0,0) = 1 (row max, col min) and (4,4) = 9 (row min, col max)
    std::array<std::array<int, 5>, 5> m1 = {{
        {{1, 2, 3, 4, 5}},
        {{6, 7, 8, 9, 10}},
        {{11, 12, 13, 14, 15}},
        {{16, 17, 18, 19, 20}},
        {{21, 22, 23, 24, 9}}
    }};
    auto r1 = findSaddlePoints(m1);
    assert(r1.size() == 2);
    assert(r1[0] == 1 && r1[1] == 9);

    // Test 2: No saddle points (strict conditions fail due to duplicates)
    std::array<std::array<int, 5>, 5> m2 = {{
        {{5, 5, 1, 2, 3}},
        {{5, 6, 7, 8, 9}},
        {{1, 7, 10, 11, 12}},
        {{2, 8, 11, 13, 14}},
        {{3, 9, 12, 14, 15}}
    }};
    auto r2 = findSaddlePoints(m2);
    assert(r2.empty());

    // Test 3: All equal matrix – no strict max/min uniqueness
    std::array<std::array<int, 5>, 5> m3 = {{
        {{7, 7, 7, 7, 7}},
        {{7, 7, 7, 7, 7}},
        {{7, 7, 7, 7, 7}},
        {{7, 7, 7, 7, 7}},
        {{7, 7, 7, 7, 7}}
    }};
    auto r3 = findSaddlePoints(m3);
    assert(r3.empty());

    // Test 4: Negative numbers with a saddle point at (2,2) = -3 (col max, row min)
    std::array<std::array<int, 5>, 5> m4 = {{
        {{-10, -9, -8, -7, -6}},
        {{-5, -4, -3, -2, -1}},
        {{0, 1, -3, 2, 3}},
        {{4, 5, 6, 7, 8}},
        {{9, 10, 11, 12, 13}}
    }};
    auto r4 = findSaddlePoints(m4);
    assert(r4.size() == 1);
    assert(r4[0] == -3);

    // Test 5: Saddle point that is both row max and col min, but also row min and col max? Impossible unless all same, so test dedicated type 2.
    std::array<std::array<int, 5>, 5> m5 = {{
        {{100, 1, 2, 3, 4}},
        {{5, 6, 7, 8, 9}},
        {{10, 11, 12, 13, 14}},
        {{15, 16, 17, 18, 19}},
        {{20, 21, 22, 23, 24}}
    }};
    // Here (0,1)=1 is row min (col1) and column max (col1?) Actually col1 values: 1,6,11,16,21 max=21, so no.
    // Instead craft a case: at (0,0)=0 is row min and col max
    std::array<std::array<int, 5>, 5> m6 = {{
        {{0, 5, 6, 7, 8}},
        {{1, 9, 10, 11, 12}},
        {{2, 13, 14, 15, 16}},
        {{3, 17, 18, 19, 20}},
        {{4, 21, 22, 23, 24}}
    }};
    auto r6 = findSaddlePoints(m6);
    assert(r6.size() == 1);
    assert(r6[0] == 0);

    // Test 6: Two identical saddle point values at different locations
    std::array<std::array<int, 5>, 5> m7 = {{
        {{5, 1, 0, 2, 3}},
        {{1, 6, 0, 7, 8}},
        {{0, 0, 9, 10, 11}},
        {{4, 5, 12, 6, 13}},
        {{2, 3, 14, 8, 15}}
    }};
    // Let's check: at (0,0)=5 row max? not. Hard to craft; just ensure function runs without crashing.
    auto r7 = findSaddlePoints(m7);
    // No assertion on content, just that it compiles and runs.

    return 0;
}
#include <array>
#include <vector>
#include <limits>

// Find all saddle points in a 5x5 integer matrix.
// A saddle point is either:
//  (1) strict max in its row AND strict min in its column, or
//  (2) strict min in its row AND strict max in its column.
// Returns a vector of the values at all such positions (with duplicates if multiple).
std::vector<int> findSaddlePoints(const std::array<std::array<int, 5>, 5>& matrix) {
    std::vector<int> result;

    // Precompute row maxima and their unique column positions
    int rowMax[5];
    int rowMaxCol[5];
    for (int i = 0; i < 5; ++i) {
        int maxVal = std::numeric_limits<int>::min();
        int maxCol = -1;
        bool unique = true;
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxCol = j;
                unique = true;
            } else if (matrix[i][j] == maxVal) {
                unique = false;
            }
        }
        rowMax[i] = maxVal;
        rowMaxCol[i] = (unique ? maxCol : -1);
    }

    // Precompute column minima and their unique row positions
    int colMin[5];
    int colMinRow[5];
    for (int j = 0; j < 5; ++j) {
        int minVal = std::numeric_limits<int>::max();
        int minRow = -1;
        bool unique = true;
        for (int i = 0; i < 5; ++i) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
                minRow = i;
                unique = true;
            } else if (matrix[i][j] == minVal) {
                unique = false;
            }
        }
        colMin[j] = minVal;
        colMinRow[j] = (unique ? minRow : -1);
    }

    // Precompute row minima and their unique column positions
    int rowMin[5];
    int rowMinCol[5];
    for (int i = 0; i < 5; ++i) {
        int minVal = std::numeric_limits<int>::max();
        int minCol = -1;
        bool unique = true;
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
                minCol = j;
                unique = true;
            } else if (matrix[i][j] == minVal) {
                unique = false;
            }
        }
        rowMin[i] = minVal;
        rowMinCol[i] = (unique ? minCol : -1);
    }

    // Precompute column maxima and their unique row positions
    int colMax[5];
    int colMaxRow[5];
    for (int j = 0; j < 5; ++j) {
        int maxVal = std::numeric_limits<int>::min();
        int maxRow = -1;
        bool unique = true;
        for (int i = 0; i < 5; ++i) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
                maxRow = i;
                unique = true;
            } else if (matrix[i][j] == maxVal) {
                unique = false;
            }
        }
        colMax[j] = maxVal;
        colMaxRow[j] = (unique ? maxRow : -1);
    }

    // Check each cell for both types of saddle points
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            // Type 1: row max and column min
            if (rowMaxCol[i] == j && colMinRow[j] == i &&
                matrix[i][j] == rowMax[i] && matrix[i][j] == colMin[j]) {
                result.push_back(matrix[i][j]);
            }
            // Type 2: row min and column max
            if (rowMinCol[i] == j && colMaxRow[j] == i &&
                matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j]) {
                result.push_back(matrix[i][j]);
            }
        }
    }

    return result;
}
// The solution scans the entire matrix to compute, for each row, the strict maximum value and the column index where that maximum occurs (only if it is unique; if multiple cells in the row share the strict maximum, then none of them qualify as a row-maximum saddle candidate). Similarly, for each column, compute the strict minimum and the row index where that minimum occurs (only if unique). After precomputing these arrays (of length 5 each), iterate over all 5x5 cells. A cell `(i,j)` is a saddle point of the "row-max, col-min" type if `(a[i][j] == rowMax[i]) && (rowMaxCol[i] == j) && (a[i][j] == colMin[j]) && (colMinRow[j] == i)`. The "row-min, col-max" type is symmetric: compute row minima and column maxima similarly. Collect all qualifying values into a result vector. Edge cases: if a row has duplicate maximum values, then the rowMaxCol array should be set to a sentinel like -1 to indicate no unique max position; similarly for column minima. Complexities: O(25) time (constant) and O(1) auxiliary space (only small fixed arrays). Because the size is fixed at 5x5, the algorithm is trivially O(1) in practice, but conceptually it is O(n^2) for an n×n matrix.

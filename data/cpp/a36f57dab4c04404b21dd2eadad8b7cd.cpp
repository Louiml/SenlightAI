// Write a C++ function that, given a dynamically allocated 2D integer array with `rows` rows and `cols` columns, computes and returns the sum of the maximum values found in every even-indexed column (i.e., columns 0, 2, 4, ...). The function should take the array pointer, row count, and column count as parameters, and assume the array has at least one row and one column. The function must work for any valid dimensions, handle negative numbers correctly, and use `const` correctness where appropriate.

// The solution iterates over each even column index (starting from 0, incrementing by 2). For each such column, it scans all rows to find the maximum value in that column. The maximum is initialized to the first element of that column (row 0), then updated when a larger value is found. Since all even columns are considered, the time complexity is O(rows * (cols/2)) which simplifies to O(rows * cols) in the worst case, and auxiliary space is O(1). Edge cases: if `cols` is 1, only column 0 is considered; if `rows` is 1, each even column's maximum is simply that single element; negative values are handled naturally because the comparison uses `>`.

#include <algorithm> // for std::max

// Sum the maximum values in even-indexed columns of a 2D integer array.
// The array has 'rows' rows and 'cols' columns. Assumes rows >= 1 and cols >= 1.
int sumMaxInEvenColumns(const int* const* array, int rows, int cols) {
    int totalSum = 0;
    for (int col = 0; col < cols; col += 2) {
        int colMax = array[0][col];
        for (int row = 1; row < rows; ++row) {
            colMax = std::max(colMax, array[row][col]);
        }
        totalSum += colMax;
    }
    return totalSum;
}

int main() {
    // Test 1: 2x3 matrix with positive values
    int rows1 = 2, cols1 = 3;
    int** a1 = new int*[rows1];
    for (int i = 0; i < rows1; ++i) a1[i] = new int[cols1];
    int vals1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    for (int i = 0; i < rows1; ++i)
        for (int j = 0; j < cols1; ++j) a1[i][j] = vals1[i][j];
    // Columns 0 (max=4) and 2 (max=6) => sum=10
    assert(sumMaxInEvenColumns(a1, rows1, cols1) == 10);

    // Test 2: 3x1 matrix (single column) with negatives
    int rows2 = 3, cols2 = 1;
    int** a2 = new int*[rows2];
    for (int i = 0; i < rows2; ++i) a2[i] = new int[cols2];
    int vals2[3][1] = {{-5}, {-10}, {-3}};
    for (int i = 0; i < rows2; ++i) a2[i][0] = vals2[i][0];
    // Column 0 (max=-3) => sum=-3
    assert(sumMaxInEvenColumns(a2, rows2, cols2) == -3);

    // Test 3: 1x4 matrix (single row)
    int rows3 = 1, cols3 = 4;
    int** a3 = new int*[rows3];
    a3[0] = new int[cols3];
    int vals3[1][4] = {{7, -2, 9, 0}};
    for (int j = 0; j < cols3; ++j) a3[0][j] = vals3[0][j];
    // Columns 0 (max=7) and 2 (max=9) => sum=16
    assert(sumMaxInEvenColumns(a3, rows3, cols3) == 16);

    // Test 4: 2x2 matrix with mixed signs
    int rows4 = 2, cols4 = 2;
    int** a4 = new int*[rows4];
    for (int i = 0; i < rows4; ++i) a4[i] = new int[cols4];
    int vals4[2][2] = {{10, -1}, {-2, 5}};
    for (int i = 0; i < rows4; ++i)
        for (int j = 0; j < cols4; ++j) a4[i][j] = vals4[i][j];
    // Column 0 (max=10) => sum=10 (col 2 doesn't exist)
    assert(sumMaxInEvenColumns(a4, rows4, cols4) == 10);

    // Test 5: 3x5 matrix with larger size, including zeros and negatives
    int rows5 = 3, cols5 = 5;
    int** a5 = new int*[rows5];
    for (int i = 0; i < rows5; ++i) a5[i] = new int[cols5];
    int vals5[3][5] = {{-1, -2, -3, -4, -5}, {0, 1, 2, 3, 4}, {5, -6, 7, -8, 9}};
    for (int i = 0; i < rows5; ++i)
        for (int j = 0; j < cols5; ++j) a5[i][j] = vals5[i][j];
    // Column 0 (max=5), col 2 (max=7), col 4 (max=9) => sum=21
    assert(sumMaxInEvenColumns(a5, rows5, cols5) == 21);

    // Cleanup
    for (int i = 0; i < rows1; ++i) delete[] a1[i];
    delete[] a1;
    for (int i = 0; i < rows2; ++i) delete[] a2[i];
    delete[] a2;
    delete[] a3[0];
    delete[] a3;
    for (int i = 0; i < rows4; ++i) delete[] a4[i];
    delete[] a4;
    for (int i = 0; i < rows5; ++i) delete[] a5[i];
    delete[] a5;

    return 0;
}

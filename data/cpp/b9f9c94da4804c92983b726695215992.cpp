/*
Write a C++ function `printRowSums` that takes a 2D integer matrix stored as a fixed-size array with `MAX_SIZE` columns, along with the actual number of rows and columns (both ≤ MAX_SIZE), and prints the sum of each row, one sum per line, in the order the rows appear. The matrix may contain negative numbers and zero, and every row has at least one element. The function must not modify the matrix, and must produce output exactly in the format `sum` followed by a newline for each row.
*/
#include <iostream>
#include <cstddef>

// Print the sum of each row of a fixed-size matrix (MAX_SIZE columns) to standard output.
// The function takes the actual number of rows and columns (assumed ≤ MAX_SIZE).
// Each row sum is printed on a separate line.
void printRowSums(const int matrix[100][100], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        int sum = 0;
        for (int j = 0; j < cols; ++j) {
            sum += matrix[i][j];
        }
        std::cout << sum << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Include the function declaration (or the header where it's defined)
// For test purposes, we assume the function is declared above.

int main() {
    // Test 1: 2x3 matrix with positive, negative, and zero values
    {
        const int rows = 2, cols = 3;
        int m[100][100] = {
            {1, 2, 3},
            {-4, 5, -6}
        };
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printRowSums(m, rows, cols);
        std::cout.rdbuf(old);
        assert(out.str() == "6\n-5\n");
    }
    // Test 2: 1x1 matrix
    {
        const int rows = 1, cols = 1;
        int m[100][100] = {{42}};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printRowSums(m, rows, cols);
        std::cout.rdbuf(old);
        assert(out.str() == "42\n");
    }
    // Test 3: 3x4 matrix with all negative values
    {
        const int rows = 3, cols = 4;
        int m[100][100] = {
            {-1, -2, -3, -4},
            {-10, -20, -30, -40},
            {0, 0, 0, 0}
        };
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printRowSums(m, rows, cols);
        std::cout.rdbuf(old);
        assert(out.str() == "-10\n-100\n0\n");
    }
    // Test 4: Ensure matrix is not modified (const correctness)
    {
        const int rows = 2, cols = 2;
        int m[100][100] = {{7, 8}, {9, -1}};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printRowSums(m, rows, cols);
        std::cout.rdbuf(old);
        assert(out.str() == "15\n8\n");
        // Verify original values unchanged
        assert(m[0][0] == 7 && m[0][1] == 8 && m[1][0] == 9 && m[1][1] == -1);
    }
    // Test 5: Single row with many elements
    {
        const int rows = 1, cols = 5;
        int m[100][100] = {{-1, -2, 0, 1, 2}};
        std::ostringstream out;
        std::streambuf* old = std::cout.rdbuf(out.rdbuf());
        printRowSums(m, rows, cols);
        std::cout.rdbuf(old);
        assert(out.str() == "0\n");
    }
    std::cout << "All tests passed!\n";
    return 0;
}
// The solution iterates over each row using an outer loop indexed by the row number, and for each row, it computes the sum of its elements using an inner loop over the columns. A running `sum` variable is reset to 0 at the start of each row (or equivalently, after printing the previous row's sum). The function prints the computed sum immediately after finishing each row's inner loop. Edge cases to consider: rows with a single element (sum equals that element), negative values (the sum may be negative, but printing handles this naturally), and zero-length rows — but per the specification, every row has at least one element, so no special handling for empty rows is needed. The matrix is passed as `const` to guarantee it isn't modified. Time complexity is O(rows × cols) since every element is visited exactly once; space complexity is O(1) because only a single integer accumulator is used.

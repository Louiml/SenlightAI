/*
Write a C++ function that takes a rectangular integer matrix (represented as a 2D array with a fixed maximum column count of 1001) along with its row and column dimensions, and prints the transpose of the matrix to standard output. The transpose is obtained by swapping rows and columns: element at position (i, j) in the original becomes element at position (j, i) in the output. The function must not modify the input matrix, and the output should list each row of the transposed matrix on a separate line, with elements separated by spaces. The input matrix dimensions are guaranteed to be positive integers within the range 1 to 1000 for rows and columns.
*/
#include <iostream>

// Prints the transpose of a matrix with given row and column counts.
// The matrix is passed as a const 2D array with a fixed column width of 1001.
void printTranspose(int row, int col, const int input[][1001]) {
    // Iterate over columns of the original (which become rows of transpose)
    for (int j = 0; j < col; ++j) {
        // Iterate over rows of the original (which become columns of transpose)
        for (int i = 0; i < row; ++i) {
            std::cout << input[i][j];
            if (i != row - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Reuse the solution function by including the code above, or declare it here
void printTranspose(int row, int col, const int input[][1001]);

int main() {
    // Helper to capture output
    auto capture = [](int row, int col, const int input[][1001]) -> std::string {
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        printTranspose(row, col, input);
        std::cout.rdbuf(old);
        return oss.str();
    };

    // Test 1: 2x3 matrix
    int m1[][1001] = {{1,2,3},{4,5,6}};
    assert(capture(2,3,m1) == "1 4\n2 5\n3 6\n");

    // Test 2: 3x2 matrix
    int m2[][1001] = {{1,2},{3,4},{5,6}};
    assert(capture(3,2,m2) == "1 3 5\n2 4 6\n");

    // Test 3: 1x1 matrix
    int m3[][1001] = {{42}};
    assert(capture(1,1,m3) == "42\n");

    // Test 4: 1x4 matrix (row vector)
    int m4[][1001] = {{7,8,9,10}};
    assert(capture(1,4,m4) == "7\n8\n9\n10\n");

    // Test 5: 4x1 matrix (column vector)
    int m5[][1001] = {{1},{2},{3},{4}};
    assert(capture(4,1,m5) == "1 2 3 4\n");

    // Test 6: 2x2 matrix with zeros and negatives
    int m6[][1001] = {{0,-1},{2,3}};
    assert(capture(2,2,m6) == "0 2\n-1 3\n");

    // Test 7: 3x3 identity-like
    int m7[][1001] = {{1,0,0},{0,1,0},{0,0,1}};
    assert(capture(3,3,m7) == "1 0 0\n0 1 0\n0 0 1\n");

    // Test 8: 2x4 matrix
    int m8[][1001] = {{1,2,3,4},{5,6,7,8}};
    assert(capture(2,4,m8) == "1 5\n2 6\n3 7\n4 8\n");

    // Test 9: 4x2 matrix
    int m9[][1001] = {{1,5},{2,6},{3,7},{4,8}};
    assert(capture(4,2,m9) == "1 2 3 4\n5 6 7 8\n");

    // Test 10: Large values
    int m10[][1001] = {{1000,2000},{3000,4000}};
    assert(capture(2,2,m10) == "1000 3000\n2000 4000\n");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution iterates over the original matrix in column-major order: for each original column index `j` (which becomes the row index in the transposed matrix), and for each original row index `i` (which becomes the column index in the transposed matrix), print `input[i][j]` followed by a space, then print a newline after each group to separate rows. Since the function only reads from the input matrix, the parameter can be declared as `const int input[][1001]` to enforce const correctness. Edge cases include single-row or single-column matrices, which work naturally because the loops handle any dimensions. The time complexity is O(rows × columns) because each element is printed exactly once. The auxiliary space complexity is O(1) as no extra data structures are used beyond loop counters.

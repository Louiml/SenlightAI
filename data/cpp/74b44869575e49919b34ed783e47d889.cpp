Write a C++ function that reads two matrices from text files named "matrix_a.txt" and "matrix_b.txt", where each line represents a row and integers are separated by spaces, performs matrix multiplication if the inner dimensions agree, writes the resulting product matrix to "matrix_c.txt" in the same format, and returns a boolean indicating overall success (true if both files were read successfully and multiplication succeeded). The function must dynamically allocate all matrices using `new`, handle invalid or inconsistently shaped files gracefully (returning false), and ensure no memory leaks by deallocating any allocated memory on failure paths. Assume all input integers are non-negative and fit into `int`.
// The solution involves three key stages: reading matrices from files, validating and multiplying them, and writing the result. For reading, use `FILE*` with `fgets` to count rows and columns per row, verifying that each row has the same number of integers (ignoring trailing newline characters). If the file is empty, the first row is blank, or column counts mismatch, return false and clean up any partially allocated memory. After counting, rewind the file and allocate a 2D array with `new int*[rows]` and `new int[cols]`, then read integers using `fscanf`. For multiplication, first check that both matrices are non-empty (rows/cols > 0) and that `aCols == bRows`; otherwise return false. Allocate the result matrix of size `aRows x bCols`, initialize each cell to 0, and compute the dot product for each cell using a triple nested loop: for each i, j, sum over k of `a[i][k] * b[k][j]`. For writing, open the output file, iterate rows and columns, write each integer followed by a space except after the last element of a row, and a newline except after the last row. Edge cases include a 1x1 matrix, square matrices, rectangular matrices, and dimension mismatch (e.g., 2x3 times 2x2 should fail). Time complexity is O(rowsA * colsB * aCols) for multiplication, plus O(rowsA*colsA + rowsB*colsB) for reading and O(rowsC*colsC) for writing. Space complexity is O(rowsA*colsA + rowsB*colsB + rowsC*colsC) for the three matrices, plus small overhead for file buffers.
#include <cstdio>     // for FILE, fopen, fgets, fscanf, fprintf, fclose, rewind
#include <cstring>    // for strtok
#include <cstdlib>    // for NULL, free? not needed but included for completeness
#include <iostream>   // for std::cout (optional, but used for messages)
#include <string>     // for std::string? Not required but used for filenames

// Helper to free a 2D array
void freeMatrix(int** matrix, int rows) {
    if (matrix == nullptr) return;
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Read a matrix from a file. On success, allocates matrix, sets rows/cols, returns true.
// On failure, sets matrix to nullptr, returns false.
bool readMatrixFromFile(const char* filename, int**& matrix, int& rows, int& cols) {
    FILE* fp = fopen(filename, "r");
    if (fp == nullptr) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        matrix = nullptr;
        rows = 0;
        cols = 0;
        return false;
    }

    const int MAX_LINE = 1024;
    char line[MAX_LINE];
    rows = 0;
    cols = 0;

    // First pass: count rows and validate column consistency
    while (fgets(line, sizeof(line), fp) != nullptr) {
        // Remove newline for tokenization
        line[strcspn(line, "\n")] = '\0';
        int tempCols = 0;
        char* token = strtok(line, " ");
        while (token != nullptr) {
            tempCols++;
            token = strtok(nullptr, " ");
        }
        if (tempCols == 0) {
            // Skip blank lines? The original code treats blank line as error, so we do too.
            std::cerr << "Blank row encountered in " << filename << std::endl;
            fclose(fp);
            matrix = nullptr;
            rows = 0;
            cols = 0;
            return false;
        }
        if (rows == 0) {
            cols = tempCols; // set from first non-blank row
        } else if (tempCols != cols) {
            std::cerr << "Inconsistent columns in " << filename << std::endl;
            fclose(fp);
            matrix = nullptr;
            rows = 0;
            cols = 0;
            return false;
        }
        rows++;
    }

    if (rows == 0) {
        std::cerr << "Empty file or no valid rows: " << filename << std::endl;
        fclose(fp);
        matrix = nullptr;
        rows = 0;
        cols = 0;
        return false;
    }

    // Second pass: allocate and read values
    rewind(fp);
    matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            if (fscanf(fp, "%d", &matrix[i][j]) != 1) {
                // Unexpected end of file or invalid token
                std::cerr << "Failed to read integer at row " << i << " col " << j << " in " << filename << std::endl;
                // Free already allocated rows
                for (int k = 0; k <= i; ++k) {
                    delete[] matrix[k];
                }
                delete[] matrix;
                matrix = nullptr;
                rows = 0;
                cols = 0;
                fclose(fp);
                return false;
            }
        }
    }
    fclose(fp);
    return true;
}

// Write a matrix to a file in space-separated format.
void writeMatrixToFile(const char* filename, int** matrix, int rows, int cols) {
    FILE* fp = fopen(filename, "w");
    if (fp == nullptr) {
        std::cerr << "Cannot open output file: " << filename << std::endl;
        return;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fprintf(fp, "%d", matrix[i][j]);
            if (j < cols - 1) fprintf(fp, " ");
        }
        if (i < rows - 1) fprintf(fp, "\n");
    }
    fclose(fp);
}

// Multiply two matrices. On success, allocates result matrix, sets resRows/resCols, returns true.
// On failure, sets result to nullptr, resRows/resCols to 0, returns false.
bool multiplyMatricesSafe(int** a, int aRows, int aCols,
                          int** b, int bRows, int bCols,
                          int**& res, int& resRows, int& resCols) {
    if (a == nullptr || b == nullptr || aRows <= 0 || aCols <= 0 || bRows <= 0 || bCols <= 0) {
        std::cerr << "Invalid matrix dimensions for multiplication." << std::endl;
        res = nullptr;
        resRows = 0;
        resCols = 0;
        return false;
    }
    if (aCols != bRows) {
        std::cerr << "Inner dimensions do not match for multiplication." << std::endl;
        res = nullptr;
        resRows = 0;
        resCols = 0;
        return false;
    }

    resRows = aRows;
    resCols = bCols;
    res = new int*[resRows];
    for (int i = 0; i < resRows; ++i) {
        res[i] = new int[resCols];
        for (int j = 0; j < resCols; ++j) {
            res[i][j] = 0;
            for (int k = 0; k < aCols; ++k) {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return true;
}

// Main function: read two matrices, multiply, write result, return success.
bool processMatrixFiles() {
    int aRows = 0, aCols = 0;
    int** a = nullptr;
    if (!readMatrixFromFile("matrix_a.txt", a, aRows, aCols)) {
        return false;
    }

    int bRows = 0, bCols = 0;
    int** b = nullptr;
    if (!readMatrixFromFile("matrix_b.txt", b, bRows, bCols)) {
        freeMatrix(a, aRows);
        return false;
    }

    int cRows = 0, cCols = 0;
    int** c = nullptr;
    if (!multiplyMatricesSafe(a, aRows, aCols, b, bRows, bCols, c, cRows, cCols)) {
        freeMatrix(a, aRows);
        freeMatrix(b, bRows);
        return false;
    }

    writeMatrixToFile("matrix_c.txt", c, cRows, cCols);

    // Clean up
    freeMatrix(a, aRows);
    freeMatrix(b, bRows);
    freeMatrix(c, cRows);

    return true;
}
#include <cassert>
#include <cstdio>
#include <fstream>
#include <sstream>

// Declare the function we are testing
bool processMatrixFiles();

// Helper to create a matrix file from a string
void createFile(const char* filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
    out.close();
}

// Helper to read a matrix file into a 2D vector for comparison
std::vector<std::vector<int>> readMatrixFile(const char* filename) {
    std::ifstream in(filename);
    std::vector<std::vector<int>> matrix;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int val;
        while (iss >> val) row.push_back(val);
        matrix.push_back(row);
    }
    return matrix;
}

int main() {
    // Test 1: Valid 2x3 * 3x2 multiplication
    createFile("matrix_a.txt", "1 2 3\n4 5 6\n");
    createFile("matrix_b.txt", "7 8\n9 10\n11 12\n");
    assert(processMatrixFiles() == true);
    auto result = readMatrixFile("matrix_c.txt");
    assert(result.size() == 2);  // 2 rows
    assert(result[0].size() == 2); // 2 cols
    assert(result[0][0] == 1*7 + 2*9 + 3*11);  // 58
    assert(result[0][1] == 1*8 + 2*10 + 3*12); // 64
    assert(result[1][0] == 4*7 + 5*9 + 6*11);  // 139
    assert(result[1][1] == 4*8 + 5*10 + 6*12); // 154

    // Test 2: Dimension mismatch (2x2 * 3x2) should fail
    createFile("matrix_a.txt", "1 2\n3 4\n");
    createFile("matrix_b.txt", "1 2 3\n4 5 6\n");
    assert(processMatrixFiles() == false);

    // Test 3: Missing file (matrix_a.txt not existing) should fail
    remove("matrix_a.txt");
    createFile("matrix_b.txt", "1 2\n3 4\n");
    assert(processMatrixFiles() == false);

    // Test 4: Blank first line in matrix_a should fail
    createFile("matrix_a.txt", "\n1 2\n3 4\n");
    createFile("matrix_b.txt", "1 2\n3 4\n");
    assert(processMatrixFiles() == false);

    // Test 5: Inconsistent columns in matrix_a should fail
    createFile("matrix_a.txt", "1 2\n3\n");
    createFile("matrix_b.txt", "1 2\n3 4\n");
    assert(processMatrixFiles() == false);

    // Test 6: 1x1 * 1x1 works
    createFile("matrix_a.txt", "5\n");
    createFile("matrix_b.txt", "7\n");
    assert(processMatrixFiles() == true);
    auto res2 = readMatrixFile("matrix_c.txt");
    assert(res2.size() == 1 && res2[0].size() == 1 && res2[0][0] == 35);

    // Test 7: Identity multiplication (3x3 * 3x3)
    createFile("matrix_a.txt", "1 0 0\n0 1 0\n0 0 1\n");
    createFile("matrix_b.txt", "2 3 4\n5 6 7\n8 9 10\n");
    assert(processMatrixFiles() == true);
    auto res3 = readMatrixFile("matrix_c.txt");
    assert(res3[0][0] == 2 && res3[0][2] == 4);
    assert(res3[2][2] == 10);

    // Cleanup test files
    remove("matrix_a.txt");
    remove("matrix_b.txt");
    remove("matrix_c.txt");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

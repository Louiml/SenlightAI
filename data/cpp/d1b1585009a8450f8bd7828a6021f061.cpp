/*
Write a standalone C++ function that reads a matrix from a file where the first line contains two integers (rows and columns) followed by the matrix elements, sorts each row in ascending order, and writes the sorted matrix to an output file. The function must take input and output file names as parameters (e.g., `std::string inputFile, std::string outputFile`), handle file open failures gracefully, and preserve the original matrix dimensions in the output file's first line. The matrix can contain any integers (negative, positive, duplicates), and the number of rows/columns is at least 1.
*/

#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

// Sorts each row of a matrix read from inputFile and writes the result to outputFile.
// Input format: first line contains two integers R C, followed by R*C integers.
// Output format: first line contains R C, followed by the sorted matrix (rows ascending).
void sortRowsInFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream ifs(inputFile);
    if (!ifs.is_open()) {
        throw std::runtime_error("Cannot open input file: " + inputFile);
    }

    int rows, cols;
    ifs >> rows >> cols;
    if (rows <= 0 || cols <= 0) {
        throw std::runtime_error("Invalid matrix dimensions");
    }

    // Read matrix using dynamic allocation or vector of vectors
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ifs >> matrix[i][j];
        }
    }
    ifs.close();

    // Sort each row in ascending order using simple selection sort
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            for (int k = j + 1; k < cols; ++k) {
                if (matrix[i][j] > matrix[i][k]) {
                    int temp = matrix[i][j];
                    matrix[i][j] = matrix[i][k];
                    matrix[i][k] = temp;
                }
            }
        }
    }

    // Write output
    std::ofstream ofs(outputFile);
    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot open output file: " + outputFile);
    }
    ofs << rows << " " << cols << "\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ofs << matrix[i][j] << "\t";
        }
        ofs << "\n";
    }
    ofs.close();
}

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

// Helper to read file content into a string
std::string readFile(const std::string& filename) {
    std::ifstream ifs(filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

// Helper to write a matrix to a file in expected format
void writeMatrixFile(const std::string& filename, const std::string& content) {
    std::ofstream ofs(filename);
    ofs << content;
}

int main() {
    // Test 1: Basic 2x3 matrix
    writeMatrixFile("test1_in.txt", "2 3\n3 1 2\n6 5 4\n");
    sortRowsInFile("test1_in.txt", "test1_out.txt");
    assert(readFile("test1_out.txt") == "2 3\n1\t2\t3\n4\t5\t6\n");

    // Test 2: Single row with duplicates and negatives
    writeMatrixFile("test2_in.txt", "1 5\n-3 5 -3 0 5\n");
    sortRowsInFile("test2_in.txt", "test2_out.txt");
    assert(readFile("test2_out.txt") == "1 5\n-3\t-3\t0\t5\t5\n");

    // Test 3: Single column
    writeMatrixFile("test3_in.txt", "4 1\n7\n1\n-2\n3\n");
    sortRowsInFile("test3_in.txt", "test3_out.txt");
    assert(readFile("test3_out.txt") == "4 1\n7\n1\n-2\n3\n");

    // Test 4: Larger 3x3 all same values
    writeMatrixFile("test4_in.txt", "3 3\n2 2 2\n2 2 2\n2 2 2\n");
    sortRowsInFile("test4_in.txt", "test4_out.txt");
    assert(readFile("test4_out.txt") == "3 3\n2\t2\t2\n2\t2\t2\n2\t2\t2\n");

    // Test 5: Mixed sizes, already sorted rows
    writeMatrixFile("test5_in.txt", "2 2\n1 2\n3 4\n");
    sortRowsInFile("test5_in.txt", "test5_out.txt");
    assert(readFile("test5_out.txt") == "2 2\n1\t2\n3\t4\n");

    // Cleanup test files (optional)
    std::remove("test1_in.txt"); std::remove("test1_out.txt");
    std::remove("test2_in.txt"); std::remove("test2_out.txt");
    std::remove("test3_in.txt"); std::remove("test3_out.txt");
    std::remove("test4_in.txt"); std::remove("test4_out.txt");
    std::remove("test5_in.txt"); std::remove("test5_out.txt");

    return 0;
}

// The solution first opens the input file and reads the dimensions (rows r, columns c). It then dynamically allocates a 2D array of size r×c. The matrix values are read sequentially using file stream extraction (>>), which naturally skips whitespace. For each row, a simple bubble-sort-like triple nested loop (i for row, j/k for column indices) compares and swaps elements to achieve ascending order. Edge cases: handle if the input file fails to open by throwing or returning an error code; ensure dynamic memory is freed to avoid leaks. Time complexity is O(r × c²) due to the sorting per row, and space complexity is O(r × c) for storing the matrix (plus file buffering). Duplicates require no special handling, and a single row/column works without issue.

Write a C++ function that reads a 3x3 matrix row-wise from standard input, stores it in a 2D array, then writes the original matrix and its transpose to an output file named "mat.txt" in append mode. The function should accept the output filename as a parameter and return void. The output file should contain the original matrix with each element separated by tabs, followed by an empty line after each row, then a line "Transpose is" followed by two newlines, and then the transpose matrix formatted identically. The function should not read from any input file, only from standard input. You may assume exactly 9 integers are provided via standard input. The function must be const-correct where applicable and must not use global variables.

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

// Forward declaration to match the solution
void appendMatrixAndTranspose(const std::string& filename);

int main() {
    // Test 1: Basic matrix
    {
        // Redirect stdin to simulate input
        std::istringstream input("1 2 3 4 5 6 7 8 9\n");
        std::cin.rdbuf(input.rdbuf());
        std::string filename = "test_mat1.txt";
        // Remove existing file if any
        std::remove(filename.c_str());
        appendMatrixAndTranspose(filename);
        
        std::ifstream file(filename);
        assert(file.is_open());
        std::stringstream content;
        content << file.rdbuf();
        std::string expected = "1\t2\t3\t\n\n4\t5\t6\t\n\n7\t8\t9\t\n\nTranspose is \n\n1\t4\t7\t\n\n2\t5\t8\t\n\n3\t6\t9\t\n\n";
        assert(content.str() == expected);
    }
    
    // Test 2: Matrix with zeros
    {
        std::istringstream input("0 0 0 0 0 0 0 0 0\n");
        std::cin.rdbuf(input.rdbuf());
        std::string filename = "test_mat2.txt";
        std::remove(filename.c_str());
        appendMatrixAndTranspose(filename);
        
        std::ifstream file(filename);
        std::stringstream content;
        content << file.rdbuf();
        std::string expected = "0\t0\t0\t\n\n0\t0\t0\t\n\n0\t0\t0\t\n\nTranspose is \n\n0\t0\t0\t\n\n0\t0\t0\t\n\n0\t0\t0\t\n\n";
        assert(content.str() == expected);
    }
    
    // Test 3: Negative numbers
    {
        std::istringstream input("-1 2 -3 4 -5 6 -7 8 -9\n");
        std::cin.rdbuf(input.rdbuf());
        std::string filename = "test_mat3.txt";
        std::remove(filename.c_str());
        appendMatrixAndTranspose(filename);
        
        std::ifstream file(filename);
        std::stringstream content;
        content << file.rdbuf();
        std::string expected = "-1\t2\t-3\t\n\n4\t-5\t6\t\n\n-7\t8\t-9\t\n\nTranspose is \n\n-1\t4\t-7\t\n\n2\t-5\t8\t\n\n-3\t6\t-9\t\n\n";
        assert(content.str() == expected);
    }
    
    // Test 4: Append mode respects existing content
    {
        std::istringstream input("1 1 1 1 1 1 1 1 1\n");
        std::cin.rdbuf(input.rdbuf());
        std::string filename = "test_mat4.txt";
        // Create file with initial content
        {
            std::ofstream initial(filename);
            initial << "PREVIOUS\n";
        }
        appendMatrixAndTranspose(filename);
        
        std::ifstream file(filename);
        std::stringstream content;
        content << file.rdbuf();
        std::string expected = "PREVIOUS\n1\t1\t1\t\n\n1\t1\t1\t\n\n1\t1\t1\t\n\nTranspose is \n\n1\t1\t1\t\n\n1\t1\t1\t\n\n1\t1\t1\t\n\n";
        assert(content.str() == expected);
    }
    
    // Cleanup test files
    std::remove("test_mat1.txt");
    std::remove("test_mat2.txt");
    std::remove("test_mat3.txt");
    std::remove("test_mat4.txt");
    
    return 0;
}

#include <iostream>
#include <fstream>
#include <string>

// Reads a 3x3 matrix from standard input and appends it and its transpose to a file.
void appendMatrixAndTranspose(const std::string& filename) {
    int matrix[3][3];
    
    // Read input row-wise
    std::cout << "Enter the elements row-wise \n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cin >> matrix[i][j];
        }
    }
    
    // Open output file in append mode
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile) {
        std::cerr << "Error: could not open file " << filename << "\n";
        return;
    }
    
    // Write original matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            outFile << matrix[i][j] << "\t";
        }
        outFile << "\n\n";
    }
    
    // Write transpose header
    outFile << "Transpose is \n\n";
    
    // Write transpose (iterating columns first)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            outFile << matrix[j][i] << "\t";
        }
        outFile << "\n\n";
    }
    
    // outFile closes automatically when it goes out of scope
}

// The main algorithm is straightforward: allocate a 3x3 integer array, read 9 integers in row-major order from `std::cin` using a nested loop, then open an output file stream in append mode. After that, write the original matrix to the file, each element followed by a tab, and after each row, write two newlines. Then write "Transpose is" followed by two newlines. For the transpose, iterate over columns first and rows second (i.e., `a[j][i]`), writing each element with a tab and after each row two newlines. The file stream is closed automatically when the function returns. Edge cases: ensure the file opens successfully; if not, print an error message and return. The complexity is O(9) time and O(9) auxiliary space for the matrix.

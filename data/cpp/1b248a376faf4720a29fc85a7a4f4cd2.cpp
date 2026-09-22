Write a C++ function named `transposeAndDisplay` that takes a dynamically allocated 2D integer array represented by `int**`, along with its row count `rows` and column count `cols`, and returns a new dynamically allocated 2D array (as `int**`) containing the transpose of the input matrix. The transpose of an `rows × cols` matrix is a `cols × rows` matrix where the element at position `[i][j]` in the output equals the element at `[j][i]` in the input. The function must allocate memory for the result using `new`, fill it correctly, and also print the original matrix in the same format as the given code snippet (each row on one line with a space between elements, then a newline). Handle the edge case where either dimension is zero by returning a `nullptr` and printing nothing. The function must be `const`-correct: the input matrix data should be treated as read-only, and the input pointer parameters should be `const`-qualified appropriately. Do not include any `main` function; only provide the free function.
The solution requires two main steps: first, print the original matrix row by row exactly as shown in the snippet (space-separated integers, newline per row). Then, allocate a new 2D array with dimensions swapped (`cols` rows and `rows` columns). Iterate over the input indices such that for each output position `[i][j]`, we assign `result[i][j] = input[j][i]`. Edge case: if either `rows == 0` or `cols == 0`, the matrix has no elements; return `nullptr` without printing (since output nothing). For non-zero dimensions, we assume valid pointers to allocated arrays (no need to validate memory). Time complexity is O(rows × cols) for both printing and transposing, space complexity is O(rows × cols) for the new array. The transpose itself does not modify the input. Note: memory for the result must be freed by the caller, but that is outside this function’s responsibility.
#include <iostream>

// Prints the matrix row by row and returns its transpose as a newly allocated 2D array.
int** transposeAndDisplay(const int* const* matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) {
        return nullptr;
    }

    // Print original matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << (j < cols - 1 ? " " : "");
        }
        std::cout << std::endl;
    }

    // Allocate transpose: cols rows, rows columns
    int** transposed = nullptr;
    try {
        transposed = new int*[cols];
        for (int i = 0; i < cols; ++i) {
            transposed[i] = new int[rows];
        }
    } catch (const std::bad_alloc&) {
        // Cleanup if allocation fails
        if (transposed) {
            for (int i = 0; i < cols; ++i) {
                delete[] transposed[i];
            }
            delete[] transposed;
        }
        throw;
    }

    // Fill transpose
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            transposed[i][j] = matrix[j][i];
        }
    }

    return transposed;
}
#include <cassert>
#include <iostream>

// Declare the solution function (assuming it's defined above or in another header)
int** transposeAndDisplay(const int* const* matrix, int rows, int cols);

int main() {
    // Test 1: Basic 2x3 matrix
    int row1[] = {1, 2, 3};
    int row2[] = {4, 5, 6};
    int* matrix1[] = {row1, row2};
    int** t1 = transposeAndDisplay(matrix1, 2, 3);
    assert(t1 != nullptr);
    assert(t1[0][0] == 1 && t1[0][1] == 4);
    assert(t1[1][0] == 2 && t1[1][1] == 5);
    assert(t1[2][0] == 3 && t1[2][1] == 6);
    for (int i = 0; i < 3; ++i) delete[] t1[i];
    delete[] t1;

    // Test 2: Single element
    int single[] = {7};
    int* matrix2[] = {single};
    int** t2 = transposeAndDisplay(matrix2, 1, 1);
    assert(t2 != nullptr);
    assert(t2[0][0] == 7);
    delete[] t2[0];
    delete[] t2;

    // Test 3: Zero row (should return nullptr)
    int** t3 = transposeAndDisplay(nullptr, 0, 3);
    assert(t3 == nullptr);

    // Test 4: Square matrix 3x3
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    int a3[] = {7, 8, 9};
    int* matrix4[] = {a1, a2, a3};
    int** t4 = transposeAndDisplay(matrix4, 3, 3);
    assert(t4[1][2] == 8);
    assert(t4[2][0] == 3);
    for (int i = 0; i < 3; ++i) delete[] t4[i];
    delete[] t4;

    std::cout << "All tests passed." << std::endl;
    return 0;
}

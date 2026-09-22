Write a C++ function that takes two positive integers `rows` and `cols` and reads a `rows`×`cols` matrix of integers from standard input, then returns the sum of all elements in the matrix. The function must allocate the matrix dynamically using a 2D array of pointers (`int**`), read the values row by row, compute the sum, and finally deallocate all memory properly to avoid leaks. The function signature should be `int matrixSum(int rows, int cols)`, and it should assume the input matrix contains exactly `rows * cols` integers.
// The solution involves allocating a 2D dynamic array: first allocate an array of `int*` of size `rows`, then for each row allocate an array of `int` of size `cols`. Read each element sequentially using nested loops, accumulating the total sum in a local variable. After reading and summing, deallocate memory in reverse order: delete each row array, then delete the top‑level pointer array, and set pointers to `nullptr` to avoid dangling pointers. Edge cases: if `rows` or `cols` is zero, the sum should be 0; ensure the input stream is well‑formed (the problem guarantees valid input). Time complexity is O(rows × cols) for reading and summing, and space complexity is O(rows × cols) for the matrix itself (plus O(rows) for the pointer array). The solution must handle dynamic allocation failure gracefully (using `new` will throw `std::bad_alloc`, which is acceptable for this exercise).
#include <iostream>

// Reads a rows x cols matrix from standard input and returns the sum of all elements.
int matrixSum(int rows, int cols) {
    if (rows == 0 || cols == 0) {
        return 0;
    }

    // Allocate the row pointer array.
    int** matrix = new int*[rows];

    // Allocate each row and read its elements.
    int sum = 0;
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
        for (int j = 0; j < cols; ++j) {
            std::cin >> matrix[i][j];
            sum += matrix[i][j];
        }
    }

    // Deallocate each row.
    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
        matrix[i] = nullptr;
    }

    // Deallocate the top-level pointer array.
    delete[] matrix;
    matrix = nullptr;

    return sum;
}
#include <cassert>
#include <iostream>
#include <sstream>

// Forward declaration of the solution function.
int matrixSum(int rows, int cols);

int main() {
    // Test 1: 2x3 matrix
    {
        std::istringstream input("1 2 3\n4 5 6\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(2, 3) == 21);
    }

    // Test 2: 1x1 matrix
    {
        std::istringstream input("42\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(1, 1) == 42);
    }

    // Test 3: 3x2 matrix with negative numbers
    {
        std::istringstream input("-1 -2\n3 4\n-5 -6\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(3, 2) == -7);
    }

    // Test 4: zero rows
    {
        std::istringstream input("");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(0, 5) == 0);
    }

    // Test 5: zero cols
    {
        std::istringstream input("");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(5, 0) == 0);
    }

    // Test 6: 4x1 matrix
    {
        std::istringstream input("10\n20\n30\n40\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(4, 1) == 100);
    }

    // Test 7: 2x2 matrix with zeros
    {
        std::istringstream input("0 0\n0 0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(2, 2) == 0);
    }

    // Test 8: large values (within int range)
    {
        std::istringstream input("1000000 2000000\n3000000 4000000\n");
        std::cin.rdbuf(input.rdbuf());
        assert(matrixSum(2, 2) == 10000000);
    }

    std::cout << "All tests passed!" << std::endl;

    // Restore standard input (optional, but good practice)
    std::cin.clear();
    return 0;
}

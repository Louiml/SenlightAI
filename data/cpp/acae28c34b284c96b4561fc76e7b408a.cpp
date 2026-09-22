Write a C++ function that dynamically allocates a 2D array of doubles with dimensions provided by the user (both rows and columns must be between 1 and 3 inclusive), fills it with user input, and returns the sum of all elements in the array. The function should handle invalid dimensions gracefully by returning 0.0 and printing an error message. The input and output should be handled via standard input/output streams, and the function must properly deallocate all dynamically allocated memory before returning. The function signature should be `double sum2DArray(int rows, int cols)`.
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the solution function (should be defined above in actual code)
double sum2DArray(int rows, int cols);

int main() {
    // Test 1: Valid 1x1
    {
        std::istringstream input("5.5\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        double result = sum2DArray(1, 1);
        std::cin.rdbuf(oldCin);
        assert(result == 5.5);
    }

    // Test 2: Valid 2x3
    {
        std::istringstream input("1 2 3 4 5 6\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        double result = sum2DArray(2, 3);
        std::cin.rdbuf(oldCin);
        assert(result == 21.0);
    }

    // Test 3: Invalid dimensions (rows=0) returns 0.0
    {
        std::ostringstream output;
        std::streambuf* oldCerr = std::cerr.rdbuf(output.rdbuf());
        double result = sum2DArray(0, 2);
        std::cerr.rdbuf(oldCerr);
        assert(result == 0.0);
    }

    // Test 4: Invalid dimensions (cols=4) returns 0.0
    {
        std::ostringstream output;
        std::streambuf* oldCerr = std::cerr.rdbuf(output.rdbuf());
        double result = sum2DArray(2, 4);
        std::cerr.rdbuf(oldCerr);
        assert(result == 0.0);
    }

    // Test 5: Valid 3x3 with negative values
    {
        std::istringstream input("-1 -2 -3 -4 -5 -6 -7 -8 -9\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        double result = sum2DArray(3, 3);
        std::cin.rdbuf(oldCin);
        assert(result == -45.0);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>

// Dynamically allocates a 2D array, reads user input for each element,
// returns the sum of all elements, and deallocates memory.
double sum2DArray(int rows, int cols) {
    if (rows < 1 || rows > 3 || cols < 1 || cols > 3) {
        std::cerr << "Error: Dimensions must be between 1 and 3 inclusive." << std::endl;
        return 0.0;
    }

    // Allocate 2D array dynamically
    double** arr = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        arr[i] = new double[cols];
    }

    double total = 0.0;

    // Read elements and accumulate sum
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << "Enter value for row " << i + 1 << ", column " << j + 1 << ": ";
            std::cin >> arr[i][j];
            total += arr[i][j];
        }
    }

    // Deallocate memory
    for (int i = 0; i < rows; ++i) {
        delete[] arr[i];
    }
    delete[] arr;

    return total;
}
// The solution uses dynamic memory allocation with `new double*[rows]` and then allocates each row as `new double[cols]`. It validates that `rows` and `cols` are within the allowed range of 1 to 3 inclusive; if not, it prints an error to `std::cerr` and returns `0.0`. For valid dimensions, it prompts the user for each element, storing it in the 2D array while accumulating a running total. After reading all elements, it deallocates the memory using `delete[]` for each row then `delete[]` for the array of pointers. The function returns the accumulated sum. Edge cases include: zero or negative dimensions (rejected), dimensions larger than 3 (rejected), and the potential for memory leak if allocation fails midway (though we assume standard behavior). Since dimensions are small (max 9 elements), no special exception handling is needed. Time complexity is O(rows × cols) and space complexity is O(rows × cols) for the dynamic allocation, plus O(1) auxiliary space for the sum.

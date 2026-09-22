// Write a C++ function that reads a 3×3 matrix of floating-point values from standard input and returns the sum of only the diagonal entries (the trace: A[0][0] + A[1][1] + A[2][2]). The input is given as nine whitespace-separated doubles (row-major order). The function must not print anything; it should just consume the nine values and return the computed trace. Assume the input is always valid and contains exactly nine numbers.
The solution reads all nine values using a nested loop or a simple loop over 9 elements, storing them into a local 3×3 array (or directly accumulating the diagonal values). The main algorithm is straightforward: initialize `sum = 0.0`, then for each index `i` from 0 to 2, read the full row, but only add `A[i][i]` to the sum. An alternative is to read all nine values into a flat array and then add the entries at positions 0, 4, and 8. Edge cases: since the input is guaranteed to have exactly nine numbers, no validation is needed; duplicates or negative values are fine. Floating-point comparisons for the trace are done directly. Time complexity is O(1) (constant, exactly 9 reads and 3 additions). Space complexity is O(1) because we only store a fixed-size 3×3 array or a few scalar variables. Const correctness applies to the internal array (though it is not const here because it is being written) and the function itself takes no parameters and returns a double.
#include <iostream>

// Read a 3x3 matrix from standard input and return the trace (sum of diagonal elements).
double readMatrixTrace() {
    double matrix[3][3];
    double sum = 0.0;
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cin >> matrix[i][j];
            if (i == j) {
                sum += matrix[i][j];  // Only add diagonal entries
            }
        }
    }
    
    return sum;
}
#include <cassert>
#include <iostream>
#include <sstream>

// Declare the solution function
double readMatrixTrace();

int main() {
    // Helper to simulate standard input with a string.
    auto testWithInput = [](const std::string& input, double expected) {
        std::istringstream iss(input);
        std::cin.rdbuf(iss.rdbuf());
        double result = readMatrixTrace();
        assert(result == expected);
    };

    // Diagonal: 1+5+9 = 15
    testWithInput("1 2 3 4 5 6 7 8 9", 15.0);
    
    // All zeros: trace = 0
    testWithInput("0 0 0 0 0 0 0 0 0", 0.0);
    
    // Only diagonal nonzero, others can be anything
    testWithInput("2 100 100 100 3 100 100 100 4", 9.0);
    
    // Negative diagonal values
    testWithInput("-1 1 1 1 -2 1 1 1 -3", -6.0);
    
    // Floating point values
    testWithInput("0.5 1 1 1 1.5 1 1 1 2.5", 4.5);
    
    // Random matrix: diagonal entries are 7, 8, 9 -> sum 24
    testWithInput("7 1 2 3 8 4 5 6 9", 24.0);
    
    // All same values
    testWithInput("3 3 3 3 3 3 3 3 3", 9.0);
    
    // Large numbers
    testWithInput("1000 1 1 1 2000 1 1 1 3000", 6000.0);
    
    // Mixed signs
    testWithInput("-5 2 2 2 0 2 2 2 5", 0.0);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

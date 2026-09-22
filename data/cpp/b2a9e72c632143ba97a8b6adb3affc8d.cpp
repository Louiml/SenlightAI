Write a C++ function called `multiplyMatricesElementwise` that accepts two 3x3 matrices of integers (passed as 2D arrays with fixed dimensions) and returns a new 3x3 matrix where each element at position (i, j) is the product of the corresponding elements from the two input matrices (i.e., `result[i][j] = matrix1[i][j] * matrix2[i][j]`). The function must be free-standing (no `main` function inside), must not modify the input matrices, must use `const` correctness for inputs, and must work with matrices that contain any integer values (including negative numbers, zeros, and large positive/negative values). Since the matrices are fixed at 3x3, no dynamic memory allocation is needed; the returned matrix can be a `std::array` of `std::array` or a plain 2D array returned via a struct or `std::array`. For simplicity and safety, use `std::array<std::array<int, 3>, 3>` as the container for both inputs and the output. The function signature should be: `std::array<std::array<int, 3>, 3> multiplyMatricesElementwise(const std::array<std::array<int, 3>, 3>& matrix1, const std::array<std::array<int, 3>, 3>& matrix2);` The task is to implement this function correctly and efficiently, handling all integer values without overflow concerns (the input values are assumed to be small enough to fit in `int` when multiplied, but the code should not perform any unnecessary checks).

#include <cassert>

int main() {
    // Test 1: Basic positive multiplication
    std::array<std::array<int, 3>, 3> m1 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    std::array<std::array<int, 3>, 3> m2 = {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}};
    auto result = multiplyMatricesElementwise(m1, m2);
    assert(result == m1);

    // Test 2: Multiplying by zeros yields zeros
    std::array<std::array<int, 3>, 3> m3 = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    result = multiplyMatricesElementwise(m1, m3);
    assert(result == m3);

    // Test 3: Negative numbers
    std::array<std::array<int, 3>, 3> m4 = {{{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}}};
    std::array<std::array<int, 3>, 3> m5 = {{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}};
    result = multiplyMatricesElementwise(m4, m5);
    assert(result == m4);

    // Test 4: Element-wise multiplication with distinct values
    std::array<std::array<int, 3>, 3> m6 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    std::array<std::array<int, 3>, 3> m7 = {{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}}};
    std::array<std::array<int, 3>, 3> expected = {{{9, 16, 21}, {24, 25, 24}, {21, 16, 9}}};
    result = multiplyMatricesElementwise(m6, m7);
    assert(result == expected);

    // Test 5: Both matrices identical (squares)
    std::array<std::array<int, 3>, 3> m8 = {{{2, 3, 4}, {5, 6, 7}, {8, 9, 10}}};
    std::array<std::array<int, 3>, 3> expectedSquares = {{{4, 9, 16}, {25, 36, 49}, {64, 81, 100}}};
    result = multiplyMatricesElementwise(m8, m8);
    assert(result == expectedSquares);

    // Test 6: Mixed signs
    std::array<std::array<int, 3>, 3> m9 = {{{-1, 2, -3}, {4, -5, 6}, {-7, 8, -9}}};
    std::array<std::array<int, 3>, 3> m10 = {{{1, -2, 3}, {-4, 5, -6}, {7, -8, 9}}};
    std::array<std::array<int, 3>, 3> expectedMixed = {{{-1, -4, -9}, {-16, -25, -36}, {-49, -64, -81}}};
    result = multiplyMatricesElementwise(m9, m10);
    assert(result == expectedMixed);

    // Test 7: All ones * all twos
    std::array<std::array<int, 3>, 3> ones = {{{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}}};
    std::array<std::array<int, 3>, 3> twos = {{{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}}};
    std::array<std::array<int, 3>, 3> expectedTwos = {{{{2, 2, 2}, {2, 2, 2}, {2, 2, 2}}}};
    result = multiplyMatricesElementwise(ones, twos);
    assert(result == expectedTwos);

    // Test 8: Large values (still within int range)
    std::array<std::array<int, 3>, 3> large1 = {{{{1000, 2000, 3000}, {4000, 5000, 6000}, {7000, 8000, 9000}}}};
    std::array<std::array<int, 3>, 3> large2 = {{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}}};
    std::array<std::array<int, 3>, 3> expectedLarge = {{{{1000, 4000, 9000}, {16000, 25000, 36000}, {49000, 64000, 81000}}}};
    result = multiplyMatricesElementwise(large1, large2);
    assert(result == expectedLarge);

    return 0;
}

#include <array>

// Multiply two 3x3 matrices element-wise (Hadamard product).
// Returns a new matrix where result[i][j] = matrix1[i][j] * matrix2[i][j].
std::array<std::array<int, 3>, 3> multiplyMatricesElementwise(
    const std::array<std::array<int, 3>, 3>& matrix1,
    const std::array<std::array<int, 3>, 3>& matrix2) {
    
    std::array<std::array<int, 3>, 3> result{};
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }
    
    return result;
}

// The solution uses a straightforward double nested loop that iterates over all 3 rows and 3 columns. For each cell (i, j), the result is computed as `matrix1[i][j] * matrix2[i][j]` directly. The input matrices are passed as `const` references to guarantee they are not modified and to avoid unnecessary copying. The returned matrix is constructed and filled in the same loop; after the loops complete, the result is returned by value (which is efficient because `std::array` is a lightweight aggregate). Edge cases include matrices with negative numbers, zeros, or a mix of signs; the multiplication of two integers works correctly in all cases as long as no overflow occurs (the problem assumes inputs are chosen to avoid overflow). The time complexity is O(9) = O(1) because the matrix dimensions are fixed at 3x3, and the space complexity is O(1) as only a fixed-size result matrix is allocated on the stack. No special handling is needed for empty matrices because the size is fixed at 3x3.

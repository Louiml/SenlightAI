// Write a C++ function that takes a fixed 3x3 matrix of integers (e.g., using a `std::array<std::array<int,3>,3>` or a custom struct) and returns a `std::array<int,3>` containing the sum of each row, from top to bottom. The function should handle negative numbers and zeros correctly, and must not modify the input matrix. The returned array's elements must be in order: first row sum, second row sum, third row sum. You may assume the input matrix always has exactly 3 rows and 3 columns.
#include <cassert>

int main() {
    // Test 1: Simple positive numbers
    std::array<std::array<int,3>,3> m1 = {{{1,2,3},{4,5,6},{7,8,9}}};
    auto r1 = rowSums(m1);
    assert(r1[0] == 6 && r1[1] == 15 && r1[2] == 24);

    // Test 2: Negative numbers
    std::array<std::array<int,3>,3> m2 = {{{-1,-2,-3},{-4,-5,-6},{-7,-8,-9}}};
    auto r2 = rowSums(m2);
    assert(r2[0] == -6 && r2[1] == -15 && r2[2] == -24);

    // Test 3: Mixed positives and negatives with zeros
    std::array<std::array<int,3>,3> m3 = {{{1,-1,0},{2,0,-2},{3,-3,0}}};
    auto r3 = rowSums(m3);
    assert(r3[0] == 0 && r3[1] == 0 && r3[2] == 0);

    // Test 4: All zeros
    std::array<std::array<int,3>,3> m4 = {{{0,0,0},{0,0,0},{0,0,0}}};
    auto r4 = rowSums(m4);
    assert(r4[0] == 0 && r4[1] == 0 && r4[2] == 0);

    // Test 5: Check that input matrix is not modified
    std::array<std::array<int,3>,3> m5 = {{{5,6,7},{8,9,10},{11,12,13}}};
    auto r5 = rowSums(m5);
    assert(m5[0][0] == 5 && m5[2][2] == 13); // original still intact
    assert(r5[0] == 18 && r5[1] == 27 && r5[2] == 36);

    // Test 6: Duplicate equal values in a row
    std::array<std::array<int,3>,3> m6 = {{{7,7,7},{0,0,0},{-3,-3,-3}}};
    auto r6 = rowSums(m6);
    assert(r6[0] == 21 && r6[1] == 0 && r6[2] == -9);

    return 0;
}
#include <array>

// Returns a std::array<int,3> where element i is the sum of row i of the input matrix.
// The input matrix is taken by const reference to avoid copying and guarantee no modification.
std::array<int, 3> rowSums(const std::array<std::array<int, 3>, 3>& matrix) {
    std::array<int, 3> result = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        // Sum all three elements of the current row
        result[i] = matrix[i][0] + matrix[i][1] + matrix[i][2];
    }
    return result;
}
// The solution involves iterating over each row of the matrix, accumulating the sum of its three elements, and storing that sum in the corresponding position of the output array. Since the matrix size is fixed at 3x3, the time complexity is O(9) = O(1) and space complexity is O(1) for the output (excluding the input storage). No edge cases are problematic because all integers are valid, and the input is guaranteed to be 3x3. The main consideration is to use `const` references to avoid copying the matrix and to ensure the input is not modified. The algorithm is straightforward: for each row index `i` from 0 to 2, compute `matrix[i][0] + matrix[i][1] + matrix[i][2]` and assign to `result[i]`. This works with negative numbers, zeros, and duplicate values. Time complexity: O(1) because the size is constant. Space complexity: O(1) for the result array beyond input storage.

Write a C++ function that takes a 2D integer array (as a 1D flattened representation) along with its row and column counts, and a target value `k`. The function should return a boolean indicating whether `k` exists in the array. The array will be provided as a plain array of integers with dimensions `row × col`, where both `row` and `col` are positive integers. The function must handle arrays containing negative numbers and duplicate values. The search should be a simple linear traversal of all elements. The function signature should be `bool findInMatrix(const int* matrix, int row, int col, int k)` and it must be `const`-correct (the matrix pointer must be `const int*`).

The solution is straightforward: iterate over every element in the flattened array, which represents the matrix in row-major order. For each element, compare it to the target value `k`. If a match is found, return `true` immediately; otherwise, continue until all elements are checked. If no match is found after the loop, return `false`. Edge cases include: an empty row or col (though the task says positive, we can defensively return false), negative numbers, duplicates, and `k` being equal to the first or last element. Time complexity is O(row × col) since we visit each element once. Space complexity is O(1) — no extra storage beyond a loop index is used. `const` correctness ensures the function does not modify the matrix.

#include <cstddef> // for size_t if needed, but we use int parameters

// Searches for value k in a row x col matrix stored in row-major order.
// Returns true if k is found, false otherwise.
bool findInMatrix(const int* matrix, int row, int col, int k) {
    if (matrix == nullptr || row <= 0 || col <= 0) {
        return false;
    }
    int totalElements = row * col;
    for (int i = 0; i < totalElements; ++i) {
        if (matrix[i] == k) {
            return true;
        }
    }
    return false;
}

#include <cassert>

// Declare the function under test (assume it is defined above).
bool findInMatrix(const int* matrix, int row, int col, int k);

int main() {
    // Test 1: 2x3 matrix with target present
    int m1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    assert(findInMatrix(&m1[0][0], 2, 3, 5) == true);

    // Test 2: target not present
    assert(findInMatrix(&m1[0][0], 2, 3, 10) == false);

    // Test 3: single element matrix, target present
    int m2[1][1] = {{7}};
    assert(findInMatrix(&m2[0][0], 1, 1, 7) == true);

    // Test 4: single element matrix, target not present
    assert(findInMatrix(&m2[0][0], 1, 1, 3) == false);

    // Test 5: matrix with negative numbers
    int m3[2][2] = {{-1, -2}, {-3, -4}};
    assert(findInMatrix(&m3[0][0], 2, 2, -3) == true);

    // Test 6: duplicate values, target found
    int m4[1][3] = {{5, 5, 5}};
    assert(findInMatrix(&m4[0][0], 1, 3, 5) == true);

    // Test 7: row or col zero (defensive), should return false
    int m5[1][2] = {{1, 2}};
    assert(findInMatrix(&m5[0][0], 0, 2, 1) == false);

    // Test 8: target equals first element
    assert(findInMatrix(&m1[0][0], 2, 3, 1) == true);

    // Test 9: target equals last element
    assert(findInMatrix(&m1[0][0], 2, 3, 6) == true);

    // Test 10: larger matrix, target in middle
    int m6[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    assert(findInMatrix(&m6[0][0], 3, 3, 2) == true);
    assert(findInMatrix(&m6[0][0], 3, 3, 0) == false);

    return 0;
}

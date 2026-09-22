/*
Write a C++ function that accepts a fixed-size 3×3 matrix of integers (represented as a 3×3 `std::array<std::array<int,3>,3>` or a simple struct) and returns a `std::array<int,3>` containing the minimum value of each column, in left-to-right column order. The input matrix is guaranteed to contain only finite integers (no special values like NaN or infinity). The function must be `const`-correct and work with a matrix passed by const reference. Your solution must not rely on any external linear algebra library; implement the logic manually.
*/
#include <array>
#include <algorithm>

// Given a 3x3 matrix of integers, return an array containing the minimum
// value of each column, in order from column 0 to column 2.
std::array<int, 3> columnMinimums(const std::array<std::array<int, 3>, 3>& matrix) {
    std::array<int, 3> result{};

    for (int col = 0; col < 3; ++col) {
        int minVal = matrix[0][col];
        for (int row = 1; row < 3; ++row) {
            minVal = std::min(minVal, matrix[row][col]);
        }
        result[col] = minVal;
    }

    return result;
}
#include <cassert>
#include <array>

// Assuming columnMinimums is declared above.

int main() {
    std::array<std::array<int, 3>, 3> m1 = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    std::array<int, 3> r1 = columnMinimums(m1);
    assert((r1 == std::array<int, 3>{1, 2, 3}));

    std::array<std::array<int, 3>, 3> m2 = {{{-5, 0, 10}, {-3, -1, 7}, {2, 4, -8}}};
    std::array<int, 3> r2 = columnMinimums(m2);
    assert((r2 == std::array<int, 3>{-5, -1, -8}));

    std::array<std::array<int, 3>, 3> m3 = {{{7, 7, 7}, {7, 7, 7}, {7, 7, 7}}};
    std::array<int, 3> r3 = columnMinimums(m3);
    assert((r3 == std::array<int, 3>{7, 7, 7}));

    std::array<std::array<int, 3>, 3> m4 = {{{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}}};
    std::array<int, 3> r4 = columnMinimums(m4);
    assert((r4 == std::array<int, 3>{-7, -8, -9}));

    std::array<std::array<int, 3>, 3> m5 = {{{10, 20, 30}, {1, 2, 3}, {100, 200, 300}}};
    std::array<int, 3> r5 = columnMinimums(m5);
    assert((r5 == std::array<int, 3>{1, 2, 3}));

    return 0;
}
// The problem reduces to iterating over each column index `c` from 0 to 2. For each column, initialize a running minimum to the element in row 0 of that column (`matrix[0][c]`). Then, iterate rows `r` from 1 to 2, updating the minimum if the current element is smaller. After processing all rows for a given column, store the resulting minimum in the output array at position `c`. Because the matrix is fixed at 3×3, the algorithm always visits exactly 9 elements. Time complexity is O(3×3)=O(1) constant time, and auxiliary space is O(3) for the output array, also constant. Edge cases: all elements equal – each column minimum is that common value; negative numbers are handled naturally by `<` comparison; no need to worry about empty matrices because size is fixed. The solution uses a `const&` parameter, and the output is returned by value for simplicity.

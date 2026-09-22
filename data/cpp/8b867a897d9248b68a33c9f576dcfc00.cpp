Write a standalone C++ function that takes a 2D matrix of integers represented as a `std::vector<std::vector<int>>` and returns a new matrix (also as `std::vector<std::vector<int>>`) that is the result of resizing the input matrix to have exactly `R` rows and `C` columns, where `R * C` equals the total number of elements in the original matrix. The elements must be reordered in **row-major order**: take the original matrix’s elements row by row, flatten them into a single sequence, then fill the new matrix row by row with that sequence. The function should accept the input matrix by const reference, and the new dimensions as parameters. You may assume the input is non-empty and `R * C` equals the total element count. The function must return the resized matrix by value.
The main algorithm is straightforward: flatten the input matrix into a temporary 1D vector by iterating over each row and each element, pushing them into the vector. Then create a result 2D vector with `R` rows and `C` columns, and fill it by iterating over the flattened vector in order, assigning each element to the correct row and column index (row = index / C, column = index % C). Since we guarantee `R * C` equals the total number of elements, the index will always be valid. Edge cases: the input could be a single row or single column; the flattened order still works. If `R` or `C` is 0? The problem implies positive dimensions, but we can handle 0 by returning an empty matrix if total elements is 0, but since input is non-empty and R*C equals total, R and C are positive. Time complexity is O(N) where N is the total number of elements, because we traverse each element twice (once to flatten, once to fill). Space complexity is O(N) for the temporary vector and O(N) for the result, but since the result is the required output, the extra temporary is O(N) auxiliary space. We can optimize by avoiding the temporary vector if we directly compute the row and column from the source element index, but the simple approach is clear.
#include <vector>
#include <cstddef>

// Resize a 2D matrix to new dimensions R x C preserving row-major order.
// Precondition: R * C equals the total number of elements in the input matrix.
std::vector<std::vector<int>> reshapeMatrix(const std::vector<std::vector<int>>& input, int R, int C) {
    // Flatten input into a single sequence.
    std::vector<int> flat;
    for (const auto& row : input) {
        for (int val : row) {
            flat.push_back(val);
        }
    }

    // Build the new matrix with R rows and C columns.
    std::vector<std::vector<int>> result(R, std::vector<int>(C));
    int idx = 0;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            result[i][j] = flat[idx++];
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above (or included from the solution).
// Assume it is available here.

int main() {
    // Test 1: 2x2 -> 4x1
    std::vector<std::vector<int>> m1 = {{1,2},{3,4}};
    auto r1 = reshapeMatrix(m1, 4, 1);
    assert(r1.size() == 4);
    assert(r1[0][0] == 1 && r1[1][0] == 2 && r1[2][0] == 3 && r1[3][0] == 4);

    // Test 2: 2x2 -> 1x4
    auto r2 = reshapeMatrix(m1, 1, 4);
    assert(r2.size() == 1);
    assert(r2[0] == std::vector<int>({1,2,3,4}));

    // Test 3: 3x1 -> 1x3
    std::vector<std::vector<int>> m3 = {{7},{8},{9}};
    auto r3 = reshapeMatrix(m3, 1, 3);
    assert(r3.size() == 1);
    assert(r3[0] == std::vector<int>({7,8,9}));

    // Test 4: 1x3 -> 3x1
    std::vector<std::vector<int>> m4 = {{10,20,30}};
    auto r4 = reshapeMatrix(m4, 3, 1);
    assert(r4.size() == 3);
    assert(r4[0][0] == 10 && r4[1][0] == 20 && r4[2][0] == 30);

    // Test 5: 2x3 -> 3x2
    std::vector<std::vector<int>> m5 = {{1,2,3},{4,5,6}};
    auto r5 = reshapeMatrix(m5, 3, 2);
    assert(r5.size() == 3);
    assert(r5[0] == std::vector<int>({1,2}));
    assert(r5[1] == std::vector<int>({3,4}));
    assert(r5[2] == std::vector<int>({5,6}));

    // Test 6: 1x1 -> 1x1
    std::vector<std::vector<int>> m6 = {{42}};
    auto r6 = reshapeMatrix(m6, 1, 1);
    assert(r6.size() == 1 && r6[0][0] == 42);

    return 0;
}

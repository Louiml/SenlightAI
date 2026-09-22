// Write a C++ function named `flipAndInvertImage` that accepts a 2D square vector of integers (only 0s and 1s) representing a binary matrix, and returns a new 2D vector where each row is first reversed horizontally (left-to-right flipped), and then every bit in the reversed row is inverted (0 becomes 1, 1 becomes 0). The input matrix must be non-empty, square (rows == columns), and contain only 0s or 1s. The function should not modify the input matrix — it must return a new matrix. Assume the input is always valid, so no error handling for invalid dimensions or values is required.
The solution processes the matrix row by row. For each row, we need to reverse the order of elements and invert each bit. Instead of doing two separate passes (first reverse, then invert or vice versa), we can combine both operations in a single pass over each row: for each index `i` from 0 to `n` (where `n` is the number of columns), we take the value at the symmetric position `n - 1 - i` from the original row, invert it, and place it at index `i` in the new row. This in-place logical construction works because we are building a new row vector from the original row without modifying the original. We must be careful to copy all elements exactly once; since we iterate `i` from 0 to `n-1`, every original element is visited exactly once through its mirrored index. Edge cases: when the matrix has a single row or single column, the logic still works because reversing a single element yields itself, and inverting flips its value. The time complexity is O(n * m) where n is number of rows and m is number of columns (and since it's square, O(N^2) for N×N matrix), and space complexity is O(N^2) to store the output matrix (or O(1) extra space if we consider rebuilding rows one at a time, but we allocate the full result). The main algorithm is straightforward: for each row, create a new row vector of the same size, loop through indices, fetch original value at `n-1-i`, invert with `value == 0 ? 1 : 0` or `1 - value`, and store.
#include <vector>

// Given a square binary matrix, return a new matrix where each row is
// reversed and every bit is inverted. The input is unchanged.
std::vector<std::vector<int>> flipAndInvertImage(const std::vector<std::vector<int>>& input) {
    const int rows = input.size();
    const int cols = input[0].size();

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // Original value at the mirrored column, then invert (0->1, 1->0)
            int original = input[r][cols - 1 - c];
            result[r][c] = original == 0 ? 1 : 0;
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Prototype of the solution function (for testing).
std::vector<std::vector<int>> flipAndInvertImage(const std::vector<std::vector<int>>& input);

int main() {
    // Single element matrix
    assert(flipAndInvertImage({{0}}) == std::vector<std::vector<int>>({{1}}));
    assert(flipAndInvertImage({{1}}) == std::vector<std::vector<int>>({{0}}));

    // 1x2 matrix (row reversal and inversion)
    assert(flipAndInvertImage({{0, 1}}) == std::vector<std::vector<int>>({{0, 1}})); // reverse -> [1,0], invert -> [0,1]
    assert(flipAndInvertImage({{1, 0}}) == std::vector<std::vector<int>>({{1, 0}})); // reverse -> [0,1], invert -> [1,0]

    // 2x2 matrix
    std::vector<std::vector<int>> input1 = {{1, 1}, {0, 0}};
    std::vector<std::vector<int>> expected1 = {{0, 0}, {1, 1}};
    assert(flipAndInvertImage(input1) == expected1);

    std::vector<std::vector<int>> input2 = {{1, 0}, {0, 1}};
    std::vector<std::vector<int>> expected2 = {{0, 1}, {1, 0}};
    assert(flipAndInvertImage(input2) == expected2);

    // 3x3 matrix
    std::vector<std::vector<int>> input3 = {{1, 1, 0}, {0, 1, 0}, {0, 0, 1}};
    // Reverse each row: [0,1,1], [0,1,0], [1,0,0]
    // Invert each:       [1,0,0], [1,0,1], [0,1,1]
    std::vector<std::vector<int>> expected3 = {{1, 0, 0}, {1, 0, 1}, {0, 1, 1}};
    assert(flipAndInvertImage(input3) == expected3);

    // Ensure input is not modified
    std::vector<std::vector<int>> input4 = {{1, 0}, {1, 0}};
    std::vector<std::vector<int>> copy4 = input4;
    flipAndInvertImage(input4);
    assert(input4 == copy4);

    return 0;
}

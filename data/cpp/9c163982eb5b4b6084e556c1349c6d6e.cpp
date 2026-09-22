Write a C++ function `findEscapingPath` that determines whether a sequence of 9 integer moves starting from cell 13 in a 4x4 grid (numbered row-wise from 1 to 16) can reach cell 4, given that the path must pass through exactly 9 cells (including start and end) and the sum of the values of all 9 visited cells must equal 82. The function takes a `const std::array<int,9>&` representing the proposed path (in order of visitation) and returns a `bool` indicating whether the path is exactly `{13, 14, 15, 11, 10, 6, 7, 3, 4}`. The function must validate that the path is a legal sequence of adjacent moves (horizontally or vertically, with no diagonals), does not revisit any cell, stays within 1-16, has exactly 9 entries, and its sum is 82. Also verify that the first element is 13 and the last is 4.
#include <cassert>
#include <array>
#include <iostream>

int main() {
    // Correct path given in the problem statement
    std::array<int,9> correct = {13, 14, 15, 11, 10, 6, 7, 3, 4};
    assert(findEscapingPath(correct) == true);

    // Wrong start
    std::array<int,9> wrongStart = {12, 14, 15, 11, 10, 6, 7, 3, 4};
    assert(findEscapingPath(wrongStart) == false);

    // Wrong end
    std::array<int,9> wrongEnd = {13, 14, 15, 11, 10, 6, 7, 3, 5};
    assert(findEscapingPath(wrongEnd) == false);

    // Non-adjacent move (16 to 15 is adjacent? 16 row3 col3, 15 row3 col2, adjacent. Use 13 to 16: not adjacent)
    std::array<int,9> nonAdjacent = {13, 16, 15, 11, 10, 6, 7, 3, 4};
    assert(findEscapingPath(nonAdjacent) == false);

    // Duplicate cell (13 appears twice)
    std::array<int,9> duplicate = {13, 13, 15, 11, 10, 6, 7, 3, 4};
    assert(findEscapingPath(duplicate) == false);

    // Sum not 83 (replace 4 with 5, but then end not 4. Instead, replace 7 with 8, sum becomes 84)
    std::array<int,9> wrongSum = {13, 14, 15, 11, 10, 6, 8, 3, 4}; // sum = 13+14+15+11+10+6+8+3+4 = 84
    assert(findEscapingPath(wrongSum) == false);

    // Out of range cell (0)
    std::array<int,9> outOfRange = {13, 14, 15, 11, 10, 6, 7, 0, 4};
    assert(findEscapingPath(outOfRange) == false);

    // Valid path with sum 83 but different order? Actually the given path is unique. Test a shuffled version that breaks adjacency
    std::array<int,9> shuffled = {13, 15, 14, 11, 10, 6, 7, 3, 4};
    assert(findEscapingPath(shuffled) == false);

    // Test a valid path that is not the exact answer (e.g., a different legal path with sum 83? But only one such path exists? We can test a path that is legal and sum 83 but wrong end? Not needed)
    // At least ensure the function handles a 9-element path correctly.
    return 0;
}
#include <array>
#include <cmath>
#include <cstdlib>

// Checks if a proposed escape path from cell 13 to cell 4 in a 4x4 grid is valid.
// Valid conditions:
//  - Exactly 9 cells, starting at 13 and ending at 4.
//  - Each consecutive pair of cells must be adjacent (up/down/left/right, no diagonals).
//  - No cell is visited more than once.
//  - The sum of all visited cell numbers equals 82.
// Returns true if all conditions hold, false otherwise.
bool findEscapingPath(const std::array<int, 9>& path) {
    // Check start and end cells
    if (path[0] != 13 || path[8] != 4) {
        return false;
    }

    // Track visited cells (indices 1..16)
    bool visited[17] = {false};
    int sum = 0;

    for (int i = 0; i < 9; ++i) {
        int cell = path[i];
        // Check valid range
        if (cell < 1 || cell > 16) {
            return false;
        }
        // Check for duplicate visit
        if (visited[cell]) {
            return false;
        }
        visited[cell] = true;
        sum += cell;

        // For each consecutive pair (except the last element), verify adjacency
        if (i < 8) {
            int next = path[i+1];
            // Convert 1-based to 0-based row/col
            int row1 = (cell - 1) / 4;
            int col1 = (cell - 1) % 4;
            int row2 = (next - 1) / 4;
            int col2 = (next - 1) % 4;
            // Adjacency means Manhattan distance == 1
            if (std::abs(row1 - row2) + std::abs(col1 - col2) != 1) {
                return false;
            }
        }
    }

    // Check required sum
    return sum == 82;
}
// The solution approach: First, check that the array size is 9 (by definition of `std::array<int,9>` it is fixed, but we still check the first and last elements). Validate that `path[0]==13` and `path[8]==4`. Then verify that every consecutive pair `(path[i], path[i+1])` are adjacent on the 4x4 grid. Adjacency means the Manhattan distance is exactly 1: either row difference is 1 and column difference is 0, or row difference is 0 and column difference is 1. Convert cell number to (row, col) using zero‑based indices: `row = (cell-1)/4`, `col = (cell-1)%4`. Compute the absolute differences. Also ensure no duplicate cells appear by using a boolean visited array of size 17. Compute the sum of all 9 values and verify it equals 82. Edge cases: If any cell is out of 1..16 (though impossible given array input, but we still check), if a duplicate appears, if adjacency fails, or if sum is wrong, return false. Time complexity is O(n) where n=9 (constant), space O(1) for visited array (or O(16) but constant). Since input size is fixed, both are O(1) effectively.

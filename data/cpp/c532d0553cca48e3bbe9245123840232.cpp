Write a C++ function that takes a square matrix (represented as a `std::vector<std::vector<int>>`) and swaps the elements on its main diagonal with the corresponding elements on the anti-diagonal, in-place. For each row `i`, the element at `matrix[i][i]` (main diagonal) should be swapped with the element at `matrix[i][size-1-i]` (anti-diagonal). The function must handle square matrices of any size, including size 0 and size 1 (where no swap is needed for size 1 since both diagonals overlap at the center). The matrix must be modified directly; return `void`. The input is guaranteed to be a non-empty square matrix (rows == columns), but you should still check for consistency to be safe.

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Provide the function declaration for testing.
void swapDiagonals(std::vector<std::vector<int>>&);

int main() {
    // Test 1: 2x2 matrix
    {
        std::vector<std::vector<int>> m = {{1, 2}, {3, 4}};
        swapDiagonals(m);
        assert(m == std::vector<std::vector<int>>({{2, 1}, {4, 3}}));
    }
    
    // Test 2: 3x3 matrix
    {
        std::vector<std::vector<int>> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        swapDiagonals(m);
        // Expected: swap (0,0)<->(0,2), (1,1)<->(1,1) (no change), (2,0)<->(2,2)
        // Row 0: 3, 2, 1
        // Row 1: 4, 5, 6
        // Row 2: 9, 8, 7
        assert(m == std::vector<std::vector<int>>({{3, 2, 1}, {4, 5, 6}, {9, 8, 7}}));
    }
    
    // Test 3: 1x1 matrix (no change)
    {
        std::vector<std::vector<int>> m = {{42}};
        swapDiagonals(m);
        assert(m == std::vector<std::vector<int>>({{42}}));
    }
    
    // Test 4: Empty matrix (no change)
    {
        std::vector<std::vector<int>> m;
        swapDiagonals(m);
        assert(m.empty());
    }
    
    // Test 5: Non-square matrix (should do nothing)
    {
        std::vector<std::vector<int>> m = {{1, 2, 3}, {4, 5, 6}};
        swapDiagonals(m);
        assert(m == std::vector<std::vector<int>>({{1, 2, 3}, {4, 5, 6}}));
    }
    
    // Test 6: 4x4 matrix with negative values
    {
        std::vector<std::vector<int>> m = {{-1, 0, 1, 2}, {3, -4, 5, 6}, {7, 8, -9, 10}, {11, 12, 13, -14}};
        swapDiagonals(m);
        // Expected after swap:
        // Row 0: original (0,3)=2, (0,1)=0, (0,2)=1, (0,0)=-1 -> {2,0,1,-1}
        // Row 1: (1,1)=-4, (1,2)=5 -> swap -> (1,1)=5, (1,2)=-4 -> {3,5,-4,6}
        // Row 2: (2,0)=7, (2,2)=-9 -> swap? Actually (2,2) is main, (2,1)=8 is anti? Wait size=4, anti index for row 2 is 4-1-2=1, so swap (2,2) with (2,1): -9 with 8 -> {7,8,-9,10} becomes {7,-9,8,10}? Let's compute carefully.
        // Actually let's just compare with known result: We'll construct expected.
        std::vector<std::vector<int>> expected = {{2,0,1,-1}, {3,5,-4,6}, {7,-9,8,10}, {11,12,13,-14}};
        // Wait check row 3: anti index for row 3 is 0, so swap (3,3)=-14 with (3,0)=11 -> row becomes {11,12,13,-14}? No, swap (3,3) and (3,0): original row {11,12,13,-14} -> after swap: {-14,12,13,11}
        // Let's recalc all:
        // Row 0: swap (0,0)=-1 and (0,3)=2 -> {2,0,1,-1}
        // Row 1: swap (1,1)=-4 and (1,2)=5 -> {3,5,-4,6}
        // Row 2: swap (2,2)=-9 and (2,1)=8 -> {7,8,-9,10} becomes {7,-9,8,10}? Actually original row 2 is {7,8,-9,10}, swap index 1 and 2 -> {7,-9,8,10}
        // Row 3: swap (3,3)=-14 and (3,0)=11 -> {-14,12,13,11}
        // So expected matrix:
        std::vector<std::vector<int>> expected = {{2,0,1,-1}, {3,5,-4,6}, {7,-9,8,10}, {-14,12,13,11}};
        swapDiagonals(m);
        assert(m == expected);
    }
    
    return 0;
}

#include <vector>
#include <utility> // for std::swap
#include <cstddef> // for std::size_t

// Swaps main diagonal elements with anti-diagonal elements in a square matrix.
// If the matrix is not square, the function does nothing.
void swapDiagonals(std::vector<std::vector<int>>& matrix) {
    const std::size_t size = matrix.size();
    
    // Check squareness: all rows must have length equal to the number of rows.
    for (const auto& row : matrix) {
        if (row.size() != size) {
            return; // Not square, do nothing.
        }
    }
    
    // Swap for each row.
    for (std::size_t i = 0; i < size; ++i) {
        std::size_t j = size - 1 - i;
        std::swap(matrix[i][i], matrix[i][j]);
    }
}

// The main algorithm iterates over each row index `i` from 0 to `size-1`. For each row, we swap the value at column `i` (main diagonal) with the value at column `size-1-i` (anti-diagonal). The swap can be done using `std::swap` to avoid a temporary variable. Important edge cases:  
// - **Size 1:** `i=0`, both positions are the same element, so `std::swap` with the same element is a safe no-op.  
// - **Size 0:** The loop simply does not execute, leaving the empty vector unchanged.  
// - **Non-square input:** If the matrix is not square, the function should do nothing (or throw, but doing nothing is safer and matches typical library behavior). We check that all rows have the same length as the number of rows.  
// Time complexity is **O(size)** because we perform exactly `size` swaps (one per row), regardless of the matrix's total area (`size^2`). Space complexity is **O(1)** additional space, since we modify the matrix in-place and only use a loop index and the temporary storage inside `std::swap`.

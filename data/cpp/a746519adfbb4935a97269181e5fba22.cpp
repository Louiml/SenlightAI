// Write a C++ function that takes an integer `size` (where `size >= 1`) and dynamically allocates a square 2D array of dimensions `size x size`, fills it so that it forms a "ziggurat" pattern (concentric square rings), and returns a pointer to the allocated 2D array. The value at each cell `(row, column)` (using 1-based indexing similar to the snippet) should be equal to the minimum distance from that cell to any edge of the square, where distance is measured in "layers" (e.g., the outermost ring gets value 1, the next inner ring gets value 2, and so on until the center which gets `(size+1)/2` for odd sizes or `size/2` for even sizes). The function must allocate memory using `new` and the caller is responsible for deallocating it. The function signature should be `int** build_ziggurat(int size)` and it should not rely on any pre-allocated array—it allocates and returns a new one. Handle the edge case where `size` is 0 by returning `nullptr`. Ensure the fill pattern is correct for both odd and even sizes.
#include <cassert>
#include <iostream>

// Forward declaration of the function being tested.
int** build_ziggurat(int size);

// Helper to deallocate a 2D array created by build_ziggurat.
void delete_ziggurat(int** array, int size) {
    if (array == nullptr) return;
    for (int i = 0; i < size; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

int main() {
    // Test size 0 → nullptr
    assert(build_ziggurat(0) == nullptr);

    // Test size 1
    {
        int size = 1;
        int** z = build_ziggurat(size);
        assert(z != nullptr);
        assert(z[0][0] == 1);
        delete_ziggurat(z, size);
    }

    // Test size 3 (odd)
    {
        int size = 3;
        int** z = build_ziggurat(size);
        // Expected pattern:
        // 1 1 1
        // 1 2 1
        // 1 1 1
        assert(z[0][0] == 1); assert(z[0][1] == 1); assert(z[0][2] == 1);
        assert(z[1][0] == 1); assert(z[1][1] == 2); assert(z[1][2] == 1);
        assert(z[2][0] == 1); assert(z[2][1] == 1); assert(z[2][2] == 1);
        delete_ziggurat(z, size);
    }

    // Test size 4 (even)
    {
        int size = 4;
        int** z = build_ziggurat(size);
        // Expected pattern:
        // 1 1 1 1
        // 1 2 2 1
        // 1 2 2 1
        // 1 1 1 1
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                int expected = std::min(std::min(r + 1, c + 1),
                                        std::min(size - r, size - c));
                assert(z[r][c] == expected);
            }
        }
        delete_ziggurat(z, size);
    }

    // Test size 5 (odd, center = 3)
    {
        int size = 5;
        int** z = build_ziggurat(size);
        // Check center
        assert(z[2][2] == 3);
        // Check a corner
        assert(z[0][0] == 1);
        // Check an inner ring cell
        assert(z[1][1] == 2);
        // Check full pattern
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                int expected = std::min(std::min(r + 1, c + 1),
                                        std::min(size - r, size - c));
                assert(z[r][c] == expected);
            }
        }
        delete_ziggurat(z, size);
    }

    // Test size 6 (even, innermost 2x2 are all 3)
    {
        int size = 6;
        int** z = build_ziggurat(size);
        // Check the 2x2 center block (indices 2,3 and 2,3)
        assert(z[2][2] == 3); assert(z[2][3] == 3);
        assert(z[3][2] == 3); assert(z[3][3] == 3);
        // Check the outer ring
        assert(z[0][0] == 1);
        assert(z[5][5] == 1);
        delete_ziggurat(z, size);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <cstddef>

// Builds a square "ziggurat" pattern of the given size, returns a newly allocated 2D array.
// The caller must delete the array (delete[] each row and then delete[] the pointer array).
// Returns nullptr if size == 0.
int** build_ziggurat(int size) {
    if (size <= 0) {
        return nullptr;
    }

    // Allocate array of row pointers (size rows).
    int** ziggurat = new int*[size];
    for (int row = 0; row < size; ++row) {
        ziggurat[row] = new int[size];
    }

    // Fill with concentric layers: value at (row, col) = min distance to outer edge (1-based).
    for (int row = 1; row <= size; ++row) {
        for (int col = 1; col <= size; ++col) {
            // Compute the minimum of four distances to the edges:
            // top (row), bottom (size - row + 1), left (col), right (size - col + 1)
            int top = row;
            int bottom = size - row + 1;
            int left = col;
            int right = size - col + 1;

            int value = top;
            if (bottom < value) value = bottom;
            if (left < value) value = left;
            if (right < value) value = right;

            ziggurat[row - 1][col - 1] = value;
        }
    }

    return ziggurat;
}
// The problem is a classic "concentric squares" pattern similar to a ziggurat or a nesting doll. The key observation is that for any cell `(i, j)` (using 1-based indexing from 1 to size), the value is `min(i, j, size - i + 1, size - j + 1)`. This gives the "layer" depth from the nearest edge. For example, in a 5x5 grid, the outermost ring is all 1s, the next ring is all 2s, and the center cell is 3. For even sizes, the innermost 2x2 block will all have the same value equal to `size/2`. The main algorithm: allocate a 2D array using `new int*[size]` and for each row allocate `new int[size]`, then loop through all cells and assign the value using the formula above. Edge cases: `size == 0` must return `nullptr`. `size == 1` returns a 1x1 array with value 1. Time complexity is O(size^2) because we fill every cell. Space complexity is O(size^2) for the allocated array itself, plus a small constant for loop counters. The function does not require any auxiliary data structures beyond the array itself.

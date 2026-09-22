Write a C++ function `int getElementAt(int row, int col)` that simulates accessing a fixed 3×4 two-dimensional integer array where all elements are initialized to 0 except the element at row index 2 and column index 1, which is set to 42. The function should return the value at the given row and column. Use a static local array inside the function to represent the grid, initialize it once using a nested initialization list, and return the element at the specified position. The function must handle out-of-bounds indices gracefully by returning -1 instead of causing undefined behavior. The function should be `const`-correct (i.e., it does not modify the array), and the array should be declared `static const` so it is shared across calls and immutable.

// The main idea is to encapsulate a fixed 2D array inside a free function using a `static const` local variable. This guarantees the array is initialized exactly once (on first call) and cannot be modified. The array is 3 rows (indices 0–2) and 4 columns (indices 0–3). All elements default to 0, but we explicitly set `arr[2][1] = 42` in the initializer list. For an input pair `(row, col)`, first check if both indices are within valid bounds: `row >= 0 && row < 3 && col >= 0 && col < 4`. If valid, return `arr[row][col]`; otherwise return `-1` as an error sentinel. Edge cases include negative indices, indices equal to array dimensions, and valid corners like (0,0) and (2,3). Time complexity is O(1) because array access is constant time; space complexity is O(1) for the fixed array (12 integers) regardless of number of calls.

#include <cstddef> // for size_t if needed, but not required

// Returns the element at (row, col) in a fixed 3x4 grid.
// All elements are 0 except arr[2][1] == 42.
// If indices are out of bounds, returns -1.
int getElementAt(int row, int col) {
    // Static and const: initialized once, immutable, shared across calls.
    static const int arr[3][4] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 42, 0, 0}
    };

    // Check bounds.
    if (row < 0 || row >= 3 || col < 0 || col >= 4) {
        return -1; // out-of-bounds sentinel
    }

    // Return the element.
    return arr[row][col];
}

#include <cassert>

int main() {
    // Valid indices
    assert(getElementAt(0, 0) == 0);
    assert(getElementAt(0, 3) == 0);
    assert(getElementAt(1, 2) == 0);
    assert(getElementAt(2, 1) == 42); // The special value
    assert(getElementAt(2, 0) == 0);
    assert(getElementAt(2, 3) == 0);
    assert(getElementAt(1, 1) == 0);

    // Out-of-bounds
    assert(getElementAt(-1, 0) == -1);
    assert(getElementAt(3, 0) == -1);
    assert(getElementAt(0, -1) == -1);
    assert(getElementAt(0, 4) == -1);
    assert(getElementAt(5, 5) == -1);
    
    return 0;
}

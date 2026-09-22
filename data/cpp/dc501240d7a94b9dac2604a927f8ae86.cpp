/*
Given a 20x20 grid of organisms where ants and doodlebugs are placed at specific coordinates, write a standalone C++ function named `countDoodlebugs` that accepts a 2D array of `organism` objects (with the same class hierarchy as shown in the snippet: a base class `organism`, and derived classes `ant` and `doodlebug`) and returns the number of doodlebugs present in the grid. The function must use `dynamic_cast` to identify doodlebugs, mirroring the display logic in the snippet. The grid is always exactly 20x20, and coordinates are zero-indexed. The function should be `const`-correct, taking a `const` reference to the grid (or a pointer to the array) and not modifying it. The caller will populate the grid with exactly 100 ants placed at even-even coordinates (i%2==0 && k%2==0) and 5 doodlebugs at the coordinates (1,0), (3,5), (5,15), (8,5), (17,12), but your function must work for any valid placement, including zero doodlebugs or many more. You may assume the `organism` classes have a virtual destructor so that `dynamic_cast` works correctly. Provide the function definition only, not a `main` function, but in the test section you will write a `main` that creates the grid, populates it as described, calls your function, and asserts the count equals 5.
*/

#include <cstddef>

// Forward declarations matching the snippet's assumed classes.
class organism {
public:
    virtual ~organism() = default;
};

class ant : public organism {
public:
    ant(int, int) {}
};

class doodlebug : public organism {
public:
    doodlebug(int, int) {}
};

// Count the number of doodlebugs in the 20x20 grid.
int countDoodlebugs(const organism grid[20][20]) {
    int count = 0;
    for (std::size_t i = 0; i < 20; ++i) {
        for (std::size_t k = 0; k < 20; ++k) {
            if (dynamic_cast<const doodlebug*>(&grid[i][k]) != nullptr) {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>

// Assume the classes and function from the solution are included above.

int main() {
    organism grid[20][20];

    // Population as in the snippet: ants on even-even coordinates.
    for (int i = 0; i < 20; ++i) {
        for (int k = 0; k < 20; ++k) {
            if (i % 2 == 0 && k % 2 == 0) {
                grid[i][k] = ant(i, k);
            }
        }
    }
    // Add exactly 5 doodlebugs at given coordinates.
    grid[1][0] = doodlebug(1, 0);
    grid[3][5] = doodlebug(3, 5);
    grid[5][15] = doodlebug(5, 15);
    grid[8][5] = doodlebug(8, 5);
    grid[17][12] = doodlebug(17, 12);

    assert(countDoodlebugs(grid) == 5);

    // Test with zero doodlebugs: fill grid with only ants (0,0 is ant, but others empty?).
    organism grid2[20][20] = {};
    for (int i = 0; i < 20; ++i) {
        for (int k = 0; k < 20; ++k) {
            if (i % 2 == 0 && k % 2 == 0) {
                grid2[i][k] = ant(i, k);
            }
        }
    }
    assert(countDoodlebugs(grid2) == 0);

    // Test with many doodlebugs: fill all cells with doodlebugs.
    organism grid3[20][20] = {};
    for (int i = 0; i < 20; ++i) {
        for (int k = 0; k < 20; ++k) {
            grid3[i][k] = doodlebug(i, k);
        }
    }
    assert(countDoodlebugs(grid3) == 400);

    // Test a small mixed set: 2 doodlebugs at corners.
    organism grid4[20][20] = {};
    grid4[0][0] = doodlebug(0, 0);
    grid4[19][19] = doodlebug(19, 19);
    assert(countDoodlebugs(grid4) == 2);

    return 0;
}

// The solution iterates over all 20x20 = 400 cells of the grid. For each cell, we check whether the stored object is of type `doodlebug` using `dynamic_cast<doodlebug*>(&grid[i][k])`. Because `organism` is a polymorphic base class (with at least a virtual destructor), `dynamic_cast` returns a non-null pointer if and only if the object is actually a `doodlebug` (or a subclass, but here it's exact). We increment a counter for each successful cast. The algorithm is straightforward: two nested loops covering row index 0..19 and column index 0..19. Edge cases include an empty grid (but the grid is always exactly 20x20), grids with no doodlebugs (count returns 0), or grids with doodlebugs at any positions including corners and edges. No special handling is needed for ants or empty cells; they simply do not match the cast. Time complexity is O(400) = O(1) since the grid size is fixed, but more generally O(R*C) for an R×C grid; here it's constant. Space complexity is O(1) additional, as only a counter is used. The function must be declared to accept `const organism grid[20][20]` or `const organism (&grid)[20][20]` to preserve const correctness; since the function does not modify the grid, `const` reference or pointer is appropriate. Note that `dynamic_cast` on a `const` object requires `const` pointer, which is fine. The solution is simple and directly mirrors the display logic from the given snippet, but instead of printing, we count.

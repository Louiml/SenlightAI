/*
Write a C++ function named `create2DGrid` that takes two integer parameters `rows` and `cols` (both guaranteed to be non-negative) and returns a 2D `std::vector<std::vector<int>>` where every cell is initialized to `0`. The function should also include a separate helper function `printGrid` that takes the 2D grid by const reference and prints each element separated by a space, with each row on a new line. In the main program (not part of the solution function), you will call `create2DGrid` to build a grid of size `5×10` and then `printGrid` to display it. The task focuses on correct vector-of-vector initialization, const-correctness for printing, and handling edge cases like `0` rows or columns.
*/

#include <vector>
#include <iostream>

// Creates a 2D grid with given dimensions, all cells initialized to 0.
std::vector<std::vector<int>> create2DGrid(int rows, int cols) {
    // Using the fill constructor for each row.
    return std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
}

// Prints the grid row by row, each element separated by a space.
// Takes a const reference to avoid copying and preserves const-correctness.
void printGrid(const std::vector<std::vector<int>>& grid) {
    if (grid.empty()) {
        return; // Nothing to print.
    }
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[0].size(); ++j) {
            std::cout << grid[i][j];
            if (j + 1 < grid[0].size()) {
                std::cout << " ";
            }
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <vector>
#include <sstream>
#include <iostream>

// Declare the functions (for test purposes, we assume they are defined above).
std::vector<std::vector<int>> create2DGrid(int rows, int cols);
void printGrid(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Grid of size 5x10, verify all elements are 0.
    auto grid1 = create2DGrid(5, 10);
    assert(grid1.size() == 5);
    for (const auto& row : grid1) {
        assert(row.size() == 10);
        for (int val : row) {
            assert(val == 0);
        }
    }

    // Test 2: Empty grid when rows=0.
    auto grid2 = create2DGrid(0, 5);
    assert(grid2.empty());

    // Test 3: Empty grid when cols=0.
    auto grid3 = create2DGrid(5, 0);
    assert(grid3.size() == 5);
    for (const auto& row : grid3) {
        assert(row.empty());
    }

    // Test 4: 1x1 grid.
    auto grid4 = create2DGrid(1, 1);
    assert(grid4.size() == 1 && grid4[0].size() == 1 && grid4[0][0] == 0);

    // Test 5: Verify print output for a 2x3 grid using a stringstream.
    // We capture cout and compare after calling printGrid.
    auto grid5 = create2DGrid(2, 3);
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printGrid(grid5);
    std::cout.rdbuf(oldCout);
    assert(buffer.str() == "0 0 0\n0 0 0\n");

    // Test 6: printGrid on empty grid should not crash and output nothing.
    auto grid6 = create2DGrid(0, 10);
    std::stringstream buffer2;
    std::streambuf* oldCout2 = std::cout.rdbuf(buffer2.rdbuf());
    printGrid(grid6);
    std::cout.rdbuf(oldCout2);
    assert(buffer2.str().empty());

    return 0;
}

// The solution uses the standard library's `std::vector` to create a 2D structure. The `create2DGrid` function constructs a vector with `rows` elements, each being a vector<int> of size `cols`, all initialized to `0` — this is done efficiently with the fill constructor `vector<int>(cols, 0)`. If either `rows` or `cols` is 0, the resulting grid will be empty (or contain empty inner vectors), which is valid and requires no special handling. For `printGrid`, we iterate through the outer vector using `arr.size()` and the inner vectors using `arr[0].size()` only if the grid is non-empty; otherwise, we output nothing. We must guard against `arr[0]` access when the outer vector is empty, so check `if (grid.empty())` before using `grid[0].size()`. The time complexity for creation is O(rows * cols) because each cell is initialized. Printing also takes O(rows * cols) time. Space complexity is O(rows * cols) for the grid itself, plus O(1) auxiliary space for the printing function (the grid is passed by const reference).

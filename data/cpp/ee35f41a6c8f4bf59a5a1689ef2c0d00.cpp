Write a C++ function that, given a positive integer `n`, creates an `n × n` checkerboard pattern grid where cells with both row and column indices having the same parity (i.e., `(i + j) % 2 == 0`) are marked with the character `'C'`, and all other cells are marked with `'.'`. The function must return a `std::vector<std::string>` representing the grid, where each string is exactly `n` characters long. The function should also support returning the total count of `'C'` cells. Since C++ functions can return only one value, design the function to return the grid, and separately provide a helper function that returns the count of `'C'` cells for a given `n`. Handle edge cases such as `n = 1` (single cell: `'C'`) and very large `n` (up to 1000) efficiently. The output grid rows should not contain any trailing spaces or extra characters.
#include <cassert>
#include <vector>
#include <string>

// The solution functions are declared above; here we test them.
int main() {
    // n = 1: single cell is 'C'
    auto grid1 = buildCheckerboard(1);
    assert(grid1.size() == 1);
    assert(grid1[0] == "C");
    assert(countCheckerboardCells(1) == 1);

    // n = 2: pattern C . / . C -> count 2
    auto grid2 = buildCheckerboard(2);
    assert(grid2.size() == 2);
    assert(grid2[0] == "C.");
    assert(grid2[1] == ".C");
    assert(countCheckerboardCells(2) == 2);

    // n = 3: pattern C . C / . C . / C . C -> count 5
    auto grid3 = buildCheckerboard(3);
    assert(grid3.size() == 3);
    assert(grid3[0] == "C.C");
    assert(grid3[1] == ".C.");
    assert(grid3[2] == "C.C");
    assert(countCheckerboardCells(3) == 5);

    // n = 4: count 8 (even-even and odd-odd cells: 4+4=8)
    assert(countCheckerboardCells(4) == 8);
    auto grid4 = buildCheckerboard(4);
    assert(grid4.size() == 4);
    assert(grid4[0] == "C.C.");
    assert(grid4[1] == ".C.C");
    assert(grid4[2] == "C.C.");
    assert(grid4[3] == ".C.C");

    // n = 5: count 13 (ceil(2.5)^2 + floor(2.5)^2 = 9+4=13)
    assert(countCheckerboardCells(5) == 13);
    auto grid5 = buildCheckerboard(5);
    assert(grid5.size() == 5);
    assert(grid5[0] == "C.C.C");
    assert(grid5[4] == "C.C.C");

    // Large n sanity check: n=1000, count should be 500*500 + 500*500 = 500000
    assert(countCheckerboardCells(1000) == 500000);
    auto grid1000 = buildCheckerboard(1000);
    assert(grid1000.size() == 1000);
    assert(grid1000[0].size() == 1000);
    assert(grid1000[0][0] == 'C');
    assert(grid1000[999][999] == 'C'); // (999+999)%2=0
    assert(grid1000[0][1] == '.');     // (0+1)%2=1
}
#include <string>
#include <vector>

// Builds an n x n checkerboard grid where cells with (i+j)%2==0 are 'C', else '.'.
// Returns the grid as a vector of strings.
std::vector<std::string> buildCheckerboard(int n) {
    std::vector<std::string> grid;
    grid.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::string row(n, '.');
        for (int j = 0; j < n; ++j) {
            if ((i + j) % 2 == 0) {
                row[j] = 'C';
            }
        }
        grid.push_back(row);
    }
    return grid;
}

// Returns the number of 'C' cells in the checkerboard pattern for size n.
int countCheckerboardCells(int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((i + j) % 2 == 0) {
                ++count;
            }
        }
    }
    return count;
}
// The solution is straightforward: for an `n × n` grid, each cell at row `i` and column `j` is checked for the condition `(i + j) % 2 == 0`. If true, place `'C'`; otherwise place `'.'`. The count of such cells can be computed mathematically without iterating, but iterating is simpler and still `O(n^2)` time, which is acceptable for `n ≤ 1000` (1,000,000 cells max). The space complexity is `O(n^2)` to store the grid. Edge cases: `n = 0` is not allowed per problem (positive integer), but if encountered, return an empty vector and count 0. For `n = 1`, the only cell satisfies the condition, so grid is `{"C"}` and count is 1. The parity condition is symmetric, so for any `n`, the number of `'C'` cells is `ceil(n/2) * ceil(n/2) + floor(n/2) * floor(n/2)` because even-even and odd-odd combinations are counted. However, to keep the solution simple and generic, we can just iterate and count. The grid construction uses a `std::string` of length `n`, initialized with `'.'`, then set `'C'` at positions where `(i + j) % 2 == 0`.

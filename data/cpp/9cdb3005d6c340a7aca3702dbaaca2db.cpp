// Create a C++ function that simulates counting "dangerous cells" in a grid-like structure. The function should take two integers, `rows` and `cols`, followed by a sequence of `2 * rows * cols` integers representing coordinate pairs (x, y) for each cell in a `rows x cols` grid. For each cell, if either x equals 1 or y equals 1, the cell is considered dangerous. The function should return the total count of such dangerous cells. The input is guaranteed to have at least one row and one column, and all coordinate values are non-negative integers. The function must be standalone (no `main`), use `const` where appropriate, and should not rely on global variables.

// The main algorithm is straightforward: we process the input as a flat sequence of coordinate pairs. There are `rows * cols` total cells, and for each cell we read two integers (x and y). We increment a counter if either coordinate equals 1. The order of cells does not matter since we only care about the total count. Edge cases include rows or cols being zero (though constraints likely avoid this), values that are not 1 (e.g., 0, 2, or larger), and having all cells non-dangerous. The algorithm runs in O(rows * cols) time because we read exactly that many pairs, and uses O(1) auxiliary space since we only store a counter and temporary values. We must be careful to read all inputs correctly without missing any pairs, and handle the case where both coordinates are 1 (which still counts as one dangerous cell, not two).

#include <iostream>

// Count cells in a rows x cols grid where either coordinate equals 1.
int countDangerousCells(int rows, int cols, std::istream& input = std::cin) {
    int dangerousCount = 0;
    int totalCells = rows * cols;

    for (int cell = 0; cell < totalCells; ++cell) {
        int x, y;
        input >> x >> y;
        if (x == 1 || y == 1) {
            ++dangerousCount;
        }
    }

    return dangerousCount;
}

#include <iostream>
#include <sstream>
#include <cassert>

// Count cells in a rows x cols grid where either coordinate equals 1.
int countDangerousCells(int rows, int cols, std::istream& input = std::cin) {
    int dangerousCount = 0;
    int totalCells = rows * cols;

    for (int cell = 0; cell < totalCells; ++cell) {
        int x, y;
        input >> x >> y;
        if (x == 1 || y == 1) {
            ++dangerousCount;
        }
    }

    return dangerousCount;
}

int main() {
    // Test 1: Basic 1x1 grid with dangerous cell
    std::istringstream input1("1 1");
    assert(countDangerousCells(1, 1, input1) == 1);

    // Test 2: 2x2 grid with one dangerous cell
    std::istringstream input2("0 0 1 0 0 1 2 2");
    assert(countDangerousCells(2, 2, input2) == 2);

    // Test 3: 3x1 grid (3 rows, 1 column) all dangerous
    std::istringstream input3("1 5 1 0 1 1");
    assert(countDangerousCells(3, 1, input3) == 3);

    // Test 4: 1x3 grid (1 row, 3 columns) none dangerous
    std::istringstream input4("0 0 0 2 3 4");
    assert(countDangerousCells(1, 3, input4) == 0);

    // Test 5: Larger grid with mixed values, including both coordinates = 1
    std::istringstream input5("1 1 0 1 1 0 0 0 2 3 1 2");
    assert(countDangerousCells(3, 2, input5) == 4);

    // Test 6: Single cell with x=1 but y=0
    std::istringstream input6("1 0");
    assert(countDangerousCells(1, 1, input6) == 1);

    // Test 7: Single cell with x=0 but y=1
    std::istringstream input7("0 1");
    assert(countDangerousCells(1, 1, input7) == 1);

    // Test 8: Single cell with neither coordinate = 1
    std::istringstream input8("0 0");
    assert(countDangerousCells(1, 1, input8) == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

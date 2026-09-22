// Write a C++ function `createMultiplicationTable(int rows, int cols)` that dynamically allocates a 2D array of integers using `new`, fills each element at row `i` and column `j` with the product `i * j` (so that row 0 is all zeros, column 0 is all zeros, and the value at `(i, j)` equals the multiplication of the row index and the column index), and returns the pointer to the array of pointers. The function must also provide a companion `printMultiplicationTable(int** table, int rows, int cols)` that prints the table with a header row showing column indices from 0 to `cols-1`, a header column showing row indices from 0 to `rows-1`, and right-aligned cell values with a width of 4, followed by a separate `destroyTable(int** table, int rows)` function that properly deallocates all column arrays and then the row pointer array. Your task is to implement these three functions with proper memory management, handling edge cases where `rows` or `cols` may be 0, in which case the allocation should return `nullptr` for `createMultiplicationTable` (and print nothing) and `destroyTable` should do nothing. Ensure the printed output exactly matches the format: first line "THIS IS YOUR MULTIPLICATION TABLE!!!", second line starts with a right-aligned 0 with width 8, then each column index from 1 to `cols-1` right-aligned with width 4, then a newline, and then each row header (row index) right-aligned with width 4 followed by each cell value right-aligned with width 4, one row per line.
The core algorithm involves three separate responsibilities. For `createMultiplicationTable`, we first handle the edge case: if either `rows` or `cols` is 0, return `nullptr` without allocating anything. Otherwise, allocate an array of `rows` pointers using `new int*[rows]`. Then, for each row index from 0 to `rows-1`, allocate an array of `cols` integers using `new int[cols]`. After all allocations succeed, we fill each element by nested loops: for each row index `i`, for each column index `j`, set `array[i][j] = i * j`. This guarantees that the first row and first column are all zeros, and the table is a proper multiplication table (e.g., row 3, column 4 gives 12). Time complexity is O(rows * cols) for both allocation and filling, and space complexity is O(rows * cols) for the stored data plus O(rows) for the pointer array, which is part of the returned structure.

For `printMultiplicationTable`, we first check if `table` is `nullptr`; if so, print nothing and return. Otherwise, print the heading line exactly as specified. Then print the column header row: first, output `setw(8) << 0` to align the zero column header with the row header column (which uses width 4) plus 4 extra spaces to make room for the row index column, then for each column index from 1 to `cols-1`, print `setw(4) << j`. After that, print a newline. Then, for each row index from 0 to `rows-1`, print `setw(4) << i` (the row header), then for each column index from 0 to `cols-1`, print `setw(4) << table[i][j]`, and finally print a newline. No extra spaces or lines are added. Time complexity is O(rows * cols) due to printing all cells, and space complexity is O(1) auxiliary.

For `destroyTable`, if `table` is `nullptr` or `rows` is 0, return immediately. Otherwise, loop from 0 to `rows-1` and delete each column array with `delete [] table[i]`. After the loop, delete the row pointer array with `delete [] table`. This prevents memory leaks. Time complexity is O(rows) for deallocation, and space complexity is O(1).

Edge cases to consider: zero rows or zero columns must be handled gracefully without crashes, and the print function must not dereference a null pointer. Also, ensure that the `iomanip` header is included for `setw`, and that the functions use `const` correctly where possible: the pointer parameter in `printMultiplicationTable` could be `int* const* table` to indicate that the array of pointers is const, but the pointed-to arrays are not const (since we don't modify them, we can use `const int* const*` for read-only access, but for simplicity and to match the original style, we keep `int**`). However, for const correctness, we should use `const int* const* table` in the print function to signal that we only read the data. In the solution below, we appropriately apply `const` to the pointer type.
#include <iostream>
#include <iomanip>

// Dynamically allocate a 2D multiplication table of size rows x cols.
// Returns nullptr if rows or cols is 0.
int** createMultiplicationTable(int rows, int cols) {
    if (rows == 0 || cols == 0) {
        return nullptr;
    }

    int** table = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        table[i] = new int[cols];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            table[i][j] = i * j;
        }
    }

    return table;
}

// Print the multiplication table with row and column headers.
// If table is nullptr, does nothing.
void printMultiplicationTable(const int* const* table, int rows, int cols) {
    if (table == nullptr || rows == 0 || cols == 0) {
        return;
    }

    std::cout << "THIS IS YOUR MULTIPLICATION TABLE!!!" << std::endl;
    std::cout << std::setw(8) << 0;
    for (int j = 1; j < cols; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << std::endl;

    for (int i = 0; i < rows; ++i) {
        std::cout << std::setw(4) << i;
        for (int j = 0; j < cols; ++j) {
            std::cout << std::setw(4) << table[i][j];
        }
        std::cout << std::endl;
    }
}

// Deallocate a 2D array previously created by createMultiplicationTable.
// If table is nullptr or rows is 0, does nothing.
void destroyTable(int** table, int rows) {
    if (table == nullptr || rows == 0) {
        return;
    }

    for (int i = 0; i < rows; ++i) {
        delete[] table[i];
    }
    delete[] table;
}
#include <cassert>
#include <sstream>

// Helper to capture printed output for testing (placeholder for demonstration)
// The actual assertions below directly test the table contents and integrity.

int main() {
    // Test 1: Basic 3x4 table
    int rows = 3, cols = 4;
    int** table = createMultiplicationTable(rows, cols);
    assert(table != nullptr);
    assert(table[0][0] == 0);
    assert(table[0][3] == 0);
    assert(table[2][3] == 6);
    assert(table[1][2] == 2);
    destroyTable(table, rows);

    // Test 2: Zero rows returns nullptr
    assert(createMultiplicationTable(0, 5) == nullptr);

    // Test 3: Zero cols returns nullptr
    assert(createMultiplicationTable(5, 0) == nullptr);

    // Test 4: Single row and single column
    rows = 1; cols = 1;
    table = createMultiplicationTable(rows, cols);
    assert(table != nullptr);
    assert(table[0][0] == 0);
    destroyTable(table, rows);

    // Test 5: Larger table, check diagonals and symmetry
    rows = 5; cols = 5;
    table = createMultiplicationTable(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            assert(table[i][j] == i * j);
        }
    }
    // Check first row and first column are all zeros
    for (int j = 0; j < cols; ++j) {
        assert(table[0][j] == 0);
    }
    for (int i = 0; i < rows; ++i) {
        assert(table[i][0] == 0);
    }
    destroyTable(table, rows);

    // Test 6: Non-square table (e.g., 2 rows, 7 cols)
    rows = 2; cols = 7;
    table = createMultiplicationTable(rows, cols);
    assert(table[1][6] == 6);
    assert(table[0][5] == 0);
    destroyTable(table, rows);

    // Test 7: Destroying nullptr should not crash
    destroyTable(nullptr, 3);

    // Test 8: Destroying with rows=0 should not crash even if table is non-null
    rows = 0; cols = 3;
    table = createMultiplicationTable(rows, cols); // returns nullptr
    assert(table == nullptr);
    destroyTable(table, rows); // safe

    // Test 9: Verify all values for 2x2
    rows = 2; cols = 2;
    table = createMultiplicationTable(rows, cols);
    assert(table[0][0] == 0);
    assert(table[0][1] == 0);
    assert(table[1][0] == 0);
    assert(table[1][1] == 1);
    destroyTable(table, rows);

    // Test 10: Verify 3x3 full check
    rows = 3; cols = 3;
    table = createMultiplicationTable(rows, cols);
    int expected[3][3] = {
        {0, 0, 0},
        {0, 1, 2},
        {0, 2, 4}
    };
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            assert(table[i][j] == expected[i][j]);
        }
    }
    destroyTable(table, rows);

    return 0;
}

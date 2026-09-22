// Write a C++ function that takes a string `s` of length 5 representing a rectangular range of spreadsheet cells (e.g., `"A1:C3"`), where the first and fourth characters are uppercase letters (`'A'`–`'Z'`) for columns, and the second and fifth characters are digits (`'1'`–`'9'`) for rows. The function must return a vector of strings, sorted first by column (left-to-right) and then by row (top-to-bottom), containing all cell references in the range, inclusive of both boundaries. For example, `"A1:C3"` should return `{"A1","A2","A3","B1","B2","B3","C1","C2","C3"}`. Assume valid input (first column ≤ second column, first row ≤ second row).
#include <cassert>
#include <string>
#include <vector>

std::vector<std::string> cellsInRange(const std::string& rangeSpec);

int main() {
    // Standard rectangular range
    std::vector<std::string> expected1 = {"A1","A2","A3","B1","B2","B3","C1","C2","C3"};
    assert(cellsInRange("A1:C3") == expected1);

    // Single cell range
    assert(cellsInRange("B2:B2") == std::vector<std::string>{"B2"});

    // Single column, multiple rows
    std::vector<std::string> expectedCol = {"X5","X6","X7"};
    assert(cellsInRange("X5:X7") == expectedCol);

    // Single row, multiple columns
    std::vector<std::string> expectedRow = {"D1","E1","F1","G1"};
    assert(cellsInRange("D1:G1") == expectedRow);

    // Full alphabet start and maximum digit
    std::vector<std::string> expectedFull = {"A9","B9"};
    assert(cellsInRange("A9:B9") == expectedFull);

    // Non-adjacent letters and rows
    std::vector<std::string> expectedMixed = {"E2","F2","G2","E3","F3","G3"};
    assert(cellsInRange("E2:G3") == expectedMixed);

    // Single row, same column (still valid)
    assert(cellsInRange("C4:C4") == std::vector<std::string>{"C4"});
}
#include <string>
#include <vector>

// Return all cell references in the rectangular range specified by
// a string of the form "C1R1:C2R2" (e.g., "A1:C3"), sorted by column then row.
std::vector<std::string> cellsInRange(const std::string& rangeSpec) {
    std::vector<std::string> result;

    char startCol = rangeSpec[0];
    char endCol   = rangeSpec[3];
    char startRow = rangeSpec[1];
    char endRow   = rangeSpec[4];

    for (char col = startCol; col <= endCol; ++col) {
        for (char row = startRow; row <= endRow; ++row) {
            result.push_back(std::string(1, col) + std::string(1, row));
        }
    }

    return result;
}
// The solution uses two nested loops driven by ASCII character values. The outer loop iterates `c` from the first column character (`s[0]`) up to and including the second column character (`s[3]`). The inner loop iterates `r` from the first row character (`s[1]`) up to and including the second row character (`s[4]`). For each pair, construct a two-character string `temp` by concatenating the column character and the row character, then push it into the result vector. Because the loops run in the natural ASCII order (which aligns with alphabetic for uppercase letters and numeric order for digits), the resulting vector is correctly sorted by column first, then by row. No special handling is needed for edge cases like single-row or single-column ranges; the loops naturally handle them. Time complexity is \(O((C_2 - C_1 + 1) \times (R_2 - R_1 + 1))\), i.e., proportional to the number of cells in the range, and space complexity is \(O(1)\) extra besides the output vector itself.

Write a C++ function `std::string convertToZigzag(const std::string& s, int numRows)` that takes an input string `s` and a positive integer `numRows` (representing the number of rows in a zigzag pattern) and returns the string that results from reading the characters of `s` row by row after arranging them in a zigzag pattern. The zigzag pattern is formed by writing the characters of `s` down the rows from top to bottom, then diagonally up to the top (skipping the top row on the way up), then down again, and so on, until all characters are placed. If `numRows` is 1 or greater than or equal to the length of the string, the function should return the original string unchanged. The input string may contain any printable ASCII characters including spaces, and the function must handle empty strings (return an empty string). The function must not use any external libraries beyond the standard C++ libraries, and must be efficient for large inputs.
// The core algorithm simulates the zigzag traversal by distributing characters into `numRows` separate strings (one per row). We iterate through the input string character by character, moving downward (increasing row index) until we hit the bottom row, then reverse direction (decreasing row index) until we reach the top row, and so on. For each character, we append it to the string corresponding to the current row. After processing all characters, we concatenate the row strings in order from row 0 to `numRows-1` to produce the result.  
// Edge cases:  
// - If `numRows == 1`, the zigzag is just the original string because there is no vertical movement.  
// - If `numRows >= s.length()`, each character lands on a distinct row, and reading row by row yields the original string (since rows 0..len-1 each have one character, and the order is preserved).  
// - Empty string: no characters to process, return empty.  
// - When `numRows` is large but the string is short, the algorithm naturally handles it because we only ever have rows that get characters; the extra rows remain empty and contribute nothing to the final concatenation.  
// Time complexity: O(n) where n is the length of `s`, since we visit each character exactly once. Space complexity: O(n) for storing the row strings, plus O(numRows) for the vector of strings (which is at most O(n) when numRows ≤ n, but if numRows > n, the vector still allocates numRows empty strings, so worst-case O(numRows + n) which is O(n) if we consider numRows could be arbitrarily large, but typically we treat it as O(n) because we only have n characters to distribute).
#include <string>
#include <vector>

// Convert a string into its zigzag pattern reading order for a given number of rows.
std::string convertToZigzag(const std::string& s, int numRows) {
    // Edge cases: single row or empty input returns the original string.
    if (numRows <= 1 || s.empty()) {
        return s;
    }

    // Create a vector of strings for each row.
    std::vector<std::string> rows(numRows);

    int currentRow = 0;
    bool goingDown = true;

    // Distribute each character to the appropriate row.
    for (char c : s) {
        rows[currentRow].push_back(c);
        // Move to the next row based on direction.
        if (goingDown) {
            ++currentRow;
            if (currentRow == numRows - 1) {
                goingDown = false;
            }
        } else {
            --currentRow;
            if (currentRow == 0) {
                goingDown = true;
            }
        }
    }

    // Concatenate all rows in order.
    std::string result;
    for (const std::string& row : rows) {
        result += row;
    }
    return result;
}
#include <cassert>
#include <string>

// Assume convertToZigzag is defined above.
int main() {
    // Basic examples.
    assert(convertToZigzag("ABCDEF", 3) == "AEBDFC");
    assert(convertToZigzag("PAYPALISHIRING", 3) == "PAHNAPLSIIGYIR");
    assert(convertToZigzag("PAYPALISHIRING", 4) == "PINALSIGYAHRPI");

    // Edge cases: single row, empty string, rows >= length.
    assert(convertToZigzag("HELLO", 1) == "HELLO");
    assert(convertToZigzag("", 3) == "");
    assert(convertToZigzag("AB", 5) == "AB");

    // Important: when numRows equals length.
    assert(convertToZigzag("ZIGZAG", 6) == "ZIGZAG");

    // Test with spaces and special characters.
    assert(convertToZigzag("A B C", 2) == "ABC ");  // Note: space is placed on row 2.

    // Test with single character.
    assert(convertToZigzag("X", 1) == "X");
    assert(convertToZigzag("X", 3) == "X");

    // Test a longer string with many rows.
    std::string longStr = "abcdefghijklmnopqrstuvwxyz";
    assert(convertToZigzag(longStr, 5) == "aiqybhjprxzcgkoswdflntvemu");

    // Test when numRows = 2.
    assert(convertToZigzag("ABCDEF", 2) == "ACEBDF");
}

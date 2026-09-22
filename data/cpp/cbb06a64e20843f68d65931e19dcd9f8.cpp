Write a C++ function named `zigzagConvert` that takes a string `s` and an integer `numRows` as input, and returns a new string formed by reading the characters of `s` in a row-by-row order after arranging them in a zigzag pattern across `numRows` rows. The zigzag pattern is created by writing characters downward from row 0 to row `numRows-1`, then diagonally upward back to row 0, repeating this cycle until all characters are placed. The output should be the concatenation of each row’s characters from top to bottom. Handle edge cases where `numRows` is 1 or greater than or equal to the string length (in which case the output is the original string). Assume the input string is non-empty.

// The core idea is to avoid constructing a 2D grid; instead, compute the column index (or position in the cycle) for each character. In a full zigzag cycle of `numRows` rows, the vertical distance from row 0 down to row `numRows-1` and back is `(numRows - 1) * 2` steps. For a given row `i` (0-indexed), the first jump (distance to the next character in the same row when moving downward) is `totalJump - 2*i`, and the second jump (when moving upward) is `2*i`. For the first and last rows, one of these jumps is zero, so we use the non‑zero jump repeatedly. For every other row, alternate between the two jumps. We iterate row by row, starting at index `i` for row `i`, and repeatedly add the appropriate jump to collect all characters belonging to that row. Edge cases: if `numRows == 1` or `s.length() == 1`, the zigzag pattern is trivial—return `s` as is. Also, if `numRows >= s.length()`, the zigzag will have no diagonals, and the output equals `s` (the function naturally handles this because each row will contain at most one character). Time complexity is \(O(n)\) where \(n = s.length()\), since each character is visited exactly once. Space complexity is \(O(n)\) for the result string, with \(O(1)\) extra auxiliary space.

#include <string>
#include <algorithm>

// Convert a string into a zigzag pattern across numRows rows and read row-wise.
std::string zigzagConvert(const std::string& s, int numRows) {
    if (numRows <= 1 || s.length() <= 1) {
        return s;
    }

    const int totalJump = (numRows - 1) * 2;
    std::string result;
    result.reserve(s.length());

    for (int row = 0; row < numRows; ++row) {
        const int firstJump = totalJump - 2 * row;
        const int secondJump = 2 * row;
        const int step = (firstJump == 0 || secondJump == 0) ? std::max(firstJump, secondJump) : firstJump;
        
        int index = row;
        bool useFirst = true;
        while (index < static_cast<int>(s.length())) {
            result.push_back(s[index]);
            index += useFirst ? firstJump : secondJump;
            useFirst = !useFirst;
        }
    }

    return result;
}

#include <cassert>
#include <string>

// The solution function is assumed to be declared above.
std::string zigzagConvert(const std::string& s, int numRows);

int main() {
    // Simple case with 3 rows: "PAYPALISHIRING" -> "PAHNAPLSIIGYIR"
    assert(zigzagConvert("PAYPALISHIRING", 3) == "PAHNAPLSIIGYIR");
    // Case with 4 rows: "PAYPALISHIRING" -> "PINALSIGYAHRPI"
    assert(zigzagConvert("PAYPALISHIRING", 4) == "PINALSIGYAHRPI");
    // Single row: output equals input
    assert(zigzagConvert("ABC", 1) == "ABC");
    // numRows larger than the string length: output equals input
    assert(zigzagConvert("ABC", 5) == "ABC");
    // Single character input
    assert(zigzagConvert("A", 3) == "A");
    // Two rows simple pattern
    assert(zigzagConvert("ABCDEF", 2) == "ACEBDF");
    // Empty string (edge case not in spec, but safe to test)
    assert(zigzagConvert("", 3) == "");
    // Large numRows with short string
    assert(zigzagConvert("HELLO", 10) == "HELLO");
    // All same characters
    assert(zigzagConvert("AAAA", 2) == "AAAA");
    return 0;
}

Write a C++ function that takes a chessboard square coordinate as a string (e.g., `"a1"`, `"h8"`) and returns `true` if the square is white, `false` if it is black. The chessboard is an 8×8 grid where the bottom-left square `a1` is black, columns are labeled `a` through `h` from left to right, and rows are numbered 1 through 8 from bottom to top. A square is white if the sum of its column index (0-based, `a=0`, `b=1`, …, `h=7`) and its row number (0-based, `1=0`, `2=1`, …, `8=7`) is even; otherwise it is black. You must implement the function without using any built-in chess or board utilities, and the input is guaranteed to be a valid lowercase letter followed by a digit from `1` to `8`.
// The problem reduces to determining the parity of the sum of the column index and the row index. The column character can be converted to a zero-based index by subtracting the ASCII value of `'a'` (97). The row digit can be converted to a zero-based index by subtracting `'1'` (49). A square is white if `(colIndex + rowIndex) % 2 == 0`; otherwise it is black. This works because the board alternates colors in a checkerboard pattern, and the parity of the indices determines the color. Edge cases include the corner squares `a1` (black, indices 0+0=0, even, so false) and `h8` (white, indices 7+7=14, even, so true). The time complexity is \(O(1)\) because only a couple of character arithmetic operations are performed, and the space complexity is \(O(1)\) because no extra data structures are used.
#include <string>

// Determine whether a chessboard square (e.g., "a1") is white.
// Returns true if the square is white, false if it is black.
bool isSquareWhite(const std::string& coordinates) {
    // Convert column letter to zero-based index (a=0, b=1, ..., h=7)
    const int colIndex = coordinates[0] - 'a';
    // Convert row digit to zero-based index (1=0, 2=1, ..., 8=7)
    const int rowIndex = coordinates[1] - '1';
    // White squares have an even sum of indices
    return (colIndex + rowIndex) % 2 == 0;
}
#include <cassert>

int main() {
    // Basic corners
    assert(isSquareWhite("a1") == false); // black
    assert(isSquareWhite("h8") == true);  // white
    assert(isSquareWhite("a8") == true);  // white
    assert(isSquareWhite("h1") == true);  // white

    // Known alternating pattern
    assert(isSquareWhite("b1") == true);  // white
    assert(isSquareWhite("a2") == true);  // white
    assert(isSquareWhite("b2") == false); // black

    // Middle squares
    assert(isSquareWhite("d4") == true);  // white
    assert(isSquareWhite("e4") == false); // black
    assert(isSquareWhite("f5") == true);  // white

    // Edges
    assert(isSquareWhite("c3") == true);  // white
    assert(isSquareWhite("g7") == false); // black
}

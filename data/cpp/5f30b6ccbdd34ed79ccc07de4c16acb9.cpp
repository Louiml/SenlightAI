// In a chess game, a queen can move any number of squares vertically, horizontally, or diagonally, but it cannot jump over pieces and cannot stay in the same square. Write a standalone C++ function named `isQueenLegalMove` that takes two `std::pair<char, char>` parameters representing the start and end coordinates on a standard 8x8 chessboard (columns 'a'–'h', rows '1'–'8'). The function must return `true` if the movement from start to end is a legal queen move shape (ignoring any pieces blocking), and `false` otherwise. The function must handle both uppercase and lowercase column letters (e.g., 'A' and 'a' both represent the first column). Consider these edge cases: the same square, moves that are only horizontal, only vertical, only diagonal (equal absolute row and column distance), and any move that is not aligned in one of these patterns.

The solution checks if the move is legal for a queen by verifying that either the row distance is zero (horizontal), the column distance is zero (vertical), or the absolute row distance equals the absolute column distance (diagonal). First, normalize the column letters to lowercase (or uppercase) so both cases work. Compute integer distances: `colDist = abs(end.first - start.first)` after converting characters to lowercase, and `rowDist = abs(end.second - start.second)` using the character difference (or convert to integer digits). If both distances are zero, the move is to the same square, so return `false`. Otherwise, return `true` if `colDist == 0 || rowDist == 0 || colDist == rowDist`. The approach is O(1) time and O(1) space, as it only performs a few arithmetic operations and comparisons. Important edge cases include same-square (must be false), moves that are not along a rank, file, or diagonal (false), and any move that matches one of the three patterns (true). Since we only consider shape, blockers are ignored.

#include <utility>
#include <cctype>
#include <cstdlib>

// Returns true if the queen's move shape from start to end is legal.
// start and end are pairs of (column, row) where column is 'a'..'h' (case-insensitive)
// and row is '1'..'8'. The function checks only the geometric shape, not path clearance.
bool isQueenLegalMove(std::pair<char, char> start, std::pair<char, char> end) {
    // Normalize column letters to lowercase for case-insensitive comparison
    char sc = std::tolower(static_cast<unsigned char>(start.first));
    char ec = std::tolower(static_cast<unsigned char>(end.first));

    // Compute absolute distances in columns and rows
    int colDist = std::abs(static_cast<int>(ec) - static_cast<int>(sc));
    int rowDist = std::abs(static_cast<int>(end.second) - static_cast<int>(start.second));

    // No move to the same square is allowed
    if (colDist == 0 && rowDist == 0) {
        return false;
    }

    // Legal if horizontal (colDist == 0), vertical (rowDist == 0), or diagonal (colDist == rowDist)
    return (colDist == 0) || (rowDist == 0) || (colDist == rowDist);
}

#include <cassert>

int main() {
    // Same square is illegal
    assert(!isQueenLegalMove({'a', '1'}, {'a', '1'}));
    assert(!isQueenLegalMove({'d', '4'}, {'d', '4'}));

    // Horizontal moves
    assert(isQueenLegalMove({'a', '1'}, {'h', '1'}));
    assert(isQueenLegalMove({'e', '5'}, {'a', '5'}));
    assert(isQueenLegalMove({'C', '3'}, {'A', '3'})); // case-insensitive

    // Vertical moves
    assert(isQueenLegalMove({'a', '1'}, {'a', '8'}));
    assert(isQueenLegalMove({'f', '8'}, {'f', '2'}));

    // Diagonal moves
    assert(isQueenLegalMove({'a', '1'}, {'h', '8'}));
    assert(isQueenLegalMove({'h', '1'}, {'a', '8'}));
    assert(isQueenLegalMove({'e', '4'}, {'b', '1'}));
    assert(isQueenLegalMove({'d', '5'}, {'g', '8'}));

    // Illegal moves (not horizontal, vertical, or diagonal)
    assert(!isQueenLegalMove({'a', '1'}, {'b', '3'}));
    assert(!isQueenLegalMove({'c', '2'}, {'e', '5'}));
    assert(!isQueenLegalMove({'a', '1'}, {'a', '3'} && false)); // placeholder, removed
}

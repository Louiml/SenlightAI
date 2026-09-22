/*
Write a C++ function `bool isLegalLoopInterchange(const std::vector<std::vector<char>>& depMatrix, unsigned innerLoopId, unsigned outerLoopId)` that determines whether it is legal to interchange two nested loops based on a direction dependence matrix. The matrix has one row per dependency, with each row containing exactly `N` direction characters (where `N` is the loop nest depth). Direction characters can be `'='` (loop-independent), `'<'` (positive direction), `'>'` (negative direction), `'S'` (scalar dependence), `'I'` (irrelevant beyond known depth), or `'*'` (unknown/ambiguous direction). The function must implement the theorem: a permutation of loops is legal if and only if after applying the same permutation to the matrix columns, no row has a `'>'` as the leftmost non-`'='`, non-`'S'`, non-`'I'` direction. The function receives the indices of the loops to swap (inner and outer), and must return `true` if the interchange is legal according to this theorem, `false` otherwise. Handle the edge case where either direction is `'*'` (immediately illegal), and properly interpret `'S'` and `'I'` as loop-independent dependencies that do not affect legality.
*/
#include <vector>
#include <cstddef>

// Check if the direction character is a non-trivial dependency (not '=', 'S', or 'I').
static bool isNonTrivial(char c) {
    return c != '=' && c != 'S' && c != 'I';
}

// Determine if interchanging loops at innerLoopId and outerLoopId is legal
// according to the dependence direction matrix theorem.
// depMatrix: each row is a vector of direction chars, one per loop level.
// innerLoopId, outerLoopId: indices of the two loops to swap.
bool isLegalLoopInterchange(const std::vector<std::vector<char>>& depMatrix,
                            unsigned innerLoopId, unsigned outerLoopId) {
    // For each dependency row, check legality after swapping the two columns.
    for (const auto& row : depMatrix) {
        // If either direction is unknown, we cannot prove legality.
        if (row[innerLoopId] == '*' || row[outerLoopId] == '*')
            return false;

        // We need to find the leftmost non-trivial direction after swapping
        // the two columns. Simulate the swap by processing the original row
        // but with positions innerLoopId and outerLoopId exchanged.
        // We scan columns left to right; at each step, determine which original
        // column maps to the current position.
        size_t n = row.size();
        bool foundFirst = false;
        for (size_t col = 0; col < n; ++col) {
            // Determine the original column index that maps to position 'col'
            // after the swap.
            size_t origCol;
            if (col == innerLoopId) origCol = outerLoopId;
            else if (col == outerLoopId) origCol = innerLoopId;
            else origCol = col;

            char dir = row[origCol];
            if (isNonTrivial(dir)) {
                // This is the leftmost non-trivial direction after swapping.
                if (dir == '>')
                    return false;  // illegal for this row
                foundFirst = true;
                break;
            }
        }
        // If no non-trivial direction found, the row is trivially legal.
        (void)foundFirst;
    }
    return true;
}
#include <cassert>
#include <vector>

// Function under test declaration (provided in solution).
bool isLegalLoopInterchange(const std::vector<std::vector<char>>& depMatrix,
                            unsigned innerLoopId, unsigned outerLoopId);

int main() {
    // Empty matrix: no dependencies -> legal.
    std::vector<std::vector<char>> empty;
    assert(isLegalLoopInterchange(empty, 0, 1));

    // Single row with all trivial dependencies.
    std::vector<std::vector<char>> trivial = {{'=', 'S', 'I'}};
    assert(isLegalLoopInterchange(trivial, 1, 0));

    // Single row with '<' as leftmost after swap -> legal.
    std::vector<std::vector<char>> rowLT = {{'=', '<', '='}};
    assert(isLegalLoopInterchange(rowLT, 1, 0)); // swap cols 1 and 0 -> '<' becomes at col0

    // Single row with '>' as leftmost after swap -> illegal.
    std::vector<std::vector<char>> rowGT = {{'=', '>', '='}};
    assert(!isLegalLoopInterchange(rowGT, 1, 0)); // after swap: '>' at col0, illegal

    // Case from snippet: innerDep='>', outerDep='=' and all before outer are '=' -> illegal.
    std::vector<std::vector<char>> row1 = {{'=', '=', '>'}};
    // Swap inner=2 (value '>'), outer=1 (value '='). After swap: row becomes {'=', '>', '='} -> '>' at col1, leftmost non-trivial at col1 -> illegal.
    assert(!isLegalLoopInterchange(row1, 2, 1));

    // Swap inner=1 (value '='), outer=2 (value '>') => after swap row: {'=', '>', '='}? Actually swap cols 1 and 2: original {0:'=',1:'=',2:'>'} -> {0:'=',1:'>',2:'='} -> leftmost non-trivial is '>' at col1 -> illegal.
    assert(!isLegalLoopInterchange(row1, 1, 2));

    // Case where leftmost non-trivial is '<' after swap -> legal.
    std::vector<std::vector<char>> rowLT2 = {{'<', '>', '='}};
    // Swap inner=1 (value '>'), outer=2 (value '=') -> after swap: { '<', '=', '>' } -> leftmost non-trivial '<' at col0 -> legal.
    assert(isLegalLoopInterchange(rowLT2, 1, 2));

    // Case with '*' in swapped position -> illegal.
    std::vector<std::vector<char>> rowStar = {{'=', '*', '='}};
    assert(!isLegalLoopInterchange(rowStar, 1, 0));

    // Multiple rows: all must be legal.
    std::vector<std::vector<char>> matrix = {
        {'=', '<', '='},
        {'<', '=', '>'}
    };
    // Swap inner=2, outer=1: row0 -> {'=', '=', '<'} legal; row1 -> {'<', '>', '='} legal; overall legal.
    assert(isLegalLoopInterchange(matrix, 2, 1));

    // Same matrix but swap inner=1, outer=0: row0 -> {'<', '=', '='} legal; row1 -> {'=', '<', '>'} legal; overall legal.
    assert(isLegalLoopInterchange(matrix, 1, 0));

    // Matrix where one row becomes illegal after swap.
    std::vector<std::vector<char>> matrixIllegal = {
        {'=', '>', '='},
        {'<', '=', '>'}
    };
    // Swap inner=2, outer=1: row0 -> {'=', '=', '>'} leftmost non-trivial '>' at col2 but before that all trivial -> legal? Actually leftmost non-trivial is at col2 and it's '>' -> illegal, so overall illegal.
    assert(!isLegalLoopInterchange(matrixIllegal, 2, 1));

    // Check outerLoopId=0 case with innerDep='>' -> illegal.
    std::vector<std::vector<char>> rowOuter0 = {{'>', '=', '='}};
    // Swap inner=1 (value '='), outer=0 (value '>') -> after swap: {'=', '>', '='} -> leftmost non-trivial '>' at col1 -> illegal.
    assert(!isLegalLoopInterchange(rowOuter0, 1, 0));

    // Case where innerDep='>' but there is a '<' before outerLoopId -> legal (because leftmost non-trivial after swap will be that '<').
    std::vector<std::vector<char>> rowWithEarlierLT = {{'<', '>', '='}};
    // Swap inner=2 (value '>'), outer=1 (value '=') -> after swap: {'<', '=', '>'} -> leftmost non-trivial '<' -> legal.
    assert(isLegalLoopInterchange(rowWithEarlierLT, 2, 1));

    return 0;
}
// The algorithm directly implements the legality condition from the code snippet. For each row in the dependency matrix, we extract the direction characters at the two column indices being swapped (`innerDep` and `outerDep`). If either is `'*'`, we cannot prove legality, so return `false`. Otherwise, we apply the column swap conceptually: after swapping, the character that was at `outerLoopId` moves to `innerLoopId` and vice versa. The legality condition requires that after this swap, no row has `'>'` as the leftmost non-`'='`/`'S'`/`'I'` dependency. 
//
// To check this efficiently, we can reason about the original row. The swap affects only the positions at `innerLoopId` and `outerLoopId`. The leftmost non-trivial direction in the swapped row is determined by scanning the original row from left to right, but with the two positions interchanged. A simpler equivalent check is: 
// - If the original row has any `'*'` in either swapped position, illegal.
// - Otherwise, define the "first meaningful direction" (first character that is not `'='`, `'S'`, or `'I'`) in the swapped row. This can be found by iterating through the original row with the positions swapped. If that first meaningful direction is `'>'`, the interchange is illegal for this row; otherwise it's legal for that row.
// - The overall legality requires every row to be legal.
//
// An alternative direct approach (matching the reference code) is to use `validDepInterchange` logic: 
// - If `innerDep == '>'` and `outerDep == '<'` or `'='`/`'S'`/`'I'`, the swap may be illegal depending on the leftmost non-trivial before `outerLoopId`. Specifically, if `outerLoopId == 0` and `innerDep == '>'`, illegal; if all directions before `outerLoopId` are `'='`,`'S'`,`'I'` and `innerDep == '>'`, illegal.
// - If `innerDep == '>'` and `outerDep == '>'`, legal only if the outermost dependency before `outerLoopId` is not `'>'` (i.e., `isOuterMostDepPositive` check).
// - If `innerDep == '<'` or `'='`/`'S'`/`'I'`, always legal for that row.
// - Also if `innerDep == outerDep`, legal.
//
// We can implement a clean, self-contained version that directly simulates the column swap and checks the leftmost non-trivial direction. Time complexity is O(rows × depth) since for each row we might scan up to the outer column to find the leftmost meaningful direction; space is O(1) extra. Edge cases: empty matrix (no dependencies) → legal; rows with only trivial directions → legal; `'*'` in either swapped position → illegal immediately.

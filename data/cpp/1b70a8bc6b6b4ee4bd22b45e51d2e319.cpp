/*
Write a standalone C++ function that models a simplified loop interchange legality check inspired by LLVM's loop interchange pass. The function should take a dependency matrix represented as a `std::vector<std::vector<char>>`, where each row describes dependencies between two memory operations across multiple loop levels, and each character is one of `'<'`, `'>'`, `'='`, `'S'`, `'I'`, or `'*'` (same semantics as in the LLVM code). The function also takes two indices `innerLoopId` and `outerLoopId` (with `innerLoopId > outerLoopId`). It should determine whether swapping the inner loop with the outer loop is legal according to the theorem: after applying the same column permutation to the direction matrix, no row may have a `'>'` as the leftmost non-`'='`, non-`'S'`, non-`'I'` entry. If any row contains `'*'` in either of the two columns being swapped, the interchange is illegal. Implement the helper functions `isOuterMostDepPositive` and `containsNoDependence` as described, and use `validDepInterchange` to check each row. The function should be `const`-correct and efficient.
*/
#include <vector>

// Returns true if the leftmost non-'=', non-'S', non-'I' entry in row up to and including column is '>'.
bool isOuterMostDepPositive(const std::vector<std::vector<char>>& DepMatrix, unsigned Row, unsigned Column) {
    for (unsigned i = 0; i <= Column; ++i) {
        if (DepMatrix[Row][i] == '<')
            return false;
        if (DepMatrix[Row][i] == '>')
            return true;
    }
    return false;
}

// Returns true if all entries in row before column are '=', 'S', or 'I'.
bool containsNoDependence(const std::vector<std::vector<char>>& DepMatrix, unsigned Row, unsigned Column) {
    for (unsigned i = 0; i < Column; ++i) {
        char d = DepMatrix[Row][i];
        if (d != '=' && d != 'S' && d != 'I')
            return false;
    }
    return true;
}

// Checks if a single row permits the interchange given the inner and outer dependency characters.
bool validDepInterchange(const std::vector<std::vector<char>>& DepMatrix, unsigned Row,
                         unsigned OuterLoopId, char InnerDep, char OuterDep) {
    if (isOuterMostDepPositive(DepMatrix, Row, OuterLoopId))
        return false;

    if (InnerDep == OuterDep)
        return true;

    if (InnerDep == '=' || InnerDep == 'S' || InnerDep == 'I')
        return true;

    if (InnerDep == '<')
        return true;

    if (InnerDep == '>') {
        if (OuterLoopId == 0)
            return false;
        if (!containsNoDependence(DepMatrix, Row, OuterLoopId))
            return true;
    }

    return false;
}

// Determines whether swapping the loop at innerLoopId with the loop at outerLoopId
// is legal based on the direction matrix theorem.
bool isLegalToInterchangeLoops(const std::vector<std::vector<char>>& DepMatrix,
                               unsigned InnerLoopId, unsigned OuterLoopId) {
    for (const auto& Row : DepMatrix) {
        char InnerDep = Row[InnerLoopId];
        char OuterDep = Row[OuterLoopId];
        if (InnerDep == '*' || OuterDep == '*')
            return false;
        unsigned RowIndex = static_cast<unsigned>(&Row - &DepMatrix[0]);
        if (!validDepInterchange(DepMatrix, RowIndex, OuterLoopId, InnerDep, OuterDep))
            return false;
    }
    return true;
}
#include <cassert>
#include <vector>

// Assume the solution function declarations are included above.

int main() {
    // Empty matrix: trivially legal.
    std::vector<std::vector<char>> empty;
    assert(isLegalToInterchangeLoops(empty, 1, 0) == true);

    // No dependencies: all '='.
    std::vector<std::vector<char>> noDep = {{'=', '='}};
    assert(isLegalToInterchangeLoops(noDep, 1, 0) == true);

    // Inner '<' should be legal.
    std::vector<std::vector<char>> innerLess = {{'=', '<'}};
    assert(isLegalToInterchangeLoops(innerLess, 1, 0) == true);

    // Inner '>' with outer '=' at outermost: illegal.
    std::vector<std::vector<char>> innerGreaterOuterEq = {{'=', '>'}};
    assert(isLegalToInterchangeLoops(innerGreaterOuterEq, 1, 0) == false);

    // Inner '>' but outer not outermost and no prior dep: illegal.
    std::vector<std::vector<char>> innerGreaterNoPrior = {{'=', '=', '>'}};
    assert(isLegalToInterchangeLoops(innerGreaterNoPrior, 2, 1) == false);

    // Inner '>' with prior '<' before outer: legal.
    std::vector<std::vector<char>> innerGreaterWithPrior = {{'<', '=', '>'}};
    assert(isLegalToInterchangeLoops(innerGreaterWithPrior, 2, 1) == true);

    // Unknown direction in either column: illegal.
    std::vector<std::vector<char>> withStar = {{'*', '='}};
    assert(isLegalToInterchangeLoops(withStar, 1, 0) == false);

    // Both directions same '>': legal (since inner == outer).
    std::vector<std::vector<char>> bothGreater = {{'>', '>'}};
    assert(isLegalToInterchangeLoops(bothGreater, 1, 0) == true);

    // Mixed rows: one legal, one illegal -> overall illegal.
    std::vector<std::vector<char>> mixed = {{'=', '<'}, {'=', '>'}};
    assert(isLegalToInterchangeLoops(mixed, 1, 0) == false);

    // Multiple rows all legal.
    std::vector<std::vector<char>> multiLegal = {{'<', '='}, {'=', '<'}, {'=', '='}};
    assert(isLegalToInterchangeLoops(multiLegal, 1, 0) == true);

    return 0;
}
// The solution mirrors the logic in the LLVM `isLegalToInterChangeLoops` function. For each row in the dependency matrix, we extract the characters at the inner and outer loop columns. If either is `'*'`, the interchange is immediately illegal because the direction is unknown. Otherwise, we apply the `validDepInterchange` logic: first check if the outermost non-`'='/'S'/'I'` dependency before or at the outer column is `'>'`; if so, illegal. If the inner dependency equals the outer dependency, it is legal. If the inner dependency is `'='`, `'S'`, or `'I'`, it is legal. If the inner dependency is `'<'`, it is legal. If the inner dependency is `'>'`, then if the outer column is index 0, illegal; otherwise, if there is any non-trivial dependency before the outer column, it is legal (because after swapping, the leftmost non-`'='` becomes `'<'` or depends on earlier rows); if no dependency exists before the outer column, it is illegal. The time complexity is O(rows × outerLoopId) in the worst case due to scanning prefixes for each row, and O(1) extra space beyond input storage. The algorithm processes each row independently, so it naturally handles edge cases like empty matrices (returns true), single-row matrices, and rows with only trivial dependencies.

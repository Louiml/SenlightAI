/*
Write a C++ function that takes two non-negative integers `rows` and `cols` and returns the total number of blank cells in a rectangular grid with `rows` rows and `cols` columns, where the first row contains only one blank cell (at position 1) and every other cell in that row is filled, and every subsequent row contains only one blank cell (at the same position 1) with all other cells filled. The grid is considered to have at least 2 rows and at least 1 column. Specifically, for a grid of size `N` rows × `M` columns, the total number of blank cells is `(N - 1) + N * (M - 1)`. Your function should compute and return this value as an `int` without using any loops or recursion.
*/

// Return total number of blank cells in a grid with rows*cols cells,
// where each row has exactly one blank cell (at column 1) and all other cells are filled.
int totalBlankCells(const int rows, const int cols) {
    // First row contributes 1 blank, but formula is (rows-1) + rows*(cols-1)
    // which equals rows*cols - (cols-1) - 1? Actually compute directly.
    return (rows - 1) + rows * (cols - 1);
}

int main() {
    // Direct tests from the snippet's arithmetic
    assert(computeGridExpression(3, 4) == 11);  // (3-1)+3*(4-1)=2+9=11
    assert(computeGridExpression(1, 1) == 0);   // (0)+1*0=0, and also 1*1-1=0
    assert(computeGridExpression(5, 2) == 9);   // (4)+5*1=9
    assert(computeGridExpression(0, 7) == -1);  // (-1)+0*6=-1
    assert(computeGridExpression(-2, 3) == -7); // (-3)+(-2)*2=-3-4=-7
    assert(computeGridExpression(10, 1) == 9);  // (9)+10*0=9
    assert(computeGridExpression(2, 5) == 9);   // (1)+2*4=9
    assert(computeGridExpression(7, 3) == 20);  // (6)+7*2=6+14=20
    assert(computeGridExpression(4, 4) == 15);  // (3)+4*3=3+12=15
    // Also test that simplified form matches: N*M-1
    assert(computeGridExpression(100, 100) == 100*100 - 1);
    return 0;
}

// The problem reduces to a simple arithmetic expression. For a grid with `N` rows and `M` columns:
// - Each of the `N` rows has exactly one blank cell in column 1, so there are `N` blank cells from that column. However, the first row’s blank cell is counted separately in the given formula, but we can interpret it directly: The first row contributes `1` blank (since the formula gives `T1 = N - 1` for rows 2..N, plus `T2 = N*(M-1)` for columns 2..M across all rows). The total is `(N-1) + N*(M-1)`.
// - Edge cases: If `N = 1`, then the formula gives `0 + 1*(M-1) = M-1`, which matches: only one row, one blank in column 1, and all other cells are filled? Actually for N=1, the problem says at least 2 rows, but if not, the formula still works. For `M=1`, the formula gives `(N-1) + N*0 = N-1`, which means each row has one blank cell, and that’s correct. The formula works for all non-negative integers, but for `N=0` or `M=0`, the result would be negative? The problem states non-negative, but typical input is positive. We can assume `N >= 2` and `M >= 1` as specified. The solution uses integer arithmetic; no overflow for reasonable inputs (int range). Time complexity is O(1), space complexity O(1). No loops, recursion, or containers needed. The function should be `const` correct (though no mutable state, we can mark parameters as `const int`).

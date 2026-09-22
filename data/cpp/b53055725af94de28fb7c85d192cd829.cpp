// Given an `m x n` grid of characters, where each cell contains one of the four DNA bases `'A'`, `'G'`, `'T'`, or `'C'`, and given the constraint that the final output grid must consist of only two alternating row patterns repeated down the rows (i.e., rows 0,2,4,... are identical to one another, and rows 1,3,5,... are identical to the other pattern), write a C++ function `int maxMatchingCells(const std::vector<std::string>& grid)` that returns the maximum possible number of cells that match the original grid after you are allowed to replace all characters in the output grid with any valid DNA bases, subject to the alternating-row-pattern constraint. Additionally, the function should produce and store the output grid that achieves this maximum (e.g., by setting a global or reference parameter) so that it can be verified. The input grid will have at least 1 row and at least 1 column, and the characters are guaranteed to be exactly `'A'`, `'G'`, `'T'`, `'C'`. The solution must handle both even and odd numbers of rows/columns and must be efficient for grids up to 300,000 cells.
The problem reduces to choosing two DNA base strings (each of length `n`), call them `rowEven` and `rowOdd`, such that all even-indexed rows (0,2,4,...) use `rowEven` and all odd-indexed rows use `rowOdd`. For each column position `j`, the choices for `rowEven[j]` and `rowOdd[j]` are independent across columns, but the two values at the same column must be different? Wait, the original problem’s DP enforces that within a column, the two row patterns differ, but reading the problem statement above, there is no such constraint—it only says two alternating row patterns, which could be the same across a column. However, to be faithful to the original snippet, we enforce that in each column, the two row patterns must use different bases. Actually, looking at the original code, it uses four distinct bases and for each column it picks two different bases, one for even rows and one for odd rows, but that is only one possible solution. For the standalone task, we should keep the general problem: you may choose any two strings of length `n` over `{A,G,T,C}` (they can be equal or different) to alternate. The maximum matching count is the sum over columns of the maximum over pairs of bases `(x,y)` of `count_even[j][x] + count_odd[j][y]`, where `count_even[j][x]` is the number of even rows that have base `x` at column `j`, and similarly for odd. This is a per-column independent optimization, so we can compute for each column the best pair (which might be the same base if that gives a higher sum, but the problem doesn’t forbid it). The answer is the sum of these column-wise maxima. For the output grid, simply assign each column the chosen bases. This approach is O(m*n) time to count and O(n) additional space. Edge cases: when there is only one row, all rows are even, and the odd pattern is unused; we still pick the best base per column. When there is only one column, the same logic applies. The solution must handle grids up to 300,000 cells efficiently.
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <cassert>

// Given a grid of DNA bases, return the maximum number of matches achievable
// by replacing the grid with two alternating rows (row 0 pattern for even rows,
// row 1 pattern for odd rows). Also fill 'output' with the best grid.
int maxMatchingCells(const std::vector<std::string>& grid, std::vector<std::string>& output) {
    const int m = static_cast<int>(grid.size());
    const int n = static_cast<int>(grid[0].size());
    output.assign(m, std::string(n, 'A'));  // initialize to 'A'
    
    // Count per column: for each column j and each base b (0..3 for A,G,T,C),
    // countEven[j][b] = number of even rows with grid[i][j] == b
    // countOdd[j][b] = number of odd rows with that base.
    std::vector<std::array<int,4>> countEven(n);
    std::vector<std::array<int,4>> countOdd(n);
    for (int j = 0; j < n; ++j) {
        countEven[j] = {0,0,0,0};
        countOdd[j] = {0,0,0,0};
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int idx = 0;
            char c = grid[i][j];
            if (c == 'G') idx = 1;
            else if (c == 'T') idx = 2;
            else if (c == 'C') idx = 3;
            if (i % 2 == 0) countEven[j][idx]++;
            else countOdd[j][idx]++;
        }
    }
    
    int totalMatches = 0;
    const std::string bases = "AGTC";  // order matches indices 0,1,2,3
    for (int j = 0; j < n; ++j) {
        // Try all 4*4 = 16 possible pairs (x,y) for even and odd rows.
        int bestMatch = -1;
        int bestX = 0, bestY = 0;
        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                int cur = countEven[j][x] + countOdd[j][y];
                if (cur > bestMatch) {
                    bestMatch = cur;
                    bestX = x;
                    bestY = y;
                }
            }
        }
        totalMatches += bestMatch;
        for (int i = 0; i < m; ++i) {
            output[i][j] = (i % 2 == 0) ? bases[bestX] : bases[bestY];
        }
    }
    
    return totalMatches;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (assume it's included).

int main() {
    // Test 1: 2x2 grid, best is to use "AG" for even rows and "CT" for odd rows? Let's compute manually.
    std::vector<std::string> g1 = {"AA", "TT"};
    std::vector<std::string> out1;
    int ans1 = maxMatchingCells(g1, out1);
    // Even row pattern best: column0 A (matches row0), column1 A (matches row0) => 2. Odd row pattern: column0 T, column1 T => 2. Total 4.
    assert(ans1 == 4);
    // Verify output matches grid exactly? In this case, output can be same as input.
    assert(out1 == g1);

    // Test 2: 1x3 grid, only even rows exist.
    std::vector<std::string> g2 = {"AGT"};
    std::vector<std::string> out2;
    int ans2 = maxMatchingCells(g2, out2);
    // Best per column is the existing character, so ans = 3.
    assert(ans2 == 3);
    assert(out2 == g2);  // output should match exactly

    // Test 3: 3x1 grid, only one column.
    std::vector<std::string> g3 = {"A", "G", "A"};
    std::vector<std::string> out3;
    int ans3 = maxMatchingCells(g3, out3);
    // Even rows: indices 0,2 have A (2 matches). Odd row: index1 has G (1 match). Total = 3.
    assert(ans3 == 3);
    // output should be "A", "G", "A" exactly.
    assert(out3 == g3);

    // Test 4: All characters same in a 2x2, e.g., all 'C'.
    std::vector<std::string> g4 = {"CC", "CC"};
    std::vector<std::string> out4;
    int ans4 = maxMatchingCells(g4, out4);
    assert(ans4 == 4);
    // output can be all 'C'.
    for (const auto& s : out4) {
        for (char c : s) assert(c == 'C');
    }

    // Test 5: 2x3 grid with mixed.
    std::vector<std::string> g5 = {"AGC", "TGC"};
    std::vector<std::string> out5;
    int ans5 = maxMatchingCells(g5, out5);
    // Column0: even has A, odd has T => max pair gives 2 (A for even, T for odd). 
    // Column1: both rows have G => even G + odd G = 2. Column2: even C, odd C => 2. Total 6.
    assert(ans5 == 6);
    // Verify output matches input.
    assert(out5 == g5);

    // Test 6: 3x3 grid where optimal is not the original.
    std::vector<std::string> g6 = {"AAA", "GGG", "AAA"};
    // Even rows: row0 and row2 both have A at all columns. Odd row: row1 has G. 
    // For each column, even count for A = 2, odd count for G = 1. Choose A for even, G for odd => 3 matches per column => total 9.
    std::vector<std::string> out6;
    int ans6 = maxMatchingCells(g6, out6);
    assert(ans6 == 9);
    // The output should be exactly "AAA", "GGG", "AAA".
    assert(out6 == g6);

    // Test 7: Edge case m=1, n=1.
    std::vector<std::string> g7 = {"C"};
    std::vector<std::string> out7;
    int ans7 = maxMatchingCells(g7, out7);
    assert(ans7 == 1);
    assert(out7[0] == "C");
    
    // Test 8: Large grid with impossible to match all, but answer counts correct.
    std::vector<std::string> g8 = {"AT", "GC", "AT"};
    // Even rows: row0 and row2 have A,T. Odd row: row1 has G,C. 
    // Column0: even A count=2, odd G count=1 => best pair (A,G)=3. Column1: even T count=2, odd C count=1 => best pair (T,C)=3. Total 6.
    std::vector<std::string> out8;
    int ans8 = maxMatchingCells(g8, out8);
    assert(ans8 == 6);

    // All tests passed.
    return 0;
}

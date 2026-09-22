// Create a C++ function that simulates a simplified match-3 candy grid. The function should take a square character grid (as a 2D array) and its size (`n`, where `n` is either 8 or 10) and modify the grid in-place by removing any three or more identical characters that appear consecutively in any row, column, or main diagonal (both forward and backward diagonals, considering 3-cell segments). When a match is found, replace those cells with a space character `' '` to indicate removal. The function should return the total score earned, where a horizontal or vertical run of 3 gives 10 points, a diagonal run of 3 gives 15 points, and an L-shaped pattern (5 cells: three vertical and two horizontal extending from the bottom of the vertical) gives 25 points. Only consider matches that are exactly 3 cells (do not handle longer runs or overlapping matches — process each cell as a starting point in row-major order, and if a match is found, clear those cells and continue). The grid is guaranteed to contain only the characters `'@'`, `'#'`, `'$'`, `'%'`, `'&'` (and possibly `'*'` and `'?'` for size 10). The function must not assume any external headers beyond the standard ones, and must be const-correct where appropriate.
The solution involves iterating through each cell of the grid in row-major order. For each cell, check five possible match patterns starting at that cell (as the top-left or top-most point):
1. Horizontal: cells `(i,j)`, `(i,j+1)`, `(i,j+2)` — requires `j+2 < n`.
2. Vertical: cells `(i,j)`, `(i+1,j)`, `(i+2,j)` — requires `i+2 < n`.
3. Forward diagonal: cells `(i,j)`, `(i+1,j+1)`, `(i+2,j+2)` — requires `i+2 < n` and `j+2 < n`.
4. Backward diagonal: cells `(i,j)`, `(i+1,j-1)`, `(i+2,j-2)` — requires `i+2 < n` and `j>=2`.
5. L-shape: cells `(i,j)`, `(i+1,j)`, `(i+2,j)`, `(i+2,j+1)`, `(i+2,j+2)` — requires `i+2 < n` and `j+2 < n`, and all five cells equal.

For each pattern, first check bounds, then check that all involved cells are non-space and equal to the starting cell’s character. If true, set all involved cells to `' '` and add the appropriate points to the score. After clearing, continue to the next cell; do not re-check cleared cells for other patterns in the same pass. Important edge cases: bounds checking must be done before accessing any array element to avoid out-of-range access. The L-shape pattern has higher priority than vertical or horizontal alone, so check it first when applicable. Also, since we clear cells during the iteration, later cells may become space, but we still check them; however, since we only look at the starting cell’s character, if it’s already space we skip it (the character is space, which won’t match any pattern). Time complexity is O(n^2) because we visit each cell once and check a constant number of patterns per cell. Space complexity is O(1) auxiliary.
#include <cstddef>

// Remove 3-cell matches from a square grid and return the score.
// Grid size n is either 8 or 10. Matches: horizontal/vertical=10, diagonal=15, L-shape=25.
int clearMatches(char grid[][10], int n) {
    int score = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char current = grid[i][j];
            if (current == ' ') continue;

            // L-shape: vertical of 3 at (i,j),(i+1,j),(i+2,j) plus (i+2,j+1),(i+2,j+2)
            if (i + 2 < n && j + 2 < n &&
                grid[i][j] == current &&
                grid[i + 1][j] == current &&
                grid[i + 2][j] == current &&
                grid[i + 2][j + 1] == current &&
                grid[i + 2][j + 2] == current) {
                grid[i][j] = ' ';
                grid[i + 1][j] = ' ';
                grid[i + 2][j] = ' ';
                grid[i + 2][j + 1] = ' ';
                grid[i + 2][j + 2] = ' ';
                score += 25;
                continue; // skip other patterns for this starting cell
            }

            // Vertical
            if (i + 2 < n &&
                grid[i][j] == current &&
                grid[i + 1][j] == current &&
                grid[i + 2][j] == current) {
                grid[i][j] = ' ';
                grid[i + 1][j] = ' ';
                grid[i + 2][j] = ' ';
                score += 10;
                continue;
            }

            // Horizontal
            if (j + 2 < n &&
                grid[i][j] == current &&
                grid[i][j + 1] == current &&
                grid[i][j + 2] == current) {
                grid[i][j] = ' ';
                grid[i][j + 1] = ' ';
                grid[i][j + 2] = ' ';
                score += 10;
                continue;
            }

            // Forward diagonal
            if (i + 2 < n && j + 2 < n &&
                grid[i][j] == current &&
                grid[i + 1][j + 1] == current &&
                grid[i + 2][j + 2] == current) {
                grid[i][j] = ' ';
                grid[i + 1][j + 1] = ' ';
                grid[i + 2][j + 2] = ' ';
                score += 15;
                continue;
            }

            // Backward diagonal
            if (i + 2 < n && j >= 2 &&
                grid[i][j] == current &&
                grid[i + 1][j - 1] == current &&
                grid[i + 2][j - 2] == current) {
                grid[i][j] = ' ';
                grid[i + 1][j - 1] = ' ';
                grid[i + 2][j - 2] = ' ';
                score += 15;
            }
        }
    }
    return score;
}
#include <cassert>
#include <iostream>

// Function under test is declared here (for the test file, include the solution above or a header).
int clearMatches(char grid[][10], int n);

int main() {
    // Test 1: Simple horizontal match of 3
    char g1[10][10] = {{'@','@','@','$', 0,0,0,0,0,0}, /* fill rest with 0 automatically */};
    // But our function requires 10 columns, so we need to initialize fully. Let's use size 8 with a smaller array.
    // Since the function expects a 10-column array, we'll allocate a 10x10 and only use first n rows/cols.
    char grid1[10][10] = {};
    grid1[0][0]='@'; grid1[0][1]='@'; grid1[0][2]='@';
    grid1[0][3]='$';
    int s1 = clearMatches(grid1, 4); // n=4 not allowed, but we use n=8 for full check? Actually n must be 8 or 10, but tests can use n=8.
    // Redefine properly:
    char a[10][10] = {};
    a[0][0]='@'; a[0][1]='@'; a[0][2]='@';
    int scoreA = clearMatches(a, 8);
    assert(scoreA == 10);
    assert(a[0][0]==' ' && a[0][1]==' ' && a[0][2]==' ');

    // Test 2: Vertical match
    char b[10][10] = {};
    b[2][5]='#'; b[3][5]='#'; b[4][5]='#';
    int scoreB = clearMatches(b, 8);
    assert(scoreB == 10);
    assert(b[2][5]==' ' && b[3][5]==' ' && b[4][5]==' ');

    // Test 3: Forward diagonal match
    char c[10][10] = {};
    c[1][1]='$'; c[2][2]='$'; c[3][3]='$';
    int scoreC = clearMatches(c, 8);
    assert(scoreC == 15);
    assert(c[1][1]==' ' && c[2][2]==' ' && c[3][3]==' ');

    // Test 4: Backward diagonal match
    char d[10][10] = {};
    d[1][6]='&'; d[2][5]='&'; d[3][4]='&';
    int scoreD = clearMatches(d, 8);
    assert(scoreD == 15);
    assert(d[1][6]==' ' && d[2][5]==' ' && d[3][4]==' ');

    // Test 5: L-shape match
    char e[10][10] = {};
    e[0][0]='*'; e[1][0]='*'; e[2][0]='*'; e[2][1]='*'; e[2][2]='*';
    int scoreE = clearMatches(e, 8);
    assert(scoreE == 25);
    for(int i=0;i<3;i++) for(int j=0;j<3;j++) if(!(i==0&&j==1) && !(i==0&&j==2) && !(i==1&&j==1) && !(i==1&&j==2)) assert(e[i][j]==' ');

    // Test 6: No matches
    char f[10][10] = {};
    f[0][0]='@'; f[0][1]='#'; f[0][2]='$';
    f[1][0]='%'; f[1][1]='&'; f[1][2]='@';
    f[2][0]='#'; f[2][1]='$'; f[2][2]='%';
    int scoreF = clearMatches(f, 3); // n=3 not allowed, but function uses n, we can pass 3 for testing logic? Actually n should be 8. Use n=8 with only those cells filled.
    // Better: create 8x8 with no matches
    char g[10][10] = {};
    g[0][0]='@'; g[0][1]='#'; g[0][2]='$';
    g[1][0]='%'; g[1][1]='&'; g[1][2]='@';
    g[2][0]='#'; g[2][1]='$'; g[2][2]='%';
    int scoreG = clearMatches(g, 8);
    assert(scoreG == 0);

    // Test 7: Overlapping matches processed once
    char h[10][10] = {};
    h[0][0]='@'; h[1][0]='@'; h[2][0]='@'; h[3][0]='@'; // vertical 4 cells but only first 3 counted
    int scoreH = clearMatches(h, 8);
    assert(scoreH == 10);
    assert(h[0][0]==' ' && h[1][0]==' ' && h[2][0]==' ' && h[3][0]=='@');

    // Test 8: Multiple separate matches
    char k[10][10] = {};
    k[0][0]='#'; k[0][1]='#'; k[0][2]='#';
    k[1][3]='$'; k[2][3]='$'; k[3][3]='$';
    int scoreK = clearMatches(k, 8);
    assert(scoreK == 20);
    assert(k[0][0]==' ' && k[1][3]==' ' && k[2][3]==' ' && k[3][3]==' ');

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

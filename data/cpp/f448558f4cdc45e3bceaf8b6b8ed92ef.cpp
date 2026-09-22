Write a C++ function `int solveDominoTiling(int n, int m, const std::vector<std::string>& board, std::vector<std::string>& solution)` that, given a rectangular grid of size `n x m` (1 ≤ n,m ≤ 50) containing characters `'.'` for empty cells and lowercase letters `'a'`–`'d'` for cells that must be covered by exactly one 2×2 tile, determines whether the board can be tiled completely using 2×2 square tiles, where each tile is assigned a color from 0 to 6 (i.e., 7 distinct colors). A tile may cover a 2×2 block of cells only if all four cells in that block are non-`.` and not already covered by another tile; a tile may cover cells of different letters, and a letter may appear in more than one tile. The constraint is that for any two distinct letters that appear together in the same tile, that unordered pair of letters can appear together in at most one tile across the entire board. Additionally, each tile color can be used at most twice. If a valid tiling exists, the function must store in `solution` an `n x m` grid where each cell that was originally a letter is replaced by a digit character `'0'`–`'6'` representing the color of the tile covering that cell, and each originally empty cell remains `'.'`. The function returns the total number of distinct valid tilings modulo 5040 (note: the reference algorithm multiplies the count by 5040 because tile colors are permuted; simplify: count each color assignment as a distinct tiling, and return that exact count modulo 5040). If no tiling exists, return 0 and leave `solution` as a grid of `'.'` for all cells. The function must handle multiple calls independently.
// The problem is a constrained exact-cover tiling problem. The grid is small (n,m ≤ 50), but the number of possible 2×2 tile placements is at most (n-1)*(m-1). The key is to perform a depth-first search over the grid in row-major order, placing tiles only at top-left corners (x,y) where the 2×2 block is entirely non-`.` and not yet covered. For each such placement, we try assigning one of up to 7 colors (0 to 6). We must enforce two conditions: (1) a color cannot be used more than twice across all tiles; (2) for any pair of distinct letters that appear together in the same tile, that unordered pair cannot appear together in more than one tile. We maintain three global arrays: `cnt[c]` counts how many tiles currently use color c (initialize 0, increment when placing, decrement when backtracking); `used[p][q]` counts how many tiles currently contain the unordered pair of letters (p,q) where p and q are from the set {a..d} mapped to 0..3; and `dom[letter][pair_index]` stores the two colors assigned to each letter so far. However, a simpler approach: for each tile, we can track which letters are inside it (the four characters), and for each pair of distinct letters inside that tile, increment a counter `pairCount[letter1][letter2]`. When backtracking, decrement. To handle colors, we also track `colorCount[c]`. Because colors are distinguishable (a tile color is a digit in the output), each placement of a tile with a different color is a distinct tiling. Note: the original snippet multiplies by 5040 (7!) because it treats colors as permutations only if all colors are used; but in this task we require exact counts, so we return the raw count modulo 5040. We must ensure that when there are multiple identical letters, they are allowed in the same tile; pairs are only considered for distinct letters. For example, if a tile contains letters "a a b c", then pairs are (a,b) and (a,c) and (b,c). The function must be deterministic and produce one valid solution (arbitrary) when the count > 0. Edge cases: boards with no letters (all '.') return 1 with all '.' solution (empty tiling? But tiles must cover 2×2 blocks, so if there are letters, each letter must be covered; if no letters, there are no tiles, and one trivial tiling exists? The original code starts with (0,0) and if all cells are '.' it will reach the end and output all '.' and return 1*5040. So we adopt that: if no letters exist, return 1 and solution is all '.'. Also, boards with isolated letters (e.g., a single letter not fitting in a 2×2 block) result in 0. Time complexity: In the worst case, each placement tries up to 7 colors, and the number of valid placements is O(n*m). Branching is limited by constraints, but theoretical worst-case exponential; for n,m ≤ 50, typical inputs are sparse. Space complexity: O(n*m) for visited and solution, O(1) for counters.
#include <vector>
#include <string>
#include <cstring>

// Counts distinct tilings of a board using 2x2 tiles with color constraints.
// board: n x m grid with '.' or lowercase 'a'-'d'.
// solution: output grid where letters replaced by '0'-'6' or '.' for empty.
// Returns number of distinct color assignments modulo 5040.
int solveDominoTiling(int n, int m, const std::vector<std::string>& board,
                      std::vector<std::string>& solution) {
    const int MOD = 5040;
    // Initialize solution with all '.'.
    solution.assign(n, std::string(m, '.'));

    // If there are no letters, one empty tiling exists.
    bool hasLetter = false;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (board[i][j] != '.') { hasLetter = true; break; }
    if (!hasLetter) return 1;

    // Visited cells.
    std::vector<std::vector<bool>> vis(n, std::vector<bool>(m, false));

    // Color use counts (0..6).
    int colorCnt[7] = {0};

    // Pair counts for distinct letters (a=0,b=1,c=2,d=3).
    int pairCnt[4][4] = {{0}};

    // Letter-to-color mapping: each letter appears in tiles; we store up to two colors.
    // Since a letter may appear in multiple tiles, we need to keep track of which
    // colors are used for that letter? Actually the original code maintained dom
    // for each cell (each cell's letter gets a color). But a letter can appear
    // in multiple tiles, each tile has its own color. The constraint is only
    // about pairs of letters in the same tile. So we don't need to globally
    // bind a letter to a single color. So we can ignore dom.
    // The only constraint is color count <=2 and pair count <=1.

    // Recursive search function.
    int ans = 0;
    // Placeholder for recursive lambda.
    // We'll implement as a separate function via a struct or std::function.

    // We use a recursive function inside.
    std::function<void(int,int)> go = [&](int x, int y) {
        // Skip to next cell.
        if (y == m) { x++; y = 0; }
        if (x == n) {
            // Found a valid tiling: copy solution.
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j)
                    if (board[i][j] != '.') {
                        // The color is stored in a separate array.
                        // We'll use a separate grid to store colors.
                    }
            ans++;
            return;
        }
        // If cell is empty or already visited, move on.
        if (board[x][y] == '.' || vis[x][y]) {
            go(x, y+1);
            return;
        }
        // Check if 2x2 block fits.
        if (x+1 >= n || y+1 >= m) return;
        if (board[x+1][y] == '.' || board[x][y+1] == '.' || board[x+1][y+1] == '.') return;
        // Mark block visited.
        vis[x][y] = vis[x+1][y] = vis[x][y+1] = vis[x+1][y+1] = true;
        // Get the letters in this block.
        int letters[4];
        letters[0] = board[x][y] - 'a';
        letters[1] = board[x+1][y] - 'a';
        letters[2] = board[x][y+1] - 'a';
        letters[3] = board[x+1][y+1] - 'a';
        // Try each color.
        for (int c = 0; c < 7; ++c) {
            if (colorCnt[c] >= 2) continue;
            // Check pair constraints.
            bool ok = true;
            // Generate all distinct pairs.
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    if (pairCnt[l1][l2] > 0) { ok = false; break; }
                }
            if (!ok) continue;
            // Place tile.
            colorCnt[c]++;
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    pairCnt[l1][l2]++;
                }
            // Record color in a temporary grid.
            // We'll use a separate grid to store final colors.
            // For simplicity, we store a color grid.
            // Actually we need to fill solution at the end. We'll use a global colorGrid.
            // Since we are in a lambda, we can declare a local static? Better pass it.
            // We'll use a member of the function scope? Actually we can use a vector< vector<int> >.
            // Let's define a colorGrid outside.
            // We'll modify solution at the end.
            // For now we just recurse.
            go(x, y+1);
            // Backtrack.
            colorCnt[c]--;
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    pairCnt[l1][l2]--;
                }
        }
        vis[x][y] = vis[x+1][y] = vis[x][y+1] = vis[x+1][y+1] = false;
    };

    // We need to store colors during recursion. Let's restructure:
    // We'll keep a colorGrid of size n x m, initialized to -1.
    // On placing a tile, set the 4 cells to c.
    // On backtrack, set back to -1.
    // At the end, when x==n, copy colorGrid to solution.

    // Reimplement with a cleaner approach.
    std::vector<std::vector<int>> colorGrid(n, std::vector<int>(m, -1));
    int finalCount = 0;

    // Define recursive function using std::function.
    std::function<void(int,int)> dfs = [&](int x, int y) {
        if (y == m) { x++; y = 0; }
        if (x == n) {
            // Valid tiling found.
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < m; ++j) {
                    if (board[i][j] == '.') solution[i][j] = '.';
                    else solution[i][j] = char('0' + colorGrid[i][j]);
                }
            finalCount++;
            return;
        }
        if (board[x][y] == '.' || vis[x][y]) {
            dfs(x, y+1);
            return;
        }
        if (x+1 >= n || y+1 >= m) return;
        if (board[x+1][y] == '.' || board[x][y+1] == '.' || board[x+1][y+1] == '.') return;
        vis[x][y] = vis[x+1][y] = vis[x][y+1] = vis[x+1][y+1] = true;
        int letters[4];
        letters[0] = board[x][y] - 'a';
        letters[1] = board[x+1][y] - 'a';
        letters[2] = board[x][y+1] - 'a';
        letters[3] = board[x+1][y+1] - 'a';
        for (int c = 0; c < 7; ++c) {
            if (colorCnt[c] >= 2) continue;
            bool ok = true;
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    if (pairCnt[l1][l2] > 0) { ok = false; break; }
                }
            if (!ok) continue;
            colorCnt[c]++;
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    pairCnt[l1][l2]++;
                }
            colorGrid[x][y] = colorGrid[x+1][y] = colorGrid[x][y+1] = colorGrid[x+1][y+1] = c;
            dfs(x, y+1);
            colorCnt[c]--;
            for (int a = 0; a < 4; ++a)
                for (int b = a+1; b < 4; ++b) {
                    int l1 = letters[a], l2 = letters[b];
                    if (l1 == l2) continue;
                    if (l1 > l2) std::swap(l1, l2);
                    pairCnt[l1][l2]--;
                }
            colorGrid[x][y] = colorGrid[x+1][y] = colorGrid[x][y+1] = colorGrid[x+1][y+1] = -1;
        }
        vis[x][y] = vis[x+1][y] = vis[x][y+1] = vis[x+1][y+1] = false;
    };

    dfs(0,0);
    // If no tiling, solution already all '.'.
    return finalCount % MOD;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// The function from the solution (declared here for test).
int solveDominoTiling(int n, int m, const std::vector<std::string>& board,
                      std::vector<std::string>& solution);

int main() {
    // Test 1: empty board -> 1 tiling, all '.'.
    std::vector<std::string> b1 = {"..", ".."};
    std::vector<std::string> sol1;
    assert(solveDominoTiling(2,2,b1,sol1) == 1);
    assert(sol1[0] == ".." && sol1[1] == "..");

    // Test 2: 2x2 all 'a' -> one tile, 7 color choices -> 7 tilings (but colors can repeat? each tile uses one color, color count max 2, so 7 distinct colors, so 7).
    std::vector<std::string> b2 = {"aa", "aa"};
    std::vector<std::string> sol2;
    assert(solveDominoTiling(2,2,b2,sol2) == 7);
    // Solution must have same letter positions replaced by digits.
    assert(sol2[0][0] >= '0' && sol2[0][0] <= '6');

    // Test 3: impossible because isolated letter.
    std::vector<std::string> b3 = {"a.", ".."};
    std::vector<std::string> sol3;
    assert(solveDominoTiling(2,2,b3,sol3) == 0);
    assert(sol3[0][0] == '.' && sol3[0][1] == '.' && sol3[1][0] == '.' && sol3[1][1] == '.');

    // Test 4: Two disjoint tiles, each with a single letter pair.
    std::vector<std::string> b4 = {"aabb", "aabb"};
    std::vector<std::string> sol4;
    // We expect multiple tilings. At least one exists.
    assert(solveDominoTiling(2,4,b4,sol4) > 0);
    // Check that solution has digits in all letter positions and '.' elsewhere.
    bool ok = true;
    for (int i=0;i<2;i++) for (int j=0;j<4;j++) {
        if (b4[i][j]=='.') ok &= (sol4[i][j]=='.');
        else ok &= (sol4[i][j]>='0' && sol4[i][j]<='6');
    }
    assert(ok);

    // Test 5: pair constraint (a and b together only once).
    std::vector<std::string> b5 = {"abab", "abab"};
    // Top-left tile has a,b; top-right tile has a,b again -> should be invalid because pair (a,b) repeated.
    // But there might be alternative tilings? Actually only one way to place tiles? Let's see: 2x4, tiles are at (0,0) and (0,2). Both have letters a,b. So pair (a,b) appears twice -> invalid. So count should be 0.
    std::vector<std::string> sol5;
    assert(solveDominoTiling(2,4,b5,sol5) == 0);

    // Test 6: Edge case with n=1 (no possible 2x2 tiles if any letter).
    std::vector<std::string> b6 = {"a"};
    std::vector<std::string> sol6;
    assert(solveDominoTiling(1,1,b6,sol6) == 0);

    // Test 7: A valid 4x4 board with exactly 4 tiles, all same letter.
    std::vector<std::string> b7 = {
        "aaaa",
        "aaaa",
        "aaaa",
        "aaaa"
    };
    std::vector<std::string> sol7;
    // Each tile uses one color, colors can repeat up to 2 times. Now many combinations? Brute force not needed; just check >0.
    assert(solveDominoTiling(4,4,b7,sol7) > 0);

    std::cout << "All tests passed.\n";
    return 0;
}

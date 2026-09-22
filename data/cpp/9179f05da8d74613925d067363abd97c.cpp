Write a C++ function that takes a 2D grid of characters where '#' represents a cell containing a mine and '.' represents an empty cell. The grid has H rows and W columns. Determine the minimum number of connected groups of mines such that when you divide the grid into rectangular regions by drawing horizontal or vertical lines (full-width or full-height separators), every region contains at most one connected group of mines. A connected group is defined by 4-directional adjacency of '#' cells. If no valid division is possible, return -1. The function signature is `int minimumSegments(const vector<string>& grid)`. The input is guaranteed to have at least 1 row and 1 column. The grid dimensions can be up to 1000x1000, so avoid unnecessary copying and use efficient algorithms.

The solution approach involves several observations. First, consider each row and column. For a valid division to exist, in each row, there must be at most one transition from empty cell to mine as you scan left-to-right (i.e., the mines in a row must form a contiguous block), and similarly for each column scanning top-to-bottom. This is because if a row had two separate mine blocks, any vertical separator inside that row would split the mine group, but a horizontal separator would cross both blocks, so you cannot isolate them into separate regions. The code checks this by counting transitions from `.` to `#` in each row and column. If any transition count exceeds 1, the answer is -1. Additionally, there's a constraint about empty rows and columns: if all rows contain at least one mine but there is at least one completely empty column, or vice versa, then it's impossible because you cannot place a separator that isolates the empty region without splitting a mine group. The code enforces this by checking if `empR == 0 && empC != 0` or `empR != 0 && empC == 0`, returning -1.

After passing these checks, the problem reduces to counting the number of connected components of mines using a union-find structure. However, the final answer is not simply the number of connected components. Instead, it's the number of components minus the number of empty rows minus the number of empty columns. The reasoning: each empty row and empty column can be used as a separator without affecting mine groups. Each connected component of mines must be completely contained within a single region, and each region can contain at most one component. The starting number of regions is the number of rows plus columns (if we drew all possible lines), but we subtract those that are not needed. More precisely, the union-find is built on H+W nodes representing each row and each column; each mine cell connects its row and column. After unioning all mines, the number of connected components in this bipartite graph (which corresponds to mine components in a certain way) is `UF.SZ`. Then the answer is `UF.SZ - empR - empC`. This works because each empty row and empty column are isolated nodes in the union-find and reduce the effective component count. The time complexity is O(H*W) for scanning and union operations (with nearly constant union-find amortized). Space complexity is O(H+W) for union-find plus O(H*W) for the grid if stored, but the grid can be processed as a 2D boolean array; the original code uses a fixed 1002x1002 array.

#include <vector>
#include <string>
#include <cassert>

class UnionFind {
public:
    std::vector<int> parent;
    int size;
    UnionFind(int n) : parent(n), size(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            parent[ra] = rb;
            --size;
        }
    }
};

// Returns the minimum number of segments (regions) or -1 if impossible.
int minimumSegments(const std::vector<std::string>& grid) {
    const int H = static_cast<int>(grid.size());
    const int W = static_cast<int>(grid[0].size());
    
    // Build a 2D bool array for faster access (grid is small enough to copy).
    std::vector<std::vector<bool>> mine(H, std::vector<bool>(W, false));
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            mine[y][x] = (grid[y][x] == '#');
        }
    }
    
    // Check row constraints: at most one transition from empty to mine.
    int emptyRows = 0;
    for (int y = 0; y < H; ++y) {
        int transitions = 0;
        // Scan left to right
        bool prev = true; // treat left boundary as mine? Actually start with empty
        bool prevMine = false;
        for (int x = 0; x < W; ++x) {
            if (!prevMine && mine[y][x]) {
                ++transitions;
                if (transitions > 1) return -1;
            }
            prevMine = mine[y][x];
        }
        if (transitions == 0) ++emptyRows;
    }
    
    // Check column constraints
    int emptyCols = 0;
    for (int x = 0; x < W; ++x) {
        int transitions = 0;
        bool prevMine = false;
        for (int y = 0; y < H; ++y) {
            if (!prevMine && mine[y][x]) {
                ++transitions;
                if (transitions > 1) return -1;
            }
            prevMine = mine[y][x];
        }
        if (transitions == 0) ++emptyCols;
    }
    
    // Empty row/col consistency
    if (emptyRows == 0 && emptyCols != 0) return -1;
    if (emptyRows != 0 && emptyCols == 0) return -1;
    
    // Union-find over rows and columns
    UnionFind uf(H + W);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            if (mine[y][x]) {
                uf.unite(y, H + x);
            }
        }
    }
    
    return uf.size - emptyRows - emptyCols;
}

#include <cassert>
#include <vector>
#include <string>

// The function is provided above; test it here.
int main() {
    // Single mine
    assert(minimumSegments({"#"}) == 1);
    // Single empty cell
    assert(minimumSegments({"."}) == 0);
    // 2x2 with one mine
    assert(minimumSegments({"#.", ".."}) == 1);
    // 2x2 with two diagonal mines - impossible?
    std::vector<std::string> diagonal = {"#.", ".#"};
    assert(minimumSegments(diagonal) == -1); // each row/col has two transitions
    // 2x2 full of mines
    assert(minimumSegments({"##", "##"}) == 1);
    // 3x3 with a line of mines in a row
    assert(minimumSegments({"###", "...", "..."}) == 1);
    // 3x3 with two separate mine islands side by side in same row? impossible
    std::vector<std::string> twoInRow = {"#.#", "...", "..."};
    assert(minimumSegments(twoInRow) == -1); // row has two transitions
    // Empty rows vs empty cols mismatch
    std::vector<std::string> oneRowMine = {"#", "."};
    assert(minimumSegments(oneRowMine) == -1); // all rows have mines (only first row) and one empty col? Actually H=2, W=1, row0 has mine, row1 empty -> emptyRows=1? Wait row1 empty, so emptyRows=1, emptyCols=0 -> mismatch? Let's compute: row0 mine, row1 empty -> row1 has no transitions -> emptyRows=1. Column has one mine -> transitions=1, emptyCols=0. Since both non-zero? Actually emptyRows=1 !=0, emptyCols=0 -> condition `emptyRows !=0 && emptyCols ==0` -> -1. Correct.
    assert(minimumSegments({"#", "."}) == -1);
    // A valid case with empty row and column
    std::vector<std::string> valid = {"#.", ".."};
    assert(minimumSegments(valid) == 1);
    // More complex valid case: 2x3 with mines forming a corner
    std::vector<std::string> corner = {"#..", "..."};
    // row0 has one transition, row1 no transition -> emptyRows=1, col0 has one transition, col1 empty, col2 empty -> emptyCols=2 -> both non-zero, answer = uf.size -1-2? uf.size initially H+W=5, connect row0-col0 -> size=4, answer=4-1-2=1, correct.
    assert(minimumSegments(corner) == 1);
    // Larger valid: 3x3 with mine in top-left and bottom-right? Both rows and cols have two transitions -> -1.
    std::vector<std::string> diagonal3 = {"#..", ".#.", "..#"};
    assert(minimumSegments(diagonal3) == -1);
    // All empty
    std::vector<std::string> allEmpty = {"...", "..."};
    // H=2, W=3, emptyRows=2, emptyCols=3, uf.size=5, answer=5-2-3=0
    assert(minimumSegments(allEmpty) == 0);
    return 0;
}

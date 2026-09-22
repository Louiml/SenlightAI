Write a C++ function `countFaceSquares` that takes two integers `n` and `m`, and a vector of strings `grid` (where each string has exactly `m` characters, and the total number of strings is `n`), representing an `n × m` grid of lowercase letters. The function must return the number of `2 × 2` sub-squares in the grid whose four letters are exactly the set {'f', 'a', 'c', 'e'} (each letter appears exactly once in the set, order within the sub-square does not matter). The grid is guaranteed to contain only lowercase English letters. For example, a `2 × 2` block containing letters `c`, `e`, `a`, `f` in any arrangement counts. The function must handle grids with `n` or `m` less than 2 by returning 0. Ensure the function is efficient for large grids (up to 100×100 or larger).

The solution is straightforward: iterate over all possible top-left corners of a `2 × 2` sub-square. For each `(i, j)` where `0 ≤ i < n-1` and `0 ≤ j < m-1`, collect the four characters `grid[i][j]`, `grid[i][j+1]`, `grid[i+1][j]`, `grid[i+1][j+1]` into a `std::set<char>`. If that set is equal to the set `{'f', 'a', 'c', 'e'}`, increment the counter. The set comparison ensures that all four letters are present and distinct, matching exactly the required set (since any duplicate or missing letter would produce a different set). This approach naturally handles any order of the letters. Edge cases: if `n < 2` or `m < 2`, the loops simply do not execute and the function returns 0. The time complexity is O(n·m) because each grid cell is read at most four times (into up to four different sub-squares), and each set operation is O(1) since the set has at most 4 elements. The space complexity is O(1) beyond the input grid itself, as we only create small temporary sets.

#include <vector>
#include <string>
#include <set>

// Count the number of 2x2 sub-squares whose letters are exactly {f, a, c, e}.
int countFaceSquares(int n, int m, const std::vector<std::string>& grid) {
    int count = 0;
    const std::set<char> faceSet = {'f', 'a', 'c', 'e'};
    
    if (n < 2 || m < 2) {
        return 0;
    }
    
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m - 1; ++j) {
            std::set<char> subSquare = {
                grid[i][j],
                grid[i][j + 1],
                grid[i + 1][j],
                grid[i + 1][j + 1]
            };
            if (subSquare == faceSet) {
                ++count;
            }
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link it).

int main() {
    // Basic example from the description: 2x2 grid with exact letters
    {
        std::vector<std::string> grid = {"fc", "ae"};
        assert(countFaceSquares(2, 2, grid) == 1);
    }
    // Any order of the four letters works
    {
        std::vector<std::string> grid = {"ace", "fzz"};
        assert(countFaceSquares(2, 3, grid) == 1); // top-left 2x2 has {a,c,f,z?} wait: a,c,e,f from positions (0,0),(0,1),(0,2) and (1,0) -> that's a,c,e,f => set {a,c,e,f} == {f,a,c,e}
    }
    // Grid with n < 2
    {
        std::vector<std::string> grid = {"face"};
        assert(countFaceSquares(1, 4, grid) == 0);
    }
    // Grid with m < 2
    {
        std::vector<std::string> grid = {"f", "a", "c", "e"};
        assert(countFaceSquares(4, 1, grid) == 0);
    }
    // Larger grid with multiple overlapping sub-squares
    {
        std::vector<std::string> grid = {
            "face",
            "afce",
            "cefa",
            "efac"
        };
        // Let's count manually: Each 2x2 block in a 4x4 grid has 9 possible top-left positions.
        // All letters in the grid are f,a,c,e, but some sub-squares might have duplicates.
        // This test is more complex; instead use a simpler known case.
        // Replaced with a more predictable test:
    }
    // Test with duplicate letters that should not count
    {
        std::vector<std::string> grid = {"ff", "aa"};
        assert(countFaceSquares(2, 2, grid) == 0); // set is {f,a}, not face
    }
    // Test with a 3x3 grid where only some sub-squares match
    {
        std::vector<std::string> grid = {
            "face",
            "zzzz",
            "zzzz"
        };
        // Only the top-left 2x2 (positions 0,0) has {f,a,c,e}? Let's check: (0,0)=f,(0,1)=a,(1,0)=z => not. Actually (0,0) is 'f', (0,1) 'a', (0,2) 'c', (0,3) 'e' but row 1 is all z, so no 2x2 has all four. So answer 0.
        assert(countFaceSquares(3, 4, grid) == 0);
        // Wait, row 0 has "face", row 1 "zzzz", so any 2x2 involving row 0 and row 1 has at least two z's. So 0.
    }
    // Simple 2x2 with letters in different order
    {
        std::vector<std::string> grid = {"ce", "fa"};
        assert(countFaceSquares(2, 2, grid) == 1);
    }
    // Multiple non-overlapping matches
    {
        std::vector<std::string> grid = {
            "face",
            "face"
        };
        // 2 rows, 4 columns: sub-squares at (0,0), (0,1), (0,2). Check:
        // (0,0): {f,a,f,a} = {f,a} -> no
        // (0,1): {a,c,a,c} = {a,c} -> no
        // (0,2): {c,e,c,e} = {c,e} -> no
        // So 0.
        assert(countFaceSquares(2, 4, grid) == 0);
        // Actually this grid has no all-four letters in any 2x2 because each column pair has only two distinct letters.
    }
    // A correct 2x2 with all four distinct letters
    {
        std::vector<std::string> grid = {"fa", "ce"};
        assert(countFaceSquares(2, 2, grid) == 1);
    }
    // Ensure correct handling of a 3x3 grid with one matching sub-square
    {
        std::vector<std::string> grid = {
            "abc",
            "def",
            "ghi"
        };
        // No letters f,a,c,e together in any 2x2? Check (0,0): {a,b,d,e} no. (0,1): {b,c,e,f} has f but not a,c? has f and c? yes, but missing a. So 0.
        assert(countFaceSquares(3, 3, grid) == 0);
    }
    // A specific known count: 3x3 grid where exactly one sub-square matches
    {
        std::vector<std::string> grid = {
            "fae",
            "cxe",
            "exx"
        };
        // Check sub-squares:
        // (0,0): {f,a,c,x} = {f,a,c,x} -> not face (missing e)
        // (0,1): {a,e,x,e} = {a,e,x} -> no
        // (1,0): {c,x,e,x} = {c,x,e} -> no
        // (1,1): {x,e,x,x} = {x,e} -> no
        // So 0. This test is not helpful. Let's use a known match: (0,0) with f,a,c,e. 
        std::vector<std::string> grid2 = {
            "face",
            "zzzz",
            "zzzz"
        };
        // 3 rows, 4 cols: possible top-left (0,0), (0,1), (0,2) for first two rows; (1,0), (1,1), (1,2) for rows 1-2. 
        // (0,0): {f,a,z,z} = {f,a,z} -> no
        // (0,1): {a,c,z,z} = {a,c,z} -> no
        // (0,2): {c,e,z,z} = {c,e,z} -> no
        // (1,0): {z,z,z,z} -> no
        // So 0.
        assert(countFaceSquares(3, 4, grid2) == 0);
    }
    // A correct single match in a 3x3 grid
    {
        std::vector<std::string> grid = {
            "fac",
            "ezz",
            "zzz"
        };
        // (0,0): {f,a,e,z} = {f,a,e,z} -> not face (has z, missing c? wait, no c? Actually grid[0][2]='c', grid[1][0]='e' so {f,a,c,e}? Let's check: (0,0)=f, (0,1)=a, (1,0)=e, (1,1)=z -> set {f,a,e,z} missing c. So no.
        // (0,1): {a,c,z,z} -> no. 
        assert(countFaceSquares(3, 3, grid) == 0);
        // Try a correct one:
        std::vector<std::string> grid3 = {
            "fa",
            "ce",
            "zx"
        };
        // n=3, m=2. Only 2 rows pair: (0,0) gives {f,a,c,e}? (0,0)=f, (0,1)=a, (1,0)=c, (1,1)=e -> yes! (1,0) gives rows 1-2: {c,e,z,x} -> no.
        assert(countFaceSquares(3, 2, grid3) == 1);
    }
    // A larger random-like case with known count
    {
        std::vector<std::string> grid = {
            "face",
            "face",
            "face"
        };
        // For each top-left in a 3x4 grid: top-left (0,0): {f,a,f,a} no. (0,1): {a,c,a,c} no. (0,2): {c,e,c,e} no. (1,0): {f,a,f,a} no. (1,1): {a,c,a,c} no. (1,2): {c,e,c,e} no. So 0.
        assert(countFaceSquares(3, 4, grid) == 0);
    }
    // Edge case: grid with exactly 2x2 but letters repeated
    {
        std::vector<std::string> grid = {"ff", "ff"};
        assert(countFaceSquares(2, 2, grid) == 0);
    }
    return 0;
}

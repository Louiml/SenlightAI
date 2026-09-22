// Write a C++ function `std::vector<std::vector<int>> makeSpiral(int N, int target, int& targetRow, int& targetCol)` that, given a positive integer `N`, fills an `N x N` grid with numbers from `N*N` down to 1 in a clockwise spiral starting from the top-left corner (row 0, column 0), moving down first (south), then right, then up, then left, and repeating. The function must also find the 1-based row and column indices (r and c, with 1 ≤ r, c ≤ N) where the number `target` (which is guaranteed to be between 1 and N*N inclusive) is placed, and store these indices in `targetRow` and `targetCol` by reference. Return the completed grid as a vector of vectors of ints, where each inner vector represents a row. For example, for N=3 and target=5, the spiral is:  
// 9 8 7  
// 2 1 6  
// 3 4 5  
// So target=5 is at row 3, column 3 (1-based). The function must handle N=1 correctly. The algorithm must use recursion or iteration, but avoid any global variables; all state must be passed explicitly.

The solution uses a recursive depth-first fill. Start at (0,0) with the current number `N*N` and direction 0 (south). At each cell, write the current number and check if it equals `target`; if so, record the 1-based row and column. Then compute the next cell in the same direction. If that next cell is out of bounds or already filled (nonzero), turn clockwise by incrementing direction modulo 4 and recompute the next cell. Recurse with `cnt-1` until `cnt==0`. Base case: when `cnt==0`, return immediately without writing. Edge cases: N=1 produces a single cell with value 1 and target must be 1; the recursion step must avoid going out of range. Time complexity is O(N^2) because each cell is visited exactly once. Space complexity is O(N^2) for the grid plus O(N^2) recursion depth in the worst case (though the spiral path depth is N^2), but the recursion stack is O(N^2) in the worst case for a spiral that never backtracks; however, since each step is a new cell and no branching, it's actually a chain of depth N^2, so space O(N^2) for recursion plus grid. In practice, iterative would be better, but recursive is acceptable for small N. For N up to maybe 1000, recursion depth could cause stack overflow; but the task does not specify limits, so it's fine as an exercise.

#include <vector>

// Recursive helper to fill the spiral.
// v is the grid being filled.
// r, c are current 0-based coordinates.
// cnt is the current number to write.
// dir is 0=south, 1=east, 2=north, 3=west.
// N is grid size.
// target is the number whose position we are finding.
// targetRow, targetCol are output 1-based positions (set when target is found).
void fillSpiral(std::vector<std::vector<int>>& v, int r, int c, int cnt, int dir,
                int N, int target, int& targetRow, int& targetCol) {
    if (cnt == 0) return;
    if (cnt == target) {
        targetRow = r + 1;
        targetCol = c + 1;
    }
    v[r][c] = cnt;

    // Direction vectors: south, east, north, west.
    const int dr[] = {1, 0, -1, 0};
    const int dc[] = {0, 1, 0, -1};

    int nr = r + dr[dir];
    int nc = c + dc[dir];

    // If out of bounds or already visited, turn clockwise.
    if (nr < 0 || nr >= N || nc < 0 || nc >= N || v[nr][nc] != 0) {
        dir = (dir + 1) % 4;
        nr = r + dr[dir];
        nc = c + dc[dir];
    }

    fillSpiral(v, nr, nc, cnt - 1, dir, N, target, targetRow, targetCol);
}

// Main function: creates spiral grid and finds target position.
// Returns the grid; sets targetRow and targetCol to 1-based indices.
std::vector<std::vector<int>> makeSpiral(int N, int target, int& targetRow, int& targetCol) {
    std::vector<std::vector<int>> v(N, std::vector<int>(N, 0));
    targetRow = -1; // sentinel
    targetCol = -1;
    fillSpiral(v, 0, 0, N * N, 0, N, target, targetRow, targetCol);
    return v;
}

#include <cassert>
#include <vector>

// Include the solution code here or via header.
// (Function declarations are assumed from the section.)

int main() {
    // Test N=1
    int r, c;
    auto grid1 = makeSpiral(1, 1, r, c);
    assert(grid1.size() == 1 && grid1[0].size() == 1);
    assert(grid1[0][0] == 1);
    assert(r == 1 && c == 1);

    // Test N=2 (spiral: 4 3 / 1 2)
    auto grid2 = makeSpiral(2, 4, r, c);
    assert(grid2[0][0] == 4 && grid2[0][1] == 3);
    assert(grid2[1][0] == 1 && grid2[1][1] == 2);
    assert(r == 1 && c == 1); // target=4 at (1,1)

    auto grid2b = makeSpiral(2, 2, r, c);
    assert(r == 2 && c == 2); // target=2 at bottom-right

    // Test N=3 from example
    auto grid3 = makeSpiral(3, 5, r, c);
    assert(grid3[0][0] == 9 && grid3[0][1] == 8 && grid3[0][2] == 7);
    assert(grid3[1][0] == 2 && grid3[1][1] == 1 && grid3[1][2] == 6);
    assert(grid3[2][0] == 3 && grid3[2][1] == 4 && grid3[2][2] == 5);
    assert(r == 3 && c == 3); // target=5 at bottom-right

    // Test target at center for N=3 (target=1 is center)
    auto grid3c = makeSpiral(3, 1, r, c);
    assert(r == 2 && c == 2);

    // Test N=4, check corners
    auto grid4 = makeSpiral(4, 16, r, c);
    assert(grid4[0][0] == 16);
    assert(r == 1 && c == 1);

    auto grid4b = makeSpiral(4, 1, r, c);
    assert(grid4b[2][2] == 1);
    assert(r == 3 && c == 3);

    // Test N=5, check last number (1) position
    auto grid5 = makeSpiral(5, 1, r, c);
    assert(grid5[2][2] == 1);
    assert(r == 3 && c == 3);

    // Test N=5, target=25 (top-left)
    auto grid5b = makeSpiral(5, 25, r, c);
    assert(r == 1 && c == 1);

    // Test N=4, target=10 (should be at row 3, col 2? Let's brute mentally)
    auto grid4c = makeSpiral(4, 10, r, c);
    // Full grid for N=4: 
    // 16 15 14 13
    // 5  4  3  12
    // 6  1  2  11
    // 7  8  9  10
    // So 10 at row 4, col 4? Wait top-left 16, down south 5, then 6,7, then east? Actually spiral: south from (0,0): (1,0)=5, (2,0)=6, (3,0)=7, then east (3,1)=8, (3,2)=9, (3,3)=10, so 10 at row 4 col 4.
    assert(r == 4 && c == 4);

    assert(true); // all passed
}

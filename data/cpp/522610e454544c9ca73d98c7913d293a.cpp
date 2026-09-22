/*
Write a C++ function `int minimumExtraLadderLines(int n, int m, int h, const std::vector<std::pair<int,int>>& existingLadders)` that solves the following problem: There is a vertical grid with `h` horizontal rows (numbered 1 to h from top to bottom) and `n` vertical columns (numbered 1 to n from left to right). You can place horizontal ladder segments (each connecting two adjacent columns) on any row, so a segment at row `r` and column `c` connects column `c` to column `c+1` (representation: `ladder[r][c] = true`). A person starts at the top of column `i` (row 0, before any row) and moves down: at each row, if there is a ladder segment at the current column `c` (i.e., `ladder[r][c]` true), she moves to column `c+1`; else if there is a segment at column `c-1` (i.e., `ladder[r][c-1]` true), she moves to column `c-1`; otherwise she stays in the same column. The goal is that for every starting column `i` from 1 to n, after traversing all `h` rows, the person ends in the same column `i`. You are already given `m` existing segments (each given as `(a,b)` meaning row `a`, column `b`). You may add additional segments, but the total number of added segments must be at most 3. If it is possible to achieve the goal with at most 3 additions, return the minimum number of added segments required; if impossible, return -1. Constraints: `n` (1 ≤ n ≤ 10), `h` (1 ≤ h ≤ 30), `m` (0 ≤ m ≤ n*h). Existing segments are valid and do not overlap (no two segments on same row and same column, and no segment violates adjacency constraints). You cannot add a segment that would overlap existing or another added segment, and a segment at (r,c) is only allowed if `c` is between 1 and n-1 (since it connects c and c+1), and also it must not have adjacent segments on the same row (i.e., cannot have segments at (r,c-1) or (r,c+1)). The function must return the minimal number of added segments (0 to 3) or -1 if not possible.
*/

#include <vector>
#include <algorithm>

int minimumExtraLadderLines(int n, int m, int h, const std::vector<std::pair<int,int>>& existingLadders) {
    // ladder[row][col] = true means a segment connects col to col+1 at this row
    std::vector<std::vector<bool>> ladder(h + 1, std::vector<bool>(n + 2, false));
    for (const auto& seg : existingLadders) {
        ladder[seg.first][seg.second] = true;
    }

    // Helper to check if a simulation passes
    auto check = [&]() -> bool {
        for (int start = 1; start <= n; ++start) {
            int col = start;
            for (int row = 1; row <= h; ++row) {
                if (ladder[row][col]) {
                    col++; // move right
                } else if (ladder[row][col - 1]) {
                    col--; // move left
                }
            }
            if (col != start) return false;
        }
        return true;
    };

    // Depth-limited search: add exactly (depth) new segments
    bool found = false;
    int answer = -1;
    std::function<void(int, int, int, int)> dfs = [&](int depth, int maxDepth, int startRow, int startCol) {
        if (found) return;
        if (depth == maxDepth) {
            if (check()) {
                answer = depth;
                found = true;
                return;
            }
            return;
        }
        // Try adding next segment, scanning rows/cols in order
        for (int r = startRow; r <= h; ++r) {
            int cBegin = (r == startRow) ? startCol : 1;
            for (int c = cBegin; c <= n - 1; ++c) {
                // Can't place if already a segment here or adjacent on same row
                if (!ladder[r][c] && !ladder[r][c - 1] && !ladder[r][c + 1]) {
                    ladder[r][c] = true;
                    dfs(depth + 1, maxDepth, r, c);
                    if (found) return;
                    ladder[r][c] = false;
                }
            }
        }
    };

    for (int totalAdded = 0; totalAdded <= 3; ++totalAdded) {
        dfs(0, totalAdded, 1, 1);
        if (found) break;
    }
    return answer;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: No existing ladders, n=1, h=1 -> trivially works
    assert(minimumExtraLadderLines(1, 0, 1, {}) == 0);

    // Test 2: Single existing ladder breaks the condition, need one fix
    // n=2,h=1, ladder at row1 col1 causes column1->2 and column2->1, so fails.
    // Can fix by removing? Cannot remove, so impossible? Actually cannot add any segment (only row1, no room). Must be -1.
    assert(minimumExtraLadderLines(2, 1, 1, {{1,1}}) == -1);

    // Test 3: n=2,h=1, no ladders -> works
    assert(minimumExtraLadderLines(2, 0, 1, {}) == 0);

    // Test 4: n=3,h=1, existing ladder at row1 col1 breaks it (1->2,2->1? actually 1->2,2->1? 2->1 if ladder[1][1]? 2 has ladder at col-1=1 so moves left to 1, 3 unchanged) -> need fix
    // Could add ladder at row1 col2 to create a cycle? Let's see: with ladder[1][1] and ladder[1][2], paths: 1->2, 2->3, 3->2? Actually 2 has right ladder (col2) so moves to 3; 3 has left ladder at col2 so moves to 2; not identity. Not fixable with ≤3? Possibly -1.
    assert(minimumExtraLadderLines(3, 1, 1, {{1,1}}) == -1);

    // Test 5: n=4,h=1, two ladders that form disjoint swaps? e.g. ladder[1][1] and ladder[1][3] -> 1<->2 and 3<->4, so identity? Actually 1->2,2->1,3->4,4->3, fails. Add one more ladder at col2? Not allowed adjacent. Add at col? None. So -1.
    assert(minimumExtraLadderLines(4, 2, 1, {{1,1},{1,3}}) == -1);

    // Test 6: n=2,h=2, no ladders -> 0
    assert(minimumExtraLadderLines(2, 0, 2, {}) == 0);

    // Test 7: n=3,h=2, existing ladder at row1 col1 and row2 col2. Check: start1: row1 moves to2, row2 at2 has ladder at col2? true moves to3 -> ends at3, fails. Need 1 addition? Add ladder at row2 col1? Then start1: row1->2, row2 has left ladder? at col2? no, has ladder at col1? no, wait ladder at row2 col1 connects 1-2, not at col2. So start1 at row2 col2: has left? col2-1=1 is true, so moves left to1 -> ends at1. Good. Start2: row1 col2 has left? col1 true moves to1? row1 col1 is true, so from col2 moves left to1. row2 col1 has right ladder col1 true moves to2. Ends at2. Start3: row1 col3 no ladder, stays 3. row2 col3 no ladder, stays 3. So works. So answer 1.
    assert(minimumExtraLadderLines(3, 2, 2, {{1,1},{2,2}}) == 1);

    // Test 8: n=3,h=2, one existing ladder at row1 col2. Paths: start1: row1 col1 no, row2 col1 no -> stays1. start2: row1 col2 has right ladder -> moves to3, row2 col3 no -> ends3 fails. Can add ladder at row2 col2? Then start2: row1->3, row2 col3 has left ladder at col2? yes (row2 col2 true) -> moves to2. start3: row1 col3 no, row2 col3 has left at col2 -> moves to2 -> ends2 fails. Add ladder at row1 col1? then start1->2, row2 col2 no? stays2 fails. No fix? Try adding two: e.g. row1 col1 and row2 col2: start1->2, row2 col2 right? no left? col1 true? no -> stays2 fail. So impossible, -1.
    assert(minimumExtraLadderLines(3, 1, 2, {{1,2}}) == -1);

    // Test 9: n=4,h=2, existing empty, need 0
    assert(minimumExtraLadderLines(4, 0, 2, {}) == 0);

    // Test 10: n=5,h=3, a configuration that requires exactly 2 additions (example from problem? Use known scenario)
    // We'll craft: existing ladder at (1,1) and (2,3). This should be impossible? Just check that function returns -1 or non-negative.
    // For testing the algorithm, we can assert the return is either 0..3 or -1, but better to test a known solvable case.
    // Known from BOJ 15684: n=2,h=1,m=0 -> 0; n=2,h=1,m=1 at (1,1) -> -1; n=2,h=2,m=1 at (1,1) -> can add (2,1)? Let's check: ladder[1][1] and ladder[2][1] makes swap? start1: row1->2, row2 col2 has left at col1? yes -> back to1; start2: row1 col1 left? none, stays2? actually col2 has left at col1 true? Yes row1 col1 true so from col2 moves left to1; row2 col1 has right at col1 true moves to2. So works. So answer 1.
    assert(minimumExtraLadderLines(2, 1, 2, {{1,1}}) == 1);

    return 0;
}

// The problem is a classic DFS-backtracking search, similar to the popular "Ladder" or "Screw" puzzles (e.g., from BOJ 15684). The key is to simulate the path for each starting column given a set of ladders (existing plus added). The main algorithm: For each possible additional segment count from 0 to 3 (in increasing order), try to place exactly that many new segments in all possible valid positions, without exceeding the adjacency constraints (no two segments on the same row adjacent, i.e., cannot have both at (r,c) and (r,c+1) because they'd overlap in columns). Recursively pick positions in row-major order (row 1..h, column 1..n-1) to add segments until the desired count is reached. After adding exactly `depth` segments, run a simulation: for each start column, move down the rows, updating column based on `ladder[row][column]` (true means move right) and `ladder[row][column-1]` (true means move left). If all end columns equal their start columns, output the depth. The search is pruned by the adjacency rule: a new segment at (r,c) is valid only if `!ladder[r][c] && !ladder[r][c-1] && !ladder[r][c+1]` (where out-of-bounds indices are treated as false). Since we try depths in increasing order, the first successful depth is minimal. Edge cases: If no addition is needed (all existing ladders already satisfy the condition), return 0; if the existing setup already fails and cannot be fixed with ≤3 additions, return -1. Also, a subtle point: you cannot add a segment at (r,c) if there is already one at (r,c+1) because that would overlap or create an invalid adjacency (actually the constraint `!ladder[r][c+1]` prevents two neighboring segments on the same row, which would make the movement ambiguous). Time complexity: In the worst case, there are about h*(n-1) possible positions (max 30*9=270). For each depth d (0..3), we try C(positions, d) combinations, but with pruning the actual search is much smaller. The simulation per state costs O(h*n). Worst-case upper bound is O( Σ_{d=0..3} C(270,d) * n*h ), which is enormous, but in practice n≤10 and h≤30, and the adjacency pruning reduces possibilities drastically; typical competitive problem accepts this depth-limited DFS. Space complexity is O(h*n) for the ladder grid plus O(depth) recursion stack.

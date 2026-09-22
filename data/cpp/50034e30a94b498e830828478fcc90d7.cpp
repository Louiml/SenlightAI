You are given a 2-row grid with `n` columns (rows numbered 1 and 2, columns 1 to n). Initially, all cells are empty (value 0). You will process `q` operations. Each operation provides two integers `x` and `y`, meaning you toggle the state of cell `(x, y)` — if it was 0, set it to 1; if it was 1, set it to 0. After each toggle, you must check whether there exists any "blocking pair": a cell in row 1 at column `c` and a cell in row 2 at column `d` such that both are set to 1 and `|c - d| ≤ 1`. If no such pair exists, print "Yes"; otherwise print "No". Write a C++ function `void processGrid(int n, const vector<pair<int,int>>& ops, vector<string>& results)` that takes the number of columns, a list of operations, and returns the list of "Yes"/"No" answers in order of operations. The function should not print anything itself; it should fill the `results` vector.
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// Assume processGrid is defined above.

int main() {
    // Test 1: Basic toggles with no blocking pair
    {
        int n = 3;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,2}, {1,2}};
        std::vector<std::string> res;
        processGrid(n, ops, res);
        std::vector<std::string> expected = {"Yes", "No", "Yes"}; // after (1,1) alone: yes; after (2,2) adjacent: no; after (1,2) toggled off: only (1,1) remains, yes
        assert(res == expected);
    }

    // Test 2: Same cell toggled twice, boundary
    {
        int n = 1;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,1}, {1,1}, {2,1}};
        std::vector<std::string> res;
        processGrid(n, ops, res);
        std::vector<std::string> expected = {"Yes", "No", "No", "Yes"}; // after each: (1,1); (1,1)+(2,1) blocking; (1,1) off, (2,1) still on -> no pair? Wait: after third op, grid has (2,1) only, so yes. Let's compute: op1: cell(1,1)=1, bad=0 -> Yes; op2: cell(2,1)=1, neighbor count=1 (row1 col1) -> bad=1 -> No; op3: cell(1,1) toggled off, neighbor count=1 (row2 col1) -> bad=0 -> Yes; op4: cell(2,1) toggled off, neighbor count=1 (row1 col1? but row1 col1 is 0 now) actually before op4, grid has row2 col1=1 and row1 col1=0, so neighbor count=0 -> bad=0 -> Yes. Expected: {"Yes","No","Yes","Yes"}.
        std::vector<std::string> expected_fixed = {"Yes", "No", "Yes", "Yes"};
        assert(res == expected_fixed);
    }

    // Test 3: All toggles on, blocking pair exists immediately
    {
        int n = 2;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,1}};
        std::vector<std::string> res;
        processGrid(n, ops, res);
        std::vector<std::string> expected = {"Yes", "No"};
        assert(res == expected);
    }

    // Test 4: Large spacing, no blocking pairs even with many cells
    {
        int n = 5;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,5}, {1,3}, {2,2}};
        std::vector<std::string> res;
        processGrid(n, ops, res);
        // After each: (1,1) yes; (1,1)+(2,5) yes; add (1,3) yes; add (2,2) -> (1,3) and (2,2) distance 1 -> no
        std::vector<std::string> expected = {"Yes", "Yes", "Yes", "No"};
        assert(res == expected);
    }

    // Test 5: Toggle off removing blocking pair
    {
        int n = 4;
        std::vector<std::pair<int,int>> ops = {{1,2}, {2,2}, {2,3}, {2,1}};
        std::vector<std::string> res;
        processGrid(n, ops, res);
        // Step1: (1,2) -> Yes
        // Step2: (2,2) -> neighbor (1,2) -> bad=1 -> No
        // Step3: (2,3) -> neighbor (1,2)? distance 1 -> bad becomes 2 -> No
        // Step4: (2,1) -> neighbor (1,2) distance 1 -> bad becomes 3 -> No
        std::vector<std::string> expected = {"Yes", "No", "No", "No"};
        assert(res == expected);
    }

    // Test 6: Empty operations
    {
        int n = 10;
        std::vector<std::pair<int,int>> ops;
        std::vector<std::string> res;
        processGrid(n, ops, res);
        assert(res.empty());
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Process toggles on a 2-row grid and fill results with "Yes"/"No" per operation.
// Grid rows are 1 and 2, columns 1..n. A blocking pair exists if row1[c] and row2[d] both set and |c-d| <= 1.
void processGrid(int n, const std::vector<std::pair<int,int>>& ops, std::vector<std::string>& results) {
    const int ROWS = 3; // index 0 unused, rows 1 and 2
    std::vector<std::vector<int>> grid(ROWS, std::vector<int>(n + 2, 0)); // sentinel columns 0 and n+1
    int bad = 0;
    results.clear();
    results.reserve(ops.size());

    for (const auto& op : ops) {
        int x = op.first;
        int y = op.second;
        int opp = 3 - x; // if x is 1, opp is 2; if x is 2, opp is 1

        // Count opponent neighbors in columns y-1, y, y+1 (bounds safe due to sentinels)
        int neighborCount = 0;
        for (int c = y - 1; c <= y + 1; ++c) {
            if (c >= 1 && c <= n && grid[opp][c] == 1) {
                ++neighborCount;
            }
        }

        // Toggle the cell
        if (grid[x][y] == 0) {
            grid[x][y] = 1;
            bad += neighborCount;
        } else {
            grid[x][y] = 0;
            bad -= neighborCount;
        }

        results.push_back(bad == 0 ? "Yes" : "No");
    }
}
// The naive approach would re-scan all pairs after each operation, but that is too slow. Instead, maintain the current grid state in a `2 x (n+2)` integer array (1-indexed, with sentinel zero columns at both ends to simplify bounds checking). Also keep a counter `bad` of the current number of blocking pairs. When toggling cell `(x, y)`, let `opp = 3 - x` (since rows are 1 and 2). Before the toggle, count how many of the opponent's cells in columns `y-1`, `y`, `y+1` (within bounds) are set to 1. If the current cell was 0 and becomes 1, add that count to `bad`; if it was 1 and becomes 0, subtract that count. Then set the cell to its new value. After each operation, if `bad == 0`, the answer is "Yes", otherwise "No". Edge cases: toggling the same cell multiple times, operations at the boundaries (columns 1 and n), and when a cell is toggled from 1 to 0, the count of opponent neighbors must be computed before the toggle (since the cell itself is not counted). Time complexity is O(n + q) because each toggle checks at most 3 neighbor cells; space complexity O(n) for the grid plus O(q) for results.

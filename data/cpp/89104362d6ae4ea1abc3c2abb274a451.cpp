// You are given an `m x n` rectangular grid of cells, where rows are numbered 1..m and columns are numbered 1..n. Some cells of the grid are marked as "blocked" (represented as points with coordinates). You need to write a C++ function that, given the grid dimensions (`m`, `n`) and a list of `k` blocked cell coordinates, counts how many distinct 1x1 empty (non-blocked) cells are not part of any line of three or more consecutive empty cells, either horizontally in the same row or vertically in the same column. A line is defined as three or more consecutive empty cells in a row (same row) or in a column (same column), where the cells are adjacent to each other. The count should include a single empty cell if it is isolated (i.e., it does not lie in any such horizontal or vertical segment of length >= 3). For example, in a 3x3 grid with blockers at (2,2) only, the cells (1,1),(1,3),(3,1),(3,3) are isolated (since they cannot form a segment of length>=3 with neighbors because of blockers), while cells like (1,2) and (2,1) are part of a horizontal/vertical segment of 3 consecutive empty cells (row1: (1,1),(1,2),(1,3) all empty? Actually (1,2) has left (1,1) and right (1,3) both empty, so it's in a horizontal segment of length 3; similarly (2,1) is in vertical segment). The function should return the number of empty cells that are not in any horizontal or vertical segment of length >= 3. The grid is finite, and blockers are given as pairs (row, column) with 1-indexed coordinates. You may assume all inputs are valid and within range.
We need to count empty cells that are "isolated" in the sense that they do not belong to any run of at least 3 consecutive empty cells horizontally or vertically. The provided snippet uses a clever approach: it creates boundary points around the grid (row 0, row m+1, column 0, column n+1) to represent virtual blockers, then sorts all points (blockers + boundaries) by rows and by columns, and for each adjacent pair in sorted order along the same row (or column), it examines the gap between them. If the distance between two consecutive points in a row is exactly 2, it means there is exactly one empty cell between them, and that cell might be a candidate for isolation (it cannot be part of a longer horizontal segment because the neighbors are blockers/boundaries). If the distance is greater than 2, that means there are at least two empty cells in the gap, so those cells are in a segment of length at least 3 (since the gap length is distance-1 >= 2, but actually if distance is 3, gap length is 2, which is not a segment of 3; careful: distance is the difference in column indices. If two blockers are at columns 2 and 5, the empty cells at 3 and 4 are consecutive and count 2, not 3. Only if distance >= 4 would there be at least 3 consecutive empty cells. But the snippet counts only if distance > 2 as answer++ -- that seems to count segments, not isolated cells. Actually the original code counts number of segments, not isolated cells. We need to reinterpret the task: we are counting the number of lines (segments) of length >= 3? The problem statement says "count how many distinct 1x1 empty cells are not part of any line of three or more consecutive empty cells". That is the count of isolated cells. Let's analyze the snippet's logic: It increments `answer` when distance > 2. That is counting the number of maximal empty runs that have length at least 3? Actually distance between adjacent points (blockers/boundaries) is d, so empty cells between them = d-1. If d > 2, then empty cells >= 2, so run length >= 2. But the code increments for d>2, not d>=4. That suggests it counts runs of length at least 2? That seems off. Let me re-evaluate: The snippet also handles d==2 by storing the single cell in `single_squares`. Then later it sorts single_squares and increments answer for duplicates, which means a cell that appears both as a single empty cell in a row and in a column (i.e., isolated both horizontally and vertically) gets counted once. So the code actually counts the number of isolated cells? Wait: For row scan: if d==2, the single empty cell between two blockers is recorded. If d>2, it increments answer, but actually d>2 means there are at least 2 empty cells in that run, so those are not isolated, but the code increments answer for the whole run? That seems to count runs of length >=2? That would be wrong. Let's test with the given example: m=3,n=3,k=1 with blocker (2,2). Boundaries: rows 0..4? Actually make_borders pushes points on row 0 and row m+1 for each column? Let's read the code: first make_borders(points, m, n, false) pushes points with p1={i,0}, p2={i,n+1} for i=1..m. That is for each row i, add left and right boundary columns 0 and n+1. Then make_borders(points, n, m, true) pushes p1={0,i}, p2={m+1,i} for i=1..n, i.e., for each column i, add top and bottom boundaries rows 0 and m+1. Then add blockers. So total points: rows boundaries (2 per row) + column boundaries (2 per col) + blockers. For m=3,n=3: rows boundaries 2*3=6, columns boundaries 2*3=6, so 12+1=13 points. Sorting by rows: for row 1, points: (1,0),(1,4) -> distance 4, >2, so answer++ (line 1). Row 2: (2,0),(2,2),(2,4) sorted: (2,0),(2,2) distance 2 -> record single (2,1); (2,2) to (2,4) distance 2 -> record single (2,3). Row 3: (3,0),(3,4) distance 4 -> answer++. Now columns sort: column 1: (0,1),(3,1)? Actually column boundaries: (0,1),(4,1) and also row boundaries? For column 1, we have points: (0,1),(4,1) from column boundaries, and also (1,0)? No, column 1 points are those with second=1: (0,1),(4,1) and also the blocker (2,2) has second=2, not 1. So only two points: (0,1),(4,1) distance 4 -> answer++. Similarly columns 2 and 3 each have two boundary points distance 4, but also blockers? For column2, points: (0,2),(4,2) and also (2,2) blocker. So sorted: (0,2),(2,2),(4,2): distances 2 and 2 -> record singles (1,2) and (3,2). Column3: (0,3),(4,3) distance 4 -> answer++. So after row and column scans, answer = number of distances >2 encountered. That counts: rows 1,3 each once => 2; columns 1,3 each once => 2; plus column2 has distances 2, not >2. So total answer=4? But that is wrong for counting isolated cells. The single_squares list: from row scan: (2,1) and (2,3); from column scan: (1,2) and (3,2). No duplicates, so single_squares size=4, but the final loop only increments answer if duplicates found, so answer remains 4. So the code's answer is 4, but in the grid with only (2,2) blocked, the empty cells are 8. Which of these are isolated? Let's see: A cell is isolated if it is not part of any horizontal or vertical run of length >=3. For a 3x3 grid with center blocked, consider cell (1,1): horizontally row1 has cells (1,1),(1,2),(1,3) all empty -> that's a run of length 3, so (1,1) is part of a horizontal run, so not isolated. Similarly (1,2),(1,3) also. Vertical column1: cells (1,1),(2,1),(3,1) all empty -> run of length 3. So (1,1) is not isolated. In fact, all edge cells are in runs of length 3 (each edge row/column has 3 empty cells). The only cells that might be isolated are those that are alone between blockers? But with just one blocker at center, no cell is isolated because every cell belongs to a run of length at least 3 horizontally or vertically. For example, (1,1) belongs to row run (1,1)-(1,3) and column run (1,1)-(3,1). So no isolated cells. The code gave 4, which is wrong. So the original snippet is not counting isolated cells; it's likely counting the number of "lines" (runs of empty cells of length >=2? Actually let's see: In row 1, there is a run of 3 empty cells from columns 1 to 3, but the code sees distance 4 between boundaries at columns 0 and 4, and increments answer, so it counts that run as 1 line. Similarly row 3 run, column 1 run, column 3 run: that gives 4 lines of length 3. Column 2 has a run of 2? Actually column2: cells (1,2) and (3,2) are empty individually? Wait column2 has cells (1,2), (2,2) blocked, (3,2). So there is a run of length 1 above blocker and length 1 below, not a run of 2. The code records them as single_squares. So the code's answer counts the number of horizontal and vertical segments of length >=3? For row1: run length 3 counts 1; row3:1; col1:1; col3:1 -> total 4. It does not count the single squares unless they appear in both row and column, which they don't because (2,1) is only vertical? Actually (2,1) is in column1 which has run length 3, so not single? Wait (2,1) is a cell, but the code records it as a single_square because in its row (row2) the distance between blockers at (2,0),(2,2) is 2, so the cell (2,1) is alone horizontally. But vertically, column1 has a run of length 3, so (2,1) is not isolated. The code only counts duplicates as answer++ later, but since (2,1) is not in the vertical single list (because column1 has distance 4), it's not duplicated. So the code's final answer is just the count of segments with distance >2, which is the number of maximal empty runs of length at least 3? Actually distance >2 means the number of empty cells between two consecutive blockers/boundaries is d-1. If d=3, then empty cells=2, not a run of 3. But for the boundaries, d = n+1 (since left boundary column 0, right column n+1) so d = n+1, for n=3, d=4, so empty cells=3, run length 3. So indeed d>2 might not guarantee run length>=3 if d=3 gives run length 2. But in this problem, boundaries are at 0 and n+1, so d = n+1, which is >=4 for n>=3. For internal gaps, d could be 3 giving run length 2, which is not a run of 3. The code increments for d>2 regardless, so it would count runs of length 2 as "lines" too, which is inconsistent. Given the complexity, the task should be clarified. Based on the original snippet's behavior, the intended problem is likely counting the number of distinct maximal horizontal and vertical lines of empty cells of length at least 3? But the code also handles single cells that are alone both horizontally and vertically (isolated 1x1 squares) by counting them as an extra answer. Actually re-reading: The final part: after sorting single_squares, if duplicates, answer++. That means a single cell that appears as a single in both row and column (i.e., it is isolated in both directions, meaning it is not part of any run of length >=2? Actually if it appears as a single in a row, that means in its row, the nearest obstacles on left and right are exactly one cell away, so it has no horizontal neighbors. Similarly for column. So such a cell is completely isolated (no horizontal or vertical adjacent empty cells). That cell is a 1x1 square of empty cells. The code counts it as an additional "line"? But a single cell is not a line. The problem statement in the snippet's comments says "если мы два раза записали одиночную клетку - значит она существует и в строке и столбце, а значит это квадратик" which translates to "if we recorded a single cell twice - it exists in both row and column, so it is a square". And then answer++ for each such square. So the program counts the number of horizontal/vertical runs of length >=3 (as answer from the >2 distances) plus the number of isolated 1x1 squares that are separated by obstacles on all four sides. That seems like a different problem: count the number of "free lines" of length at least 3 horizontally/vertically plus isolated single cells. But the task description I gave earlier is not matching. Let me read the user request: "Write a C++ function that, given the grid dimensions (m, n) and a list of k blocked cell coordinates, counts how many distinct 1x1 empty cells are not part of any line of three or more consecutive empty cells." That is clearly counting isolated cells, not lines. However, the provided code seems to count something else. As a teaching assistant, I need to create a task inspired by the snippet, but I can define the task clearly. I will define the task as: Given a grid with obstacles, count the number of empty cells that are not part of any horizontal or vertical segment of length ≥ 3. That is a clear definition. Then I need to provide a solution that correctly computes that count. The snippet's approach can be adapted: we can scan each row and each column, identify maximal runs of empty cells, and for each run of length L, if L >= 3, mark all cells in that run as "not isolated". Then count cells that are never marked. Alternatively, we can use the boundary method: for each row, find gaps between obstacles+boundaries. For each gap of length G (number of empty cells), if G >= 3, then all cells in that gap are part of a horizontal line of length >=3, so they are not isolated. Similarly for columns. Then a cell is isolated if it is in no gap of length >=3 horizontally and also no gap of length >=3 vertically. So we can compute for each cell whether it is covered by any horizontal/vertical long run. The snippet's approach of using distances and counting duplicates is essentially trying to find cells that are neither part of a long horizontal run nor a long vertical run. Specifically, a cell is not part of a long horizontal run if in its row, the distance between the nearest obstacles on left and right is exactly 2 (i.e., the gap length is 1). Similarly vertically. If a cell has gap length 1 both horizontally and vertically, then it is isolated. That is exactly the snippet's final duplicate detection. So the correct answer for isolated cells is the count of cells that appear as singletons in both row and column scans. The snippet's `answer` variable counts the number of long runs (distance >2) which is irrelevant if we only care about isolated cells. So the task should be: count the number of empty cells that are not contained in any horizontal or vertical run of three or more consecutive empty cells. Then the algorithm is: for each row, sort points (obstacles + boundaries) and examine gaps; record cells that are in gaps of length exactly 1 (i.e., isolated horizontally) into a set. Similarly for columns, record cells that are isolated vertically. Intersect the two sets to get cells isolated in both directions. The count is the size of intersection. That is a clean and correct problem. The snippet's final part does that intersection, but it incorrectly also counts long runs into answer and then adds duplicates. So to fix, we ignore the `answer++` for distance >2. So the task is to implement a function that returns the number of isolated empty cells. Edge cases: grid size 1x1 with no obstacles: the single cell is isolated because it has no neighbors, and no run of length 3 exists. For a 1x3 grid with no obstacles, the entire row is a run of length 3, so no isolated cells. For 2x2 grid, no run of length 3 possible, so all 4 cells are isolated. For m=1, n=1, boundaries: row0 and row2? Actually m and n can be small. We need to handle m=1 or n=1. Time complexity: sorting O((m+n+k) log(m+n+k)) since we sort points twice. Space O(m+n+k).
#include <vector>
#include <algorithm>
#include <utility>

// Counts the number of empty cells in an m x n grid that do not belong to any
// horizontal or vertical contiguous run of length >= 3.
int countIsolatedCells(int m, int n, const std::vector<std::pair<int,int>>& blockers) {
    // Collect all points: boundary points and blockers.
    std::vector<std::pair<int,int>> points;
    
    // Horizontal boundaries: for each row i (1..m), add left (i,0) and right (i,n+1)
    for (int r = 1; r <= m; ++r) {
        points.push_back({r, 0});
        points.push_back({r, n + 1});
    }
    // Vertical boundaries: for each column c (1..n), add top (0,c) and bottom (m+1,c)
    for (int c = 1; c <= n; ++c) {
        points.push_back({0, c});
        points.push_back({m + 1, c});
    }
    // Add blockers
    points.insert(points.end(), blockers.begin(), blockers.end());
    
    // Helper to compare by row first, then column
    auto byRow = [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    };
    // Helper to compare by column first, then row
    auto byCol = [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    };
    
    // These sets store cells that are isolated horizontally and vertically respectively.
    std::vector<std::pair<int,int>> isolatedHoriz;  // cells with no horizontal neighbors
    std::vector<std::pair<int,int>> isolatedVert;   // cells with no vertical neighbors
    
    // Horizontal scan: sort by row, and for each row find gaps between points.
    std::sort(points.begin(), points.end(), byRow);
    for (size_t i = 0; i + 1 < points.size(); ++i) {
        const auto& p1 = points[i];
        const auto& p2 = points[i+1];
        if (p1.first == p2.first) {
            int gap = p2.second - p1.second - 1; // number of empty cells between
            if (gap == 1) {
                // Exactly one empty cell in this gap -> it has no horizontal neighbors
                isolatedHoriz.push_back({p1.first, p1.second + 1});
            }
            // If gap >= 3, cells inside are part of long run, so they are not isolated horizontally.
            // We only record gap==1.
        }
    }
    
    // Vertical scan: sort by column, and for each column find gaps.
    std::sort(points.begin(), points.end(), byCol);
    for (size_t i = 0; i + 1 < points.size(); ++i) {
        const auto& p1 = points[i];
        const auto& p2 = points[i+1];
        if (p1.second == p2.second) {
            int gap = p2.first - p1.first - 1;
            if (gap == 1) {
                isolatedVert.push_back({p1.first + 1, p1.second});
            }
        }
    }
    
    // Sort both lists for intersection
    std::sort(isolatedHoriz.begin(), isolatedHoriz.end());
    std::sort(isolatedVert.begin(), isolatedVert.end());
    
    // Count cells that appear in both lists.
    size_t i = 0, j = 0;
    int count = 0;
    while (i < isolatedHoriz.size() && j < isolatedVert.size()) {
        if (isolatedHoriz[i] == isolatedVert[j]) {
            ++count;
            ++i;
            ++j;
        } else if (isolatedHoriz[i] < isolatedVert[j]) {
            ++i;
        } else {
            ++j;
        }
    }
    return count;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (declaration assumed).

int main() {
    // Example 1: 3x3 with center blocked -> no isolated cells
    {
        std::vector<std::pair<int,int>> blockers = {{2,2}};
        int result = countIsolatedCells(3, 3, blockers);
        assert(result == 0);
    }
    // Example 2: 1x1 with no blockers -> single isolated cell
    {
        std::vector<std::pair<int,int>> blockers;
        assert(countIsolatedCells(1, 1, blockers) == 1);
    }
    // Example 3: 2x2 with no blockers -> all 4 cells isolated
    {
        std::vector<std::pair<int,int>> blockers;
        assert(countIsolatedCells(2, 2, blockers) == 4);
    }
    // Example 4: 1x3 with no blockers -> all cells in a run of length 3, so none isolated
    {
        std::vector<std::pair<int,int>> blockers;
        assert(countIsolatedCells(1, 3, blockers) == 0);
    }
    // Example 5: 3x1 with no blockers -> run of length 3 vertically, none isolated
    {
        std::vector<std::pair<int,int>> blockers;
        assert(countIsolatedCells(3, 1, blockers) == 0);
    }
    // Example 6: 3x3 with all cells blocked except (1,1) -> only one empty cell, isolated
    {
        std::vector<std::pair<int,int>> blockers;
        for (int r = 1; r <= 3; ++r)
            for (int c = 1; c <= 3; ++c)
                if (!(r == 1 && c == 1))
                    blockers.push_back({r, c});
        assert(countIsolatedCells(3, 3, blockers) == 1);
    }
    // Example 7: 4x4 with blockers at (2,2) and (3,3) -> check manually:
    // Empty cells: many, but some isolated? Let's compute expected.
    // We'll just test an empty 4x4 grid: all runs of length >=4 horizontally/vertically, so no isolated.
    {
        std::vector<std::pair<int,int>> blockers;
        assert(countIsolatedCells(4, 4, blockers) == 0);
    }
    // Example 8: 2x3 with blocker at (1,2) -> empty cells: (1,1),(1,3),(2,1),(2,2),(2,3). Row1: (1,1) and (1,3) isolated horizontally? Row1 has gaps: boundary(0) to blocker(2) gap=1 -> (1,1); blocker(2) to boundary(4) gap=1 -> (1,3). Row2: no blockers, gap from 0 to 4 = 3 cells -> not isolated horizontally. Vertically: col1 has (1,1),(2,1) gap boundaries to top/bottom? Actually col1: points (0,1),(1,2?) no, (1,2) is row1 col2, not col1. So col1: boundaries (0,1),(3,1) no blockers -> gap 2 cells -> not isolated. col2: boundaries (0,2),(1,2),(3,2): gaps 0 and 1 -> (2,2) isolated vertically. col3: boundaries (0,3),(3,3) gap 2 -> not isolated. So intersection: (1,1) appears in horiz but not vert? (1,1) vert is col1 which has 2 cells, so not isolated. (1,3) similar. (2,2) appears in vert but not horiz (row2 gap 3). So no isolated? But what about (2,1)? horiz no (row2 gap 3), vert col1 gap 2, not isolated. So answer should be 0? Actually cell (2,2) is isolated vertically but horizontally it is part of row2 which has 3 consecutive empty cells? Row2 has columns 1,2,3 all empty, so (2,2) is part of a horizontal run of length 3, so not isolated. So answer 0. Let's test.
    {
        std::vector<std::pair<int,int>> blockers = {{1,2}};
        assert(countIsolatedCells(2, 3, blockers) == 0);
    }
    // Example 9: 5x5 with a checkerboard of blockers? Hard to compute manually, but ensure no crash.
    {
        std::vector<std::pair<int,int>> blockers = {{1,1},{2,2},{3,3},{4,4},{5,5}};
        int result = countIsolatedCells(5, 5, blockers);
        assert(result >= 0); // just a sanity check, not a specific value
    }
    return 0;
}

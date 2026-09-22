You are given a rectangular grid of size `n x m` (1 ≤ n, m ≤ 50) filled with characters `'.'` (water) and `'X'` (land). The grid is surrounded by an implicit border of land on all sides. A lake is defined as a connected component of water cells (connected by up/down/left/right) that is completely enclosed by land, meaning it does not touch the outer border of the grid. Your task is to write a C++ function that: (1) identifies all lakes and their sizes (number of water cells), (2) fills in (converts to `'X'`) the smallest lakes until exactly `k` lakes remain (k is given, and it is guaranteed that the number of lakes is at least k), filling the smallest ones first (ties broken arbitrarily), and (3) returns a pair consisting of the total number of water cells filled and the final grid (as a vector of strings, preserving the original `'X'` and `'.'` cells, but with filled cells now `'X'`). The function must handle the case where no lakes exist (then no filling occurs and total is 0). Edge cases include lakes that are single cells, lakes that touch the grid border (which are not lakes and must be ignored), and grids where all cells are land.

The solution first finds all connected components of `'.'` cells using iterative DFS or BFS, marking visited cells. For each component, we check if any of its cells is on the border of the grid (i.e., row 1, row n, column 1, column m, or the implicit outside border—but since the input grid is exactly n×m, we only check the actual outer border of the grid; any cell at i=1, i=n, j=1, j=m is considered connected to outside, so the component is not a lake). We also track the size of each component and a representative starting cell (for later filling). For each valid lake, we store (size, startRow, startCol) in a min-heap (priority queue) ordered by size, then by row, then by column. After collecting all lakes, we compute how many we need to fill: `toFill = lakeCount - k`. If toFill ≤ 0, no filling happens. Otherwise, we pop the smallest lake from the heap, perform a BFS/DFS from its start cell to mark all cells of that lake as `'X'` (or `'*'` temporarily, but then we set to `'X'`), and accumulate the size. We repeat this until we have filled `toFill` lakes. The final grid is returned with all filled cells turned to `'X'`. Time complexity is O(n·m) for the initial component scan plus O(total cells filled) for the filling phase, overall O(n·m). Space complexity is O(n·m) for the visited arrays and the grid copy.

#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <utility>
#include <algorithm>

// Returns {totalFilled, finalGrid}
std::pair<int, std::vector<std::string>> fillSmallestLakes(
    const std::vector<std::string>& grid,
    int k
) {
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    
    // Work copy
    std::vector<std::string> mapa = grid;
    
    // Visited for initial component detection
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));
    
    // Min-heap: {size, row, col}
    std::priority_queue<
        std::tuple<int,int,int>,
        std::vector<std::tuple<int,int,int>>,
        std::greater<std::tuple<int,int,int>>
    > pq;
    
    int lakeCount = 0;
    
    // Directions: up, left, down, right
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, -1, 0, 1};
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (mapa[i][j] == '.' && !visited[i][j]) {
                // BFS to find component
                std::queue<std::pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int size = 0;
                bool touchesBorder = false;
                int startR = i, startC = j;
                
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    ++size;
                    
                    // Check if on outer border
                    if (x == 0 || x == n-1 || y == 0 || y == m-1) {
                        touchesBorder = true;
                    }
                    
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                            mapa[nx][ny] == '.' && !visited[nx][ny]) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
                
                if (!touchesBorder) {
                    pq.push({size, startR, startC});
                    ++lakeCount;
                }
            }
        }
    }
    
    int toFill = lakeCount - k;
    int totalFilled = 0;
    
    // Visited for filling phase
    std::vector<std::vector<bool>> filledVisited(n, std::vector<bool>(m, false));
    
    while (toFill > 0 && !pq.empty()) {
        auto [sz, sr, sc] = pq.top();
        pq.pop();
        
        totalFilled += sz;
        
        // BFS to fill lake
        std::queue<std::pair<int,int>> q;
        q.push({sr, sc});
        filledVisited[sr][sc] = true;
        
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            mapa[x][y] = 'X';
            
            for (int d = 0; d < 4; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m &&
                    mapa[nx][ny] == '.' && !filledVisited[nx][ny]) {
                    filledVisited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
        --toFill;
    }
    
    return {totalFilled, mapa};
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Declare the function from the solution (assume it's in the same translation unit)
std::pair<int, std::vector<std::string>> fillSmallestLakes(
    const std::vector<std::string>& grid,
    int k
);

int main() {
    // Test 1: Simple single lake, need to fill it
    {
        std::vector<std::string> grid = {
            "XXX",
            "X.X",
            "XXX"
        };
        auto [filled, result] = fillSmallestLakes(grid, 0);
        assert(filled == 1);
        assert(result == std::vector<std::string>({"XXX", "XXX", "XXX"}));
    }
    
    // Test 2: Two lakes, keep one (fill the smaller)
    {
        std::vector<std::string> grid = {
            "XXXXX",
            "X...X",
            "X.X.X",
            "X...X",
            "XXXXX"
        };
        // Two lakes: one of size 4 (center), one of size 1 (top middle area? actually the border is all X, so inner areas: the 3x3 area has two separate lakes? Let's design: row1..3 col1..3 has X at (1,1)? Actually define clearly:
        // Better test:
        std::vector<std::string> g2 = {
            "XXXXXX",
            "X....X",
            "X.XX.X",
            "X.XX.X",
            "X....X",
            "XXXXXX"
        };
        // There is a lake of size 4 (corners around the center block) and a lake of size 8 (the outer ring). Actually the center block is 2x2 X, so the ring is a single lake of size 8? Let's compute: outer ring cells: (1,1),(1,2),(1,3),(1,4), (2,1),(2,4),(3,1),(3,4),(4,1),(4,2),(4,3),(4,4) = 12 cells? Wait grid is 6x6, rows 0..5, cols 0..5. The border is all X. Inner area rows1..4 cols1..4: cells (1,1).(1,2).(1,3).(1,4) all '.'? Actually row1: X....X so (1,1) to (1,4) are '.' – but there is a X block at rows2-3 cols2-3. So the inner '.' area is connected? Let's see: (1,1) connects to (1,2) etc, and down to (2,1) but (2,2) is X, so (2,1) is isolated? Actually (2,1) has neighbors (1,1) up, (3,1) down, so it's connected. So the inner area is a single ring? Actually (1,1)-(2,1)-(3,1)-(4,1)-(4,2)-(4,3)-(4,4)-(3,4)-(2,4)-(1,4)-(1,3)-(1,2) is a ring, plus the center X block. So it's one lake of size 12? Let's count: positions (1,1),(1,2),(1,3),(1,4) = 4; (2,1),(2,4)=2; (3,1),(3,4)=2; (4,1),(4,2),(4,3),(4,4)=4; total 12. So there is only one lake. To have two lakes, put an X in the middle to split.
        // Simpler: use a 5x5 with two separate '.' regions.
        std::vector<std::string> g3 = {
            "XXXXX",
            "X...X",
            "XXXXX",
            "X...X",
            "XXXXX"
        };
        auto [filled3, result3] = fillSmallestLakes(g3, 1);
        // Lakes: two of size 3 each. Need to fill one (smallest, tie any). Filled=3, result has one lake remaining.
        assert(filled3 == 3);
        // Count '.' in result: should be 3
        int dots = 0;
        for (const auto& row : result3) for (char c : row) if (c == '.') ++dots;
        assert(dots == 3);
    }
    
    // Test 3: No lakes
    {
        std::vector<std::string> grid = {
            "XXX",
            "XXX",
            "XXX"
        };
        auto [filled, result] = fillSmallestLakes(grid, 0);
        assert(filled == 0);
        assert(result == grid);
    }
    
    // Test 4: Lake touches border but not counted
    {
        std::vector<std::string> grid = {
            ".XX",
            "XXX",
            "XXX"
        };
        auto [filled, result] = fillSmallestLakes(grid, 0);
        assert(filled == 0); // no lakes because touches border
        assert(result == grid);
    }
    
    // Test 5: Multiple lakes, keep exactly k=2
    {
        std::vector<std::string> grid = {
            "XXXXXXX",
            "X.....X",
            "X.XXX.X",
            "X.X.X.X",
            "X.XXX.X",
            "X.....X",
            "XXXXXXX"
        };
        // Lakes: outer ring size? Actually the outer ring is one lake, center single is another? Let's see: outer ring is all '.' except the center block? Actually the pattern: there is a 3x3 X block in the middle, surrounded by a ring of '.' (size 16?), and the center has one '.'? Wait row2 col2 is '.'? Let's map: rows 0..6, cols 0..6. Row1: X.....X -> col1..5 are '.', but row2: X.XXX.X -> col2-4 are X, col1 and col5 are '.', row3: X.X.X.X -> col1,3,5 are '.', row4 same as row2, row5 same as row1. So the inner structure: There is a cross of X at rows2-4, cols2-4? Actually col3 is X in rows2-4, col3 row3 is X? Wait row3: X.X.X.X, so col2 is '.', col3 is 'X', col4 is '.', col5 is 'X'? Actually row3: index0 X, index1 '.', index2 X, index3 '.', index4 X, index5 '.', index6 X. So there are multiple separate '.' components? Let's identify: The main ring: (1,1)-(1,2)-(1,3)-(1,4)-(1,5) down to (2,5) etc, and also (2,1) and (4,1) etc. But the center has a small '.' at (3,3)? Row3 col3 is '.'? Row3 is "X.X.X.X": indices 0 X,1 '.',2 X,3 '.',4 X,5 '.',6 X. So (3,3) is '.'. But is it connected to anything? It's surrounded by X at (2,3),(4,3),(3,2),(3,4) all X. So it's a lake of size 1. The outer ring is connected? Let's check (1,2) connects to (2,2)? row2 col2 is X, so no. Actually (1,2) has down (2,2) X, left (1,1) '.', right (1,3) '.', so it's part of a larger component. Let's trace: (1,1) connects to (1,2),(1,3),(1,4),(1,5) and then down to (2,5), (3,5), (4,5), (5,5) etc. Also (2,1) connects to (1,1) and (3,1) etc. So the outer ring is a single lake. So we have two lakes: outer ring (size? count: perimeter cells: top row 5, bottom row 5, left col rows2-4 (3 cells), right col rows2-4 (3 cells) = 5+5+3+3=16) and center single (size 1). If we want k=1, we fill the smallest (size 1) and keep the ring. Let's test:
        auto [filled, result] = fillSmallestLakes(grid, 1);
        assert(filled == 1);
        assert(result[3][3] == 'X'); // center filled
        // Count '.' should be 16
        int dots = 0;
        for (const auto& row : result) for (char c : row) if (c == '.') ++dots;
        assert(dots == 16);
    }
    
    return 0;
}

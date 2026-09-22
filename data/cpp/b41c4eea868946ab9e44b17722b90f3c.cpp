// Write a C++ function that simulates a sequence of rectangular boundary rotations and local averaging on an \( n \times m \) integer grid. Given the grid dimensions \( n, m \) and \( q \) queries, each query specifies a rectangle defined by top-left \((r_1, c_1)\) and bottom-right \((r_2, c_2)\) (1-indexed, inclusive). For each rectangle, you must first rotate its boundary cells clockwise by one position (the top-left corner moves to its right neighbor, the top row shifts right, the right column shifts down, the bottom row shifts left, and the left column shifts up), then replace every cell inside the rectangle with the floor of the average of its own value and the values of its four orthogonal neighbors that are inside the entire grid (i.e., ignore out-of-bounds neighbors; note the cell itself is always counted). The function should take the grid, its dimensions, and a list of queries as inputs and return the final grid after processing all queries. The grid values are integers, and during averaging, integer division (floor) is used. The operation must be applied consistently across all queries, with each query’s rotation and averaging applied to the latest grid state. The rectangle boundaries are guaranteed to be within \( 1 \le r_1 \le r_2 \le n \) and \( 1 \le c_1 \le c_2 \le m \).
#include <cassert>
#include <vector>
#include <array>

int main() {
    // Example 1: 2x3 grid, one query covering entire grid
    std::vector<std::vector<int>> grid1 = {
        {1, 2, 3},
        {4, 5, 6}
    };
    std::vector<std::array<int, 4>> queries1 = {{1, 1, 2, 3}};
    auto result1 = simulate_rotations_and_average(grid1, 2, 3, queries1);
    // After rotation: boundary becomes
    // 4 1 2
    // 6 5 3
    // Then average each cell:
    // (0,0): neighbors (0,0)=4, (1,0)=6, (0,1)=1 -> sum=11, avg=3 (3)
    // (0,1): neighbors (0,1)=1, (1,1)=5, (0,0)=4, (0,2)=2 -> sum=12, avg=2 (2)
    // (0,2): neighbors (0,2)=2, (1,2)=3, (0,1)=1 -> sum=6, avg=2 (2)
    // (1,0): neighbors (1,0)=6, (0,0)=4, (1,1)=5 -> sum=15, avg=5 (5)
    // (1,1): neighbors (1,1)=5, (0,1)=1, (1,0)=6, (1,2)=3 -> sum=15, avg=3 (3)
    // (1,2): neighbors (1,2)=3, (0,2)=2, (1,1)=5 -> sum=10, avg=3 (3)
    std::vector<std::vector<int>> expected1 = {{3, 2, 2}, {5, 3, 3}};
    assert(result1 == expected1);

    // Example 2: 1x1 grid
    std::vector<std::vector<int>> grid2 = {{7}};
    std::vector<std::array<int, 4>> queries2 = {{1, 1, 1, 1}};
    auto result2 = simulate_rotations_and_average(grid2, 1, 1, queries2);
    assert(result2 == std::vector<std::vector<int>>{{7}});

    // Example 3: 3x3 grid with a 2x2 subrectangle
    std::vector<std::vector<int>> grid3 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::array<int, 4>> queries3 = {{1, 1, 2, 2}};
    auto result3 = simulate_rotations_and_average(grid3, 3, 3, queries3);
    // Before rotation: subrectangle rows1-2, cols1-2: 1 2 / 4 5
    // Rotate boundary: top-left goes right: -> 4 1 / 5 2
    // Then average each cell in rectangle:
    // (0,0): neighbors: self4, down5, right1 -> sum=10, avg=3 (3)
    // (0,1): self1, down2, left4, right3 -> sum=10, avg=2 (2)
    // (1,0): self5, up4, right2, down8 -> sum=19, avg=4 (4)
    // (1,1): self2, up1, left5, right6, down9 -> sum=23, avg=4 (4)
    // Outside rectangle unchanged: row0 col2=3, row2 col0=7, row2 col1=8, row2 col2=9
    std::vector<std::vector<int>> expected3 = {
        {3, 2, 3},
        {4, 4, 6},
        {7, 8, 9}
    };
    assert(result3 == expected3);

    // Example 4: two queries sequential
    std::vector<std::vector<int>> grid4 = {
        {10, 20},
        {30, 40}
    };
    std::vector<std::array<int, 4>> queries4 = {{1, 1, 2, 2}, {1, 1, 2, 2}};
    auto result4 = simulate_rotations_and_average(grid4, 2, 2, queries4);
    // First query: rotate full grid: -> 30 10 / 40 20? Let's compute.
    // Save 10, left col up: a[0][0]=a[1][0]=30, a[1][0] stays? Actually left col shift up: a[0][0]=a[1][0]=30, a[1][0] unchanged? Wait need correct:
    // left col: row0=row1=30, row1=30 (but we don't change row1's left col because loop goes row<r2 i.e., row0 only, so a[0][0]=a[1][0]=30)
    // bottom row left: a[1][0]=a[1][1]=40
    // right col down: a[1][1]=a[0][1]=20
    // top row right: a[0][1]=a[0][0]=30 (original a[0][0] was already changed to 30? Actually after left col shift, a[0][0]=30, so a[0][1]=30)
    // Then place temp=10 to a[0][1] -> a[0][1]=10
    // So rotated grid: row0: 30 10, row1: 40 20? Let's verify step-by-step:
    // original: a[0][0]=10, a[0][1]=20, a[1][0]=30, a[1][1]=40
    // left col shift: row0 col0 = row1 col0 = 30, so a[0][0]=30, a[1][0] remains 30
    // bottom row shift: col0 col1? Actually bottom row left: for col=0 to col<1 (since c2=1), a[1][0]=a[1][1]=40
    // right col down: row1 col1 = row0 col1 = 20
    // top row right: col1 col0? for col=1 to col>0, a[0][1]=a[0][0]=30
    // place temp=10 at a[0][1] -> a[0][1]=10
    // So after rotation: a[0][0]=30, a[0][1]=10, a[1][0]=40, a[1][1]=20
    // Then average each:
    // (0,0): self30, down40, right10 => sum=80, avg=26 (26)
    // (0,1): self10, down20, left30 => sum=60, avg=20 (20)
    // (1,0): self40, up30, right20 => sum=90, avg=30 (30)
    // (1,1): self20, up10, left40 => sum=70, avg=23 (23)
    // After first query: grid = [[26,20],[30,23]]
    // Second query on same rectangle: rotate that grid
    // Original for second: a[0][0]=26, a[0][1]=20, a[1][0]=30, a[1][1]=23
    // left col shift: a[0][0]=a[1][0]=30, a[1][0]=30
    // bottom row shift: a[1][0]=a[1][1]=23
    // right col down: a[1][1]=a[0][1]=20
    // top row right: a[0][1]=a[0][0]=30
    // place temp=26 at a[0][1] -> a[0][1]=26
    // rotated: a[0][0]=30, a[0][1]=26, a[1][0]=23, a[1][1]=20
    // average each:
    // (0,0): self30, down23, right26 => sum=79, avg=26 (26)
    // (0,1): self26, down20, left30 => sum=76, avg=25 (25)
    // (1,0): self23, up30, right20 => sum=73, avg=24 (24)
    // (1,1): self20, up26, left23 => sum=69, avg=23 (23)
    // Final: [[26,25],[24,23]]
    std::vector<std::vector<int>> expected4 = {{26, 25}, {24, 23}};
    assert(result4 == expected4);

    // Example 5: rectangle with height 1 (single row) width>1
    std::vector<std::vector<int>> grid5 = {{1, 2, 3, 4}};
    std::vector<std::array<int, 4>> queries5 = {{1, 1, 1, 4}};
    auto result5 = simulate_rotations_and_average(grid5, 1, 4, queries5);
    // Rotate boundary of a single row: left col shift (no-op because row<row? start_row==end_row, loop doesn't run)
    // bottom row shift: left shift entire row: for col=0..2, a[0][col] = a[0][col+1] -> becomes [2,3,4,4]
    // right col down: no-op
    // top row right: for col=3..1, a[0][col]=a[0][col-1] -> a[0][3]=a[0][2]=4? Wait, careful: after bottom shift, row is [2,3,4,4], then top row right: col=3: a[0][3]=a[0][2]=4; col=2: a[0][2]=a[0][1]=3; col=1: a[0][1]=a[0][0]=2 -> now [2,2,3,4]? Actually let's compute: after bottom shift: [2,3,4,4]. Then top right shift:
    // col=3: a[0][3]=a[0][2]=4 -> becomes [2,3,4,4] (no change)
    // col=2: a[0][2]=a[0][1]=3 -> becomes [2,3,3,4]
    // col=1: a[0][1]=a[0][0]=2 -> becomes [2,2,3,4]
    // Then place temp (original a[0][0]=1) at a[0][1] -> becomes [2,1,3,4]
    // Averaging each cell (only row 0, consider horizontal neighbors, self):
    // (0,0): self2, right1 => sum=3, avg=1 (1)
    // (0,1): self1, left2, right3 => sum=6, avg=2 (2)
    // (0,2): self3, left1, right4 => sum=8, avg=2 (2)
    // (0,3): self4, left3 => sum=7, avg=3 (3)
    // Final: [[1,2,2,3]]
    std::vector<std::vector<int>> expected5 = {{1, 2, 2, 3}};
    assert(result5 == expected5);

    return 0;
}
#include <vector>
#include <algorithm>

// Simulate rectangle boundary rotation and local averaging on a grid.
// grid: current state, modified in-place? We'll return a copy to avoid side effects.
std::vector<std::vector<int>> simulate_rotations_and_average(
    const std::vector<std::vector<int>>& input_grid,
    int n, int m,
    const std::vector<std::array<int, 4>>& queries) {
    
    // Copy the input grid to work on
    std::vector<std::vector<int>> a = input_grid;
    // Temporary array for averaging
    std::vector<std::vector<int>> temp_arr(n, std::vector<int>(m, 0));

    auto in_range = [n, m](int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < m;
    };

    auto rotate = [&](int r1, int c1, int r2, int c2) {
        // Convert to 0-indexed internal
        // r1, c1, r2, c2 are 0-indexed here
        // Save top-left value
        int temp = a[r1][c1];
        // Shift left column up (from top to bottom-1)
        for (int row = r1; row < r2; row++) {
            a[row][c1] = a[row+1][c1];
        }
        // Shift bottom row left (from left to right-1)
        for (int col = c1; col < c2; col++) {
            a[r2][col] = a[r2][col+1];
        }
        // Shift right column down (from bottom to top+1)
        for (int row = r2; row > r1; row--) {
            a[row][c2] = a[row-1][c2];
        }
        // Shift top row right (from right to left+1)
        for (int col = c2; col > c1; col--) {
            a[r1][col] = a[r1][col-1];
        }
        // Place saved temp to the right of top-left, only if rectangle has width >= 2
        if (c2 > c1) {
            a[r1][c1+1] = temp;
        }
    };

    auto average = [&](int x, int y) {
        int dx[5] = {0, 1, -1, 0, 0};
        int dy[5] = {0, 0, 0, 1, -1};
        long long sum = 0;
        int cnt = 0;
        for (int d = 0; d < 5; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (in_range(nx, ny)) {
                sum += a[nx][ny];
                cnt++;
            }
        }
        return static_cast<int>(sum / cnt);
    };

    auto set_average = [&](int r1, int c1, int r2, int c2) {
        // Compute averages into temp
        for (int row = r1; row <= r2; row++) {
            for (int col = c1; col <= c2; col++) {
                temp_arr[row][col] = average(row, col);
            }
        }
        // Copy back
        for (int row = r1; row <= r2; row++) {
            for (int col = c1; col <= c2; col++) {
                a[row][col] = temp_arr[row][col];
            }
        }
    };

    for (const auto& q : queries) {
        int r1 = q[0] - 1; // convert to 0-index
        int c1 = q[1] - 1;
        int r2 = q[2] - 1;
        int c2 = q[3] - 1;
        // Step 1: rotate boundary
        rotate(r1, c1, r2, c2);
        // Step 2: average inside rectangle
        set_average(r1, c1, r2, c2);
    }

    return a;
}
// The solution processes each query sequentially. For every query, two phases occur. First, the boundary rotation: store the top-left corner’s value, then shift the left column up (excluding the top cell), shift the bottom row left (excluding the rightmost cell), shift the right column down (excluding the bottom cell), and finally shift the top row right (excluding the leftmost cell). After completing the four shifts, place the saved top-left value into the cell immediately to the right of the top-left corner. This correctly performs a clockwise rotation of the rectangle’s perimeter. Second, the averaging: compute the average for each cell inside the rectangle using its own value plus any valid orthogonal neighbors (up, down, left, right) that lie within the grid. Use a temporary array to store the computed averages to avoid modifying the grid during computation, then copy the temporary values back. Critical edge cases include rectangles of width or height 1 (where rotation shifts only a line and the top-left corner’s right neighbor may not exist if width is 1; handle by ensuring the placement step only occurs if the rectangle has width ≥ 2, otherwise the rotation still works with a degenerate loop; also ensure that for a 1×1 rectangle, rotation does nothing and averaging is just the cell’s own value). For averaging, a cell at a grid boundary uses only available neighbors; self is always counted. Time complexity per query is \( O(h \times w) \) where \( h = r_2 - r_1 + 1 \) and \( w = c_2 - c_1 + 1 \), due to rotation traversing the perimeter (O(h+w)) and averaging traversing the entire rectangle (O(hw)), so overall \( O(q \cdot n \cdot m) \) in worst case. Space complexity is \( O(n \times m) \) for the temporary array.

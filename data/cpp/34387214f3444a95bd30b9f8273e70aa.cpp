/*
Write a C++ function `std::vector<std::vector<int>> generateSpiral(int N)` that takes a positive integer `N` and returns an `N x N` matrix (as a vector of vectors of ints) filled with the integers from 1 to N² in an outward‑spiral order starting from the top‑left cell (row 0, column 0) and moving initially rightward, then down, then left, then up, then right again, and so on. The spiral must fill the matrix completely, and the function must handle the edge case N = 1 gracefully. The output matrix must have exactly N rows and N columns, with each cell containing a unique integer from 1 to N².
*/
#include <vector>

// Generate an N x N matrix filled with numbers 1..N^2 in outward spiral order.
std::vector<std::vector<int>> generateSpiral(int N) {
    // Initialize an N x N matrix with zeros.
    std::vector<std::vector<int>> a(N, std::vector<int>(N, 0));
    
    // Direction deltas: right, down, left, up.
    const int dr[] = {0, 1, 0, -1};
    const int dc[] = {1, 0, -1, 0};
    
    int cnt = 1;
    int r = 0, c = 0;
    a[r][c] = cnt;
    
    while (true) {
        bool move = false;
        for (int d = 0; d < 4; ++d) {
            // Try to move as far as possible in this direction.
            while (true) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= N || nc < 0 || nc >= N || a[nr][nc] > 0) {
                    break;
                }
                // Move one step.
                r = nr;
                c = nc;
                a[r][c] = ++cnt;
                move = true;
            }
        }
        // If no move was made in a full cycle, the spiral is complete.
        if (!move) break;
    }
    
    return a;
}
#include <cassert>
#include <vector>

// The solution function (declared externally or defined here for the test).
std::vector<std::vector<int>> generateSpiral(int N);

int main() {
    // N = 1
    std::vector<std::vector<int>> m1 = generateSpiral(1);
    assert(m1.size() == 1 && m1[0].size() == 1);
    assert(m1[0][0] == 1);

    // N = 2
    std::vector<std::vector<int>> m2 = generateSpiral(2);
    std::vector<std::vector<int>> expected2 = {{1, 2}, {4, 3}};
    assert(m2 == expected2);

    // N = 3
    std::vector<std::vector<int>> m3 = generateSpiral(3);
    std::vector<std::vector<int>> expected3 = {{1, 2, 3}, {8, 9, 4}, {7, 6, 5}};
    assert(m3 == expected3);

    // N = 4
    std::vector<std::vector<int>> m4 = generateSpiral(4);
    std::vector<std::vector<int>> expected4 = {
        {1, 2, 3, 4},
        {12, 13, 14, 5},
        {11, 16, 15, 6},
        {10, 9, 8, 7}
    };
    assert(m4 == expected4);

    // N = 5, check first row, last column, and last row
    std::vector<std::vector<int>> m5 = generateSpiral(5);
    assert(m5[0][0] == 1 && m5[0][1] == 2 && m5[0][4] == 5);
    assert(m5[1][4] == 6 && m5[4][4] == 17 && m5[4][0] == 21);
    // Ensure all numbers 1..25 appear exactly once
    std::vector<int> seen(26, 0);
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            seen[m5[i][j]]++;
    for (int v = 1; v <= 25; ++v) assert(seen[v] == 1);

    // N = 6, check a few values
    std::vector<std::vector<int>> m6 = generateSpiral(6);
    assert(m6[0][0] == 1 && m6[0][5] == 6);
    assert(m6[5][5] == 7 && m6[5][0] == 24 && m6[2][2] == 19);

    // N = 0 (edge case, though spec says positive, but test graceful behavior)
    std::vector<std::vector<int>> m0 = generateSpiral(0);
    assert(m0.empty());

    return 0;
}
// The algorithm simulates the movement of a "turtle" that starts at (0,0) with the number 1 placed there. It then repeatedly attempts to move in a fixed priority order: right, down, left, up. For each direction, it keeps moving straight as long as the next cell is inside the matrix and has not yet been filled (i.e., its value is still 0, indicating unvisited). When it can no longer move in that direction, it tries the next direction in the priority order. If none of the four directions leads to a valid move, the spiral is complete. The `move` flag tracks whether any cell was filled in the current full pass over the four directions; if not, the loop terminates. This approach fills every cell because the spiral will eventually cover the entire N×N grid. For N = 1, only the starting cell is filled, and then no moves are possible, so the loop exits immediately. Time complexity is O(N²) because each cell is visited exactly once; space complexity is O(N²) for storing the result matrix. The method avoids recursion and uses only simple loops and a direction‑delta array.

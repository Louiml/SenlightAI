// Write a C++ function that processes a rectangular binary image (2D grid of 0s and 1s) and returns a `std::map<int, int>` mapping each connected-component size (in terms of number of pixels) to the count of connected components having that exact size. A connected component is defined as a maximal group of adjacent 1‑cells (up, down, left, right — no diagonal adjacency). The input is an `std::vector<std::vector<int>>` where each inner vector has the same length. The function must not modify the input image. If the grid is empty or contains only zeros, return an empty map. The output map must be sorted by size in ascending order (as `std::map` naturally does).
The solution uses a depth‑first search (DFS) flood fill. We maintain a `visited` matrix of the same dimensions as the input to avoid reprocessing cells. For each unvisited cell that contains a `1`, we start a DFS that counts the number of connected `1`s using recursion. The DFS moves only to valid neighboring cells (within bounds, not visited, and value `1`). We track the count for that component, then increment the map entry for that size. The algorithm iterates over all cells, so every cell is visited at most once; thus the time complexity is \(O(R \times C)\) where \(R\) is the number of rows and \(C\) the number of columns. Space complexity is \(O(R \times C)\) for the visited matrix plus the recursion stack in the worst case (when the whole grid is one large component), giving \(O(R \times C)\) auxiliary space. Edge cases include an empty grid, a grid with no `1`, and a grid where all `1`s form one connected component. The dimensions must be consistent (all rows have equal length); if not, we can treat the row length as the first row's size and ignore extra columns, but for a well‑formed input we assume equal lengths.
#include <map>
#include <vector>
#include <functional>

// Counts connected components of 1s in a binary image.
// Returns a map: size -> number of components with that size.
std::map<int, int> countConnectedComponents(const std::vector<std::vector<int>>& image) {
    std::map<int, int> result;
    if (image.empty() || image[0].empty()) return result;

    const int rows = static_cast<int>(image.size());
    const int cols = static_cast<int>(image[0].size());

    // Visited matrix, initialized to false.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Depth‑first search function to count cells in a component.
    std::function<int(int, int)> dfs = [&](int r, int c) -> int {
        // Bounds and visited or zero check.
        if (r < 0 || r >= rows || c < 0 || c >= cols) return 0;
        if (visited[r][c] || image[r][c] != 1) return 0;

        visited[r][c] = true;
        int count = 1;
        count += dfs(r - 1, c); // up
        count += dfs(r + 1, c); // down
        count += dfs(r, c - 1); // left
        count += dfs(r, c + 1); // right
        return count;
    };

    // Iterate over all cells.
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!visited[r][c] && image[r][c] == 1) {
                int size = dfs(r, c);
                result[size]++;
            }
        }
    }

    return result;
}
#include <cassert>
#include <map>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Simple 2x2 with one component of size 4.
    std::vector<std::vector<int>> img1 = {{1,1},{1,1}};
    std::map<int,int> expected1 = {{4,1}};
    assert(countConnectedComponents(img1) == expected1);

    // Test 2: Two separate components of different sizes.
    std::vector<std::vector<int>> img2 = {{1,0,1},{0,0,0},{1,1,1}};
    std::map<int,int> expected2 = {{1,2},{3,1}}; // two singletons, one component of size 3?
    // Actually: top-left 1 and top-right 1 are separate singletons (no adjacency). Bottom row has three 1s -> size 3.
    assert(countConnectedComponents(img2) == expected2);

    // Test 3: All zeros -> empty map.
    std::vector<std::vector<int>> img3 = {{0,0},{0,0}};
    assert(countConnectedComponents(img3).empty());

    // Test 4: Single cell.
    std::vector<std::vector<int>> img4 = {{1}};
    std::map<int,int> expected4 = {{1,1}};
    assert(countConnectedComponents(img4) == expected4);

    // Test 5: Empty grid.
    std::vector<std::vector<int>> img5 = {};
    assert(countConnectedComponents(img5).empty());

    // Test 6: Diagonal ones are not connected.
    std::vector<std::vector<int>> img6 = {{1,0},{0,1}};
    std::map<int,int> expected6 = {{1,2}};
    assert(countConnectedComponents(img6) == expected6);

    // Test 7: Large component with a hole of zeros.
    std::vector<std::vector<int>> img7 = {{1,1,1},{1,0,1},{1,1,1}};
    std::map<int,int> expected7 = {{8,1}};
    assert(countConnectedComponents(img7) == expected7);

    // Test 8: Multiple sizes.
    std::vector<std::vector<int>> img8 = {{1,1,0,0},{0,1,0,1},{0,0,1,1}};
    // Components: top-left 2x2 block? Actually row0 col0,col1 -> (0,0),(0,1),(1,1) form size 3.
    // Then (1,3) alone? It's adjacent to (2,2),(2,3)? (1,3) is adjacent to (2,3) and (2,2) not adjacent? (1,3) has down (2,3) -> yes, so (1,3),(2,3),(2,2) form size 3.
    // Total: two components of size 3.
    std::map<int,int> expected8 = {{3,2}};
    assert(countConnectedComponents(img8) == expected8);

    return 0;
}

// Write a C++ function `int countRooms(const std::vector<std::string>& grid)` that takes a rectangular grid of characters where `'#'` denotes a wall and `'.'` (or any other non-`#` character) denotes a floor. The function must return the number of distinct connected components (rooms) of floor cells, where two floor cells are connected if they share an edge (up, down, left, or right), not just a corner. The grid has at least 1 row and at least 1 column, and can contain any mix of `#` and non-`#` characters. You may assume the grid is non-empty. The function should not modify the input. Hint: treat each non-wall cell as an open vertex in a graph and use BFS or DFS to count connected components.

The problem asks to count connected components in a 2D grid, where each cell is either a wall (`#`) or traversable (anything else). The main algorithm is to iterate through every cell; when we find an unvisited traversable cell, we increment a component counter and perform a flood-fill (BFS or DFS) to mark all cells in that component as visited. This ensures each connected region is counted exactly once. Key edge cases: a grid with no floors returns 0; a grid with all floors returns 1; single-cell grids work; grids with only walls work; and diagonal adjacency does not count as connectivity. We can use a visited Boolean matrix (same dimensions as input) to avoid modifying the input. Time complexity is O(rows × cols) because each cell is visited at most once during the entire process, and each cell is checked a constant number of times in the main loop. Space complexity is O(rows × cols) for the visited matrix and, if BFS is implemented recursively, the recursion stack could go up to O(rows × cols) in the worst case; a queue-based BFS uses O(rows × cols) as well in the worst case (e.g., a large open area). The solution is straightforward and robust.

#include <vector>
#include <queue>
#include <utility>

// Count the number of distinct connected floor regions in a grid.
// Walls are represented by '#', all other characters are considered floor.
int countRooms(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return 0;
    }

    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());

    // visited matrix, false means not visited yet
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    // Directions: up, down, left, right
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    int room_count = 0;

    // Lambda to check if a cell is inside the grid and is a floor.
    auto is_floor = [&](int x, int y) -> bool {
        if (x < 0 || x >= rows || y < 0 || y >= cols) {
            return false;
        }
        return grid[x][y] != '#';
    };

    // Iterate over all cells
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // If this cell is a floor and not visited, start a new component
            if (is_floor(i, j) && !visited[i][j]) {
                ++room_count;

                // BFS to mark the entire component
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();

                    for (int dir = 0; dir < 4; ++dir) {
                        int nx = x + dx[dir];
                        int ny = y + dy[dir];
                        if (is_floor(nx, ny) && !visited[nx][ny]) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
    }

    return room_count;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.
int main() {
    // Example from the snippet
    std::vector<std::string> grid1 = {
        "########",
        "#..##.##",
        "####...#",
        "#..#####",
        "########"
    };
    assert(countRooms(grid1) == 4);

    // All walls
    std::vector<std::string> all_walls = {"###", "###", "###"};
    assert(countRooms(all_walls) == 0);

    // All floors
    std::vector<std::string> all_floors = {"...", "...", "..."};
    assert(countRooms(all_floors) == 1);

    // Single cell floor
    std::vector<std::string> single_floor = {"."};
    assert(countRooms(single_floor) == 1);

    // Single cell wall
    std::vector<std::string> single_wall = {"#"};
    assert(countRooms(single_wall) == 0);

    // Diagonal only, not connected
    std::vector<std::string> diagonal = {".#", "#."};
    assert(countRooms(diagonal) == 2);

    // Two separate small rooms
    std::vector<std::string> two_rooms = {"#.#", "...", "#.#"};
    assert(countRooms(two_rooms) == 2);

    // One large L-shaped room
    std::vector<std::string> l_shape = {
        "..##",
        "..#.",
        "...#",
        "...."
    };
    assert(countRooms(l_shape) == 1);

    // Mixed walls and floors with a ring (holes not counted as separate rooms)
    std::vector<std::string> ring = {
        "#####",
        "#...#",
        "#.#.#",
        "#...#",
        "#####"
    };
    assert(countRooms(ring) == 1);

    // A plus-shaped corridor connecting four branches
    std::vector<std::string> plus = {
        "..#..",
        "..#..",
        "#####",
        "..#..",
        "..#.."
    };
    assert(countRooms(plus) == 1);

    return 0;
}

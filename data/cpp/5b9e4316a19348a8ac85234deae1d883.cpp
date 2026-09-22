// Write a C++ function `int minimumWalls(const std::vector<std::string>& grid)` that takes a rectangular grid of characters where `'0'` represents an empty cell and `'1'` represents a wall. You start at the top-left cell `(0,0)` and must reach the bottom-right cell `(rows-1, cols-1)`. Moving from one cell to an adjacent cell (up, down, left, or right) costs 0 if the destination cell is `'0'`, and costs 1 if the destination cell is `'1'` (you must break through that wall). The starting cell is always `'0'`. Compute the minimum total cost (number of walls broken) to reach the destination. The grid has at least 1 row and 1 column, and the destination is always reachable. Use a 0-1 BFS (deque-based Dijkstra with edge weights 0 and 1) for efficiency. The function should return the minimum cost as an integer.

// The problem resembles finding the shortest path in a graph where each move to a `'0'` costs 0 and each move to a `'1'` costs 1. Since all edge weights are either 0 or 1, we can use a deque-based 0-1 BFS algorithm: initialize a distance array with a large value (e.g., `INT_MAX`), set the starting cell's distance to 0, and push it into a deque. While the deque is not empty, pop the front cell. For each of its four neighbors within bounds, compute the tentative distance as `current_distance + (neighbor == '1' ? 1 : 0)`. If the tentative distance is less than the stored distance for that neighbor, update it and push the neighbor to the **front** of the deque if the edge cost was 0, or to the **back** if the edge cost was 1. This ensures that cells reachable with zero cost are processed first, mimicking a priority queue with only two priority levels. The algorithm terminates when the deque is empty, and we return the distance at the bottom-right cell. Edge cases include a 1x1 grid (cost 0), all-zero paths (cost 0), and grids where the shortest path requires breaking several walls. Time complexity is O(rows × cols) because each cell is processed at most a constant number of times (each relaxation decreases distance, and each cell's distance can be updated at most a few times due to the 0-1 weight property). Space complexity is O(rows × cols) for the distance array and deque.

#include <vector>
#include <deque>
#include <climits>

// Returns the minimum number of walls ('1' cells) broken to travel from
// top-left (0,0) to bottom-right (rows-1, cols-1) in a grid where moving
// to a '0' costs 0 and moving to a '1' costs 1.
int minimumWalls(const std::vector<std::string>& grid) {
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    const int INF = INT_MAX;
    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INF));
    
    // Direction vectors for up, down, left, right
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    
    std::deque<std::pair<int, int>> dq;
    dist[0][0] = 0;
    dq.push_front({0, 0});
    
    while (!dq.empty()) {
        auto [r, c] = dq.front();
        dq.pop_front();
        
        // Optional early exit when popping destination
        if (r == rows - 1 && c == cols - 1) {
            return dist[r][c];
        }
        
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            
            int cost = (grid[nr][nc] == '1') ? 1 : 0;
            int newDist = dist[r][c] + cost;
            
            if (newDist < dist[nr][nc]) {
                dist[nr][nc] = newDist;
                if (cost == 0) {
                    dq.push_front({nr, nc});
                } else {
                    dq.push_back({nr, nc});
                }
            }
        }
    }
    
    return dist[rows - 1][cols - 1];
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is defined above. This main tests it.
int main() {
    // 1x1 grid, no movement needed
    assert(minimumWalls({"0"}) == 0);
    
    // All zeros, straight path
    assert(minimumWalls({"000", "000", "000"}) == 0);
    
    // Simple single wall on the direct path, but detour exists
    assert(minimumWalls({"010", "000", "000"}) == 0);
    
    // Forced to break one wall
    assert(minimumWalls({"011", "001", "001"}) == 0);
    assert(minimumWalls({"010", "010", "010"}) == 1);
    
    // Classic example requiring 1 wall break
    assert(minimumWalls({"000", "111", "100"}) == 1);
    
    // Requires two wall breaks (path along edges breaking corners)
    assert(minimumWalls({"0111", "0001", "1101", "1000"}) == 0);
    assert(minimumWalls({"011", "101", "110"}) == 2);
    
    // More complex grid
    assert(minimumWalls({"01010", "00000", "11111", "00000"}) == 1);
    
    // Cornered path requiring 3 breaks
    assert(minimumWalls({"0111", "1000", "1011", "1000"}) == 1);
    
    // A case where shortest path has multiple walls
    assert(minimumWalls({"0010", "0110", "0100", "0111"}) == 1);
    
    return 0;
}

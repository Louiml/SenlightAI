/*
In a 4x4 mesh network on a chip (NoC), each router has a coordinate (x,y) with x and y ranging from 0 to 3. A packet can move one step in the four cardinal directions (north: dir 0, east: dir 1, south: dir 2, west: dir 3), but each move is valid only if the destination router is within the mesh. Additionally, up to two "faulty links" are specified as a coordinate plus a direction (e.g., at (1,2) east is broken), meaning no packet can move in that direction from that router. Write a C++ function `int route(int sx, int sy, int dx, int dy, int fx1, int fy1, int fd1, int fx2, int fy2, int fd2)` that computes the minimum number of moves required to send a packet from source `(sx,sy)` to destination `(dx,dy)` while avoiding the given faulty links (the second faulty link may be a dummy with negative coordinates to indicate none). If no path exists, return 1000. The function must handle any combination of faulty links, including cases where the source equals the destination (return 0), faulty links are out-of-bounds or point outside the mesh (ignore them), and two faulty links may be identical (treat as one). Do not use global variables; the function must be self-contained and pure.
*/
#include <queue>
#include <vector>
#include <utility>

// Compute minimum moves on a 4x4 mesh, avoiding up to two faulty links.
// Each link is (x, y, dir) where dir: 0=north, 1=east, 2=south, 3=west.
// A dummy fault has negative coordinates or negative direction (ignored).
// Returns 1000 if unreachable.
int route(int sx, int sy, int dx, int dy,
          int fx1, int fy1, int fd1,
          int fx2, int fy2, int fd2) {
    const int N = 4; // mesh dimension 0..3
    
    // If source equals destination, no moves needed.
    if (sx == dx && sy == dy) return 0;

    // Helper to check if a move from (x,y) in direction d is valid.
    auto is_valid_move = [&](int x, int y, int d) -> bool {
        // Check target is inside mesh.
        if (d == 0 && y + 1 >= N) return false; // north
        if (d == 1 && x + 1 >= N) return false; // east
        if (d == 2 && y - 1 < 0)   return false; // south
        if (d == 3 && x - 1 < 0)   return false; // west
        return true;
    };

    // Build set of blocked moves (x, y, dir) from valid faulty links.
    // We store as a vector and check manually; size is tiny.
    std::vector<std::tuple<int,int,int>> blocked;
    auto add_fault = [&](int fx, int fy, int fd) {
        // Ignore dummy faults.
        if (fx < 0 || fy < 0 || fd < 0) return;
        // Only consider if the fault is inside mesh and points to a valid neighbor.
        if (fx < 0 || fx >= N || fy < 0 || fy >= N) return;
        if (fd < 0 || fd > 3) return;
        if (!is_valid_move(fx, fy, fd)) return;
        // Deduplicate.
        for (const auto& b : blocked) {
            if (std::get<0>(b) == fx && std::get<1>(b) == fy && std::get<2>(b) == fd) {
                return;
            }
        }
        blocked.emplace_back(fx, fy, fd);
    };
    add_fault(fx1, fy1, fd1);
    add_fault(fx2, fy2, fd2);

    // BFS from (sx, sy).
    std::vector<std::vector<int>> dist(N, std::vector<int>(N, -1));
    std::queue<std::pair<int,int>> q;
    dist[sx][sy] = 0;
    q.push({sx, sy});

    // Direction vectors: 0=north (0,1), 1=east (1,0), 2=south (0,-1), 3=west (-1,0)
    const int dx_dir[4] = {0, 1, 0, -1};
    const int dy_dir[4] = {1, 0, -1, 0};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        int d = dist[x][y];

        for (int dir = 0; dir < 4; ++dir) {
            // Check if move is valid (in mesh) and not blocked.
            if (!is_valid_move(x, y, dir)) continue;
            bool is_blocked = false;
            for (const auto& b : blocked) {
                if (std::get<0>(b) == x && std::get<1>(b) == y && std::get<2>(b) == dir) {
                    is_blocked = true;
                    break;
                }
            }
            if (is_blocked) continue;

            int nx = x + dx_dir[dir];
            int ny = y + dy_dir[dir];
            if (dist[nx][ny] == -1) {
                dist[nx][ny] = d + 1;
                if (nx == dx && ny == dy) return d + 1;
                q.push({nx, ny});
            }
        }
    }

    return 1000; // unreachable
}
#include <cassert>

// Global main for testing the route function (built-in test harness).
int main() {
    // Basic direct path (same row, east 2 moves)
    assert(route(0,0, 2,0, -9,-9,-1, -9,-9,-1) == 2);
    // Diagonal (2 moves: east then north or north then east)
    assert(route(0,0, 1,1, -9,-9,-1, -9,-9,-1) == 2);
    // Source equals destination
    assert(route(2,3, 2,3, 0,1,0, -9,-9,-1) == 0);
    // One fault blocking the direct path, detour required
    // From (0,0) to (2,0): block (0,0) east; must go north, east, east, south = 4 moves
    assert(route(0,0, 2,0, 0,0,1, -9,-9,-1) == 4);
    // Two faults forcing a longer detour
    // From (0,0) to (1,1): block (0,0) east and (0,0) north; go east? blocked, north? blocked => unreachable
    assert(route(0,0, 1,1, 0,0,1, 0,0,0) == 1000);
    // Faulty link out of bounds (ignored)
    assert(route(0,0, 1,0, 4,4,1, -9,-9,-1) == 1);
    // Faulty link on boundary pointing outside (ignored)
    assert(route(0,0, 1,0, 0,3,0, -9,-9,-1) == 1);
    // Two identical faults treated as one
    assert(route(0,0, 2,0, 0,0,1, 0,0,1) == 4);
    // Faulty link that doesn't affect the path (different location)
    assert(route(0,0, 1,0, 3,3,1, -9,-9,-1) == 1);
    // Destination unreachable by isolating source with all four directions blocked
    assert(route(0,0, 1,0, 0,0,0, 0,0,3) == 1000);
    // Faulty links that lead to a longer but valid path (e.g., block east twice, go around)
    // From (0,0) to (1,0) only east is needed; block it, try north then east then south: 3 moves
    assert(route(0,0, 1,0, 0,0,1, -9,-9,-1) == 3);
}
// The problem reduces to finding the shortest path in a small 4x4 grid with blocked moves. Since the grid is only 16 nodes and each node has at most 4 outgoing edges, the solution is straightforward: perform a Breadth-First Search (BFS) from the source to the destination. The state is simply the current coordinate `(x,y)`. For each state, consider the four directions: north (x, y+1), east (x+1, y), south (x, y-1), west (x-1, y), but only if the target coordinate is within [0,3] for both x and y. Additionally, a move is blocked if the current coordinate and direction match any valid faulty link. A faulty link is valid if it is inside the mesh (coordinate in [0,3] for x and y) and the direction points to a valid neighbor (e.g., north requires y<3). If a faulty link is out-of-bounds or points outside the mesh, it cannot affect any move and is ignored. Since two faulty links may be identical, the set of blocked edges should be deduplicated. Also, if the source equals the destination, return 0 immediately. BFS explores level by level; each node is enqueued at most once, so the algorithm terminates in O(V+E) = O(16 + 16*4) = O(1) time and O(V) space (constant). The BFS is optimal because edges are unweighted. Edge cases: no faulty links (the dummy with negative coordinates is ignored), two identical faulty links (deduplicate), faulty links that are duplicates but with different directions (that's fine), and cases where the destination is unreachable due to blocked moves (BFS completes without reaching destination, return 1000). The function is `const` correct? It does not modify inputs and only uses local variables, so it is naturally pure.

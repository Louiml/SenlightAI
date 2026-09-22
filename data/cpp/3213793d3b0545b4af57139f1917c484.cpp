/*
Implement a C++ function `generateMapWithRoomsAndCorridors(int width, int height, int roomCount, unsigned int seed)` that returns a `std::vector<std::vector<int>>` representing a map grid where `0` = empty floor (walkable), `1` = room floor, `2` = wall, and `3` = corridor floor. The function must generate a specified number of non-overlapping rectangular rooms (each with width and height between 10 and 40, randomly placed with at least 3 cells margin from edges), then connect all consecutive rooms (in generation order) with a corridor path using a greedy best-first search (A* with Manhattan heuristic, no diagonal moves, zero movement cost for the heuristic-only search). After connecting, mark all cells adjacent (8-directionally) to any walkable cell (0, 1, or 3) that are still empty (0) as walls (2). If a corridor cannot be completed (e.g., due to grid boundaries), stop connecting and return the map as-is. The map must be exactly `width` × `height` in size, with all cells initialized to 0, rooms placed as 1, corridors as 3, and walls as 2. The function must use the provided `seed` for deterministic random generation (use `srand(seed)` and `rand()`). Ensure that rooms never exceed the grid boundaries and never overlap. Return the 2D vector (outer index = row/y, inner = column/x). Provide a complete reference implementation.
*/

#include <vector>
#include <queue>
#include <cstdlib>
#include <cmath>
#include <algorithm>

// Generate a map with rooms, corridors, and walls.
// Returns a 2D vector (row-major: outer vector is y, inner is x) with:
// 0 = empty, 1 = room floor, 2 = wall, 3 = corridor floor.
std::vector<std::vector<int>> generateMapWithRoomsAndCorridors(int width, int height, int roomCount, unsigned int seed) {
    srand(seed);
    std::vector<int> data(width * height, 0);

    struct Room { int x, y, w, h; };
    std::vector<Room> rooms;

    // Generate non-overlapping rooms
    for (int i = 0; i < roomCount; ++i) {
        for (int attempt = 0; attempt < 1000; ++attempt) {
            int w = 10 + rand() % 31;  // 10..40
            int h = 10 + rand() % 31;
            // margin of 3 from edges
            if (width - w - 6 < 3 || height - h - 6 < 3) continue;
            int x = 3 + rand() % (width - w - 6);
            int y = 3 + rand() % (height - h - 6);
            Room candidate = {x, y, w, h};

            bool overlap = false;
            for (const auto& r : rooms) {
                if (!(candidate.x + candidate.w <= r.x || r.x + r.w <= candidate.x ||
                      candidate.y + candidate.h <= r.y || r.y + r.h <= candidate.y)) {
                    overlap = true;
                    break;
                }
            }
            if (!overlap) {
                rooms.push_back(candidate);
                // Fill room cells with 1
                for (int dy = 0; dy < h; ++dy) {
                    for (int dx = 0; dx < w; ++dx) {
                        data[(y + dy) * width + (x + dx)] = 1;
                    }
                }
                break;
            }
        }
    }

    // Helper for Manhattan distance
    auto manhattan = [](int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    };

    // Greedy best-first search to connect two points with a corridor
    auto connect = [&](int sx, int sy, int fx, int fy) {
        const int dirs[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};
        std::vector<int> parent(width * height, -1);
        std::priority_queue<std::pair<int,int>> pq; // (-heuristic, index)
        int startIdx = sy * width + sx;
        int finishIdx = fy * width + fx;
        pq.push({-manhattan(sx,sy,fx,fy), startIdx});
        parent[startIdx] = 4; // mark start as visited (non-negative)

        bool found = false;
        while (!pq.empty()) {
            auto [negH, idx] = pq.top();
            pq.pop();
            int cx = idx % width;
            int cy = idx / width;
            if (cx == fx && cy == fy) {
                found = true;
                break;
            }
            for (int d = 0; d < 4; ++d) {
                int nx = cx + dirs[d][0];
                int ny = cy + dirs[d][1];
                if (nx < 0 || ny < 0 || nx >= width || ny >= height) continue;
                int nIdx = ny * width + nx;
                if (parent[nIdx] == -1) {
                    parent[nIdx] = d;
                    int h = manhattan(nx, ny, fx, fy);
                    pq.push({-h, nIdx});
                }
            }
        }

        if (!found) return; // no path found (shouldn't happen)

        // Mark corridor from finish back to start
        int cx = fx, cy = fy;
        while (!(cx == sx && cy == sy)) {
            int idx = cy * width + cx;
            data[idx] = 3;
            int d = parent[idx];
            if (d == 4) break; // reached start
            cx -= dirs[d][0];
            cy -= dirs[d][1];
        }
        // Mark start as corridor too (if not already a room)
        int startVal = data[sy * width + sx];
        if (startVal != 1) data[sy * width + sx] = 3;
    };

    // Connect consecutive rooms by their centers
    if (rooms.size() >= 2) {
        for (size_t i = 0; i + 1 < rooms.size(); ++i) {
            int sx = rooms[i].x + rooms[i].w / 2;
            int sy = rooms[i].y + rooms[i].h / 2;
            int fx = rooms[i+1].x + rooms[i+1].w / 2;
            int fy = rooms[i+1].y + rooms[i+1].h / 2;
            connect(sx, sy, fx, fy);
        }
    }

    // Generate walls: any empty cell adjacent to walkable (1 or 3) becomes wall (2)
    const int offsets[8][2] = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            if (data[idx] == 0) {
                bool adjacentWalkable = false;
                for (int d = 0; d < 8; ++d) {
                    int nx = x + offsets[d][0];
                    int ny = y + offsets[d][1];
                    if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                        int nb = data[ny * width + nx];
                        if (nb == 1 || nb == 3) {
                            adjacentWalkable = true;
                            break;
                        }
                    }
                }
                if (adjacentWalkable) data[idx] = 2;
            }
        }
    }

    // Convert to 2D vector (outer = y, inner = x)
    std::vector<std::vector<int>> result(height, std::vector<int>(width));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            result[y][x] = data[y * width + x];
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// assumes solution function is declared above

int main() {
    // Test 1: Basic generation with seeded randomness
    auto map1 = generateMapWithRoomsAndCorridors(40, 20, 5, 12345);
    assert(map1.size() == 20);
    for (const auto& row : map1) assert(row.size() == 40);

    // Test 2: All cells have valid values (0-3)
    bool validValues = true;
    for (const auto& row : map1) {
        for (int v : row) {
            if (v < 0 || v > 3) { validValues = false; break; }
        }
        if (!validValues) break;
    }
    assert(validValues);

    // Test 3: At least one room exists (value 1)
    bool hasRoom = false;
    for (const auto& row : map1) {
        for (int v : row) if (v == 1) { hasRoom = true; break; }
        if (hasRoom) break;
    }
    assert(hasRoom);

    // Test 4: Deterministic with same seed
    auto map2 = generateMapWithRoomsAndCorridors(40, 20, 5, 12345);
    assert(map1 == map2);

    // Test 5: Deterministic with different seed produces different maps (likely)
    auto map3 = generateMapWithRoomsAndCorridors(40, 20, 5, 99999);
    assert(map1 != map3);

    // Test 6: Corridor exists connecting rooms (at least one cell of value 3)
    bool hasCorridor = false;
    for (const auto& row : map1) {
        for (int v : row) if (v == 3) { hasCorridor = true; break; }
        if (hasCorridor) break;
    }
    assert(hasCorridor);

    // Test 7: Walls are generated around walkable cells
    bool hasWall = false;
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 40; ++x) {
            if (map1[y][x] == 2) { hasWall = true; break; }
        }
        if (hasWall) break;
    }
    assert(hasWall);

    // Test 8: Edge case - zero rooms (should return all zeros)
    auto mapEmpty = generateMapWithRoomsAndCorridors(10, 10, 0, 42);
    for (const auto& row : mapEmpty) {
        for (int v : row) assert(v == 0);
    }

    // Test 9: Edge case - too many rooms may not all fit, but no crash
    auto mapMany = generateMapWithRoomsAndCorridors(30, 30, 50, 7);
    assert(mapMany.size() == 30);
    for (const auto& row : mapMany) assert(row.size() == 30);

    // Test 10: Small grid still works
    auto mapSmall = generateMapWithRoomsAndCorridors(25, 25, 2, 1);
    assert(mapSmall.size() == 25);
    for (const auto& row : mapSmall) assert(row.size() == 25);

    return 0;
}

// The solution maintains a 1D grid internally for efficiency (size width*height) but returns a 2D vector. First, generate rooms: repeatedly try up to 1000 random positions/sizes per room to find a placement that doesn’t intersect any existing room (using axis-aligned rectangle intersection test). For each successful room, fill its cells with 1. Then, for each pair of consecutive rooms (i and i+1), compute the centers as start/finish points. Use a greedy best-first search (priority queue ordered by Manhattan distance to finish, no cost accumulation since we only care about path existence and not shortest length) to find a path, storing parent direction indices to reconstruct. The search expands from start, avoids out-of-bounds, and stops when reaching finish or queue empties. If path found, mark each cell along the reconstructed path as 3. Edge cases: if a room's center is outside bounds (shouldn't happen given margin), skip; if corridor fails due to no path (e.g., blocked by rooms? but rooms are passable since they are 1, but the search treats all cells as traversable regardless of current value, so it will always find a path unless the grid is completely walled; but since the search doesn't consider walls yet, it always finds a path as long as the grid is finite and start/finish are inside). After all corridors, scan every cell; if a cell is currently 0 and has any neighbor (8-directional) that is 1 or 3 (walkable), set it to 2 (wall). Complexity: room generation O(rooms*1000*rooms) for intersection checks (can be optimized but fine), corridor search O(width*height*log(width*height)) per corridor in worst case, wall generation O(width*height*8). Overall time O(rooms^2 + rooms*width*height*log) ≈ O(width*height*log) for typical sizes. Space O(width*height) for grid and parents.

/*
Implement a C++ function `std::vector<std::pair<int,int>> extractConnections(int width, int height, bool pillar)` that, given the dimensions of a Witness-style puzzle (where `width` and `height` are the number of cells horizontally and vertically, and `pillar` indicates whether the grid wraps horizontally), returns a vector of all initial connection endpoints (each connection is a pair of grid coordinates `(x, y)`). The grid has dimensions `2*width + (pillar ? 0 : 1)` horizontally and `2*height + 1` vertically. Nodes are placed at even coordinates in both directions: x ∈ {0, 2, 4, ..., 2*width} and y ∈ {0, 2, 4, ..., 2*height}. For each node at position `(jx, jy)` where `jx = 2*(j % (width+1))` and `jy = 2*(height - j/(width+1))` for `j` from 0 to `(height+1)*(width+1)-1`, you must add: (1) a vertical connection from `(jx, jy)` to `(jx, jy+1)` if `jy < 2*height` (i.e., the node is not on the bottom row), and (2) a horizontal connection from `(jx, jy)` to `(jx+1, jy)` if `jx < 2*width` (i.e., the node is not on the rightmost column). The order of pushing connections into the vector must match the original code: for each node in row-major order of `j` (increasing), push the vertical connection first (if any) as `(x, y+1)` where `x=jx`, `y=jy`, then the horizontal connection (if any) as `(x+1, y)` where `x=jx`, `y=jy`. The function should return a vector where each element is `std::pair<int,int>` representing the endpoint coordinates of a connection (the starting point is implicit in the order). Your implementation must handle arbitrary positive `width` and `height` (at least 1) and both `pillar = true` and `pillar = false`. Validate that the total number of connections equals `(width+1)*height + width*(height+1)` for non-pillar and also for pillar (the formula is the same). Do not use any external libraries beyond the C++ standard library.
*/

#include <vector>
#include <utility>

// Extract initial connection endpoints for a Witness-style puzzle grid.
// width and height are the number of cells; pillar indicates horizontal wrap.
// Returns a vector of (x, y) endpoint pairs for each connection, in the same
// order as the original Puzzle constructor.
std::vector<std::pair<int,int>> extractConnections(int width, int height, bool pillar) {
    std::vector<std::pair<int,int>> connections;
    // Total nodes: (width+1)*(height+1). Number of connections formula:
    // (width+1)*height + width*(height+1)
    int totalNodes = (height + 1) * (width + 1);
    connections.reserve((width + 1) * height + width * (height + 1));

    for (int j = 0; j < totalNodes; ++j) {
        int jx = 2 * (j % (width + 1));
        int jy = 2 * (height - j / (width + 1));

        // Vertical connection to the node below, if not on bottom row.
        if (jy < 2 * height) {
            connections.emplace_back(jx, jy + 1);
        }
        // Horizontal connection to the node right, if not on rightmost column.
        if (jx < 2 * width) {
            connections.emplace_back(jx + 1, jy);
        }
    }

    // pillar is not used in the connection logic; it only affects later operations.
    (void)pillar;
    return connections;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be available from the section.
std::vector<std::pair<int,int>> extractConnections(int width, int height, bool pillar);

int main() {
    // Test 1: 1x1 grid, non-pillar
    auto conn1 = extractConnections(1, 1, false);
    // Expected connections:
    // Nodes: (0,2), (0,0), (2,2), (2,0) in order j=0..3? Let's compute:
    // j=0: jx=0, jy=2 -> vertical to (0,3) [since jy<2? no, jy=2 is not <2, skip], horizontal to (1,2)
    // j=1: jx=2, jy=2 -> vertical skip, horizontal skip because jx=2 not < 2? Actually jx=2 equals 2*width=2, so skip; so no connections
    // j=2: jx=0, jy=0 -> vertical to (0,1), horizontal to (1,0)
    // j=3: jx=2, jy=0 -> vertical to (2,1), horizontal skip
    // Wait, the above is incorrect; let's recompute systematically.
    // width=1, height=1 => grid width=2 (non-pillar: 2*1+1=3), height=3. Nodes at (0,0),(0,2),(2,0),(2,2). For j=0: x=0,y=2; j=1: x=2,y=2; j=2: x=0,y=0; j=3: x=2,y=0.
    // For each node:
    // j=0 (0,2): vertical? y=2 not <2 => skip. horizontal? x=0 <2 => push (1,2)
    // j=1 (2,2): vertical skip, horizontal? x=2 not <2 => skip
    // j=2 (0,0): vertical? y=0 <2 => push (0,1); horizontal? x=0 <2 => push (1,0)
    // j=3 (2,0): vertical? y=0 <2 => push (2,1); horizontal? x=2 not <2 => skip
    // Total connections: [(1,2), (0,1), (1,0), (2,1)] size=4, matches (1+1)*1+1*(1+1)=4.
    assert(conn1.size() == 4);
    assert(conn1[0] == std::make_pair(1,2));
    assert(conn1[1] == std::make_pair(0,1));
    assert(conn1[2] == std::make_pair(1,0));
    assert(conn1[3] == std::make_pair(2,1));

    // Test 2: 2x2 grid, non-pillar
    auto conn2 = extractConnections(2, 2, false);
    // Expected number: (2+1)*2 + 2*(2+1) = 6 + 6 = 12
    assert(conn2.size() == 12);
    // Verify first few connections manually:
    // Nodes: j=0 (0,4): vertical skip (y=4 not <4), horizontal (1,4)
    // j=1 (2,4): vertical skip, horizontal (3,4)
    // j=2 (4,4): both skip
    // j=3 (0,2): vertical (0,3), horizontal (1,2)
    // j=4 (2,2): vertical (2,3), horizontal (3,2)
    // j=5 (4,2): vertical (4,3), horizontal skip
    // j=6 (0,0): vertical (0,1), horizontal (1,0)
    // j=7 (2,0): vertical (2,1), horizontal (3,0)
    // j=8 (4,0): vertical (4,1), horizontal skip
    // So first six: (1,4),(3,4),(0,3),(1,2),(2,3),(3,2) ...
    assert(conn2[0] == std::make_pair(1,4));
    assert(conn2[1] == std::make_pair(3,4));
    assert(conn2[2] == std::make_pair(0,3));
    assert(conn2[3] == std::make_pair(1,2));
    assert(conn2[4] == std::make_pair(2,3));
    assert(conn2[5] == std::make_pair(3,2));

    // Test 3: 1x1 pillar
    auto conn3 = extractConnections(1, 1, true);
    // Total connections same formula: 4, and pillar doesn't change the initial edges.
    assert(conn3.size() == 4);
    assert(conn3 == conn1); // same as non-pillar for this small case

    // Test 4: 3x2 non-pillar
    auto conn4 = extractConnections(3, 2, false);
    // Expected: (3+1)*2 + 3*(2+1) = 8 + 9 = 17
    assert(conn4.size() == 17);
    // Check that all coordinates are within valid bounds: x in [0, 2*3+0? Actually non-pillar width=2*3+1=7] => x in [0,6], y in [0, 2*2+1=5]
    for (const auto& p : conn4) {
        assert(p.first >= 0 && p.first < 7);
        assert(p.second >= 0 && p.second < 5);
    }

    // Test 5: Larger grid with pillar, ensure size matches formula.
    auto conn5 = extractConnections(4, 3, true);
    assert(conn5.size() == (4+1)*3 + 4*(3+1)); // 15 + 16 = 31
    // Check no duplicate connections? Not required by task, but ensure monotonic by first coordinate? Skip.

    // Test 6: Edge case width=0? Not allowed per spec (positive), but we can test height=1, width=1 already done.

    return 0;
}

// The task is to replicate the connection-initialization logic from the provided `Puzzle` constructor. The algorithm iterates over all nodes in a grid of size `(width+1)` by `(height+1)`. For each node, we compute its x-coordinate as `2 * (j % (width+1))` and y-coordinate as `2 * (height - j/(width+1))`. The iteration order is `j` from 0 up to `(height+1)*(width+1)-1`, which corresponds to traversing nodes row-by-row from top to bottom (since y decreases as j increases). For each node, we add a vertical connection to the node below (increase y by 1) if the current y is less than `2*height`, and a horizontal connection to the node to the right (increase x by 1) if the current x is less than `2*width`. The vertical connection is always pushed before the horizontal one, preserving the original order. The total number of connections is exactly `(width+1)*height` vertical ones (one per node per row, except the bottom row) plus `width*(height+1)` horizontal ones (one per node per column, except the rightmost column). For a pillar grid, horizontal wrapping exists but does not affect this initial connection list since we only add edges between adjacent nodes in the non-wrapped sense; the pillar flag does not change which edges are added here. Edge cases include extremely small grids like `width=1, height=1` (which yields 6 nodes and 4 connections), and ensuring coordinates fit in an `int` (they do). Time complexity is O((width+1)*(height+1)) because we visit each node once and perform constant work. Space complexity is O(width*height) for the returned vector, which holds exactly `(width+1)*height + width*(height+1)` pairs.

// Given a rectangular grid of tiles where each tile can have an overlap (displacement) with its right neighbor or bottom neighbor in three orthogonal directions (x, y, z), write a C++ function that assembles a complete displacement field. The function must accept the grid dimensions, two vectors describing the per-edge displacements (horizontal edges first, then vertical edges), and an initial absolute position for the top-left tile in each direction. It should compute and return the absolute positions for all tiles as a flat vector in row-major order (tile `(r, c)` at index `r * cols + c`), where each entry is a 3-element array `{dx, dy, dz}`. The horizontal edge at row `r`, column `c` connects tile `(r, c)` to `(r, c+1)` and has displacement `dx`, `dy`, `dz`; the vertical edge at row `r`, column `c` connects tile `(r, c)` to `(r+1, c)` and also has three components. The assembly must propagate absolute positions starting from the top-left tile's given initial position, using cumulative displacement sums along each row and each column. The function must return the flat vector of triplets.
The core is to propagate absolute coordinates from the top-left anchor. For each direction d (0=x,1=y,2=z), initialize position of tile (0,0) to the given initial value. Then for each row r from 0 to rows-1: for each column c from 0 to cols-2, set tile (r,c+1) position[d] = tile(r,c) position[d] + horizontal_edge[r*(cols-1)+c][d]. For the next row (r+1,0), set its position[d] = tile(r,0) position[d] + vertical_edge[r*cols][d]. However, since rows are processed sequentially, after finishing an entire row, the next row's first tile gets computed from the first tile of the current row, but note that tile(r,0) has already been assigned when entering the row; the row's first tile was set either from the top row or from the vertical edge of the row above. So proceed iteratively: set (0,0), then fill first row left-to-right, then for each r from 0 to rows-2, set (r+1,0) from (r,0) using vertical_edge[r*cols], then fill row r+1 left-to-right using horizontal edges for that row. This yields correct positions because the grid is rectangular and edges form a spanning tree (actually a grid graph, but the process is deterministic). Edge cases: rows=1 or cols=1; handle loops correctly. Complexity O(rows*cols) time and O(rows*cols) space for the output.
#include <vector>
#include <cstddef>

// Structure to hold a 3D displacement or position.
struct Triple {
    int x = 0;
    int y = 0;
    int z = 0;
    
    Triple() = default;
    Triple(int a, int b, int c) : x(a), y(b), z(c) {}
};

/**
 * Assemble absolute positions for all tiles in a grid given edge displacements.
 * 
 * @param rows Number of tile rows (>=1)
 * @param cols Number of tile columns (>=1)
 * @param horizEdges Flat vector of size rows*(cols-1), each entry is a Triple for displacement from (r,c) to (r,c+1)
 * @param vertEdges  Flat vector of size (rows-1)*cols, each entry is a Triple for displacement from (r,c) to (r+1,c)
 * @param initialPosition Triple for absolute position of tile (0,0)
 * @return Flat vector of size rows*cols, each entry is Triple absolute position of tile at row-major index
 */
std::vector<Triple> assembleAbsolutePositions(
    int rows,
    int cols,
    const std::vector<Triple>& horizEdges,
    const std::vector<Triple>& vertEdges,
    const Triple& initialPosition
) {
    std::vector<Triple> positions(rows * cols);
    
    // Assign top-left tile
    positions[0] = initialPosition;
    
    // Fill first row (r=0) left to right
    for (int c = 0; c < cols - 1; ++c) {
        int curIdx = 0 * cols + c;
        int nextIdx = 0 * cols + (c + 1);
        const Triple& h = horizEdges[0 * (cols - 1) + c];
        positions[nextIdx] = {
            positions[curIdx].x + h.x,
            positions[curIdx].y + h.y,
            positions[curIdx].z + h.z
        };
    }
    
    // For each subsequent row, set its first tile from the row above, then fill row
    for (int r = 1; r < rows; ++r) {
        int aboveFirstIdx = (r - 1) * cols + 0;
        int curFirstIdx = r * cols + 0;
        const Triple& v = vertEdges[(r - 1) * cols + 0];
        positions[curFirstIdx] = {
            positions[aboveFirstIdx].x + v.x,
            positions[aboveFirstIdx].y + v.y,
            positions[aboveFirstIdx].z + v.z
        };
        
        // Fill the rest of the row
        for (int c = 0; c < cols - 1; ++c) {
            int curIdx = r * cols + c;
            int nextIdx = r * cols + (c + 1);
            const Triple& h = horizEdges[r * (cols - 1) + c];
            positions[nextIdx] = {
                positions[curIdx].x + h.x,
                positions[curIdx].y + h.y,
                positions[curIdx].z + h.z
            };
        }
    }
    
    return positions;
}
#include <cassert>
#include <vector>

// Declare the function and struct from solution (normally would be in header)
// For demonstration, include the whole solution here (or just declare)
struct Triple {
    int x, y, z;
    Triple(int a=0, int b=0, int c=0) : x(a), y(b), z(c) {}
    bool operator==(const Triple& other) const { return x==other.x && y==other.y && z==other.z; }
};

std::vector<Triple> assembleAbsolutePositions(
    int rows,
    int cols,
    const std::vector<Triple>& horizEdges,
    const std::vector<Triple>& vertEdges,
    const Triple& initialPosition
);

int main() {
    // Test 1: 1x1 grid, no edges, initial (10,20,30)
    {
        std::vector<Triple> result = assembleAbsolutePositions(1, 1, {}, {}, Triple(10,20,30));
        assert(result.size() == 1);
        assert(result[0] == Triple(10,20,30));
    }
    
    // Test 2: 1x3 grid, only horizontal edges, initial (0,0,0)
    {
        std::vector<Triple> h = {Triple(1,2,3), Triple(4,5,6)};
        std::vector<Triple> v = {};
        auto result = assembleAbsolutePositions(1, 3, h, v, Triple(0,0,0));
        assert(result.size() == 3);
        assert(result[0] == Triple(0,0,0));
        assert(result[1] == Triple(1,2,3));
        assert(result[2] == Triple(5,7,9)); // 1+4=5, 2+5=7, 3+6=9
    }
    
    // Test 3: 3x1 grid, only vertical edges, initial (5,5,5)
    {
        std::vector<Triple> h = {};
        std::vector<Triple> v = {Triple(1,0,0), Triple(2,0,0)};
        auto result = assembleAbsolutePositions(3, 1, h, v, Triple(5,5,5));
        assert(result.size() == 3);
        assert(result[0] == Triple(5,5,5));
        assert(result[1] == Triple(6,5,5));
        assert(result[2] == Triple(8,5,5));
    }
    
    // Test 4: 2x2 grid with mixed edges, initial (0,0,0)
    {
        std::vector<Triple> h = {Triple(1,0,0), Triple(0,2,0)}; // row0 hor, row1 hor (only one per row for 2 cols)
        std::vector<Triple> v = {Triple(10,0,0), Triple(0,0,20)}; // col0 vert, col1 vert
        auto result = assembleAbsolutePositions(2, 2, h, v, Triple(0,0,0));
        // Positions:
        // (0,0)=(0,0,0), (0,1)=(1,0,0)
        // (1,0)=(10,0,0), (1,1)=(10,2,0) from (1,0)+horiz(0,2,0)
        assert(result.size() == 4);
        assert(result[0] == Triple(0,0,0));
        assert(result[1] == Triple(1,0,0));
        assert(result[2] == Triple(10,0,0));
        assert(result[3] == Triple(10,2,0));
    }
    
    // Test 5: 3x3 grid, verify consistency
    {
        std::vector<Triple> h;
        // rows=3, cols=3 => horizontal edges = 3*(2)=6
        h.push_back(Triple(1,0,0)); // row0: (0,0)->(0,1)
        h.push_back(Triple(0,1,0)); // row0: (0,1)->(0,2)
        h.push_back(Triple(2,0,0)); // row1
        h.push_back(Triple(0,2,0)); // row1
        h.push_back(Triple(3,0,0)); // row2
        h.push_back(Triple(0,3,0)); // row2
        
        std::vector<Triple> v;
        // vertical edges = (3-1)*3 = 6
        v.push_back(Triple(10,0,0)); // (0,0)->(1,0)
        v.push_back(Triple(20,0,0)); // (0,1)->(1,1)
        v.push_back(Triple(30,0,0)); // (0,2)->(1,2)
        v.push_back(Triple(40,0,0)); // (1,0)->(2,0)
        v.push_back(Triple(50,0,0)); // (1,1)->(2,1)
        v.push_back(Triple(60,0,0)); // (1,2)->(2,2)
        
        auto result = assembleAbsolutePositions(3, 3, h, v, Triple(0,0,0));
        assert(result.size() == 9);
        // Manual check some key tiles:
        // (0,0)=(0,0,0)
        // (1,0)=(10,0,0)
        // (2,0)=(10+40,0,0)=(50,0,0)
        // (0,2)=(1+0, 0+1, 0)=(1,1,0) from (0,0)+(1,0,0)+(0,1,0)
        // (1,2) via row1: from (1,0)+(2,0,0)+(0,2,0) = (10+2, 0+2, 0)=(12,2,0)
        // (2,2) via row2: from (2,0)+(3,0,0)+(0,3,0) = (50+3, 0+3, 0)=(53,3,0)
        assert(result[2] == Triple(1,1,0)); // row0,col2
        assert(result[4] == Triple(12,2,0)); // row1,col1? Actually row1 col1: from (1,0)+row1 horiz (2,0,0)+(0,2,0) => (10+2+0, 0+0+2, 0) = (12,2,0) yes
        assert(result[5] == Triple(12,2,0)); // row1,col2: from (1,1)+row1 hor2? Wait row1 has two horizontal edges, we need row1 col2 = row1 col1 + h[3]? h[3] = (0,2,0) so (12+0,2+2,0)=(12,4,0) but careful: row1 col2 = (row1 col0) + h[2] + h[3] = (10+2+0, 0+0+2, 0) = (12,2,0) because h[2]=(2,0,0) and h[3]=(0,2,0) sum=(2,2,0), so from (10,0,0) to (12,2,0) yes. Actually check consistency: (1,1) should be (12,2,0) and (1,2) = (1,1)+(0,2,0) = (12,4,0). Let's compute manually:
        // row0: (0,0)=(0,0,0), (0,1)=(1,0,0), (0,2)=(1,1,0)
        // row1: (1,0)=(10,0,0) from v[0], then (1,1)=(10+2, 0+0, 0)=(12,0,0) + h[2]? Wait h[2]=(2,0,0) so (1,1) = (10+2,0,0)=(12,0,0). Then h[3]=(0,2,0) gives (1,2)=(12,2,0).
        // row2: (2,0)=(1,0)+v[3]=(10+40,0,0)=(50,0,0). Then h[4]=(3,0,0) gives (2,1)=(53,0,0). h[5]=(0,3,0) gives (2,2)=(53,3,0).
        // So check:
        assert(result[1] == Triple(1,0,0));
        assert(result[2] == Triple(1,1,0));
        assert(result[3] == Triple(10,0,0));
        assert(result[4] == Triple(12,0,0));
        assert(result[5] == Triple(12,2,0));
        assert(result[6] == Triple(50,0,0));
        assert(result[7] == Triple(53,0,0));
        assert(result[8] == Triple(53,3,0));
    }
    
    return 0;
}

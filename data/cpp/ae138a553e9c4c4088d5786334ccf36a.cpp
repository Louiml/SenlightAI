// Write a standalone C++ function `recordSpatialProfile` that simulates the accumulation of dust particle data into a 3D spatial grid. The function must take a 3D grid represented as a `std::vector<std::vector<std::vector<double>>>` (indexed as [x][y][z]), a point represented by a simple struct `Vec3{float x,y,z;}` (already defined), a particle weight (float), and a particle diameter in micrometers (float). It should add the weight to the grid cell containing the point, assuming the grid covers a cubic region from `-50.0` to `50.0` in each axis, with a fixed spatial resolution of `0.5` units per cell. The grid dimensions are therefore 200×200×200. Handle points exactly on the upper boundary (e.g., x=50.0) by clamping them to the last valid index (199). Points outside the range are silently ignored. The function should return `void` and modify the grid in place. The grid is passed by reference, and the point is passed by const reference.

The core task is to map a continuous 3D point to a discrete grid cell. For each axis, compute the index as `floor((coordinate + 50.0) / 0.5)`. Since the grid extends from -50.0 to 50.0 inclusive at the upper boundary, any coordinate exactly 50.0 must be clamped to index 199 (because 50.0 maps to 200, which is out of bounds). For coordinates less than -50.0 or greater than or equal to 50.0 after clamping (except the exact 50.0 case), ignore the point. We must check all three coordinates independently; if any is out of range, the point is ignored entirely. The surface area calculation is not needed here—only weight accumulation. Time complexity is O(1) per call since we only compute three index calculations and do a single array access. Space complexity is O(1) extra space.

#include <vector>
#include <cmath>

struct Vec3 {
    float x, y, z;
};

// Accumulate particle weight into a 3D grid covering [-50,50]^3 with cell size 0.5.
// Grid dimensions are 200x200x200. Points outside range are ignored, boundary clamped.
void recordSpatialProfile(std::vector<std::vector<std::vector<double>>>& grid,
                          const Vec3& point,
                          float weight) {
    const double halfSize = 50.0;
    const double cellSize = 0.5;
    const int gridDim = 200;

    // Convert coordinate to index. Clamp upper boundary exactly at +50.0.
    auto toIndex = [&](double coord) -> int {
        if (coord < -halfSize || coord > halfSize) return -1; // outside
        if (coord == halfSize) return gridDim - 1; // exact upper boundary
        return static_cast<int>(std::floor((coord + halfSize) / cellSize));
    };

    int x = toIndex(point.x);
    if (x < 0) return;
    int y = toIndex(point.y);
    if (y < 0) return;
    int z = toIndex(point.z);
    if (z < 0) return;

    grid[x][y][z] += weight;
}

#include <cassert>
#include <vector>

// Assume Vec3 and recordSpatialProfile are defined above.

int main() {
    std::vector<std::vector<std::vector<double>>> grid(200,
        std::vector<std::vector<double>>(200, std::vector<double>(200, 0.0)));

    // Center of grid
    recordSpatialProfile(grid, Vec3{0.0f, 0.0f, 0.0f}, 1.0f);
    assert(grid[100][100][100] == 1.0);

    // Exact upper boundary clamp
    recordSpatialProfile(grid, Vec3{50.0f, 0.0f, 0.0f}, 2.0f);
    assert(grid[199][100][100] == 2.0);

    // Lower boundary valid
    recordSpatialProfile(grid, Vec3{-50.0f, -50.0f, -50.0f}, 3.0f);
    assert(grid[0][0][0] == 3.0);

    // Just inside upper boundary (49.75 -> index 199)
    recordSpatialProfile(grid, Vec3{49.75f, 49.75f, 49.75f}, 4.0f);
    assert(grid[199][199][199] == 4.0);

    // Just below lower boundary (-50.1) ignored
    recordSpatialProfile(grid, Vec3{-50.1f, 0.0f, 0.0f}, 5.0f);
    assert(grid[0][100][100] == 0.0);

    // Above upper boundary (50.1) ignored
    recordSpatialProfile(grid, Vec3{0.0f, 0.0f, 50.1f}, 6.0f);
    assert(grid[100][100][199] == 0.0);

    // Non-axis aligned point
    recordSpatialProfile(grid, Vec3{10.25f, -20.75f, 30.5f}, 7.0f);
    int x = static_cast<int>((10.25 + 50.0) / 0.5); // 120.5 -> 120
    int y = static_cast<int>((-20.75 + 50.0) / 0.5); // 58.5 -> 58
    int z = static_cast<int>((30.5 + 50.0) / 0.5); // 161.0 -> 161
    assert(grid[x][y][z] == 7.0);

    // Accumulation works
    recordSpatialProfile(grid, Vec3{0.0f, 0.0f, 0.0f}, 10.0f);
    assert(grid[100][100][100] == 11.0);

    return 0;
}

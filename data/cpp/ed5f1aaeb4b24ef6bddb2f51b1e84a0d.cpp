// Write a standalone C++ function named `buildClothGrid` that, given a specified width `w`, height `h`, and two positive integers `gridW` and `gridH`, constructs and returns a 2D vector of `Particle` structs representing a rectangular cloth grid. Each `Particle` must contain a 3D position (using `float` coordinates) and a boolean flag `pinned`. The grid should have exactly `gridW` particles along the x-axis and `gridH` particles along the y-axis. The x-coordinates should range from `0` to `w` inclusive, evenly spaced, and the y-coordinates should range from `0` to `-h` inclusive (negative downward), evenly spaced. The particle at grid index `(0,0)` (top-left) must be pinned, as well as the first three particles in the top row (indices `(0,0)`, `(1,0)`, `(2,0)`) and the first three particles in the bottom row (indices `(0,gridH-1)`, `(1,gridH-1)`, `(2,gridH-1)`). All other particles must be unpinned. The function must be `const`-correct, validate that `gridW >= 4` and `gridH >= 4`, and throw `std::invalid_argument` if invalid dimensions are passed.
The approach is straightforward: allocate a 2D vector of size `gridH` rows and `gridW` columns. For each cell `(j, i)` where `j` is the row index (0 to `gridH-1`) and `i` is the column index (0 to `gridW-1`), compute the x-coordinate as `w * (i / (gridW - 1.0f))` and the y-coordinate as `-h * (j / (gridH - 1.0f))`. This ensures that the first column has x=0, the last column has x=w, the first row has y=0, and the last row has y=-h. The pinned condition: pin if `j == 0` and `i < 3` (top row first three), or if `j == gridH - 1` and `i < 3` (bottom row first three). Note that `(0,0)` is covered by the top row condition. Edge cases: check both dimensions are at least 4 to ensure there are at least 4 columns and 4 rows, making the pinning region valid. Time complexity is O(gridW * gridH) because we iterate over every cell exactly once. Space complexity is also O(gridW * gridH) for the returned vector, which is necessary. The function is `const`-correct by taking all parameters by value and returning a `const`-qualified vector (although returning a mutable vector is fine, we can mark the function parameters as not modified; we'll use `const` where appropriate in the implementation).
#include <vector>
#include <stdexcept>

struct Particle {
    float x, y, z;   // position
    bool pinned;     // true if immovable

    Particle(float x, float y, float z, bool pinned)
        : x(x), y(y), z(z), pinned(pinned) {}
};

// Builds a rectangular cloth grid with evenly spaced particles.
// width and height are the physical dimensions; gridW and gridH are particle counts.
// Pins the first three particles on the top row and the first three on the bottom row.
std::vector<std::vector<Particle>> buildClothGrid(float width, float height, int gridW, int gridH)
{
    if (gridW < 4 || gridH < 4) {
        throw std::invalid_argument("Grid dimensions must be at least 4x4");
    }

    std::vector<std::vector<Particle>> grid;
    grid.reserve(gridH);

    const int lastCol = gridW - 1;
    const int lastRow = gridH - 1;

    for (int j = 0; j < gridH; ++j) {
        std::vector<Particle> row;
        row.reserve(gridW);

        for (int i = 0; i < gridW; ++i) {
            float x = width * (static_cast<float>(i) / lastCol);
            float y = -height * (static_cast<float>(j) / lastRow);
            bool pinned = (j == 0 && i < 3) || (j == lastRow && i < 3);
            row.emplace_back(x, y, 0.0f, pinned);
        }

        grid.push_back(row);
    }

    return grid;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Basic 5x5 grid
    auto grid = buildClothGrid(10.0f, 5.0f, 5, 5);
    assert(grid.size() == 5);
    assert(grid[0].size() == 5);

    // Check x coordinates: 0, 2.5, 5, 7.5, 10
    assert(grid[0][0].x == 0.0f);
    assert(std::fabs(grid[0][1].x - 2.5f) < 1e-5);
    assert(std::fabs(grid[0][2].x - 5.0f) < 1e-5);
    assert(std::fabs(grid[0][3].x - 7.5f) < 1e-5);
    assert(std::fabs(grid[0][4].x - 10.0f) < 1e-5);

    // Check y coordinates: 0, -1.25, -2.5, -3.75, -5
    assert(grid[0][0].y == 0.0f);
    assert(std::fabs(grid[1][0].y + 1.25f) < 1e-5);
    assert(std::fabs(grid[2][0].y + 2.5f) < 1e-5);
    assert(std::fabs(grid[3][0].y + 3.75f) < 1e-5);
    assert(std::fabs(grid[4][0].y + 5.0f) < 1e-5);

    // Check z coordinate is 0
    assert(grid[0][0].z == 0.0f);

    // Check pinned: top row first three, bottom row first three
    assert(grid[0][0].pinned == true);
    assert(grid[0][1].pinned == true);
    assert(grid[0][2].pinned == true);
    assert(grid[0][3].pinned == false);
    assert(grid[4][0].pinned == true);
    assert(grid[4][1].pinned == true);
    assert(grid[4][2].pinned == true);
    assert(grid[4][3].pinned == false);
    assert(grid[1][0].pinned == false);
    assert(grid[3][4].pinned == false);

    // Test 2: Non-square dimensions
    auto grid2 = buildClothGrid(8.0f, 4.0f, 4, 6);
    assert(grid2.size() == 6);
    assert(grid2[0].size() == 4);
    assert(std::fabs(grid2[0][0].x - 0.0f) < 1e-5);
    assert(std::fabs(grid2[0][3].x - 8.0f) < 1e-5);
    assert(std::fabs(grid2[5][0].y + 4.0f) < 1e-5);

    // Test 3: Invalid dimensions throw
    bool threw = false;
    try {
        auto bad = buildClothGrid(1.0f, 1.0f, 3, 5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        auto bad2 = buildClothGrid(1.0f, 1.0f, 5, 2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

Write a C++ function that implements a simplified version of the fluid dynamics grid initialization and boundary condition setup from the provided Vulkan code snippet. Specifically, implement a function that takes dimensions `NX`, `NY`, `NZ`, a cell size, and a particle count, and returns a data structure representing a 3D grid of velocity vectors and pressure scalars, initialized to zero. Additionally, implement a function that applies boundary conditions: for each grid cell on the boundary (where any dimension index is 0 or the maximum), set the velocity to zero (no-slip condition). The solution should simulate the key computational aspect of the simulation grid setup and boundary enforcement, abstracting away all Vulkan-specific details. The task should produce a grid with zero-initialized velocity and pressure, then apply no-slip boundary conditions to set boundary velocities to zero. The function should handle edge cases such as 1x1x1 grids and ensure memory efficiency using a flat 1D array representation.
// The solution involves representing the 3D grid as a flat 1D array (or vector) for both velocity vectors and pressure scalars. The index mapping from 3D coordinates `(x, y, z)` to a flat index uses the formula `index = x + y * NX + z * NX * NY`. The initialization function allocates vectors for velocity (as `glm::vec3` or a simple `struct Vec3` with x, y, z components) and pressure (as `float`), filled with zeros. For the boundary condition function, iterate over all grid cells and check if `x == 0 || x == NX-1 || y == 0 || y == NY-1 || z == 0 || z == NZ-1`. If true, set the velocity at that index to zero. For a 1x1x1 grid, every cell is on the boundary, so all velocities become zero. Time complexity is O(NX*NY*NZ) for both initialization and boundary application, and space complexity is O(NX*NY*NZ) to store the grid data. Edge cases include grids where some dimensions are 1, which still work correctly with the same index formula and boundary checks. The solution uses a struct to hold the grid data (dimensions, velocities, pressures) and provides functions for initialization and boundary enforcement.
#include <vector>
#include <cstdint>
#include <stdexcept>

// Simple 3D vector struct (equivalent to glm::vec3 for this task)
struct Vec3 {
    float x, y, z;
    Vec3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}
    
    bool operator==(const Vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Grid data structure holding dimensions, velocities, and pressures
struct FluidGrid {
    uint32_t NX, NY, NZ;
    float cellSize;
    std::vector<Vec3> velocities;
    std::vector<float> pressures;

    // Flat index calculation for (x, y, z)
    size_t index(uint32_t x, uint32_t y, uint32_t z) const {
        return static_cast<size_t>(x) + static_cast<size_t>(y) * NX + static_cast<size_t>(z) * NX * NY;
    }
};

// Initialize a 3D grid with given dimensions and cell size, all values zero
FluidGrid createInitializedGrid(uint32_t NX, uint32_t NY, uint32_t NZ, float cellSize) {
    if (NX == 0 || NY == 0 || NZ == 0) {
        throw std::invalid_argument("Grid dimensions must be positive");
    }

    FluidGrid grid;
    grid.NX = NX;
    grid.NY = NY;
    grid.NZ = NZ;
    grid.cellSize = cellSize;

    // Total number of cells
    const size_t totalCells = static_cast<size_t>(NX) * NY * NZ;
    
    // Initialize velocities to zero (Vec3 default is zero)
    grid.velocities.assign(totalCells, Vec3(0.0f, 0.0f, 0.0f));
    
    // Initialize pressures to zero
    grid.pressures.assign(totalCells, 0.0f);

    return grid;
}

// Apply no-slip boundary conditions: set boundary cell velocities to zero
void applyBoundaryConditions(FluidGrid& grid) {
    const uint32_t NX = grid.NX;
    const uint32_t NY = grid.NY;
    const uint32_t NZ = grid.NZ;

    // Iterate over all cells and zero out velocities on boundaries
    for (uint32_t z = 0; z < NZ; ++z) {
        for (uint32_t y = 0; y < NY; ++y) {
            for (uint32_t x = 0; x < NX; ++x) {
                // Check if on boundary: any dimension at 0 or max
                if (x == 0 || x == NX - 1 || y == 0 || y == NY - 1 || z == 0 || z == NZ - 1) {
                    // Set velocity to zero at this cell
                    grid.velocities[grid.index(x, y, z)] = Vec3(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Basic 2x2x2 grid initialization
    {
        FluidGrid grid = createInitializedGrid(2, 2, 2, 0.1f);
        assert(grid.NX == 2 && grid.NY == 2 && grid.NZ == 2);
        assert(grid.cellSize == 0.1f);
        assert(grid.velocities.size() == 8);
        assert(grid.pressures.size() == 8);
        for (const auto& vel : grid.velocities) {
            assert(vel == Vec3(0.0f, 0.0f, 0.0f));
        }
        for (float p : grid.pressures) {
            assert(p == 0.0f);
        }
    }

    // Test 2: Boundary conditions on 2x2x2 grid (all cells are boundary)
    {
        FluidGrid grid = createInitializedGrid(2, 2, 2, 0.05f);
        // Set some interior-like values but since 2x2x2 has no interior, all are boundary
        grid.velocities[0] = Vec3(1.0f, 2.0f, 3.0f);
        grid.velocities[7] = Vec3(4.0f, 5.0f, 6.0f);
        grid.pressures[3] = 2.5f;
        
        applyBoundaryConditions(grid);
        // All velocities should be zero
        for (const auto& vel : grid.velocities) {
            assert(vel == Vec3(0.0f, 0.0f, 0.0f));
        }
        // Pressures unchanged
        assert(grid.pressures[3] == 2.5f);
    }

    // Test 3: 3x3x3 grid, only center cell is interior
    {
        FluidGrid grid = createInitializedGrid(3, 3, 3, 0.02f);
        // Set velocities at various positions
        grid.velocities[grid.index(1, 1, 1)] = Vec3(1.0f, 1.0f, 1.0f);  // center (interior)
        grid.velocities[grid.index(0, 0, 0)] = Vec3(2.0f, 2.0f, 2.0f);  // corner (boundary)
        grid.velocities[grid.index(2, 2, 2)] = Vec3(3.0f, 3.0f, 3.0f);  // opposite corner (boundary)
        
        applyBoundaryConditions(grid);
        
        // Center velocity unchanged
        assert(grid.velocities[grid.index(1, 1, 1)] == Vec3(1.0f, 1.0f, 1.0f));
        // Boundary velocities zeroed
        assert(grid.velocities[grid.index(0, 0, 0)] == Vec3(0.0f, 0.0f, 0.0f));
        assert(grid.velocities[grid.index(2, 2, 2)] == Vec3(0.0f, 0.0f, 0.0f));
    }

    // Test 4: 1x1x1 grid (single cell, boundary)
    {
        FluidGrid grid = createInitializedGrid(1, 1, 1, 1.0f);
        grid.velocities[0] = Vec3(5.0f, 5.0f, 5.0f);
        applyBoundaryConditions(grid);
        assert(grid.velocities[0] == Vec3(0.0f, 0.0f, 0.0f));
    }

    // Test 5: Non-cubic grid 4x2x3
    {
        FluidGrid grid = createInitializedGrid(4, 2, 3, 0.5f);
        assert(grid.velocities.size() == 24);
        assert(grid.pressures.size() == 24);
        // Check index mapping: (3,1,2) should be last element
        assert(grid.index(3, 1, 2) == 23);
        // Set a boundary velocity and apply
        grid.velocities[grid.index(2, 1, 0)] = Vec3(1.0f, 2.0f, 3.0f);
        applyBoundaryConditions(grid);
        assert(grid.velocities[grid.index(2, 1, 0)] == Vec3(0.0f, 0.0f, 0.0f));
    }

    // Test 6: Invalid dimensions throw exception
    {
        bool threw = false;
        try {
            createInitializedGrid(0, 2, 3, 0.1f);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    return 0;
}

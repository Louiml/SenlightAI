// Write a C++ function `int countConwayActiveCubes(const std::vector<std::string>& initialGrid, int cycles)` that simulates a 3D cellular automaton inspired by Conway's Game of Life, but operating on a 3D grid with coordinates `(x, y, z)`. The input is a list of strings representing the initial 2D slice at `z=0`, where each character is either `'.'` (inactive) or `'#'` (active). The grid expands in all three dimensions as cycles proceed. At each cycle, a cube is active in the next generation if: (1) it is currently active and has exactly 2 or 3 active neighbors in the 26 surrounding positions in 3D space, or (2) it is currently inactive and has exactly 3 active neighbors. The function must return the number of active cubes after the specified number of cycles (e.g., 6). The input grid can have non-zero dimensions, and the coordinate system starts with `(x=0, y=0)` for the first character of the first row. Neighbors are all 26 positions differing by -1, 0, or 1 in each coordinate, excluding the cube itself.
The solution uses a `std::map` (or `std::set`) to store only active cubes, avoiding a large sparse grid. We precompute the 26 direction vectors (excluding the zero vector) into separate arrays `dx`, `dy`, `dz`. For each cycle:
1. Create an empty map for the next active set.
2. For each currently active cube, consider all 26 neighbors. For each neighbor, increment its active-neighbor count in a temporary map and also add it to a set of candidate positions (the neighbor itself plus the current cube, but since we only need to evaluate the current cube and all neighbors, we process them separately).
3. Apply rules: For each active cube, if its neighbor count (of active neighbors) is 2 or 3, keep it active in the next map. For each candidate neighbor that is not currently active, if its neighbor count is exactly 3, activate it.
4. Replace the current map with the next map.

Edge cases: The grid may start empty (no `#`), in which case the result is always 0. The grid can have arbitrary width/height. Since coordinates can become negative, using `std::map` with a custom `operator<` (or a `tuple`) handles that correctly. Time complexity per cycle: O(A * 26) where A is the number of active cubes (since we only examine active cubes and their neighbors). In worst case, A grows, but typical simulations are small. Space complexity: O(A) for storing active cubes and O(A * 26) for the neighbor-count map in each cycle.
#include <map>
#include <set>
#include <vector>
#include <string>
#include <tuple>

// Count active cubes after a given number of cycles in a 3D Conway-like automaton.
// initialGrid: 2D slice at z=0, '#' means active, '.' inactive.
// cycles: number of simulation steps.
// Returns the number of active cubes at the end.
int countConwayActiveCubes(const std::vector<std::string>& initialGrid, int cycles) {
    // Precompute the 26 neighbor direction offsets (excluding (0,0,0)).
    std::vector<int> dx, dy, dz;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if (!(x == 0 && y == 0 && z == 0)) {
                    dx.push_back(x);
                    dy.push_back(y);
                    dz.push_back(z);
                }
            }
        }
    }

    // Active points as a set of tuples (x, y, z).
    std::set<std::tuple<int, int, int>> active;

    // Initialize from the input grid at z=0.
    for (int x = 0; x < static_cast<int>(initialGrid.size()); ++x) {
        for (int y = 0; y < static_cast<int>(initialGrid[x].size()); ++y) {
            if (initialGrid[x][y] == '#') {
                active.emplace(x, y, 0);
            }
        }
    }

    // Simulate for the requested number of cycles.
    for (int step = 0; step < cycles; ++step) {
        std::set<std::tuple<int, int, int>> nextActive;
        // For each active cube, count its active neighbors and gather neighbor candidates.
        std::map<std::tuple<int, int, int>, int> neighborCount;
        std::set<std::tuple<int, int, int>> candidates;

        for (const auto& point : active) {
            int x, y, z;
            std::tie(x, y, z) = point;
            // The cube itself is also a candidate for potential activation (if rule applies).
            candidates.insert(point);

            for (int i = 0; i < static_cast<int>(dx.size()); ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                int nz = z + dz[i];
                auto neighbor = std::make_tuple(nx, ny, nz);
                // Count how many times this neighbor is adjacent to an active cube.
                neighborCount[neighbor]++;
                candidates.insert(neighbor);
            }
        }

        // Apply the rules to each candidate.
        for (const auto& point : candidates) {
            int x, y, z;
            std::tie(x, y, z) = point;
            bool isActive = active.find(point) != active.end();
            int cnt = neighborCount[point];
            if (isActive) {
                // Active survives if it has 2 or 3 active neighbors.
                if (cnt == 2 || cnt == 3) {
                    nextActive.insert(point);
                }
            } else {
                // Inactive becomes active if it has exactly 3 active neighbors.
                if (cnt == 3) {
                    nextActive.insert(point);
                }
            }
        }

        active = std::move(nextActive);
    }

    return static_cast<int>(active.size());
}
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test.
int countConwayActiveCubes(const std::vector<std::string>& initialGrid, int cycles);

int main() {
    // Single active cube: after 1 cycle it becomes inactive (0 neighbors).
    assert(countConwayActiveCubes({"."}, 1) == 0);
    assert(countConwayActiveCubes({"#"}, 1) == 0);

    // Two adjacent active cubes: each has 1 neighbor, so both die after 1 cycle.
    assert(countConwayActiveCubes({"##"}, 1) == 0);

    // A 2D block (2x2) of active cells: each has 3 active neighbors (in 3D, still 3 neighbors in the plane), survive.
    assert(countConwayActiveCubes({"##", "##"}, 1) == 4);

    // A 3x3 2D pattern with center active and 8 neighbors: center has 8 neighbors, dies; corners have 3 neighbors, become active, etc.
    // After 1 cycle, a 3x3 fully active 2D plane: each has 8 neighbors (in 3D, all 8 surrounding in-plane), so all die (8 neighbors not 2-3).
    assert(countConwayActiveCubes({"###", "###", "###"}, 1) == 0);

    // Classic blinker-like pattern in 2D (three in a line): each has 2 neighbors, will remain active (since exactly 2).
    std::vector<std::string> blinker = {"#"};
    // But blinker is just one column, so it's a 1x1 grid; need a proper line.
    // Let's test with a horizontal line of 3 active cells at z=0.
    std::vector<std::string> line = {"###"};
    assert(countConwayActiveCubes(line, 1) == 3); // each has exactly 2 neighbors, survives.

    // After 2 cycles, that line becomes a vertical line (since each neighbor has 2 neighbors), still 3 active.
    assert(countConwayActiveCubes(line, 2) == 3);

    // Initial empty grid always returns 0 for any cycles.
    assert(countConwayActiveCubes({}, 6) == 0);
    assert(countConwayActiveCubes({"."}, 6) == 0);

    // Example from the problem statement (Advent of Code 2020 Day 17): 
    // Input: ".#.", "..#", "###" => after 6 cycles, answer is 112.
    std::vector<std::string> aoc = {".#.", "..#", "###"};
    assert(countConwayActiveCubes(aoc, 6) == 112);

    return 0;
}

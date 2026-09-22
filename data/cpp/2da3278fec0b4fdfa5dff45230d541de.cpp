/*
Write a C++ function named `interpolateGradient` that takes a 2D grid of grayscale pixel values as a `std::vector<std::vector<int>>` (each value between 0 and 255 inclusive), and performs edge-preserving interpolation on the grid. Specifically, the function should return a new grid of the same dimensions where every non-zero pixel remains unchanged, and every zero-valued pixel is filled with the average of the four orthogonal neighbors (up, down, left, right) that are non-zero, but only if at least one such neighbor exists. If a zero pixel has no non-zero orthogonal neighbors, leave it as zero. The interpolation should be performed iteratively over the entire grid until no further changes occur (i.e., all fillable zeros are filled). The function must preserve the original grid's dimensions and be `const`-correct for the input parameter.
*/
#include <vector>
#include <algorithm>

// Interpolate zero-valued pixels in a grayscale grid by averaging non-zero orthogonal neighbors.
// Returns a new grid where all fillable zeros are replaced; unfillable remain zero.
std::vector<std::vector<int>> interpolateGradient(const std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return grid;

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());

    std::vector<std::vector<int>> result = grid;

    bool changed = true;
    while (changed) {
        changed = false;
        // Snapshot to avoid using already-updated values within the same pass
        std::vector<std::vector<int>> next = result;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (result[r][c] != 0) continue;

                int sum = 0;
                int count = 0;

                // Up
                if (r > 0 && result[r-1][c] != 0) {
                    sum += result[r-1][c];
                    ++count;
                }
                // Down
                if (r + 1 < rows && result[r+1][c] != 0) {
                    sum += result[r+1][c];
                    ++count;
                }
                // Left
                if (c > 0 && result[r][c-1] != 0) {
                    sum += result[r][c-1];
                    ++count;
                }
                // Right
                if (c + 1 < cols && result[r][c+1] != 0) {
                    sum += result[r][c+1];
                    ++count;
                }

                if (count > 0) {
                    int avg = sum / count; // integer division truncates
                    if (avg != 0) {
                        next[r][c] = avg;
                        changed = true;
                    }
                }
            }
        }
        result = next;
    }

    return result;
}
#include <cassert>
#include <vector>

// Declare the function (assume it is defined above or linked from the solution)
std::vector<std::vector<int>> interpolateGradient(const std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Simple fill with one non-zero neighbor
    {
        std::vector<std::vector<int>> input = {{0, 0, 0},
                                               {0, 5, 0},
                                               {0, 0, 0}};
        std::vector<std::vector<int>> expected = {{0, 5, 0},
                                                  {5, 5, 5},
                                                  {0, 5, 0}};
        auto output = interpolateGradient(input);
        assert(output == expected);
    }

    // Test 2: Cross-shaped non-zero seeds fill all zeros
    {
        std::vector<std::vector<int>> input = {{0, 0, 0, 0, 0},
                                               {0, 0, 0, 0, 0},
                                               {0, 0, 100, 0, 0},
                                               {0, 0, 0, 0, 0},
                                               {0, 0, 0, 0, 0}};
        std::vector<std::vector<int>> output = interpolateGradient(input);
        // No zero should remain because the central 100 propagates outward
        for (const auto& row : output) {
            for (int v : row) {
                assert(v != 0);
            }
        }
        assert(output[2][2] == 100); // original preserved
    }

    // Test 3: No zeros input
    {
        std::vector<std::vector<int>> input = {{10, 20}, {30, 40}};
        auto output = interpolateGradient(input);
        assert(output == input);
    }

    // Test 4: Isolated zero with no non-zero neighbors
    {
        std::vector<std::vector<int>> input = {{0, 0, 0},
                                               {0, 0, 0},
                                               {0, 0, 7}};
        auto output = interpolateGradient(input);
        // The zero at (0,0) is never reachable from 7 via orthogonal adjacency? Actually it is reachable.
        // Let's check: 7 at (2,2) -> (1,2) gets avg=7, then (0,2) gets 7, (1,1) gets 7, etc. So all get filled.
        // To test isolated, make a grid with no non-zero neighbors at all.
        assert(true); // placeholder, replace with actual testing below
    }

    // Test 4 (corrected): Grid with no non-zero values at all
    {
        std::vector<std::vector<int>> input = {{0, 0}, {0, 0}};
        auto output = interpolateGradient(input);
        std::vector<std::vector<int>> expected = {{0, 0}, {0, 0}};
        assert(output == expected);
    }

    // Test 5: Boundaries and multiple iterations
    {
        std::vector<std::vector<int>> input = {{0, 0, 0},
                                               {0, 6, 0},
                                               {0, 0, 0}};
        std::vector<std::vector<int>> output = interpolateGradient(input);
        // The center 6 fills its four orthogonal neighbors (all become 6).
        // Then the corners become 6 (average of two 6s = 6).
        std::vector<std::vector<int>> expected = {{6, 6, 6},
                                                  {6, 6, 6},
                                                  {6, 6, 6}};
        assert(output == expected);
    }

    // Test 6: Non-uniform neighbors produce average
    {
        std::vector<std::vector<int>> input = {{0, 10, 0},
                                               {0, 0, 20},
                                               {0, 0, 0}};
        // Center (1,1) has right neighbor 20 and top neighbor 10 → avg = 15
        // After fill, more cells become fillable, but check first pass result.
        auto output = interpolateGradient(input);
        // The center (1,1) becomes 15 in first pass, then it propagates.
        // After full convergence, all zeros will be filled.
        assert(output[1][1] == 15); // since it's filled in first pass
        // But final grid should have no zeros? Check:
        for (const auto& row : output) {
            for (int v : row) {
                assert(v != 0);
            }
        }
    }

    // Test 7: Single row and single column
    {
        std::vector<std::vector<int>> input = {{0, 3, 0}};
        auto output = interpolateGradient(input);
        std::vector<std::vector<int>> expected = {{3, 3, 3}};
        assert(output == expected);
    }

    // Test 8: Single element
    {
        std::vector<std::vector<int>> input = {{5}};
        auto output = interpolateGradient(input);
        assert(output == input);
    }

    // Test 9: Large grid with checkerboard pattern
    {
        std::vector<std::vector<int>> input(4, std::vector<int>(4, 0));
        input[0][0] = 10;
        input[3][3] = 20;
        auto output = interpolateGradient(input);
        // All zeros should be filled eventually
        for (const auto& row : output) {
            for (int v : row) {
                assert(v != 0);
            }
        }
        // Corner values preserved
        assert(output[0][0] == 10);
        assert(output[3][3] == 20);
    }

    // Test 10: Zero grid with one non-zero far away
    {
        std::vector<std::vector<int>> input(5, std::vector<int>(5, 0));
        input[4][4] = 8;
        auto output = interpolateGradient(input);
        // All should be filled
        for (const auto& row : output) {
            for (int v : row) {
                assert(v != 0);
            }
        }
    }

    return 0;
}
// The problem requires iterative filling of zero pixels based on the average of their non-zero orthogonal neighbors. The main algorithm is a fixed-point iteration: repeatedly scan the grid and for each zero pixel, compute the sum and count of its four orthogonal neighbors that are non-zero. If the count is greater than zero, set the pixel to the integer average (sum / count, using integer division truncating toward zero, with values guaranteed to be integers). Repeat this process until a full pass over the grid produces no changes. This ensures that zeros that become fillable after their neighbors are filled get handled in subsequent iterations. Edge cases include boundaries (pixels at grid edges have fewer neighbors), grids with no zeros (immediate return), and disconnected zero regions with no adjacent non-zero pixels that remain unfilled. Time complexity is O(k * R * C), where R and C are grid dimensions and k is the number of iterations until convergence (in the worst case, k can be the maximum distance from a non-zero pixel to the farthest zero, bounded by R+C). Space complexity is O(R*C) for the output grid (and a copy for comparison), ignoring the input's own storage.

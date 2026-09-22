// Write a standalone C++ function that takes a 2D grid (represented as a `std::vector<std::vector<bool>>` where `true` indicates an occupied cell and `false` indicates empty) and returns a `std::vector<std::pair<int,int>>` containing the centroid coordinates (as integers, rounded down) of each connected component of `true` cells, but only for components whose area (number of cells) is between a given minimum and maximum (inclusive). The function must also filter out duplicate centroids by keeping only one centroid if two centroids are within a Euclidean distance of 5 and a horizontal distance of 3 of each other (the first encountered in the order of processing is kept). The returned centroids must be sorted by their y-coordinate in descending order (largest y first). Handle edge cases: empty grid, no components within the area range, and single-cell components. The function signature should be `std::vector<std::pair<int,int>> getComponentCentroids(const std::vector<std::vector<bool>>& grid, int areaMin, int areaMax)`.
The solution requires a two-pass approach. First, identify all connected components of `true` cells using a standard flood-fill or BFS/DFS from each unvisited `true` cell. For each component, compute its area (count of cells) and its centroid by summing x and y coordinates and dividing by the area (integer division). Only accept components with area in `[areaMin, areaMax]`. After collecting all candidate centroids, sort them by y descending (if tie, any order). Then filter duplicates: maintain a list of accepted centroids; for each candidate (in sorted order), check against already accepted ones—if the Euclidean distance is < 5 and horizontal distance ≤ 3, skip it; otherwise accept. The sorting ensures that centroids with larger y are processed first, which affects which duplicate is kept but the spec allows first encountered. Edge cases: handle empty grid (return empty), components with zero area (impossible for `true` cells), and when no components pass the area filter. Time complexity: O(R*C) for grid traversal plus O(K^2) for duplicate filtering where K is the number of candidate centroids (K ≤ number of cells). Space: O(R*C) for visited array and O(K) for storage.
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

// Compute centroids of connected components in a boolean grid, filtered by area and duplicate distance.
std::vector<std::pair<int,int>> getComponentCentroids(const std::vector<std::vector<bool>>& grid, int areaMin, int areaMax) {
    if (grid.empty() || grid[0].empty()) return {};
    
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<int,int>> centroids;
    
    // Directions: 4-connected (up, down, left, right)
    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] && !visited[r][c]) {
                // BFS to collect component cells
                std::vector<std::pair<int,int>> component;
                std::vector<std::pair<int,int>> stack;
                stack.push_back({r, c});
                visited[r][c] = true;
                
                while (!stack.empty()) {
                    auto [cr, cc] = stack.back();
                    stack.pop_back();
                    component.push_back({cr, cc});
                    
                    for (int d = 0; d < 4; ++d) {
                        int nr = cr + dr[d];
                        int nc = cc + dc[d];
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && 
                            grid[nr][nc] && !visited[nr][nc]) {
                            visited[nr][nc] = true;
                            stack.push_back({nr, nc});
                        }
                    }
                }
                
                int area = static_cast<int>(component.size());
                if (area >= areaMin && area <= areaMax) {
                    long long sumX = 0, sumY = 0;
                    for (const auto& [x, y] : component) {
                        sumX += x;
                        sumY += y;
                    }
                    int cx = static_cast<int>(sumX / area);
                    int cy = static_cast<int>(sumY / area);
                    centroids.push_back({cx, cy});
                }
            }
        }
    }
    
    // Sort by y descending
    std::sort(centroids.begin(), centroids.end(), 
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  return a.second > b.second;
              });
    
    // Filter duplicates: keep first occurrence (in sorted order)
    std::vector<std::pair<int,int>> filtered;
    const double DIST_EUCLID = 5.0;
    const int DIST_X = 3;
    
    for (const auto& cand : centroids) {
        bool isNew = true;
        for (const auto& accepted : filtered) {
            double dx = static_cast<double>(cand.first - accepted.first);
            double dy = static_cast<double>(cand.second - accepted.second);
            double euclid = std::sqrt(dx*dx + dy*dy);
            if (euclid < DIST_EUCLID && std::abs(cand.first - accepted.first) <= DIST_X) {
                isNew = false;
                break;
            }
        }
        if (isNew) {
            filtered.push_back(cand);
        }
    }
    
    return filtered;
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (assumed from solution)
std::vector<std::pair<int,int>> getComponentCentroids(const std::vector<std::vector<bool>>& grid, int areaMin, int areaMax);

int main() {
    // Test 1: Simple single component 2x2, area 4, centroid at (0,0) since (0+1)/2=0
    std::vector<std::vector<bool>> grid1 = {
        {true, true},
        {true, true}
    };
    auto res1 = getComponentCentroids(grid1, 1, 10);
    assert(res1.size() == 1);
    assert(res1[0] == std::make_pair(0, 0));

    // Test 2: Two separate components, one too small, one accepted
    std::vector<std::vector<bool>> grid2 = {
        {true, false, false, false},
        {false, false, false, false},
        {false, false, true, true}
    };
    auto res2 = getComponentCentroids(grid2, 2, 10);
    // Component1 area=1 (ignored), Component2 area=2, centroid (2,2)
    assert(res2.size() == 1);
    assert(res2[0] == std::make_pair(2, 2));

    // Test 3: Duplicate centroids (two components at same centroid, but one filtered)
    std::vector<std::vector<bool>> grid3 = {
        {true, false, false},
        {false, false, false},
        {false, false, true}
    };
    auto res3 = getComponentCentroids(grid3, 1, 10);
    // Two components each area 1, centroids (0,0) and (2,2) — distance >5, both kept
    assert(res3.size() == 2);
    // Sorted by y descending: (0,0) then (2,2) has y=2 > 0, so first is (2,2)
    assert(res3[0] == std::make_pair(2, 2));
    assert(res3[1] == std::make_pair(0, 0));

    // Test 4: Duplicate filtering within distance
    std::vector<std::vector<bool>> grid4 = {
        {true, true, false},
        {true, false, false},
        {false, false, false}
    };
    // One component area 3, centroid (0,0), plus another component? Actually no. Create two close ones:
    std::vector<std::vector<bool>> grid5 = {
        {true, false, false},
        {false, false, false},
        {false, false, true}
    };
    // Add a third cell near the first to create duplicate-like centroids? Let's craft carefully.
    // Better: two components at (0,0) and (1,0) — distance 1 <5 and dx=1 ≤3 → filtered
    std::vector<std::vector<bool>> grid5b = {
        {true, false, true},
        {false, false, false},
        {false, false, false}
    };
    auto res5 = getComponentCentroids(grid5b, 1, 10);
    // Two components: (0,0) and (2,0) — distance 2 <5, dx=2 ≤3 → only one retained (first in sorted order, y=0 both, order by y then undefined; but sort stable? We'll just check size)
    assert(res5.size() == 1);

    // Test 5: Empty grid
    std::vector<std::vector<bool>> gridEmpty;
    auto res6 = getComponentCentroids(gridEmpty, 1, 10);
    assert(res6.empty());

    // Test 6: No components in area range
    std::vector<std::vector<bool>> grid6 = {
        {true, false},
        {false, false}
    };
    auto res7 = getComponentCentroids(grid6, 2, 10); // area=1 not in range
    assert(res7.empty());

    // Test 7: Vertical ordering by y descending
    std::vector<std::vector<bool>> grid7 = {
        {false, false, true},
        {false, false, false},
        {true, false, false}
    };
    auto res8 = getComponentCentroids(grid7, 1, 10);
    // Components: (2,0) and (0,2) — sorted by y desc → (2,0) first (y=0? wait y is row? Check: we used x as row, y as col? Our code uses x=row, y=col. So (2,0) means row=2,col=0 → y=0; (0,2) means row=0,col=2 → y=2. Sorted by y desc → (0,2) first.
    assert(res8.size() == 2);
    assert(res8[0] == std::make_pair(0, 2));
    assert(res8[1] == std::make_pair(2, 0));

    return 0;
}

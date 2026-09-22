/*
Write a C++ function `std::vector<int> getTilesInRadius(const std::vector<std::pair<double,double>>& tilePositions, int startIdx, int radius)` that, given a vector of 2D tile center coordinates (as `(x,y)` pairs), returns the indices of all tiles whose distance from the starting tile at index `startIdx` is strictly less than `radius` times a fixed threshold `TILE_WIDTH_X = 1.0`. More precisely: for `radius == 1`, return only the starting tile index; for `radius == 2`, return indices of tiles within distance < `1.2 * TILE_WIDTH_X`; for `radius == 3`, return indices of tiles within distance < `2.4 * TILE_WIDTH_X`. The function must handle arbitrary layouts (not necessarily a grid) and must validate `radius` (1–3) and `startIdx` bounds, returning an empty vector on invalid input. Tiles are considered to be on a single team, so no coordinate mirroring is needed. Assume `TILE_WIDTH_X = 1.0` and use Euclidean distance. The function should be efficient for up to 1000 tiles.
*/

#include <vector>
#include <cmath>
#include <cstddef>

// Return indices of tiles within a distance threshold from a starting tile.
// radius 1: only startIdx; radius 2: distance < 1.2; radius 3: distance < 2.4.
// TILE_WIDTH_X is fixed at 1.0. Invalid inputs return empty vector.
std::vector<int> getTilesInRadius(
    const std::vector<std::pair<double,double>>& tilePositions,
    int startIdx,
    int radius
) {
    const double TILE_WIDTH_X = 1.0;
    const double NEIGHBOR_MAX_RADIUS = TILE_WIDTH_X * 1.2; // for radius 2
    const double NEIGHBOR_MAX_RADIUS_2 = NEIGHBOR_MAX_RADIUS * 2.0; // for radius 3

    // Validate input
    if (radius < 1 || radius > 3) return {};
    if (startIdx < 0 || static_cast<size_t>(startIdx) >= tilePositions.size()) return {};
    if (tilePositions.empty()) return {};

    // Radius 1: only the starting tile
    if (radius == 1) {
        return {startIdx};
    }

    // Determine threshold based on radius
    double threshold = (radius == 2) ? NEIGHBOR_MAX_RADIUS : NEIGHBOR_MAX_RADIUS_2;
    double thresholdSq = threshold * threshold;

    const auto& start = tilePositions[static_cast<size_t>(startIdx)];
    std::vector<int> result;

    for (size_t i = 0; i < tilePositions.size(); ++i) {
        double dx = tilePositions[i].first - start.first;
        double dy = tilePositions[i].second - start.second;
        double distSq = dx*dx + dy*dy;
        if (distSq < thresholdSq) {
            result.push_back(static_cast<int>(i));
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Tile positions forming a simple line: (0,0), (1,0), (2,0), (-1,0)
    std::vector<std::pair<double,double>> tiles = {
        {0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {-1.0, 0.0}
    };

    // Radius 1 returns only start index
    assert(getTilesInRadius(tiles, 0, 1) == std::vector<int>({0}));
    assert(getTilesInRadius(tiles, 1, 1) == std::vector<int>({1}));

    // Radius 2: distance < 1.2 from (0,0) includes (0,0) and (1,0)
    auto r2 = getTilesInRadius(tiles, 0, 2);
    assert(r2.size() == 2);
    assert(r2[0] == 0 && r2[1] == 1);

    // Radius 3: distance < 2.4 from (0,0) includes (0,0),(1,0),(2,0),(-1,0)
    auto r3 = getTilesInRadius(tiles, 0, 3);
    assert(r3.size() == 4);
    assert(r3[0] == 0 && r3[1] == 1 && r3[2] == 2 && r3[3] == 3);

    // Invalid radius and invalid index
    assert(getTilesInRadius(tiles, 0, 0).empty());
    assert(getTilesInRadius(tiles, 0, 4).empty());
    assert(getTilesInRadius(tiles, 5, 2).empty());

    // Empty tile list
    std::vector<std::pair<double,double>> empty;
    assert(getTilesInRadius(empty, 0, 2).empty());

    // Self-distance always included
    assert(getTilesInRadius(tiles, 2, 2).size() >= 1);
    assert(getTilesInRadius(tiles, 2, 2)[0] == 2);

    return 0;
}

// The solution follows the neighbor-generation logic from the snippet: precompute neighbor lists via distance checks. For each call, first validate `radius` (1–3) and `startIdx`. For radius 1, return `{startIdx}` directly. For radius 2 and 3, iterate over all tile positions, compute squared Euclidean distance to the starting tile (avoiding floating-point square roots for speed and precision), and compare against threshold^2 (`(1.2*1.0)^2 = 1.44` for radius 2, `(2.4*1.0)^2 = 5.76` for radius 3). Include the starting tile itself because distance 0 is always under the threshold. Edge cases: an empty input vector returns empty; invalid indices or radius return empty. Since each call is O(n) where n is number of tiles, and we could precompute neighbors once if many queries are expected, but the task asks for a single query; still, we can implement by scanning each call. Space is O(1) aside from the output vector. The thresholds are chosen to match typical hex grid spacing, but the function works for arbitrary layouts; the key is consistent distance-based selection.

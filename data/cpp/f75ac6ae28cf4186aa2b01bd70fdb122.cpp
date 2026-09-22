// Create a C++ function that simulates the core point-cloud registration logic of the provided NDT (Normal Distributions Transform) mapping snippet. Specifically, implement a function that takes as input a sequence of 2D point clouds (each represented as a `std::vector<std::pair<double,double>>` of (x,y) coordinates), along with a voxel grid leaf size, a minimum and maximum scan range for filtering points, and a minimum shift threshold for adding scans to the map. The function must perform the following steps for each point cloud in the sequence: (1) filter points by distance from origin (keep only points with distance between `min_scan_range` and `max_scan_range`), (2) apply a simple voxel grid downsampling (grid cell size = `voxel_leaf_size`) to the filtered points, (3) perform a trivial one-step translation-only alignment: compute the centroid of the current downsampled scan and the centroid of the accumulated map (initially empty, first scan becomes the map), then compute the translation vector as (map_centroid - scan_centroid), and update the current pose by adding this translation to the previous pose (starting from (0,0)), (4) accumulate the downsampled scan into the map (using the original unfiltered points transformed by the accumulated pose), and (5) if the Euclidean distance between the current accumulated pose and the pose at which the last scan was added exceeds `min_add_scan_shift_`, add the transformed scan to the map and update the "last added" pose. The function should return a `std::vector<std::pair<double,double>>` containing the sequence of accumulated poses (x,y) after processing each input scan. Assume all input scans are in a fixed coordinate frame (no rotations needed). Handle the edge case of an empty input sequence by returning an empty vector. Also, for the very first scan, set the pose to (0,0) and add the filtered points (after voxel grid) directly to the map without any alignment.
// The solution simulates a simplified version of the NDT mapping pipeline: for each incoming scan, we first filter out points outside the radial range, then downsample via voxel grid to reduce computational load. The alignment is simplified to centroid-based translation: we compute the centroid of the downsampled scan and the centroid of the current map (if the map is empty, we treat the pose as identity and add the scan directly). The translation offset is the difference between these centroids, and we update the current pose by adding this offset to the previous pose. Then we transform the original (unfiltered) scan points by the accumulated pose (applying the translation) and add them to the map. However, to control map growth and match the original snippet's behavior, we only add the transformed scan to the map when the shift from the last added scan exceeds `min_add_scan_shift_`. Important edge cases: an empty input vector returns an empty pose list; the first scan initializes the map and pose; if a scan has no points after filtering and downsampling, we should skip alignment and keep the pose unchanged (but still record it). For voxel grid, we use a simple hash map keyed by integer grid coordinates (floor(x/leaf), floor(y/leaf)) and keep one representative point per cell (e.g., the first encountered). Time complexity: for each scan with `n` points, filtering is O(n), voxel grid is O(n) (hash map operations), centroid computation is O(m) where m ≤ n after downsampling, and map update is O(k) where k is the number of points added (at most n per added scan). Overall, for `N` scans, total time is O(N * n) assuming constant number of points per scan. Space complexity: O(N * n) for storing the accumulated map in the worst case (if every scan is added), but in practice bound by the shift threshold.
#include <vector>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <algorithm>

// Compute the accumulated poses from a sequence of 2D point clouds.
// Each point cloud is a vector of (x,y) pairs. The function filters,
// downsamples, aligns via centroid translation, and accumulates a map.
std::vector<std::pair<double,double>> simulateNdtMapping(
    const std::vector<std::vector<std::pair<double,double>>>& scans,
    double voxel_leaf_size,
    double min_scan_range,
    double max_scan_range,
    double min_add_scan_shift)
{
    std::vector<std::pair<double,double>> poses;
    if (scans.empty()) {
        return poses;
    }

    // Accumulated map (unfiltered transformed points) as vector of pairs.
    std::vector<std::pair<double,double>> map;

    // Current pose (x,y) and pose at last map addition.
    double pose_x = 0.0, pose_y = 0.0;
    double last_add_x = 0.0, last_add_y = 0.0;

    bool first_scan = true;

    for (const auto& scan : scans) {
        // Step 1: Filter points by radial distance.
        std::vector<std::pair<double,double>> filtered;
        for (const auto& p : scan) {
            double r = std::sqrt(p.first * p.first + p.second * p.second);
            if (r > min_scan_range && r < max_scan_range) {
                filtered.push_back(p);
            }
        }

        // Step 2: Voxel grid downsampling.
        std::unordered_map<long long, std::pair<double,double>> grid;
        auto grid_key = [&](double x, double y) {
            long long ix = static_cast<long long>(std::floor(x / voxel_leaf_size));
            long long iy = static_cast<long long>(std::floor(y / voxel_leaf_size));
            return ix * 1000003LL + iy;
        };
        for (const auto& p : filtered) {
            long long key = grid_key(p.first, p.second);
            if (grid.find(key) == grid.end()) {
                grid[key] = p;
            }
        }
        std::vector<std::pair<double,double>> downsampled;
        downsampled.reserve(grid.size());
        for (const auto& kv : grid) {
            downsampled.push_back(kv.second);
        }

        if (first_scan) {
            // Initialize map with downsampled points (no alignment).
            map.insert(map.end(), downsampled.begin(), downsampled.end());
            first_scan = false;
            poses.push_back({0.0, 0.0});
            continue;
        }

        // Step 3: Centroid-based translation alignment.
        double scan_cx = 0.0, scan_cy = 0.0;
        if (!downsampled.empty()) {
            for (const auto& p : downsampled) {
                scan_cx += p.first;
                scan_cy += p.second;
            }
            scan_cx /= static_cast<double>(downsampled.size());
            scan_cy /= static_cast<double>(downsampled.size());
        }

        double map_cx = 0.0, map_cy = 0.0;
        if (!map.empty()) {
            for (const auto& p : map) {
                map_cx += p.first;
                map_cy += p.second;
            }
            map_cx /= static_cast<double>(map.size());
            map_cy /= static_cast<double>(map.size());
        }

        // Update pose by the translation offset.
        if (!downsampled.empty() && !map.empty()) {
            pose_x += (map_cx - scan_cx);
            pose_y += (map_cy - scan_cy);
        }

        // Step 4: Transform original (filtered, not downsampled) points by pose.
        std::vector<std::pair<double,double>> transformed;
        transformed.reserve(filtered.size());
        for (const auto& p : filtered) {
            transformed.emplace_back(p.first + pose_x, p.second + pose_y);
        }

        // Step 5: Add to map if shift since last addition exceeds threshold.
        double shift = std::sqrt((pose_x - last_add_x) * (pose_x - last_add_x) +
                                 (pose_y - last_add_y) * (pose_y - last_add_y));
        if (shift >= min_add_scan_shift) {
            map.insert(map.end(), transformed.begin(), transformed.end());
            last_add_x = pose_x;
            last_add_y = pose_y;
        }

        poses.push_back({pose_x, pose_y});
    }

    return poses;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The function declaration (provided here for the test; in actual solution it's separate).
std::vector<std::pair<double,double>> simulateNdtMapping(
    const std::vector<std::vector<std::pair<double,double>>>& scans,
    double voxel_leaf_size,
    double min_scan_range,
    double max_scan_range,
    double min_add_scan_shift);

int main() {
    // Test 1: Empty input.
    auto poses1 = simulateNdtMapping({}, 1.0, 0.0, 10.0, 1.0);
    assert(poses1.empty());

    // Test 2: Single scan, filtered points become map, pose is (0,0).
    std::vector<std::vector<std::pair<double,double>>> scans2;
    scans2.push_back({{1.0, 0.0}, {0.0, 1.0}, {-1.0, 0.0}, {0.0, -1.0}});
    auto poses2 = simulateNdtMapping(scans2, 0.5, 0.0, 2.0, 0.5);
    assert(poses2.size() == 1);
    assert(std::fabs(poses2[0].first - 0.0) < 1e-9);
    assert(std::fabs(poses2[0].second - 0.0) < 1e-9);

    // Test 3: Two identical scans with voxel leaf large enough to keep all points,
    // map centroid equals scan centroid, so no pose change.
    std::vector<std::vector<std::pair<double,double>>> scans3;
    scans3.push_back({{2.0, 0.0}, {0.0, 2.0}, {-2.0, 0.0}, {0.0, -2.0}});
    scans3.push_back({{2.0, 0.0}, {0.0, 2.0}, {-2.0, 0.0}, {0.0, -2.0}});
    auto poses3 = simulateNdtMapping(scans3, 1.0, 0.0, 5.0, 0.1);
    assert(poses3.size() == 2);
    assert(std::fabs(poses3[0].first - 0.0) < 1e-9);
    assert(std::fabs(poses3[0].second - 0.0) < 1e-9);
    // Since map is initialized with first scan's points, and second scan is identical,
    // centroids match, so pose remains (0,0).
    assert(std::fabs(poses3[1].first - 0.0) < 1e-9);
    assert(std::fabs(poses3[1].second - 0.0) < 1e-9);

    // Test 4: Second scan shifted to the right; centroid alignment should shift pose.
    std::vector<std::vector<std::pair<double,double>>> scans4;
    scans4.push_back({{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}});
    scans4.push_back({{2.0, 0.0}, {3.0, 0.0}, {2.0, 1.0}, {3.0, 1.0}});
    auto poses4 = simulateNdtMapping(scans4, 1.0, 0.0, 5.0, 0.1);
    assert(poses4.size() == 2);
    assert(std::fabs(poses4[1].first - 2.0) < 1e-9);
    assert(std::fabs(poses4[1].second - 0.0) < 1e-9);

    // Test 5: Skip alignment if downsampled scan is empty (all points filtered out).
    std::vector<std::vector<std::pair<double,double>>> scans5;
    scans5.push_back({{1.0, 0.0}, {0.0, 1.0}});
    scans5.push_back({{100.0, 100.0}, {101.0, 101.0}}); // outside max_scan_range=10
    auto poses5 = simulateNdtMapping(scans5, 1.0, 0.0, 10.0, 0.5);
    assert(poses5.size() == 2);
    assert(std::fabs(poses5[0].first - 0.0) < 1e-9);
    // Second scan has no points after filtering, so pose stays unchanged.
    assert(std::fabs(poses5[1].first - 0.0) < 1e-9);
    assert(std::fabs(poses5[1].second - 0.0) < 1e-9);

    return 0;
}

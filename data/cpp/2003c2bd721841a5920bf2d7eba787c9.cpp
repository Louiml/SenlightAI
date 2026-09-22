Write a C++ function named `findRoomClusters` that takes as input a vector of `uint64_t` timestamps representing the last update time of each place node, a vector of cluster IDs (one per node, where nodes with the same cluster ID belong to the same initial cluster), and a minimum room size (`size_t min_room_size`). The function must return a vector of cluster IDs, sorted by the oldest timestamp within each cluster (ascending order of the minimum timestamp in that cluster). Clusters with fewer than `min_room_size` nodes must be excluded from the result. Ties in the oldest timestamp must be broken by the smaller cluster ID first. The input vectors are guaranteed to have equal length, and cluster IDs are non-negative integers. The function should be `const`-correct and take the inputs by `const` reference. For example, if nodes have timestamps `[100, 50, 200, 150]` and cluster IDs `[1, 1, 2, 2]`, with `min_room_size = 2`, then cluster 1 has oldest timestamp 50, cluster 2 has oldest timestamp 150, so the output is `[1, 2]`. If `min_room_size = 3`, both clusters have size 2, so the output is empty.
#include <cassert>
#include <cstdint>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Basic example from the task.
    std::vector<uint64_t> times1 = {100, 50, 200, 150};
    std::vector<size_t> clusters1 = {1, 1, 2, 2};
    auto result1 = findRoomClusters(times1, clusters1, 2);
    assert(result1 == std::vector<size_t>({1, 2}));

    // Minimum room size filters out all clusters.
    auto result2 = findRoomClusters(times1, clusters1, 3);
    assert(result2.empty());

    // Tie in oldest timestamp broken by smaller cluster ID.
    std::vector<uint64_t> times3 = {30, 30, 10, 10};
    std::vector<size_t> clusters3 = {5, 5, 3, 3};
    auto result3 = findRoomClusters(times3, clusters3, 2);
    assert(result3 == std::vector<size_t>({3, 5}));

    // Single node cluster.
    std::vector<uint64_t> times4 = {42};
    std::vector<size_t> clusters4 = {9};
    auto result4 = findRoomClusters(times4, clusters4, 1);
    assert(result4 == std::vector<size_t>({9}));
    auto result5 = findRoomClusters(times4, clusters4, 2);
    assert(result5.empty());

    // Duplicate timestamps within a cluster.
    std::vector<uint64_t> times6 = {5, 5, 7, 4, 6};
    std::vector<size_t> clusters6 = {1, 1, 1, 2, 2};
    auto result6 = findRoomClusters(times6, clusters6, 2);
    // Cluster 1 min=5, cluster 2 min=4, so sorted by min: cluster 2 first.
    assert(result6 == std::vector<size_t>({2, 1}));

    // Larger test with three clusters.
    std::vector<uint64_t> times7 = {100, 90, 80, 70, 60, 50};
    std::vector<size_t> clusters7 = {0, 0, 1, 1, 2, 2};
    auto result7 = findRoomClusters(times7, clusters7, 2);
    // Cluster 0 min=90, cluster 1 min=70, cluster 2 min=50.
    assert(result7 == std::vector<size_t>({2, 1, 0}));

    // Empty input.
    std::vector<uint64_t> times8;
    std::vector<size_t> clusters8;
    auto result8 = findRoomClusters(times8, clusters8, 1);
    assert(result8.empty());
}
#include <algorithm>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

// Given timestamps and cluster IDs for each node, return cluster IDs that have
// at least min_room_size nodes, sorted by the oldest timestamp in the cluster
// (ascending), breaking ties by smaller cluster ID first.
std::vector<size_t> findRoomClusters(const std::vector<uint64_t>& timestamps,
                                     const std::vector<size_t>& cluster_ids,
                                     size_t min_room_size) {
    // Accumulate cluster size and minimum timestamp.
    std::unordered_map<size_t, std::pair<size_t, uint64_t>> cluster_info;
    for (size_t i = 0; i < timestamps.size(); ++i) {
        auto& info = cluster_info[cluster_ids[i]];
        ++info.first;  // increment size
        if (info.second > timestamps[i]) {
            info.second = timestamps[i];  // update min timestamp
        }
    }

    // Collect clusters that meet the minimum size requirement.
    std::vector<std::pair<uint64_t, size_t>> valid_clusters;
    for (const auto& entry : cluster_info) {
        if (entry.second.first >= min_room_size) {
            valid_clusters.emplace_back(entry.second.second, entry.first);
        }
    }

    // Sort by (min_timestamp, cluster_id) in ascending order.
    std::sort(valid_clusters.begin(), valid_clusters.end(),
              [](const auto& a, const auto& b) {
                  return std::tie(a.first, a.second) < std::tie(b.first, b.second);
              });

    // Extract cluster IDs in sorted order.
    std::vector<size_t> result;
    result.reserve(valid_clusters.size());
    for (const auto& pair : valid_clusters) {
        result.push_back(pair.second);
    }
    return result;
}
// The problem requires grouping nodes by cluster ID, computing for each cluster the minimum timestamp among its member nodes, and then sorting valid clusters (those with size ≥ `min_room_size`) by that minimum timestamp. The algorithm proceeds in three main steps: (1) iterate through the input arrays, accumulating for each cluster ID a count (for size) and the minimum timestamp; (2) filter out clusters with size below `min_room_size`; (3) sort the remaining clusters by `(min_timestamp, cluster_id)` in ascending order. Edge cases include: duplicate timestamps within a cluster (the minimum is still correctly identified), clusters with only one node (size 1 may or may not pass the threshold), empty input (return empty vector), and negative timestamps are not possible since the type is `uint64_t`. For time complexity, building the cluster map is O(N) for N nodes, the filter is O(C) for C distinct clusters, and sorting is O(C log C), so overall O(N + C log C). Space complexity is O(C) for the map and result vector, which is at most O(N) in the worst case. The solution uses `std::unordered_map` for O(1) average access when accumulating, then copies valid entries into a vector of pairs and applies `std::sort` with a custom comparator. The comparator uses `std::tie` for lexicographical comparison of `(min_timestamp, cluster_id)` to guarantee deterministic ordering.

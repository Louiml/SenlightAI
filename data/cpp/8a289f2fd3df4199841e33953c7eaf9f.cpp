// Write a standalone C++ function named `findBestReps` that takes a square symmetric matrix of double precision distances (representing pairwise distances between frames, where `dist[i][j]` is the distance between frame `i` and frame `j`), a vector of integers `clusterAssignment` where each element indicates which cluster (0-based) the corresponding frame belongs to (all frames must be assigned to a valid cluster), and an integer `nToSave` (number of best representative frames to return per cluster). The function should return a `std::map<int, std::vector<std::pair<double,int>>>` where for each cluster ID, it maps to a vector of pairs `(cumulative_distance, frame_index)` for the `nToSave` frames in that cluster with the smallest cumulative distance to all other frames in the same cluster. The cumulative distance for a frame `f` in a cluster is defined as the sum of distances from `f` to every other frame in the same cluster. The returned vector must be sorted in ascending order by cumulative distance, and if two frames have equal cumulative distance, break ties by lower frame index. If a cluster has fewer frames than `nToSave`, save all frames in that cluster. If a cluster has zero frames, it should be omitted from the result. Assume the input matrix has at least one frame, the cluster assignment vector has the same length as the matrix dimension, and all distances are non-negative. The function must handle cases where the matrix may contain `DBL_MAX` (representing infinite distance) by treating structures as extremely dissimilar but still including them in cumulative sums (i.e., adding `DBL_MAX` to totals may overflow; if overflow occurs, use `DBL_MAX` as the cumulative sum). Use appropriate `const` correctness and do not modify the input.

// The solution iterates over each cluster ID that appears in `clusterAssignment`. For each cluster, we collect the indices of all frames belonging to that cluster. Then, for each frame in that cluster, we compute its cumulative distance by summing the distances to every other frame in the same cluster. We must be careful about overflow when adding `DBL_MAX` values: if the running total is already `DBL_MAX` or adding `dist` causes overflow (i.e., `total > DBL_MAX - dist`), we set the total to `DBL_MAX`. After computing cumulative distances, we need to select the smallest `nToSave` pairs. We use a max-heap (priority queue) of size `nToSave` to keep track of the worst (largest) among the best `nToSave` so far; when we encounter a pair with a smaller cumulative distance, we replace the top of the heap. To break ties by lower frame index, we define a comparator that orders by cumulative distance descending (so the largest distance is at the top), and if distances are equal, by frame index descending (so larger index is at the top). After processing all frames, we extract the pairs from the heap and sort them ascending by distance then by frame index to produce the final sorted vector. If a cluster has fewer frames than `nToSave`, all frames are kept. Time complexity is \(O(F_c^2 + F_c \log nToSave)\) per cluster, where \(F_c\) is the number of frames in that cluster. Across all clusters, the total is \(O(N^2 + N \log nToSave)\) where \(N\) is total number of frames. Space complexity is \(O(N + N_{clusters} \times nToSave)\) for storing results and per-cluster frame lists.

#include <map>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <cfloat> // DBL_MAX
#include <limits>

// Compute best representative frames for each cluster based on minimum cumulative distance.
std::map<int, std::vector<std::pair<double,int>>> findBestReps(
        const std::vector<std::vector<double>>& dist,
        const std::vector<int>& clusterAssignment,
        int nToSave)
{
    // Validate inputs: number of frames must match matrix dimension.
    int nFrames = static_cast<int>(clusterAssignment.size());
    if (nFrames == 0 || nToSave < 1)
        return {};

    // Group frame indices by cluster.
    std::map<int, std::vector<int>> clusterFrames;
    for (int i = 0; i < nFrames; ++i)
        clusterFrames[clusterAssignment[i]].push_back(i);

    std::map<int, std::vector<std::pair<double,int>>> result;

    for (const auto& entry : clusterFrames) {
        const int clusterId = entry.first;
        const std::vector<int>& frames = entry.second;
        if (frames.empty())
            continue; // Should not happen, but safe.

        // Data structure to keep the best nToSave (lowest cumulative distance) pairs.
        // Max-heap ordered by distance descending, then by frame index descending.
        auto cmp = [](const std::pair<double,int>& a, const std::pair<double,int>& b) {
            if (a.first != b.first)
                return a.first < b.first; // Larger distance at top
            return a.second < b.second;   // Larger index at top for tie-breaking
        };
        std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, decltype(cmp)> bestHeap(cmp);

        for (int f1 : frames) {
            double total = 0.0;
            for (int f2 : frames) {
                if (f1 == f2) continue;
                double d = dist[f1][f2];
                // Handle overflow when adding DBL_MAX.
                if (total == DBL_MAX || (d > DBL_MAX - total))
                    total = DBL_MAX;
                else
                    total += d;
            }
            std::pair<double,int> candidate = {total, f1};
            if (static_cast<int>(bestHeap.size()) < nToSave) {
                bestHeap.push(candidate);
            } else if (candidate < bestHeap.top() || !(bestHeap.top() < candidate)) {
                // If candidate is smaller (or equal but with smaller index) than top, replace.
                if (candidate.first < bestHeap.top().first ||
                    (candidate.first == bestHeap.top().first && candidate.second < bestHeap.top().second)) {
                    bestHeap.pop();
                    bestHeap.push(candidate);
                }
            }
        }

        // Extract pairs from heap and sort ascending by distance then by index.
        std::vector<std::pair<double,int>> reps;
        reps.reserve(bestHeap.size());
        while (!bestHeap.empty()) {
            reps.push_back(bestHeap.top());
            bestHeap.pop();
        }
        std::sort(reps.begin(), reps.end());
        result[clusterId] = reps;
    }

    return result;
}

#include <cassert>
#include <map>
#include <vector>
#include <utility>

// Include the solution function here (or link to it). For standalone, paste the code above.

int main() {
    // Test 1: simple distances
    std::vector<std::vector<double>> d1 = {
        {0, 1, 2},
        {1, 0, 3},
        {2, 3, 0}
    };
    std::vector<int> assign1 = {0, 0, 0};
    auto res1 = findBestReps(d1, assign1, 1);
    assert(res1.size() == 1);
    assert(res1[0].size() == 1);
    assert(res1[0][0].first == 3.0); // frame 0: 1+2=3, frame 1: 1+3=4, frame 2: 2+3=5
    assert(res1[0][0].second == 0);

    // Test 2: two clusters, nToSave > cluster size
    std::vector<std::vector<double>> d2 = {
        {0, 1},
        {1, 0}
    };
    std::vector<int> assign2 = {0, 1};
    auto res2 = findBestReps(d2, assign2, 5);
    assert(res2.size() == 2);
    assert(res2[0].size() == 1);
    assert(res2[1].size() == 1);
    assert(res2[0][0].first == 1.0);
    assert(res2[1][0].first == 1.0);

    // Test 3: tie-breaking by frame index
    std::vector<std::vector<double>> d3 = {
        {0, 2, 1},
        {2, 0, 2},
        {1, 2, 0}
    };
    std::vector<int> assign3 = {0, 0, 0};
    auto res3 = findBestReps(d3, assign3, 2);
    assert(res3[0].size() == 2);
    assert(res3[0][0].first == 3.0); // frame 0: 2+1=3
    assert(res3[0][0].second == 0);
    assert(res3[0][1].first == 4.0); // frame 2: 1+2=3? Wait, frame 2 distances: to 0=1, to 1=2 => total=3, frame 1: 2+2=4
    // Actually frame 2 total is 1+2=3, so both frame 0 and frame 2 have total 3. Tie broken by index.
    // Since we save 2 reps, both should be present with total 3, but sorted ascending by distance then index, so (3,0) then (3,2).
    assert(res3[0][0].second == 0 && res3[0][1].second == 2);
    assert(res3[0][1].first == 3.0);

    // Test 4: DBL_MAX overflow handling
    std::vector<std::vector<double>> d4 = {
        {0, DBL_MAX, 1},
        {DBL_MAX, 0, DBL_MAX},
        {1, DBL_MAX, 0}
    };
    std::vector<int> assign4 = {0, 0, 0};
    auto res4 = findBestReps(d4, assign4, 1);
    assert(res4[0].size() == 1);
    assert(res4[0][0].first == DBL_MAX); // frame 0: max+1 -> overflow => DBL_MAX
    assert(res4[0][0].second == 1); // frame 1 has max+max => DBL_MAX, frame 2 has 1+max=>DBL_MAX, all equal; tie by index => frame 0 has DBL_MAX too, but lowest index is 0. However, frame 0 total = DBL_MAX+1 = DBL_MAX (overflow), frame 1 = DBL_MAX+DBL_MAX = DBL_MAX, frame 2 = 1+DBL_MAX=DBL_MAX. All same, pick lowest index 0.
    assert(res4[0][0].second == 0);

    // Test 5: cluster with no frames (should be omitted)
    std::vector<std::vector<double>> d5 = {{0}};
    std::vector<int> assign5 = {3};
    auto res5 = findBestReps(d5, assign5, 1);
    assert(res5.size() == 1);
    assert(res5[3].size() == 1);
    assert(res5[3][0].second == 0);

    // Test 6: larger nToSave than frames, check sorted order
    std::vector<std::vector<double>> d6 = {
        {0, 5, 1},
        {5, 0, 2},
        {1, 2, 0}
    };
    std::vector<int> assign6 = {0, 0, 0};
    auto res6 = findBestReps(d6, assign6, 10);
    assert(res6[0].size() == 3);
    assert(res6[0][0].second == 2); // frame 2 total = 1+2=3
    assert(res6[0][1].second == 0); // frame 0 total = 5+1=6
    assert(res6[0][2].second == 1); // frame 1 total = 5+2=7
    assert(res6[0][0].first == 3.0);
    assert(res6[0][1].first == 6.0);
    assert(res6[0][2].first == 7.0);

    return 0;
}

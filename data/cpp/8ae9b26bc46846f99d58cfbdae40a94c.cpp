Write a C++ function that reads a set of 2D points from a text file (each line containing two floating-point numbers `x` and `y`), assigns a cluster label to each point using the DBSCAN algorithm, and returns the number of clusters found. The function must accept as parameters the filename, the radius `eps` (maximum distance between two points to be considered neighbors), and the minimum number of points `minPts` required to form a dense region. The function should modify a `std::vector<Point>` passed by reference, where `Point` is a struct with `float x`, `float y`, and `int lable` (note the spelling; initialize to -1 before calling). The labeling must be deterministic and handle noise points (points not in any cluster) by assigning them label 0. The function should be self-contained (no external libraries beyond standard C++ and the provided `Point` definition) and must work for arbitrary input files, including those with empty lines or trailing whitespace. The algorithm should use Euclidean distance and standard DBSCAN expansion (using a queue/stack for region growing), marking visited points to avoid reprocessing. Edge cases include: empty file (return 0, no labels changed), all points noise, points on the boundary between clusters, and points at exactly distance `eps` (they are considered neighbors, i.e., use `<=`). The function signature: `int dbscan(std::vector<Point>& points, float eps, int minPts)`. Note: the original code reads from a file, but for testing we will instead populate a vector directly and call the function; however, your solution must only implement the function, not the file reading.
The solution implements classic DBSCAN. For each point not yet visited, mark it visited and find all points within Euclidean distance `eps` (using squared distance to avoid sqrt overhead, comparing with `eps*eps`). If the number of neighbors (including itself) is less than `minPts`, mark it as noise (label 0). Otherwise, start a new cluster (increment cluster ID, assign this point the new label) and perform a breadth-first or depth-first expansion: for each neighbor, if not visited, mark visited and find its neighbors; if those neighbors count ≥ `minPts`, add them to the queue; if the neighbor’s label is -1 (unclustered) or 0 (previously noise), assign it the current cluster label. This ensures noise points that later become reachable are absorbed into the cluster. Complexity: For n points, the naive neighbor search is O(n) per point, leading to O(n^2) time; using a spatial index could improve, but here naive is acceptable. Space is O(n) for the queue and neighbor lists. Edge cases: points exactly at distance `eps` are included (use `<= distSq`), minPts of 1 would label every point as its own cluster, empty input returns 0, and all-noise input yields 0 clusters with labels 0.
#include <vector>
#include <cmath>
#include <queue>

struct Point {
    float x, y;
    int lable; // Note: typo from original, but kept for compatibility
};

// Helper: squared Euclidean distance between two points
inline float distSq(const Point& a, const Point& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

// Perform DBSCAN clustering on a vector of points.
// Modifies each point's 'lable' field: 0 = noise, >=1 = cluster ID.
// Returns number of clusters found (excluding noise).
int dbscan(std::vector<Point>& points, float eps, int minPts) {
    const float epsSq = eps * eps;
    const int n = static_cast<int>(points.size());
    std::vector<bool> visited(n, false);
    std::vector<int> neighborIndices;
    int clusterId = 0;

    for (int i = 0; i < n; ++i) {
        if (visited[i]) continue;
        visited[i] = true;

        // Find all neighbors of point i
        neighborIndices.clear();
        for (int j = 0; j < n; ++j) {
            if (distSq(points[i], points[j]) <= epsSq) {
                neighborIndices.push_back(j);
            }
        }

        if (static_cast<int>(neighborIndices.size()) < minPts) {
            points[i].lable = 0; // noise (tentatively)
        } else {
            ++clusterId;
            points[i].lable = clusterId;
            std::queue<int> toExpand;
            for (int idx : neighborIndices) {
                if (points[idx].lable == -1 || points[idx].lable == 0) {
                    points[idx].lable = clusterId;
                }
                if (!visited[idx]) {
                    visited[idx] = true;
                    toExpand.push(idx);
                }
            }

            while (!toExpand.empty()) {
                int current = toExpand.front();
                toExpand.pop();

                // Find neighbors of current
                std::vector<int> currentNeighbors;
                for (int j = 0; j < n; ++j) {
                    if (distSq(points[current], points[j]) <= epsSq) {
                        currentNeighbors.push_back(j);
                    }
                }

                if (static_cast<int>(currentNeighbors.size()) >= minPts) {
                    for (int idx : currentNeighbors) {
                        if (points[idx].lable == -1 || points[idx].lable == 0) {
                            points[idx].lable = clusterId;
                        }
                        if (!visited[idx]) {
                            visited[idx] = true;
                            toExpand.push(idx);
                        }
                    }
                }
            }
        }
    }

    // After clustering, any point still with lable -1 (should not happen) set to 0
    for (auto& p : points) {
        if (p.lable == -1) p.lable = 0;
    }
    return clusterId;
}
#include <cassert>
#include <vector>
#include <cmath>

// The Point struct and dbscan are assumed available (include them above in actual code)
// Here we just define them again for completeness, but in a real test they'd be included.
struct Point { float x, y; int lable; };

int dbscan(std::vector<Point>&, float, int); // forward declaration

int main() {
    // Test 1: Simple two clusters with noise
    std::vector<Point> pts1 = {
        {0,0,-1}, {1,0,-1}, {0,1,-1}, {1,1,-1}, // cluster 1
        {10,10,-1}, {11,10,-1}, {10,11,-1}, {11,11,-1}, // cluster 2
        {5,5,-1}, {100,100,-1} // noise
    };
    int c1 = dbscan(pts1, 2.0f, 3);
    assert(c1 == 2);
    for (size_t i = 0; i < 4; i++) assert(pts1[i].lable == 1);
    for (size_t i = 4; i < 8; i++) assert(pts1[i].lable == 2);
    assert(pts1[8].lable == 0 || pts1[8].lable == 0);
    assert(pts1[9].lable == 0);

    // Test 2: Boundary condition - points exactly at eps distance
    std::vector<Point> pts2 = {{0,0,-1}, {2,0,-1}, {4,0,-1}};
    int c2 = dbscan(pts2, 2.0f, 2); // eps=2, points at 0-2 and 2-4
    assert(c2 == 1); // all connected via chain
    assert(pts2[0].lable == 1 && pts2[1].lable == 1 && pts2[2].lable == 1);

    // Test 3: All noise
    std::vector<Point> pts3 = {{0,0,-1}, {10,10,-1}, {20,20,-1}};
    int c3 = dbscan(pts3, 1.0f, 2);
    assert(c3 == 0);
    for (auto& p : pts3) assert(p.lable == 0);

    // Test 4: Empty vector
    std::vector<Point> pts4;
    int c4 = dbscan(pts4, 1.0f, 1);
    assert(c4 == 0);

    // Test 5: minPts=1, every point is its own cluster
    std::vector<Point> pts5 = {{0,0,-1}, {1,1,-1}, {2,2,-1}};
    int c5 = dbscan(pts5, 0.5f, 1);
    assert(c5 == 3);
    assert(pts5[0].lable != pts5[1].lable && pts5[1].lable != pts5[2].lable);

    // Test 6: Single point isolated with minPts=2 -> noise
    std::vector<Point> pts6 = {{0,0,-1}};
    int c6 = dbscan(pts6, 1.0f, 2);
    assert(c6 == 0);
    assert(pts6[0].lable == 0);

    // Test 7: Dense cluster with more than minPts points
    std::vector<Point> pts7 = {
        {0,0,-1}, {0.5,0,-1}, {0,0.5,-1}, {0.5,0.5,-1}, {0.2,0.2,-1}
    };
    int c7 = dbscan(pts7, 1.0f, 4);
    assert(c7 == 1);
    for (auto& p : pts7) assert(p.lable == 1);

    return 0;
}

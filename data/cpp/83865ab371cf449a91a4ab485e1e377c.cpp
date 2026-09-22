// Write a standalone C++ function named `quantizePoints` that takes a vector of 3D points (represented as a simple struct with `float x, y, z`), a target maximum cluster count, an error threshold, and a collapse distance. The function should perform k-means clustering on the points, always producing at most the requested number of clusters, and return a struct containing: (1) the list of cluster centroids, (2) for each input point, the index of its assigned cluster (remapped to the final output cluster indices after pruning), and (3) the actual number of clusters produced. The algorithm must normalize input points into a unit bounding box before clustering, run k-means with up to 64 iterations (early exit on convergence), and then prune clusters that are empty or too close to each other (using squared distance compared to the collapse distance squared). Handle the edge case where the number of input points is less than or equal to the requested cluster count by returning each point as its own cluster. The function must be self-contained, use only standard library utilities, and avoid memory leaks.
// The core algorithm is Lloyd's k-means clustering with normalization and pruning. **Normalization:** Compute the axis-aligned bounding box of all input points, derive center and per-axis scale (with a 0.001 safety factor), and map each point to `(p - center) / scale` so the data fits roughly in [-1,1]. This prevents clustering from being biased by coordinate magnitudes. **Initialization:** The requested cluster count `K` is capped at the input size. Initial centroids are chosen by evenly spaced indices into the input array: `index = (i * N) / K`. **Iteration:** Repeatedly assign each point to its nearest centroid (by squared Euclidean distance), accumulate sums per cluster, and recompute centroids as the mean of assigned points. Track total squared error; stop when the error change is below the threshold, when error is below the threshold (e.g., zero error), or after 64 iterations. Handle empty clusters by leaving their centroid unchanged (count=0). **Pruning:** After convergence, first remove clusters with zero counts. Then, comparing surviving clusters in order, if a cluster is within squared `collapseDistance` of an already-accepted cluster, discard it and remap its assigned indices to the accepted cluster's index. Finally, compact the output clusters. **Edge cases:** If `N <= K`, just copy each point as its own cluster. If all points are identical, the error becomes zero in the first iteration, so the loop exits early. The time complexity is `O(iterations * N * K)` for the main loop plus `O(K^2)` for pruning; with 64 iterations this is effectively `O(N * K)` per iteration. Space usage is `O(N + K)` for temporary arrays (centroid sums, counts, indices). All memory is managed with `std::vector`.
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

struct Point3 {
    float x, y, z;
    Point3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};

struct QuantizeResult {
    std::vector<Point3> centroids;
    std::vector<unsigned int> indices;
    unsigned int clusterCount;
};

// Helper: squared Euclidean distance between two points.
static inline float squaredDist(const Point3& a, const Point3& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

QuantizeResult quantizePoints(const std::vector<Point3>& input,
                              unsigned int maxClusters,
                              float errorThreshold = 0.01f,
                              float collapseDistance = 0.01f) {
    QuantizeResult result;
    unsigned int N = static_cast<unsigned int>(input.size());
    if (N == 0) {
        result.clusterCount = 0;
        return result;
    }

    // Normalize input: translate to center, scale to unit box (with slight inflate).
    Point3 minP(input[0]), maxP(input[0]);
    for (const auto& p : input) {
        minP.x = std::min(minP.x, p.x); minP.y = std::min(minP.y, p.y); minP.z = std::min(minP.z, p.z);
        maxP.x = std::max(maxP.x, p.x); maxP.y = std::max(maxP.y, p.y); maxP.z = std::max(maxP.z, p.z);
    }
    Point3 center((minP.x + maxP.x) * 0.5f, (minP.y + maxP.y) * 0.5f, (minP.z + maxP.z) * 0.5f);
    Point3 dim(maxP.x - minP.x, maxP.y - minP.y, maxP.z - minP.z);
    dim.x *= 1.001f; dim.y *= 1.001f; dim.z *= 1.001f;
    if (dim.x == 0.0f) dim.x = 1.0f;
    if (dim.y == 0.0f) dim.y = 1.0f;
    if (dim.z == 0.0f) dim.z = 1.0f;
    Point3 scale(dim.x * 0.5f, dim.y * 0.5f, dim.z * 0.5f);
    std::vector<Point3> normInput(N);
    for (unsigned int i = 0; i < N; ++i) {
        normInput[i].x = (input[i].x - center.x) / scale.x;
        normInput[i].y = (input[i].y - center.y) / scale.y;
        normInput[i].z = (input[i].z - center.z) / scale.z;
    }

    unsigned int K = std::min(maxClusters, N);
    std::vector<Point3> clusters(K);
    std::vector<unsigned int> counts(K, 0);
    result.indices.resize(N, 0);

    // If we have fewer or equal points than clusters, output each point as its own cluster.
    if (N <= maxClusters) {
        K = N;
        for (unsigned int i = 0; i < K; ++i) {
            clusters[i] = normInput[i];
            result.indices[i] = i;
            counts[i] = 1;
        }
    }
    else {
        // Initialize centroids by evenly spaced sampling.
        for (unsigned int i = 0; i < K; ++i) {
            unsigned int idx = (i * N) / K;
            clusters[i] = normInput[idx];
        }

        std::vector<Point3> centroids(K); // sum accumulators
        float old_error = std::numeric_limits<float>::max();
        float error = std::numeric_limits<float>::max();
        unsigned int maxIter = 64;
        bool shouldBreak = false;

        do {
            old_error = error;
            for (unsigned int i = 0; i < K; ++i) {
                counts[i] = 0;
                centroids[i] = Point3(0.0f, 0.0f, 0.0f);
            }
            error = 0.0f;

            // Assign each point to nearest cluster and accumulate.
            for (unsigned int i = 0; i < N; ++i) {
                float minDist = std::numeric_limits<float>::max();
                unsigned int bestIdx = 0;
                for (unsigned int j = 0; j < K; ++j) {
                    float d = squaredDist(normInput[i], clusters[j]);
                    if (d < minDist) {
                        minDist = d;
                        bestIdx = j;
                    }
                }
                result.indices[i] = bestIdx;
                centroids[bestIdx].x += normInput[i].x;
                centroids[bestIdx].y += normInput[i].y;
                centroids[bestIdx].z += normInput[i].z;
                counts[bestIdx]++;
                error += minDist;
            }

            // Update centroids as means.
            for (unsigned int j = 0; j < K; ++j) {
                if (counts[j] > 0) {
                    float recip = 1.0f / static_cast<float>(counts[j]);
                    clusters[j].x = centroids[j].x * recip;
                    clusters[j].y = centroids[j].y * recip;
                    clusters[j].z = centroids[j].z * recip;
                }
            }

            if (--maxIter == 0) break;
            if (error < errorThreshold) break;
            shouldBreak = (std::fabs(error - old_error) < errorThreshold);
        } while (!shouldBreak);
    }

    // Prune clusters: remove empty and those too close to already accepted ones.
    std::vector<unsigned int> remap(K, 0); // old index -> new index
    std::vector<Point3> finalClusters;
    finalClusters.reserve(K);
    float d2 = collapseDistance * collapseDistance;
    unsigned int outCount = 0;

    for (unsigned int i = 0; i < K; ++i) {
        if (counts[i] == 0) continue; // empty cluster eliminated

        bool add = true;
        unsigned int remapIndex = outCount;
        for (unsigned int j = 0; j < outCount; ++j) {
            if (squaredDist(clusters[i], finalClusters[j]) < d2) {
                remapIndex = j;
                add = false;
                break;
            }
        }

        // Remap all points that used old index i.
        if (!add || outCount != i) {
            for (unsigned int p = 0; p < N; ++p) {
                if (result.indices[p] == i) {
                    result.indices[p] = remapIndex;
                }
            }
        }

        if (add) {
            finalClusters.push_back(clusters[i]);
            remap[i] = outCount;
            outCount++;
        }
    }

    // Denormalize centroids back to original coordinate space.
    result.centroids.resize(outCount);
    for (unsigned int i = 0; i < outCount; ++i) {
        result.centroids[i].x = finalClusters[i].x * scale.x + center.x;
        result.centroids[i].y = finalClusters[i].y * scale.y + center.y;
        result.centroids[i].z = finalClusters[i].z * scale.z + center.z;
    }
    result.clusterCount = outCount;
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume the Point3 and quantizePoints are defined above (or via #include).

static bool approxEqual(const Point3& a, const Point3& b, float eps = 1e-4f) {
    return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps && std::fabs(a.z - b.z) < eps;
}

int main() {
    // Case 1: single point, expect one cluster at that point.
    {
        std::vector<Point3> pts = { Point3(1.0f, 2.0f, 3.0f) };
        auto res = quantizePoints(pts, 5, 0.01f, 0.01f);
        assert(res.clusterCount == 1);
        assert(res.indices.size() == 1 && res.indices[0] == 0);
        assert(approxEqual(res.centroids[0], Point3(1.0f, 2.0f, 3.0f)));
    }

    // Case 2: fewer points than requested clusters; each becomes its own cluster.
    {
        std::vector<Point3> pts = { Point3(0.0f, 0.0f, 0.0f), Point3(10.0f, 0.0f, 0.0f) };
        auto res = quantizePoints(pts, 10, 0.01f, 0.01f);
        assert(res.clusterCount == 2);
        assert(res.indices.size() == 2);
        assert(res.indices[0] != res.indices[1]); // different clusters
        // Clusters should match input points (in some order).
        bool found0 = approxEqual(res.centroids[0], Point3(0,0,0)) || approxEqual(res.centroids[1], Point3(0,0,0));
        bool found10 = approxEqual(res.centroids[0], Point3(10,0,0)) || approxEqual(res.centroids[1], Point3(10,0,0));
        assert(found0 && found10);
    }

    // Case 3: two tight groups, request max 2 clusters, expect separation.
    {
        std::vector<Point3> pts;
        for (int i = 0; i < 10; ++i) {
            pts.push_back(Point3(0.0f + i*0.01f, 0.0f, 0.0f));
            pts.push_back(Point3(100.0f + i*0.01f, 0.0f, 0.0f));
        }
        auto res = quantizePoints(pts, 2, 0.0001f, 0.1f);
        assert(res.clusterCount == 2);
        assert(res.indices.size() == 20);
        // All indices should be 0 or 1.
        for (unsigned int i = 0; i < 20; ++i) {
            assert(res.indices[i] < 2);
        }
        // Centroids should be near 0 and 100.
        bool hasNear0 = approxEqual(res.centroids[0], Point3(0.05f,0,0), 0.1f) || approxEqual(res.centroids[1], Point3(0.05f,0,0), 0.1f);
        bool hasNear100 = approxEqual(res.centroids[0], Point3(100.05f,0,0), 0.1f) || approxEqual(res.centroids[1], Point3(100.05f,0,0), 0.1f);
        assert(hasNear0 && hasNear100);
    }

    // Case 4: all identical points, should produce one cluster.
    {
        std::vector<Point3> pts(100, Point3(5.0f, 5.0f, 5.0f));
        auto res = quantizePoints(pts, 5, 0.01f, 0.01f);
        assert(res.clusterCount == 1);
        assert(approxEqual(res.centroids[0], Point3(5.0f, 5.0f, 5.0f)));
        for (unsigned int i = 0; i < 100; ++i) assert(res.indices[i] == 0);
    }

    // Case 5: collapse distance prunes nearby clusters.
    {
        std::vector<Point3> pts = { Point3(0,0,0), Point3(0.001f,0,0), Point3(10,0,0), Point3(10.001f,0,0) };
        auto res = quantizePoints(pts, 4, 0.0001f, 0.01f); // collapse distance 0.01 > 0.001 gap
        assert(res.clusterCount <= 2); // should merge the close pairs
        assert(res.indices.size() == 4);
    }

    // Case 6: empty input returns zero clusters.
    {
        std::vector<Point3> pts;
        auto res = quantizePoints(pts, 3, 0.01f, 0.01f);
        assert(res.clusterCount == 0);
        assert(res.centroids.empty());
        assert(res.indices.empty());
    }

    return 0;
}

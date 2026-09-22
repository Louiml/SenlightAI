Implement a C++ function `int farthestPointSampling(const std::vector<float>& points, int numPoints, int numSamples, std::vector<int>& sampledIndices)` that performs farthest point sampling on a set of 3D points. The input `points` is a flat vector of size `numPoints * 3` where each consecutive triple `(x, y, z)` represents a point. The function should select `numSamples` points such that the first selected point is the one with the smallest Euclidean norm (distance from origin), and each subsequent selected point is the one in the remaining set that is farthest (in terms of minimum Euclidean distance) from all previously selected points. This is a greedy algorithm, not requiring distant-point replacement. The output `sampledIndices` must contain exactly `numSamples` integers (indices into the original points array, 0-based), or be empty if `numPoints == 0` or `numSamples <= 0` (also handle the case where `numSamples > numPoints` by only sampling `numPoints` points). The function must return 1 on success, 0 if inputs are invalid (e.g., `points.size()` not equal to `numPoints * 3`, or `numPoints < 1` with a non-empty points vector). The algorithm must be deterministic; ties are broken by always choosing the smallest index. The function should not modify the input `points` vector and should be implemented with proper `const` correctness.
#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Empty input
    {
        std::vector<float> points;
        std::vector<int> idx;
        assert(farthestPointSampling(points, 0, 3, idx) == 1);
        assert(idx.empty());
    }

    // Test 2: Invalid size mismatch
    {
        std::vector<float> points = {1.0f, 2.0f}; // only 2 floats, but numPoints=1 requires 3
        std::vector<int> idx;
        assert(farthestPointSampling(points, 1, 1, idx) == 0);
    }

    // Test 3: numSamples <= 0
    {
        std::vector<float> points = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 2, 0, idx) == 1);
        assert(idx.empty());
    }

    // Test 4: Single point
    {
        std::vector<float> points = {5.0f, -2.0f, 3.0f};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 1, 5, idx) == 1);
        assert(idx.size() == 1);
        assert(idx[0] == 0);
    }

    // Test 5: Two points, sample both (numSamples > numPoints)
    {
        std::vector<float> points = {0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 2, 10, idx) == 1);
        assert(idx.size() == 2);
        // First is origin (norm 0), second is the other point (distance 10)
        assert(idx[0] == 0);
        assert(idx[1] == 1);
    }

    // Test 6: Three points forming a triangle, sample all
    {
        std::vector<float> points = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 3, 3, idx) == 1);
        assert(idx.size() == 3);
        // First: origin (norm 0)
        assert(idx[0] == 0);
        // Second: farthest from origin among others -> distance 1, tie between idx1 and idx2, pick smallest index 1
        assert(idx[1] == 1);
        // Third: the remaining point idx2, minDistSq to {0,1} is 1 (to 1)
        assert(idx[2] == 2);
    }

    // Test 7: Four points, sample 2
    {
        // points: A(0,0,0), B(1,0,0), C(2,0,0), D(0,1,0)
        std::vector<float> points = {0,0,0, 1,0,0, 2,0,0, 0,1,0};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 4, 2, idx) == 1);
        assert(idx.size() == 2);
        // First: A (norm 0)
        assert(idx[0] == 0);
        // Farthest from A: C (dist 2) vs D (dist 1) -> C
        assert(idx[1] == 2);
    }

    // Test 8: Tie-breaking for first selection (same norm)
    {
        // Points: (1,0,0) and (-1,0,0) both norm 1; smallest index 0
        std::vector<float> points = {1,0,0, -1,0,0, 0,1,0};
        std::vector<int> idx;
        assert(farthestPointSampling(points, 3, 2, idx) == 1);
        assert(idx[0] == 0);
        // Farthest from point0: point1 (dist sqrt(4)=2) vs point2 (dist sqrt(2)) -> point1
        assert(idx[1] == 1);
    }

    // Test 9: Determinism and no modification of input
    {
        std::vector<float> points = {0,0,0, 1,1,1, -1,0,0, 2,0,0};
        std::vector<float> original = points;
        std::vector<int> idx1, idx2;
        assert(farthestPointSampling(points, 4, 3, idx1) == 1);
        assert(farthestPointSampling(points, 4, 3, idx2) == 1);
        assert(idx1 == idx2);
        assert(points == original); // input unchanged
    }

    return 0;
}
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

// Farthest point sampling on a set of 3D points.
// points: flat vector of size numPoints*3, each consecutive triple (x,y,z)
// numPoints: number of input points
// numSamples: desired number of samples (may exceed numPoints)
// sampledIndices: output vector containing selected indices in selection order
// Returns 1 on success, 0 on invalid input.
int farthestPointSampling(const std::vector<float>& points, int numPoints, int numSamples, std::vector<int>& sampledIndices) {
    // Validate input
    if (numPoints < 0 || points.size() != static_cast<size_t>(numPoints * 3)) {
        return 0;
    }
    if (numPoints == 0) {
        sampledIndices.clear();
        return 1;
    }
    // Adjust effective sample count
    int effectiveSamples = std::min(numSamples, numPoints);
    if (effectiveSamples <= 0) {
        sampledIndices.clear();
        return 1;
    }

    sampledIndices.clear();
    sampledIndices.reserve(effectiveSamples);

    // Helper lambda to compute squared distance between two points
    auto distSq = [&points](int i, int j) -> float {
        float dx = points[i*3] - points[j*3];
        float dy = points[i*3+1] - points[j*3+1];
        float dz = points[i*3+2] - points[j*3+2];
        return dx*dx + dy*dy + dz*dz;
    };

    // Track selected and min distance squared to any selected point
    std::vector<bool> selected(numPoints, false);
    std::vector<float> minDistSq(numPoints, std::numeric_limits<float>::max());

    // First selection: point with smallest distance from origin (squared norm)
    float minNormSq = std::numeric_limits<float>::max();
    int firstIdx = 0;
    for (int i = 0; i < numPoints; ++i) {
        float normSq = points[i*3]*points[i*3] + points[i*3+1]*points[i*3+1] + points[i*3+2]*points[i*3+2];
        // Tie-break: strictly less means first occurrence wins for equal values
        if (normSq < minNormSq) {
            minNormSq = normSq;
            firstIdx = i;
        }
    }
    selected[firstIdx] = true;
    sampledIndices.push_back(firstIdx);
    // Update minDistSq for all other points
    for (int i = 0; i < numPoints; ++i) {
        if (!selected[i]) {
            minDistSq[i] = distSq(i, firstIdx);
        }
    }

    // Subsequent selections: choose point with largest minDistSq (farthest from any selected)
    for (int k = 1; k < effectiveSamples; ++k) {
        float bestDistSq = -1.0f;
        int bestIdx = -1;
        // Find max minDistSq among unselected; tie-break by smallest index
        for (int i = 0; i < numPoints; ++i) {
            if (!selected[i]) {
                if (minDistSq[i] > bestDistSq) { // strictly greater for tie-break (smallest index)
                    bestDistSq = minDistSq[i];
                    bestIdx = i;
                }
            }
        }
        // bestIdx should always be found since k < effectiveSamples <= numPoints
        selected[bestIdx] = true;
        sampledIndices.push_back(bestIdx);
        // Update minDistSq for unselected points based on this new selected point
        for (int i = 0; i < numPoints; ++i) {
            if (!selected[i]) {
                float d = distSq(i, bestIdx);
                if (d < minDistSq[i]) {
                    minDistSq[i] = d;
                }
            }
        }
    }
    return 1;
}
// The solution uses an iterative greedy approach. First, we validate input: if `points.size() != numPoints * 3` or `numPoints < 1` (but `points` is non-empty) return 0; set `numSamples = min(numSamples, numPoints)`; if `numSamples <= 0` clear indices and return 1. We maintain a boolean vector `selected` of size `numPoints` (false initially) and a vector `minDistSq` of size `numPoints` storing the squared distance from each point to the nearest selected point, initialized to `FLT_MAX`. For the first selection, compute the squared norm for each point (distance from origin) and pick the point with the smallest value; tie-break by smallest index. Mark it selected, push its index, update `minDistSq` for all unselected points by computing distance to this selected point and taking the minimum between current `minDistSq` and this new distance. For each subsequent selection (loop `numSamples - 1` times), find the unselected point with the maximum `minDistSq`; if a tie, pick the smallest index. Mark selected, push index, and update `minDistSq` for all unselected points similarly. Edge cases: when `numSamples >= numPoints`, we still select all points, but after the first selection, the "farthest" logic will naturally select remaining points in order of maximum min-distance (which is 0 for already-selected neighbors eventually). Complexity: The selection loop runs `numSamples` times, and each selection scans all `numPoints` points twice (once to find max, once to update), so overall O(`numSamples * numPoints`) time, O(`numPoints`) extra space. This is acceptable for typical point cloud sizes. The norm comparisons use squared distances to avoid floating-point sqrt overhead, but since we compare distances, the relative ordering is preserved.

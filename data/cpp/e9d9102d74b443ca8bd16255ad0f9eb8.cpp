/*
Write a standalone C++ function `float hausdorffDistance(const std::vector<cv::Point2f>& set1, const std::vector<cv::Point2f>& set2, int distType, float rankProportion)` that computes the rank‑based Hausdorff distance between two finite sets of 2D points. The function must: (1) compute the pairwise distance matrix where the distance between points uses either `NORM_L1` (Manhattan) or `NORM_L2` (Euclidean), (2) for each point in the first set, find the minimum distance to any point in the second set, (3) sort these minimum distances in descending order, (4) select the element at rank index `K = floor(rankProportion * (N-1))` where `N` is the number of points in the first set, and (5) return the maximum of the two directional values (from set1→set2 and set2→set1). Valid inputs: both sets are non‑empty, `rankProportion` is strictly between 0 and 1 (inclusive of 1), and `distType` is either `NORM_L1` or `NORM_L2`. The function must handle the case where the input sets are identical, and must use efficient computation without overly large memory usage.
*/
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

// Compute rank-based Hausdorff distance between two sets of 2D points.
// distType: 1 for NORM_L1 (Manhattan), 2 for NORM_L2 (Euclidean)
float hausdorffDistance(const std::vector<cv::Point2f>& set1,
                        const std::vector<cv::Point2f>& set2,
                        int distType,
                        float rankProportion) {
    // Validate inputs
    if (set1.empty() || set2.empty())
        throw std::invalid_argument("Both point sets must be non-empty");
    if (distType != 1 && distType != 2)
        throw std::invalid_argument("distType must be 1 (L1) or 2 (L2)");
    if (rankProportion <= 0.0f || rankProportion > 1.0f)
        throw std::invalid_argument("rankProportion must be in (0, 1]");

    // Helper lambda to compute distance between two points
    auto pointDistance = [&](const cv::Point2f& a, const cv::Point2f& b) -> float {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        if (distType == 1) {
            return std::abs(dx) + std::abs(dy);
        } else { // L2
            return std::sqrt(dx*dx + dy*dy);
        }
    };

    // Compute directional distance from set A to set B
    auto directional = [&](const std::vector<cv::Point2f>& A,
                           const std::vector<cv::Point2f>& B) -> float {
        size_t n = A.size();
        size_t m = B.size();

        // Build distance matrix (n x m)
        std::vector<std::vector<float>> distMat(n, std::vector<float>(m));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                distMat[i][j] = pointDistance(A[i], B[j]);
            }
        }

        // Find minimum distance for each point in A
        std::vector<float> shortest(n);
        for (size_t i = 0; i < n; ++i) {
            float minVal = distMat[i][0];
            for (size_t j = 1; j < m; ++j) {
                if (distMat[i][j] < minVal)
                    minVal = distMat[i][j];
            }
            shortest[i] = minVal;
        }

        // Sort descending
        std::sort(shortest.begin(), shortest.end(), std::greater<float>());

        // Select rank index
        int K = static_cast<int>(rankProportion * (n - 1));
        return shortest[K];
    };

    // The Hausdorff distance is the max of the two directional distances
    float d1 = directional(set1, set2);
    float d2 = directional(set2, set1);
    return std::max(d1, d2);
}
#include <cassert>
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>

// The solution function is assumed to be defined above.
// This test uses OpenCV Point2f, which is a simple struct with x and y.

int main() {
    // Test 1: Identical sets -> distance 0
    std::vector<cv::Point2f> setA = { {0,0}, {1,1}, {2,2} };
    std::vector<cv::Point2f> setB = { {0,0}, {1,1}, {2,2} };
    assert(std::abs(hausdorffDistance(setA, setB, 1, 1.0f) - 0.0f) < 1e-5);
    assert(std::abs(hausdorffDistance(setA, setB, 2, 1.0f) - 0.0f) < 1e-5);

    // Test 2: Simple L1 classic Hausdorff (rank=1) 
    // setA: (0,0), (10,0); setB: (0,0), (0,10)
    // Direction A->B: min distances: for (0,0) -> 0, for (10,0) -> 10 => max=10
    // Direction B->A: min distances: for (0,0) -> 0, for (0,10) -> 10 => max=10
    std::vector<cv::Point2f> setC = { {0,0}, {10,0} };
    std::vector<cv::Point2f> setD = { {0,0}, {0,10} };
    assert(std::abs(hausdorffDistance(setC, setD, 1, 1.0f) - 10.0f) < 1e-5);

    // Test 3: L2 with rank=1 (classic)
    // setC: (0,0),(10,0); setD: (0,0),(0,10)
    // For (10,0) to setD: nearest is (0,0) => distance 10, (0,10) => sqrt(200)
    // So min=10. Direction B->A similar => max=10
    assert(std::abs(hausdorffDistance(setC, setD, 2, 1.0f) - 10.0f) < 1e-5);

    // Test 4: Rank proportion < 1 (median of minima)
    // setA: (0,0),(100,0); setB: (0,0),(0,0) — both points identical at origin
    // Direction A->B: minima: 0 and 100 -> sorted descending [100,0], K = floor(0.5*1)=0 => 100
    // Direction B->A: both minima 0 -> sorted [0,0], K=0 => 0
    // max = 100
    std::vector<cv::Point2f> setE = { {0,0}, {100,0} };
    std::vector<cv::Point2f> setF = { {0,0}, {0,0} };
    assert(std::abs(hausdorffDistance(setE, setF, 1, 0.5f) - 100.0f) < 1e-5);

    // Test 5: rankProportion = 1 with two points gives max of minima
    // setE and setF as above, but rank=1 => K=1 => minima sorted [100,0], so index1=0
    // Direction A->B: 0, Direction B->A: 0 => result 0
    assert(std::abs(hausdorffDistance(setE, setF, 1, 1.0f) - 0.0f) < 1e-5);

    // Test 6: Larger difference , L2
    // setA: (0,0),(3,4); setB: (0,0),(0,0)
    // Direction A->B: minima: 0 and 5 => max=5; Direction B->A: all 0 => max=5
    std::vector<cv::Point2f> setG = { {0,0}, {3,4} };
    std::vector<cv::Point2f> setH = { {0,0}, {0,0} };
    assert(std::abs(hausdorffDistance(setG, setH, 2, 1.0f) - 5.0f) < 1e-5);

    // Test 7: Non-symmetric sets
    // setA: (0,0); setB: (1,0),(2,0)
    // Direction A->B: min distance = 1, K=0 => 1
    // Direction B->A: minima: 1 and 2, sorted [2,1], K=floor(0.5*1)=0 => 2
    // max = 2
    std::vector<cv::Point2f> setI = { {0,0} };
    std::vector<cv::Point2f> setJ = { {1,0}, {2,0} };
    assert(std::abs(hausdorffDistance(setI, setJ, 1, 0.5f) - 2.0f) < 1e-5);

    // Test 8: Single points with L1
    std::vector<cv::Point2f> setK = { {3,4} };
    std::vector<cv::Point2f> setL = { {0,0} };
    assert(std::abs(hausdorffDistance(setK, setL, 1, 1.0f) - 7.0f) < 1e-5);
    assert(std::abs(hausdorffDistance(setK, setL, 2, 1.0f) - 5.0f) < 1e-5);

    return 0;
}
// The algorithm proceeds in three phases. First, build a distance matrix `D` of size `n✕m` where `n = set1.size()` and `m = set2.size()`, computing each entry `D[i][j]` as the selected norm of the vector `set1[i] - set2[j]`. This is the most expensive step: `O(n·m)` time and space. Second, for each row `i` (each point in set1), determine the minimum value in that row (the closest point in set2) using a linear scan — `O(m)` per row, total `O(n·m)`. Collect these `n` minima into a vector `shortest`. Third, sort `shortest` in descending order (`O(n log n)`) and return the element at index `K = floor(rankProportion * (n-1))`. The final result is the maximum of the directional value computed from set1→set2 and the same computation from set2→set1. Important edge cases: if `rankProportion == 1.0`, then `K = n-1` which selects the maximum of the minima (the classic Hausdorff distance). If `rankProportion` is small (e.g., 0.1) with large `n`, `K` will be near 0, and the returned value approximates the median minimum. When both sets are identical, all minima will be 0 for any norm, so the result will be 0. The implementation must ensure `K` is a valid index (non‑negative and less than `n`) — given the constraint `0 < rankProportion ≤ 1`, this always holds. For `distType`, we compute Manhattan distance as `|dx|+|dy|` and Euclidean as `sqrt(dx²+dy²)`. The time complexity is dominated by the pairwise distance matrix and row minima: `O(n·m)` time and `O(n·m)` space (the matrix). Sorting adds `O(n log n)`, but this is negligible compared to `n·m` for large sets. An alternative memory‑saving approach could compute row minima on the fly without storing the full matrix, but the task explicitly asks for the matrix approach to keep the logic clear.

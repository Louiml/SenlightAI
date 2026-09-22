// Write a standalone C++ function that takes as input a positive integer `n` followed by `n+2` pairs of real coordinates: first a query point `P`, then a closed polygon with `n` vertices given in order (the last vertex implicitly connects back to the first). The function must compute the area of the annular region between the circle centered at `P` that passes through the farthest polygon vertex and the circle centered at `P` that is tangent to or closest to any polygon edge (the distance to a segment, not the infinite line, must be used). Return this area as a `double`. The polygon may be non‑convex, vertices may repeat, and edges may be zero-length. The function signature is `double annularArea(int n, const vector<pair<double,double>>& vertices, const pair<double,double>& P);` where `vertices` has exactly `n` entries and represents the polygon in order. The area is computed as `π * (maxDist² - minDist²)`, where `maxDist` is the Euclidean distance from `P` to the farthest vertex, and `minDist` is the minimum distance from `P` to any polygon edge (treating each edge as a segment). Use `π = asin(1.0)*2.0`. The function must be self-contained and handle all edge cases (e.g., zero-length edges, coincident points, distances near zero) robustly.
// The core problem is computing two distances from a fixed point `P` to a polygon defined by `n` vertices:
// 1. **Farthest vertex distance** – simply take the maximum Euclidean distance squared from `P` to any vertex, then take the square root.
// 2. **Minimum distance to any edge** – for each consecutive pair of vertices (including the closing edge from the last to the first vertex), compute the perpendicular distance from `P` to the infinite line containing the segment, but only if the projection of `P` falls within the segment’s endpoints. Otherwise, the distance to the segment is the distance to the nearest endpoint. The projection condition is verified using the dot product of `(P - A)` with the segment direction `(B - A)`: if the dot product is ≤ 0, nearest is A; if ≥ (length of AB)², nearest is B; otherwise the perpendicular distance is used. The perpendicular distance squared to the line can be computed via the formula `|cross(AB, AP)|² / |AB|²` where cross in 2D is `ax*by - ay*bx`. To avoid issues with zero-length edges, skip edges where both endpoints are identical (then the distance to that edge equals the distance to the point). Also note the original code has a bug in the condition `d1*d2 >= 1` (should be `>= 0` and also checks inside/outside) but we ignore that and write a correct, clean implementation.
//
// **Algorithm Steps:**
// - Initialize `maxDistSq = 0`, `minDistSq = +infinity`.
// - For each vertex `v`, compute `dx = v.first - P.first`, `dy = v.second - P.second`, `distSq = dx*dx + dy*dy`, update `maxDistSq = max(maxDistSq, distSq)`.
// - For each edge from `A` to `B` (including the closing edge), compute:
//   - `abx = B.first - A.first`, `aby = B.second - A.second`
//   - `abSq = abx*abx + aby*aby`
//   - If `abSq == 0`, then the edge is a point; treat distance squared as distance from `P` to `A`.
//   - Else compute `t = ( (P.first - A.first)*abx + (P.second - A.second)*aby ) / abSq`. Clamp `t` to [0,1] to get the projection parameter along the segment.
//   - Compute the closest point `C = A + t*(B-A)`, then `distSq = (C.first - P.first)² + (C.second - P.second)²`.
//   - Update `minDistSq = min(minDistSq, distSq)`.
// - Finally compute `area = PI * (sqrt(maxDistSq)² - sqrt(minDistSq)²) = PI * (maxDistSq - minDistSq)`. Since we have squared distances, that’s simply `PI * (maxDistSq - minDistSq)`. However the problem statement says "maxDist² - minDist²", which equals `maxDistSq - minDistSq` directly, so we return `PI * (maxDistSq - minDistSq)`.
//
// **Edge Cases:**  
// - `n` can be 1: then the polygon degenerates to a single point; the only edge is from that point to itself (zero-length). The farthest vertex and the min distance both equal the distance to that point; area becomes 0.  
// - All vertices coincide with `P`: both max and min distances are 0, area 0.  
// - A vertex lies exactly on an edge: the min distance could be 0.  
// - Very large coordinates: use `double` for all arithmetic to avoid overflow.  
// - Negative coordinates: handled naturally.
//
// **Time Complexity:** O(n) because we scan vertices once for max and edges once for min.  
// **Space Complexity:** O(1) extra beyond the input vector.
#include <vector>
#include <cmath>
#include <limits>
#include <utility>

// Compute the area of the annular region between the farthest and nearest circle centered at P.
// vertices: polygon vertices in order, size n.
// P: query point.
// Returns PI * (maxDistSq - minDistSq) where maxDistSq is the maximum squared distance to a vertex,
// and minDistSq is the minimum squared distance to any polygon edge (segment).
double annularArea(int n, const std::vector<std::pair<double,double>>& vertices,
                   const std::pair<double,double>& P) {
    const double PI = std::asin(1.0) * 2.0;
    const double INF = std::numeric_limits<double>::infinity();

    double maxDistSq = 0.0;
    double minDistSq = INF;

    // Farthest vertex distance squared
    for (const auto& v : vertices) {
        double dx = v.first - P.first;
        double dy = v.second - P.second;
        double distSq = dx*dx + dy*dy;
        if (distSq > maxDistSq) {
            maxDistSq = distSq;
        }
    }

    // Minimum distance squared to any edge (segment)
    for (int i = 0; i < n; ++i) {
        const auto& A = vertices[i];
        const auto& B = vertices[(i+1) % n]; // wrap around to close the polygon

        double abx = B.first - A.first;
        double aby = B.second - A.second;
        double abSq = abx*abx + aby*aby;

        double distSq;
        if (abSq == 0.0) {
            // Zero-length edge: distance to the point A
            double dx = A.first - P.first;
            double dy = A.second - P.second;
            distSq = dx*dx + dy*dy;
        } else {
            // Projection parameter t clamped to [0,1]
            double t = ((P.first - A.first)*abx + (P.second - A.second)*aby) / abSq;
            if (t < 0.0) t = 0.0;
            if (t > 1.0) t = 1.0;
            double cx = A.first + t*abx;
            double cy = A.second + t*aby;
            double dx = cx - P.first;
            double dy = cy - P.second;
            distSq = dx*dx + dy*dy;
        }

        if (distSq < minDistSq) {
            minDistSq = distSq;
        }
    }

    // Area = PI * (R_max^2 - r_min^2) = PI * (maxDistSq - minDistSq)
    return PI * (maxDistSq - minDistSq);
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Function under test (insert the provided solution code here)
double annularArea(int n, const std::vector<std::pair<double,double>>& vertices,
                   const std::pair<double,double>& P);

int main() {
    const double eps = 1e-9;

    // Test 1: Square of side 2 centered at (0,0), query point at center.
    // Vertices: (1,1), (-1,1), (-1,-1), (1,-1). Farthest distance = sqrt(2) from center
    // to any vertex, min distance to edge = 1 (perpendicular to side).
    // Area = PI * (2 - 1) = PI.
    std::vector<std::pair<double,double>> sq = {{1,1}, {-1,1}, {-1,-1}, {1,-1}};
    double a1 = annularArea(4, sq, {0,0});
    assert(std::abs(a1 - M_PI) < eps);

    // Test 2: Triangle with vertices (0,0), (2,0), (0,2). Query point at (0,0).
    // Farthest vertex distance = sqrt(8) (to (2,0) or (0,2)), min distance to edge
    // is 0 because P is on a vertex (distance to edge (0,0)-(2,0) is 0).
    // Area = PI * (8 - 0) = 8*PI.
    std::vector<std::pair<double,double>> tri = {{0,0}, {2,0}, {0,2}};
    double a2 = annularArea(3, tri, {0,0});
    assert(std::abs(a2 - 8.0*M_PI) < eps);

    // Test 3: Single point polygon at (5,5). Query point at (1,1).
    // Both max and min distances are sqrt( (4)^2 + (4)^2 ) = sqrt(32). Area = PI*(32-32)=0.
    std::vector<std::pair<double,double>> single = {{5,5}};
    double a3 = annularArea(1, single, {1,1});
    assert(std::abs(a3) < 1e-12);

    // Test 4: Rectangle with vertices (0,0),(4,0),(4,3),(0,3). Query point at (2,1.5).
    // Farthest vertex distance to (0,0) = sqrt( (2)^2 + (1.5)^2 ) = sqrt(6.25)=2.5.
    // Min distance to edge is 1.5 (to top or bottom edge). So area = PI*(6.25 - 2.25)=4*PI.
    std::vector<std::pair<double,double>> rect = {{0,0}, {4,0}, {4,3}, {0,3}};
    double a4 = annularArea(4, rect, {2,1.5});
    assert(std::abs(a4 - 4.0*M_PI) < eps);

    // Test 5: Concave polygon (L-shape) with vertices (0,0),(2,0),(2,1),(1,1),(1,2),(0,2).
    // Query point at (0.5,1.5) inside the L. Farthest vertex is (2,0) → dist² = (1.5²+1.5²)=4.5.
    // Min distance to edge (0,0)-(2,0) is 1.5, to (0,0)-(0,2) is 0.5, to (0,2)-(1,2) is 0.5,
    // to (1,2)-(1,1) is 0.5, to (1,1)-(2,1) is 0.5, to (2,1)-(2,0) is 1.5.
    // So min dist² = 0.25. Area = PI*(4.5 - 0.25) = PI*4.25 = 4.25*PI.
    std::vector<std::pair<double,double>> lshape = {{0,0},{2,0},{2,1},{1,1},{1,2},{0,2}};
    double a5 = annularArea(6, lshape, {0.5,1.5});
    assert(std::abs(a5 - 4.25*M_PI) < eps);

    // Test 6: Query point outside polygon: square (0,0)-(2,2). Query at (3,1).
    // Farthest vertex to (0,0) dist² = 10, to (0,2) dist² = 10, others 5. MaxDistSq=10.
    // Min distance to edge (2,0)-(2,2) is perpendicular distance 1 (from (3,1) to (2,1)).
    // So area = PI*(10 - 1) = 9*PI.
    std::vector<std::pair<double,double>> sq2 = {{0,0},{2,0},{2,2},{0,2}};
    double a6 = annularArea(4, sq2, {3,1});
    assert(std::abs(a6 - 9.0*M_PI) < eps);

    // Test 7: Zero-length edge: vertices (1,1),(1,1),(3,1) triangle degenerate.
    // Query at (0,0). Farthest vertex to (3,1) dist² = 10. Min distance to edge (1,1)-(1,1)
    // is dist² to (1,1) = 2. To edge (1,1)-(3,1) is perpendicular distance to line y=1,
    // projection of (0,0) onto line has t = ( -1*2 + -1*0 )/4 = -0.5 clamped to 0, nearest (1,1) dist²=2.
    // So minDistSq=2. Area = PI*(10-2)=8*PI.
    std::vector<std::pair<double,double>> degenerate = {{1,1},{1,1},{3,1}};
    double a7 = annularArea(3, degenerate, {0,0});
    assert(std::abs(a7 - 8.0*M_PI) < eps);

    return 0;
}

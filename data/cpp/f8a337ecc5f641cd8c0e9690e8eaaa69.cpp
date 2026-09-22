/*
Given a vector of 2D points represented as pairs of doubles (or a simple struct with `x` and `y`), write a C++ function `std::vector<std::array<size_t,3>> computeDelaunayTriangulation(const std::vector<Point2D>& points)` that constructs the Delaunay triangulation of the input points using the Bowyer-Watson incremental algorithm. The function must return only the triangles whose vertices are all original input points; any triangles created using temporary super-triangle vertices must be removed. Each triangle is represented by a `std::array<size_t,3>` containing indices into the original input vector (order of indices within a triangle does not matter). Handle duplicate points (if any) gracefully—you may ignore them during triangulation but must return triangles only for unique input points, or you may assume all points are distinct. The function should be robust for cases with 0, 1, 2, or 3+ points: for fewer than 3 points, return an empty vector. For exactly 3 non-collinear points, return one triangle. For collinear points, return an empty vector. The output must be a valid Delaunay triangulation satisfying the empty-circle property: no input point lies strictly inside the circumcircle of any output triangle.
*/

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <limits>

struct Point2D {
    double x;
    double y;
};

// Helper: squared distance between two points
double distSq(const Point2D& a, const Point2D& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Helper: determine if point p lies inside or on circumcircle of triangle (a,b,c)
// Uses determinant method for robustness.
bool inCircumcircle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c) {
    double ax = a.x - p.x;
    double ay = a.y - p.y;
    double bx = b.x - p.x;
    double by = b.y - p.y;
    double cx = c.x - p.x;
    double cy = c.y - p.y;

    double det = (ax*ax + ay*ay) * (bx*cy - cx*by) -
                 (bx*bx + by*by) * (ax*cy - cx*ay) +
                 (cx*cx + cy*cy) * (ax*by - bx*ay);

    // For CCW triangle (we ensure orientation), inside if det > 0
    // For CW, inside if det < 0. We ensure our triangles are CCW.
    return det > 0; // using exact orientation: we make sure all triangles have positive area
}

// Helper: cross product to determine orientation
double cross(const Point2D& a, const Point2D& b, const Point2D& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Main function: compute Delaunay triangulation of input points
std::vector<std::array<size_t,3>> computeDelaunayTriangulation(const std::vector<Point2D>& points) {
    std::vector<std::array<size_t,3>> result;
    size_t n = points.size();
    if (n < 3) return result; // too few points

    // Find bounding box to create super-triangle
    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    for (const auto& p : points) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }
    double dx = maxX - minX;
    double dy = maxY - minY;
    double dmax = std::max(dx, dy);
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;

    // Super-triangle vertices (indices n, n+1, n+2) — we'll add them to a local copy of points
    Point2D p1{midX - 20*dmax, midY - dmax};
    Point2D p2{midX, midY + 20*dmax};
    Point2D p3{midX + 20*dmax, midY - dmax};

    std::vector<Point2D> pts = points;
    size_t super1 = n, super2 = n+1, super3 = n+2;
    pts.push_back(p1);
    pts.push_back(p2);
    pts.push_back(p3);

    // Initial triangle: super-triangle (indices n, n+1, n+2)
    // Ensure CCW orientation: cross product should be positive
    std::array<size_t,3> initial = {super1, super2, super3};
    if (cross(pts[super1], pts[super2], pts[super3]) < 0) {
        std::swap(initial[1], initial[2]);
    }
    std::vector<std::array<size_t,3>> triangles;
    triangles.push_back(initial);

    // Process each original point
    for (size_t i = 0; i < n; ++i) {
        const Point2D& p = pts[i];

        // Find all triangles whose circumcircle contains p
        std::vector<std::array<size_t,3>> badTriangles;
        std::vector<size_t> badIndices;
        for (size_t t = 0; t < triangles.size(); ++t) {
            const auto& tri = triangles[t];
            if (inCircumcircle(p, pts[tri[0]], pts[tri[1]], pts[tri[2]])) {
                badTriangles.push_back(tri);
                badIndices.push_back(t);
            }
        }

        // Collect boundary edges from bad triangles (edges shared by exactly one bad triangle)
        // Use a set to deduplicate edges (each edge appears twice if shared)
        struct Edge {
            size_t a, b;
        };
        std::vector<Edge> edgeList;
        for (const auto& tri : badTriangles) {
            // All three edges
            std::array<Edge,3> edges = {{{tri[0],tri[1]}, {tri[1],tri[2]}, {tri[2],tri[0]}}};
            for (auto e : edges) {
                if (e.a > e.b) std::swap(e.a, e.b); // canonical order
                edgeList.push_back(e);
            }
        }

        // Count occurrences of each edge; boundary edges appear exactly once
        // Simplest: sort and count
        std::sort(edgeList.begin(), edgeList.end(), [](const Edge& a, const Edge& b) {
            return a.a < b.a || (a.a == b.a && a.b < b.b);
        });
        std::vector<Edge> boundary;
        for (size_t k = 0; k < edgeList.size(); ) {
            size_t j = k;
            while (j < edgeList.size() && edgeList[j].a == edgeList[k].a && edgeList[j].b == edgeList[k].b) ++j;
            if (j - k == 1) boundary.push_back(edgeList[k]);
            k = j;
        }

        // Remove bad triangles from list
        // We can build a new list
        std::vector<std::array<size_t,3>> newTriangles;
        for (size_t t = 0; t < triangles.size(); ++t) {
            bool isBad = false;
            for (size_t b : badIndices) if (b == t) { isBad = true; break; }
            if (!isBad) newTriangles.push_back(triangles[t]);
        }

        // Add new triangles connecting p to each boundary edge
        for (const auto& e : boundary) {
            std::array<size_t,3> tri = {e.a, e.b, i};
            // Ensure CCW orientation
            if (cross(pts[tri[0]], pts[tri[1]], pts[tri[2]]) < 0) {
                std::swap(tri[0], tri[1]);
            }
            newTriangles.push_back(tri);
        }

        triangles = std::move(newTriangles);
    }

    // Remove triangles that use any super-triangle vertex
    for (const auto& tri : triangles) {
        if (tri[0] < n && tri[1] < n && tri[2] < n) {
            result.push_back(tri);
        }
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <iostream>

// Include the solution header or code here (from above)

bool sameTriangle(const std::array<size_t,3>& a, const std::array<size_t,3>& b) {
    // Compare sets of three indices
    std::array<size_t,3> sa = a, sb = b;
    std::sort(sa.begin(), sa.end());
    std::sort(sb.begin(), sb.end());
    return sa == sb;
}

int main() {
    // Test 1: Square 4 points -> 2 triangles
    std::vector<Point2D> square = {{0,0}, {1,0}, {0,1}, {1,1}};
    auto tris = computeDelaunayTriangulation(square);
    assert(tris.size() == 2);
    // Verify each triangle indices are within [0,3]
    for (auto& t : tris) {
        assert(t[0] < 4 && t[1] < 4 && t[2] < 4);
    }

    // Test 2: Triangle 3 points -> 1 triangle
    std::vector<Point2D> tri = {{0,0}, {1,0}, {0,1}};
    tris = computeDelaunayTriangulation(tri);
    assert(tris.size() == 1);
    assert(sameTriangle(tris[0], {0,1,2}));

    // Test 3: Collinear points -> 0 triangles
    std::vector<Point2D> line = {{0,0}, {1,1}, {2,2}};
    tris = computeDelaunayTriangulation(line);
    assert(tris.empty());

    // Test 4: Two points -> empty
    std::vector<Point2D> two = {{0,0}, {1,1}};
    tris = computeDelaunayTriangulation(two);
    assert(tris.empty());

    // Test 5: One point -> empty
    std::vector<Point2D> one = {{5,5}};
    tris = computeDelaunayTriangulation(one);
    assert(tris.empty());

    // Test 6: Regular hexagon (6 points) -> should have 4 triangles (n-2)
    std::vector<Point2D> hex;
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * M_PI * i / 6;
        hex.push_back({cos(angle), sin(angle)});
    }
    tris = computeDelaunayTriangulation(hex);
    // For convex polygon with n vertices, Delaunay triangulation has n-2 triangles
    assert(tris.size() == 4);

    // Test 7: Concentric / random points (validity check) — verify no point inside any circumcircle
    std::vector<Point2D> random = {{0,0}, {2,0}, {1,1}, {0,2}, {2,2}, {1,3}};
    tris = computeDelaunayTriangulation(random);
    // Verify empty-circle property for each triangle
    for (const auto& t : tris) {
        Point2D a = random[t[0]], b = random[t[1]], c = random[t[2]];
        // Compute circumcenter
        double d = 2 * (a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y));
        if (std::abs(d) < 1e-12) continue; // degenerate
        double ux = ((a.x*a.x + a.y*a.y)*(b.y - c.y) + (b.x*b.x + b.y*b.y)*(c.y - a.y) + (c.x*c.x + c.y*c.y)*(a.y - b.y)) / d;
        double uy = ((a.x*a.x + a.y*a.y)*(c.x - b.x) + (b.x*b.x + b.y*b.y)*(a.x - c.x) + (c.x*c.x + c.y*c.y)*(b.x - a.x)) / d;
        double r2 = distSq({ux,uy}, a);
        for (size_t idx = 0; idx < random.size(); ++idx) {
            // Skip triangles' own vertices
            if (idx == t[0] || idx == t[1] || idx == t[2]) continue;
            if (distSq({ux,uy}, random[idx]) < r2 - 1e-9) {
                assert(false && "Point inside circumcircle");
            }
        }
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The Bowyer-Watson algorithm works by maintaining a set of triangles that form a valid triangulation of a subset of points. Initially, we create a large "super-triangle" that contains all input points. For each input point `p`, we find all triangles whose circumcircle contains `p` (including on boundary) — these are the "bad" triangles. Removing these bad triangles leaves a polygonal cavity. The boundary of this cavity consists of edges that belong to exactly one bad triangle; these are the "boundary edges." We then re-triangulate the cavity by connecting `p` to each boundary edge, forming new triangles. After processing all input points, we remove any triangle that includes any super-triangle vertex. Important edge cases: (1) Handling the super-triangle: we need to ensure it is large enough to contain all points, and we track its vertex indices (which are original indices like `n`, `n+1`, `n+2`). (2) Duplicate points: if the input contains duplicates, the algorithm may produce degenerate triangles or fail; we assume distinct points (or we can filter duplicates first). (3) Collinear points: the circumcircle test becomes degenerate; we must ensure we don't produce triangles for collinear sets. (4) Numerical robustness: we use squared distances and a cross-product-based orientation test, but the circumcircle test should use double precision and proper comparisons with a small epsilon. The algorithm uses a set to collect boundary edges (or a sorted unique edge list) to avoid duplicates. Time complexity: For each of `n` points, we iterate over all current triangles, which can be `O(n)` in the worst case, leading to `O(n^2)` total, but practical performance is near-linear for random points. Space complexity is `O(n)` for storing triangles.

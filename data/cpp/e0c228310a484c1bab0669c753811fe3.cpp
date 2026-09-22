Write a C++ function `fillHole(std::vector<Point2D> boundary, std::vector<Triangle>& outputTriangles)` that takes a closed polygonal boundary (a simple polygon, possibly non-convex but without self-intersections) represented as an ordered list of 2D points and returns a triangulation of the polygon's interior using the ear-clipping algorithm. The function must handle polygons with collinear consecutive points (skip them), polygons with all points collinear (return empty), and polygons with duplicate consecutive points (skip duplicates). The output should be a list of triangles, where each triangle is represented by three indices into the original boundary points (after deduplication/removal of collinear points). If the polygon has fewer than 3 valid vertices after cleaning, return an empty list. The triangulation must be valid: triangles must not overlap, must cover the entire polygon interior, and their vertices must be from the cleaned boundary set. For a convex polygon, the algorithm degenerates to a fan triangulation. Use the standard point-in-triangle test and orientation test (cross product sign) with an epsilon tolerance of 1e-9.

// The ear-clipping algorithm works by repeatedly finding an "ear" — a vertex where the triangle formed by it and its two neighbors lies entirely inside the polygon and contains no other polygon vertices. The algorithm maintains a doubly linked list of remaining vertices. First, clean the input: remove consecutive duplicates (if two consecutive points are equal, keep one) and remove collinear points (if the cross product of (p[i+1]-p[i]) and (p[i+2]-p[i+1]) is zero, remove p[i+1], and repeat until no collinear triples remain). Then, if fewer than 3 points remain, return empty. For each vertex in the current list, test whether it forms an ear: the triangle (prev, current, next) must have the same orientation as the polygon (positive for CCW, negative for CW — we can normalize the polygon to CCW by computing signed area; if area < 0, reverse the order). Then, check that no other vertex in the polygon lies inside or on the triangle. Use a point-in-triangle test with a small epsilon to avoid floating-point issues. If an ear is found, output the triangle (indices of prev, current, next in the original cleaned list), remove the current vertex from the list, and continue. The process finishes when only 3 vertices remain; output that final triangle. Edge cases: all points collinear (return empty), self-intersecting polygons (not handled by this algorithm, but we assume valid input), and degenerate triangles (area zero → skip if orientation is zero). Time complexity is O(n^2) in the worst case because for each ear candidate we scan all remaining vertices to check if any lies inside the triangle — that's O(n) per ear, and there are O(n) ears. Space complexity is O(n) for the linked list and output.

#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

struct Point2D {
    double x, y;
};

struct Triangle {
    int a, b, c; // indices into the cleaned vertex list
};

// Helper: cross product (p2-p1) x (p3-p1)
static double cross(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

// Helper: check if a point is inside or on triangle (a,b,c) given a sign (positive for CCW)
static bool pointInTriangle(const Point2D& p, const Point2D& a, const Point2D& b, const Point2D& c, double sign) {
    const double eps = 1e-9;
    double d1 = cross(a, b, p) * sign;
    double d2 = cross(b, c, p) * sign;
    double d3 = cross(c, a, p) * sign;
    return (d1 >= -eps) && (d2 >= -eps) && (d3 >= -eps);
}

// Main function: triangulate a simple polygon (possibly non-convex) using ear clipping.
std::vector<Triangle> fillHole(const std::vector<Point2D>& boundary) {
    std::vector<Triangle> result;
    if (boundary.size() < 3) return result;

    // 1. Clean input: remove consecutive duplicates and collinear points.
    std::vector<Point2D> pts;
    pts.reserve(boundary.size());
    for (const auto& p : boundary) {
        if (pts.empty() || std::abs(pts.back().x - p.x) > 1e-9 || std::abs(pts.back().y - p.y) > 1e-9) {
            pts.push_back(p);
        }
    }
    // Remove possible duplicate of last point wrapping around
    if (pts.size() > 1 && std::abs(pts.front().x - pts.back().x) < 1e-9 && std::abs(pts.front().y - pts.back().y) < 1e-9) {
        pts.pop_back();
    }
    if (pts.size() < 3) return result;

    // Remove collinear consecutive points (iterate until stable)
    bool changed = true;
    const double eps = 1e-9;
    while (changed && pts.size() >= 3) {
        changed = false;
        std::vector<Point2D> newPts;
        newPts.reserve(pts.size());
        size_t n = pts.size();
        for (size_t i = 0; i < n; ++i) {
            Point2D p = pts[i];
            Point2D prev = pts[(i + n - 1) % n];
            Point2D next = pts[(i + 1) % n];
            double cr = cross(prev, p, next);
            if (std::abs(cr) < eps) {
                // p is collinear -> skip it
                changed = true;
                continue;
            }
            newPts.push_back(p);
        }
        pts = std::move(newPts);
    }
    if (pts.size() < 3) return result;

    // 2. Determine polygon orientation (ensure CCW)
    double area = 0.0;
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i) {
        const Point2D& p1 = pts[i];
        const Point2D& p2 = pts[(i + 1) % n];
        area += p1.x * p2.y - p2.x * p1.y;
    }
    double sign = (area > 0) ? 1.0 : -1.0; // +1 for CCW, -1 for CW

    // 3. Build a doubly linked list of remaining vertex indices (into pts)
    std::vector<int> next(n);
    std::vector<int> prev(n);
    for (size_t i = 0; i < n; ++i) {
        next[i] = (int)((i + 1) % n);
        prev[i] = (int)((i + n - 1) % n);
    }

    int remaining = (int)n;
    int current = 0; // start scanning from vertex 0

    while (remaining > 3) {
        bool earFound = false;
        // Try up to 'remaining' vertices to find an ear
        for (int attempt = 0; attempt < remaining; ++attempt) {
            int i = current;
            int ip = prev[i];
            int in = next[i];
            const Point2D& a = pts[ip];
            const Point2D& b = pts[i];
            const Point2D& c = pts[in];

            // Check if angle at b is convex (same sign as polygon)
            double cr = cross(a, b, c);
            if (cr * sign <= eps) {
                // Reflex or collinear -> not an ear
                current = in;
                continue;
            }

            // Check if any other vertex lies inside triangle (a,b,c)
            bool inside = false;
            int j = next[in];
            while (j != ip) {
                const Point2D& p = pts[j];
                if (pointInTriangle(p, a, b, c, sign)) {
                    inside = true;
                    break;
                }
                j = next[j];
            }
            if (!inside) {
                // Ear found
                result.push_back({ip, i, in});
                // Remove vertex i from the list
                next[ip] = in;
                prev[in] = ip;
                remaining--;
                current = ip; // continue from previous vertex (safe)
                earFound = true;
                break;
            }
            current = in;
        }
        if (!earFound) {
            // Should not happen for a simple polygon, but return partial result
            break;
        }
    }

    // Add the final triangle (the last three remaining vertices)
    if (remaining == 3) {
        int a = current;
        int b = next[a];
        int c = next[b];
        result.push_back({a, b, c});
    }

    return result;
}

#include <cassert>
#include <cmath>

// Helper to check if two double values are nearly equal
static bool near(double a, double b) {
    return std::abs(a - b) < 1e-9;
}

// Helper: compute area of a triangle given indices
static double triArea(const std::vector<Point2D>& pts, const Triangle& t) {
    const Point2D& a = pts[t.a];
    const Point2D& b = pts[t.b];
    const Point2D& c = pts[t.c];
    return std::abs((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2.0;
}

int main() {
    // Test 1: Simple square (CCW)
    std::vector<Point2D> square = {{0,0}, {1,0}, {1,1}, {0,1}};
    auto tris = fillHole(square);
    assert(tris.size() == 2);
    double totalArea = triArea(square, tris[0]) + triArea(square, tris[1]);
    assert(near(totalArea, 1.0));

    // Test 2: Triangle (already triangulated)
    std::vector<Point2D> tri = {{0,0}, {2,0}, {1,2}};
    tris = fillHole(tri);
    assert(tris.size() == 1);
    assert(near(triArea(tri, tris[0]), 2.0));

    // Test 3: Pentagon (convex, 5 vertices -> 3 triangles)
    std::vector<Point2D> pent = {{0,0}, {2,0}, {3,1}, {1,3}, {-1,1}};
    tris = fillHole(pent);
    assert(tris.size() == 3);
    double pentaArea = 0.0;
    for (const auto& t : tris) pentaArea += triArea(pent, t);
    // Expected area computed by shoelace: (1/2)*|sum(x_i*y_{i+1} - x_{i+1}*y_i)|
    double shoelace = 0.0;
    for (size_t i = 0; i < pent.size(); ++i) {
        const auto& p1 = pent[i];
        const auto& p2 = pent[(i+1)%pent.size()];
        shoelace += p1.x * p2.y - p2.x * p1.y;
    }
    assert(near(pentaArea, std::abs(shoelace)/2.0));

    // Test 4: Concave polygon (arrow shape) — expect correct area
    std::vector<Point2D> arrow = {{0,0}, {4,0}, {4,1}, {2,3}, {4,5}, {4,6}, {0,6}, {0,0}};
    tris = fillHole(arrow);
    assert(tris.size() == 6); // n-2 = 6 triangles
    double arrowArea = 0.0;
    for (const auto& t : tris) arrowArea += triArea(arrow, t);
    // Area by shoelace: 4*6 - (2*2) = 20 (approximately, compute exactly)
    double expected = 0.0;
    for (size_t i = 0; i < arrow.size(); ++i) {
        const auto& p1 = arrow[i];
        const auto& p2 = arrow[(i+1)%arrow.size()];
        expected += p1.x * p2.y - p2.x * p1.y;
    }
    assert(near(arrowArea, std::abs(expected)/2.0));

    // Test 5: Collinear points (square with an extra collinear point on one edge)
    std::vector<Point2D> squareCollinear = {{0,0}, {2,0}, {1,0}, {1,1}, {0,1}}; // note (2,0) -> (1,0) collinear
    tris = fillHole(squareCollinear);
    assert(tris.size() == 2); // still a square after removing collinear
    double area2 = 0.0;
    for (const auto& t : tris) area2 += triArea(squareCollinear, t);
    assert(near(area2, 2.0)); // actual area = 2 (since width 2, height 1)

    // Test 6: Duplicate consecutive points
    std::vector<Point2D> dup = {{0,0}, {1,0}, {1,0}, {1,1}, {0,1}};
    tris = fillHole(dup);
    assert(tris.size() == 2);

    // Test 7: All collinear -> empty
    std::vector<Point2D> line = {{0,0}, {1,1}, {2,2}, {3,3}};
    tris = fillHole(line);
    assert(tris.empty());

    // Test 8: Fewer than 3 vertices -> empty
    std::vector<Point2D> tiny = {{0,0}, {1,1}};
    tris = fillHole(tiny);
    assert(tris.empty());

    return 0;
}

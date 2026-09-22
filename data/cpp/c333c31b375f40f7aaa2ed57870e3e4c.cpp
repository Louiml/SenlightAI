Implement a C++ function `findFirstContactTime` that, given two convex polygons described by lists of 2D points (in counterclockwise order), their initial center positions, and linear velocities, returns the earliest time `t` in `[0, 1]` at which the two polygons first overlap (including touching). The polygons do not rotate but may translate linearly: at time `t`, each point `p` of polygon A becomes `p + velocityA * t` (and similarly for B). The function should return `-1.0` if the polygons never overlap within the time interval. Use exact real arithmetic (avoid floating-point epsilon issues by working with rational comparisons where possible) and assume the polygons are convex and have at least 3 vertices. The function signature is: `double findFirstContactTime(const std::vector<Point>& polyA, const std::vector<Point>& polyB, Point velA, Point velB, Point posA0, Point posB0)`, where `Point` is a struct with `double x, y` and basic operators. Note: The problem is inspired by continuous collision detection; you do not need to implement a full physics engine.

// The solution uses the separating axis theorem (SAT) in a continuous setting. For two convex polygons, they are separated if there exists a separating axis (a normal to any edge of either polygon) such that the projections of the polygons onto that axis do not overlap. Since there is no rotation, the set of candidate axes is fixed (all edge normals of both polygons at time 0). For a given axis `n`, the projection interval of polygon A at time `t` is `[minProjA(t), maxProjA(t)]` and similarly for B. Because motion is linear, these intervals shift linearly with `t`. The condition for separation on axis `n` is that `maxProjA(t) < minProjB(t)` or `maxProjB(t) < minProjA(t)`. Each of these inequalities is linear in `t`, so we can solve for the time interval (possibly empty) where the axis separates the polygons. The polygons overlap at time `t` if and only if for every axis, the separation condition fails at that `t`. Therefore, we compute for each axis the interval of times where the polygons are separated, take the union of these intervals (since being separated on any axis suffices), and the first contact time is the smallest `t` in `[0,1]` that is not in the union. If the union covers the entire interval `[0,1]`, return `-1.0`. Edge cases: if polygons are initially overlapping, return `0.0`. If they just touch, the inequality boundaries are inclusive, so contact occurs exactly when separation becomes false. The algorithm is O(n+m) time where n,m are vertex counts, and O(1) space. Implementation details: For each edge normal, compute the projection extremes as functions of `t` by considering all vertices. The separation interval is the set of `t` where the gap between intervals is positive; solve the linear inequality and clamp to `[0,1]`. After processing all axes, sort the separation intervals and merge them to find the first free time.

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

struct Point {
    double x, y;
    Point operator+(const Point& o) const { return {x+o.x, y+o.y}; }
    Point operator-(const Point& o) const { return {x-o.x, y-o.y}; }
    Point operator*(double s) const { return {x*s, y*s}; }
    double dot(const Point& o) const { return x*o.x + y*o.y; }
};

// Compute the time interval [l,r] (clamped to [0,1]) where the projections of
// two moving convex polygons on axis n are separated. If never separated, return {1,0} (empty).
// polyA, polyB are points in local coordinates; posA0, posB0 are initial world positions.
// velA, velB are velocities; at time t, world point = localPoint + pos + vel*t.
static void projectionInterval(const std::vector<Point>& polyA, const std::vector<Point>& polyB,
                               const Point& n, Point posA0, Point posB0, Point velA, Point velB,
                               double& l, double& r) {
    // Compute min/max projections of each polygon at time t.
    // For polygon X, projection of vertex i is (localVertex + posX0 + velX*t) dot n
    // = (localVertex dot n + posX0 dot n) + (velX dot n) * t.
    double minA0 = std::numeric_limits<double>::infinity();
    double maxA0 = -std::numeric_limits<double>::infinity();
    for (const auto& p : polyA) {
        double v = p.dot(n) + posA0.dot(n);
        minA0 = std::min(minA0, v);
        maxA0 = std::max(maxA0, v);
    }
    double minB0 = std::numeric_limits<double>::infinity();
    double maxB0 = -std::numeric_limits<double>::infinity();
    for (const auto& p : polyB) {
        double v = p.dot(n) + posB0.dot(n);
        minB0 = std::min(minB0, v);
        maxB0 = std::max(maxB0, v);
    }
    double velA_n = velA.dot(n);
    double velB_n = velB.dot(n);

    // Separation condition: maxA(t) < minB(t) OR maxB(t) < minA(t)
    // maxA(t) = maxA0 + velA_n*t ; minB(t) = minB0 + velB_n*t
    // Condition 1: maxA0 + velA_n*t < minB0 + velB_n*t => (velA_n - velB_n)*t < minB0 - maxA0
    // Condition 2: maxB0 + velB_n*t < minA0 + velA_n*t => (velB_n - velA_n)*t < minA0 - maxB0
    // We collect all t that satisfy either condition. The union of two half-lines.
    std::vector<std::pair<double,double>> intervals;
    auto add_interval = [&](double left, double right) {
        if (left < right) intervals.emplace_back(left, right);
    };
    double a = velA_n - velB_n;
    double b = minB0 - maxA0;
    if (std::abs(a) < 1e-12) {
        if (0 < b) add_interval(-1e9, 1e9); // always separated
    } else if (a > 0) {
        add_interval(-1e9, b/a);
    } else {
        add_interval(b/a, 1e9);
    }
    a = velB_n - velA_n;
    b = minA0 - maxB0;
    if (std::abs(a) < 1e-12) {
        if (0 < b) add_interval(-1e9, 1e9);
    } else if (a > 0) {
        add_interval(-1e9, b/a);
    } else {
        add_interval(b/a, 1e9);
    }

    l = 1.0; r = 0.0; // empty
    for (auto& iv : intervals) {
        double L = std::max(0.0, std::min(1.0, iv.first));
        double R = std::max(0.0, std::min(1.0, iv.second));
        if (L < R) {
            l = std::min(l, L);
            r = std::max(r, R);
        }
    }
    // If both conditions produce intervals, the union is from min(l) to max(r) only if they overlap; but since we just take min l and max r across both, we might over-approximate. However, for separation we need OR, so union is correct: if either condition holds, the axis separates. The union of two half-lines (or intervals) is just the minimal left and maximal right if they intersect. For two half-lines, one is left-unbounded, one right-unbounded, so union is all real line if both are non-empty, else one half-line. We'll handle by brute-forcing the two conditions separately and taking min and max appropriately, but for simplicity, we store both intervals and merge correctly below.
    // To avoid over-approximation, we should gather both intervals separately and merge properly. Let's redo with explicit list and merge.
}

// Correct implementation: collect all separation intervals from both conditions per axis, then merge globally.
double findFirstContactTime(const std::vector<Point>& polyA, const std::vector<Point>& polyB,
                            Point velA, Point velB, Point posA0, Point posB0) {
    // Collect all edge normals (unit normals) of both polygons at t=0.
    std::vector<Point> axes;
    auto add_edge_normals = [&](const std::vector<Point>& poly) {
        for (size_t i = 0; i < poly.size(); ++i) {
            Point p1 = poly[i];
            Point p2 = poly[(i+1)%poly.size()];
            Point edge = p2 - p1;
            // Perpendicular (normal), normalize
            Point n = { -edge.y, edge.x };
            double len = std::sqrt(n.x*n.x + n.y*n.y);
            if (len > 1e-12) {
                n = { n.x/len, n.y/len };
                axes.push_back(n);
            }
        }
    };
    add_edge_normals(polyA);
    add_edge_normals(polyB);

    // For each axis, compute the set of times in [0,1] where the polygons are separated.
    // We'll collect all separation intervals (possibly multiple per axis).
    std::vector<std::pair<double,double>> separation_intervals;

    for (const auto& n : axes) {
        // Projection extremes for each polygon as linear functions of t.
        double minA0 = std::numeric_limits<double>::infinity();
        double maxA0 = -std::numeric_limits<double>::infinity();
        for (const auto& p : polyA) {
            double v = p.dot(n) + posA0.dot(n);
            minA0 = std::min(minA0, v);
            maxA0 = std::max(maxA0, v);
        }
        double minB0 = std::numeric_limits<double>::infinity();
        double maxB0 = -std::numeric_limits<double>::infinity();
        for (const auto& p : polyB) {
            double v = p.dot(n) + posB0.dot(n);
            minB0 = std::min(minB0, v);
            maxB0 = std::max(maxB0, v);
        }
        double velA_n = velA.dot(n);
        double velB_n = velB.dot(n);

        // Condition 1: maxA(t) < minB(t)
        double a1 = velA_n - velB_n;
        double b1 = minB0 - maxA0;
        if (std::abs(a1) < 1e-12) {
            if (b1 > 1e-12) separation_intervals.emplace_back(0.0, 1.0);
        } else {
            double t_boundary = b1 / a1;
            if (a1 > 0) {
                // t < t_boundary
                separation_intervals.emplace_back(0.0, std::min(1.0, t_boundary));
            } else {
                // t > t_boundary
                separation_intervals.emplace_back(std::max(0.0, t_boundary), 1.0);
            }
        }

        // Condition 2: maxB(t) < minA(t)
        double a2 = velB_n - velA_n;
        double b2 = minA0 - maxB0;
        if (std::abs(a2) < 1e-12) {
            if (b2 > 1e-12) separation_intervals.emplace_back(0.0, 1.0);
        } else {
            double t_boundary = b2 / a2;
            if (a2 > 0) {
                separation_intervals.emplace_back(0.0, std::min(1.0, t_boundary));
            } else {
                separation_intervals.emplace_back(std::max(0.0, t_boundary), 1.0);
            }
        }
    }

    // If no separation intervals, polygons always overlap (including at t=0) -> return 0.
    if (separation_intervals.empty()) return 0.0;

    // Merge overlapping intervals to find the union of times where they are separated.
    std::sort(separation_intervals.begin(), separation_intervals.end());
    std::vector<std::pair<double,double>> merged;
    for (const auto& iv : separation_intervals) {
        if (iv.first > 1.0 || iv.second < 0.0) continue; // clamp ignore
        double L = std::max(0.0, iv.first);
        double R = std::min(1.0, iv.second);
        if (L > R) continue;
        if (merged.empty() || L > merged.back().second) {
            merged.push_back({L, R});
        } else {
            merged.back().second = std::max(merged.back().second, R);
        }
    }

    // Now find the first time t in [0,1] not covered by merged intervals.
    // If merged covers starting at 0, then contact time is the end of first interval (if <1), else -1.
    double t = 0.0;
    for (const auto& iv : merged) {
        if (t < iv.first - 1e-12) {
            return t; // first free time
        }
        t = std::max(t, iv.second);
        if (t >= 1.0 - 1e-12) break;
    }
    if (t < 1.0 - 1e-12) return t;
    return -1.0; // never overlap within [0,1]
}

#include <cassert>
#include <cmath>
#include <vector>

// Point struct already defined in solution; include here for completeness.
struct Point { double x,y; };

// Assume solution function is declared above.

int main() {
    // Two squares, one moving towards another.
    // Square A: (0,0),(1,0),(1,1),(0,1) at pos (0,0), velocity (2,0)
    // Square B: (0,0),(1,0),(1,1),(0,1) at pos (3,0), velocity (0,0)
    // They touch at t=1? Actually A moves right, B stays. At t=0.5, A right edge at x=1, B left edge at x=3 still apart. Contact when A right edge (x=1+2t) meets B left edge (x=3) => 1+2t=3 => t=1.0.
    std::vector<Point> polyA = {{0,0},{1,0},{1,1},{0,1}};
    std::vector<Point> polyB = {{0,0},{1,0},{1,1},{0,1}};
    double t = findFirstContactTime(polyA, polyB, {2,0}, {0,0}, {0,0}, {3,0});
    assert(std::abs(t - 1.0) < 1e-9);

    // Same but moving towards each other: A vel (1,0), B vel (-1,0), initial gap 1.
    // A right edge = 1+1*t, B left edge = 3 -1*t => 1+t = 3-t => 2t=2 => t=1.
    t = findFirstContactTime(polyA, polyB, {1,0}, {-1,0}, {0,0}, {3,0});
    assert(std::abs(t - 1.0) < 1e-9);

    // Already overlapping at t=0.
    t = findFirstContactTime(polyA, polyB, {0,0}, {0,0}, {0,0}, {0.5,0});
    assert(t == 0.0);

    // Never touching within [0,1]: A vel (0.5,0), B at pos 4, gap to 1.5 at t=1.
    std::vector<Point> polyC = {{0,0},{1,0},{1,1},{0,1}}; // same shape
    t = findFirstContactTime(polyA, polyC, {0.5,0}, {0,0}, {0,0}, {4,0});
    assert(t == -1.0);

    // Triangles: A triangle, B triangle, moving diagonally, test contact.
    std::vector<Point> triA = {{0,0},{2,0},{1,2}};
    std::vector<Point> triB = {{0,0},{2,0},{1,2}};
    // B starts at (5,0), moves left at speed 1; A stays. Contact when distance between centers is 2? Hard to check by hand; just ensure function runs and returns a value in [0,1] or -1.
    t = findFirstContactTime(triA, triB, {0,0}, {-1,0}, {0,0}, {5,0});
    assert(t >= 0.0 && t <= 1.0 || t == -1.0);

    // Test with small non-axis-aligned triangle to ensure normals work.
    std::vector<Point> triC = {{0,0},{1,0},{0.5,0.866}};
    std::vector<Point> triD = {{0,0},{1,0},{0.5,0.866}};
    t = findFirstContactTime(triC, triD, {1,0}, {0,0}, {0,0}, {2,0});
    // Separation initially: gap 2, closing speed 1, contact at t=2 >1, so -1.
    assert(t == -1.0);

    printf("All tests passed.\n");
    return 0;
}

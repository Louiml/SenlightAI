// Given \(n\) line segments in a 2D plane, each defined by its two endpoints \((x_1,y_1)\) and \((x_2,y_2)\), write a C++ function `int countIntersectionPairs(const std::vector<std::array<int,4>>& segments)` that returns the total number of ordered pairs \((i,j)\) with \(i \neq j\) such that segment \(i\) and segment \(j\) intersect (including touching at endpoints or overlapping collinearly). The segments are axis-aligned or arbitrary, but all coordinates are integers. The function must handle up to \(n \leq 100\) segments, and use double precision with an epsilon of \(10^{-9}\) for geometric comparisons. For each segment, the endpoints may be given in any order; the function should not assume any ordering. The result is the sum over each segment \(i\) of the count of segments \(j\) (excluding \(i\) itself) that intersect segment \(i\). Note that if two segments intersect, the pair contributes to both counts (once for each direction), so the final sum counts each intersection twice. If two segments are identical (same endpoints, possibly reversed), they are considered to intersect. The function must be self-contained and not rely on global state.
#include <cassert>
#include <vector>
#include <array>

// Include the solution code here (function definition above)

int main() {
    // Single segment: no pairs
    assert(countIntersectionPairs({{0,0,1,1}}) == 0);
    
    // Two crossing segments
    assert(countIntersectionPairs({{0,0,2,2}, {0,2,2,0}}) == 2);
    
    // Two parallel non-intersecting
    assert(countIntersectionPairs({{0,0,1,0}, {0,1,1,1}}) == 0);
    
    // Shared endpoint
    assert(countIntersectionPairs({{0,0,1,1}, {1,1,2,0}}) == 2);
    
    // Identical segments (reversed order)
    assert(countIntersectionPairs({{0,0,2,2}, {2,2,0,0}}) == 2);
    
    // Vertical and horizontal crossing
    assert(countIntersectionPairs({{1,0,1,2}, {0,1,2,1}}) == 2);
    
    // Vertical and non-crossing horizontal
    assert(countIntersectionPairs({{1,0,1,2}, {3,1,4,1}}) == 0);
    
    // Three segments: two intersecting a third, but not each other
    std::vector<std::array<int,4>> segs = {{0,0,2,0}, {1,1,1,-1}, {3,0,3,2}};
    // seg0 intersects seg1 at (1,0); seg1 intersects seg0; seg2 doesn't intersect either
    assert(countIntersectionPairs(segs) == 2);
    
    // Collinear overlapping segments
    assert(countIntersectionPairs({{0,0,3,0}, {1,0,2,0}}) == 2);
    
    // Collinear but non-overlapping
    assert(countIntersectionPairs({{0,0,1,0}, {2,0,3,0}}) == 0);
    
    return 0;
}
#include <vector>
#include <array>
#include <cmath>

const double EPS = 1e-9;

struct LineSegment {
    double a, b, c;
    double x1, y1, x2, y2;
    
    LineSegment(int x1, int y1, int x2, int y2) {
        if (x1 == x2) {
            a = 1.0;
            b = 0.0;
            c = -static_cast<double>(x1);
        } else {
            a = -static_cast<double>(y1 - y2) / (x1 - x2);
            b = 1.0;
            c = -(a * x1) - y1;
        }
        // normalize bounding box
        this->x1 = std::min(x1, x2);
        this->x2 = std::max(x1, x2);
        this->y1 = std::min(y1, y2);
        this->y2 = std::max(y1, y2);
    }
    
    bool isParallel(const LineSegment& other) const {
        return std::fabs(a - other.a) < EPS && std::fabs(b - other.b) < EPS;
    }
    
    bool isSameLine(const LineSegment& other) const {
        return isParallel(other) && std::fabs(c - other.c) < EPS;
    }
    
    bool containsPoint(double px, double py) const {
        return (px >= x1 - EPS && px <= x2 + EPS &&
                py >= y1 - EPS && py <= y2 + EPS);
    }
};

bool doIntersect(const LineSegment& l1, const LineSegment& l2) {
    if (l1.isSameLine(l2)) {
        return true; // collinear overlapping or touching
    }
    if (l1.isParallel(l2)) {
        return false; // parallel, not same line
    }
    // Solve for intersection point
    double denom = l2.a * l1.b - l1.a * l2.b;
    if (std::fabs(denom) < EPS) {
        return false; // Shouldn't happen if not parallel, but guard
    }
    double x = (l2.b * l1.c - l1.b * l2.c) / denom;
    double y;
    if (std::fabs(l1.b) > EPS) {
        y = -(l1.a * x + l1.c);
    } else {
        y = -(l2.a * x + l2.c);
    }
    return l1.containsPoint(x, y) && l2.containsPoint(x, y);
}

// Returns total sum over ordered pairs (i,j) where segments intersect.
int countIntersectionPairs(const std::vector<std::array<int,4>>& segments) {
    int n = static_cast<int>(segments.size());
    std::vector<LineSegment> lines;
    lines.reserve(n);
    for (const auto& seg : segments) {
        lines.emplace_back(seg[0], seg[1], seg[2], seg[3]);
    }
    int total = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && doIntersect(lines[i], lines[j])) {
                ++total;
            }
        }
    }
    return total;
}
// The solution models each line segment using the general line equation \(a x + b y + c = 0\). For vertical segments (\(x_1 = x_2\)), set \(a=1, b=0, c=-x_1\); otherwise, set \(b=1, a=-(y_1-y_2)/(x_1-x_2), c=-a x_1 - y_1\). Two segments are parallel if their \((a,b)\) pairs are close within EPS; they are the same line if also \(c\) matches. If not parallel, compute the intersection point \((x,y)\) by solving the two linear equations. Then check that this point lies within the bounding box of both segments (allowing EPS tolerance). For parallel lines, they intersect only if they are the same line; if they are the same line, they intersect (since any point on one segment lies on the other, and they overlap or touch; the problem expects this to count as an intersection). The algorithm iterates over all ordered pairs \(i \neq j\), checks intersection, and increments a counter. Time complexity is \(O(n^2)\), with \(n \leq 100\) making it efficient; space complexity is \(O(n)\) for storing line objects.

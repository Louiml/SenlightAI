/*
Write a C++ function that determines whether a finite line segment (defined by its midpoint, direction vector, and half-length) intersects an axis-aligned box (defined by its center and half-extents in x, y, and z). The function must return `true` if any point of the segment lies inside or on the surface of the box, and `false` otherwise. Use only fundamental vector operations from `<cmath>` and custom 3D vector and box structs (no external libraries). The segment's direction vector is not required to be normalized, and the box can have zero extents (a degenerate box that is a point, line, or rectangle). The function should be robust to floating-point rounding and work for both positive and negative coordinate values. Provide a standalone free function named `segmentIntersectsBox` that takes the segment and box parameters by const reference and returns a boolean.
*/
#include <cmath>
#include <algorithm>

// Minimal 3D vector structure for the task.
struct Vec3 {
    double x, y, z;
    
    Vec3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
    
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(double scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
};

inline double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline double absValue(double v) { return std::fabs(v); }

// Axis-aligned box defined by center and half-extents.
struct Box {
    Vec3 center;
    Vec3 extent; // positive half-lengths along x, y, z
};

// Segment defined by midpoint, direction (not necessarily normalized), and half-length.
struct Segment {
    Vec3 midpoint;
    Vec3 direction;
    double halfLength; // must be >= 0
};

// Returns true if the segment intersects (or touches) the box.
bool segmentIntersectsBox(const Segment& seg, const Box& box) {
    const Vec3 diff = seg.midpoint - box.center;
    
    // Background: use Separating Axis Theorem.
    // 1) Test box axes (x, y, z)
    {
        const double absDirX = absValue(seg.direction.x);
        double absDiffX = absValue(diff.x);
        if (absDiffX > box.extent.x + seg.halfLength * absDirX)
            return false;
        
        const double absDirY = absValue(seg.direction.y);
        double absDiffY = absValue(diff.y);
        if (absDiffY > box.extent.y + seg.halfLength * absDirY)
            return false;
        
        const double absDirZ = absValue(seg.direction.z);
        double absDiffZ = absValue(diff.z);
        if (absDiffZ > box.extent.z + seg.halfLength * absDirZ)
            return false;
        
        // 2) Test cross product axes: direction × box axis
        const Vec3 w0 = cross(seg.direction, Vec3(1,0,0));
        double absCross0 = absValue(dot(w0, diff));
        double rhs0 = box.extent.y * absDirZ + box.extent.z * absDirY;
        if (absCross0 > rhs0)
            return false;
        
        const Vec3 w1 = cross(seg.direction, Vec3(0,1,0));
        double absCross1 = absValue(dot(w1, diff));
        double rhs1 = box.extent.x * absDirZ + box.extent.z * absDirX;
        if (absCross1 > rhs1)
            return false;
        
        const Vec3 w2 = cross(seg.direction, Vec3(0,0,1));
        double absCross2 = absValue(dot(w2, diff));
        double rhs2 = box.extent.x * absDirY + box.extent.y * absDirX;
        if (absCross2 > rhs2)
            return false;
    }
    
    // No separating axis found → intersection exists.
    return true;
}
#include <cassert>
#include <cmath>

// (Forward declarations of Vec3, dot, cross, absValue, Box, Segment, segmentIntersectsBox
//  would appear here if not already included from the solution context.)

int main() {
    // Helper to create a segment from two endpoints and a box from center+extents.
    auto makeSegment = [](Vec3 a, Vec3 b) {
        Vec3 dir = b - a;
        return Segment{ a, dir, std::sqrt(dot(dir, dir)) / 2.0 };
    };
    auto makeBox = [](Vec3 c, Vec3 e) {
        return Box{ c, e };
    };

    // 1. Segment entirely inside the box.
    {
        Segment s = makeSegment(Vec3(0,0,0), Vec3(1,0,0));
        Box b = makeBox(Vec3(5,0,0), Vec3(10,10,10));
        assert(segmentIntersectsBox(s, b) == true);
    }

    // 2. Segment completely outside, separated along x.
    {
        Segment s = makeSegment(Vec3(-10,0,0), Vec3(-5,0,0));
        Box b = makeBox(Vec3(0,0,0), Vec3(2,2,2));
        assert(segmentIntersectsBox(s, b) == false);
    }

    // 3. Segment passes through the box (intersects).
    {
        Segment s = makeSegment(Vec3(-5,1,1), Vec3(5,1,1));
        Box b = makeBox(Vec3(0,0,0), Vec3(2,2,2));
        assert(segmentIntersectsBox(s, b) == true);
    }

    // 4. Segment just touches the box face (tangent).
    {
        Segment s = makeSegment(Vec3(2,0,0), Vec3(4,0,0));
        Box b = makeBox(Vec3(0,0,0), Vec3(2,2,2));
        assert(segmentIntersectsBox(s, b) == true); // touches at x=2 point.
    }

    // 5. Degenerate segment (point) inside.
    {
        Segment s = makeSegment(Vec3(1,1,1), Vec3(1,1,1));
        Box b = makeBox(Vec3(0,0,0), Vec3(3,3,3));
        assert(segmentIntersectsBox(s, b) == true);
    }

    // 6. Degenerate segment (point) outside.
    {
        Segment s = makeSegment(Vec3(10,10,10), Vec3(10,10,10));
        Box b = makeBox(Vec3(0,0,0), Vec3(1,1,1));
        assert(segmentIntersectsBox(s, b) == false);
    }

    // 7. Degenerate box (zero extents) and segment passing via center.
    {
        Segment s = makeSegment(Vec3(-2,0,0), Vec3(2,0,0));
        Box b = makeBox(Vec3(0,0,0), Vec3(0,0,0));
        assert(segmentIntersectsBox(s, b) == true); // passes through the point (0,0,0).
    }

    // 8. Degenerate box (a line along y, zero x and z) and segment parallel but offset.
    {
        Segment s = makeSegment(Vec3(-1,0,0), Vec3(2,0,0)); // segment along x, z=0, y=0
        Box b = makeBox(Vec3(0,1,0), Vec3(0,1,0));          // box is a point at (0,1,0)
        assert(segmentIntersectsBox(s, b) == false);        // no intersection (y differs).
    }

    // 9. Non‑normalized direction vector with large magnitude.
    {
        Segment s = makeSegment(Vec3(0,0,0), Vec3(100,100,100));
        Box b = makeBox(Vec3(50,50,50), Vec3(10,10,10));
        assert(segmentIntersectsBox(s, b) == true); // passes through the box.
    }

    // 10. Negative coordinates and offset center.
    {
        Segment s = makeSegment(Vec3(-10,-2,-2), Vec3(-1,-2,-2));
        Box b = makeBox(Vec3(-5,0,0), Vec3(3,3,3));
        assert(segmentIntersectsBox(s, b) == false); // y-offset from box center.
    }

    return 0;
}
// The solution uses the Separating Axis Theorem (SAT), but specialized for the case of a segment vs. an axis-aligned box. The key idea is that two convex shapes are disjoint if and only if there exists a separating axis (a line such that the projections of the shapes onto that axis do not overlap). For a segment and a box, the candidate separating axes are:
// 1. The three box axes (x, y, z) — these check if the segment is entirely to one side of one of the box faces.
// 2. The cross product of the segment direction with each box axis — these check for edge‑edge separations, where the segment might pass between two adjacent edges of the box.
//
// The algorithm works as follows:
// - Compute the vector from the box center to the segment midpoint: `diff = segment.center - box.center`.
// - For each box axis `u` (x, y, z):
//   - Compute `absSegmentProjection = |segment.direction · u|` (how much the segment extends along that axis).
//   - Compute `absDiffProjection = |diff · u|` (distance between centers along that axis).
//   - The projected interval of the segment onto that axis is `[diff·u - extent·absSegmentProjection, diff·u + extent·absSegmentProjection]`. The box interval is `[-boxExtent[u], boxExtent[u]]`. If `absDiffProjection > boxExtent[u] + segmentExtent·absSegmentProjection`, then the intervals are disjoint → return `false`.
// - For each pair of the three axes, take the cross product of the segment direction with the box axis (three cross products). For each such axis `w = direction × u`:
//   - Compute `absCrossProjection = | (direction × diff) · u |` (this is the projection of the segment's "cross vector" onto that axis). 
//   - The box's projection radius onto `w` is computed as a combination of its extents along the other two axes: for example, for `w = direction × u[0]`, the radius is `boxExtent[1]·|direction·u[2]| + boxExtent[2]·|direction·u[1]|`.
//   - If `absCrossProjection > that radius`, then the projections are disjoint → return `false`.
//
// If none of the six tests find a separation, the segment intersects the box (or is tangent). Edge cases include a degenerate segment (extent = 0, which is a point) — the cross product tests still work because the direction vector may be zero; in that case, cross products are zero, and the point‑in‑box test is handled by the axis tests. A degenerate box with zero extents along one or more axes also works, as the radii formulas degrade gracefully. Time complexity is O(1) (constant number of dot/cross products), and space complexity is O(1).

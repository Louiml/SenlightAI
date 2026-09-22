// Write a C++ function that, given a 3D segment defined by two endpoints and a plane (defined by a normal vector and a constant), returns the portion of the segment that lies inside the halfspace defined by `dot(normal, point) <= constant`. The function should output the clipped segment as a list of points (0, 1, or 2 points) representing the intersection of the segment with the halfspace. If the entire segment is inside the halfspace, return both endpoints; if partially inside, return one or two points at the boundary and/or inside; if completely outside, return an empty vector. The function must be robust to floating-point precision by using a small epsilon tolerance when checking point-plane relationships. The input segment endpoints are `Vector3` objects with `x`, `y`, `z` fields, and the plane is given by a `Vector3` normal and a `Real` constant.
// The core approach is to clip the segment against the plane that bounds the halfspace. Since the halfspace is defined as `dot(normal, p) <= constant`, the boundary is the plane `dot(normal, p) == constant`. We evaluate the signed distance `dot(normal, p) - constant` for both endpoints. If both distances are less than or equal to zero (within epsilon), the entire segment is inside, return both endpoints. If both are greater than epsilon, the segment is completely outside, return empty. If one endpoint is inside and the other outside, we compute the intersection point with the plane using linear interpolation parameter `t = dist0 / (dist0 - dist1)` where `dist0` and `dist1` are the signed distances of the two endpoints; the intersection point is `P0 + t*(P1-P0)`. If both are outside but the segment crosses the plane (which cannot happen for a convex halfspace since both endpoints on the same side means the entire segment is on that side), we still handle it generically by clipping: in the general case you might clip a convex polygon, but for a segment, the logic simplifies to: if both distances <= eps -> both endpoints; if one <= eps and the other > eps -> inside endpoint plus the intersection point; otherwise empty. However, due to numerical issues, it's safer to consider all four cases: both inside, both outside, P0 inside/P1 outside, P0 outside/P1 inside. We must also handle the degenerate case where the segment is parallel to the plane (both distances equal within eps); if both are inside, return both, else empty. Time complexity is O(1), space complexity O(1) (excluding output vector).
#include <vector>
#include <cmath>

struct Vector3 {
    double x, y, z;
    Vector3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
};

// Compute dot product of two vectors.
double dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Subtract two vectors.
Vector3 operator-(const Vector3& a, const Vector3& b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Add two vectors.
Vector3 operator+(const Vector3& a, const Vector3& b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Multiply vector by scalar.
Vector3 operator*(double s, const Vector3& v) {
    return Vector3(s * v.x, s * v.y, s * v.z);
}

// Clip a segment against the halfspace dot(normal, point) <= constant.
// Returns the portion of the segment inside the halfspace.
// The result is either 0, 1, or 2 points.
std::vector<Vector3> clipSegmentToHalfspace(
    const Vector3& p0, const Vector3& p1,
    const Vector3& normal, double constant, double eps = 1e-9)
{
    // Signed distances of endpoints from the boundary plane.
    double d0 = dot(normal, p0) - constant;
    double d1 = dot(normal, p1) - constant;

    bool inside0 = d0 <= eps;
    bool inside1 = d1 <= eps;

    if (inside0 && inside1) {
        // Entire segment is inside.
        return {p0, p1};
    }
    if (!inside0 && !inside1) {
        // Entire segment is outside (both on same side).
        return {};
    }

    // One endpoint inside, one outside. Compute intersection point.
    // t = d0 / (d0 - d1) since at intersection dot(normal, point) == constant.
    double t = d0 / (d0 - d1);
    Vector3 intersection = p0 + t * (p1 - p0);

    if (inside0 && !inside1) {
        // p0 inside, p1 outside -> keep p0 and intersection.
        return {p0, intersection};
    } else {
        // p1 inside, p0 outside -> keep intersection and p1.
        return {intersection, p1};
    }
}
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution code here (Vector3, dot, operators, clipSegmentToHalfspace).

// Helper to check if two vectors are approximately equal.
bool vecApproxEq(const Vector3& a, const Vector3& b, double tol = 1e-7) {
    return std::fabs(a.x - b.x) < tol &&
           std::fabs(a.y - b.y) < tol &&
           std::fabs(a.z - b.z) < tol;
}

// Helper to check if two point lists are approximately equal (order-insensitive when length 2? We'll allow exact order matching).
bool ptsApproxEq(const std::vector<Vector3>& a, const std::vector<Vector3>& b, double tol = 1e-7) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!vecApproxEq(a[i], b[i], tol)) return false;
    }
    return true;
}

int main() {
    // Test 1: Entire segment inside halfspace (normal (0,0,1), constant 5, plane z=5, inside z<=5).
    {
        Vector3 p0(0,0,0), p1(1,1,1);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0) && vecApproxEq(result[1], p1));
    }

    // Test 2: Entire segment outside (both z > 5).
    {
        Vector3 p0(0,0,6), p1(1,1,7);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.empty());
    }

    // Test 3: Partial intersection, p0 inside, p1 outside.
    // Segment from (0,0,0) to (0,0,10), plane z=5. Intersection at (0,0,5).
    {
        Vector3 p0(0,0,0), p1(0,0,10);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0));
        assert(vecApproxEq(result[1], Vector3(0,0,5)));
    }

    // Test 4: Partial intersection, p0 outside, p1 inside.
    {
        Vector3 p0(0,0,10), p1(0,0,0);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], Vector3(0,0,5)));
        assert(vecApproxEq(result[1], p1));
    }

    // Test 5: Segment exactly on the boundary (both z=5) -> inside (<=eps).
    {
        Vector3 p0(0,0,5), p1(0,0,5);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0) && vecApproxEq(result[1], p1));
    }

    // Test 6: Segment with one endpoint exactly on boundary, other outside -> intersection is that endpoint.
    {
        Vector3 p0(0,0,5), p1(0,0,10);
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0));
        assert(vecApproxEq(result[1], p0)); // Intersection equals p0
    }

    // Test 7: Non-axis-aligned normal (e.g., normal (1,1,0)/sqrt(2), constant 1, halfspace x+y <= sqrt(2)? Actually let's use normal (1,0,0), constant -1 => x <= -1.
    {
        Vector3 p0(-2,0,0), p1(-0.5,0,0);
        Vector3 normal(1,0,0); double constant = -1.0; // x <= -1
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0));
        assert(vecApproxEq(result[1], Vector3(-1,0,0)));
    }

    // Test 8: Degenerate segment (zero length) inside.
    {
        Vector3 p0(1,2,3), p1(1,2,3);
        Vector3 normal(0,0,1); double constant = 10.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0) && vecApproxEq(result[1], p1));
    }

    // Test 9: Degenerate segment (zero length) outside.
    {
        Vector3 p0(5,5,5), p1(5,5,5);
        Vector3 normal(0,0,1); double constant = 10.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.empty());
    }

    // Test 10: Segment crossing plane twice? Not possible for a single plane, but ensure correctness when both outside but on opposite sides (should not happen with convex halfspace test, but if it occurs we handle via generic clipping? Our code returns empty because both distances > eps -> outside). Let's test a case where one distance is -1 and other +1, but the halfspace is <= constant, so one inside one outside actually handled. So just confirm we don't crash.
    {
        Vector3 p0(0,0,4), p1(0,0,6); // cross plane z=5
        Vector3 normal(0,0,1); double constant = 5.0;
        auto result = clipSegmentToHalfspace(p0, p1, normal, constant);
        assert(result.size() == 2);
        assert(vecApproxEq(result[0], p0) && vecApproxEq(result[1], Vector3(0,0,5)));
    }

    return 0;
}

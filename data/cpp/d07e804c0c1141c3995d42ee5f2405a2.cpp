// Write a standalone C++ function `bool isPointInsideSlab(const std::array<double,3>& point, const std::array<double,3>& boxCenter, const std::array<double,3>& boxHalfExtents, const std::array<double,3>& boxOrientationAxis0, const std::array<double,3>& boxOrientationAxis1, const std::array<double,3>& boxOrientationAxis2)` that determines whether a 3D point lies inside an oriented bounding box (OBB). The box is defined by its center `boxCenter`, axis-aligned half-extents `boxHalfExtents` (representing the box's dimensions along its local axes before rotation), and three orthonormal orientation axes (`boxOrientationAxis0`, `boxOrientationAxis1`, `boxOrientationAxis2`) that form the box's local coordinate frame (each axis is a unit vector). The point is considered inside if, after transforming the point into the box's local coordinate system (by subtracting the center and projecting onto each local axis), the absolute coordinate along each axis is less than or equal to the corresponding half-extent component. Use tolerance `1e-9` to handle floating‑point precision. The function must not assume the axes are unit-length but you may normalize them internally. The input arrays are `std::array<double,3>`; return `true` if the point is inside (including on the boundary), otherwise `false`.

// The solution uses the standard OBB point‑containment test. First, compute the vector from the box center to the point: `d = point - center`. Then, for each of the three local axes `a_i` (i=0,1,2), compute the projection `p_i = dot(d, a_i)`. The point is inside if `|p_i| <= halfExtents[i] + tolerance` for all `i`. Important edge cases: (1) the point exactly on the boundary must return `true`, handled by the tolerance; (2) the axes may not be perfectly orthonormal in practice, but the projection approach still yields a correct result for any set of three linearly independent axes; to be safe we normalize each axis because the provided axes might have floating‑point error; (3) a zero-length axis is invalid, but we can guard by returning `false` if any axis norm is near zero (though the problem statement assumes valid axes). Time complexity is O(1) (constant number of operations, exactly 9 multiplies and 6 adds for dot products plus comparisons). Space complexity is O(1) auxiliary (only a few local variables).

#include <array>
#include <cmath>

// Determine if a point is inside an oriented bounding box defined by center,
// half-extents (in local axes), and three (possibly non-unit) orientation axes.
// Tolerance 1e-9 handles floating-point precision on the boundary.
bool isPointInsideSlab(const std::array<double,3>& point,
                       const std::array<double,3>& boxCenter,
                       const std::array<double,3>& boxHalfExtents,
                       const std::array<double,3>& boxOrientationAxis0,
                       const std::array<double,3>& boxOrientationAxis1,
                       const std::array<double,3>& boxOrientationAxis2) {
    const double epsilon = 1e-9;

    // Vector from box center to point
    std::array<double,3> d;
    for (int i = 0; i < 3; ++i) {
        d[i] = point[i] - boxCenter[i];
    }

    // Normalize each axis (guard against near-zero length)
    auto normalize = [epsilon](const std::array<double,3>& v) -> std::array<double,3> {
        double len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        if (len < epsilon) {
            return {1.0, 0.0, 0.0}; // fallback (should not happen with valid input)
        }
        double inv = 1.0 / len;
        return {v[0]*inv, v[1]*inv, v[2]*inv};
    };

    std::array<std::array<double,3>,3> axes = {
        normalize(boxOrientationAxis0),
        normalize(boxOrientationAxis1),
        normalize(boxOrientationAxis2)
    };

    // Check each local axis projection against half-extent
    for (int i = 0; i < 3; ++i) {
        double proj = d[0]*axes[i][0] + d[1]*axes[i][1] + d[2]*axes[i][2];
        if (std::fabs(proj) > boxHalfExtents[i] + epsilon) {
            return false;
        }
    }

    return true;
}

#include <cassert>
#include <array>

// Declaration of the tested function
bool isPointInsideSlab(const std::array<double,3>& point,
                       const std::array<double,3>& boxCenter,
                       const std::array<double,3>& boxHalfExtents,
                       const std::array<double,3>& boxOrientationAxis0,
                       const std::array<double,3>& boxOrientationAxis1,
                       const std::array<double,3>& boxOrientationAxis2);

int main() {
    // Axis-aligned box: center (0,0,0), half extents (1,2,3), axes are unit vectors
    std::array<double,3> center = {0,0,0};
    std::array<double,3> half = {1,2,3};
    std::array<double,3> axis0 = {1,0,0};
    std::array<double,3> axis1 = {0,1,0};
    std::array<double,3> axis2 = {0,0,1};

    // Inside point
    assert(isPointInsideSlab({0.5,1.5,2.5}, center, half, axis0, axis1, axis2));
    // Boundary point (on face)
    assert(isPointInsideSlab({1.0,0,0}, center, half, axis0, axis1, axis2));
    // Outside point
    assert(!isPointInsideSlab({1.1,0,0}, center, half, axis0, axis1, axis2));
    // Corner exactly at boundary
    assert(isPointInsideSlab({1.0,2.0,3.0}, center, half, axis0, axis1, axis2));
    // Below minimum in y
    assert(!isPointInsideSlab({0,-2.1,0}, center, half, axis0, axis1, axis2));

    // Rotated box: rotate axes 45 degrees around z
    double s = 1.0/std::sqrt(2.0);
    std::array<double,3> rot0 = {s, s, 0};
    std::array<double,3> rot1 = {-s, s, 0};
    std::array<double,3> rot2 = {0,0,1};
    // Point that is inside the rotated box (e.g., along the new x-axis)
    assert(isPointInsideSlab({0.5*s, 0.5*s, 0}, center, half, rot0, rot1, rot2));
    // Point that is outside after rotation (purely along original y but beyond rotated y)
    assert(!isPointInsideSlab({0, 1.5*s, 0}, center, half, rot0, rot1, rot2));

    // Non-unit axes (should still work after normalization)
    std::array<double,3> axis0n = {2,0,0};
    std::array<double,3> axis1n = {0,2,0};
    std::array<double,3> axis2n = {0,0,2};
    assert(isPointInsideSlab({0.5,0,0}, center, half, axis0n, axis1n, axis2n));
    assert(!isPointInsideSlab({1.1,0,0}, center, half, axis0n, axis1n, axis2n));

    // Shifted center
    std::array<double,3> shiftedCenter = {10, -5, 3};
    assert(isPointInsideSlab({10.5,-4.5,3.5}, shiftedCenter, half, axis0, axis1, axis2));
    assert(!isPointInsideSlab({11.1,-4.5,3.5}, shiftedCenter, half, axis0, axis1, axis2));

    return 0;
}

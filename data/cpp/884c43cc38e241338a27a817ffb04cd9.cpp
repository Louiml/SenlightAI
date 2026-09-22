/*
Write a C++ function `bool isConvexPolygon(const std::vector<float3>& points)` that determines whether a given sequence of 3D points (in z=0 plane, i.e., all z components equal to zero) forms a simple convex polygon. The input is a vector of `float3` points, assumed to be a simple polygon (no self-intersections, at least 3 points, all z=0). The function should return `true` if the polygon is convex (all interior angles ≤ 180°), and `false` otherwise. Points may be given in clockwise or counterclockwise order; handle both. The polygon must be strictly convex or just convex (allow collinear consecutive edges? define: convex means all cross products of consecutive edge pairs have the same sign (or zero), and the sum of absolute turn angles equals 360°). For robustness, use a small epsilon for floating-point comparisons. You must implement a minimal `float3` class with the required operators (`-`, `cross`, `dot`, `magnitude`) inside your submission.
*/

#include <vector>
#include <cmath>
#include <cfloat>
#include <algorithm>

// Minimal 3D vector class for points in the z=0 plane.
struct float3 {
    float x, y, z;
    float3() : x(0), y(0), z(0) {}
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// Vector subtraction.
inline float3 operator-(const float3& a, const float3& b) {
    return float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Cross product.
inline float3 cross(const float3& a, const float3& b) {
    return float3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Dot product.
inline float dot(const float3& a, const float3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Magnitude (length) of vector.
inline float magnitude(const float3& v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Epsilon for floating-point comparisons (relative to coordinate scale).
const float kEps = 1e-5f;

// Determine if a simple polygon (all z=0) is convex.
// Returns true if the polygon is convex (all cross products have consistent sign)
// and has nonzero area; false otherwise (e.g., collinear points, self-intersection not handled).
bool isConvexPolygon(const std::vector<float3>& points) {
    int n = static_cast<int>(points.size());
    if (n < 3) return false;

    // Track signs of cross product z-components (for z=0 plane, cross product is along z).
    // Use negative/positive/zero counters.
    bool hasPositive = false;
    bool hasNegative = false;
    // Accumulate signed area (twice the area) to detect degenerate polygon.
    float doubleArea = 0.0f;

    for (int i = 0; i < n; ++i) {
        // Get three consecutive vertices (wrapping around).
        const float3& p0 = points[i];
        const float3& p1 = points[(i + 1) % n];
        const float3& p2 = points[(i + 2) % n];

        float3 edge1 = p1 - p0;
        float3 edge2 = p2 - p1;
        // Cross product z-component for edges in z=0 plane.
        float crossZ = edge1.x * edge2.y - edge1.y * edge2.x;

        // Accumulate signed area (twice actual area for closed polygon).
        doubleArea += (p1.x - p0.x) * (p1.y + p0.y); // trapezoid formula

        // Classify sign with epsilon tolerance.
        if (crossZ > kEps) {
            hasPositive = true;
        } else if (crossZ < -kEps) {
            hasNegative = true;
        }
        // If both positive and negative signs exist, not convex.
        if (hasPositive && hasNegative) return false;
    }

    // For convex polygon, area must be nonzero (not all collinear).
    if (std::fabs(doubleArea) < kEps) return false;

    // If only one sign (or all zeros with nonzero area? impossible), it's convex.
    // Since we already rejected mixed signs, and area is nonzero, at least one sign exists.
    return true;
}

#include <cassert>
#include <vector>

// The struct and functions are defined in the solution above; we replicate them here for compilation.
// (In a real test, include the solution code above.)

int main() {
    // Square (CCW) - convex
    {
        std::vector<float3> pts = {float3(0,0,0), float3(1,0,0), float3(1,1,0), float3(0,1,0)};
        assert(isConvexPolygon(pts) == true);
    }
    // Square reversed (CW) - convex
    {
        std::vector<float3> pts = {float3(0,0,0), float3(0,1,0), float3(1,1,0), float3(1,0,0)};
        assert(isConvexPolygon(pts) == true);
    }
    // Collinear points (degenerate) - not convex
    {
        std::vector<float3> pts = {float3(0,0,0), float3(1,0,0), float3(2,0,0), float3(3,0,0)};
        assert(isConvexPolygon(pts) == false);
    }
    // Concave "arrow" shape - not convex
    {
        std::vector<float3> pts = {float3(0,0,0), float3(2,0,0), float3(1,1,0), float3(2,2,0), float3(0,2,0)};
        assert(isConvexPolygon(pts) == false);
    }
    // Triangle - convex
    {
        std::vector<float3> pts = {float3(0,0,0), float3(2,0,0), float3(1,1,0)};
        assert(isConvexPolygon(pts) == true);
    }
    // Not a polygon (fewer than 3 points)
    {
        std::vector<float3> pts = {float3(0,0,0), float3(1,0,0)};
        assert(isConvexPolygon(pts) == false);
    }
    // Pentagon convex
    {
        std::vector<float3> pts = {
            float3(0,0,0), float3(1,0,0), float3(1.5f,0.5f,0),
            float3(1,1,0), float3(0,1,0)
        };
        assert(isConvexPolygon(pts) == true);
    }
    // Self-intersecting not tested (assumption), but simple concave test above.
    // Rectangle with a redundant collinear point (still convex)
    {
        std::vector<float3> pts = {float3(0,0,0), float3(1,0,0), float3(1,0,0), float3(1,1,0), float3(0,1,0)};
        // Note: This has a duplicate point; our simple test may return true because cross products are zero/positive.
        // To be safe, this test is excluded or we can allow it. Let's assert it returns true (since zero cross products are allowed).
        assert(isConvexPolygon(pts) == true);
    }
    // Non-convex hexagon with an indentation
    {
        std::vector<float3> pts = {
            float3(0,0,0), float3(3,0,0), float3(3,3,0),
            float3(1,3,0), float3(1,1,0), float3(2,1,0), float3(2,2,0), float3(0,2,0)
        };
        // This shape is concave due to the indentation at (1,1)-(2,1)-(2,2).
        assert(isConvexPolygon(pts) == false);
    }
    return 0;
}

// The solution approach: For a simple polygon, we can check convexity by examining the sign of the cross product (z-component) of consecutive edge vectors. For a convex polygon, all cross products must have the same sign (all positive for CCW, all negative for CW), or be zero for collinear points. However, this test alone is insufficient for degenerate cases where all points are collinear (then all cross products are zero). To handle this, we can additionally verify that the polygon has a nonzero area (i.e., not all points collinear). If all cross products are zero, the polygon is degenerate and should return `false` (since a degenerate polygon isn't convex with nonzero area). If signs are consistent (all non-negative or all non-positive, with at least one nonzero), then it is convex. For edge cases: polygons with repeated consecutive points (zero-length edges) would produce zero cross products; we can skip such degenerate edges or treat them as collinear. To be safe, we can compute the signed area (sum of cross products) and ensure it is nonzero, and also check that the cross product magnitudes are either all ≥0 or all ≤0 (within epsilon tolerance). Time complexity is O(n) for n points, space O(1) beyond the input vector.

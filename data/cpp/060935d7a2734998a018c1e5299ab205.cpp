Write a C++ function that, given three 3D points defining an infinite plane, determines whether a fourth query point lies in front of, behind, or exactly on that plane. The plane's front side is defined by the direction of the cross product `(p2 - p1) × (p3 - p2)`. The function should return an enum value `0` for `BEHIND_PLANE`, `1` for `IN_PLANE`, and `2` for `FRONT_PLANE`. Input points are provided as three `float` components each, and the function must handle degenerate (collinear or coincident) triangles gracefully by returning `1` (IN_PLANE) since no valid plane exists.

The solution constructs the plane normal from the three points using the cross product of two edge vectors. If the cross product is zero (degenerate triangle), the plane is undefined, so return `1`. Otherwise, normalize the normal vector to ensure consistent distance calculations. The plane equation is `n · p = dist`, where `dist = n · p1`. For the query point, compute `signedDist = n · point - dist`. If `signedDist > ε` (with a small tolerance like `1e-5` to handle floating-point errors) return `2` (FRONT); if `< -ε` return `0` (BEHIND); otherwise return `1` (IN). Time complexity is O(1) with constant auxiliary space. Edge cases include degenerate normals, nearly coincident points, and points lying exactly on the plane where floating-point imprecision could cause misclassification—hence the epsilon tolerance.

#include <cmath>

enum PointSide {
    BEHIND_PLANE = 0,
    IN_PLANE = 1,
    FRONT_PLANE = 2
};

// Determine the side of a plane defined by three 3D points where a query point lies.
// Returns BEHIND_PLANE, IN_PLANE, or FRONT_PLANE.
PointSide classifyPlaneSide(
    const float p1[3], const float p2[3], const float p3[3],
    const float point[3]) {
    
    // Compute edge vectors
    float v1[3] = {p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
    float v2[3] = {p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]};
    
    // Cross product: normal = v1 × v2
    float n[3] = {
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]
    };
    
    // Check for degenerate triangle (zero normal)
    float lenSq = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
    if (lenSq < 1e-12f) {
        return IN_PLANE; // No valid plane, treat as on-plane
    }
    
    // Normalize normal
    float invLen = 1.0f / std::sqrt(lenSq);
    n[0] *= invLen;
    n[1] *= invLen;
    n[2] *= invLen;
    
    // Plane distance from origin: dist = n · p1
    float dist = n[0]*p1[0] + n[1]*p1[1] + n[2]*p1[2];
    
    // Signed distance of query point
    float signedDist = n[0]*point[0] + n[1]*point[1] + n[2]*point[2] - dist;
    
    const float epsilon = 1e-5f;
    if (signedDist > epsilon) {
        return FRONT_PLANE;
    } else if (signedDist < -epsilon) {
        return BEHIND_PLANE;
    } else {
        return IN_PLANE;
    }
}

#include <cassert>

int main() {
    // Positive normal case: plane z=0, points on xy-plane, normal points +z
    float p1[3] = {0,0,0};
    float p2[3] = {1,0,0};
    float p3[3] = {0,1,0};
    float front[3] = {0.5f, 0.5f, 1.0f};
    float behind[3] = {0.5f, 0.5f, -1.0f};
    float onPlane[3] = {2.0f, -3.0f, 0.0f};
    assert(classifyPlaneSide(p1,p2,p3,front) == FRONT_PLANE);
    assert(classifyPlaneSide(p1,p2,p3,behind) == BEHIND_PLANE);
    assert(classifyPlaneSide(p1,p2,p3,onPlane) == IN_PLANE);

    // Degenerate: collinear points -> IN_PLANE
    float col1[3] = {0,0,0};
    float col2[3] = {1,1,1};
    float col3[3] = {2,2,2};
    float query[3] = {100, -50, 0};
    assert(classifyPlaneSide(col1,col2,col3,query) == IN_PLANE);

    // Plane x=1, normal points +x (order yields normal in +x direction)
    float p1x[3] = {1,0,0};
    float p2x[3] = {1,1,0};
    float p3x[3] = {1,0,1};
    float frontX[3] = {2,0,0};
    float behindX[3] = {0,0,0};
    assert(classifyPlaneSide(p1x,p2x,p3x,frontX) == FRONT_PLANE);
    assert(classifyPlaneSide(p1x,p2x,p3x,behindX) == BEHIND_PLANE);

    // Negative normal direction: reversed order flips normal
    // Points define plane z=0 but normal points -z (reverse two points)
    float p1r[3] = {0,1,0};
    float p2r[3] = {1,0,0};
    float p3r[3] = {0,0,0};
    float frontNeg[3] = {0.5f, 0.5f, -1.0f};
    float behindNeg[3] = {0.5f, 0.5f, 1.0f};
    assert(classifyPlaneSide(p1r,p2r,p3r,frontNeg) == FRONT_PLANE);
    assert(classifyPlaneSide(p1r,p2r,p3r,behindNeg) == BEHIND_PLANE);

    // Point very close to plane but not exactly (small epsilon)
    float nearPoint[3] = {0.00001f, 0.00001f, 0.99999f};
    assert(classifyPlaneSide(p1,p2,p3,nearPoint) == FRONT_PLANE);
    float nearBehind[3] = {0.00001f, 0.00001f, -0.99999f};
    assert(classifyPlaneSide(p1,p2,p3,nearBehind) == BEHIND_PLANE);

    // Duplicate points - degenerate
    float dup1[3] = {5,5,5};
    float dup2[3] = {5,5,5};
    float dup3[3] = {1,2,3};
    assert(classifyPlaneSide(dup1,dup2,dup3,{0,0,0}) == IN_PLANE);

    return 0;
}

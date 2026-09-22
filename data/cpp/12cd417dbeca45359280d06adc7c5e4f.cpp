Given a 3D quadrilateral defined by a corner point `q`, two edge vectors `u` and `v`, and a normal vector `n = cross(u, v)`, write a C++ function `bool pointInQuad(CommonMath::Point3 p, CommonMath::Point3 q, CommonMath::Vec3 u, CommonMath::Vec3 v, double& alpha, double& beta)` that determines whether an arbitrary point `p` lies inside (or on the boundary of) the parallelogram spanned by `u` and `v` from `q`. The function must compute the normalized barycentric-like coordinates `alpha` and `beta` (both in [0,1]) such that `p = q + alpha*u + beta*v`. Use the same mathematical approach as the provided snippet: first compute the vector `n = cross(u,v)` and the constant `c = n / dot(n,n)`, then compute `alpha = dot(c, cross(p-q, v))` and `beta = dot(c, cross(u, p-q))`. Return `true` if both `alpha` and `beta` are within `[0,1]` (inclusive), otherwise `false`. The function must handle degenerate cases where `u` or `v` are zero vectors or parallel (i.e., `dot(n,n) == 0`) by returning `false`. Assume `CommonMath` is a namespace with `Point3` (having `x`, `y`, `z` members and operator- for vector difference), `Vec3` (having `x`, `y`, `z` members), and free functions `cross(Vec3, Vec3) -> Vec3` and `dot(Vec3, Vec3) -> double`. Implement the function without any external libraries beyond standard headers.

#include <cassert>
#include <cmath>

// Ensure the CommonMath definitions are available (for testing)
namespace CommonMath {
    struct Vec3 { double x, y, z; };
    struct Point3 { double x, y, z; };
    Vec3 operator-(const Point3& a, const Point3& b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }
    Vec3 cross(const Vec3& a, const Vec3& b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }
    double dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
}

// Declaration of the solution function (assume it's linked)
bool pointInQuad(CommonMath::Point3 p, CommonMath::Point3 q,
                 CommonMath::Vec3 u, CommonMath::Vec3 v,
                 double& alpha, double& beta);

int main() {
    // Define a parallelogram: q=(0,0,0), u=(2,0,0), v=(0,3,0)
    CommonMath::Point3 q = {0,0,0};
    CommonMath::Vec3 u = {2,0,0};
    CommonMath::Vec3 v = {0,3,0};

    double alpha, beta;

    // Interior point
    assert(pointInQuad({1,1,0}, q, u, v, alpha, beta));
    assert(std::fabs(alpha - 0.5) < 1e-9);
    assert(std::fabs(beta - 1.0/3.0) < 1e-9);

    // Corner point (q itself)
    assert(pointInQuad({0,0,0}, q, u, v, alpha, beta));
    assert(std::fabs(alpha - 0.0) < 1e-9);
    assert(std::fabs(beta - 0.0) < 1e-9);

    // Boundary point (on u edge)
    assert(pointInQuad({2,0,0}, q, u, v, alpha, beta));
    assert(std::fabs(alpha - 1.0) < 1e-9);
    assert(std::fabs(beta - 0.0) < 1e-9);

    // Point outside (x too large)
    assert(!pointInQuad({3,1,0}, q, u, v, alpha, beta));

    // Point outside (negative beta)
    assert(!pointInQuad({1,-1,0}, q, u, v, alpha, beta));

    // Point on plane but outside parallelogram (diagonal extension)
    assert(!pointInQuad({3,3,0}, q, u, v, alpha, beta));

    // Degenerate: u and v parallel
    CommonMath::Vec3 u2 = {1,0,0};
    CommonMath::Vec3 v2 = {2,0,0}; // parallel
    assert(!pointInQuad({1,0,0}, q, u2, v2, alpha, beta));

    // Degenerate: zero vector u
    CommonMath::Vec3 u3 = {0,0,0};
    assert(!pointInQuad({0,1,0}, q, u3, v, alpha, beta));

    // 3D parallelogram (plane not axis-aligned)
    CommonMath::Point3 q3 = {1,2,3};
    CommonMath::Vec3 u3d = {1,0,0};
    CommonMath::Vec3 v3d = {0,1,1};
    // Point = q + 0.5*u + 0.25*v = (1.5, 2.25, 3.25)
    assert(pointInQuad({1.5, 2.25, 3.25}, q3, u3d, v3d, alpha, beta));
    assert(std::fabs(alpha - 0.5) < 1e-9);
    assert(std::fabs(beta - 0.25) < 1e-9);

    // Point off-plane (z coordinate wrong)
    assert(!pointInQuad({1.5, 2.25, 3.5}, q3, u3d, v3d, alpha, beta));

    return 0;
}

#include <cmath>
#include <limits>

// Forward declare CommonMath functions (assumed provided)
namespace CommonMath {
    struct Vec3 { double x, y, z; };
    struct Point3 { double x, y, z; };
    Vec3 operator-(const Point3& a, const Point3& b) {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }
    Vec3 cross(const Vec3& a, const Vec3& b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }
    double dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
}

// Determine if point p lies in the parallelogram q + alpha*u + beta*v, 0<=alpha<=1, 0<=beta<=1.
// On success, sets alpha and beta to the barycentric coordinates. Returns false for degenerate u,v.
bool pointInQuad(
        CommonMath::Point3 p,
        CommonMath::Point3 q,
        CommonMath::Vec3 u,
        CommonMath::Vec3 v,
        double& alpha,
        double& beta) {

    // Degenerate if u or v is zero or they are parallel (cross product zero).
    CommonMath::Vec3 n = CommonMath::cross(u, v);
    double n_len_sq = CommonMath::dot(n, n);
    if (n_len_sq < 1e-12) {
        return false;
    }

    // Constant vector as used in the snippet: n / dot(n,n)
    CommonMath::Vec3 c = {n.x / n_len_sq, n.y / n_len_sq, n.z / n_len_sq};

    // Vector from corner to point
    CommonMath::Vec3 w = p - q; // operator- defined above

    // Compute alpha = dot(c, cross(w, v))
    CommonMath::Vec3 cross_w_v = CommonMath::cross(w, v);
    alpha = CommonMath::dot(c, cross_w_v);

    // Compute beta = dot(c, cross(u, w))
    CommonMath::Vec3 cross_u_w = CommonMath::cross(u, w);
    beta = CommonMath::dot(c, cross_u_w);

    // Check bounds (inclusive)
    const double eps = 1e-9;
    return (alpha >= -eps && alpha <= 1.0 + eps &&
            beta  >= -eps && beta  <= 1.0 + eps);
}

// The solution directly mirrors the plane-intersection logic from the snippet, but specialized to a point (no ray). The core idea: for a point `p`, compute the vector `w = p - q`. In the parallelogram's basis `{u, v}`, we want `w = alpha*u + beta*v`. The snippet uses the geometric property that `cross(w, v)` is perpendicular to `v` and its dot with `c = n/dot(n,n)` extracts `alpha` (since `cross(u,v)=n`, and `cross(alpha*u + beta*v, v) = alpha*cross(u,v) = alpha*n`, and dot with `n/dot(n,n)` gives `alpha`). Similarly, `cross(u,w)` gives `beta*cross(u,v) = beta*n`, so dot with `c` yields `beta`. The key edge case: if `u` and `v` are linearly dependent (parallel or zero), then `cross(u,v) = 0` (zero vector), and `dot(n,n) = 0`, causing division by zero. So we must check `dot(n,n) == 0` first. Also, because of floating-point precision, "zero" should be compared with a small epsilon (e.g., `1e-9`). Another edge case: points exactly on the boundary (alpha=0 or 1, beta=0 or 1) should be considered inside. Time complexity: O(1) operations (a few cross products, dot products, comparisons). Space complexity: O(1) auxiliary.

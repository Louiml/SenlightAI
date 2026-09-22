/*
Write a standalone C++ function `bool triangleContainsPoint(const std::array<std::array<double,3>,3>& triangle, const std::array<double,3>& point, double eps)` that determines whether a 3D point lies **on or inside** the triangular face defined by three vertices. Use the barycentric coordinate approach: compute the triangle’s normal via the cross product of two edge vectors, then check if the point lies in the plane (within tolerance `eps`), and if so, compute barycentric coordinates `(λ1, λ2)` such that `point = v0 + λ1*e1 + λ2*e2`, with `λ1 ≥ -eps`, `λ2 ≥ -eps`, and `λ1+λ2 ≤ 1+eps`. Return `true` if all conditions hold; otherwise `false`. The function must handle degenerate triangles (zero area) by returning `false`, and must be robust to floating-point inaccuracies. Do not use any external libraries beyond standard headers; implement vector arithmetic manually. The solution must be self-contained, const-correct, and not include a `main` function.
*/
#include <array>
#include <cmath>
#include <algorithm>

// Determine if a 3D point lies on or inside a triangular face.
// Uses barycentric coordinates with an orthonormal projection onto the triangle's plane.
bool triangleContainsPoint(const std::array<std::array<double,3>,3>& triangle,
                           const std::array<double,3>& point,
                           double eps = 1e-8) {
    const auto& v0 = triangle[0];
    const auto& v1 = triangle[1];
    const auto& v2 = triangle[2];

    // Edge vectors
    std::array<double,3> e1 = {v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]};
    std::array<double,3> e2 = {v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]};

    // Normal vector n = cross(e1, e2)
    std::array<double,3> n = {
        e1[1]*e2[2] - e1[2]*e2[1],
        e1[2]*e2[0] - e1[0]*e2[2],
        e1[0]*e2[1] - e1[1]*e2[0]
    };

    // Check degenerate triangle (zero area)
    double n_len2 = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
    if (n_len2 < 1e-24) return false;
    double n_len = std::sqrt(n_len2);

    // Vector from v0 to point
    std::array<double,3> w = {point[0]-v0[0], point[1]-v0[1], point[2]-v0[2]};

    // Check if point is in the plane: |w·n| <= eps * |n|
    double dot = w[0]*n[0] + w[1]*n[1] + w[2]*n[2];
    if (std::fabs(dot) > eps * n_len) return false;

    // Build an orthonormal basis in the plane: u = normalize(e1), v = normalize(n × u)
    double e1_len = std::sqrt(e1[0]*e1[0] + e1[1]*e1[1] + e1[2]*e1[2]);
    if (e1_len < 1e-12) return false; // e1 degenerate
    std::array<double,3> u = {e1[0]/e1_len, e1[1]/e1_len, e1[2]/e1_len};

    // v = normalize(cross(n, u))
    std::array<double,3> t = {
        n[1]*u[2] - n[2]*u[1],
        n[2]*u[0] - n[0]*u[2],
        n[0]*u[1] - n[1]*u[0]
    };
    double t_len = std::sqrt(t[0]*t[0] + t[1]*t[1] + t[2]*t[2]);
    if (t_len < 1e-12) return false;
    std::array<double,3> v = {t[0]/t_len, t[1]/t_len, t[2]/t_len};

    // Project edges and point onto u,v coordinates
    double e1_u = e1[0]*u[0] + e1[1]*u[1] + e1[2]*u[2];
    double e1_v = e1[0]*v[0] + e1[1]*v[1] + e1[2]*v[2];
    double e2_u = e2[0]*u[0] + e2[1]*u[1] + e2[2]*u[2];
    double e2_v = e2[0]*v[0] + e2[1]*v[1] + e2[2]*v[2];
    double w_u = w[0]*u[0] + w[1]*u[1] + w[2]*u[2];
    double w_v = w[0]*v[0] + w[1]*v[1] + w[2]*v[2];

    // Solve 2x2 system: w = λ1*e1 + λ2*e2 in (u,v) coordinates
    double det = e1_u * e2_v - e1_v * e2_u;
    if (std::fabs(det) < 1e-12) return false; // should not happen for non-degenerate triangles

    double lam1 = (w_u * e2_v - w_v * e2_u) / det;
    double lam2 = (e1_u * w_v - e1_v * w_u) / det;

    // Barycentric inequalities
    return (lam1 >= -eps) && (lam2 >= -eps) && (lam1 + lam2 <= 1.0 + eps);
}
#include <array>
#include <cassert>
#include <cmath>

// The solution function is assumed to be declared above or in a header.
// We declare it here for the test.
bool triangleContainsPoint(const std::array<std::array<double,3>,3>& triangle,
                           const std::array<double,3>& point,
                           double eps = 1e-8);

int main() {
    // Triangle in the XY plane: (0,0,0), (1,0,0), (0,1,0)
    std::array<std::array<double,3>,3> tri = {{{0,0,0}, {1,0,0}, {0,1,0}}};

    // Point inside
    assert(triangleContainsPoint(tri, {0.25, 0.25, 0.0}));
    // Point on vertex
    assert(triangleContainsPoint(tri, {0.0, 0.0, 0.0}));
    // Point on edge
    assert(triangleContainsPoint(tri, {0.5, 0.0, 0.0}));
    // Point outside but in plane
    assert(!triangleContainsPoint(tri, {0.6, 0.6, 0.0}));
    // Point below plane (slightly off due to tolerance)
    assert(!triangleContainsPoint(tri, {0.25, 0.25, 0.01}));
    // Point above plane but within tol
    assert(triangleContainsPoint(tri, {0.25, 0.25, 1e-9}));

    // Degenerate triangle (collinear points)
    std::array<std::array<double,3>,3> degenerate = {{{0,0,0}, {1,1,1}, {2,2,2}}};
    assert(!triangleContainsPoint(degenerate, {0.5,0.5,0.5}));

    // Triangle not axis-aligned
    std::array<std::array<double,3>,3> tri3 = {{{1,2,3}, {4,5,6}, {7,8,0}}};
    // Center point of triangle (average of vertices) should be inside
    std::array<double,3> center = {(1+4+7)/3.0, (2+5+8)/3.0, (3+6+0)/3.0};
    assert(triangleContainsPoint(tri3, center, 1e-6));
    // Clearly outside
    assert(!triangleContainsPoint(tri3, {10,10,10}));

    // Test tolerance: point slightly off plane but within eps
    std::array<std::array<double,3>,3> tri_eps = {{{0,0,0}, {2,0,0}, {0,2,0}}};
    assert(triangleContainsPoint(tri_eps, {1,1,1e-7}, 1e-6));
    assert(!triangleContainsPoint(tri_eps, {1,1,1e-5}, 1e-6));

    return 0;
}
// The algorithm computes two edge vectors `e1 = v1 - v0` and `e2 = v2 - v0`, then the normal `n = e1 × e2`. If the normal’s squared length is less than a small threshold (e.g., `1e-24`), the triangle is degenerate and we return `false`. Next, compute the vector `w = point - v0` and check if it lies in the plane by testing `|w·n| ≤ eps * |n|`; using the normalized normal would require division, so compare `|w·n| ≤ eps * |n|` to avoid scaling issues. If in-plane, we need barycentric coordinates. Since `e1` and `e2` are not orthonormal, we solve the 2×3 linear system. A simple and robust method is to project everything onto two orthogonal axes perpendicular to `n`. Let `u = normalize(e1)`, then `v = normalize(n × u)`. Then the 2D coordinates of `w`, `e1`, and `e2` in this orthonormal basis follow, and we solve for `λ1, λ2` via the 2×2 system: `w_u = λ1*e1_u + λ2*e2_u`, `w_v = λ1*e1_v + λ2*e2_v`. The determinant is `det = e1_u*e2_v - e1_v*e2_u`; if `|det|` is tiny, fallback to using the pseudo-inverse approach with `w1` and `w2` as in the snippet (projecting onto the triangle plane’s two basis vectors). However, the projection method is simpler and numerically stable. Compute `λ1 = (w_u*e2_v - w_v*e2_u)/det` and `λ2 = (e1_u*w_v - e1_v*w_u)/det`. Then check the inequalities. Time complexity is O(1) with constant space. Edge cases: point exactly on vertex, on edge, outside, and degenerate triangle.

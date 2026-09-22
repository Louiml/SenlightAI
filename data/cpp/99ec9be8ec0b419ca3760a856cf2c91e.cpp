/*
Write a standalone C++ function `nearest_ray_triangle_hit` that takes as input: a ray origin `source` (as a `std::array<double,3>`), a ray direction `dir` (as a `std::array<double,3>`), a list of triangle vertices `V` (as a `std::vector<std::array<double,3>>` where each triangle is represented by 3 consecutive vertices), and a list of triangle indices `F` (as a `std::vector<std::array<int,3>>` where each entry gives the three vertex indices of a triangle). The function must return a `std::optional<Hit>` where `Hit` is a struct containing `triangle_id` (int), `barycentric_u` (double), `barycentric_v` (double), and `t` (double). The function should cast a ray against all triangles in the mesh using the Möller–Trumbore intersection algorithm, keep only hits where `t > 0` (not behind the ray origin), and return the hit with the smallest positive `t` value. If no triangle is hit, return `std::nullopt`. The function must be self-contained (no external libraries), use `const` correctly for all inputs, and not modify them.
*/
#include <optional>
#include <vector>
#include <array>
#include <cmath>
#include <limits>

struct Hit {
    int triangle_id;
    double barycentric_u;
    double barycentric_v;
    double t;
};

// Returns the nearest (smallest positive t) ray-triangle intersection among all triangles,
// or std::nullopt if no hit occurs. Uses the Möller–Trumbore algorithm.
std::optional<Hit> nearest_ray_triangle_hit(
    const std::array<double,3>& source,
    const std::array<double,3>& dir,
    const std::vector<std::array<double,3>>& V,
    const std::vector<std::array<int,3>>& F)
{
    const double epsilon = 1e-9;
    double best_t = std::numeric_limits<double>::infinity();
    std::optional<Hit> best_hit;

    for (int f = 0; f < static_cast<int>(F.size()); ++f) {
        const auto& idx = F[f];
        const auto& v0 = V[idx[0]];
        const auto& v1 = V[idx[1]];
        const auto& v2 = V[idx[2]];

        // Edge vectors
        std::array<double,3> e1 = {v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]};
        std::array<double,3> e2 = {v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]};

        // h = cross(dir, e2)
        std::array<double,3> h = {
            dir[1]*e2[2] - dir[2]*e2[1],
            dir[2]*e2[0] - dir[0]*e2[2],
            dir[0]*e2[1] - dir[1]*e2[0]
        };

        double a = e1[0]*h[0] + e1[1]*h[1] + e1[2]*h[2];
        if (std::fabs(a) < epsilon) continue; // parallel to triangle

        double inv_a = 1.0 / a;

        // s = source - v0
        std::array<double,3> s = {source[0]-v0[0], source[1]-v0[1], source[2]-v0[2]};

        double u = inv_a * (s[0]*h[0] + s[1]*h[1] + s[2]*h[2]);
        if (u < 0.0 || u > 1.0) continue;

        // q = cross(s, e1)
        std::array<double,3> q = {
            s[1]*e1[2] - s[2]*e1[1],
            s[2]*e1[0] - s[0]*e1[2],
            s[0]*e1[1] - s[1]*e1[0]
        };

        double v = inv_a * (q[0]*dir[0] + q[1]*dir[1] + q[2]*dir[2]);
        if (v < 0.0 || u + v > 1.0) continue;

        double t = inv_a * (q[0]*e2[0] + q[1]*e2[1] + q[2]*e2[2]);
        if (t > 0.0 && t < best_t) {
            best_t = t;
            best_hit = Hit{f, u, v, t};
        }
    }

    return best_hit;
}
#include <cassert>
#include <cmath>
#include <optional>
#include <vector>
#include <array>

// The solution function is assumed to be declared above, but for the test we include it here.
// For brevity, the actual function definition is not repeated; this is the test only.
int main() {
    // Simple triangle in the XY plane at z=0: vertices (0,0,0), (1,0,0), (0,1,0)
    std::vector<std::array<double,3>> V = {{0,0,0}, {1,0,0}, {0,1,0}};
    std::vector<std::array<int,3>> F = {{0,1,2}};

    // Ray origin above the triangle pointing down (0,0,-1) should hit at t=1
    {
        auto hit = nearest_ray_triangle_hit({0.25,0.25,1.0}, {0,0,-1}, V, F);
        assert(hit.has_value());
        assert(hit->triangle_id == 0);
        assert(std::fabs(hit->t - 1.0) < 1e-9);
        assert(std::fabs(hit->barycentric_u - 0.25) < 1e-6);
        assert(std::fabs(hit->barycentric_v - 0.25) < 1e-6);
    }

    // Ray pointing away from the triangle (t negative) should not hit
    {
        auto hit = nearest_ray_triangle_hit({0.25,0.25,1.0}, {0,0,1}, V, F);
        assert(!hit.has_value());
    }

    // Two triangles: one nearer, one farther; the nearer should be returned
    {
        std::vector<std::array<double,3>> V2 = {
            {0,0,0}, {1,0,0}, {0,1,0},   // triangle 0 at z=0
            {0,0,-1}, {1,0,-1}, {0,1,-1} // triangle 1 at z=-1
        };
        std::vector<std::array<int,3>> F2 = {{0,1,2}, {3,4,5}};
        auto hit = nearest_ray_triangle_hit({0.25,0.25,2.0}, {0,0,-1}, V2, F2);
        assert(hit.has_value());
        assert(hit->triangle_id == 0);
        assert(std::fabs(hit->t - 2.0) < 1e-9);
    }

    // Ray completely missing all triangles
    {
        auto hit = nearest_ray_triangle_hit({2.0,2.0,1.0}, {0,0,-1}, V, F);
        assert(!hit.has_value());
    }

    // Ray hitting exactly on the edge (u=0) is included
    {
        auto hit = nearest_ray_triangle_hit({0.0,0.0,1.0}, {0,0,-1}, V, F);
        assert(hit.has_value());
        assert(std::fabs(hit->barycentric_u) < 1e-9);
        assert(std::fabs(hit->barycentric_v) < 1e-9);
    }
}
// The core algorithm is the Möller–Trumbore ray-triangle intersection test, which efficiently computes the intersection point of a ray with a triangle using barycentric coordinates. For each triangle, extract its three vertices `v0, v1, v2` from the `V` and `F` arrays. Compute the edge vectors `e1 = v1 - v0` and `e2 = v2 - v0`. Compute the determinant `det = dot(dir, cross(e1, e2))`. If `det` is near zero (within a small epsilon like `1e-9`), the ray is parallel to the triangle and we skip it. Otherwise, compute `inv_det = 1/det`, vector `pvec = cross(dir, e2)`, and scalar `u = inv_det * dot(pvec, (source - v0))`. If `u < 0` or `u > 1`, skip. Compute `qvec = cross(e1, (source - v0))` and `v = inv_det * dot(qvec, dir)`. If `v < 0` or `u+v > 1`, skip. Compute `t = inv_det * dot(e2, qvec)`. If `t > 0` and `t` is smaller than the current best, update the best hit. After looping all triangles, return the best hit if found. Edge cases include degenerate triangles (zero area), rays parallel to the plane, hits exactly on boundaries (use inclusive checks for `u` and `v` boundaries but consistent with typical implementations), and negative or zero `t` (reject). Time complexity is O(T) where T is the number of triangles, since each triangle is processed in constant time; space complexity is O(1) auxiliary.

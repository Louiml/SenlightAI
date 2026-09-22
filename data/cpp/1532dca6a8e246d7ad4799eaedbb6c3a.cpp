Write a C++ function named `computeAmbientOcclusion` that estimates ambient occlusion values at a set of query points on or near a triangle mesh surface. The function takes as input: a dense matrix `V` of vertex positions (each row is a 3D point), an integer matrix `F` of triangle faces (each row contains three vertex indices), a dense matrix `P` of query points (each row is a 3D position), a dense matrix `N` of surface normals at the query points (each row is a 3D unit vector), and an integer `num_samples` specifying the number of random hemisphere rays to cast per query point. The function must output a dense matrix `S` with one row per query point and a single column containing the estimated ambient occlusion value in the range [0,1] (0 = fully occluded, 1 = fully open). The algorithm should work in a standalone manner (no external mesh library) and must handle valid, non-degenerate triangle meshes with at least one face. The returned occlusion value for a query point is the fraction of `num_samples` rays (sampled uniformly over the hemisphere aligned with the given normal) that do **not** intersect any triangle in the mesh before a far-away bounding sphere (e.g., radius equal to the mesh's bounding sphere radius times 10). Rays that miss all triangles count as unoccluded. The function must be robust to query points exactly on the surface (use a small epsilon offset to avoid self-intersection) and must handle meshes with arbitrary scale and translation. The implementation must not rely on any external libraries beyond the C++ standard library and Eigen (which is allowed for matrix types, but ray-triangle tests must be implemented explicitly).
The core algorithm is Monte Carlo ray casting. For each query point `P_i` with normal `N_i`, generate `num_samples` random directions uniformly distributed over the hemisphere centered at `N_i`. This is done by first generating uniform random points on the unit sphere (e.g., using spherical coordinates: `theta = acos(1-2*u)`, `phi = 2*pi*v`, then `dir = (sin(theta)cos(phi), sin(theta)sin(phi), cos(theta))`), then reflecting the direction to align with `N_i` using a rotation that maps the Z-axis to `N_i`. For each ray, compute the intersection with every triangle in the mesh using the Möller–Trumbore algorithm. If any intersection occurs within a finite ray length `t_max` (set to 10 times the mesh bounding sphere radius), the ray is considered occluded; otherwise it is open. Important edge cases: (1) If `num_samples <= 0`, return all zeros (no rays cast, assume fully occluded). (2) If the mesh has no faces, return all ones (nothing to occlude). (3) To avoid self-intersection when `P_i` lies exactly on a triangle, offset the ray origin slightly along the normal (e.g., by `1e-6 * bounding_radius`). (4) Use double precision for intersection tests to avoid numerical issues. (5) For efficiency, precompute triangle vertices and edge vectors, but the simplest correct version is O(num_samples * F) per query point; with a bounding box rejection test to skip triangles whose bounding box does not intersect the ray, average performance improves. Time complexity: O(Q * S * F) worst-case, where Q is number of query points. Space complexity: O(Q) for output, plus O(F) if triangle data is precomputed. The solution uses only Eigen for matrices and standard library for random number generation (e.g., `std::mt19937` and `std::uniform_real_distribution`).
#include <Eigen/Core>
#include <random>
#include <cmath>
#include <limits>

// Efficient ray-triangle intersection (Möller–Trumbore).
// Returns true if the ray (origin, dir) intersects triangle (v0,v1,v2) at distance t in (t_min, t_max).
static bool rayTriangleIntersect(
    const Eigen::Vector3d& origin,
    const Eigen::Vector3d& dir,
    const Eigen::Vector3d& v0,
    const Eigen::Vector3d& v1,
    const Eigen::Vector3d& v2,
    double t_min,
    double t_max,
    double epsilon = 1e-12)
{
    const Eigen::Vector3d edge1 = v1 - v0;
    const Eigen::Vector3d edge2 = v2 - v0;
    const Eigen::Vector3d pvec = dir.cross(edge2);
    const double det = edge1.dot(pvec);

    if (std::abs(det) < epsilon) return false; // Ray parallel to triangle

    const double inv_det = 1.0 / det;
    const Eigen::Vector3d tvec = origin - v0;
    const double u = tvec.dot(pvec) * inv_det;
    if (u < 0.0 || u > 1.0) return false;

    const Eigen::Vector3d qvec = tvec.cross(edge1);
    const double v = dir.dot(qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) return false;

    const double t = edge2.dot(qvec) * inv_det;
    return (t > t_min && t < t_max);
}

// Compute ambient occlusion at query points P with normals N on mesh (V,F).
// S is output: one row per query point, single column with AO value in [0,1].
void computeAmbientOcclusion(
    const Eigen::MatrixXd& V,
    const Eigen::MatrixXi& F,
    const Eigen::MatrixXd& P,
    const Eigen::MatrixXd& N,
    int num_samples,
    Eigen::MatrixXd& S)
{
    const int Q = static_cast<int>(P.rows());
    S.resize(Q, 1);
    S.setZero();

    if (num_samples <= 0 || F.rows() == 0) {
        // If no samples or no faces, return zeros (fully occluded) or ones? 
        // The specification: num_samples <= 0 => zeros; no faces => ones.
        if (F.rows() == 0) {
            S.setOnes();
        }
        return;
    }

    // Compute bounding sphere radius of mesh for t_max and epsilon offset.
    Eigen::Vector3d center = Eigen::Vector3d::Zero();
    for (int i = 0; i < V.rows(); ++i) {
        center += V.row(i).transpose();
    }
    center /= static_cast<double>(V.rows());

    double max_dist_sq = 0.0;
    for (int i = 0; i < V.rows(); ++i) {
        Eigen::Vector3d v = V.row(i).transpose() - center;
        max_dist_sq = std::max(max_dist_sq, v.squaredNorm());
    }
    const double bounding_radius = std::sqrt(max_dist_sq);
    const double t_max = 10.0 * bounding_radius;
    const double eps_offset = 1e-6 * bounding_radius;

    // Random generator
    std::mt19937_64 rng(12345); // fixed seed for reproducibility
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    // Precompute triangle vertices for faster access
    std::vector<Eigen::Vector3d> tri_v0(F.rows()), tri_v1(F.rows()), tri_v2(F.rows());
    for (int f = 0; f < F.rows(); ++f) {
        tri_v0[f] = V.row(F(f,0)).transpose();
        tri_v1[f] = V.row(F(f,1)).transpose();
        tri_v2[f] = V.row(F(f,2)).transpose();
    }

    for (int q = 0; q < Q; ++q) {
        Eigen::Vector3d origin = P.row(q).transpose();
        Eigen::Vector3d normal = N.row(q).transpose();
        normal.normalize();

        // Build orthonormal basis (tangent, bitangent, normal) to rotate hemisphere sample.
        Eigen::Vector3d helper(1.0, 0.0, 0.0);
        if (std::abs(normal.dot(helper)) > 0.9) {
            helper = Eigen::Vector3d(0.0, 1.0, 0.0);
        }
        Eigen::Vector3d tangent = normal.cross(helper).normalized();
        Eigen::Vector3d bitangent = normal.cross(tangent).normalized();

        int occluded_count = 0;

        // Offset origin slightly along normal to avoid self-intersection.
        Eigen::Vector3d ray_origin = origin + normal * eps_offset;

        for (int s = 0; s < num_samples; ++s) {
            // Sample uniform point on unit sphere, then keep only upper hemisphere in local coordinates
            // Actually we sample full sphere and reflect to hemisphere if needed.
            double u1 = uni(rng);
            double u2 = uni(rng);
            double theta = 2.0 * M_PI * u1;
            double z = 2.0 * u2 - 1.0; // from -1 to 1
            double r = std::sqrt(std::max(0.0, 1.0 - z*z));
            Eigen::Vector3d dir_local(r * std::cos(theta), r * std::sin(theta), z);

            // Reflect to upper hemisphere if z<0
            if (dir_local.z() < 0.0) {
                dir_local.z() = -dir_local.z();
            }
            // Normalize (already unit length from sphere construction, but z change may break)
            dir_local.normalize();

            // Transform to world space using basis
            Eigen::Vector3d ray_dir = tangent * dir_local.x() + bitangent * dir_local.y() + normal * dir_local.z();
            ray_dir.normalize();

            bool hit = false;
            for (int f = 0; f < F.rows() && !hit; ++f) {
                if (rayTriangleIntersect(ray_origin, ray_dir,
                                         tri_v0[f], tri_v1[f], tri_v2[f],
                                         eps_offset, t_max)) {
                    hit = true;
                }
            }
            if (hit) {
                occluded_count++;
            }
        }

        double ao = 1.0 - static_cast<double>(occluded_count) / static_cast<double>(num_samples);
        ao = std::min(1.0, std::max(0.0, ao));
        S(q, 0) = ao;
    }
}
#include <cassert>
#include <Eigen/Core>
#include <cmath>

// Already included in solution header
// Include the solution function here (or link it)

int main() {
    // Test 1: Simple single-triangle mesh, query point far above surface.
    // The triangle is in the xy-plane, query point at (0,0,1), normal +z.
    // Hemisphere rays should mostly miss, so AO close to 1.
    Eigen::MatrixXd V(3,3);
    V << 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0,
         0.0, 1.0, 0.0;
    Eigen::MatrixXi F(1,3);
    F << 0, 1, 2;

    Eigen::MatrixXd P(1,3);
    P << 0.0, 0.0, 1.0;
    Eigen::MatrixXd N(1,3);
    N << 0.0, 0.0, 1.0;

    Eigen::MatrixXd S;
    computeAmbientOcclusion(V, F, P, N, 1000, S);
    assert(S.rows() == 1 && S.cols() == 1);
    assert(std::abs(S(0,0) - 1.0) < 0.1); // should be near 1

    // Test 2: Query point directly on the triangle face, normal +z.
    // Ray origin offset slightly up; many rays should hit the triangle immediately, so AO near 0.
    P << 0.25, 0.25, 0.0;
    computeAmbientOcclusion(V, F, P, N, 1000, S);
    assert(std::abs(S(0,0) - 0.0) < 0.2); // should be near 0

    // Test 3: Empty mesh (no faces) should give all ones.
    Eigen::MatrixXd V2(0,3);
    Eigen::MatrixXi F2(0,3);
    Eigen::MatrixXd P2(2,3);
    P2 << 0.0,0.0,0.0,
          1.0,1.0,1.0;
    Eigen::MatrixXd N2(2,3);
    N2 << 0.0,0.0,1.0,
          0.0,0.0,1.0;
    Eigen::MatrixXd S2;
    computeAmbientOcclusion(V2, F2, P2, N2, 100, S2);
    assert((S2.array() - 1.0).abs().maxCoeff() < 1e-12);

    // Test 4: num_samples = 0 should give zeros.
    computeAmbientOcclusion(V, F, P, N, 0, S);
    assert((S.array().abs()).maxCoeff() < 1e-12);

    // Test 5: Closed box (6 faces) query point inside should be fully occluded (AO ~0).
    // Build a simple cube with 12 triangles (two per face)
    Eigen::MatrixXd Vcube(8,3);
    Vcube << 0,0,0,
             1,0,0,
             1,1,0,
             0,1,0,
             0,0,1,
             1,0,1,
             1,1,1,
             0,1,1;
    Eigen::MatrixXi Fcube(12,3);
    Fcube << 0,1,2, 0,2,3,  // bottom
            4,5,6, 4,6,7,   // top
            0,4,5, 0,5,1,   // front
            3,2,6, 3,6,7,   // back
            0,3,7, 0,7,4,   // left
            1,2,6, 1,6,5;   // right
    Eigen::MatrixXd Pcube(1,3);
    Pcube << 0.5,0.5,0.5;
    Eigen::MatrixXd Ncube(1,3);
    Ncube << 0.0, 1.0, 0.0; // any normal direction
    Eigen::MatrixXd Scube;
    computeAmbientOcclusion(Vcube, Fcube, Pcube, Ncube, 500, Scube);
    assert(std::abs(Scube(0,0) - 0.0) < 0.2);

    // Test 6: Query point outside a large planar mesh, normal pointing away, should have AO near 1.
    // Reuse test 1 with more samples and tighter tolerance
    computeAmbientOcclusion(V, F, P, N, 5000, S);
    assert(std::abs(S(0,0) - 1.0) < 0.05);

    return 0;
}

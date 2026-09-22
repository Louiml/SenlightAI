Write a C++ function `raycastAveragedDistance` that takes a scene defined by a set of triangles (vertices and triangle indices), a set of query points, the corresponding outward normals at those points, and a number `num_samples`. For each query point, the function must cast exactly `num_samples` ray directions (provided by a deterministic stratified sampling strategy: for each sample, generate a random direction on the sphere, and if the direction has a positive dot product with the normal, flip it to point inward) from a slight offset `origin + 1e-4 * dir` along the ray, compute the distance to the first triangle intersection (return `+infinity` if no hit), accumulate these finite distances, and store the average over the finite hits in an output column vector `S`. The ray-triangle intersection must be implemented naively by iterating over all triangles; if no triangle is hit, ignore that sample. If zero samples hit, set the output to 0. The function signature must be: `void raycastAveragedDistance(const std::vector<Eigen::Vector3d>& V, const std::vector<Eigen::Vector3i>& F, const std::vector<Eigen::Vector3d>& P, const std::vector<Eigen::Vector3d>& N, int num_samples, std::vector<double>& S)`.
// The core algorithm iterates over each query point `p` (with associated outward normal `n`). For each sample `s` from 0 to `num_samples-1`, generate a random direction `d` on the unit sphere (e.g., using a uniform distribution over the sphere surface). To ensure the ray goes "inward" relative to the surface, if `d.dot(n) > 0`, negate `d`. Then offset the ray origin slightly along the ray direction to avoid self-intersection at the query point: `origin = P[p] + 1e-4 * d`. Perform a brute‑force ray–triangle intersection against every triangle in `F`. Use the Möller–Trumbore algorithm; if the intersection parameter `t` is finite and positive, record that distance. After all samples, if at least one hit occurred, set `S[p] = total_distance / num_hits`; otherwise set `S[p] = 0.0`. Edge cases: (1) `num_samples` could be zero – then output 0. (2) A sample might hit a triangle exactly at the offset origin (due to numerical precision) – the offset helps avoid that. (3) Some directions may miss all triangles – those samples are ignored. (4) The random directions must be independent per sample; use a simple deterministic pseudo‑random generator (e.g., `std::mt19937` seeded with a fixed value) for reproducibility. Complexity: For each query point, `num_samples` rays, each intersecting with all `F.size()` triangles; if there are `P.size()` query points, total time is O(P * samples * F). Space is O(P) for the output, plus constant per intersection.
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>
#include <Eigen/Core>

// Compute the averaged distance of inward‑pointing rays from each query point to the first triangle hit.
// V: vertices (3D), F: triangle vertex indices (triplets), P: query points, N: normals at P,
// num_samples: number of random rays per point, S: output averaged distances.
void raycastAveragedDistance(
    const std::vector<Eigen::Vector3d>& V,
    const std::vector<Eigen::Vector3i>& F,
    const std::vector<Eigen::Vector3d>& P,
    const std::vector<Eigen::Vector3d>& N,
    int num_samples,
    std::vector<double>& S)
{
    S.clear();
    S.resize(P.size(), 0.0);
    if (num_samples <= 0 || P.empty() || F.empty()) return;

    // Deterministic random generator for reproducible results.
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    const double inf = std::numeric_limits<double>::infinity();

    for (size_t p = 0; p < P.size(); ++p) {
        const Eigen::Vector3d& origin_point = P[p];
        const Eigen::Vector3d& normal = N[p];

        double total_distance = 0.0;
        int num_hits = 0;

        for (int s = 0; s < num_samples; ++s) {
            // Generate a random direction on unit sphere.
            Eigen::Vector3d dir;
            do {
                dir = Eigen::Vector3d(dist(gen), dist(gen), dist(gen));
            } while (dir.norm() < 1e-9);
            dir.normalize();

            // Ensure direction points inward relative to the normal.
            if (dir.dot(normal) > 0.0) {
                dir = -dir;
            }

            // Offset origin slightly along ray to avoid self‑intersection.
            Eigen::Vector3d origin = origin_point + 1e-4 * dir;

            // Brute‑force ray–triangle intersection.
            double best_t = inf;
            for (const auto& tri : F) {
                const Eigen::Vector3d& v0 = V[tri[0]];
                const Eigen::Vector3d& v1 = V[tri[1]];
                const Eigen::Vector3d& v2 = V[tri[2]];

                // Möller–Trumbore algorithm.
                Eigen::Vector3d e1 = v1 - v0;
                Eigen::Vector3d e2 = v2 - v0;
                Eigen::Vector3d tvec = origin - v0;
                Eigen::Vector3d pvec = dir.cross(e2);
                double det = e1.dot(pvec);
                if (std::fabs(det) < 1e-12) continue; // parallel
                double inv_det = 1.0 / det;
                double u = tvec.dot(pvec) * inv_det;
                if (u < 0.0 || u > 1.0) continue;
                Eigen::Vector3d qvec = tvec.cross(e1);
                double v = dir.dot(qvec) * inv_det;
                if (v < 0.0 || u + v > 1.0) continue;
                double t = e2.dot(qvec) * inv_det;
                if (t > 1e-6 && t < best_t) {
                    best_t = t;
                }
            }

            if (std::isfinite(best_t)) {
                total_distance += best_t;
                ++num_hits;
            }
        }

        S[p] = (num_hits > 0) ? (total_distance / num_hits) : 0.0;
    }
}
#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Core>
// Assume raycastAveragedDistance is declared above.

int main() {
    // Test 1: Single triangle, single query point at centroid, normal outward.
    {
        std::vector<Eigen::Vector3d> V = {
            {0.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0}
        };
        std::vector<Eigen::Vector3i> F = {{0, 1, 2}};
        std::vector<Eigen::Vector3d> P = {{1.0/3.0, 1.0/3.0, 0.0}};
        std::vector<Eigen::Vector3d> N = {{0.0, 0.0, 1.0}}; // outward from triangle plane
        int samples = 100;
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, samples, S);
        assert(S.size() == 1);
        // The triangle is planar at z=0. If the normal is +z, all inward rays go towards negative z and miss,
        // so no hits → S=0. Testing non‑hit case.
        assert(S[0] == 0.0);
    }

    // Test 2: Two triangles forming a box corner, query point near corner.
    {
        std::vector<Eigen::Vector3d> V = {
            {0.0, 0.0, 0.0}, // 0
            {1.0, 0.0, 0.0}, // 1
            {0.0, 1.0, 0.0}, // 2
            {0.0, 0.0, 1.0}  // 3
        };
        std::vector<Eigen::Vector3i> F = {
            {0, 1, 2}, // floor triangle (z=0)
            {0, 2, 3}  // side triangle (x=0)
        };
        // Query point just above floor and off the side, pointing inward (toward both faces).
        std::vector<Eigen::Vector3d> P = {{0.2, 0.2, 0.2}};
        // Inward normal: average of the two face normals? Use (-1,-1,0) normalized inward.
        Eigen::Vector3d n(-1.0, -1.0, 0.0);
        n.normalize();
        std::vector<Eigen::Vector3d> N = {n};
        int samples = 200;
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, samples, S);
        assert(S.size() == 1);
        // Many rays will hit either the floor (z=0) or the side (x=0) at distance around 0.2.
        // Empirically the average should be close to 0.2 but not exactly. Just check it's positive and < 1.
        assert(S[0] > 0.0 && S[0] < 1.0);
    }

    // Test 3: Box with six faces (unit cube), query at center, all rays hit a face.
    {
        std::vector<Eigen::Vector3d> V;
        V.push_back({0,0,0}); V.push_back({1,0,0}); V.push_back({1,1,0}); V.push_back({0,1,0});
        V.push_back({0,0,1}); V.push_back({1,0,1}); V.push_back({1,1,1}); V.push_back({0,1,1});
        std::vector<Eigen::Vector3i> F = {
            {0,1,2}, {0,2,3}, // bottom
            {4,6,5}, {4,7,6}, // top
            {0,4,5}, {0,5,1}, // front
            {3,2,6}, {3,6,7}, // back
            {0,3,7}, {0,7,4}, // left
            {1,5,6}, {1,6,2}  // right
        };
        std::vector<Eigen::Vector3d> P = {{0.5, 0.5, 0.5}};
        // Any direction is inward if normal is zero? But we set normal to (1,1,1)/sqrt(3) → inward is negative of that.
        Eigen::Vector3d n(1.0, 1.0, 1.0);
        n.normalize();
        std::vector<Eigen::Vector3d> N = {n};
        int samples = 500;
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, samples, S);
        assert(S.size() == 1);
        // Every ray from center hits a face, distance exactly 0.5 in that direction.
        // Average should be exactly 0.5 because all distances are 0.5.
        assert(std::fabs(S[0] - 0.5) < 1e-9);
    }

    // Test 4: Empty output when no points.
    {
        std::vector<Eigen::Vector3d> V = {{0,0,0}, {1,0,0}, {0,1,0}};
        std::vector<Eigen::Vector3i> F = {{0,1,2}};
        std::vector<Eigen::Vector3d> P;
        std::vector<Eigen::Vector3d> N;
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, 10, S);
        assert(S.empty());
    }

    // Test 5: Zero samples → output zeros.
    {
        std::vector<Eigen::Vector3d> V = {{0,0,0}, {1,0,0}, {0,1,0}};
        std::vector<Eigen::Vector3i> F = {{0,1,2}};
        std::vector<Eigen::Vector3d> P = {{0.1,0.1,0.1}};
        std::vector<Eigen::Vector3d> N = {{0,0,1}};
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, 0, S);
        assert(S.size() == 1 && S[0] == 0.0);
    }

    // Test 6: Three points, each receives a distinct but consistent value (just check size).
    {
        std::vector<Eigen::Vector3d> V = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
        std::vector<Eigen::Vector3i> F = {{0,1,2},{0,2,3}};
        std::vector<Eigen::Vector3d> P = {{0.1,0.1,0.1}, {0.5,0.5,0.5}, {0.9,0.9,0.9}};
        std::vector<Eigen::Vector3d> N = {{-1,0,0},{0,-1,0},{0,0,-1}};
        std::vector<double> S;
        raycastAveragedDistance(V, F, P, N, 100, S);
        assert(S.size() == 3);
        // All should be non‑negative.
        for (double v : S) assert(v >= 0.0);
    }

    return 0;
}

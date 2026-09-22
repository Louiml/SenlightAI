// Implement a C++ function `computeIsolines` that, given a 2D triangle mesh with vertex coordinates `V` (an `nV x 2` matrix of doubles, row-major) and triangle indices `F` (an `nF x 3` integer matrix), plus a scalar field `z` (an `nV`-length vector of doubles, one value per vertex), and an integer `n` (number of isoline intervals, so `n+1` isolines including boundary values), produces two outputs: `isoV` (a vector of 2D points) and `isoE` (a vector of 2-element integer pairs representing line segments) that together trace the isolines at levels equally spaced between the min and max of `z`. The function must return these via output parameters. The mesh is assumed to be a valid triangulation (each triangle has vertices indexed 0..nV-1), and the scalar field is defined at vertices only. The output should contain no duplicate vertices (i.e., if two segments share a point, that point appears only once in `isoV`, and `isoE` references the same index). The vertices of each segment are the endpoints of the linear interpolation on the triangle edges where the scalar field crosses the isoline level. For each triangle and each isoline level, a segment is generated for each edge pair that straddles the level (i.e., the isoline crosses the triangle, producing one segment). If an isoline level exactly equals a vertex value, handle it naturally without special-casing (the interpolation formula may produce endpoints at that vertex). The function should be robust to degenerate cases such as `n=0` (then one isoline at min=max, which may produce no segments if the field is constant) or triangles with zero area (but such triangles are not expected; still, the function should not crash). Provide the implementation as a free function with the signature:
//
// ```cpp
// void computeIsolines(
//     const std::vector<std::array<double,2>>& V,
//     const std::vector<std::array<int,3>>& F,
//     const std::vector<double>& z,
//     int n,
//     std::vector<std::array<double,2>>& isoV,
//     std::vector<std::array<int,2>>& isoE
// );
// ```
//
// Use double precision. The function must be self-contained (no external libraries). Include necessary headers and comments.

#include <cassert>
#include <vector>
#include <array>
#include <cmath>

// The solution function declaration is assumed to be available above.

int main() {
    // Test 1: Single triangle, linear field from 0 to 1, n=1 => one isoline at 0.5
    {
        std::vector<std::array<double,2>> V = {{0,0}, {1,0}, {0,1}};
        std::vector<std::array<int,3>> F = {{0,1,2}};
        std::vector<double> z = {0.0, 1.0, 0.0};
        int n = 1;
        std::vector<std::array<double,2>> isoV;
        std::vector<std::array<int,2>> isoE;
        computeIsolines(V, F, z, n, isoV, isoE);
        // Level 0.5 crosses edges 0-1 and 0-2 (since z0=0, z1=1, z2=0)
        // Edge0 t=0.5 => (0.5,0)
        // Edge2 (v2->v0) z2=0, z0=0 no crossing, wait edge2 is (v2,v0): z2=0,z0=0 no. Edge1 (v1,v2): z1=1,z2=0 => t=(0.5-1)/(0-1)=0.5 => point (0.5,0.5). So two crossings.
        // Expect 2 vertices and 1 edge
        assert(isoV.size() == 2);
        assert(isoE.size() == 1);
        // Check vertices approximately
        assert(std::fabs(isoV[0][0]-0.5) < 1e-9 && std::fabs(isoV[0][1]-0.0) < 1e-9);
        assert(std::fabs(isoV[1][0]-0.5) < 1e-9 && std::fabs(isoV[1][1]-0.5) < 1e-9);
        assert(isoE[0][0] == 0 && isoE[0][1] == 1);
    }

    // Test 2: n=0, constant field => no isolines (no segments)
    {
        std::vector<std::array<double,2>> V = {{0,0}, {1,0}, {0,1}};
        std::vector<std::array<int,3>> F = {{0,1,2}};
        std::vector<double> z = {2.0, 2.0, 2.0};
        int n = 0;
        std::vector<std::array<double,2>> isoV;
        std::vector<std::array<int,2>> isoE;
        computeIsolines(V, F, z, n, isoV, isoE);
        assert(isoV.empty() && isoE.empty());
    }

    // Test 3: Two triangles sharing a diagonal, test deduplication
    {
        // Square (0,0)-(1,0)-(1,1)-(0,1) with triangles (0,1,2) and (0,2,3)
        std::vector<std::array<double,2>> V = {{0,0}, {1,0}, {1,1}, {0,1}};
        std::vector<std::array<int,3>> F = {{0,1,2}, {0,2,3}};
        // z = x coordinate (so bilinear? actually linear interpolation on each triangle)
        std::vector<double> z = {0.0, 1.0, 1.0, 0.0};
        int n = 2; // levels at 0, 0.5, 1
        std::vector<std::array<double,2>> isoV;
        std::vector<std::array<int,2>> isoE;
        computeIsolines(V, F, z, n, isoV, isoE);
        // We expect a line at x=0.5 across the square, and also at x=1? Actually at x=0 (z=0) the edges with z0=0 are degenerate? Let's manually compute:
        // Level 0: on triangular mesh, crossing edges where one endpoint has z=0 and other has >0? Since min=0, max=1, level 0 is min. On edge (0,1) z0=0,z1=1 => t=0 => point (0,0) (vertex 0). Edge (1,2) z1=1,z2=1 no. Edge (2,0) z2=1,z0=0 => t=(0-1)/(0-1)=1 => point (0,0) same. For second triangle (0,2,3): edge (0,2) z0=0,z2=1 => t=0 => (0,0); edge (2,3) z2=1,z3=0 => t=(0-1)/(0-1)=1 => (0,1); edge (3,0) z3=0,z0=0 no. So two distinct points (0,0) and (0,1) => segment. Similarly level 0.5 gives a segment at x=0.5, level 1 gives a segment at x=1 (points (1,0)-(1,1)). Total 3 segments, each with 2 unique points, but points shared at corners? The point (0,0) appears in level 0 segment, and (1,1) appears in level 1? Actually level 1: edges where z=1 and z<1? For triangle 0: edge (0,1) t=1 => (1,0); edge (1,2) z1=1,z2=1 no; edge (2,0) t=0 => (1,1). So points (1,0) and (1,1). For triangle 1: edge (0,2) t=1 => (1,1); edge (2,3) t=0 => (1,1) same; edge (3,0) z3=0,z0=0 no. So only one unique crossing? Actually triangle 1 gives only one point (1,1), not two, so no segment because crossings count=1. Wait we need exactly two; for triangle 1 level 1, edge (0,2) gives (1,1), edge (2,3) gives (1,1) duplicate, edge (3,0) no. So only one unique point, so no segment. So total segments: level 0: one segment (0,0)-(0,1). level 0.5: two triangles each give a segment? Triangle 0: edges (0,1) t=0.5 => (0.5,0); (1,2) z1=1,z2=1 no; (2,0) t=0.5 => (0.5,1). So segment. Triangle 1: edge (0,2) z0=0,z2=1 => t=0.5 => (0.5,0.5?) wait (0,0) to (1,1) gives (0.5,0.5); edge (2,3) z2=1,z3=0 => t=0.5 => (0.5,0.5) same; edge (3,0) z3=0,z0=0 no. So only one unique point, no segment. So level 0.5 gives one segment from triangle 0 with points (0.5,0) and (0.5,1). Level 1: triangle 0 gives (1,0)-(1,1) segment. So total 3 segments. Unique vertices: from level0: (0,0),(0,1); level0.5: (0.5,0),(0.5,1); level1: (1,0),(1,1). Total 6 unique points. Check isoV.size()==6 and isoE.size()==3.
        assert(isoV.size() == 6);
        assert(isoE.size() == 3);
        // Ensure no duplicate vertices (check all unique by brute force)
        for (size_t i=0; i<isoV.size(); ++i) {
            for (size_t j=i+1; j<isoV.size(); ++j) {
                assert(!(std::fabs(isoV[i][0]-isoV[j][0])<1e-12 && std::fabs(isoV[i][1]-isoV[j][1])<1e-12));
            }
        }
        // Check edge indices are valid
        for (const auto& e : isoE) {
            assert(e[0] >= 0 && e[0] < (int)isoV.size());
            assert(e[1] >= 0 && e[1] < (int)isoV.size());
            assert(e[0] != e[1]);
        }
    }

    // Test 4: n negative should be treated as 0? Our function returns empty; no crash
    {
        std::vector<std::array<double,2>> V = {{0,0},{1,0},{0,1}};
        std::vector<std::array<int,3>> F = {{0,1,2}};
        std::vector<double> z = {0,1,0};
        std::vector<std::array<double,2>> isoV;
        std::vector<std::array<int,2>> isoE;
        computeIsolines(V,F,z,-5,isoV,isoE);
        // Should not crash, likely empty because n<0 triggers early return? Actually we check n<0 return, so empty.
        assert(isoV.empty() && isoE.empty());
    }

    return 0;
}

#include <vector>
#include <array>
#include <cmath>
#include <unordered_map>
#include <cstddef>

// Helper to hash 2D points for deduplication (using exact double bits)
struct PointHash {
    std::size_t operator()(const std::array<double,2>& p) const {
        std::size_t h1 = std::hash<double>{}(p[0]);
        std::size_t h2 = std::hash<double>{}(p[1]);
        return h1 ^ (h2 + 0x9e3779b9 + (h1<<6) + (h1>>2));
    }
};
struct PointEqual {
    bool operator()(const std::array<double,2>& a, const std::array<double,2>& b) const {
        const double eps = 1e-12;
        return std::fabs(a[0]-b[0]) < eps && std::fabs(a[1]-b[1]) < eps;
    }
};

/**
 * Compute isolines of a scalar field z defined on vertices of a 2D triangle mesh.
 *
 * @param V  Vertex coordinates, size nV x 2
 * @param F  Triangle indices, size nF x 3 (each entry 0..nV-1)
 * @param z  Scalar value per vertex, size nV
 * @param n  Number of intervals (n+1 isoline levels)
 * @param isoV Output: unique 2D points of all segment endpoints
 * @param isoE Output: edges (each pair is a segment index into isoV)
 */
void computeIsolines(
    const std::vector<std::array<double,2>>& V,
    const std::vector<std::array<int,3>>& F,
    const std::vector<double>& z,
    int n,
    std::vector<std::array<double,2>>& isoV,
    std::vector<std::array<int,2>>& isoE
) {
    isoV.clear();
    isoE.clear();
    if (V.empty() || F.empty() || z.empty() || n < 0) return;

    // Determine min and max of z
    double minZ = z[0], maxZ = z[0];
    for (double val : z) {
        if (val < minZ) minZ = val;
        if (val > maxZ) maxZ = val;
    }
    const int levels = n + 1;
    std::vector<double> iso(levels);
    for (int j = 0; j < levels; ++j) {
        iso[j] = (double)j / (double)n * (maxZ - minZ) + minZ;
    }

    // Map from point to index for deduplication
    std::unordered_map<std::array<double,2>, int, PointHash, PointEqual> pointToIndex;
    auto getIndex = [&](const std::array<double,2>& p) -> int {
        auto it = pointToIndex.find(p);
        if (it != pointToIndex.end()) return it->second;
        int idx = (int)isoV.size();
        isoV.push_back(p);
        pointToIndex[p] = idx;
        return idx;
    };

    // For each triangle and each level
    for (const auto& tri : F) {
        // Get vertex coordinates and z values
        std::array<double,2> v0 = V[tri[0]], v1 = V[tri[1]], v2 = V[tri[2]];
        double z0 = z[tri[0]], z1 = z[tri[1]], z2 = z[tri[2]];
        for (double level : iso) {
            // Collect crossing points for the three edges
            std::vector<std::array<double,2>> crossings;
            // Edge 0: (v0,v1)
            {
                double zA = z0, zB = z1;
                auto va = v0, vb = v1;
                double denom = zB - zA;
                if (std::fabs(denom) > 1e-15) {
                    double t = (level - zA) / denom;
                    if (t >= -1e-12 && t <= 1.0 + 1e-12) {
                        // clamp
                        if (t < 0) t = 0; if (t > 1) t = 1;
                        crossings.push_back({va[0] + t*(vb[0]-va[0]), va[1] + t*(vb[1]-va[1])});
                    }
                }
            }
            // Edge 1: (v1,v2)
            {
                double zA = z1, zB = z2;
                auto va = v1, vb = v2;
                double denom = zB - zA;
                if (std::fabs(denom) > 1e-15) {
                    double t = (level - zA) / denom;
                    if (t >= -1e-12 && t <= 1.0 + 1e-12) {
                        if (t < 0) t = 0; if (t > 1) t = 1;
                        crossings.push_back({va[0] + t*(vb[0]-va[0]), va[1] + t*(vb[1]-va[1])});
                    }
                }
            }
            // Edge 2: (v2,v0)
            {
                double zA = z2, zB = z0;
                auto va = v2, vb = v0;
                double denom = zB - zA;
                if (std::fabs(denom) > 1e-15) {
                    double t = (level - zA) / denom;
                    if (t >= -1e-12 && t <= 1.0 + 1e-12) {
                        if (t < 0) t = 0; if (t > 1) t = 1;
                        crossings.push_back({va[0] + t*(vb[0]-va[0]), va[1] + t*(vb[1]-va[1])});
                    }
                }
            }
            // If exactly two crossings exist, add a segment
            if (crossings.size() == 2) {
                // Avoid duplicate segment if the two points are the same (degenerate)
                if (PointEqual()(crossings[0], crossings[1])) continue;
                int idx0 = getIndex(crossings[0]);
                int idx1 = getIndex(crossings[1]);
                isoE.push_back({idx0, idx1});
            }
        }
    }
}

// The approach follows linear interpolation on each triangle edge. For each triangle, consider its three directed edges from vertex k to vertex (k+1)%3. For each isoline level `iso[j]` (j=0..n), compute the interpolation parameter `t = (iso[j] - z1) / (z2 - z1)` where z1 and z2 are the scalar values at the edge’s two endpoints. If `t` is in [0,1], the isoline crosses that edge at the interpolated point. For a given triangle and level, if exactly two edges (out of the three) are crossed, then those two crossing points define a segment. That is the standard marching triangles method. Efficient implementation: precompute for each triangle and each of the three edges a boolean (or NaN) indicating whether the level crosses that edge. Then for each triangle and level, count crossings; if exactly two, generate a segment. However, the snippet uses a triple loop over edges to generate segments, but the simpler approach is to iterate triangles and for each level check all three edges, collect crossing points, and if exactly two, add a segment. To remove duplicate vertices, we can maintain a hash map from (x,y) to index, and when producing a point, check if it already exists; if yes, reuse the index, else append. Since floating-point equality is unreliable, we use a tolerance (e.g., 1e-12) for deduplication, but for simplicity we can use exact equality because all points are computed from the same coordinates via linear interpolation, and identical points will have identical double values. For robustness, we use a small epsilon in a function to check closeness. Complexity: Let `nF` be number of triangles, `n` be number of intervals. For each triangle and each level (n+1 levels) and each edge (3 edges), we compute `t` and possibly a point, so O(nF * n) time. Deduplication uses a hash map, so O(1) amortized per point. Space O(nF * n) for output in worst case (each triangle can contribute up to n+1 segments). Edge cases: `n=0` gives one level, but if the field is not constant, each triangle may have a segment; if constant, no crossings because all z equal, but then min=max so `t` is division by zero? Actually if z1==z2 and iso==z1, then t=0/0 which is NaN. Handle by checking `z2-z1` is not zero, else skip. If an isoline level equals a vertex value exactly, the interpolation may give t=0 or t=1, which is valid, and the crossing point is that vertex. Two adjacent triangles may produce the same vertex, deduplication handles that. For triangles where the scalar field is constant and equal to an isoline, the entire triangle is at that level, but we produce no segment because the crossings are degenerate (all three edges have t=0/0). This is acceptable for isoline tracing. For better behavior, we can skip when z1==z2 and we produce no crossing because the edge does not straddle; we only consider t in [0,1] and finite.

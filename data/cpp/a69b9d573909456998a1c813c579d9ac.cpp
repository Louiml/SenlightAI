// Given a vector of 3D points representing the vertices of a triangle mesh (where each consecutive group of three points forms a triangle), write a C++ function that computes and returns the angle (in degrees, between 0 and 180) between the normals of every pair of adjacent triangles that share a common edge, storing them in a map keyed by the pair of triangle indices (the smaller index first) and the shared edge (represented as a pair of vertex indices in the shared edge, sorted). The input is a vector of `Vec3` structs (with `double x`, `y`, `z`). For each pair of consecutive triangles `(i, i+1)` where `i` starts at 0 and both triangles share exactly two vertices (meaning they are adjacent in the mesh), compute the angle between their face normals, and store it in the map with key `(i, i+1)` and value being a pair `(edge, angle_degrees)`, where `edge` is a pair of the two shared vertex indices sorted in increasing order. Skip any triangles that are degenerate (zero area). If a pair does not share two vertices, skip it. The function should return this map.
#include <cassert>
#include <vector>
#include <map>
#include <cmath>
#include <utility>

// (We assume Vec3 and the function are already defined above.)

int main() {
    // Test 1: Two triangles sharing an edge (square divided into two triangles)
    // Vertices: A=(0,0,0), B=(1,0,0), C=(1,1,0), D=(0,1,0)
    // Triangle0: A,B,C ; Triangle1: A,C,D
    std::vector<Vec3> verts1 = {
        {0,0,0}, {1,0,0}, {1,1,0},
        {0,0,0}, {1,1,0}, {0,1,0}
    };
    auto res1 = computeAdjacentTriangleAngles(verts1);
    assert(res1.size() == 1);
    auto it1 = res1.find({0,1});
    assert(it1 != res1.end());
    // Shared edge should be vertices A(0) and C(2) -> sorted (0,2)
    assert(it1->second.first.first == 0 && it1->second.first.second == 2);
    // Both normals are (0,0,1) so angle = 0
    assert(std::abs(it1->second.second) < 1e-9);

    // Test 2: Two triangles at 90 degrees (like a folded square)
    // Triangle0: (0,0,0), (1,0,0), (1,1,0) normal +Z
    // Triangle1: (0,0,0), (1,1,0), (0,1,1) normal? Compute: e1=(1,1,0), e2=(0,1,1) cross = (1,-1,1) normalized; angle with (0,0,1) = atan? Actually cos = 1/sqrt(3)*1? Let's just test existence.
    // Instead, create two triangles with normals at 90 degrees: 
    // Triangle0: (0,0,0), (1,0,0), (0,1,0) => normal (0,0,1)
    // Triangle1: share edge (0,0,0)-(0,1,0) and third point (0,1,1) => triangle1 vertices (0,0,0), (0,1,0), (0,1,1) => normal = cross((0,1,0),(0,1,1)) = (1,0,0) - wait cross of (0,1,0) and (0,1,1) = (1*1-0*1, 0*0-0*1, 0*1-1*0) = (1,0,0) normalized. Angle between (0,0,1) and (1,0,0) is 90 degrees.
    std::vector<Vec3> verts2 = {
        {0,0,0}, {1,0,0}, {0,1,0},
        {0,0,0}, {0,1,0}, {0,1,1}
    };
    auto res2 = computeAdjacentTriangleAngles(verts2);
    assert(res2.size() == 1);
    auto it2 = res2.find({0,1});
    assert(it2 != res2.end());
    // Shared vertices: (0,0,0) index0, (0,1,0) index2 -> sorted (0,2)
    assert(it2->second.first.first == 0 && it2->second.first.second == 2);
    assert(std::abs(it2->second.second - 90.0) < 1e-6);

    // Test 3: Non-adjacent triangles (only one shared vertex) should be skipped
    std::vector<Vec3> verts3 = {
        {0,0,0}, {1,0,0}, {0,1,0}, // triangle0
        {0,0,0}, {2,0,0}, {0,2,0}  // triangle1 shares only vertex index0
    };
    auto res3 = computeAdjacentTriangleAngles(verts3);
    assert(res3.empty());

    // Test 4: Degenerate triangle (zero area) should be skipped
    std::vector<Vec3> verts4 = {
        {0,0,0}, {1,0,0}, {2,0,0}, // degenerate (collinear)
        {0,0,0}, {1,1,0}, {0,2,0}  // normal triangle sharing edge with degenerate? But degenerate skipped, so no result
    };
    auto res4 = computeAdjacentTriangleAngles(verts4);
    assert(res4.empty());

    // Test 5: Three triangles, check multiple entries
    // Build a strip: t0 (0,0,0),(1,0,0),(1,1,0) ; t1 (0,0,0),(1,1,0),(0,1,0) ; t2 (0,1,0),(1,1,0),(1,1,1)
    // t0 and t1 share edge (0,0,0)-(1,1,0) indices 0 and 2 (sorted 0,2) angle 0
    // t1 and t2 share edge (0,1,0)-(1,1,0) indices? t1 has vertices 0,2,5 (0,0,0),(1,1,0),(0,1,0); t2 has vertices 5,2,7? Wait let's just check count.
    std::vector<Vec3> verts5 = {
        {0,0,0}, {1,0,0}, {1,1,0},   // t0 indices 0,1,2
        {0,0,0}, {1,1,0}, {0,1,0},   // t1 indices 3,4,5
        {0,1,0}, {1,1,0}, {1,1,1}    // t2 indices 6,7,8
    };
    auto res5 = computeAdjacentTriangleAngles(verts5);
    // t0 and t1 share two vertices: (0,0,0) and (1,1,0) -> global indices 0 and 4 (since t1's (1,1,0) is index4). Sorted -> (0,4)
    // t1 and t2 share two vertices: (0,1,0) index5 and (1,1,0) index4 -> sorted (4,5)
    // So map should have 2 entries
    assert(res5.size() == 2);
    auto it5a = res5.find({0,1});
    assert(it5a != res5.end());
    assert(it5a->second.first.first == 0 && it5a->second.first.second == 4);
    // Both normals are +Z? t0 normal (0,0,1), t1 normal (0,0,1) -> angle 0
    assert(std::abs(it5a->second.second) < 1e-9);
    auto it5b = res5.find({1,2});
    assert(it5b != res5.end());
    assert(it5b->second.first.first == 4 && it5b->second.first.second == 5);

    return 0;
}
#include <vector>
#include <map>
#include <cmath>
#include <utility>
#include <algorithm>

struct Vec3 {
    double x, y, z;
};

// Helper: cross product
Vec3 cross(const Vec3& a, const Vec3& b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

// Helper: dot product
double dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Helper: length
double length(const Vec3& v) {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Helper: normalized vector
Vec3 normalize(const Vec3& v) {
    double len = length(v);
    if (len < 1e-12) return {0,0,0};
    return {v.x/len, v.y/len, v.z/len};
}

// Helper: compare two vertices for equality (within epsilon)
bool sameVertex(const Vec3& a, const Vec3& b, double eps = 1e-9) {
    return std::abs(a.x-b.x) < eps && std::abs(a.y-b.y) < eps && std::abs(a.z-b.z) < eps;
}

// Main function: compute angles between normals of adjacent triangles that share an edge.
// Returns map: key = (triangle_index_i, triangle_index_i+1), value = ((vertex_idx1, vertex_idx2), angle_degrees)
// where vertex_idx1 and vertex_idx2 are the global indices of the two shared vertices (sorted).
std::map<std::pair<int,int>, std::pair<std::pair<int,int>, double>>
computeAdjacentTriangleAngles(const std::vector<Vec3>& vertices) {
    std::map<std::pair<int,int>, std::pair<std::pair<int,int>, double>> result;
    int numTriangles = static_cast<int>(vertices.size()) / 3;
    const double eps = 1e-9;

    for (int i = 0; i < numTriangles - 1; ++i) {
        // Triangle i: vertices at positions 3*i, 3*i+1, 3*i+2
        Vec3 vA0 = vertices[3*i], vA1 = vertices[3*i+1], vA2 = vertices[3*i+2];
        // Triangle i+1
        Vec3 vB0 = vertices[3*(i+1)], vB1 = vertices[3*(i+1)+1], vB2 = vertices[3*(i+1)+2];

        // Skip degenerate triangles
        Vec3 eA1 = {vA1.x-vA0.x, vA1.y-vA0.y, vA1.z-vA0.z};
        Vec3 eA2 = {vA2.x-vA0.x, vA2.y-vA0.y, vA2.z-vA0.z};
        Vec3 crossA = cross(eA1, eA2);
        if (length(crossA) < eps) continue;

        Vec3 eB1 = {vB1.x-vB0.x, vB1.y-vB0.y, vB1.z-vB0.z};
        Vec3 eB2 = {vB2.x-vB0.x, vB2.y-vB0.y, vB2.z-vB0.z};
        Vec3 crossB = cross(eB1, eB2);
        if (length(crossB) < eps) continue;

        // Find shared vertices between triangle i and i+1 by comparing coordinates
        int sharedGlobalA[3] = {3*i, 3*i+1, 3*i+2};
        int sharedGlobalB[3] = {3*(i+1), 3*(i+1)+1, 3*(i+1)+2};
        std::vector<int> sharedIndicesGlobal; // global indices of shared vertices
        for (int a = 0; a < 3; ++a) {
            for (int b = 0; b < 3; ++b) {
                if (sameVertex(vertices[sharedGlobalA[a]], vertices[sharedGlobalB[b]], eps)) {
                    // Avoid adding duplicate if the same vertex appears twice (shouldn't happen for valid mesh)
                    bool already = false;
                    for (int s : sharedIndicesGlobal) {
                        if (s == sharedGlobalA[a]) { already = true; break; }
                    }
                    if (!already) {
                        sharedIndicesGlobal.push_back(sharedGlobalA[a]);
                    }
                }
            }
        }

        // We only consider pairs that share exactly two vertices (adjacent triangles sharing an edge)
        if (sharedIndicesGlobal.size() != 2) continue;

        int idx1 = sharedIndicesGlobal[0];
        int idx2 = sharedIndicesGlobal[1];
        if (idx1 > idx2) std::swap(idx1, idx2);

        // Compute face normals
        Vec3 normalA = normalize(cross(eA1, eA2));
        Vec3 normalB = normalize(cross(eB1, eB2));

        double cosAngle = dot(normalA, normalB);
        // Clamp to avoid numerical issues
        if (cosAngle > 1.0) cosAngle = 1.0;
        if (cosAngle < -1.0) cosAngle = -1.0;
        double angleRad = std::acos(cosAngle);
        double angleDeg = angleRad * 180.0 / M_PI;

        result[std::make_pair(i, i+1)] = std::make_pair(std::make_pair(idx1, idx2), angleDeg);
    }

    return result;
}
// For each consecutive pair of triangles `(i, i+1)`, we first check if either triangle is degenerate by checking if the cross product of two edge vectors has length near zero (within a small epsilon like `1e-9`). If either is degenerate, skip. Then extract the three vertices for each triangle from the global vertex vector: triangle `i` uses positions `3*i`, `3*i+1`, `3*i+2` and triangle `i+1` uses `3*(i+1)`, `3*(i+1)+1`, `3*(i+1)+2`. We find shared vertex indices by comparing coordinates exactly (since the problem implies the mesh is shared vertices, but to be safe use a tolerance, say `1e-9`). The shared vertices are global indices into the vertex vector. If the number of shared vertices is exactly two, we compute the face normal for each triangle via cross product of two edge vectors and normalize. Then compute the angle between the two normals: `angle = acos(dot(normalA, normalB))`. Since normals are unit vectors, acos yields a value in `[0, pi]`. Convert to degrees by multiplying by `180/pi`. The key is the pair `(i, i+1)` (since `i < i+1`). The value is a pair `(edge, angle)` where `edge` is a pair of the two shared global vertex indices sorted ascending. Insert or assign into the map. Time complexity is O(n) where n is the number of triangles (we process each adjacent pair once). Space complexity is O(n) for the map containing at most n-1 entries. Edge cases: degenerate triangles, no shared vertices, exactly one shared vertex (skip), all shared three vertices (skip as duplicates). Use `std::map` or `std::unordered_map`; here we use `std::map` to have deterministic ordering of keys.

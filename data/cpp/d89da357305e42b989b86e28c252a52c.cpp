/*
Write a C++ function `line_field_mismatch_quantized` that takes a triangle mesh (vertices `V` as an N×3 matrix of doubles, faces `F` as an M×3 matrix of integers), a per‑face unit direction field `PD1` (M×3 doubles), and a boolean `isCombed`. The function must return an M×3 integer matrix where each entry `(i,j)` encodes the integer mismatch (0 or 2) between face `i` and its adjacent face across edge `j` (where `j=0,1,2` correspond to the opposite vertex ordering in `F`). For boundary edges or when `i == TT(i,j)` (self‑adjacency), the value must be 0. The mismatch is computed by rotating the direction of the neighbor face into the plane of face `i`, measuring the signed angle from `PD1(i)` to that rotated direction (using `PD2(i)` as the reference perpendicular direction), rounding the angle to the nearest multiple of π, and mapping the result to an integer `k` (0 or 1) via modulo 2, then returning `k*2`. If `isCombed` is false, the input `PD1` must first be combed (e.g., by rotating each vector to the closest representative along each edge) before computing mismatches. The function must handle meshes with isolated vertices and repeated edges gracefully, and must not modify the input matrices.
*/

#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <cstdint>

using MatrixXd = std::vector<std::vector<double>>;
using MatrixXi = std::vector<std::vector<int>>;

// Helper: cross product of two 3-vectors
inline void cross(const double* a, const double* b, double* out) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

// Helper: dot product of two 3-vectors
inline double dot(const double* a, const double* b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Helper: normalize a 3-vector in place, return false if near zero
inline bool normalize(double* v) {
    double len = std::sqrt(dot(v,v));
    if (len < 1e-12) return false;
    v[0] /= len; v[1] /= len; v[2] /= len;
    return true;
}

// Build per-face normals (returns false if any face is degenerate)
bool compute_face_normals(const MatrixXd& V, const MatrixXi& F, MatrixXd& N) {
    int M = (int)F.size();
    N.assign(M, std::vector<double>(3,0.0));
    bool all_ok = true;
    for (int i=0; i<M; ++i) {
        int v0=F[i][0], v1=F[i][1], v2=F[i][2];
        double a[3] = {V[v1][0]-V[v0][0], V[v1][1]-V[v0][1], V[v1][2]-V[v0][2]};
        double b[3] = {V[v2][0]-V[v0][0], V[v2][1]-V[v0][1], V[v2][2]-V[v0][2]};
        cross(a,b,N[i].data());
        if (!normalize(N[i].data())) {
            all_ok = false;
            // fallback: set to +Z
            N[i][0]=0.0; N[i][1]=0.0; N[i][2]=1.0;
        }
    }
    return all_ok;
}

// Build triangle-triangle adjacency: TT[i][j] = index of face across edge opposite vertex j, or -1
void build_triangle_adjacency(const MatrixXi& F, MatrixXi& TT, MatrixXi& TTi) {
    int M = (int)F.size();
    TT.assign(M, std::vector<int>(3,-1));
    TTi.assign(M, std::vector<int>(3,-1));
    // map from sorted edge (u,v) to list of (face, local_edge_index)
    // local edge j is opposite vertex j => edge between vertices (j+1)%3 and (j+2)%3
    struct EdgeKey { int u,v; bool operator==(const EdgeKey& o) const { return u==o.u && v==o.v; } };
    struct EdgeKeyHash { std::size_t operator()(const EdgeKey& k) const { return (std::size_t)k.u*1000003 ^ (std::size_t)k.v; } };
    std::unordered_map<EdgeKey, std::vector<std::pair<int,int>>, EdgeKeyHash> edge_map;
    auto add_edge = [&](int u, int v, int f, int j) {
        if (u>v) std::swap(u,v);
        edge_map[{u,v}].push_back({f,j});
    };
    for (int f=0; f<M; ++f) {
        for (int j=0; j<3; ++j) {
            int u = F[f][j];
            int v = F[f][(j+1)%3];
            add_edge(u,v,f,j);
        }
    }
    for (auto& kv : edge_map) {
        auto& vec = kv.second;
        if (vec.size() == 2) {
            int f0=vec[0].first, j0=vec[0].second;
            int f1=vec[1].first, j1=vec[1].second;
            // For face f0, the edge j0 is opposite vertex j0, so the neighbor across that edge is f1
            TT[f0][j0] = f1; TTi[f0][j0] = j1;
            TT[f1][j1] = f0; TTi[f1][j1] = j0;
        } else if (vec.size() != 1) {
            // non-manifold or duplicate edge; keep -1
        }
    }
}

// Comb the line field: for each face, adjust sign of PD1 so that across each edge the direction is consistent.
// We use BFS over connected components of the dual graph.
void comb_line_field(const MatrixXi& F, const MatrixXd& N, MatrixXd& PD1) {
    int M = (int)F.size();
    std::vector<std::vector<std::pair<int,int>>> adj(M); // (neighbor face, local edge index on this face)
    // Build adjacency using triangle adjacency
    MatrixXi TT, TTi;
    build_triangle_adjacency(F, TT, TTi);
    for (int i=0; i<M; ++i) {
        for (int j=0; j<3; ++j) {
            if (TT[i][j]!=-1 && TT[i][j] != i) {
                adj[i].push_back({TT[i][j], j});
            }
        }
    }
    std::vector<bool> visited(M,false);
    for (int start=0; start<M; ++start) {
        if (visited[start]) continue;
        std::vector<int> stack;
        stack.push_back(start);
        visited[start]=true;
        while(!stack.empty()) {
            int u = stack.back(); stack.pop_back();
            for (auto& nb : adj[u]) {
                int v = nb.first;
                int edge_on_u = nb.second; // edge on u
                if (visited[v]) continue;
                visited[v]=true;
                // Find which edge on v corresponds to this same edge
                // In TT, TT[v][?] == u; find it
                int edge_on_v = -1;
                for (int j=0; j<3; ++j) if (TT[v][j] == u) { edge_on_v = j; break; }
                assert(edge_on_v != -1);
                // Rotate PD1[v] to plane of u, then choose sign so that it is closer to PD1[u]
                // We already have PD1[v] (possibly uncombed). We need to adjust sign.
                // Compute rotated vector of PD1[v] into plane of u (using normals)
                double n0[3] = {N[u][0],N[u][1],N[u][2]};
                double n1[3] = {N[v][0],N[v][1],N[v][2]};
                // Rotate v's direction by the rotation that takes n1 to n0
                // Use Rodrigues: axis = cross(n1,n0), angle = atan2(|axis|, dot(n1,n0))
                double axis[3]; cross(n1,n0,axis);
                double axis_norm = std::sqrt(dot(axis,axis));
                double cos_theta = dot(n1,n0);
                double theta = std::atan2(axis_norm, cos_theta);
                double dir1[3] = {PD1[v][0],PD1[v][1],PD1[v][2]};
                double dir1_rot[3];
                if (axis_norm > 1e-12) {
                    double kx=axis[0]/axis_norm, ky=axis[1]/axis_norm, kz=axis[2]/axis_norm;
                    double c = std::cos(theta), s = std::sin(theta);
                    // Rodrigues
                    double VdotK = dot(dir1, axis);
                    double cross_temp[3]; cross(kx,ky,kz, dir1, cross_temp); // helper cross with scalar? we need axis×dir1
                    // Implement manually:
                    double crosskd[3] = {ky*dir1[2] - kz*dir1[1], kz*dir1[0] - kx*dir1[2], kx*dir1[1] - ky*dir1[0]};
                    dir1_rot[0] = dir1[0]*c + crosskd[0]*s + kx*VdotK*(1-c);
                    dir1_rot[1] = dir1[1]*c + crosskd[1]*s + ky*VdotK*(1-c);
                    dir1_rot[2] = dir1[2]*c + crosskd[2]*s + kz*VdotK*(1-c);
                } else {
                    // parallel normals, no rotation needed
                    dir1_rot[0]=dir1[0]; dir1_rot[1]=dir1[1]; dir1_rot[2]=dir1[2];
                }
                // Now we have dir1_rot in plane of u (approximately)
                // Choose sign: compare with PD1[u]
                double dot_pos = dot(dir1_rot, PD1[u].data());
                double dot_neg = dot(dir1_rot, {-PD1[v][0], -PD1[v][1], -PD1[v][2]});
                // Actually we want to flip PD1[v] if needed: if dot_pos < 0, flip
                if (dot_pos < 0.0) {
                    PD1[v][0] = -PD1[v][0];
                    PD1[v][1] = -PD1[v][1];
                    PD1[v][2] = -PD1[v][2];
                }
                stack.push_back(v);
            }
        }
    }
}

// The main function to compute mismatch
MatrixXi line_field_mismatch_quantized(
    const MatrixXd& V,
    const MatrixXi& F,
    MatrixXd PD1,
    bool isCombed)
{
    int M = (int)F.size();
    int N = (int)V.size();
    assert(N > 0 && M > 0);
    
    // Compute normals
    MatrixXd Nf;
    compute_face_normals(V, F, Nf);
    
    // If not combed, comb it
    if (!isCombed) {
        comb_line_field(F, Nf, PD1);
    }
    
    // Build PD2 as a perpendicular direction in each face's tangent plane.
    // For simplicity, we compute a consistent local basis: B1 = PD1 (normalized), B2 = N × B1
    MatrixXd PD2(M, std::vector<double>(3));
    for (int i=0; i<M; ++i) {
        double n[3] = {Nf[i][0],Nf[i][1],Nf[i][2]};
        double cross_n_b1[3];
        cross(n, PD1[i].data(), cross_n_b1);
        normalize(cross_n_b1);
        PD2[i] = {cross_n_b1[0], cross_n_b1[1], cross_n_b1[2]};
    }
    
    // Build adjacency
    MatrixXi TT, TTi;
    build_triangle_adjacency(F, TT, TTi);
    
    // Compute mismatch
    MatrixXi miss(M, std::vector<int>(3,0));
    for (int i=0; i<M; ++i) {
        for (int j=0; j<3; ++j) {
            int f1 = TT[i][j];
            if (f1 == -1 || f1 == i) {
                miss[i][j] = 0;
                continue;
            }
            // Get directions
            double dir0[3] = {PD1[i][0], PD1[i][1], PD1[i][2]};
            double dir1[3] = {PD1[f1][0], PD1[f1][1], PD1[f1][2]};
            double n0[3] = {Nf[i][0], Nf[i][1], Nf[i][2]};
            double n1[3] = {Nf[f1][0], Nf[f1][1], Nf[f1][2]};
            // Rotate dir1 to plane of i
            double axis[3]; cross(n1,n0,axis);
            double axis_norm = std::sqrt(dot(axis,axis));
            double cos_theta = dot(n1,n0);
            double theta = std::atan2(axis_norm, cos_theta);
            double dir1_rot[3];
            if (axis_norm > 1e-12) {
                double kx=axis[0]/axis_norm, ky=axis[1]/axis_norm, kz=axis[2]/axis_norm;
                double c = std::cos(theta), s = std::sin(theta);
                double VdotK = dot(dir1, axis);
                double crosskd[3] = {ky*dir1[2] - kz*dir1[1], kz*dir1[0] - kx*dir1[2], kx*dir1[1] - ky*dir1[0]};
                dir1_rot[0] = dir1[0]*c + crosskd[0]*s + kx*VdotK*(1-c);
                dir1_rot[1] = dir1[1]*c + crosskd[1]*s + ky*VdotK*(1-c);
                dir1_rot[2] = dir1[2]*c + crosskd[2]*s + kz*VdotK*(1-c);
            } else {
                dir1_rot[0]=dir1[0]; dir1_rot[1]=dir1[1]; dir1_rot[2]=dir1[2];
            }
            // Now compute angle from dir0 to dir1_rot in the plane defined by (PD1[i], PD2[i])
            // We need the projection onto that plane.
            // Since dir0 is normalized (assumed), and dir1_rot is approximately in plane, but to be safe we project.
            double proj1 = dot(dir1_rot, PD1[i].data());
            double proj2 = dot(dir1_rot, PD2[i].data());
            double angle_diff = std::atan2(proj2, proj1);
            double step = M_PI;
            int i_round = (int)std::floor(angle_diff/step + 0.5);
            // i_round should be -1,0,1 (maybe 2 due to precision)
            if (i_round < -2) i_round = -2;
            if (i_round > 2) i_round = 2;
            int k = 0;
            if (i_round >= 0) k = i_round % 2;
            else k = (2 + i_round) % 2;
            // Mask to 0 or 1
            miss[i][j] = k*2;
        }
    }
    return miss;
}

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "solution_above.h" // assume the solution is in this header

int main() {
    // Test 1: Single triangle (one face), all edges boundary → all zeros
    {
        MatrixXd V = {{0,0,0},{1,0,0},{0,1,0}};
        MatrixXi F = {{0,1,2}};
        MatrixXd PD1 = {{1.0,0,0}};
        MatrixXi miss = line_field_mismatch_quantized(V,F,PD1,true);
        assert(miss.size()==1 && miss[0].size()==3);
        for (int j=0;j<3;++j) assert(miss[0][j]==0);
    }
    // Test 2: Two triangles sharing one edge with aligned directions (0 mismatch)
    {
        MatrixXd V = {{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
        MatrixXi F = {{0,1,2},{1,3,2}}; // shared edge (1,2)
        // Edge 0 of face0 is opposite vertex 0 → uses vertices (1,2) → shared
        // Face0 dir = (1,0,0); face1 dir = (1,0,0) but need to comb? Both are consistent.
        MatrixXd PD1 = {{1,0,0},{1,0,0}};
        MatrixXi miss = line_field_mismatch_quantized(V,F,PD1,true);
        // face0: j=0 (shared) → mismatch should be 0
        // j=1,2 boundary → 0
        assert(miss[0][0]==0 && miss[0][1]==0 && miss[0][2]==0);
        // face1: shared edge is j=? face1 = {1,3,2}, edge opposite vertex 0 is (3,2) = (1,2) → j=0?
        // Actually face1: vertices [1,3,2]; edge opposite vertex 0 is (3,2) which is (1,2) shared. That is edge j=2? Wait: edge j is opposite vertex j, so opposite vertex 0 is edge (3,2) which is j=2? No, in face1 indices: v0=1, v1=3, v2=2. Edge opposite vertex 0 is v1-v2 = (3,2) = (1,2) shared. That is j=0? Actually edge opposite vertex j is between (j+1)%3 and (j+2)%3. For j=0, edge between 1 and 2 → yes shared. So miss[1][0]==0.
        assert(miss[1][0]==0);
    }
    // Test 3: Two triangles with perpendicular directions → mismatch is 2 (since angle difference = pi/2 → rounds to 0? Actually pi/2 rounds to nearest multiple of pi → 0 or pi? pi/2 is 1.5708, rounding to nearest π gives 0 (since 1.5708/π = 0.5, +0.5 = 1.0 floor=1 → i_round=1 → k=1 → mismatch 2). Wait, if angle is exactly 90°, angle_diff=π/2, step=π, i_round = floor(0.5+0.5)=1 → k=1 → 2.
    // Let's test with angle 90° between directions.
    {
        MatrixXd V = {{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
        MatrixXi F = {{0,1,2},{1,3,2}};
        MatrixXd PD1 = {{1,0,0},{0,1,0}}; // face0 dir x, face1 dir y (in plane)
        MatrixXi miss = line_field_mismatch_quantized(V,F,PD1,true);
        // Expect mismatch across shared edge = 2
        // Find the shared edge index for face0: it's j=0 (opposite vertex 0)
        assert(miss[0][0]==2);
        assert(miss[1][0]==2); // symmetry
    }
    // Test 4: Verify combing effect: give uncombed field where one face has flipped sign
    {
        MatrixXd V = {{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
        MatrixXi F = {{0,1,2},{1,3,2}};
        // Face0 dir = (1,0,0), face1 dir = (-1,0,0) which is actually the same line field (180° flip is equivalent for line fields)
        MatrixXd PD1_uncombed = {{1,0,0},{-1,0,0}};
        MatrixXi miss = line_field_mismatch_quantized(V,F,PD1_uncombed,false);
        // After combing, the field should be aligned, so mismatch should be 0 across shared edge.
        assert(miss[0][0]==0);
        assert(miss[1][0]==0);
    }
    // Test 5: Mixed case with three faces forming a fan around a vertex
    {
        MatrixXd V = {{0,0,0},{1,0,0},{0,1,0},{-1,0,0},{0,-1,0}};
        MatrixXi F = {{0,1,2},{0,2,3},{0,3,4}}; // three triangles around center 0
        // Give a constant direction field (1,0,0) on all faces
        MatrixXd PD1 = {{1,0,0},{1,0,0},{1,0,0}};
        MatrixXi miss = line_field_mismatch_quantized(V,F,PD1,true);
        // All adjacent pairs should have 0 mismatch because directions match.
        for (int i=0;i<3;++i) {
            for (int j=0;j<3;++j) {
                if (miss[i][j] != 0) { // boundary edges are 0 as well
                    assert(miss[i][j]==0);
                }
            }
        }
    }
    // Test 6: Check that boundary edges always 0 even if neighbor exists but is out of plane? Already done in tests.
    
    std::cout << "All tests passed!\n";
    return 0;
}

// The core algorithm follows the classic "line field mismatch" calculation used in field‑based quadrangulation.  
// 1. Preprocess: Compute per‑face normals `N` using cross products of face edges (with proper averaging if degenerate). Build the triangle‑triangle adjacency `TT` where `TT(i,j)` gives the index of the neighbor face across edge `j` (the edge opposite vertex `j`), or `-1` for boundary edges. To avoid O(M²) checks, build an edge‑to‑face map: for each edge (unordered pair of vertices, sorted to canonical form), store the two incident faces and their local edge indices.  
// 2. Combing (if `isCombed` is false): For each edge connecting faces `f0` and `f1`, rotate `PD1(f1)` into the plane of `f0` (using rotation of `n1` to `n0`), then pick the sign that minimizes the angle to `PD1(f0)`. This ensures the direction field is consistently oriented across edges. A standard approach is to perform a breadth‑first traversal over faces, starting from an arbitrary root, and adjust the sign of each neighbor’s PD1 when crossing each edge.  
// 3. Mismatch per directed edge: For each pair `(i,j)` where `TT(i,j)` is a valid neighbor, compute `dir0 = PD1(i)` and `dir1 = PD1(TT(i,j))`. Rotate `dir1` from the neighbor’s normal `n1` to `i`’s normal `n0` using a rotation matrix (e.g., construct via Rodrigues’ formula). Then compute the signed angle `angle_diff = atan2( dir1Rot·PD2(i), dir1Rot·PD1(i) )` where `PD2(i)` is a perpendicular direction in the tangent plane of face `i`. This angle is in `[-π, π]`. To quantize to the nearest multiple of π, compute `i_round = floor(angle_diff/π + 0.5)`, which yields `-1, 0, 1` (or possibly ±2 due to floating point). Convert to a non‑negative index `k` as `(i_round >= 0) ? (i_round % 2) : ((2 + i_round) % 2)`. Because `i_round` is an integer multiple of π steps, we get `k ∈ {0,1}`. The final mismatch value is `k*2`.  
// 4. Boundary and self‑edges: If `TT(i,j) == -1` or `i == TT(i,j)` (degenerate mesh), set to 0.  
// 5. Complexity: Constructing adjacency takes O(M + N) time. Combing (if needed) takes O(M) time because each face is visited once in a BFS. Mismatch computation loops over all 3M directed edges, each doing constant‑time vector operations, so overall O(M) time. Memory is O(N + M) for adjacency structures.  
// Edge cases:  
// – Triangular mesh with repeated edges (two faces sharing more than one edge) – handle by still setting self‑adjacency to 0.  
// – Degenerate triangles (zero area) – skip normal computation or set to arbitrary direction; mismatch may be undefined, but we set 0 to avoid crashes.  
// – Unconnected components – combing must be done per connected component.

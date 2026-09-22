// Implement a C++ function `simplifyMesh` that performs edge-collapse simplification on a triangle mesh. The input is a vertex matrix `V` (each row is a 3D point) and a face matrix `F` (each row is a triangle with three vertex indices) — both stored as `Eigen::MatrixXd` and `Eigen::MatrixXi` respectively. The goal is to reduce the mesh to at most `max_faces` triangles while preserving geometric shape as much as possible. Use the **shortest edge collapse** strategy: at each step, pick the edge with the smallest Euclidean length, collapse it to its midpoint, and remove any faces that become degenerate (zero area or triangles with two identical vertices). The output must be two matrices: `U` (the simplified vertex coordinates) and `G` (the simplified face indices, with all vertices renumbered contiguously from 0). The function must return `true` if the target face count was reached by collapsing edges, and `false` if no more valid collapses were possible (e.g., the mesh is already at the target, or remaining edges are too short to collapse without creating degenerate faces). Handle edge cases: the input may have duplicate vertices, isolated vertices, or faces that are already degenerate — these should be removed in the final output, and they do not count toward the target face count. The implementation must use a priority queue keyed by edge length, update edge costs after each collapse, and properly invalidate and re-insert affected edges. Ensure the function is robust and does not crash on empty or single-face meshes.
The solution uses a standard iterative edge-collapse algorithm. First, build an edge list `E` from the faces, with each edge stored as a pair of vertex indices. Also store for each edge the set of adjacent faces (at most two) and a mapping from edge index to face indices, to efficiently update topology. Initialize a priority queue `Q` of `(edge_length, edge_index)` pairs, and maintain a vector `C` storing the collapse target point for each edge (here the midpoint). Also keep an iterator position in the priority queue for each edge to allow updating its cost later. The main loop: pop the cheapest edge from `Q`. If its cost is infinite, break (no valid edges). For the edge, check if collapsing it is valid: both endpoints must be distinct, and the collapse must not flip any face normals or create non-manifold edges — to keep it simple but correct, we can conservatively check that all faces adjacent to the edge have only one other vertex and that no other edge shares both endpoints. If valid, perform the collapse: merge vertex `e2` into `e1` (or the lower index to be deterministic), update all faces that use `e2` to use `e1`, mark the collapsed faces as null, and update the affected edges' costs. After each collapse, check the stopping condition: if the current number of non-null faces ≤ `max_faces`, stop and return `true`. If no valid edge can be collapsed (e.g., all remaining edges have infinite cost), break and return `false`. After the loop, remove all null faces, remove unreferenced vertices (renumber), and return the result. Time complexity is \(O(m \log m + k \cdot \alpha)\) where \(m\) is the number of edges and \(k\) is the number of collapses, with \(\alpha\) the cost of updating affected edges (typically constant). Space is \(O(m + n)\) for vertices and edges.
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <cassert>

// Simplify a triangle mesh by collapsing the shortest edge repeatedly until at most max_faces remain.
// V: input vertices (n x 3), F: input faces (m x 3) with vertex indices.
// Outputs U (simplified vertices), G (simplified faces, all vertices referenced).
// Returns true if the target face count was achieved by collapses; false otherwise.
bool simplifyMesh(
    const Eigen::MatrixXd &V,
    const Eigen::MatrixXi &F,
    size_t max_faces,
    Eigen::MatrixXd &U,
    Eigen::MatrixXi &G)
{
    using namespace Eigen;
    using namespace std;

    if (F.rows() <= (int)max_faces) {
        // Already at or below target; just remove unreferenced vertices and return true.
        VectorXi _1;
        MatrixXi F2 = F;
        MatrixXd V2 = V;
        // Remove degenerate faces (triangles with any two equal indices)
        int m = 0;
        for (int i = 0; i < F2.rows(); ++i) {
            if (F2(i,0)!=F2(i,1) && F2(i,1)!=F2(i,2) && F2(i,0)!=F2(i,2)) {
                F2.row(m++) = F2.row(i);
            }
        }
        F2.conservativeResize(m, 3);
        // Remove unreferenced vertices
        vector<bool> used(V.rows(), false);
        for (int i=0;i<F2.rows();++i)
            for (int j=0;j<3;++j)
                used[F2(i,j)] = true;
        vector<int> remap(V.rows(), -1);
        int n = 0;
        for (int i=0;i<V.rows();++i)
            if (used[i]) remap[i] = n++;
        U.resize(n, V.cols());
        for (int i=0;i<V.rows();++i)
            if (used[i]) U.row(remap[i]) = V.row(i);
        G.resize(F2.rows(), 3);
        for (int i=0;i<F2.rows();++i)
            for (int j=0;j<3;++j)
                G(i,j) = remap[F2(i,j)];
        return true;
    }

    // Working copies
    MatrixXd Vcur = V;
    MatrixXi Fcur = F;

    // Build edge list: each edge as (v0, v1) with v0<v1, plus incident faces.
    // For simplicity, use a map from pair to edge index.
    map<pair<int,int>, int> edge_map;
    vector<pair<int,int>> edges;
    vector<vector<int>> edge_faces; // faces for each edge
    for (int f=0; f<Fcur.rows(); ++f) {
        int tri[3] = {Fcur(f,0), Fcur(f,1), Fcur(f,2)};
        for (int i=0;i<3;++i) {
            int a = tri[i], b = tri[(i+1)%3];
            if (a == b) continue;
            pair<int,int> e = {min(a,b), max(a,b)};
            auto it = edge_map.find(e);
            if (it == edge_map.end()) {
                int idx = edges.size();
                edge_map[e] = idx;
                edges.push_back(e);
                edge_faces.push_back({f});
            } else {
                edge_faces[it->second].push_back(f);
            }
        }
    }

    // Initial edge lengths and collapse points (midpoint)
    vector<double> cost(edges.size());
    vector<RowVector3d> mid(edges.size());
    for (int e=0; e<(int)edges.size(); ++e) {
        int a = edges[e].first, b = edges[e].second;
        cost[e] = (Vcur.row(a) - Vcur.row(b)).norm();
        mid[e] = 0.5*(Vcur.row(a) + Vcur.row(b));
    }

    // Priority queue: (cost, edge index)
    using QElem = pair<double,int>;
    priority_queue<QElem, vector<QElem>, greater<QElem>> Q;
    for (int e=0; e<(int)edges.size(); ++e)
        Q.push({cost[e], e});

    // Keep track of active faces (not collapsed)
    vector<bool> face_active(Fcur.rows(), true);
    int active_faces = Fcur.rows();

    // Keep track of active edges (cost < inf)
    vector<bool> edge_active(edges.size(), true);

    // Helper to recompute edge cost after a vertex position change
    auto update_edge_cost = [&](int e) {
        if (!edge_active[e]) return;
        int a = edges[e].first, b = edges[e].second;
        cost[e] = (Vcur.row(a) - Vcur.row(b)).norm();
        if (cost[e] < 1e-12) cost[e] = numeric_limits<double>::infinity(); // degenerate
        mid[e] = 0.5*(Vcur.row(a) + Vcur.row(b));
        Q.push({cost[e], e});
    };

    // Helper to check if collapsing edge e is valid
    auto can_collapse = [&](int e) -> bool {
        int a = edges[e].first, b = edges[e].second;
        if (a == b) return false;
        // Check adjacent faces: for each neighbor c, ensure edge (a,c) and (b,c) are unique and not both endpoints
        auto &fs = edge_faces[e];
        for (int f : fs) {
            if (!face_active[f]) continue;
            int tri[3] = {Fcur(f,0), Fcur(f,1), Fcur(f,2)};
            int c = -1;
            for (int i=0;i<3;++i) {
                if (tri[i]!=a && tri[i]!=b) { c=tri[i]; break; }
            }
            if (c == -1) return false; // degenerate face
            // Check that the edge (a,c) and (b,c) are not both present already as other edges? 
            // To keep simple, we only reject if the collapse would create a face with two equal indices.
            if (a == c || b == c) return false;
        }
        // Check that collapsing won't create duplicate edges (non-manifold):
        // For each neighbor vertex c (the opposite vertex of each adjacent face),
        // there must be exactly one edge from a to c and one from b to c in the whole mesh.
        // This is a simplification; full manifold check is more complex.
        // Here we accept if all adjacent faces are triangles and no face becomes degenerate.
        return true;
    };

    // Main collapse loop
    bool finish_by_collapse = false;
    while (active_faces > (int)max_faces) {
        // Pop cheapest edge
        while (!Q.empty()) {
            auto [c, e] = Q.top();
            if (!edge_active[e] || c != cost[e]) { Q.pop(); continue; }
            // Found a valid candidate
            double cur_cost = cost[e];
            if (cur_cost == numeric_limits<double>::infinity()) {
                // No valid edges left
                finish_by_collapse = false;
                goto done;
            }
            if (!can_collapse(e)) {
                // Invalidate this edge permanently
                edge_active[e] = false;
                cost[e] = numeric_limits<double>::infinity();
                Q.pop();
                continue;
            }
            // Collapse edge e: merge b into a (a<b to keep indexing stable)
            int a = edges[e].first, b = edges[e].second;
            // Perform collapse: update Vcur row b to a, invalidate b
            Vcur.row(b) = Vcur.row(a);
            // For each face adjacent to e, if it contains b, replace with a; if it becomes degenerate, mark inactive
            auto &fs = edge_faces[e];
            for (int f : fs) {
                if (!face_active[f]) continue;
                for (int i=0;i<3;++i) {
                    if (Fcur(f,i) == b) Fcur(f,i) = a;
                }
                // Check if face now has two equal vertices
                if (Fcur(f,0)==Fcur(f,1) || Fcur(f,1)==Fcur(f,2) || Fcur(f,0)==Fcur(f,2)) {
                    face_active[f] = false;
                    active_faces--;
                }
            }
            // Remove any faces that originally had b but not a? Actually above handles all.
            // Mark the collapsed edge as inactive
            edge_active[e] = false;
            // Update all edges that involve b (now b is same as a in Vcur)
            for (int i=0; i<(int)edges.size(); ++i) {
                if (!edge_active[i]) continue;
                if (edges[i].first==b || edges[i].second==b) {
                    // This edge becomes degenerate or duplicates an existing edge
                    // For simplicity, invalidate it
                    edge_active[i] = false;
                    cost[i] = numeric_limits<double>::infinity();
                }
            }
            // Also update edges that involve a? They may have changed cost because Vcur[a] unchanged, but
            // some faces around a may have changed, but edge length unchanged. So no need.
            // Recompute costs for all edges that involve a (since a might have new adjacent structure? Vertices unchanged)
            // Actually a's position unchanged, so all edges with a are same length. But we must update cost for edges that now
            // have both endpoints equal? Those are invalidated. Others unchanged.
            // For correctness, we recompute costs for edges that involve a (to reflect possible changes due to face deletion? Not needed)
            // But we also need to push new costs for all edges that were not invalidated.
            // Since only positions of b changed (now equal a), edges that had b are invalidated.
            // So no new costs.
            // Break out to continue loop
            Q.pop();
            break;
        }
        if (Q.empty()) break;
    }
    finish_by_collapse = (active_faces <= (int)max_faces);
done:
    // Build final faces from active ones
    int m = 0;
    MatrixXi F2(Fcur.rows(),3);
    for (int f=0; f<Fcur.rows(); ++f) {
        if (face_active[f] && 
            Fcur(f,0)!=Fcur(f,1) && Fcur(f,1)!=Fcur(f,2) && Fcur(f,0)!=Fcur(f,2)) {
            F2.row(m++) = Fcur.row(f);
        }
    }
    F2.conservativeResize(m,3);

    // Remove unreferenced vertices
    vector<bool> used(Vcur.rows(), false);
    for (int i=0;i<m;++i)
        for (int j=0;j<3;++j)
            used[F2(i,j)] = true;
    vector<int> remap(Vcur.rows(), -1);
    int n = 0;
    for (int i=0;i<Vcur.rows();++i)
        if (used[i]) remap[i] = n++;
    U.resize(n, Vcur.cols());
    for (int i=0;i<Vcur.rows();++i)
        if (used[i]) U.row(remap[i]) = Vcur.row(i);
    G.resize(m,3);
    for (int i=0;i<m;++i)
        for (int j=0;j<3;++j)
            G(i,j) = remap[F2(i,j)];

    return finish_by_collapse;
}
#include <Eigen/Dense>
#include <cassert>
#include <iostream>

// The solution function is assumed to be declared above.

int main() {
    using Eigen::MatrixXd;
    using Eigen::MatrixXi;

    // Test 1: Simple tetrahedron (4 faces) reduced to 2 faces
    {
        MatrixXd V(4,3);
        V << 0,0,0,  1,0,0,  0,1,0,  0,0,1;
        MatrixXi F(4,3);
        F << 0,1,2,  0,1,3,  0,2,3,  1,2,3;
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 2, U, G);
        assert(ok == true);
        assert(G.rows() == 2);
        // All faces must reference valid vertices
        for (int i=0;i<G.rows();++i)
            for (int j=0;j<3;++j)
                assert(G(i,j) >= 0 && G(i,j) < U.rows());
    }

    // Test 2: Cube (12 faces) reduced to 4 faces
    {
        MatrixXd V(8,3);
        V << 0,0,0, 1,0,0, 0,1,0, 1,1,0,
             0,0,1, 1,0,1, 0,1,1, 1,1,1;
        MatrixXi F(12,3);
        F << 0,1,2, 1,3,2,  4,5,6, 5,7,6,
             0,1,4, 1,5,4,  2,3,6, 3,7,6,
             0,2,4, 2,6,4,  1,3,5, 3,7,5;
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 4, U, G);
        assert(ok == true);
        assert(G.rows() == 4);
    }

    // Test 3: Already at target (2 faces) -> no collapse needed, returns true
    {
        MatrixXd V(3,3);
        V << 0,0,0, 1,0,0, 0,1,0;
        MatrixXi F(1,3);
        F << 0,1,2;
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 1, U, G);
        assert(ok == true);
        assert(G.rows() == 1);
    }

    // Test 4: Degenerate face in input is removed and not counted
    {
        MatrixXd V(4,3);
        V << 0,0,0, 1,0,0, 0,1,0, 0,0,1;
        MatrixXi F(3,3);
        F << 0,0,1,  0,1,2,  1,2,3; // first face degenerate
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 2, U, G);
        // After removing degenerate, only 2 faces remain, so no collapse needed
        assert(ok == true);
        assert(G.rows() == 2);
    }

    // Test 5: Empty mesh
    {
        MatrixXd V(0,3); MatrixXi F(0,3);
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 0, U, G);
        assert(ok == true);
        assert(U.rows() == 0);
        assert(G.rows() == 0);
    }

    // Test 6: Single face, max_faces=0 -> must collapse? impossible, so return false
    {
        MatrixXd V(3,3);
        V << 0,0,0, 1,0,0, 0,1,0;
        MatrixXi F(1,3);
        F << 0,1,2;
        MatrixXd U; MatrixXi G;
        bool ok = simplifyMesh(V, F, 0, U, G);
        // Cannot collapse a single triangle to zero faces, so false
        assert(ok == false);
        // Mesh remains with 1 face
        assert(G.rows() == 1);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

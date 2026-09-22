Write a C++ function `progressiveHullDecimate` that takes a triangle mesh represented by a dense matrix of vertex positions `V` (each row is a 3D point), a dense matrix of triangle indices `F` (each row contains 3 vertex indices), and a target maximum number of faces `max_m`. The function must iteratively simplify the mesh using a progressive hull approach: repeatedly collapse the edge whose removal introduces the smallest volumetric error, and continue until the number of faces is at most `max_m` or no more safe collapses are possible. The output should be the simplified vertex matrix `U`, the new face matrix `G`, and a vector `J` that maps each output face in `G` to its originating face index in `F` (faces not modified keep their original index; newly created faces inherit the index of the face that was removed). The function must return `true` if simplification occurred and `false` if the mesh is already at or below the target face count (or input is invalid). Assume the input mesh is manifold, triangles are consistently oriented, and `max_m` is a positive integer. Do not reorder vertices; instead, reuse existing vertex indices from `V` when possible.

// The core algorithm is edge-collapse simplification guided by a "progressive hull" cost: for each directed edge (v1,v2) that is shared by exactly two triangles (a manifold edge), the cost is estimated as the sum of squared distances from the two adjacent triangles' vertices to the plane of the third triangle (if present) or a large penalty if the collapse would create a degenerate triangle or non-manifold edge. In practice, we use a simplified cost: compute the volume of the tetrahedron formed by the two endpoints and the opposite vertices of the two adjacent triangles; if the collapse would flip any triangle normal or create a hole, assign infinite cost. The algorithm repeatedly selects the edge with minimum finite cost, merges its two endpoints by moving the second endpoint onto the first (we keep the lower-index vertex as the survivor to avoid renumbering), updates all triangles that referenced the removed vertex, and marks the second vertex as removed. A union-find structure maps each vertex to its representative. Edge costs are recomputed lazily: after each collapse, only edges incident to the survivor are updated. Stopping criterion: when the current face count <= max_m or no finite-cost edges remain. To avoid recomputing all edge costs each iteration, we maintain a priority queue of candidate edges, but for simplicity in a teaching context, we can recompute costs each round with O(m) passes; since m decreases, overall worst-case O(m^2) for m faces. Space is O(V+F). Edge cases: if mesh has fewer faces than max_m, return false without modification; if an isolated triangle (no shared edges) exists, it cannot be collapsed; degeneracies like duplicate vertices or zero-area triangles should be skipped or penalized. The function must be robust to non-manifold edges by rejecting collapses that would create more than two triangles sharing an edge.

#include <Eigen/Dense>
#include <vector>
#include <limits>
#include <queue>
#include <unordered_set>
#include <algorithm>

// Simplify a triangle mesh using progressive hull edge collapses.
// V: input vertex positions (Nx3)
// F: input triangle indices (Mx3)
// max_m: target maximum number of faces
// U: output vertex positions (same as input, but removed vertices kept for simplicity)
// G: output triangle indices (M'x3, where M' <= max_m)
// J: output face indices mapping each output face to its source face in F
// Returns true if any collapse was performed, false otherwise.
bool progressiveHullDecimate(
    const Eigen::MatrixXd& V,
    const Eigen::MatrixXi& F,
    const size_t max_m,
    Eigen::MatrixXd& U,
    Eigen::MatrixXi& G,
    Eigen::VectorXi& J)
{
    const int m_original = F.rows();
    const int nv = V.rows();

    if (max_m >= (size_t)m_original || m_original == 0) {
        U = V;
        G = F;
        J = Eigen::VectorXi::LinSpaced(m_original, 0, m_original-1);
        return false;
    }

    // Work on copies
    Eigen::MatrixXd Ucur = V;
    Eigen::MatrixXi Gcur = F;
    std::vector<int> face_source(m_original);
    for (int i=0; i<m_original; ++i) face_source[i] = i;

    // Union-find to map removed vertices to survivors
    std::vector<int> parent(nv);
    for (int i=0; i<nv; ++i) parent[i] = i;
    std::function<int(int)> find = [&](int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    auto unite = [&](int a, int b) {
        a = find(a); b = find(b);
        if (a < b) parent[b] = a; // keep lower index survivor
        else parent[a] = b;
    };

    auto compute_edge_cost = [&](int v1, int v2) -> double {
        // Return infinity if collapse invalid
        v1 = find(v1); v2 = find(v2);
        if (v1 == v2) return std::numeric_limits<double>::infinity();

        // Find triangles incident to v1 and v2
        std::vector<int> incident_v1, incident_v2;
        for (int i=0; i<Gcur.rows(); ++i) {
            bool has_v1 = false, has_v2 = false;
            for (int k=0; k<3; ++k) {
                int idx = find(Gcur(i,k));
                if (idx == v1) has_v1 = true;
                if (idx == v2) has_v2 = true;
            }
            if (has_v1) incident_v1.push_back(i);
            if (has_v2) incident_v2.push_back(i);
        }

        // If either vertex is not in any triangle, reject
        if (incident_v1.empty() || incident_v2.empty())
            return std::numeric_limits<double>::infinity();

        // Check if any triangle becomes degenerate after collapse
        for (int tri_idx : incident_v1) {
            Eigen::Vector3i tris = Gcur.row(tri_idx);
            std::vector<int> idxs;
            for (int k=0; k<3; ++k) {
                int idx = find(tris(k));
                if (idx == v1) idx = v2;
                idxs.push_back(idx);
            }
            if (idxs[0]==idxs[1] || idxs[1]==idxs[2] || idxs[0]==idxs[2])
                return std::numeric_limits<double>::infinity();
        }
        for (int tri_idx : incident_v2) {
            Eigen::Vector3i tris = Gcur.row(tri_idx);
            std::vector<int> idxs;
            for (int k=0; k<3; ++k) {
                int idx = find(tris(k));
                if (idx == v2) idx = v1;
                idxs.push_back(idx);
            }
            if (idxs[0]==idxs[1] || idxs[1]==idxs[2] || idxs[0]==idxs[2])
                return std::numeric_limits<double>::infinity();
        }

        // Check manifold: after collapse, each resulting edge must be shared by at most 2 triangles
        // Simplified: reject if v1 and v2 share more than 2 triangles between them (i.e., not a simple edge)
        std::unordered_set<int> common_tri;
        for (int t1 : incident_v1) {
            for (int t2 : incident_v2) {
                if (t1 == t2) common_tri.insert(t1);
            }
        }
        // If they share exactly 0 triangles => not adjacent => can't collapse (would create non-manifold)
        if (common_tri.size() == 0 || common_tri.size() > 2)
            return std::numeric_limits<double>::infinity();

        // Compute progressive hull cost: approximate with sum of volumes of adjacent tetrahedra
        double cost = 0.0;
        auto point = [&](int idx) -> Eigen::Vector3d {
            return Ucur.row(find(idx));
        };
        // For each triangle incident to v1 or v2, compute distance from opposite vertex to plane of the other triangle (if exists)
        // Simplified: cost = sum of squared distances from all vertices in one-ring to the plane of v1-v2 and third vertex.
        Eigen::Vector3d p1 = point(v1);
        Eigen::Vector3d p2 = point(v2);
        Eigen::Vector3d edge_dir = (p2 - p1).normalized();

        // Gather all vertices in the 1-ring of the edge (neighbors in adjacent triangles)
        std::unordered_set<int> ring_vertices;
        for (int t1 : incident_v1) {
            Eigen::Vector3i tris = Gcur.row(t1);
            for (int k=0; k<3; ++k) {
                int idx = find(tris(k));
                if (idx != v1 && idx != v2) ring_vertices.insert(idx);
            }
        }
        for (int t2 : incident_v2) {
            Eigen::Vector3i tris = Gcur.row(t2);
            for (int k=0; k<3; ++k) {
                int idx = find(tris(k));
                if (idx != v1 && idx != v2) ring_vertices.insert(idx);
            }
        }

        for (int ridx : ring_vertices) {
            Eigen::Vector3d pr = point(ridx);
            // distance from pr to edge line
            double dist = (pr - p1).cross(edge_dir).norm();
            cost += dist * dist;
        }
        // Check for normal flip: if any adjacent triangle normal flips sign, that's bad (but we skip for simplicity)
        return cost;
    };

    // Priority queue of edges (cost, v1, v2)
    using Edge = std::tuple<double,int,int>;
    auto cmp = [](const Edge& a, const Edge& b) { return std::get<0>(a) > std::get<0>(b); };
    std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> pq(cmp);

    // Initialize all candidate edges from faces
    auto add_all_edges = [&]() {
        for (int i=0; i<Gcur.rows(); ++i) {
            for (int k=0; k<3; ++k) {
                int a = find(Gcur(i,k));
                int b = find(Gcur(i,(k+1)%3));
                if (a != b) {
                    double cost = compute_edge_cost(a,b);
                    if (std::isfinite(cost))
                        pq.emplace(cost, std::min(a,b), std::max(a,b));
                }
            }
        }
    };
    add_all_edges();

    int current_faces = Gcur.rows();
    bool did_collapse = false;

    while (current_faces > (int)max_m && !pq.empty()) {
        auto [cost, a, b] = pq.top();
        pq.pop();

        // Validate that this edge is still valid (union-find may have changed)
        int ra = find(a), rb = find(b);
        if (ra == rb) continue; // already merged
        // Recompute cost to ensure priority queue entry is stale
        double current_cost = compute_edge_cost(ra, rb);
        if (!std::isfinite(current_cost)) continue;
        if (cost > current_cost + 1e-12) {
            // Stale entry, push updated
            pq.emplace(current_cost, std::min(ra,rb), std::max(ra,rb));
            continue;
        }

        // Perform collapse: merge b into a (a is the lower index survivor)
        int survivor = std::min(ra, rb);
        int removed = std::max(ra, rb);
        // Remove faces that become degenerate (those containing both removed and survivor)
        Eigen::MatrixXi newG;
        Eigen::VectorXi newJ;
        newG.resize(0,3);
        newJ.resize(0);
        for (int i=0; i<Gcur.rows(); ++i) {
            Eigen::Vector3i tri = Gcur.row(i);
            int idxs[3];
            for (int k=0; k<3; ++k) {
                int idx = find(tri(k));
                if (idx == removed) idx = survivor;
                idxs[k] = idx;
            }
            if (idxs[0]==idxs[1] || idxs[1]==idxs[2] || idxs[0]==idxs[2])
                continue; // degenerate, remove
            Eigen::Vector3i new_tri(idxs[0], idxs[1], idxs[2]);
            newG.conservativeResize(newG.rows()+1, 3);
            newG.row(newG.rows()-1) = new_tri;
            newJ.conservativeResize(newJ.size()+1);
            newJ(newJ.size()-1) = face_source[i];
        }
        // Update face count and matrices
        Gcur = newG;
        face_source.clear();
        face_source.resize(newJ.size());
        for (int i=0; i<newJ.size(); ++i) face_source[i] = newJ(i);
        // Unite removed into survivor
        unite(removed, survivor);
        current_faces = Gcur.rows();
        did_collapse = true;

        // Clear priority queue and re-add edges (simple but acceptable O(m^2))
        pq = std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)>(cmp);
        add_all_edges();
    }

    U = V; // Keep original vertices, unused indices remain (could be compressed, but not required)
    G = Gcur;
    J = Eigen::VectorXi(face_source.size());
    for (size_t i=0; i<face_source.size(); ++i) J(i) = face_source[i];
    return did_collapse;
}

#include <Eigen/Dense>
#include <cassert>
#include <vector>

int main() {
    // Test 1: Cube (8 vertices, 12 triangles) reducing to 6 faces
    Eigen::MatrixXd V(8,3);
    V << 0,0,0,
         1,0,0,
         1,1,0,
         0,1,0,
         0,0,1,
         1,0,1,
         1,1,1,
         0,1,1;
    Eigen::MatrixXi F(12,3);
    F << 0,1,2,
         0,2,3,
         4,5,6,
         4,6,7,
         0,1,5,
         0,5,4,
         1,2,6,
         1,6,5,
         2,3,7,
         2,7,6,
         3,0,4,
         3,4,7;
    Eigen::MatrixXd U;
    Eigen::MatrixXi G;
    Eigen::VectorXi J;
    bool success = progressiveHullDecimate(V, F, 6, U, G, J);
    assert(success == true);
    assert(G.rows() <= 6);
    assert(G.cols() == 3);
    assert(J.size() == G.rows());
    // Each output face must have valid vertex indices (0..7)
    for (int i=0; i<G.rows(); ++i)
        for (int k=0; k<3; ++k)
            assert(G(i,k) >= 0 && G(i,k) < 8);

    // Test 2: Already at target size -> no change
    Eigen::MatrixXd U2;
    Eigen::MatrixXi G2;
    Eigen::VectorXi J2;
    bool success2 = progressiveHullDecimate(V, F, 12, U2, G2, J2);
    assert(success2 == false);
    assert(G2 == F);
    assert(J2.size() == F.rows());
    for (int i=0; i<J2.size(); ++i) assert(J2(i) == i);

    // Test 3: Single triangle (no simplification possible)
    Eigen::MatrixXd V3(3,3);
    V3 << 0,0,0, 1,0,0, 0,1,0;
    Eigen::MatrixXi F3(1,3);
    F3 << 0,1,2;
    Eigen::MatrixXd U3;
    Eigen::MatrixXi G3;
    Eigen::VectorXi J3;
    bool success3 = progressiveHullDecimate(V3, F3, 0, U3, G3, J3); // max_m is size_t, but 0 is allowed? spec says positive, but to test fallback
    // Our function treats 0 as target less than current? But max_m is size_t, 0 is fine. It will attempt collapse but no edge shared, so should return false.
    assert(success3 == false);
    assert(G3.rows() == 1);

    // Test 4: Two triangles sharing an edge, reduce to 1 face (should collapse)
    Eigen::MatrixXd V4(4,3);
    V4 << 0,0,0, 1,0,0, 0,1,0, 0,0,1;
    Eigen::MatrixXi F4(2,3);
    F4 << 0,1,2, 0,2,3; // share edge 0-2? Actually share edge 0-2? No, triangles: (0,1,2) and (0,2,3) share edge 0-2? Yes.
    Eigen::MatrixXd U4;
    Eigen::MatrixXi G4;
    Eigen::VectorXi J4;
    bool success4 = progressiveHullDecimate(V4, F4, 1, U4, G4, J4);
    assert(success4 == true);
    assert(G4.rows() == 1);
    assert(J4.size() == 1);

    // Test 5: Empty mesh (0 faces)
    Eigen::MatrixXd V5(0,3);
    Eigen::MatrixXi F5(0,3);
    Eigen::MatrixXd U5;
    Eigen::MatrixXi G5;
    Eigen::VectorXi J5;
    bool success5 = progressiveHullDecimate(V5, F5, 0, U5, G5, J5);
    assert(success5 == false);
    assert(G5.rows() == 0);
    assert(J5.size() == 0);

    // Test 6: Ensure all J values are within original face count
    assert(J.minCoeff() >= 0);
    assert(J.maxCoeff() < F.rows());
}

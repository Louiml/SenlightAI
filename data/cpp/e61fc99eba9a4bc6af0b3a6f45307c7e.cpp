// Given a triangle mesh represented by vertex matrix `V` (n×3 doubles) and face matrix `F` (m×3 integers), write a C++ function that repeatedly collapses the shortest edge (by Euclidean length) to its midpoint until either no edges remain or the total number of edges reaches half the original count. For each collapse, update the mesh data structures (vertices, faces, edges, edge-to-face adjacency, and edge flaps) using a priority queue of edge costs. The function should return the number of successfully collapsed edges. The mesh may be non-manifold or contain degenerate triangles, and edge collapses that would create non-manifold edges or flip triangle orientations must be skipped. The function must not modify the input matrices but produce the decimated mesh via output references.

The solution uses a priority queue (std::set) ordered by pair(cost, edge_index) to always select the globally shortest edge. For each edge, we precompute the midpoint as the collapse target and the cost as the edge length. The core algorithm:
1. Build edge data structures (E: unique edges, EMAP: mapping from half-edge to unique edge, EF: edge-to-face adjacency, EI: index within each face) using `igl::edge_flaps`.
2. Initialize the priority queue with all edges, storing iterators in a vector for O(log n) updates.
3. While the queue is not empty and we have not collapsed the desired number of edges:
   - Pop the smallest-cost edge.
   - Call `igl::collapse_edge` with the shortest_edge_and_midpoint function. This function attempts the collapse, updates V, F, E, EMAP, EF, EI, and the priority queue automatically (handling deleted edges and re-inserting new edges). If it returns false (e.g., invalid collapse), skip.
   - Count the collapse.
4. The collapse_edge function internally checks manifoldness, orientation, and connectivity constraints, so we do not need to implement those checks manually.

Important edge cases: 
- Edges that become invalid after a collapse (e.g., edges with zero-length or those that would create duplicated faces) are automatically removed from the priority queue by the library.
- The midpoint might coincide with an existing vertex or create non-manifold geometry; `collapse_edge` detects and rejects those.
- When the mesh becomes empty (no vertices or faces), the loop terminates.

Time complexity: Each collapse potentially updates O(degree) edges in the priority queue, each O(log E). With E initial edges, total O(E log E) for building the queue and O(E log E) for all collapses. Space complexity: O(E + V + F) for the edge data structures and priority queue.

#include <igl/collapse_edge.h>
#include <igl/edge_flaps.h>
#include <igl/shortest_edge_and_midpoint.h>
#include <Eigen/Core>
#include <set>
#include <vector>

// Decimates a triangle mesh by collapsing the shortest edge to its midpoint.
// Parameters:
//   V: input vertex matrix (n×3), unchanged by reference
//   F: input face matrix (m×3), unchanged by reference
//   Vout: output vertex matrix (will be resized)
//   Fout: output face matrix (will be resized)
// Returns the number of edges successfully collapsed (target is half the original edge count).
int decimateShortestEdgesToMidpoint(
    const Eigen::MatrixXd& V,
    const Eigen::MatrixXi& F,
    Eigen::MatrixXd& Vout,
    Eigen::MatrixXi& Fout) {
    
    // Work on copies because collapse_edge modifies in place.
    Eigen::MatrixXd V_work = V;
    Eigen::MatrixXi F_work = F;
    
    // Edge data structures.
    Eigen::VectorXi EMAP;
    Eigen::MatrixXi E, EF, EI;
    igl::edge_flaps(F_work, E, EMAP, EF, EI);
    
    // Priority queue: pair (cost, edge_index). Use std::set for simple ordering.
    typedef std::set<std::pair<double, int>> PriorityQueue;
    PriorityQueue Q;
    std::vector<PriorityQueue::iterator> Qit(E.rows());
    
    // Precompute cost and midpoint for each edge.
    Eigen::MatrixXd C(E.rows(), V.cols());
    Eigen::VectorXd costs(E.rows());
    for (int e = 0; e < E.rows(); ++e) {
        double cost = e; // dummy initial cost, will be overwritten by shortest_edge_and_midpoint.
        Eigen::RowVectorXd p(1, V.cols());
        igl::shortest_edge_and_midpoint(e, V_work, F_work, E, EMAP, EF, EI, cost, p);
        C.row(e) = p;
        Qit[e] = Q.insert(std::make_pair(cost, e)).first;
    }
    
    int target_collapses = E.rows() / 2;
    int num_collapsed = 0;
    
    while (!Q.empty() && num_collapsed < target_collapses) {
        // The queue's front is the edge with smallest cost.
        // collapse_edge takes the queue and iterator, updates everything.
        if (!igl::collapse_edge(
                igl::shortest_edge_and_midpoint,
                V_work, F_work, E, EMAP, EF, EI,
                Q, Qit, C)) {
            break; // Invalid collapse (e.g., would create non-manifold), stop.
        }
        ++num_collapsed;
    }
    
    Vout = V_work;
    Fout = F_work;
    return num_collapsed;
}

#include <Eigen/Core>
#include <cassert>
#include <iostream>

// Declaration of the solution function (provided in the solution section).
int decimateShortestEdgesToMidpoint(
    const Eigen::MatrixXd& V,
    const Eigen::MatrixXi& F,
    Eigen::MatrixXd& Vout,
    Eigen::MatrixXi& Fout);

int main() {
    // Test 1: A single triangle – no edges can be collapsed (would produce a degenerate mesh).
    Eigen::MatrixXd V1(3, 3);
    V1 << 0, 0, 0,
          1, 0, 0,
          0, 1, 0;
    Eigen::MatrixXi F1(1, 3);
    F1 << 0, 1, 2;
    Eigen::MatrixXd Vout1;
    Eigen::MatrixXi Fout1;
    int n1 = decimateShortestEdgesToMidpoint(V1, F1, Vout1, Fout1);
    assert(n1 == 0);
    assert(Vout1.rows() == 3);
    assert(Fout1.rows() == 1);

    // Test 2: Two triangles sharing an edge (a square with a diagonal).
    Eigen::MatrixXd V2(4, 3);
    V2 << 0, 0, 0,
          1, 0, 0,
          1, 1, 0,
          0, 1, 0;
    Eigen::MatrixXi F2(2, 3);
    F2 << 0, 1, 2,
          0, 2, 3;
    Eigen::MatrixXd Vout2;
    Eigen::MatrixXi Fout2;
    int n2 = decimateShortestEdgesToMidpoint(V2, F2, Vout2, Fout2);
    // Initially there are 5 edges. Half of 5 is 2, but collapsing 2 edges might fail.
    // At minimum one edge should be collapsible (the diagonal of length sqrt(2) vs side length 1).
    assert(n2 > 0);
    // After at least one collapse, the mesh must have fewer vertices/faces.
    assert(Vout2.rows() < V2.rows() || Fout2.rows() < F2.rows());

    // Test 3: A tetrahedron (4 vertices, 4 faces, 6 edges).
    Eigen::MatrixXd V3(4, 3);
    V3 << 0, 0, 0,
          1, 0, 0,
          0, 1, 0,
          0, 0, 1;
    Eigen::MatrixXi F3(4, 3);
    F3 << 0, 1, 2,
          0, 1, 3,
          0, 2, 3,
          1, 2, 3;
    Eigen::MatrixXd Vout3;
    Eigen::MatrixXi Fout3;
    int n3 = decimateShortestEdgesToMidpoint(V3, F3, Vout3, Fout3);
    // Half of 6 is 3. All edges have length sqrt(2), collapse should succeed without degeneracy.
    assert(n3 == 3);
    assert(Vout3.rows() == 1); // After collapsing 3 edges, we end up with a single vertex.
    assert(Fout3.rows() == 0); // No faces remain.

    // Test 4: Empty mesh (no faces).
    Eigen::MatrixXd V4(0, 3);
    Eigen::MatrixXi F4(0, 3);
    Eigen::MatrixXd Vout4;
    Eigen::MatrixXi Fout4;
    int n4 = decimateShortestEdgesToMidpoint(V4, F4, Vout4, Fout4);
    assert(n4 == 0);
    assert(Vout4.rows() == 0);
    assert(Fout4.rows() == 0);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

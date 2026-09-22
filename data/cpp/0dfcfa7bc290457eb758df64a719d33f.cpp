/*
Write a standalone C++ function `smoothVertexAttributes` that takes a matrix of per-vertex attributes `Ain` (size V×D, where V is the number of vertices and D is the number of attribute dimensions) and a triangle mesh connectivity matrix `F` (size T×3, where each row contains three vertex indices for a triangle). The function must output a new matrix `Aout` of the same dimensions where each vertex's attributes are replaced by the average of the attributes of its neighboring vertices (vertices that share an edge with it), based on the rule that for each triangle, each vertex's neighbors include the other two vertices of that triangle. The averaging must count each neighbor exactly once per shared edge (so if two triangles share an edge, the two endpoints of that edge count as neighbors twice, but the formula in the snippet uses a weight of 2 per triangle per vertex, effectively summing the two opposite vertices' attributes and dividing by the total count of neighbors == 0 should be handled gracefully—though valid meshes will have no isolated vertices. The function must work for any floating-point or integer attribute type (use template) and preserve const correctness. The input `Ain` must not be modified.
*/
#include <vector>
#include <Eigen/Core>

/**
 * Smooth per-vertex attributes by averaging the attributes of neighboring vertices.
 * For each triangle, the two other vertices of that triangle are considered neighbors.
 * The output Aout contains the average of the neighbor attributes for each vertex.
 * Isolated vertices (no incident triangles) keep their original attribute values.
 *
 * @tparam DerivedV Eigen matrix type for vertices (float/double/int)
 * @tparam DerivedF Eigen matrix type for faces (integer)
 * @param Ain Input per-vertex attributes (V x D)
 * @param F Triangle connectivity (T x 3), each row has three vertex indices
 * @param Aout Output per-vertex smoothed attributes (V x D)
 */
template <typename DerivedV, typename DerivedF>
void smoothVertexAttributes(
    const Eigen::PlainObjectBase<DerivedV>& Ain,
    const Eigen::PlainObjectBase<DerivedF>& F,
    Eigen::PlainObjectBase<DerivedV>& Aout)
{
    const int numVertices = static_cast<int>(Ain.rows());
    const int numTriangles = static_cast<int>(F.rows());

    // Initialize output with zeros and denominator with zeros
    Aout = DerivedV::Zero(numVertices, Ain.cols());
    std::vector<double> denominator(numVertices, 0.0);

    // Accumulate sums of neighbor attributes and counts
    for (int i = 0; i < numTriangles; ++i) {
        for (int j = 0; j < 3; ++j) {
            const int j1 = (j + 1) % 3;
            const int j2 = (j + 2) % 3;
            const int vertex = static_cast<int>(F(i, j));
            Aout.row(vertex) += Ain.row(F(i, j1)) + Ain.row(F(i, j2));
            denominator[vertex] += 2.0;
        }
    }

    // Compute averages, handle isolated vertices by copying input
    for (int i = 0; i < numVertices; ++i) {
        if (denominator[i] > 0.0) {
            Aout.row(i) /= denominator[i];
        } else {
            Aout.row(i) = Ain.row(i);
        }
    }
}
#include <Eigen/Core>
#include <cassert>

int main() {
    // Test 1: Single triangle
    Eigen::MatrixXd Ain1(3, 1);
    Ain1 << 1.0, 2.0, 3.0;
    Eigen::MatrixXi F1(1, 3);
    F1 << 0, 1, 2;
    Eigen::MatrixXd Aout1;
    smoothVertexAttributes(Ain1, F1, Aout1);
    assert(Aout1(0,0) == (2.0 + 3.0) / 2.0); // 2.5
    assert(Aout1(1,0) == (1.0 + 3.0) / 2.0); // 2.0
    assert(Aout1(2,0) == (1.0 + 2.0) / 2.0); // 1.5

    // Test 2: Two triangles sharing an edge
    Eigen::MatrixXd Ain2(4, 1);
    Ain2 << 10.0, 20.0, 30.0, 40.0;
    Eigen::MatrixXi F2(2, 3);
    F2 << 0, 1, 2,
          0, 2, 3;
    Eigen::MatrixXd Aout2;
    smoothVertexAttributes(Ain2, F2, Aout2);
    // Vertex 0 has neighbors 1,2 (from tri0) and 2,3 (from tri1) → sum=20+30+30+40=120, denom=4 → 30
    assert(Aout2(0,0) == 30.0);
    // Vertex 1 has neighbors 0,2 (only tri0) → sum=10+30=40, denom=2 → 20
    assert(Aout2(1,0) == 20.0);
    // Vertex 2 has neighbors 0,1 (tri0) and 0,3 (tri1) → sum=10+20+10+40=80, denom=4 → 20
    assert(Aout2(2,0) == 20.0);
    // Vertex 3 has neighbors 0,2 (only tri1) → sum=10+30=40, denom=2 → 20
    assert(Aout2(3,0) == 20.0);

    // Test 3: Multi-dimensional attributes
    Eigen::MatrixXd Ain3(3, 2);
    Ain3 << 1.0, 10.0,
            2.0, 20.0,
            3.0, 30.0;
    Eigen::MatrixXi F3(1, 3);
    F3 << 0, 1, 2;
    Eigen::MatrixXd Aout3;
    smoothVertexAttributes(Ain3, F3, Aout3);
    assert(Aout3(0,0) == (2.0+3.0)/2.0);
    assert(Aout3(0,1) == (20.0+30.0)/2.0);
    assert(Aout3(1,0) == (1.0+3.0)/2.0);
    assert(Aout3(1,1) == (10.0+30.0)/2.0);
    assert(Aout3(2,0) == (1.0+2.0)/2.0);
    assert(Aout3(2,1) == (10.0+20.0)/2.0);

    // Test 4: Isolated vertex (no triangles) keeps its value
    Eigen::MatrixXd Ain4(4, 1);
    Ain4 << 5.0, 10.0, 15.0, 20.0;
    Eigen::MatrixXi F4(1, 3);
    F4 << 0, 1, 2; // vertex 3 isolated
    Eigen::MatrixXd Aout4;
    smoothVertexAttributes(Ain4, F4, Aout4);
    assert(Aout4(3,0) == 20.0);
    // vertex 0 has neighbors 1,2 → average = (10+15)/2 = 12.5
    assert(Aout4(0,0) == 12.5);

    // Test 5: Integer attributes (output becomes double? We'll use double matrix for output)
    Eigen::MatrixXi Ain5(3, 1);
    Ain5 << 1, 2, 3;
    Eigen::MatrixXi F5(1, 3);
    F5 << 0, 1, 2;
    Eigen::MatrixXd Aout5;
    smoothVertexAttributes(Ain5.template cast<double>(), F5, Aout5);
    assert(Aout5(0,0) == 2.5);

    return 0;
}
// The algorithm iterates over every triangle in `F`. For each triangle, for each of its three vertices (indexed by `j`), the two other vertices in that triangle (`j1` and `j2`) are considered neighbors of that vertex. The output `Aout` accumulates the sum of the attributes of these two neighbors into the row corresponding to the vertex. Simultaneously, a denominator array counts how many neighbor contributions each vertex has (each triangle contributes 2 to the denominator of each of its three vertices). After processing all triangles, each row of `Aout` is divided by its denominator to compute the average. Edge cases: if a vertex has no incident triangles (isolated), its denominator remains 0; the snippet would divide by zero, so we should handle that by leaving the output unchanged for such vertices (copy the input attribute). In valid triangle meshes, each vertex has at least one triangle, so this case is rare but safe to handle. The time complexity is O(T + V) because each triangle processes a constant number of operations, and the final division loops over V vertices. Space complexity is O(V) for the denominator array plus the output matrix of size V×D.

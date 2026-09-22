You are given a simplified graph optimization framework inspired by g2o. Write a standalone C++ function named `computeTotalChi2` that takes a vector of edges, where each edge is a struct containing a 2D measurement vector, a 2×2 information matrix, a 2D predicted measurement (computed from the current vertex estimates), and two vertex indices. The function must compute the total chi-squared error over all edges using the standard formula: for each edge, compute the residual \( r = \text{measured} - \text{predicted} \), then accumulate \( r^T \cdot \text{information} \cdot r \). The function should take the vector of edges by const reference and return a double. Ensure the function handles an arbitrary number of edges, including zero edges (return 0.0 in that case), and that it uses appropriate `const` correctness.

// The solution iterates over each edge in the input vector. For each edge, it computes the residual vector (measurement minus prediction) as a 2D Eigen vector. Then it computes the quadratic form \( r^T W r \), where \( W \) is the information matrix. This is done via `r.transpose() * information * r`, which yields a 1×1 matrix; extracting the scalar value with `(0,0)`. The total chi2 is the sum over all edges. Edge cases: an empty vector returns 0.0; the information matrix is assumed symmetric positive definite (as in the original code), but no validation is performed; the input is not modified, hence `const` reference. Time complexity is \( O(n) \) for `n` edges, with constant work per edge (matrix multiplications of fixed 2×2 size). Space complexity is \( O(1) \) auxiliary, excluding the input storage.

#include <vector>
#include <Eigen/Dense>

// Struct representing a graph edge in the simplified optimization problem.
struct GraphEdge {
    int vertexIndex1;
    int vertexIndex2;
    Eigen::Vector2d measured;    // observed measurement
    Eigen::Vector2d predicted;   // model prediction given current estimate
    Eigen::Matrix2d information; // symmetric positive definite weight matrix
};

// Compute the total chi-squared error over all edges.
// chi2 = sum_i (residual_i^T * information_i * residual_i)
// where residual_i = measured_i - predicted_i.
double computeTotalChi2(const std::vector<GraphEdge>& edges) {
    double total = 0.0;
    for (const auto& edge : edges) {
        Eigen::Vector2d residual = edge.measured - edge.predicted;
        // Quadratic form: residual^T * information * residual
        total += (residual.transpose() * edge.information * residual)(0, 0);
    }
    return total;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

// Include the solution function (in a real project, this would be in the same file or header)
// Declaration for testing.
double computeTotalChi2(const std::vector<GraphEdge>& edges);

int main() {
    // Edge with zero residual -> chi2 = 0
    GraphEdge e1;
    e1.measured = Eigen::Vector2d(1.0, 2.0);
    e1.predicted = Eigen::Vector2d(1.0, 2.0);
    e1.information = Eigen::Matrix2d::Identity();
    e1.vertexIndex1 = 0;
    e1.vertexIndex2 = 1;
    std::vector<GraphEdge> edges1 = {e1};
    assert(std::abs(computeTotalChi2(edges1) - 0.0) < 1e-9);

    // Edge with residual (1, -1), identity information -> chi2 = 1^2 + (-1)^2 = 2
    GraphEdge e2;
    e2.measured = Eigen::Vector2d(2.0, 1.0);
    e2.predicted = Eigen::Vector2d(1.0, 2.0);
    e2.information = Eigen::Matrix2d::Identity();
    e2.vertexIndex1 = 0;
    e2.vertexIndex2 = 1;
    assert(std::abs(computeTotalChi2({e2}) - 2.0) < 1e-9);

    // Edge with non-identity information matrix: W = [[2,0],[0,3]], residual (1, -1) -> 2*1 + 3*1 = 5
    GraphEdge e3;
    e3.measured = Eigen::Vector2d(3.0, 1.0);
    e3.predicted = Eigen::Vector2d(2.0, 2.0);
    e3.information = Eigen::Matrix2d();
    e3.information << 2.0, 0.0, 0.0, 3.0;
    e3.vertexIndex1 = 0;
    e3.vertexIndex2 = 2;
    assert(std::abs(computeTotalChi2({e3}) - 5.0) < 1e-9);

    // Multiple edges: e2 (chi2=2) + e3 (chi2=5) + e1 (chi2=0) = 7
    std::vector<GraphEdge> edges4 = {e2, e3, e1};
    assert(std::abs(computeTotalChi2(edges4) - 7.0) < 1e-9);

    // Empty vector -> 0.0
    std::vector<GraphEdge> empty;
    assert(std::abs(computeTotalChi2(empty) - 0.0) < 1e-9);

    // Edge with negative residual components: measured - predicted = (-2, 3), identity -> chi2 = 4+9=13
    GraphEdge e5;
    e5.measured = Eigen::Vector2d(0.0, 5.0);
    e5.predicted = Eigen::Vector2d(2.0, 2.0);
    e5.information = Eigen::Matrix2d::Identity();
    e5.vertexIndex1 = 0;
    e5.vertexIndex2 = 1;
    assert(std::abs(computeTotalChi2({e5}) - 13.0) < 1e-9);

    return 0;
}

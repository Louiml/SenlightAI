/*
Write a C++ function `solve_mesh_smoothing` that takes a mesh with vertices stored as a 2D matrix `V` (each row a 3D point), edges stored as a 2D integer matrix `E` (each row two vertex indices), a target field `G` (one scalar per vertex), a smoothing weight `lambda`, and an integer `mode` (0 or 1). The function must compute and return a smoothed field `U` (one scalar per vertex) by solving the linear system `(M - lambda * L) U = M G`, where `M` is the identity matrix and `L` is the graph Laplacian (mode 0) or the edge-weighted Laplacian (mode 1). For mode 0, `L` has `+1` off-diagonal entries for each edge and diagonal entries equal to the negative row sum. For mode 1, each off-diagonal entry is `1 / ||V(i) - V(j)||` (with a small epsilon of 0.0), and diagonal entries are the negative row sum. Use Eigen's `SimplicialLDLT` solver to solve the sparse system. Ensure the function is `const`-correct and returns the resulting matrix by value.
*/
#include <Eigen/SparseCholesky>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>

// Solves (I - lambda * L) U = G for a mesh Laplacian defined by edges.
// mode 0: graph Laplacian (unit weights).
// mode 1: edge-weighted Laplacian (weight = 1 / edge length).
Eigen::MatrixXd solve_mesh_smoothing(
    const Eigen::MatrixXd & V,
    const Eigen::MatrixXi & E,
    const Eigen::MatrixXd & G,
    double lambda,
    int mode)
{
    int n = V.rows();
    Eigen::SparseMatrix<double> L(n, n);

    // Build off-diagonal entries
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(E.rows() * 2);

    for (int i = 0; i < E.rows(); ++i) {
        int a = E(i, 0);
        int b = E(i, 1);
        if (a < 0 || a >= n || b < 0 || b >= n) continue;

        double weight = 1.0;
        if (mode == 1) {
            double dist = (V.row(a) - V.row(b)).norm();
            if (dist <= 0.0) continue; // skip degenerate edges
            weight = 1.0 / dist;
        }
        triplets.emplace_back(a, b, weight);
        triplets.emplace_back(b, a, weight);
    }

    L.setFromTriplets(triplets.begin(), triplets.end());

    // Set diagonal to negative row sum (Laplacian property)
    for (int i = 0; i < n; ++i) {
        double sum = L.row(i).sum(); // includes off-diagonals only since diagonal is zero
        L.coeffRef(i, i) = -sum;
    }

    // Build system matrix A = I - lambda * L
    Eigen::SparseMatrix<double> A = -lambda * L;
    for (int i = 0; i < n; ++i) {
        A.coeffRef(i, i) += 1.0;
    }

    // Solve (A) U = G
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);
    Eigen::MatrixXd U = solver.solve(G);
    return U;
}
#include <cassert>
#include <Eigen/Dense>
#include <Eigen/Sparse>

int main() {
    // Simple 2-vertex mesh with one edge
    Eigen::MatrixXd V(2, 3);
    V << 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0;
    Eigen::MatrixXi E(1, 2);
    E << 0, 1;

    Eigen::MatrixXd G(2, 1);
    G << 1.0, 0.0;

    // Test mode 0 (graph Laplacian)
    double lambda = 0.5;
    Eigen::MatrixXd U0 = solve_mesh_smoothing(V, E, G, lambda, 0);
    // System: [1 - lambda*1, -lambda*1; -lambda*1, 1 - lambda*1] U = G
    // With lambda=0.5: [0.5, -0.5; -0.5, 0.5] U = [1, 0]
    // Solve: U0 = [0.5, -0.5]? Check by substitution: 0.5*0.5 + (-0.5)*(-0.5)=0.25+0.25=0.5 not 1. Wait correct solve:
    // (0.5)u0 -0.5u1 = 1
    // -0.5u0 +0.5u1 = 0 => u0=u1, then 0.5u0 -0.5u0=0 not 1. Actually system is singular for lambda=1? Let's pick lambda=0.2.
    // Recompute with lambda=0.2: A = [0.8, -0.2; -0.2, 0.8], solve A U = [1,0] => U = [1.25, 0.25]? Let's test via solver.
    lambda = 0.2;
    U0 = solve_mesh_smoothing(V, E, G, lambda, 0);
    // Solve A U = G manually: (0.8)u0 -0.2u1 =1; -0.2u0 +0.8u1=0 => u1 = 0.25u0. Then 0.8u0 -0.2*0.25u0 = 0.75u0 =1 => u0=4/3, u1=1/3.
    assert(std::abs(U0(0,0) - 4.0/3.0) < 1e-6);
    assert(std::abs(U0(1,0) - 1.0/3.0) < 1e-6);

    // Test mode 1 (edge-weighted Laplacian) with distance 1 => same weights
    Eigen::MatrixXd U1 = solve_mesh_smoothing(V, E, G, lambda, 1);
    assert((U1 - U0).norm() < 1e-6);

    // Test triangle mesh: 3 vertices, 3 edges
    Eigen::MatrixXd V3(3, 3);
    V3 << 0.0, 0.0, 0.0,
          1.0, 0.0, 0.0,
          0.0, 1.0, 0.0;
    Eigen::MatrixXi E3(3, 2);
    E3 << 0, 1,
          1, 2,
          2, 0;
    Eigen::MatrixXd G3(3, 1);
    G3 << 1.0, 2.0, 3.0;
    lambda = 0.1;
    Eigen::MatrixXd U3 = solve_mesh_smoothing(V3, E3, G3, lambda, 0);
    // Verify system: (I - 0.1*L) U = G, L is graph Laplacian.
    // For triangle, L diag = 2, off-diag = -1? Wait L off-diag = +1, diag = -row_sum = -2? Actually row sum = +1+1 =2, so diag = -2. So L = [-2,1,1;1,-2,1;1,1,-2]. Then I - 0.1L = [1.2, -0.1, -0.1; -0.1, 1.2, -0.1; -0.1, -0.1, 1.2].
    // Check solution by plugging back manually? Use solver to verify residual.
    Eigen::MatrixXd residual = (Eigen::MatrixXd::Identity(3,3) - 0.1 * ( [&](){ Eigen::SparseMatrix<double> Ltmp(3,3); std::vector<Eigen::Triplet<double>> t; for (int i=0;i<3;++i){int a=E3(i,0),b=E3(i,1); t.emplace_back(a,b,1.0); t.emplace_back(b,a,1.0);} Ltmp.setFromTriplets(t.begin(),t.end()); for (int i=0;i<3;++i){Ltmp.coeffRef(i,i) = -Ltmp.row(i).sum();} return Ltmp; }() ) ) * U3 - G3;
    assert(residual.norm() < 1e-6);

    return 0;
}
// The solution constructs the appropriate sparse Laplacian matrix `L` of size `n x n` where `n = V.rows()`. For mode 0, iterate over all edges and set the two off-diagonal entries to `1.0`. For mode 1, for each edge compute the Euclidean distance between the two vertices; if the distance is greater than a threshold (0.0 here), set the two off-diagonal entries to `1.0 / distance`. After filling off-diagonals, for each row compute the sum of the row and set the diagonal entry to the negative of that sum, ensuring each row sums to zero. Then compute `A = -lambda * L + I` (identity mass matrix). Since `L` is symmetric and positive semi-definite and `lambda > 0`, `A` is symmetric positive definite, so `SimplicialLDLT` is safe. Finally, solve `A U = G` (since mass is identity) and return `U`. Edge cases: if an edge has zero distance (coincident vertices), mode 1 would divide by zero; the threshold check skips such edges. Complexity: constructing `L` takes `O(E + n)` time; solving with LDLT is typically `O(n^3)` in the worst case but is efficient for sparse meshes. Space is `O(E + n)` for the sparse matrices.

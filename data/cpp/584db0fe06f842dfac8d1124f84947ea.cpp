Write a C++ function `buildPoissonMatrixAndRHS` that, given an integer grid size `n` (where the grid has `n` rows and `n` columns, representing an `n x n` image), constructs a sparse Poisson-like system matrix `A` of size `n*n` and a right-hand side vector `b` of length `n*n`. The problem models a 2D Laplace equation with Dirichlet boundary conditions: interior grid points are coupled to their four orthogonal neighbors with coefficient `4` on the diagonal and `-1` on each neighbor (if the neighbor is inside the domain). The right-hand side vector `b` is defined as follows: for every interior point (i,j) with `0 < i < n-1` and `0 < j < n-1`, set `b[i*n + j] = 1.0`; for boundary points, set the corresponding row in `A` to the identity (i.e., diagonal coefficient `1` and no off-diagonal entries), and set `b` to `0.0` at those boundary points. Use `Eigen::SparseMatrix<double>` for the matrix and `Eigen::VectorXd` for the vector. The function must return both the matrix and the vector via output parameters (e.g., `Eigen::SparseMatrix<double>& A` and `Eigen::VectorXd& b`). The function should be self-contained, include necessary Eigen headers, and handle the trivial case `n == 1` (a single boundary point with `A = [1]` and `b = [0]`). The function must not allocate the matrix more than once and should efficiently fill it using `Eigen::Triplet` from a preallocated vector.
// The problem requires assembling a sparse linear system for a 2D Poisson equation with Dirichlet boundary conditions on a regular grid. The key steps are: (1) compute the total number of unknowns as `m = n*n`; (2) preallocate a vector of `Eigen::Triplet<double>` large enough to hold at most `5` nonzeros per interior point and `1` per boundary point. A safe upper bound is `5*m` triplets (since every point has at most one diagonal plus up to four neighbors, and boundary points have only one). (3) Iterate over each grid point `(i,j)` with linear index `idx = i*n + j`. For each point, decide whether it is a boundary or interior. For boundary points, add a triplet `(idx, idx, 1.0)` and set `b[idx] = 0.0`. For interior points, add diagonal `(idx, idx, 4.0)` and for each neighbor inside the grid (up, down, left, right) add `(idx, neighborIdx, -1.0)`. Set `b[idx] = 1.0`. (4) Construct the sparse matrix `A` using `A.setFromTriplets(triplets.begin(), triplets.end())`. The complexity is O(n^2) time and O(n^2) memory, since we visit each point once and each triplet once. Edge cases: `n == 1` (only one boundary point), `n == 0` should not occur but can be handled by an assertion. The function must be `const`-correct for inputs (but here only `n` is an input) and pass matrix/vector by reference to output. To avoid memory reallocation, reserve the triplet vector’s capacity to `5*m` using `reserve`. This approach is robust and matches the snippet’s assembly pattern.
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>
#include <cassert>

// Fill the sparse matrix A and right-hand side vector b for a Poisson problem
// on an n x n grid with Dirichlet boundary conditions (boundary values = 0).
// A is an m x m sparse matrix, b is a length m vector, where m = n * n.
void buildPoissonMatrixAndRHS(int n, Eigen::SparseMatrix<double>& A, Eigen::VectorXd& b) {
    assert(n >= 1);
    const int m = n * n;
    b = Eigen::VectorXd::Zero(m);

    // Preallocate triplet list with an upper bound of 5 nonzeros per row.
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(5 * m);

    // Helper to map (i,j) to linear index.
    auto idx = [n](int i, int j) -> int { return i * n + j; };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int linear = idx(i, j);
            bool is_boundary = (i == 0 || i == n-1 || j == 0 || j == n-1);
            if (is_boundary) {
                // Dirichlet boundary: identity row, zero RHS.
                triplets.emplace_back(linear, linear, 1.0);
                b[linear] = 0.0;
            } else {
                // Interior: second-order central difference (plus 4, minus 1 for neighbors).
                triplets.emplace_back(linear, linear, 4.0);
                // Up neighbor (i-1, j)
                triplets.emplace_back(linear, idx(i-1, j), -1.0);
                // Down neighbor (i+1, j)
                triplets.emplace_back(linear, idx(i+1, j), -1.0);
                // Left neighbor (i, j-1)
                triplets.emplace_back(linear, idx(i, j-1), -1.0);
                // Right neighbor (i, j+1)
                triplets.emplace_back(linear, idx(i, j+1), -1.0);
                b[linear] = 1.0;
            }
        }
    }

    A.resize(m, m);
    A.setFromTriplets(triplets.begin(), triplets.end());
}
#include <cassert>
#include <Eigen/Sparse>
#include <Eigen/Dense>

// Declaration of the function under test (assume it is provided from the header above).
void buildPoissonMatrixAndRHS(int n, Eigen::SparseMatrix<double>& A, Eigen::VectorXd& b);

int main() {
    // Test 1: smallest grid (1x1) – single boundary point.
    {
        int n = 1;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;
        buildPoissonMatrixAndRHS(n, A, b);
        assert(A.rows() == 1 && A.cols() == 1);
        assert(b.size() == 1);
        assert(A.coeff(0,0) == 1.0);
        assert(b[0] == 0.0);
    }

    // Test 2: 2x2 grid – all points are boundaries.
    {
        int n = 2;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;
        buildPoissonMatrixAndRHS(n, A, b);
        assert(A.rows() == 4 && A.cols() == 4);
        assert(b.size() == 4);
        for (int i = 0; i < 4; ++i) {
            assert(b[i] == 0.0);
            // Each row should be the identity row.
            assert(A.coeff(i,i) == 1.0);
            assert(A.row(i).nonZeros() == 1);
        }
    }

    // Test 3: 3x3 grid – one interior point (center).
    {
        int n = 3;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;
        buildPoissonMatrixAndRHS(n, A, b);
        assert(A.rows() == 9 && A.cols() == 9);
        assert(b.size() == 9);
        // Interior point index 4 (i=1,j=1) should have diagonal 4, neighbors -1, RHS 1.
        int center = 4;
        assert(A.coeff(center, center) == 4.0);
        assert(A.coeff(center, center-3) == -1.0); // up
        assert(A.coeff(center, center+3) == -1.0); // down
        assert(A.coeff(center, center-1) == -1.0); // left
        assert(A.coeff(center, center+1) == -1.0); // right
        assert(b[center] == 1.0);
        // Check a boundary point, e.g., (0,0) index 0.
        assert(A.coeff(0,0) == 1.0);
        assert(b[0] == 0.0);
        assert(A.row(0).nonZeros() == 1);
    }

    // Test 4: 4x4 grid – four interior points.
    {
        int n = 4;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;
        buildPoissonMatrixAndRHS(n, A, b);
        assert(A.rows() == 16 && A.cols() == 16);
        // Interior indices: (1,1)=5, (1,2)=6, (2,1)=9, (2,2)=10.
        int interiors[4] = {5,6,9,10};
        for (int idx : interiors) {
            assert(A.coeff(idx, idx) == 4.0);
            assert(b[idx] == 1.0);
            assert(A.row(idx).nonZeros() == 5);
        }
        // Check a boundary point like (0,0)=0 has only diagonal.
        assert(A.row(0).nonZeros() == 1);
        assert(b[0] == 0.0);
    }

    // Test 5: Larger 5x5 grid – verify some properties.
    {
        int n = 5;
        Eigen::SparseMatrix<double> A;
        Eigen::VectorXd b;
        buildPoissonMatrixAndRHS(n, A, b);
        assert(A.rows() == 25 && A.cols() == 25);
        // Total nonzeros expected: 9 boundary points (each 1) + 16 interior (each 5) = 9+80=89.
        // Count actual nonzeros.
        int nnz = 0;
        for (int k = 0; k < A.outerSize(); ++k) {
            for (Eigen::SparseMatrix<double>::InnerIterator it(A, k); it; ++it) {
                ++nnz;
            }
        }
        assert(nnz == 89);
        // Check matrix is symmetric? Spot-check one interior point (2,2) index 12.
        assert(A.coeff(12,12) == 4.0);
        assert(A.coeff(12,7) == -1.0); // up
        assert(A.coeff(12,17) == -1.0); // down
        assert(A.coeff(12,11) == -1.0); // left
        assert(A.coeff(12,13) == -1.0); // right
        // Check symmetry: A(7,12) should also be -1.
        assert(A.coeff(7,12) == -1.0);
    }

    return 0;
}

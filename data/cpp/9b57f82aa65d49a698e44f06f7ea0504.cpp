/*
Write a C++ function `buildPoissonMatrix` that takes a positive integer `n` and returns an Eigen `SparseMatrix<double>` representing the discrete 2D Poisson equation on an `n x n` grid with Dirichlet boundary conditions (zero on all edges). The matrix should be assembled using `Eigen::Triplet<double>` coefficients added to a `std::vector<T>`. The equation is `-Δu = f` discretized with the standard 5-point stencil: the diagonal entry is 4 (for interior points) or 1 (for boundary points, since only one adjacent interior neighbor exists when the point is on the edge but not corner), and off-diagonal entries are -1 for each interior neighbor. For corner points, only one off-diagonal entry (the interior neighbor) exists, but the diagonal remains 1. The function must return the assembled sparse matrix of size `n*n × n*n`, with row-major indexing where index `(i,j)` maps to row `i*n + j`, with `i` as the row index (from 0 to n-1) and `j` as the column index. Handle the edge case `n = 1` (single point, matrix 1×1 with value 1). The function must not use any global state and must be const-correct for its inputs.
*/
#include <Eigen/Sparse>
#include <vector>

typedef Eigen::SparseMatrix<double> SpMat;
typedef Eigen::Triplet<double> T;

// Build the discrete 2D Poisson matrix with Dirichlet boundary conditions (zero on edges).
// The grid is n x n, indexing (i,j) maps to row i*n + j.
// Returns an n*n x n*n sparse matrix.
SpMat buildPoissonMatrix(int n) {
    const int m = n * n;
    std::vector<T> coefficients;
    coefficients.reserve(5 * m); // Each point has at most 5 non-zeros (diagonal + 4 neighbors)

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            const int id = i * n + j;
            const bool interior = (i > 0 && i < n-1 && j > 0 && j < n-1);
            const double diag = interior ? 4.0 : 1.0;
            coefficients.emplace_back(id, id, diag);

            // Check four neighbors
            if (i > 0) coefficients.emplace_back(id, id - n, -1.0);       // up
            if (i < n-1) coefficients.emplace_back(id, id + n, -1.0);     // down
            if (j > 0) coefficients.emplace_back(id, id - 1, -1.0);       // left
            if (j < n-1) coefficients.emplace_back(id, id + 1, -1.0);     // right
        }
    }

    SpMat A(m, m);
    A.setFromTriplets(coefficients.begin(), coefficients.end());
    return A;
}
#include <cassert>
#include <Eigen/Sparse>
#include <vector>

typedef Eigen::SparseMatrix<double> SpMat;
typedef Eigen::Triplet<double> T;

SpMat buildPoissonMatrix(int n);

int main() {
    // n=1: single point, 1x1 matrix with value 1
    {
        SpMat A = buildPoissonMatrix(1);
        assert(A.rows() == 1 && A.cols() == 1);
        assert(A.coeff(0,0) == 1.0);
        assert(A.nonZeros() == 1);
    }

    // n=2: 4x4 matrix, each point is boundary (since n=2, no strictly interior points)
    {
        SpMat A = buildPoissonMatrix(2);
        assert(A.rows() == 4 && A.cols() == 4);
        // Check diagonal values: all are 1 (boundary)
        for (int k = 0; k < 4; ++k) {
            assert(A.coeff(k,k) == 1.0);
        }
        // Each corner has 2 neighbors, each edge (non-corner) has 3 neighbors.
        // Total non-zeros: 4 diagonals + (4 corners * 2) + (0 edge non-corner, since n=2 all are corners) = 4 + 8 = 12
        assert(A.nonZeros() == 12);
        // Check specific off-diagonal for corner (0,0): neighbors (0,1) and (1,0)
        assert(A.coeff(0,1) == -1.0);
        assert(A.coeff(0,2) == -1.0);
        // For point (1,1) (index 3), neighbors: (1,0) index 2, (0,1) index 1
        assert(A.coeff(3,2) == -1.0);
        assert(A.coeff(3,1) == -1.0);
    }

    // n=3: 9x9 matrix, one interior point at (1,1) with diagonal 4
    {
        SpMat A = buildPoissonMatrix(3);
        assert(A.rows() == 9 && A.cols() == 9);
        // Interior point index 1*3+1 = 4
        assert(A.coeff(4,4) == 4.0);
        // Boundary points have diagonal 1
        assert(A.coeff(0,0) == 1.0); // corner (0,0)
        assert(A.coeff(3,3) == 1.0); // edge left (1,0) index 3
        // Check neighbors of interior: up (0,1) index 1, down (2,1) index 7, left (1,0) index 3, right (1,2) index 5
        assert(A.coeff(4,1) == -1.0);
        assert(A.coeff(4,7) == -1.0);
        assert(A.coeff(4,3) == -1.0);
        assert(A.coeff(4,5) == -1.0);
        // Check symmetry for a sample pair
        assert(A.coeff(1,4) == -1.0);
        // Non-zeros count: 9 diagonals + 4 interior*4 + 4 corners*2 + 4 edges(non-corner)*3 = 9 + 16 + 8 + 12 = 45
        assert(A.nonZeros() == 45);
    }

    // n=4: verify row sum for interior point (2,2): diagonal 4 minus 4 neighbors = 0
    {
        SpMat A = buildPoissonMatrix(4);
        int id = 2*4 + 2; // 10
        double row_sum = 0.0;
        for (int k = 0; k < A.outerSize(); ++k) {
            for (SpMat::InnerIterator it(A, id); it; ++it) {
                row_sum += it.value();
            }
        }
        // Note: InnerIterator on a column-major matrix iterates column; but for symmetric matrix, we can use A.row(id)
        // Simpler: directly sum using coeffRef
        double sum = 0.0;
        for (int c = 0; c < 16; ++c) sum += A.coeff(id, c);
        assert(sum == 0.0);
    }

    return 0;
}
// The solution builds a sparse matrix by iterating over all grid points (i,j). For each point, compute its linear index `id = i*n + j`. The diagonal value is 4 if the point is strictly interior (i>0, i<n-1, j>0, j<n-1), otherwise it is 1 (boundary point). For each of the four possible neighbors (up, down, left, right), if the neighbor is inside the grid, add a triplet with value -1 at (id, neighbor_id). This automatically handles corners: e.g., corner (0,0) has only two neighbors (0,1) and (1,0), but both are interior, so two -1 off-diagonals appear; diagonal is 1. For n=1, the single point has no neighbors, diagonal is 1, and the matrix is 1×1. The assembly uses `setFromTriplets` which sums any duplicate entries, but since each adjacent pair adds exactly one -1 per direction, no duplicates occur. The complexity is O(n²) time and O(n²) memory for the triplets, but the sparse matrix itself has O(n²) non-zeros (about 5 per row), so the final storage is O(n²).

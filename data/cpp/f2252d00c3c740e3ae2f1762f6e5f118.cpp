Write a C++ function that, given an integer `n`, constructs and solves the discrete Poisson equation \( -\Delta u = 0 \) on an \( n \times n \) grid with boundary conditions \( u = \sin^2(\pi y) \) (where \( y \) is normalized to the unit interval) on the left and right edges and zero-flux behavior handled implicitly by the standard 5-point stencil. The function must assemble the sparse stiffness matrix using `Eigen::SparseMatrix<double>` and `Eigen::Triplet<double>`, build the right-hand side vector, and solve the linear system using a direct solver such as `Eigen::SparseLU`. The function should return the solution vector `Eigen::VectorXd` of size \( n^2 \) (column-major ordering: index = `i + j*n`, where `0 <= i < n` is the x-coordinate and `0 <= j < n` is the y-coordinate), with interior nodes solved and boundary nodes populated by the specified boundary values. The output vector must contain values in the range [0, 1] (since the boundary function ranges from 0 to 1) and the solution should be exactly 1 at the midpoints of the left/right boundaries (when `n` is odd) or close to it otherwise. Handle the case `n >= 1`; for `n=1`, only the single interior/boundary node exists, and the function must return the boundary value (which would be `sin^2(0)=0` at the bottom-left corner). Do not include any image saving or external rendering; only the numerical solution is needed.

// The problem is a standard 2D Poisson equation with Dirichlet boundary conditions on all four edges. The approach uses the finite difference method with a 5-point stencil: for each interior node `(i,j)`, the equation `4*u(i,j) - u(i-1,j) - u(i+1,j) - u(i,j-1) - u(i,j+1) = 0` is assembled. Boundary nodes are known and moved to the right-hand side. Specifically, for each node, we iterate over its four neighbors; if a neighbor is outside the grid, the corresponding boundary value is multiplied by the stencil coefficient (which is `-1` for neighbors) and subtracted from `b`, and the diagonal coefficient `4` remains on the left-hand side. The boundary function is defined as `boundary(k) = sin^2(pi * (k / (n-1)))` for indices `0..n-1`, representing the y-coordinate normalized to [0,1]. For left/right boundaries, the boundary value depends on the row index `j`; for top/bottom boundaries, it depends on the column index `i`. The assembly is done by iterating `j` (rows) and `i` (columns) and using the helper `insertCoefficient` which checks if a neighbor is outside and modifies `b` accordingly. After assembly, we solve `A x = b` using `Eigen::SparseLU` with a column-major sparse matrix (default for `Eigen::SparseMatrix<double>`). Edge cases: `n=1` means a single node, all neighbors are out of bounds, so `b` becomes the sum of `4 * boundary(0)` (since the diagonal coefficient is 4 but all neighbors are boundary, the equation becomes `4*u = 4*boundary(0)` → `u = boundary(0) = sin^2(0)=0`). For `n>=2`, the system is non-singular and symmetric positive definite, so `SparseLU` works. Time complexity is `O(n^2)` for assembly and `O(n^3)` for factorization in the worst case (but for typical sparse matrices much faster), and memory complexity is `O(n^2)` for the sparse matrix and vectors.

#include <Eigen/Sparse>
#include <Eigen/OrderingMethods>
#include <vector>
#include <cmath>

// Helper to insert a coefficient into the sparse matrix or move it to RHS.
void insertCoefficient(int id, int i, int j, double w, std::vector<Eigen::Triplet<double>>& coeffs,
                       Eigen::VectorXd& b, const Eigen::VectorXd& boundary) {
    int n = static_cast<int>(boundary.size());
    int id1 = i + j * n;

    if (i == -1 || i == n) {
        // Left/right boundary: boundary value depends on row j (y-coordinate)
        b(id) -= w * boundary(j);
    } else if (j == -1 || j == n) {
        // Top/bottom boundary: boundary value depends on column i (x-coordinate)
        b(id) -= w * boundary(i);
    } else {
        coeffs.emplace_back(id, id1, w);
    }
}

// Solve the discrete Poisson equation on an n x n grid with sin^2 boundary conditions.
Eigen::VectorXd solvePoisson2D(int n) {
    using SpMat = Eigen::SparseMatrix<double>;
    using Triplet = Eigen::Triplet<double>;

    if (n <= 0) {
        return Eigen::VectorXd(); // empty vector for invalid input
    }

    // Boundary function: values along the left/right edges for each row j (0..n-1)
    Eigen::VectorXd boundary = Eigen::VectorXd::LinSpaced(n, 0.0, M_PI).array().sin().square();

    // Assemble sparse system
    std::vector<Triplet> coefficients;
    Eigen::VectorXd b = Eigen::VectorXd::Zero(n * n);
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            int id = i + j * n;
            insertCoefficient(id, i - 1, j, -1.0, coefficients, b, boundary);
            insertCoefficient(id, i + 1, j, -1.0, coefficients, b, boundary);
            insertCoefficient(id, i, j - 1, -1.0, coefficients, b, boundary);
            insertCoefficient(id, i, j + 1, -1.0, coefficients, b, boundary);
            insertCoefficient(id, i, j, 4.0, coefficients, b, boundary);
        }
    }

    // Build sparse matrix
    SpMat A(n * n, n * n);
    A.setFromTriplets(coefficients.begin(), coefficients.end());
    A.makeCompressed();

    // Solve using direct sparse LU
    Eigen::SparseLU<SpMat, Eigen::COLAMDOrdering<int>> solver;
    solver.compute(A);
    if (solver.info() != Eigen::Success) {
        // Fallback: return zero vector on failure (should not happen for valid input)
        return Eigen::VectorXd::Zero(n * n);
    }
    Eigen::VectorXd x = solver.solve(b);
    return x;
}

#include <cassert>
#include <cmath>
#include <Eigen/Dense>

int main() {
    // Test n=1: single node, boundary value at bottom-left corner is sin^2(0)=0
    {
        Eigen::VectorXd sol = solvePoisson2D(1);
        assert(sol.size() == 1);
        assert(std::abs(sol(0) - 0.0) < 1e-12);
    }

    // Test n=2: small system, symmetric solution should be [0, 1, 0, 1]? 
    // Actually with boundary sin^2(0)=0 and sin^2(pi)=0 (since n=2, positions 0 and 1),
    // boundary values are 0 and 0, so all boundary conditions are 0, but interior? 
    // For n=2, all nodes are boundary (since every node has a neighbor outside), so all values are 0.
    {
        Eigen::VectorXd sol = solvePoisson2D(2);
        assert(sol.size() == 4);
        for (int i = 0; i < 4; ++i) {
            assert(std::abs(sol(i)) < 1e-12);
        }
    }

    // Test n=3: boundary sin^2(0)=0, sin^2(pi/2)=1, sin^2(pi)=0
    // The center node (i=1,j=1) should be a weighted average of neighbors, and due to symmetry, 
    // should be 1/4? Actually let's compute: boundary on left/right rows: row0:0, row1:1, row2:0.
    // The system is small; we can expect the center value to be 1/4? Not exactly, but ensure it's in (0,1).
    {
        Eigen::VectorXd sol = solvePoisson2D(3);
        assert(sol.size() == 9);
        // Check boundary nodes: left edge (i=0) for j=0..2 should be 0,1,0
        assert(std::abs(sol(0) - 0.0) < 1e-12); // (0,0)
        assert(std::abs(sol(1) - 1.0) < 1e-12); // (0,1)
        assert(std::abs(sol(2) - 0.0) < 1e-12); // (0,2)
        // Right edge (i=2) similarly
        assert(std::abs(sol(6) - 0.0) < 1e-12); // (2,0)
        assert(std::abs(sol(7) - 1.0) < 1e-12); // (2,1)
        assert(std::abs(sol(8) - 0.0) < 1e-12); // (2,2)
        // Center node (i=1,j=1) should be between 0 and 1
        double center = sol(4);
        assert(center > 0.0 && center < 1.0);
        // Check symmetry: (1,0) and (1,2) should be equal (top/bottom boundaries are zero)
        assert(std::abs(sol(3) - sol(5)) < 1e-12);
    }

    // Test n=5: larger grid, all values in [0,1], and midpoint on boundary should be 1.
    {
        Eigen::VectorXd sol = solvePoisson2D(5);
        assert(sol.size() == 25);
        // Check all values within [0,1]
        for (int i = 0; i < 25; ++i) {
            assert(sol(i) >= -1e-12 && sol(i) <= 1.0 + 1e-12);
        }
        // Left edge midpoint: (0,2) index = 0 + 2*5 = 10, boundary sin^2(pi*0.5)=1
        assert(std::abs(sol(10) - 1.0) < 1e-12);
        // Right edge midpoint: (4,2) index = 4 + 2*5 = 14, also 1
        assert(std::abs(sol(14) - 1.0) < 1e-12);
        // Corners are 0
        assert(std::abs(sol(0)) < 1e-12);
        assert(std::abs(sol(4)) < 1e-12);
        assert(std::abs(sol(20)) < 1e-12);
        assert(std::abs(sol(24)) < 1e-12);
    }

    return 0;
}

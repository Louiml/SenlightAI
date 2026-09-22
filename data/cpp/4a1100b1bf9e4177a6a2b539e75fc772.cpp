/*
Write a C++ function named `solveLeastSquaresSystem` that, given a compile-time fixed-size matrix `A` of dimensions 2×3 and a compile-time fixed-size vector or matrix `b` of dimensions 2×1 (or 2×N for multiple right-hand sides), solves the linear system \(A x = b\) in the least-squares sense. The function should use Eigen's `fullPivLu` decomposition to attempt an exact solve when the system is consistent (i.e., \(b\) lies in the column space of \(A\)). If the system has no exact solution, the function should compute and return the least-squares solution that minimizes \(\|A x - b\|_2\) using the normal equations via the same full-pivoting LU decomposition on the Gram matrix. The returned solution must be a `Matrix<float, 3, N>` (or `Vector3f` for N=1). The input matrix `A` is guaranteed to have full column rank (rank 2), so the least-squares solution is unique. The function must handle any number of right-hand sides N (including 1) and must not modify the inputs.
*/
#include <Eigen/Dense>

// Solve A x = b in the least-squares sense, where A is 2x3 (full column rank)
// and b is 2xN (N >= 1). Returns a 3xN matrix containing the solution.
// If the system is consistent, returns an exact solution; otherwise returns
// the unique least-squares solution that minimizes ||A x - b||_2.
Eigen::Matrix<float, 3, Eigen::Dynamic> solveLeastSquaresSystem(
    const Eigen::Matrix<float, 2, 3>& A,
    const Eigen::Matrix<float, 2, Eigen::Dynamic>& b)
{
    // Attempt an exact solution using full pivoting LU decomposition.
    Eigen::Matrix<float, 3, Eigen::Dynamic> x = A.fullPivLu().solve(b);

    // Check if the residual is essentially zero.
    if ((A * x).isApprox(b, 1e-6f))
    {
        return x;
    }

    // Otherwise, compute the least-squares solution via normal equations:
    // (A^T A) x = A^T b.  Since A has full column rank, A^T A is invertible.
    Eigen::Matrix<float, 3, 3> AtA = A.transpose() * A;
    Eigen::Matrix<float, 3, Eigen::Dynamic> Atb = A.transpose() * b;
    return AtA.fullPivLu().solve(Atb);
}
#include <Eigen/Dense>
#include <cassert>

int main() {
    // Test 1: Consistent system, exact solution exists.
    // A = [[1,0,0],[0,1,0]], b = [[2],[-3]] -> x = [2;-3;0] (many solutions, one possible)
    Eigen::Matrix<float,2,3> A1;
    A1 << 1, 0, 0, 0, 1, 0;
    Eigen::Matrix<float,2,1> b1;
    b1 << 2, -3;
    Eigen::Matrix<float,3,1> x1 = solveLeastSquaresSystem(A1, b1);
    assert((A1 * x1).isApprox(b1, 1e-6f));
    // Verify the solution satisfies least-squares minimal norm? Since consistent, any solution works.

    // Test 2: Consistent with multiple right-hand sides.
    Eigen::Matrix<float,2,2> b2;
    b2 << 1, 4, 5, -2;
    Eigen::Matrix<float,3,2> x2 = solveLeastSquaresSystem(A1, b2);
    assert((A1 * x2).isApprox(b2, 1e-6f));

    // Test 3: Inconsistent system, least-squares solution.
    // A = [[1,0,0],[0,1,0]], b = [[1],[2]] -> consistent actually. Use non-zero third column? Wait A is 2x3 full rank, columns are independent.
    // To force inconsistency, use A = [[1,2,3],[4,5,6]] but column rank might be 2? Check. Actually matrix [[1,2],[4,5]] invertible, so full rank.
    Eigen::Matrix<float,2,3> A3;
    A3 << 1, 2, 3, 4, 5, 6;
    Eigen::Matrix<float,2,1> b3;
    b3 << 1, 1;
    Eigen::Matrix<float,3,1> x3 = solveLeastSquaresSystem(A3, b3);
    // Check that the residual is minimized: verify x3 is the normal equation solution.
    Eigen::Matrix<float,3,3> AtA3 = A3.transpose() * A3;
    Eigen::Matrix<float,3,1> Atb3 = A3.transpose() * b3;
    assert(AtA3.fullPivLu().solve(Atb3).isApprox(x3, 1e-6f));
    // Also check that the residual is orthogonal to columns of A.
    Eigen::Matrix<float,2,1> residual = A3 * x3 - b3;
    assert((A3.transpose() * residual).norm() < 1e-5f);

    // Test 4: Inconsistent with multiple right-hand sides.
    Eigen::Matrix<float,2,2> b4;
    b4 << 1, 0, 0, 1;
    Eigen::Matrix<float,3,2> x4 = solveLeastSquaresSystem(A3, b4);
    for (int i = 0; i < 2; ++i) {
        Eigen::Matrix<float,2,1> res = A3 * x4.col(i) - b4.col(i);
        assert((A3.transpose() * res).norm() < 1e-5f);
    }

    // Test 5: Random consistent system constructed via A * x0.
    Eigen::Matrix<float,2,3> A5 = Eigen::Matrix<float,2,3>::Random();
    Eigen::Matrix<float,3,1> x0 = Eigen::Matrix<float,3,1>::Random();
    Eigen::Matrix<float,2,1> b5 = A5 * x0;  // guaranteed consistent
    Eigen::Matrix<float,3,1> x5 = solveLeastSquaresSystem(A5, b5);
    assert((A5 * x5).isApprox(b5, 1e-6f));

    // Test 6: Random inconsistent system (b not in column space), verify normal equations hold.
    Eigen::Matrix<float,2,1> b6 = Eigen::Matrix<float,2,1>::Random();
    // Slight chance it lands exactly in column space, but negligible for random floats.
    Eigen::Matrix<float,3,1> x6 = solveLeastSquaresSystem(A5, b6);
    Eigen::Matrix<float,2,1> res6 = A5 * x6 - b6;
    assert((A5.transpose() * res6).norm() < 1e-5f);

    // Test 7: Single column, consistent with zero third component.
    Eigen::Matrix<float,2,3> A7;
    A7 << 1, 0, 0, 0, 2, 0;
    Eigen::Matrix<float,2,1> b7;
    b7 << 3, -4;
    Eigen::Matrix<float,3,1> x7 = solveLeastSquaresSystem(A7, b7);
    assert((A7 * x7).isApprox(b7, 1e-6f));
    assert(x7(2) == 0.0f); // exact solution often picks zero for free variable

    // Test 8: N=0? Not allowed, but we skip.

    // Test 9: Large-ish matrix? Fixed 2x3, no need.

    // Test 10: Verify return type dimensions for 3D vector.
    Eigen::Matrix<float,3,1> dummy = x1;
    assert(dummy.size() == 3);
    return 0;
}
// The main challenge is to decide whether the system \(A x = b\) is consistent. Since \(A\) is 2×3 with full column rank (rank 2), the equation has either zero or infinitely many solutions. However, because the row dimension equals the rank, consistency occurs only when \(b\) lies exactly in the 2-dimensional column space spanned by the columns of \(A\). We first attempt to solve exactly using Eigen's `fullPivLu().solve(b)`. The LU decomposition with full pivoting can handle rank-deficient \(A\), but since \(A\) has full column rank, it will yield a solution (one of the infinitely many) if consistent. The issue is that `fullPivLu().solve` does not directly tell us if the system is consistent; it will produce some vector even for inconsistent systems (because it treats it as a least-squares-like solve). To check consistency, we compute the residual \(r = A x - b\) and test `isApprox` with a suitable tolerance. If the residual is near zero, we return that solution. If not, we compute the normal equations: \(A^T A x = A^T b\). Since \(A\) has full column rank, \(A^T A\) is 3×3 and invertible. We solve this 3×3 system using `fullPivLu().solve` again to obtain the least-squares solution. This approach works for any number of columns in `b`, as the solve handles multiple right-hand sides. Time complexity: forming \(A^T A\) and \(A^T b\) is \(O(2 \cdot 3 \cdot N)\), the LU decomposition of a 3×3 matrix is \(O(3^3)=O(1)\) constant, and the solve is \(O(3^2 N)\). Overall, constant time for fixed dimensions. Space complexity: \(O(1)\) additional storage beyond the inputs and the 3×N result.

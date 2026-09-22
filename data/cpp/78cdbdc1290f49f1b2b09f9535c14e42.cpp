/*
Write a C++ function that determines whether a set of linear inequality constraints of the form `lower_bound[i] <= dot(row_i, x) <= upper_bound[i]` is feasible for a given vector `x`. The function should take as input: the number of constraints `m`, the number of variables `n`, a row-major matrix `A` (size `m x n`), a vector `x` (size `n`), and vectors `lb` and `ub` (each size `m`). It should return `true` if there exists at least one `x` that satisfies every constraint (i.e., for all `i`, `lb[i] <= sum_j A[i][j] * x[j] <= ub[i]`), allowing a small tolerance `1e-9` for numerical precision. The function must handle cases where some constraints are one-sided (i.e., `lb[i] = -INFINITY` or `ub[i] = +INFINITY`), and must also handle the case where `lb[i] > ub[i]` after accounting for tolerance, which immediately makes the problem infeasible. The function should be `const`-correct and use `std::vector<double>` for all containers.
*/
#include <vector>
#include <cmath>
#include <limits>

// Check if vector x satisfies all linear inequality constraints:
// lb[i] <= A[i][:] dot x <= ub[i] for all i, with tolerance.
// Handles infinite bounds by checking only finite ones.
// Returns true if feasible, false otherwise.
bool isFeasible(
    const std::vector<double>& x,
    const std::vector<double>& lb,
    const std::vector<double>& ub,
    const std::vector<std::vector<double>>& A
) {
    const size_t m = A.size();
    const size_t n = x.size();
    const double tol = 1e-9;

    // An empty problem is trivially feasible.
    if (m == 0) return true;

    // Sanity check: dimension mismatch
    // (though typically the caller guarantees this, we handle gracefully)
    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != n) {
            // Mismatch -> treat as infeasible or handle as error.
            // For this task, we return false to be safe.
            return false;
        }
    }

    // First check basic interval consistency: lb[i] > ub[i] + tolerance => infeasible.
    for (size_t i = 0; i < m; ++i) {
        if (lb[i] > ub[i] + tol) return false;
    }

    // Evaluate each constraint.
    for (size_t i = 0; i < m; ++i) {
        // Compute dot product of row i with x.
        double dot = 0.0;
        for (size_t j = 0; j < n; ++j) {
            dot += A[i][j] * x[j];
        }

        // Check lower bound if finite.
        if (!std::isinf(lb[i])) {
            if (dot < lb[i] - tol) return false;
        }

        // Check upper bound if finite.
        if (!std::isinf(ub[i])) {
            if (dot > ub[i] + tol) return false;
        }
    }

    return true;
}
#include <cassert>
#include <cmath>
#include <vector>

// Declare the function (assuming it's in the same translation unit).

int main() {
    // Test 1: Simple 2D box constraints, feasible point.
    {
        std::vector<double> x = {1.0, 2.0};
        std::vector<double> lb = {0.0, 0.0};
        std::vector<double> ub = {3.0, 3.0};
        std::vector<std::vector<double>> A = {{1.0, 0.0}, {0.0, 1.0}};
        assert(isFeasible(x, lb, ub, A) == true);
    }

    // Test 2: Infeasible due to lower bound violation.
    {
        std::vector<double> x = {1.0, 2.0};
        std::vector<double> lb = {2.0, 0.0};
        std::vector<double> ub = {3.0, 3.0};
        std::vector<std::vector<double>> A = {{1.0, 0.0}, {0.0, 1.0}};
        assert(isFeasible(x, lb, ub, A) == false);
    }

    // Test 3: Infeasible because lb > ub.
    {
        std::vector<double> x = {0.0};
        std::vector<double> lb = {3.0};
        std::vector<double> ub = {2.0};
        std::vector<std::vector<double>> A = {{1.0}};
        assert(isFeasible(x, lb, ub, A) == false);
    }

    // Test 4: One-sided constraints (unbounded one side).
    {
        std::vector<double> x = {5.0};
        std::vector<double> lb = {-INFINITY};
        std::vector<double> ub = {10.0};
        std::vector<std::vector<double>> A = {{1.0}};
        assert(isFeasible(x, lb, ub, A) == true);
    }

    // Test 5: One-sided with violation on lower side.
    {
        std::vector<double> x = {5.0};
        std::vector<double> lb = {6.0};
        std::vector<double> ub = {INFINITY};
        std::vector<std::vector<double>> A = {{1.0}};
        assert(isFeasible(x, lb, ub, A) == false);
    }

    // Test 6: Mixed constraints with dot product.
    {
        std::vector<double> x = {1.0, 1.0};
        std::vector<double> lb = {1.0, 0.0};
        std::vector<double> ub = {2.0, 10.0};
        std::vector<std::vector<double>> A = {{1.0, 1.0}, {1.0, -1.0}};
        // Dot1 = 2, Dot2 = 0 -> both within bounds.
        assert(isFeasible(x, lb, ub, A) == true);
    }

    // Test 7: Tolerance: slightly violating but within 1e-9.
    {
        std::vector<double> x = {1.0};
        std::vector<double> lb = {1.0 - 1e-10};
        std::vector<double> ub = {1.0 + 1e-10};
        std::vector<std::vector<double>> A = {{1.0}};
        assert(isFeasible(x, lb, ub, A) == true);
    }

    // Test 8: Slightly outside tolerance -> false.
    {
        std::vector<double> x = {1.0};
        std::vector<double> lb = {1.0 + 1e-8};
        std::vector<double> ub = {2.0};
        std::vector<std::vector<double>> A = {{1.0}};
        assert(isFeasible(x, lb, ub, A) == false);
    }

    // Test 9: Empty constraints -> always feasible.
    {
        std::vector<double> x = {1.0, 2.0, 3.0};
        std::vector<double> lb;
        std::vector<double> ub;
        std::vector<std::vector<double>> A;
        assert(isFeasible(x, lb, ub, A) == true);
    }

    // Test 10: Zero variables, but check if 0 lies in each interval.
    {
        std::vector<double> x; // n=0
        std::vector<double> lb = {-1.0, 0.0};
        std::vector<double> ub = {1.0, 0.0};
        std::vector<std::vector<double>> A = {{}, {}}; // rows of length 0
        // Dot product is 0; lower -1 <= 0 <= 1, lower 0 <= 0 <= 0 -> feasible.
        assert(isFeasible(x, lb, ub, A) == true);
    }

    return 0;
}
// The solution is straightforward: for each constraint index `i`, compute the dot product of row `i` of `A` with `x`. Then check whether this value lies within the interval `[lb[i], ub[i]]` with a tolerance. To handle infinities, comparisons must be done carefully: if `lb[i]` is `-INFINITY`, there is no lower bound, so the check passes automatically for the lower side; similarly for `ub[i]` being `+INFINITY`. If `lb[i]` is finite and `value < lb[i] - tol`, return `false`. If `ub[i]` is finite and `value > ub[i] + tol`, return `false`. Also, before the loop, check that for every `i`, `lb[i] <= ub[i] + tol` (using a tolerant comparison to account for near-equal values); if not, return `false` immediately. The time complexity is `O(m*n)` because each dot product takes `O(n)` time and there are `m` constraints. Space complexity is `O(1)` extra (only loop indices and a temporary value). Edge cases include: empty constraint set (should return `true` vacuously), `n=0` (dot product is zero, so feasibility depends only on whether `0` lies in each interval), and constraints where the lower and upper bounds are extremely close (within tolerance) but not exactly equal—these should still be considered feasible if the value falls within the tolerance band.

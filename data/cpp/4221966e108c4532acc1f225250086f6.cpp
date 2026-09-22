/*
Implement a C++ function `computeReachableSet` that, given a linear constraint on acceleration `u` of the form `alpha * u + beta * x + gamma >= 0` (where `x = v^2` is the squared speed), a current reachable interval `[xmin, xmax]`, a next reachable interval `[lnext, hnext]`, and a positive path increment `delta`, computes the tightened reachable interval for `x` at the current path position. The function must return a `std::pair<double,double>` representing the feasible interval `[xmin, xmax]` after enforcing both forward reachability (that there exists some `u` such that `x + 2*delta*u >= lnext` and `x + 2*delta*u <= hnext`) and the linear acceleration constraint. Handle degenerate cases: if the constraint becomes impossible (empty interval), return `{1.0, 0.0}` to signal infeasibility. The constraints are given as three vectors `alpha`, `beta`, `gamma` of equal length, where each inequality is `alpha[j]*u + beta[j]*x + gamma[j] >= 0`. The function must correctly handle `alpha == 0` (constant constraint independent of `u`) and negative `alpha` by flipping the inequality direction appropriately. Use a tolerance `kEps = 1e-9` for comparisons.
*/

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

constexpr double kEps = 1e-9;

// Compute the tightened reachable interval [xmin, xmax] for squared speed x
// given linear acceleration constraints: alpha[j]*u + beta[j]*x + gamma[j] >= 0
// and forward reachability: x + 2*delta*u must be within [lnext, hnext].
// Returns {1.0, 0.0} if the interval becomes infeasible.
std::pair<double, double> computeReachableSet(
    const std::vector<double>& alpha,
    const std::vector<double>& beta,
    const std::vector<double>& gamma,
    double xmin,
    double xmax,
    double lnext,
    double hnext,
    double delta)
{
    // First apply constant constraints (alpha == 0) directly.
    for (size_t j = 0; j < alpha.size(); ++j) {
        double a = alpha[j];
        if (std::abs(a) < kEps) {
            double b = beta[j], c = gamma[j];
            // Constraint: b*x + c >= 0
            if (std::abs(b) < kEps) {
                if (c < -kEps) return {1.0, 0.0}; // impossible
            } else if (b > 0) {
                xmin = std::max(xmin, -c / b);
            } else {
                xmax = std::min(xmax, -c / b);
            }
        }
    }

    // Now handle alpha > 0 constraints: from lower bound on u to satisfy x_next >= lnext.
    for (size_t j = 0; j < alpha.size(); ++j) {
        double a = alpha[j];
        if (a <= kEps) continue;
        double b = beta[j], c = gamma[j];
        double denom = 1.0 - 2.0 * delta * b / a;
        double rhs = lnext + 2.0 * delta * c / a;
        if (std::abs(denom) < kEps) {
            if (rhs > kEps) return {1.0, 0.0};
        } else if (denom > 0.0) {
            xmin = std::max(xmin, rhs / denom);
        } else {
            xmax = std::min(xmax, rhs / denom);
        }
    }

    // Handle alpha < 0 constraints: from upper bound on u to satisfy x_next <= hnext.
    for (size_t j = 0; j < alpha.size(); ++j) {
        double a = alpha[j];
        if (a >= -kEps) continue;
        double b = beta[j], c = gamma[j];
        double denom = 1.0 - 2.0 * delta * b / a;
        double rhs = hnext + 2.0 * delta * c / a;
        if (std::abs(denom) < kEps) {
            if (rhs < -kEps) return {1.0, 0.0};
        } else if (denom > 0.0) {
            xmax = std::min(xmax, rhs / denom);
        } else {
            xmin = std::max(xmin, rhs / denom);
        }
    }

    // Final feasibility check.
    if (xmin > xmax + kEps) return {1.0, 0.0};
    return {xmin, xmax};
}

#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

int main() {
    // Simple constraint: u + 2x - 3 >= 0, delta=1, lnext=0, hnext=100
    // Initial x range [0,10]. Solve: x+2(u) where u >= -2x+3? Let's test known bound.
    {
        std::vector<double> alpha = {1.0}, beta = {2.0}, gamma = {-3.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 10.0, 0.0, 100.0, 1.0);
        // The constraint requires u >= -2x+3. To reach lnext=0, need x+2(-2x+3) >= 0 => x -4x+6 >=0 => -3x +6 >=0 => x <= 2.
        // So xmax should be 2. xmin remains 0.
        assert(std::abs(res.first - 0.0) < 1e-9);
        assert(std::abs(res.second - 2.0) < 1e-9);
    }

    // Negative alpha: constraint -u + x >= 0, delta=1, hnext=5, x range [0,10]
    // -u + x >= 0 => u <= x. To ensure x + 2u <= 5, worst u = x, so x + 2x <=5 => 3x <=5 => x <= 5/3.
    {
        std::vector<double> alpha = {-1.0}, beta = {1.0}, gamma = {0.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 10.0, 0.0, 5.0, 1.0);
        assert(std::abs(res.first - 0.0) < 1e-9);
        assert(std::abs(res.second - 5.0/3.0) < 1e-9);
    }

    // Impossible constraint: delta=1, constraint u >= 1 (alpha=1,beta=0,gamma=-1), lnext=100
    // Need x + 2*1 >= 100 => x >= 98, but xmax=0, so infeasible returns {1,0}
    {
        std::vector<double> alpha = {1.0}, beta = {0.0}, gamma = {-1.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 0.0, 100.0, 100.0, 1.0);
        assert(res.first == 1.0 && res.second == 0.0);
    }

    // Constant constraint: beta=2,gamma=-4 => 2x-4>=0 => x>=2
    {
        std::vector<double> alpha = {0.0}, beta = {2.0}, gamma = {-4.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 10.0, 0.0, 100.0, 1.0);
        assert(std::abs(res.first - 2.0) < 1e-9);
        assert(std::abs(res.second - 10.0) < 1e-9);
    }

    // Multiple constraints: alpha=1,beta=0,gamma=-2 (u>=2) and alpha=-1,beta=0,gamma=3 (u<=3)
    // delta=1, lnext=0, hnext=100, x range [0,10]
    // From u>=2, to reach lnext=0: x+4>=0 always true. From u<=3, to stay <=hnext: x+6<=100 always true.
    // So no change.
    {
        std::vector<double> alpha = {1.0, -1.0}, beta = {0.0, 0.0}, gamma = {-2.0, 3.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 10.0, 0.0, 100.0, 1.0);
        assert(std::abs(res.first - 0.0) < 1e-9);
        assert(std::abs(res.second - 10.0) < 1e-9);
    }

    // Edge case: denom zero with feasible condition (rhs <= 0 for a>0)
    // alpha=1, beta=0.5, gamma=0, delta=1 => denom = 1 - 2*1*0.5/1 = 0, rhs = lnext + 0 = lnext
    // If lnext <= 0, no restriction; if lnext > 0, infeasible.
    {
        std::vector<double> alpha = {1.0}, beta = {0.5}, gamma = {0.0};
        auto res = computeReachableSet(alpha, beta, gamma, 0.0, 10.0, -1.0, 100.0, 1.0);
        // Should be feasible with xmin=0, xmax=10 (since no restriction)
        assert(std::abs(res.first - 0.0) < 1e-9);
        assert(std::abs(res.second - 10.0) < 1e-9);
    }

    return 0;
}

// The problem is a simplified version of the backward pass in TOPPRA's time-optimal path parameterization. Given a state variable `x = v^2` (squared velocity) and control `u = dv/ds` (derivative of velocity w.r.t. path), the discretized dynamics are `x_{i+1} = x_i + 2*delta*u`. We need to compute the set of `x_i` for which there exists some `u` satisfying both:
// 1. The linear constraint: `alpha*u + beta*x + gamma >= 0`.
// 2. The forward reachability: `x + 2*delta*u` must lie within `[lnext, hnext]`.
//
// For each inequality `alpha*u + beta*x + gamma >= 0`, solve for `u`:
// - If `alpha > 0`, then `u >= -(beta*x + gamma)/alpha`. But since `u` must also ensure `x+2*delta*u >= lnext`, we combine.
// - More directly, eliminate `u` by considering `u` as free. For each constraint, rewrite the inequality in terms of `x` after substituting the reachability bounds.
//
// The key algebraic manipulation: For a constraint `alpha*u + beta*x + gamma >= 0`, rewrite `u` in terms of `x` and the target interval. Since `u` can be any value satisfying the inequality, the feasibility condition for a given `x` is that the intersection of the half-line of allowed `u` (from the constraint) and the interval of `u` that maps to `[lnext, hnext]` (i.e., `(lnext - x)/(2*delta)` to `(hnext - x)/(2*delta)`) is non-empty. Instead of solving this directly, we can tighten `x` bounds by considering each constraint separately.
//
// For `alpha > 0`: The inequality is `u >= -(beta*x + gamma)/alpha`. For the reachability lower bound, we need `x + 2*delta*u >= lnext`, i.e., `u >= (lnext - x)/(2*delta)`. The strongest lower bound on `u` is the max of these two lower bounds. For there to exist some `u` that also satisfies the upper bound `u <= (hnext - x)/(2*delta)`, we need the max lower bound <= max upper bound. But a simpler way (as done in the snippet) is to rearrange the inequality to bound `x`. Substitute `u = (x_next - x)/(2*delta)` into the constraint and consider both extremes of `x_next` (i.e., `lnext` and `hnext`). For `alpha > 0`, the constraint is `alpha*((x_next - x)/(2*delta)) + beta*x + gamma >= 0`. For a given `x_next`, this gives a linear inequality in `x`. Specifically, for `x_next = lnext` (the worst case for lower bound), we get `x` must be >= some expression. Similarly, for `x_next = hnext` we get another bound. The correct approach is to consider both: from `x_next >= lnext`, we require `x` such that the worst-case `u` (minimum) allows `x_next >= lnext`. This leads to a lower bound on `x`. From `x_next <= hnext`, we get an upper bound on `x`. The snippet does this by solving `x + 2*delta*(-(beta*x+gamma)/alpha) >= lnext`? Actually, the code solves a more careful inequality: it uses `denom = 1 - 2*delta*b/a` and `rhs = lnext + 2*delta*c/a`. Let's derive.
//
// Given `a*u + b*x + c >= 0`. If `a > 0`, then `u >= -(b*x + c)/a`. To guarantee that `x + 2*delta*u >= lnext` for all possible `u` that satisfy the constraint, the most restrictive `u` is the minimum `u`, which is `-(b*x + c)/a`. So we need `x + 2*delta*(-(b*x + c)/a) >= lnext`. Simplify: `x - (2*delta*b/a)*x - (2*delta*c/a) >= lnext`, so `x*(1 - 2*delta*b/a) >= lnext + 2*delta*c/a`. That yields a lower/upper bound depending on the sign of `denom`. If `denom > 0`, then `x >= rhs/denom`; if `denom < 0`, then `x <= rhs/denom`. If `denom == 0`, then the inequality becomes `0 >= rhs` – if `rhs > 0` then it's impossible; otherwise no restriction.
//
// Similarly for `a < 0`: the inequality is `u <= -(b*x + c)/a` (since dividing by negative flips). To guarantee `x + 2*delta*u <= hnext`, we use the maximum `u` (which is `-(b*x + c)/a`) and require `x + 2*delta*u <= hnext`, leading to the same algebra with `hnext` and `c` sign? Actually, the code uses `hnext` for `a < 0` and uses `rhs = hnext + 2*delta*c/a`. That is correct: For `a < 0`, the constraint is `u <= -(b*x+c)/a`. The maximum allowed `u` is `-(b*x+c)/a`. We need `x + 2*delta*u <= hnext` for the worst case (max `u`). So `x + 2*delta*(-(b*x+c)/a) <= hnext` gives same form with `lnext` replaced by `hnext`. Then same denom analysis.
//
// We must also consider constant constraints `a == 0`: then the inequality is `b*x + c >= 0`, which directly bounds `x` independent of `u`. We should enforce that directly. The given snippet does not handle `a == 0` explicitly, but for completeness we will add that in our solution.
//
// Edge cases:
// - If `delta` is non-positive? The problem says positive `delta`, so fine.
// - If the resulting interval is empty (`xmin > xmax`), return `{1.0, 0.0}`.
// - If `denom` is near zero and `rhs` has an impossible sign, we return infeasible.
//
// Time complexity: O(m), where m is the number of constraints (size of vectors). Space O(1) auxiliary.

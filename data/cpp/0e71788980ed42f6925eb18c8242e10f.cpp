// Write a C++ function `meshLineSearch` that, given a sequence of nodal gradient vectors (each represented as a `std::vector<double>` of dimension 2 or 3), starting coordinates (same dimension), a search direction vector (already scaled by a per-node step size `alpha`), and a local validity flag per node, performs a backtracking Armijo line search for each node independently. For each node, the function must return the best step length `alpha` such that a given metric function `nodal_metric(node_coord, valid)` (provided as a callable lambda) is strictly reduced compared to the original metric, subject to a sufficient decrease condition `metric(alpha) < metric(0) + c0 * alpha * ||gradient||^2`, with `c0 = 1e-4`, a backtracking factor `tau = 0.5`, and a minimum step of `1e-12`. If no valid reduction is found, set `alpha = 0.0`. The function must also enforce that the returned alpha is non-negative, and if the node is flagged as `fixed` or `ghost` (input booleans), return `0.0` immediately. The function should return a `std::vector<double>` of the same length as the input, where each entry is the optimal alpha for that node. The metric function must be evaluated with candidate coordinates `coord + alpha * direction`, and the validity flag must be updated by the metric callable. The function should not modify the input vectors except through the metric evaluation.

// The algorithm processes each node independently. For each node, we first check the `isFixed` and `isGhost` flags; if either is true, we immediately push 0.0. Otherwise, we compute the squared norm of the gradient (`norm_gradient2`). If this norm is below a tiny threshold (e.g., 1e-12), we also return 0.0 because the gradient is essentially zero. We then evaluate the original metric `f0 = metric(coord, valid0)` using the starting coordinate and a fresh validity flag (initially true). We set `alpha = local_scale` (which the input provides as a suggested initial step, but since we're simplifying, we can use a fixed initial alpha, e.g., 1.0, or we can pass it per node; for simplicity, we use a constant `initialAlpha = 1.0`). Then we loop: compute candidate coordinate `coord + alpha*direction`, call the metric `f = metric(candidate, valid)`, check `f < f0 + c0*alpha*norm_gradient2`. If true, we accept and break; otherwise, we multiply `alpha *= 0.5` and repeat, until `alpha < 1e-12`. After the loop, if no acceptable alpha was found (loop ended without accepting), we set alpha to 0.0. The returned vector contains these alphas. Edge cases: all nodes fixed/ghost → returns all zeros; zero gradient → returns zero; metric never decreases → returns zero. Time complexity: O(N * (number of backtracking iterations)), where N is the number of nodes; each iteration evaluates the metric, which is assumed O(dimension). Space: O(N) for the result vector.

#include <vector>
#include <cmath>
#include <functional>

/**
 * Performs a per-node backtracking Armijo line search for mesh smoothing.
 * 
 * @param gradients       Per-node gradient vectors (each of size dim).
 * @param coords          Per-node current coordinates (each of size dim).
 * @param directions      Per-node search directions (each of size dim), already scaled by local alpha.
 * @param isFixed         Per-node flag: if true, skip and return 0.
 * @param isGhost         Per-node flag: if true, skip and return 0.
 * @param metricFunc      Callable with signature double(const std::vector<double>&, bool&)
 *                        that returns the local metric and updates the validity flag.
 * @return                Vector of optimal alpha for each node (non-negative).
 */
std::vector<double> meshLineSearch(
    const std::vector<std::vector<double>>& gradients,
    const std::vector<std::vector<double>>& coords,
    const std::vector<std::vector<double>>& directions,
    const std::vector<bool>& isFixed,
    const std::vector<bool>& isGhost,
    const std::function<double(const std::vector<double>&, bool&)>& metricFunc)
{
    const size_t n = gradients.size();
    std::vector<double> alphas(n, 0.0);
    const double c0 = 1e-4;
    const double tau = 0.5;
    const double minAlpha = 1e-12;
    const double gradientEps = 1e-12;

    for (size_t i = 0; i < n; ++i) {
        // Skip fixed or ghost nodes
        if (isFixed[i] || isGhost[i]) {
            alphas[i] = 0.0;
            continue;
        }

        const size_t dim = gradients[i].size();
        // Compute squared gradient norm
        double normGrad2 = 0.0;
        for (size_t d = 0; d < dim; ++d) {
            normGrad2 += gradients[i][d] * gradients[i][d];
        }
        if (normGrad2 < gradientEps * gradientEps) {
            alphas[i] = 0.0;
            continue;
        }

        // Evaluate original metric at current coordinates
        bool valid0 = true;
        double f0 = metricFunc(coords[i], valid0);

        // Initial step: use a default of 1.0 (or could be local scale)
        double alpha = 1.0;
        bool accepted = false;

        while (alpha >= minAlpha) {
            // Candidate coordinate: coord + alpha * direction
            std::vector<double> candidate(dim);
            for (size_t d = 0; d < dim; ++d) {
                candidate[d] = coords[i][d] + alpha * directions[i][d];
            }

            // Evaluate metric at candidate
            bool valid = true;
            double f = metricFunc(candidate, valid);

            // Armijo sufficient decrease condition
            double armijoOffset = c0 * alpha * normGrad2;
            if (f < f0 + armijoOffset) {
                accepted = true;
                break;
            }
            alpha *= tau;
        }

        // If no acceptable alpha found, set to 0.0
        if (!accepted) {
            alpha = 0.0;
        }
        alphas[i] = alpha;
    }

    return alphas;
}

#include <cassert>
#include <vector>
#include <functional>

// The solution function is assumed to be included above.
// Test metric: a simple quadratic function of the coordinate's first component.
// metric = (x[0] - 3)^2 + x[1]^2 , validity is always true.
double testMetric(const std::vector<double>& x, bool& valid) {
    valid = true;
    return (x[0] - 3.0) * (x[0] - 3.0) + x[1] * x[1];
}

int main() {
    // Case 1: simple 2D node, gradient = (2, 0) at coord (1,0), direction = -gradient = (-2, 0)
    // Starting coord (1,0): metric = (1-3)^2 + 0 = 4
    // At alpha=1: coord = (1-2,0)=(-1,0): metric = 16 (worse)
    // Backtrack: alpha=0.5: coord=(0,0): metric=9 (worse)
    // alpha=0.25: coord=(0.5,0): metric=6.25 (worse)
    // alpha=0.125: coord=(0.75,0): metric=5.0625 (worse)
    // alpha=0.0625: coord=(0.875,0): metric=4.5156 (worse)
    // alpha=0.03125: coord=(0.9375,0): metric=4.2539 (worse)
    // alpha=0.015625: coord=(0.96875,0): metric=4.125 (worse)
    // ... until alpha < 1e-12, never accepted because f > f0 always. So alpha=0.
    {
        std::vector<std::vector<double>> grads = {{2.0, 0.0}};
        std::vector<std::vector<double>> coords = {{1.0, 0.0}};
        std::vector<std::vector<double>> dirs   = {{-2.0, 0.0}};
        std::vector<bool> fixed = {false};
        std::vector<bool> ghost = {false};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, testMetric);
        assert(res.size() == 1);
        assert(res[0] == 0.0); // no reduction possible
    }

    // Case 2: gradient points uphill, direction = -gradient with magnitude small enough that alpha=1 works.
    // We'll directly test with a custom metric where the function decreases along the direction.
    auto simpleLinearMetric = [](const std::vector<double>& x, bool& valid) {
        valid = true;
        // metric = (x[0] - 10)^2 (one-dimensional effectively)
        return (x[0] - 10.0) * (x[0] - 10.0);
    };
    {
        // Treat 1D: gradient = 2*(x0-10) at x0=9 => -2, direction = -gradient = +2
        // coord = [9], grad = [-2], dir = [2]
        // At alpha=1: coord=11, metric=(11-10)^2=1; f0=(9-10)^2=1; Armijo: 1 < 1 + 1e-4*1*4=1.0004 -> accepted.
        std::vector<std::vector<double>> grads = {{-2.0}};
        std::vector<std::vector<double>> coords = {{9.0}};
        std::vector<std::vector<double>> dirs   = {{2.0}};
        std::vector<bool> fixed = {false};
        std::vector<bool> ghost = {false};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, simpleLinearMetric);
        assert(res.size() == 1);
        assert(std::fabs(res[0] - 1.0) < 1e-12);
    }

    // Case 3: fixed node returns 0
    {
        std::vector<std::vector<double>> grads = {{1.0, 1.0}};
        std::vector<std::vector<double>> coords = {{0.0, 0.0}};
        std::vector<std::vector<double>> dirs   = {{1.0, 1.0}};
        std::vector<bool> fixed = {true};
        std::vector<bool> ghost = {false};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, testMetric);
        assert(res.size() == 1);
        assert(res[0] == 0.0);
    }

    // Case 4: ghost node returns 0
    {
        std::vector<std::vector<double>> grads = {{1.0, 1.0}};
        std::vector<std::vector<double>> coords = {{0.0, 0.0}};
        std::vector<std::vector<double>> dirs   = {{1.0, 1.0}};
        std::vector<bool> fixed = {false};
        std::vector<bool> ghost = {true};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, testMetric);
        assert(res.size() == 1);
        assert(res[0] == 0.0);
    }

    // Case 5: zero gradient returns 0
    {
        std::vector<std::vector<double>> grads = {{0.0, 0.0}};
        std::vector<std::vector<double>> coords = {{0.0, 0.0}};
        std::vector<std::vector<double>> dirs   = {{1.0, 1.0}};
        std::vector<bool> fixed = {false};
        std::vector<bool> ghost = {false};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, testMetric);
        assert(res.size() == 1);
        assert(res[0] == 0.0);
    }

    // Case 6: multiple nodes with mixed behavior
    {
        std::vector<std::vector<double>> grads = {{-2.0}, {2.0}};
        std::vector<std::vector<double>> coords = {{9.0}, {1.0}};
        std::vector<std::vector<double>> dirs   = {{2.0}, {-2.0}};
        std::vector<bool> fixed = {false, false};
        std::vector<bool> ghost = {false, false};
        auto res = meshLineSearch(grads, coords, dirs, fixed, ghost, simpleLinearMetric);
        assert(res.size() == 2);
        assert(std::fabs(res[0] - 1.0) < 1e-12); // node 0 decreases
        assert(res[1] == 0.0); // node 1 cannot decrease
    }

    return 0;
}

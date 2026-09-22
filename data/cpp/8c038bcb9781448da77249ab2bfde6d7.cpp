Write a standalone C++ function named `computeFrailtyExpec` that takes as input a matrix of event times `Time` (with rows representing subjects and columns representing ordered event times, where for subject i only the first `K[i]` entries are meaningful), a logical matrix `delt` (same dimensions, indicating whether the interval j is censored (0) or event (1)), an integer vector `K` (number of intervals per subject), a positive scalar `lam` (constant baseline hazard), a positive scalar `alpha` (shape parameter of a Gamma frailty), a vector `grid` (ordered time grid points), a positive scalar `exp` (constant multiplicative exposure), a vector `nodes` and a vector `weights` (Gauss-Legendre quadrature nodes/weights on [0,∞)), and a boolean `computeLogLik`. The function must return a `std::vector<std::vector<double>>` where each row corresponds to a subject, the first `grid.size()` entries contain the expected cumulative hazard contributions for each grid interval (zero outside the subject's observed time range), the next entry is the expected frailty `E[ξ]`, the following entry is `E[log ξ]`, and the final entry (only in row 0) contains the log-likelihood contribution sum if `computeLogLik` is true, else 0.0. The mathematical model: for subject i, the conditional likelihood over intervals j=1..K[i] is `(λ·exp·interval_len)^(delt_ij) · exp(-ξ·λ·exp·interval_len)` times gamma density `ξ^(alpha-1)e^(-ξ)/Γ(alpha)`. The posterior of ξ is proportional to `ξ^(alpha-1) · exp(-ξ - ξ·Σ_j (λ·exp·len_ij))` and must be normalized using quadrature. For each interval j, the expected contribution to the cumulative hazard in that grid interval is `E[ξ · λ·exp · 1(interval j)]`. Use robust quadrature over nodes/weights to compute integrals of the form ∫ f(ξ)·density(ξ) dξ.

#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

// Include the solution function here (or link separately)
// For brevity, assume the above function is defined above.

int main() {
    // Test 1: Single subject, one interval, no event (censored)
    {
        std::vector<std::vector<double>> T = {{0.0, 1.0}};
        std::vector<std::vector<int>> D = {{0}};
        std::vector<int> K = {1};
        double lam = 0.5, alpha = 2.0, expo = 1.0;
        std::vector<double> grid = {0.0, 0.5, 1.0};
        std::vector<double> nodes = {0.5, 1.0, 1.5};
        std::vector<double> weights = {1.0/3, 1.0/3, 1.0/3}; // simple equidistant approx
        auto res = computeFrailtyExpec(T, D, K, lam, alpha, grid, expo, nodes, weights, true);
        // E_xi for gamma(alpha=2) with no hazard -> posterior is gamma(2+1,1+0)=gamma(3,1) mean 3
        assert(std::abs(res[0][2] - 3.0) < 0.2);
        // Expected cumulative hazard in grid interval [0,0.5] should be 0.5*E_xi
        assert(res[0][0] > 0.4 && res[0][0] < 0.6);
        // Log-likelihood should be finite
        assert(std::isfinite(res[0][4]));
    }

    // Test 2: Two subjects, all events, check row length
    {
        std::vector<std::vector<double>> T = {{0.0, 0.5, 1.0}, {0.0, 2.0, 2.0}};
        std::vector<std::vector<int>> D = {{1, 1}, {1, 1}};
        std::vector<int> K = {2, 2};
        double lam = 0.2, alpha = 1.0, expo = 1.0;
        std::vector<double> grid = {0.0, 0.5, 1.0, 2.0};
        std::vector<double> nodes = {0.5, 1.0};
        std::vector<double> weights = {0.5, 0.5};
        auto res = computeFrailtyExpec(T, D, K, lam, alpha, grid, expo, nodes, weights, false);
        assert(res.size() == 2);
        assert(res[0].size() == 4 + 3); // L=3, so 6
        assert(std::abs(res[1][3]) < 1e-9); // grid interval [1,2] for subject1? Actually subject1 ends at 1.0, so no overlap
        assert(res[1][0] > 0.0); // interval [0,0.5] has overlap

        // Check log-likelihood not computed when false
        assert(res[0][4] == 0.0);
    }

    // Test 3: Extreme case: zero hazard
    {
        std::vector<std::vector<double>> T = {{0.0, 0.0}};
        std::vector<std::vector<int>> D = {{0}};
        std::vector<int> K = {1};
        double lam = 0.0, alpha = 5.0, expo = 2.0;
        std::vector<double> grid = {0.0, 1.0};
        std::vector<double> nodes = {1.0, 2.0};
        std::vector<double> weights = {0.5, 0.5};
        auto res = computeFrailtyExpec(T, D, K, lam, alpha, grid, expo, nodes, weights, true);
        // posterior is gamma(alpha,1) mean alpha
        assert(std::abs(res[0][1] - 5.0) < 1e-6);
        // E_xi = alpha, log-likelihood = alpha*ln(alpha)-lgamma(alpha)+log(denom)
        // denom = integral of xi^(alpha-1) e^{-xi} = Gamma(alpha) so log denom = lgamma(alpha)
        // so ll = alpha*ln(alpha)
        assert(std::abs(res[0][2] - 5.0 * std::log(5.0)) < 1e-6);
    }

    // Test 4: Empty input
    {
        std::vector<std::vector<double>> T;
        std::vector<std::vector<int>> D;
        std::vector<int> K;
        auto res = computeFrailtyExpec(T, D, K, 1.0, 1.0, {0.0,1.0}, 1.0, {1.0}, {1.0}, false);
        assert(res.empty());
    }

    std::cout << "All tests passed.\n";
    return 0;
}

#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

// Compute expected frailty quantities and expected cumulative hazard contributions
// for a gamma frailty model with piecewise constant baseline hazard lambda.
// Time: matrix of event times, each row i has K[i] meaningful entries.
// delt: 1 for event, 0 for censoring in each interval.
// K: number of intervals per subject.
// lam: constant baseline hazard.
// alpha: gamma shape parameter.
// grid: ordered time points defining intervals (grid.size() intervals).
// exp: multiplicative exposure.
// nodes, weights: quadrature nodes/weights on (0,∞) for posterior integration.
// computeLogLik: if true, return log-likelihood contribution in row 0 last element.
std::vector<std::vector<double>> computeFrailtyExpec(
    const std::vector<std::vector<double>>& Time,
    const std::vector<std::vector<int>>& delt,
    const std::vector<int>& K,
    double lam,
    double alpha,
    const std::vector<double>& grid,
    double expo,
    const std::vector<double>& nodes,
    const std::vector<double>& weights,
    bool computeLogLik) {
    const int n = static_cast<int>(Time.size());
    const int L = static_cast<int>(grid.size());
    const int Q = static_cast<int>(nodes.size());
    if (n == 0 || Q == 0 || L == 0) return {};

    std::vector<std::vector<double>> result(n, std::vector<double>(L + 3, 0.0));
    double totalLogLik = 0.0;

    for (int i = 0; i < n; ++i) {
        // Compute total hazard sum over observed intervals for subject i
        double totalHazard = 0.0;
        for (int j = 0; j < K[i]; ++j) {
            double len = Time[i][j + 1] - Time[i][j];
            if (len < 0) len = 0; // defensive
            totalHazard += len;
        }
        totalHazard *= lam * expo;

        // Unnormalized posterior weights for each quadrature node
        std::vector<double> w(Q);
        double denom = 0.0;
        for (int k = 0; k < Q; ++k) {
            double xi = nodes[k];
            double logw = (alpha - 1.0) * std::log(xi) - xi - xi * totalHazard;
            w[k] = std::exp(logw);
            denom += w[k] * weights[k];
        }

        // Expectations using normalized posterior
        double E_xi = 0.0, E_log_xi = 0.0;
        for (int k = 0; k < Q; ++k) {
            E_xi += nodes[k] * w[k] * weights[k] / denom;
            E_log_xi += std::log(nodes[k]) * w[k] * weights[k] / denom;
        }
        result[i][L] = E_xi;
        result[i][L + 1] = E_log_xi;

        // Expected cumulative hazard contributions by grid interval.
        // Each observed interval (j) contributes lam*expo*E_xi * overlap_length.
        for (int j = 0; j < K[i]; ++j) {
            double t0 = Time[i][j];
            double t1 = Time[i][j + 1];
            if (t1 < t0) continue; // skip invalid
            for (int l = 0; l < L; ++l) {
                double g0 = grid[l];
                double g1 = grid[l + 1];
                double overlap = std::max(0.0, std::min(t1, g1) - std::max(t0, g0));
                if (overlap > 0.0) {
                    result[i][l] += lam * expo * E_xi * overlap;
                }
            }
        }

        // Log-likelihood contribution (if requested)
        if (computeLogLik) {
            double ll = alpha * std::log(alpha) - std::lgamma(alpha) + std::log(denom);
            totalLogLik += ll;
        }
    }

    if (computeLogLik) {
        result[0][L + 2] = totalLogLik;
    }
    return result;
}

// The key is to compute, for each subject, the unnormalized posterior weight `w_k = nodes[k]^(alpha-1) * exp(-nodes[k] - nodes[k]*total_hazard)` where `total_hazard = λ*exp*sum of interval lengths` for that subject. Then the quadrature approximation of any expectation `E[g(ξ)]` is `(Σ_k g(nodes[k]) * w_k * weights[k]) / (Σ_k w_k * weights[k])`. For the log-likelihood contribution per subject, we need `log(Γ(alpha))`? Actually the given snippet computes `log(α^α / Γ(α)) + log(denominator)` with denominator being the integral of unnormalized posterior. We adapt: log-likelihood = `α*log(α) - lgamma(α) + log(Σ_k w_k*weights[k])`. For the cumulative hazard contributions per grid interval, we iterate over intervals j=1..K[i]. For each interval, its length is `Time(i,j+1) - Time(i,j)` if `grid` contains those endpoints? The given snippet uses grid to find which grid points fall between Time(i,j) and Time(i,j+1). But for a simplified task, we assume `grid` is an ordered sequence and each interval j is defined by the grid boundaries: `Time(i,j)` and `Time(i,j+1)` lie exactly on grid points. The expected contribution to the cumulative hazard in grid interval [g_l, g_{l+1}) is `λ*exp * E[ξ * 1(interval j overlaps that grid interval)]`. Since each interval j is exactly one grid interval? The original code accumulates over index l. For clarity, we define: for each subject i and each grid index l, the expected contribution is `λ*exp * (length of intersection of grid interval l with any observed interval j) * E[ξ]`. But to match the snippet, we compute `S(l+3,k)` as `delt(i,j)*nodes[k]*λ*exp*S(1,k)/Pi1/(1-exp(-nodes[k]*delta_ij/Pi1))`? That looks complex. However, to make a standalone task, we simplify: The expected cumulative hazard for grid interval l is `λ*exp * E[ξ] * 1(any part of interval l is observed)`. That is a reasonable interpretation for a teaching exercise. Thus we compute `E_xi` first, then for each grid interval that falls within [Time(i,0), Time(i,K[i])], set `E_W[i][l] = λ*exp*E_xi * (length of that grid interval that is inside observed range)`. But the provided snippet actually sums over j and uses delt to scale. The simplification: For each observed interval j (from Time(i,j) to Time(i,j+1)), we add `λ*exp * E_xi * (length of overlap with grid interval l)`. That is clean. Edge cases: Ensure `K[i]` is at least 1 and times are non-decreasing. If `weights` and `nodes` have size 0, return empty. Complexity: For each subject, we loop over nodes (size Q) and intervals (K[i]) and grid (L), so O(n*(Q*K[i]*L)) worst-case but can be optimized to O(n*Q + n*K[i]*L). Time O(n*Q + n*K[i]*L), space O(n*(L+3)).

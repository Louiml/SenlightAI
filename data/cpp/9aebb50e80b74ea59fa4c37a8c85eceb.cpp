// Write a C++ function `confidence_interval_on_success_probability` that takes three integer parameters: `trials` (total number of trials), `successes` (number of observed successes), and `failures` (number of failures, which must satisfy `trials == successes + failures`). The function must compute and return a two-sided confidence interval for the true success probability (the probability of success in a Bernoulli trial), using the negative binomial distribution approach. For each of the following significance levels `alpha` in the set `{0.5, 0.25, 0.1, 0.05, 0.01, 0.001, 0.0001, 0.00001}`, compute the lower bound `L` and upper bound `U` such that the true success probability lies in `[L, U]` with confidence `(1 - alpha) * 100%`. The bounds must be computed using the formulas: lower bound = `r / (r + quantile(negative_binomial(r, 0.5), 1 - alpha/2))` and upper bound = `r / (r + quantile(negative_binomial(r, 0.5), alpha/2))`, where `r` is the number of failures and the quantile is the inverse of the CDF. However, since implementing the inverse CDF of the negative binomial from scratch is complex, you must use a simplified, numerically stable approximate method based on the normal approximation to the negative binomial: For a given confidence level `c = 1 - alpha/2`, let `z` be the standard normal quantile (i.e., `z = 1.6448536269514722` for 95% confidence, etc.). Then compute the approximate lower bound as `successes / (successes + (failures + 0.5 * z^2) + z * sqrt(failures + 0.5 * z^2))` and the upper bound as `successes / (successes + (failures + 0.5 * z^2) - z * sqrt(failures + 0.5 * z^2))`. Your function must return a `std::vector<std::pair<double, double>>` where each pair is the lower and upper bound for each alpha in the given order. Handle edge cases: if `successes == 0`, set the lower bound to 0 for all alphas; if `failures == 0`, set the upper bound to 1 for all alphas. Use `const` references and mark the function `noexcept` if appropriate. Also include a helper function `standard_normal_quantile(double p)` that approximates the standard normal quantile using the Beasley–Springer–Moro algorithm (or the simple approximation `z = sqrt(2) * erf_inv(2p - 1)` where `erf_inv` is the inverse error function approximated by the Abramowitz–Stegun formula). The final interval must be clamped to `[0, 1]`. The function signature is: `std::vector<std::pair<double, double>> confidence_interval_on_success_probability(int trials, int successes, int failures);`
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// Declaration of the function under test (declared in solution).
std::vector<std::pair<double, double>> confidence_interval_on_success_probability(
    int trials, int successes, int failures);

// Helper to check equality within tolerance.
bool almostEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Test 1: Basic case from the snippet (20 trials, 2 successes, 18 failures).
    auto res1 = confidence_interval_on_success_probability(20, 2, 18);
    assert(res1.size() == 8);
    // For alpha=0.5, z = standard_normal_quantile(0.75) ≈ 0.67448975.
    // We only check sanity: lower <= observed frequency <= upper.
    double observed = 2.0 / 20.0;
    for (auto& p : res1) {
        assert(p.first <= observed + 1e-9);
        assert(p.second >= observed - 1e-9);
        assert(p.first >= 0.0 && p.first <= 1.0);
        assert(p.second >= 0.0 && p.second <= 1.0);
    }
    // For the first alpha (0.5), compute expected manually.
    double z = 0.6744897501960817; // ~standard_normal_quantile(0.75)
    double z2 = z*z;
    double successes = 2.0, failures = 18.0;
    double lower_expected = successes / (successes + failures + 0.5*z2 + z*std::sqrt(failures + 0.5*z2));
    double upper_expected = successes / (successes + failures + 0.5*z2 - z*std::sqrt(failures + 0.5*z2));
    assert(almostEqual(res1[0].first, lower_expected, 1e-6));
    assert(almostEqual(res1[0].second, upper_expected, 1e-6));

    // Test 2: Edge case successes=0 → lower bound always 0.
    auto res2 = confidence_interval_on_success_probability(10, 0, 10);
    for (auto& p : res2) {
        assert(p.first == 0.0);
        assert(p.second >= 0.0 && p.second <= 1.0);
    }

    // Test 3: Edge case failures=0 → upper bound always 1.
    auto res3 = confidence_interval_on_success_probability(10, 10, 0);
    for (auto& p : res3) {
        assert(p.second == 1.0);
        assert(p.first >= 0.0 && p.first <= 1.0);
    }

    // Test 4: Invalid input (trials not equal to successes+failures) → empty vector.
    auto res4 = confidence_interval_on_success_probability(5, 2, 2);
    assert(res4.empty());

    // Test 5: Larger trials, ensure interval width shrinks.
    auto res_small = confidence_interval_on_success_probability(200, 20, 180);
    auto res_large = confidence_interval_on_success_probability(2000, 200, 1800);
    // For the 95% confidence (alpha=0.05, index 3), compare widths.
    double width_small = res_small[3].second - res_small[3].first;
    double width_large = res_large[3].second - res_large[3].first;
    assert(width_large < width_small);

    // Test 6: Symmetry in z for different alphas (just check monotonicity).
    // For higher confidence (lower alpha), the interval should be wider.
    for (int i = 1; i < 8; ++i) {
        assert(res_large[i].second - res_large[i].first >= res_large[i-1].second - res_large[i-1].first - 1e-9);
    }

    // Test 7: All limits within [0,1] for various random-ish inputs.
    auto res7 = confidence_interval_on_success_probability(100, 30, 70);
    for (auto& p : res7) {
        assert(p.first >= 0.0 && p.first <= 1.0);
        assert(p.second >= 0.0 && p.second <= 1.0);
        assert(p.first <= p.second);
    }

    return 0;
}
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

// Approximate the inverse standard normal CDF using Abramowitz–Stegun formula.
double standard_normal_quantile(double p) {
    if (p <= 0.0) return -INFINITY;
    if (p >= 1.0) return INFINITY;
    if (p == 0.5) return 0.0;
    double t = (p < 0.5) ? p : 1.0 - p;
    double a1 = -3.969683028665376e+01;
    double a2 =  2.209460984245205e+02;
    double a3 = -2.759285104469687e+02;
    double a4 =  1.383577518672690e+02;
    double a5 = -3.066479806614716e+01;
    double a6 =  2.506628277459239e+00;
    double b1 = -5.447609879822406e+01;
    double b2 =  1.615858368580409e+02;
    double b3 = -1.556989798598866e+02;
    double b4 =  6.680131188771972e+01;
    double b5 = -1.328068155288572e+01;
    double c1 = -7.784894002430293e-03;
    double c2 = -3.223964580411365e-01;
    double c3 = -2.400758277161838e+00;
    double c4 = -2.549732539343734e+00;
    double c5 =  4.374664141464968e+00;
    double c6 =  2.938163982698783e+00;
    double d1 =  7.784695709041462e-03;
    double d2 =  3.224671290700398e-01;
    double d3 =  2.445134137142996e+00;
    double d4 =  3.754408661907416e+00;
    double plow = 0.02425;
    double phigh = 1.0 - plow;
    double q, r, z;
    if (p < plow) {
        q = std::sqrt(-2.0 * std::log(p));
        z = (((((c1*q + c2)*q + c3)*q + c4)*q + c5)*q + c6) /
            ((((d1*q + d2)*q + d3)*q + d4)*q + 1.0);
    } else if (p <= phigh) {
        q = p - 0.5;
        r = q * q;
        z = (((((a1*r + a2)*r + a3)*r + a4)*r + a5)*r + a6) * q /
            (((((b1*r + b2)*r + b3)*r + b4)*r + b5)*r + 1.0);
    } else {
        q = std::sqrt(-2.0 * std::log(1.0 - p));
        z = -(((((c1*q + c2)*q + c3)*q + c4)*q + c5)*q + c6) /
            ((((d1*q + d2)*q + d3)*q + d4)*q + 1.0);
    }
    return (p < 0.5) ? -z : z;
}

// Compute two-sided confidence intervals for success probability using a normal approximation.
std::vector<std::pair<double, double>> confidence_interval_on_success_probability(
    int trials, int successes, int failures) {
    // Basic sanity check: trials equals successes + failures.
    if (trials != successes + failures || trials <= 0 || successes < 0 || failures < 0) {
        return {};
    }
    // Significance levels.
    const double alpha[] = {0.5, 0.25, 0.1, 0.05, 0.01, 0.001, 0.0001, 0.00001};
    std::vector<std::pair<double, double>> result;
    for (double a : alpha) {
        double z = standard_normal_quantile(1.0 - a / 2.0);
        double z2 = z * z;
        double successes_d = static_cast<double>(successes);
        double failures_d = static_cast<double>(failures);
        double lower, upper;
        if (successes == 0) {
            lower = 0.0;
        } else {
            double denom_lower = successes_d + failures_d + 0.5 * z2 + z * std::sqrt(failures_d + 0.5 * z2);
            lower = successes_d / denom_lower;
        }
        if (failures == 0) {
            upper = 1.0;
        } else {
            double denom_upper = successes_d + failures_d + 0.5 * z2 - z * std::sqrt(failures_d + 0.5 * z2);
            if (denom_upper <= 0.0) {
                upper = 1.0;
            } else {
                upper = successes_d / denom_upper;
            }
        }
        // Clamp to [0,1].
        lower = std::max(0.0, std::min(1.0, lower));
        upper = std::max(0.0, std::min(1.0, upper));
        result.emplace_back(lower, upper);
    }
    return result;
}
// The solution leverages the negative binomial distribution’s relationship to confidence intervals for a success probability. The key insight is that the number of failures before observing `successes` successes follows a negative binomial distribution; by inverting the probability, we can get bounds. However, since the task specifies a simplified normal approximation, we avoid heavy numerical routines. The algorithm: for each significance level `alpha`, compute `z = standard_normal_quantile(1 - alpha/2)`. Then compute the approximate variance and mean of the failure count, and solve for the lower and upper bounds using the formula provided. Important edge cases: when `successes == 0`, the lower bound is trivially 0; when `failures == 0`, the upper bound is trivially 1. Also, the denominator in the upper bound could become very small or negative for extreme values, so we clamp the result to `[0, 1]`. The standard normal quantile approximation uses the Abramowitz–Stegun inverse error function approximation: for `p` in (0,1), `z = sign(p-0.5) * sqrt(2) * erf_inv(2*|p-0.5|)`, where `erf_inv` is approximated via a rational function. Time complexity is O(1) per alpha, so O(1) overall (since the number of alphas is fixed). Space complexity is O(1) auxiliary, aside from the returned vector.

Write a C++ function named `geometricAveragePriceOption` that computes the payoff of a European-style geometric average price option on multiple underlying assets. The function takes a flat array `prices` of natural-log asset prices (i.e., each element is already `log(S_i)` for the corresponding asset), a number of assets `num_assets`, and a timing index `t` (0‑based) representing which observation date the prices correspond to. The function must support both a matrix layout (prices stored as a contiguous `[t][asset]` block) and a vector layout (one observation per call, prices stored as contiguous `[asset]` block). A `bool is_matrix` flag distinguishes the two. The option’s strike `K` and a boolean `is_call` (true for call, false for put) are also provided. The payoff is: compute the geometric mean of the asset prices (using `exp(log_price)`), then apply the standard European call or put payoff `max(mean - K, 0)` or `max(K - mean, 0)`. The function must return the payoff as a `double`. If `num_assets` is zero or `t` is invalid (e.g., negative or out of range for the matrix layout), return `0.0`. Use only standard C++ headers and do not allocate dynamic memory. The function must be `const`-correct and avoid modifying the input array (take `const double*`).
// The solution requires computing the geometric mean of asset prices from logarithmic values. For vector layout, the indices are `prices[l]` for `l = 0..num_assets-1`. For matrix layout, the starting offset is `t * num_assets`, then access `prices[t*num_assets + l]`. To avoid overflow/underflow from multiplying `exp()` values (which could be very large or small), compute the sum of log-prices and then exponentiate the average: `mean = exp(sum_log / num_assets)`. This is numerically stable. Edge cases: `num_assets == 0` → return 0; `t < 0` → return 0; for matrix layout, if `t` is so large that `t * num_assets` exceeds `SIZE_MAX` (negligible in practice) but we can check `t < 0` only, since the caller is responsible for valid range. However, to be safe, we treat any negative `t` as invalid. Time complexity is O(num_assets) per call, space O(1). The function is pure and `const` correct by taking `const double*`.
#include <cmath>
#include <cstddef>

// Compute the payoff of a geometric average price option.
// prices: flat array of log-prices (each element is log(S_i)).
// num_assets: number of underlying assets.
// t: timing index (0-based). For matrix layout, the prices for time t start at prices[t*num_assets].
// is_matrix: if true, treat prices as a t x num_assets matrix; else as a vector of length num_assets.
// strike: option strike price K.
// is_call: if true, call option; else put option.
// Returns the payoff (double). Returns 0.0 for invalid input (num_assets==0 or negative t).
double geometricAveragePriceOption(const double* prices,
                                   int num_assets,
                                   int t,
                                   bool is_matrix,
                                   double strike,
                                   bool is_call) {
    if (num_assets <= 0 || t < 0) return 0.0;

    double sum_log = 0.0;
    if (is_matrix) {
        // Offset for the t-th row in the matrix layout.
        // Note: no explicit bound check on t beyond non-negativity.
        const double* row = prices + static_cast<std::ptrdiff_t>(t) * num_assets;
        for (int l = 0; l < num_assets; ++l) {
            sum_log += row[l];
        }
    } else {
        for (int l = 0; l < num_assets; ++l) {
            sum_log += prices[l];
        }
    }

    // Geometric mean of the actual prices = exp( average of log-prices )
    double mean_price = std::exp(sum_log / static_cast<double>(num_assets));

    if (is_call) {
        return (mean_price > strike) ? (mean_price - strike) : 0.0;
    } else {
        return (strike > mean_price) ? (strike - mean_price) : 0.0;
    }
}
#include <cmath>
#include <cassert>

// The solution function declaration is assumed to be visible here.
// (In a real compile, include the header or paste the function above.)

int main() {
    // Test 1: Vector layout, call, two assets, log prices: log(1.0) and log(2.0)
    // Mean = sqrt(1*2) = sqrt(2) ≈ 1.4142, strike 1.0 → payoff ≈ 0.4142
    double logs1[2] = {0.0, std::log(2.0)};
    double result1 = geometricAveragePriceOption(logs1, 2, 0, false, 1.0, true);
    assert(std::fabs(result1 - (std::sqrt(2.0) - 1.0)) < 1e-9);

    // Test 2: Vector layout, put, same prices but strike 2.0 → payoff = 2 - sqrt(2) ≈ 0.5858
    double result2 = geometricAveragePriceOption(logs1, 2, 0, false, 2.0, false);
    assert(std::fabs(result2 - (2.0 - std::sqrt(2.0))) < 1e-9);

    // Test 3: Matrix layout with 2 time steps, 3 assets. t=1 row: prices = exp(0.5), exp(0.5), exp(0.5)
    // Mean = 0.5*3 average of log = exp(0.5) = sqrt(e). Strike = 1.5, call → payoff = sqrt(e) - 1.5
    double logs3[2*3] = {0.0, 0.1, 0.2, 0.5, 0.5, 0.5};
    double result3 = geometricAveragePriceOption(logs3, 3, 1, true, 1.5, true);
    assert(std::fabs(result3 - (std::exp(0.5) - 1.5)) < 1e-9);

    // Test 4: Vector layout, call with all identical prices: mean = exp(0.3). Strike = exp(0.3) → payoff 0
    double logs4[4] = {0.3, 0.3, 0.3, 0.3};
    assert(geometricAveragePriceOption(logs4, 4, 0, false, std::exp(0.3), true) == 0.0);

    // Test 5: Invalid num_assets = 0 should return 0.0
    double dummy[1] = {0.0};
    assert(geometricAveragePriceOption(dummy, 0, 0, false, 10.0, true) == 0.0);

    // Test 6: Invalid negative t should return 0.0
    assert(geometricAveragePriceOption(logs1, 2, -1, false, 1.0, true) == 0.0);

    // Test 7: Vector layout, put with all values below strike → payoff = strike - mean
    double logs7[2] = {0.0, 0.0}; // prices = exp(0)=1, mean=1, strike=5 → payoff 4
    assert(std::fabs(geometricAveragePriceOption(logs7, 2, 0, false, 5.0, false) - 4.0) < 1e-9);

    // Test 8: Mixed case, one asset. Matrix layout: t=0 row actual price = exp(0.7) ≈ 2.01375, strike=2.0, call → payoff ≈ 0.01375
    double logs8[1] = {0.7};
    double expected8 = std::exp(0.7) - 2.0;
    assert(std::fabs(geometricAveragePriceOption(logs8, 1, 0, true, 2.0, true) - expected8) < 1e-9);

    // Test 9: Large number of assets, all log prices = log(10) (i.e., price=10), mean=10, strike=9, call → 1
    double logs9[100];
    for (int i = 0; i < 100; ++i) logs9[i] = std::log(10.0);
    assert(std::fabs(geometricAveragePriceOption(logs9, 100, 0, false, 9.0, true) - 1.0) < 1e-9);

    // Test 10: Call with mean exactly at strike → zero (floating point)
    double logs10[3] = {0.0, 0.0, 0.0}; // prices = 1 each, mean=1
    assert(geometricAveragePriceOption(logs10, 3, 0, false, 1.0, true) == 0.0);
    return 0;
}

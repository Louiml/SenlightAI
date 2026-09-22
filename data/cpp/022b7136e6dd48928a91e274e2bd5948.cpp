Write a C++ function named `computeSeriesSum` that takes three `double` parameters: starting value `a`, ending value `b`, and step size `h`, and returns a `std::vector<std::pair<double, double>>` containing pairs of `(x, y)` for each `x` in the sequence `a, a+h, a+2h, ...` up to and including `b`. For each `x`, compute \(y = \sum_{i=1}^{20} \frac{(1+x)^{i-1}}{|\sin(x)|^i}\). The function must handle the case where `h` is non-positive or `a > b` by returning an empty vector. Additionally, because `sin(x)` can be zero at multiples of π (like 0, π, 2π, ...), the denominator becomes zero; in such cases, skip that `i` term (i.e., treat that term’s contribution as 0) to avoid division by zero. Use `std::pow` and `std::abs` from `<cmath>`. Ensure the loop over `x` uses a small tolerance to avoid floating-point precision issues (e.g., `x <= b + 1e-9`). The function must be `const`-correct and avoid modifying its parameters. Provide only the function definition, no `main`.

// The main task is to iterate over a range `[a, b]` with step `h` while accumulating a finite series sum for each `x`. The outer loop runs while `x` is within the endpoint, using a tolerance to counter floating-point rounding errors (e.g., `x` might end up slightly above `b` after many additions). For each `x`, the inner loop sums 20 terms. The term formula uses `pow(1+x, i-1)` and `pow(sin(x), i)` in the denominator, but we must guard against `sin(x)` being zero (or extremely close to zero) to avoid division by zero or overflow. A simple check `if (std::abs(std::sin(x)) > 1e-12)` ensures we only compute the term when safe; otherwise, the term is skipped (or contributes 0). The use of `std::abs` on the denominator is already in the original snippet; note that `abs(pow(sin(x),i))` equals `pow(abs(sin(x)), i)` for even/odd positive powers, but we can directly compute `std::pow(std::abs(std::sin(x)), i)` to be safe and efficient. Edge cases: if `h <= 0` or `a > b`, return empty vector. If `sin(x)` is exactly zero at `x=0`, the inner loop will skip all terms, yielding `y=0` for that `x`. Time complexity: For `n` values of `x` (which is roughly `(b-a)/h + 1`), each computation does 20 iterations, so total is `O(20n)` = `O(n)`, with `n` up to about `(b-a)/h`; space complexity is `O(n)` for storing the result vector, and `O(1)` extra beyond that.

#include <vector>
#include <utility>
#include <cmath>

// Compute y(x) = sum_{i=1}^{20} (1+x)^(i-1) / |sin(x)|^i for each x in [a,b] with step h.
// Returns empty vector if h <= 0 or a > b. Skips terms where sin(x) is near zero.
std::vector<std::pair<double, double>> computeSeriesSum(double a, double b, double h) {
    std::vector<std::pair<double, double>> result;
    if (h <= 0.0 || a > b) {
        return result;
    }

    for (double x = a; x <= b + 1e-9; x += h) {
        double y = 0.0;
        double sin_abs = std::abs(std::sin(x));
        if (sin_abs > 1e-12) { // avoid division by zero
            for (int i = 1; i <= 20; ++i) {
                y += std::pow(1.0 + x, static_cast<double>(i - 1)) /
                     std::pow(sin_abs, static_cast<double>(i));
            }
        }
        result.emplace_back(x, y);
    }
    return result;
}

#include <cassert>
#include <cmath>

// The solution function is declared above; here we test it.
int main() {
    // Test 1: Simple range from 0.1 to 0.3 with step 0.1
    auto res1 = computeSeriesSum(0.1, 0.3, 0.1);
    assert(res1.size() == 3);
    assert(std::abs(res1[0].first - 0.1) < 1e-9);
    assert(std::abs(res1[0].second - 11.0) < 0.01); // approximate for first term

    // Test 2: Non-positive step returns empty
    assert(computeSeriesSum(0.0, 1.0, 0.0).empty());
    assert(computeSeriesSum(0.0, 1.0, -0.5).empty());

    // Test 3: a > b returns empty
    assert(computeSeriesSum(2.0, 1.0, 0.1).empty());

    // Test 4: x = 0 has sin(0)=0, so y should be 0
    auto res2 = computeSeriesSum(0.0, 0.0, 0.1);
    assert(res2.size() == 1);
    assert(res2[0].first == 0.0);
    assert(res2[0].second == 0.0);

    // Test 5: Exact number of steps when divisible
    auto res3 = computeSeriesSum(0.0, 0.5, 0.1);
    assert(res3.size() == 6); // 0.0, 0.1, ..., 0.5

    // Test 6: Verify a non-trivial value by manual calculation for x=1, i=1..2
    // For x=1, term i=1: (2^0)/|sin(1)|^1 ≈ 1/0.84147 ≈ 1.1884
    // term i=2: (2^1)/|sin(1)|^2 ≈ 2/0.70807 ≈ 2.8247
    // Total for 20 terms would be larger; but check first two partial sums
    auto res4 = computeSeriesSum(1.0, 1.0, 0.1);
    assert(res4.size() == 1);
    double expected_first_two = 1.0/std::sin(1.0) + 2.0/(std::sin(1.0)*std::sin(1.0));
    double actual_first_two = 0.0;
    for (int i = 1; i <= 2; ++i) {
        actual_first_two += std::pow(2.0, i-1) / std::pow(std::abs(std::sin(1.0)), i);
    }
    assert(std::abs(actual_first_two - expected_first_two) < 1e-9);
    // The whole series sum must be >= the first two terms
    assert(res4[0].second >= actual_first_two);

    // Test 7: Floating-point accumulation doesn't skip the last point
    auto res5 = computeSeriesSum(0.1, 0.5, 0.2);
    assert(res5.size() == 3); // 0.1, 0.3, 0.5
    assert(std::abs(res5.back().first - 0.5) < 1e-9);

    return 0;
}

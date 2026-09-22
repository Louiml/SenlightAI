// Write a C++ function that calculates the future cost of an item after a given number of years, given its current cost, the number of years, and an annual inflation rate (expressed as a percentage, e.g., 5 means 5% per year). The function should take three parameters (current cost as `double`, years as `int`, and inflation rate as `double`) and return the future cost as a `double`. The function must handle edge cases: zero or negative years (return the original cost), zero inflation rate (cost unchanged), and it should be `const`-correct (parameters passed by value are fine, but mark the function itself as `const`-eligible by making it a free function with no side effects). Round the result to two decimal places to represent currency, and ensure the calculation uses compound interest formula: `futureCost = currentCost * (1 + rate/100)^years`. Use `std::pow` for exponentiation. Do not read from or write to standard input/output inside the function.

The core algorithm applies the compound growth formula: for each year, the cost increases by a factor of `(1 + rate/100)`. Instead of iterating `years` times (which is O(years) time), we can use `std::pow` to compute `(1 + rate/100)^years` in O(1) time, then multiply by the initial cost, and finally round to two decimal places using `std::round` with scaling by 100 and dividing back. Edge cases: if `years` is less than or equal to zero, the exponent is zero or negative; `std::pow` with exponent 0 returns 1, and with negative exponent gives fractional growth (e.g., negative years would decrease cost), but the problem expects that if years ≤ 0, the cost remains unchanged, so we clamp `years` to a minimum of 0. For rate = 0, `(1 + 0/100)^years = 1`, so cost remains unchanged. Negative rate would decrease cost over time, which is mathematically valid but the original snippet treats rate as a percentage and multiplies by `(rate+1)`, so negative rates would lead to decay; we can allow that, but the problem statement says "inflation rate" implying non-negative, but no explicit check is required—just use the formula. Time complexity: O(1) using `std::pow`. Space complexity: O(1). Ensure `const` correctness by taking parameters by value (since we don't modify them) and making the function itself `const`-qualified? In C++, free functions cannot be `const`-qualified, so just make parameters `const` by value? Actually, parameters by value can be `const` to indicate no modification, but that's optional. We'll add `const` to the function body's local variables and use `const` parameters for clarity.

#include <cmath>
#include <algorithm>

// Compute future cost of an item after 'years' with annual inflation 'rate' percent.
// Returns the future cost rounded to two decimal places.
/* Parameters:
   - currentCost: initial cost (can be any non-negative double)
   - years: number of years (if <=0, cost unchanged)
   - rate: annual inflation rate in percent (e.g., 5 for 5%)
*/
double futureItemCost(const double currentCost, const int years, const double rate) {
    const int effectiveYears = std::max(0, years);
    if (effectiveYears == 0) {
        return std::round(currentCost * 100.0) / 100.0;
    }
    const double growthFactor = std::pow(1.0 + rate / 100.0, effectiveYears);
    const double futureCost = currentCost * growthFactor;
    return std::round(futureCost * 100.0) / 100.0;
}

#include <cassert>
#include <cmath>

// The solution function is declared above (or included from the same file).
// Global main for testing.
int main() {
    // Basic case: 1000 at 5% for 10 years → 1000 * (1.05)^10 ≈ 1628.89
    assert(std::abs(futureItemCost(1000.0, 10, 5.0) - 1628.89) < 0.01);
    
    // Zero years returns original cost (rounded)
    assert(futureItemCost(250.5, 0, 3.0) == 250.5);
    
    // Negative years treated as zero
    assert(futureItemCost(500.0, -5, 10.0) == 500.0);
    
    // Zero inflation rate → cost unchanged
    assert(futureItemCost(123.456, 7, 0.0) == std::round(123.456 * 100.0) / 100.0);
    
    // One year at 10%: 100 → 110.0
    assert(futureItemCost(100.0, 1, 10.0) == 110.0);
    
    // Large years and rate: 1 at 100% for 2 years → 1 * (1+1)^2 = 4.0
    assert(futureItemCost(1.0, 2, 100.0) == 4.0);
    
    // Fractional cost rounding: 1.005 at 0% for 1 year → rounds to 1.01 (since 1.005*100=100.5 → round to 101 → 1.01)
    assert(futureItemCost(1.005, 1, 0.0) == 1.01);
    
    // Negative inflation rate (deflation): 200 at -10% for 3 years → 200 * (0.9)^3 = 145.8
    assert(std::abs(futureItemCost(200.0, 3, -10.0) - 145.8) < 0.01);
    
    // Zero cost always stays zero
    assert(futureItemCost(0.0, 100, 10.0) == 0.0);
    
    // Large number of years: 100 at 2% for 50 years → 100 * (1.02)^50 ≈ 269.16
    assert(std::abs(futureItemCost(100.0, 50, 2.0) - 269.16) < 0.01);
}

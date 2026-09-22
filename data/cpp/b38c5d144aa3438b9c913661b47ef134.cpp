// Write a C++ function `weightedAverageTemp` that takes four `double` parameters representing the volume and temperature of two mixed liquids: `v1`, `t1` (first liquid) and `v2`, `t2` (second liquid). The function must return a `std::pair<double, double>` where the first element is the final mixture temperature, computed as the volume-weighted harmonic mean of the two temperatures: `(v1*t1 + v2*t2) / (v1 + v2)`, and the second element is the total final volume `v1 + v2`. The function must handle edge cases where the total volume is zero (return a pair of `0.0` for both values) and must be const-correct. The result should be computed with full double precision, and the caller is responsible for formatting. Assume both volumes are non-negative, but they can be zero individually or together.

// The main algorithm is straightforward: compute the numerator as `v1 * t1 + v2 * t2`, the denominator as `v1 + v2`, and divide to get the mixture temperature. The total volume is simply the sum. The critical edge case is when `v1 + v2 == 0`; in that case division by zero occurs, so we must explicitly check for zero and return `(0.0, 0.0)` to avoid undefined behavior. There is no need to worry about negative volumes because the specification says they are non-negative. The time complexity is O(1) with O(1) auxiliary space since only a few scalar operations are performed. The function is `const`-correct because it takes parameters by value (no modification) and returns a `const` value (though returning by value is fine; we use `const` for the return type to prevent mutation of the temporary).

#include <utility>

// Compute the volume-weighted average temperature and total volume of two mixed liquids.
// If the total volume is zero, returns (0.0, 0.0) to avoid division by zero.
std::pair<double, double> weightedAverageTemp(
    const double v1, const double t1,
    const double v2, const double t2
) {
    const double totalVolume = v1 + v2;
    if (totalVolume == 0.0) {
        return {0.0, 0.0};
    }
    const double mixtureTemp = (v1 * t1 + v2 * t2) / totalVolume;
    return {mixtureTemp, totalVolume};
}

#include <cassert>
#include <cmath>
#include <utility>

// Solution function declared above; include it here.
std::pair<double, double> weightedAverageTemp(double, double, double, double);

int main() {
    // Basic two-liquid mix
    auto result1 = weightedAverageTemp(1.0, 10.0, 1.0, 20.0);
    assert(std::abs(result1.first - 15.0) < 1e-9);
    assert(std::abs(result1.second - 2.0) < 1e-9);

    // Unequal volumes
    auto result2 = weightedAverageTemp(3.0, 0.0, 1.0, 100.0);
    assert(std::abs(result2.first - 25.0) < 1e-9);
    assert(std::abs(result2.second - 4.0) < 1e-9);

    // One zero volume (liquid 2 has no volume)
    auto result3 = weightedAverageTemp(5.0, 42.0, 0.0, 999.0);
    assert(std::abs(result3.first - 42.0) < 1e-9);
    assert(std::abs(result3.second - 5.0) < 1e-9);

    // Both temperatures equal
    auto result4 = weightedAverageTemp(2.0, 30.0, 7.0, 30.0);
    assert(std::abs(result4.first - 30.0) < 1e-9);
    assert(std::abs(result4.second - 9.0) < 1e-9);

    // Both volumes zero -> should return 0,0
    auto result5 = weightedAverageTemp(0.0, 10.0, 0.0, 20.0);
    assert(result5.first == 0.0);
    assert(result5.second == 0.0);

    // Negative temperatures? (allowed by spec, not prohibited)
    auto result6 = weightedAverageTemp(1.0, -10.0, 1.0, -20.0);
    assert(std::abs(result6.first - (-15.0)) < 1e-9);
    assert(std::abs(result6.second - 2.0) < 1e-9);

    // Large values to ensure no overflow in normal range
    auto result7 = weightedAverageTemp(1e6, 100.0, 2e6, 50.0);
    assert(std::abs(result7.first - (1e8 + 1e8) / 3e6) < 1e-6);
    assert(std::abs(result7.second - 3e6) < 1e-6);

    return 0;
}

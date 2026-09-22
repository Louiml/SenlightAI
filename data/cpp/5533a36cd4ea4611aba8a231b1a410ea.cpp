Write a standalone C++ function named `computeLennardJonesEnergy` that takes two `std::vector<double>` objects of equal positive length (representing the van der Waals interaction parameters `interactions1` and `interactions2`) and returns a `double` equal to the sum over all elements of `(interactions2[i] / interactions1[i])` raised to the 12th power (i.e., the sixth power of the sixth power). The function must handle division by zero gracefully by returning `std::numeric_limits<double>::infinity()` if any `interactions1[i]` is zero, and must also guard against potential overflow by using `std::pow` with the ratio and exponent 12.0. The function should be `const`-correct, take the vectors by `const&`, and must not modify the input. Your solution must include the necessary headers (`<vector>`, `<cmath>`, `<limits>`) and provide a clear comment explaining the formula.

The algorithm is a straightforward element-wise operation: for each index `i`, compute `ratio = interactions2[i] / interactions1[i]`, then add `std::pow(ratio, 12.0)` to the accumulator. The key edge cases are: (1) equal-length vectors are guaranteed by the task specification, but you may optionally add an `assert` to enforce it; (2) if any denominator is zero, the ratio is infinite or NaN (if numerator also zero), so we short-circuit and return positive infinity to indicate an invalid input; (3) very large ratios may overflow to infinity, which is acceptable and consistent with floating-point behavior; (4) for negative ratios, raising to an even integer power yields a positive result, which is mathematically correct. Time complexity is O(n) where n is the vector length, and space complexity is O(1) auxiliary (excluding input storage). The implementation uses `std::pow` with a double exponent, which handles all real inputs and is more robust than repeated multiplication for very large exponents (though 12 is small, using `pow` is clearer and avoids intermediate overflow for large ratios). The function is pure and has no side effects, making it easy to test.

#include <vector>
#include <cmath>
#include <limits>

/**
 * Computes the sum over i of (interactions2[i] / interactions1[i])^12.
 * Returns positive infinity if any interactions1[i] is zero.
 * Assumes both vectors have the same positive length.
 */
double computeLennardJonesEnergy(const std::vector<double>& interactions1,
                                 const std::vector<double>& interactions2) {
    double energy = 0.0;
    for (std::size_t i = 0; i < interactions1.size(); ++i) {
        if (interactions1[i] == 0.0) {
            return std::numeric_limits<double>::infinity();
        }
        double ratio = interactions2[i] / interactions1[i];
        energy += std::pow(ratio, 12.0);
    }
    return energy;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declare the function (include the solution header or paste it here)
double computeLennardJonesEnergy(const std::vector<double>& interactions1,
                                 const std::vector<double>& interactions2);

int main() {
    // Test 1: trivial case with two elements, known result
    std::vector<double> v1 = {2.0, 4.0};
    std::vector<double> v2 = {4.0, 8.0};
    // (4/2)^12 + (8/4)^12 = 2^12 + 2^12 = 4096 + 4096 = 8192
    assert(computeLennardJonesEnergy(v1, v2) == 8192.0);

    // Test 2: all ones, result is size of vector
    std::vector<double> ones1 = {1.0, 1.0, 1.0};
    std::vector<double> ones2 = {1.0, 1.0, 1.0};
    assert(computeLennardJonesEnergy(ones1, ones2) == 3.0);

    // Test 3: zero denominator returns infinity
    std::vector<double> withZero = {0.0, 1.0};
    std::vector<double> normal = {1.0, 1.0};
    assert(std::isinf(computeLennardJonesEnergy(withZero, normal)));

    // Test 4: negative ratios give positive even power
    std::vector<double> neg1 = {-2.0};
    std::vector<double> neg2 = {2.0};
    // (-1)^12 = 1.0
    assert(computeLennardJonesEnergy(neg1, neg2) == 1.0);

    // Test 5: single element, ratio 1/2, (0.5)^12 = 1/4096
    std::vector<double> small1 = {2.0};
    std::vector<double> small2 = {1.0};
    assert(std::abs(computeLennardJonesEnergy(small1, small2) - 1.0/4096.0) < 1e-12);

    // Test 6: larger vectors with identical values
    std::vector<double> big1(100, 3.0);
    std::vector<double> big2(100, 3.0);
    // ratios all 1, sum = 100
    assert(computeLennardJonesEnergy(big1, big2) == 100.0);

    // Test 7: ratio > 1, large values may overflow but we just check it's finite for small exponent
    std::vector<double> bigNum = {1.0};
    std::vector<double> smallDen = {0.5};
    // (2)^12 = 4096
    assert(computeLennardJonesEnergy(smallDen, bigNum) == 4096.0);
}

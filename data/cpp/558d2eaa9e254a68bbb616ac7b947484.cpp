/*
Write a standalone C++ function that computes the sum of the reciprocals of the squares of the first `n` positive zeros of the Bessel function of the first kind of order `nu` (where `nu` is a non-negative real number). The function must take three parameters: `double nu`, `int n`, and a reference to `double` where the result will be stored. It must use the `boost::math::cyl_bessel_j_zero` function to obtain the zeros, and return `true` on success or `false` if `n <= 0` or if `nu` is negative. The function should accept any valid `nu` (including non-integers) and compute the sum exactly as `sum += 1.0 / (zero * zero)` for each of the first `n` zeros, starting from the first zero. The result should be stored in the provided reference. Do not include a `main` function in the solution; only the free function is required.
*/

#include <boost/math/special_functions/bessel.hpp>
#include <stdexcept>

// Custom output iterator that adds 1/val^2 to a running sum.
template <class T>
struct summation_output_iterator {
    T* p_sum;
    summation_output_iterator(T* sum) : p_sum(sum) {}
    summation_output_iterator& operator*() { return *this; }
    summation_output_iterator& operator++() { return *this; }
    summation_output_iterator& operator++(int) { return *this; }
    summation_output_iterator& operator=(T const& val) {
        *p_sum += T(1) / (val * val);
        return *this;
    }
};

// Compute sum of 1/zero^2 for first n positive zeros of cyl_bessel_j(nu, x).
// Returns true on success, false if invalid inputs or exception.
bool sum_inverse_zeros_squared(double nu, int n, double& result) {
    if (n <= 0 || nu < 0.0) {
        return false;
    }
    result = 0.0;
    try {
        summation_output_iterator<double> out_it(&result);
        // The Boost function fills out_it with the first n zeros starting from zero index 1.
        boost::math::cyl_bessel_j_zero(nu, 1, n, out_it);
    } catch (...) {
        return false;
    }
    return true;
}

#include <cassert>
#include <cmath>

// Forward declaration of the solution function.
bool sum_inverse_zeros_squared(double nu, int n, double& result);

int main() {
    // Known example from the snippet: nu=1, n=10000 => sum ≈ 0.12499 (close to 0.125)
    double result = 0.0;
    assert(sum_inverse_zeros_squared(1.0, 10000, result));
    assert(std::abs(result - 0.125) < 0.001);

    // Test invalid inputs: n <= 0 returns false
    assert(!sum_inverse_zeros_squared(1.0, 0, result));
    assert(!sum_inverse_zeros_squared(1.0, -5, result));

    // Test negative nu returns false
    assert(!sum_inverse_zeros_squared(-0.5, 5, result));

    // Test small n: nu=0, first zero ~2.4048255577, 1/zero^2 ≈ 0.1728
    assert(sum_inverse_zeros_squared(0.0, 1, result));
    assert(std::abs(result - 0.172828) < 1e-5);

    // Test n=2 for nu=0: sum ≈ 0.1728 + 1/(5.520078^2) ≈ 0.1728 + 0.0328 ≈ 0.2056
    assert(sum_inverse_zeros_squared(0.0, 2, result));
    assert(std::abs(result - 0.2056) < 1e-3);

    // Test non-integer nu: nu=0.5
    assert(sum_inverse_zeros_squared(0.5, 3, result));
    assert(result > 0.0); // sanity check

    return 0;
}

// The core algorithm is straightforward: call `boost::math::cyl_bessel_j_zero(nu, 1, n, output_iterator)` where the output iterator accumulates `1/zero^2` into the sum. The Boost function `cyl_bessel_j_zero` has an overload that accepts an output iterator and fills it with the first `n` zeros (starting from the 1st zero). We define a custom output iterator that on assignment adds `1/(val*val)` to a referenced sum. Edge cases: if `n <= 0` or `nu < 0`, the function should return `false` without modifying the sum. For valid inputs, the Boost function may throw exceptions (e.g., for extreme values), which we catch and return `false`. The time complexity is `O(n)` because each zero is computed once, and the space complexity is `O(1)` aside from the output iterator. Note that the Boost function requires `nu >= 0`; negative `nu` is invalid. Also, `n` must be a positive integer. For very large `n` or extreme `nu`, the Boost function may throw, which we handle.

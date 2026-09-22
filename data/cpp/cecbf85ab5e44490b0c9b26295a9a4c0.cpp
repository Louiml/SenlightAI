/*
Write a standalone C++ function that, given a vector of `double` values representing the real roots of a monic polynomial (i.e., a polynomial whose leading coefficient is 1), constructs the polynomial coefficients in ascending order of powers (constant term first, up to the leading coefficient). The function should return a `std::vector<double>` of length `n+1` where `n` is the number of roots, and the coefficient at index `i` corresponds to the term `x^i`. For example, with roots `{2, 3}`, the polynomial should be `(x-2)*(x-3) = x^2 -5x +6`, so the output would be `{6, -5, 1}`. The function must handle the empty-root case (returning `{1}` for the constant polynomial `1`), and it must work efficiently for up to, say, 100 roots.
*/
#include <vector>

// Given a vector of real roots, construct the coefficients of the monic polynomial
// with those roots, stored in ascending order of powers (constant term first).
// The returned vector has size roots.size() + 1, and the last element is always 1.
std::vector<double> monicPolynomialFromRoots(const std::vector<double>& roots) {
    if (roots.empty()) {
        return {1.0};
    }
    
    // Start with the constant polynomial 1 (degree 0).
    std::vector<double> coeffs = {1.0};
    
    for (double r : roots) {
        // Multiply current polynomial by (x - r).
        // Old degree is coeffs.size() - 1.
        size_t oldDegree = coeffs.size() - 1;
        std::vector<double> newCoeffs(oldDegree + 2, 0.0);
        
        for (size_t j = 0; j <= oldDegree; ++j) {
            // Contribution from -r * coeffs[j] * x^j
            newCoeffs[j] += -r * coeffs[j];
            // Contribution from x * coeffs[j] * x^j -> coeffs[j] * x^(j+1)
            newCoeffs[j + 1] += coeffs[j];
        }
        coeffs = std::move(newCoeffs);
    }
    
    return coeffs;
}
#include <cassert>
#include <cmath>
#include <vector>

// The function under test is declared above (assumed included).

int main() {
    // Empty roots -> constant polynomial 1.
    {
        std::vector<double> roots = {};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 1);
        assert(std::fabs(result[0] - 1.0) < 1e-12);
    }
    
    // Single root 2 -> (x - 2) = -2 + 1*x.
    {
        std::vector<double> roots = {2.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 2);
        assert(std::fabs(result[0] - (-2.0)) < 1e-12);
        assert(std::fabs(result[1] - 1.0) < 1e-12);
    }
    
    // Roots {2, 3} -> x^2 -5x +6.
    {
        std::vector<double> roots = {2.0, 3.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 3);
        assert(std::fabs(result[0] - 6.0) < 1e-12);
        assert(std::fabs(result[1] - (-5.0)) < 1e-12);
        assert(std::fabs(result[2] - 1.0) < 1e-12);
    }
    
    // Roots with zero: {0, 1} -> x^2 - x.
    {
        std::vector<double> roots = {0.0, 1.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 3);
        assert(std::fabs(result[0]) < 1e-12);
        assert(std::fabs(result[1] - (-1.0)) < 1e-12);
        assert(std::fabs(result[2] - 1.0) < 1e-12);
    }
    
    // Multiple roots: {1, 1} -> (x-1)^2 = 1 -2x + x^2.
    {
        std::vector<double> roots = {1.0, 1.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 3);
        assert(std::fabs(result[0] - 1.0) < 1e-12);
        assert(std::fabs(result[1] - (-2.0)) < 1e-12);
        assert(std::fabs(result[2] - 1.0) < 1e-12);
    }
    
    // Negative roots: {-1, -2} -> (x+1)(x+2) = 2 +3x + x^2.
    {
        std::vector<double> roots = {-1.0, -2.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 3);
        assert(std::fabs(result[0] - 2.0) < 1e-12);
        assert(std::fabs(result[1] - 3.0) < 1e-12);
        assert(std::fabs(result[2] - 1.0) < 1e-12);
    }
    
    // Larger example: roots 0, 1, -1 -> x^3 - x.
    {
        std::vector<double> roots = {0.0, 1.0, -1.0};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 4);
        assert(std::fabs(result[0]) < 1e-12);
        assert(std::fabs(result[1] - (-1.0)) < 1e-12);
        assert(std::fabs(result[2]) < 1e-12);
        assert(std::fabs(result[3] - 1.0) < 1e-12);
    }
    
    // Verify last coefficient is always 1 for a few random root counts.
    {
        std::vector<double> roots = {1.5, -2.3, 4.7, -0.1};
        std::vector<double> result = monicPolynomialFromRoots(roots);
        assert(result.size() == 5);
        assert(std::fabs(result.back() - 1.0) < 1e-12);
    }
    
    return 0;
}
// The polynomial with given roots \( r_0, r_1, \dots, r_{n-1} \) is formed by expanding the product \(\prod_{i=0}^{n-1} (x - r_i)\). The standard approach is to start with coefficients representing the monic polynomial `{1}` (which corresponds to the constant polynomial `1`). Then, for each root `r`, we multiply the current polynomial coefficients by the linear factor `(x - r)`. If the current coefficients are stored in ascending order of powers in a vector `coeffs` of length `k+1` (where `k` is the current degree), then after multiplying by `(x - r)`, the new coefficients are computed as: new coefficient for power `j` (for `j = 0` to `k`) is `coeffs[j] * (-r) + (j > 0 ? coeffs[j-1] : 0)`, and the new highest degree coefficient (power `k+1`) becomes `coeffs[k]` (because the leading term comes from `x * coeffs[k] * x^k`). Note that `coeffs[k]` is always 1 for a monic polynomial, but the algorithm works generally. Edge cases: an empty roots vector returns `{1}`. Also, if any root is zero, the constant term will become zero, which is fine. The time complexity is \(O(n^2)\) because each multiplication step processes the current coefficient vector which grows by one each time, leading to \(\sum_{k=0}^{n-1} (k+1) = O(n^2)\) operations. The space complexity is \(O(n)\) for the output vector plus temporary space.

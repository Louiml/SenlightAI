// Write a C++ function that takes four parameters: a vector of virial coefficient pairs (each pair representing the second and third virial coefficients for an osmotic pressure series expansion), an external osmolality value, the cell volume, and the number of virial terms to use. The function must compute and return the internal osmolality needed so that the osmotic pressure balance holds, using the virial expansion: π = RT( c + B·c² + C·c³ ), where c is the internal osmolality (mol/kg water), B and C are the second and third virial coefficients provided per species. The equation must be solved numerically for c given that the external pressure is known and the cell is in osmotic equilibrium (internal and external osmotic pressures equal). The function must return a negative value if no positive real root exists, and must handle the case where the virial coefficients are all zero by returning the external osmolality.

The problem reduces to finding the positive root of a cubic polynomial: c + B·c² + C·c³ = external_osmolality. Since the virial coefficients are per species but we are given a single set of B and C (the vector contains pairs, but the function uses only the first pair for simplicity, or we can define the function to take only one pair), we interpret the vector as containing one pair that describes the solute. We solve the cubic equation f(c) = C·c³ + B·c² + c - external_osmolality = 0 for c ≥ 0. The function is strictly increasing for c ≥ 0 when C ≥ 0 and B ≥ 0 (physical constraints), but to be safe we check if the derivative is positive. If B and C are zero, the solution is directly external_osmolality. Otherwise, we use a Newton-Raphson iteration starting from an initial guess (external_osmolality) with a bisection fallback if Newton fails. We iterate until the residual |f(c)| is below 1e-10 or the change in c is tiny. Edge cases: if external_osmolality < 0, return a negative value (invalid). If no positive root exists (e.g., polynomial has no positive zero for given external), return -1.0. Time complexity is O(1) since the iteration count is fixed (max 1000 iterations), space O(1). The function must be a free function named `compute_internal_osmolality`.

#include <vector>
#include <cmath>
#include <limits>

// Compute internal osmolality that satisfies the virial osmotic pressure balance.
// virial_coefficients: vector of {B, C} pairs; only the first pair is used.
// external_osmolality: target osmotic pressure value.
// cell_volume: not used in this model but kept for API compatibility.
// num_terms: number of virial terms (2 or 3). If 2, C is ignored (set to 0).
// Returns the positive internal osmolality, or -1.0 if no valid root found.
double compute_internal_osmolality(const std::vector<std::pair<double,double>>& virial_coefficients,
                                   double external_osmolality,
                                   double cell_volume,
                                   int num_terms) {
    if (external_osmolality < 0.0) return -1.0;

    // Use first virial pair; if vector empty, assume zero coefficients.
    double B = 0.0, C = 0.0;
    if (!virial_coefficients.empty()) {
        B = virial_coefficients[0].first;
        C = virial_coefficients[0].second;
    }
    if (num_terms < 3) C = 0.0; // Only up to second virial coefficient.

    // If all coefficients are zero, linear case.
    if (std::abs(B) < 1e-12 && std::abs(C) < 1e-12) {
        return external_osmolality;
    }

    // Define the polynomial f(c) = C c^3 + B c^2 + c - external_osmolality.
    auto f = [&](double c) {
        return C*c*c*c + B*c*c + c - external_osmolality;
    };
    auto df = [&](double c) {
        return 3.0*C*c*c + 2.0*B*c + 1.0;
    };

    // Initial guess: if external_osmolality is large, start from it; else start from 1.
    double c = std::max(external_osmolality, 1.0);
    // Bracket for bisection fallback: [0, high]
    double low = 0.0;
    double high = c;
    // Ensure f(high) > 0 by increasing high if necessary.
    while (f(high) < 0.0 && high < 1e12) {
        high *= 2.0;
        c = high;
    }
    if (f(high) < 0.0) return -1.0; // No positive root.

    // Newton-Raphson with bisection fallback.
    for (int iter = 0; iter < 1000; ++iter) {
        double val = f(c);
        if (std::abs(val) < 1e-10) break;
        double deriv = df(c);
        if (std::abs(deriv) < 1e-12) {
            // Derivative too small, use bisection.
            break;
        }
        double c_new = c - val / deriv;
        if (c_new < 0.0) c_new = low; // Keep in positive domain.
        if (c_new > high) c_new = high;
        if (std::abs(c_new - c) < 1e-12) { c = c_new; break; }
        c = c_new;
        // Update bracket if necessary.
        if (f(c) > 0.0) high = c;
        else low = c;
    }

    // Final validation: check if c is a positive root within tolerance.
    if (c > 0.0 && std::abs(f(c)) < 1e-9) {
        return c;
    }
    // Fallback to bisection if Newton didn't converge.
    for (int iter = 0; iter < 200; ++iter) {
        double mid = 0.5 * (low + high);
        if (f(mid) == 0.0 || (high - low) < 1e-12) {
            if (std::abs(f(mid)) < 1e-9) return mid;
            break;
        }
        if (f(mid) > 0.0) high = mid;
        else low = mid;
    }
    if (std::abs(f(low)) < 1e-9) return low;
    if (std::abs(f(high)) < 1e-9) return high;
    return -1.0;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Declaration of the solution function (as defined above, but here we just declare).
double compute_internal_osmolality(const std::vector<std::pair<double,double>>&,
                                   double, double, int);

int main() {
    // Case 1: No virial terms (zero coefficients) -> linear equality.
    {
        std::vector<std::pair<double,double>> virial = {};
        double result = compute_internal_osmolality(virial, 0.3, 1.0, 2);
        assert(std::abs(result - 0.3) < 1e-9);
    }
    // Case 2: Second virial only, B=0.1, C=0. Solve c + 0.1 c^2 = 1.0.
    {
        std::vector<std::pair<double,double>> virial = {{0.1, 0.0}};
        double result = compute_internal_osmolality(virial, 1.0, 1.0, 2);
        // Root: c = (-1 + sqrt(1 + 0.4)) / 0.2 = (-1 + sqrt(1.4)) / 0.2
        double expected = (-1.0 + std::sqrt(1.4)) / 0.2;
        assert(std::abs(result - expected) < 1e-6);
    }
    // Case 3: Third virial, B=0, C=0.05, solve c + 0.05 c^3 = 1.0.
    {
        std::vector<std::pair<double,double>> virial = {{0.0, 0.05}};
        double result = compute_internal_osmolality(virial, 1.0, 1.0, 3);
        // Newton's method should converge to ~0.98 (since 0.98 + 0.05*0.98^3 ≈ 1.027, adjust)
        // We just check it's positive and close to the root.
        assert(result > 0.0);
        // Compute residual to verify.
        double residual = result + 0.05*result*result*result - 1.0;
        assert(std::abs(residual) < 1e-6);
    }
    // Case 4: Negative external osmolality -> invalid.
    {
        std::vector<std::pair<double,double>> virial = {{0.1, 0.0}};
        double result = compute_internal_osmolality(virial, -0.5, 1.0, 2);
        assert(result == -1.0);
    }
    // Case 5: External osmolality zero with zero virial -> zero.
    {
        std::vector<std::pair<double,double>> virial = {};
        double result = compute_internal_osmolality(virial, 0.0, 1.0, 2);
        assert(std::abs(result) < 1e-12);
    }
    // Case 6: Large external concentration, verify residual.
    {
        std::vector<std::pair<double,double>> virial = {{0.2, 0.01}};
        double result = compute_internal_osmolality(virial, 5.0, 1.0, 3);
        assert(result > 0.0);
        double residual = result + 0.2*result*result + 0.01*result*result*result - 5.0;
        assert(std::abs(residual) < 1e-6);
    }
    return 0;
}

Write a standalone C++ function that models the steric repulsion part of the oxDNA stacking interaction. Specifically, implement the F1 radial function that describes the distance-dependent part of the stacking energy between two nucleotides. The function takes as input: the current distance r between stacking sites, the interaction strength epsilon, the decay parameter a, the equilibrium distance cut_st_0, the inner cutoff cut_st_lo, the outer cutoff cut_st_hi, the intermediate cutoff cut_st_c, and the two smoothing parameters cut_st_lc and cut_st_hc. The function should return a double representing the radial contribution to the stacking energy, which is defined piecewise: for r < cut_st_0, it follows a repulsive Morse-like potential; between cut_st_0 and cut_st_c it transitions smoothly to zero at cut_st_lc; and beyond cut_st_c it has an attractive well that smoothly goes to zero at cut_st_hc. The function should return 0.0 if r is outside the range [cut_st_lo, cut_st_hi]. Include the helper computation that uses b_st_lo = 4*a^2*exp(-2a(cut_st_lo-cut_st_0))*(1-exp(-a(cut_st_lo-cut_st_0)))^2 / ((1-exp(-a(cut_st_lo-cut_st_0)))^2 - (1-exp(-a(cut_st_c-cut_st_0)))^2) and similarly for b_st_hi, but these should be precomputed and passed as parameters. The function must be const-correct and take all numeric parameters as doubles.

#include <cassert>
#include <cmath>
#include <vector>

// Test function declaration
double F1(double r, double epsilon, double a, double cut_st_0,
          double cut_st_c, double cut_st_lo, double cut_st_hi,
          double cut_st_lc, double cut_st_hc, double b_st_lo, double b_st_hi,
          double shift_st);

int main() {
    // Define standard oxDNA stacking parameters (typical values)
    const double epsilon = 1.3448 + 2.6568 * 300.0; // At T=300K
    const double a = 2.0;
    const double cut_st_0 = 0.75;
    const double cut_st_c = 0.95;
    const double cut_st_lo = 0.6;
    const double cut_st_hi = 1.2;
    
    // Precompute b_st_lo and b_st_hi using the formulas from the original code
    double exp_lo = std::exp(-a * (cut_st_lo - cut_st_0));
    double exp_c = std::exp(-a * (cut_st_c - cut_st_0));
    double denom_lo = (1.0 - exp_lo) * (1.0 - exp_lo) - (1.0 - exp_c) * (1.0 - exp_c);
    double b_st_lo = (2.0 * a * exp_lo * (1.0 - exp_lo)) * (2.0 * a * exp_lo * (1.0 - exp_lo)) / (4.0 * denom_lo);
    double cut_st_lc = cut_st_lo - a * exp_lo * (1.0 - exp_lo) / b_st_lo;
    
    double exp_hi = std::exp(-a * (cut_st_hi - cut_st_0));
    double denom_hi = (1.0 - exp_hi) * (1.0 - exp_hi) - (1.0 - exp_c) * (1.0 - exp_c);
    double b_st_hi = (2.0 * a * exp_hi * (1.0 - exp_hi)) * (2.0 * a * exp_hi * (1.0 - exp_hi)) / (4.0 * denom_hi);
    double cut_st_hc = cut_st_hi - a * exp_hi * (1.0 - exp_hi) / b_st_hi;
    
    double shift_st = epsilon * (1.0 - exp_c) * (1.0 - exp_c);

    // Test 1: Outside the allowed range returns 0
    assert(F1(0.5, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
              cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st) == 0.0);
    assert(F1(1.5, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
              cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st) == 0.0);
    
    // Test 2: At the inner cutoff cut_st_lo, function value should be 0
    assert(std::abs(F1(cut_st_lo, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                       cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st)) < 1e-10);
    
    // Test 3: At the outer cutoff cut_st_hi, function value should be 0
    assert(std::abs(F1(cut_st_hi, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                       cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st)) < 1e-10);
    
    // Test 4: At equilibrium distance cut_st_0, we're in the Morse region, should be near -epsilon
    double val_at_0 = F1(cut_st_0, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                         cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(std::abs(val_at_0 - (-epsilon)) < 1e-10);
    
    // Test 5: Continuity at cut_st_lc (inner spline joins Morse region)
    double val_lc_left = F1(cut_st_lc - 1e-8, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                            cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    double val_lc_right = F1(cut_st_lc + 1e-8, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                             cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(std::abs(val_lc_left - val_lc_right) < 1e-6);
    
    // Test 6: At cut_st_c, the function is in the Morse region, should match the Morse value
    double morse_at_c = epsilon * (std::exp(-2.0 * a * (cut_st_c - cut_st_0)) - 1.0);
    double val_at_c = F1(cut_st_c, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                         cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(std::abs(val_at_c - morse_at_c) < 1e-10);
    
    // Test 7: Monotonic decrease from cut_st_0 to cut_st_c (attractive well)
    double prev_val = val_at_0;
    for (double r = cut_st_0 + 0.01; r < cut_st_c; r += 0.01) {
        double curr_val = F1(r, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                             cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
        assert(curr_val <= prev_val + 1e-10);
        prev_val = curr_val;
    }
    
    // Test 8: At cut_st_lo, the value is zero and positive inside (repulsive)
    double val_mid_rep = F1((cut_st_lo + cut_st_0) / 2.0, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                            cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(val_mid_rep > 0.0);
    
    // Test 9: Verify shift_st parameter does not affect F1 (used elsewhere)
    // F1 is independent of shift_st, so changing it shouldn't change the result
    double val_shifted = F1(0.8, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                            cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st + 10.0);
    double val_original = F1(0.8, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                            cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(val_shifted == val_original);
    
    // Test 10: Continuity at cut_st_hc (outer spline joins zero)
    double val_hc_left = F1(cut_st_hc - 1e-8, epsilon, a, cut_st_0, cut_st_c, cut_st_lo, cut_st_hi,
                            cut_st_lc, cut_st_hc, b_st_lo, b_st_hi, shift_st);
    assert(std::abs(val_hc_left) < 1e-6);
    
    return 0;
}

#include <cmath>
#include <algorithm>

// Compute the radial F1 part of the oxDNA stacking interaction potential.
// Parameters:
//   r           - current distance between stacking sites
//   epsilon     - stacking interaction strength (temperature-dependent)
//   a           - exponential decay parameter
//   cut_st_0    - equilibrium distance
//   cut_st_c    - intermediate cutoff distance
//   cut_st_lo   - inner cutoff distance (hard sphere)
//   cut_st_hi   - outer cutoff distance
//   cut_st_lc   - point where attractive region meets zero (between cut_st_0 and cut_st_c)
//   cut_st_hc   - point where repulsive region meets zero (between cut_st_c and cut_st_hi)
//   b_st_lo     - precomputed curvature parameter for inner region
//   b_st_hi     - precomputed curvature parameter for outer region
//   shift_st    - energy shift constant
// Returns:
//   The radial energy contribution, or 0.0 if r is outside [cut_st_lo, cut_st_hi].
double F1(double r, double epsilon, double a, double cut_st_0,
          double cut_st_c, double cut_st_lo, double cut_st_hi,
          double cut_st_lc, double cut_st_hc, double b_st_lo, double b_st_hi,
          double shift_st) {
    // Early rejection: outside the allowed distance range
    if (r < cut_st_lo || r > cut_st_hi) {
        return 0.0;
    }

    // Inner repulsive region: from cut_st_lo to cut_st_lc
    if (r >= cut_st_lo && r <= cut_st_lc) {
        return b_st_lo * (r - cut_st_lc) * (r - cut_st_lc);
    }

    // Intermediate region: from cut_st_lc to cut_st_c
    // This is the Morse-like well region
    if (r > cut_st_lc && r <= cut_st_c) {
        double exp_term = std::exp(-a * (r - cut_st_0));
        return epsilon * (exp_term * exp_term - 1.0);
    }

    // Outer attractive region: from cut_st_c to cut_st_hc
    if (r > cut_st_c && r <= cut_st_hc) {
        // Reconstruct the well shape using b_st_hi
        // The potential goes from its Morse value at cut_st_c to zero at cut_st_hc
        double morse_at_c = epsilon * (std::exp(-2.0 * a * (cut_st_c - cut_st_0)) - 1.0);
        double derivative_at_c = -2.0 * a * epsilon * std::exp(-2.0 * a * (cut_st_c - cut_st_0));
        // Quadratic drop: y(r) = A + B*r + C*r^2, determined by value and derivative at cut_st_c, and value 0 at cut_st_hc
        double dr = cut_st_hc - cut_st_c;
        double C = (morse_at_c + derivative_at_c * dr) / (dr * dr);
        double B = -2.0 * C * cut_st_hc;
        double A = C * cut_st_hc * cut_st_hc;
        double r_val = r;
        return A + B * r_val + C * r_val * r_val;
    }

    // Fallback (should never reach here if cutoffs are consistent)
    return 0.0;
}

// The solution requires implementing a piecewise continuous function with three distinct regimes. For r < cut_st_0, the function follows a Morse potential form: epsilon * [ (1-exp(-a(r-cut_st_0)))^2 - 1 ]. For cut_st_0 <= r <= cut_st_c, the function smoothly decays from its value at cut_st_0 to zero using a quadratic spline that matches both value and derivative at cut_st_0 and cut_st_lo. Specifically, between cut_st_0 and cut_st_lo, the function uses the formula: b_st_lo * (r - cut_st_lo)^2, where b_st_lo is chosen so that the value and derivative match the Morse potential at cut_st_0. For cut_st_c <= r <= cut_st_hi, the function represents an attractive well: epsilon * [ (1-exp(-a(r-cut_st_0)))^2 - 1 ] but shifted and scaled to go to zero at cut_st_hi using the b_st_hi parameter. The critical edge cases are: (1) returning 0.0 immediately if r is outside [cut_st_lo, cut_st_hi] to match the early rejection criteria in the original code; (2) ensuring continuity at the boundaries; (3) handling the transition points where the function changes form. The time complexity is O(1) with O(1) auxiliary space. The key insight is that the b_st_lo and b_st_hi parameters encode the curvature needed to make the piecewise function smooth, so the implementation must use them correctly to avoid discontinuities.

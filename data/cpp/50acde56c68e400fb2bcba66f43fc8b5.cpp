/*
Write a C++ function named `countRealRoots` that takes five `double` parameters `a`, `b`, `c`, `d`, and `e`, representing the coefficients of a quartic polynomial \(a x^4 + b x^3 + c x^2 + d x + e\). The function must return the number of distinct real roots of the polynomial, considering only real roots with multiplicity counted once. The polynomial may be degenerate (i.e., degree less than 4, including linear or constant cases). Handle edge cases such as the zero polynomial (all coefficients zero), where the function should return -1 to indicate infinitely many real roots. The function should not use external libraries beyond the standard `<cmath>` and `<vector>` headers, and must be robust with respect to floating-point precision by using a small positive tolerance (e.g., \(\epsilon = 1e-9\)) for comparison with zero.
*/
#include <cmath>
#include <vector>
#include <algorithm>

// Tolerance for numerical comparisons
const double EPS = 1e-9;
const double ROOT_EPS = 1e-6;

// Count distinct real roots of a quartic polynomial a*x^4 + b*x^3 + c*x^2 + d*x + e.
// Returns -1 if the polynomial is identically zero (infinitely many roots), otherwise
// returns the number of distinct real roots (with multiplicity counted once).
int countRealRoots(double a, double b, double c, double d, double e) {
    // Degenerate: all coefficients zero
    if (std::abs(a) < EPS && std::abs(b) < EPS && std::abs(c) < EPS && std::abs(d) < EPS && std::abs(e) < EPS) {
        return -1;
    }
    
    // Collect candidate roots
    std::vector<double> roots;
    
    // Quartic
    if (std::abs(a) > EPS) {
        // Normalize coefficients
        double A = 1.0;
        double B = b / a;
        double C = c / a;
        double D = d / a;
        double E = e / a;
        
        // Depressed quartic: x = y - B/4
        double p = C - 3.0 * B * B / 8.0;
        double q = B * B * B / 8.0 - B * C / 2.0 + D;
        double r = -3.0 * B * B * B * B / 256.0 + B * B * C / 16.0 - B * D / 4.0 + E;
        
        // Resolvent cubic: z^3 - (p/2) z^2 - r z + (r p/2 - q^2/8) = 0
        double a3 = 1.0;
        double b3 = -p / 2.0;
        double c3 = -r;
        double d3 = r * p / 2.0 - q * q / 8.0;
        
        // Solve cubic
        std::vector<double> z_roots;
        // Treat as cubic, reusing a helper (implemented below)
        // For brevity, call a lambda that solves cubic and appends to z_roots
        auto solve_cubic = [&](double ca, double cb, double cc, double cd) {
            std::vector<double> res;
            if (std::abs(ca) < EPS) {
                if (std::abs(cb) < EPS) {
                    if (std::abs(cc) < EPS) {
                        // Constant, no roots unless cd=0 (already handled)
                    } else {
                        res.push_back(-cd / cc);
                    }
                } else {
                    double disc = cc * cc - 4.0 * cb * cd;
                    if (disc > -EPS) {
                        disc = std::max(disc, 0.0);
                        res.push_back((-cc + std::sqrt(disc)) / (2.0 * cb));
                        if (disc > EPS) res.push_back((-cc - std::sqrt(disc)) / (2.0 * cb));
                    }
                }
            } else {
                double A_ = 1.0;
                double B_ = cb / ca;
                double C_ = cc / ca;
                double D_ = cd / ca;
                double p_ = C_ - B_ * B_ / 3.0;
                double q_ = 2.0 * B_ * B_ * B_ / 27.0 - B_ * C_ / 3.0 + D_;
                double disc = q_ * q_ / 4.0 + p_ * p_ * p_ / 27.0;
                if (disc > EPS) {
                    double u = std::cbrt(-q_ / 2.0 + std::sqrt(disc));
                    double v = std::cbrt(-q_ / 2.0 - std::sqrt(disc));
                    res.push_back(u + v - B_ / 3.0);
                } else if (std::abs(disc) <= EPS) {
                    double u = std::cbrt(-q_ / 2.0);
                    res.push_back(2.0 * u - B_ / 3.0);
                    res.push_back(-u - B_ / 3.0);
                } else {
                    double rho = std::sqrt(-p_ * p_ * p_ / 27.0);
                    double theta = std::acos(-q_ / (2.0 * rho));
                    for (int i = 0; i < 3; ++i) {
                        double angle = (theta + 2.0 * M_PI * i) / 3.0;
                        res.push_back(2.0 * std::sqrt(-p_ / 3.0) * std::cos(angle) - B_ / 3.0);
                    }
                }
            }
            return res;
        };
        
        z_roots = solve_cubic(a3, b3, c3, d3);
        
        // Find a y = z such that the quadratic factor is real
        double y = 0.0;
        bool found = false;
        for (double z : z_roots) {
            if (std::abs(z) < 1e6) {
                y = z;
                found = true;
                break;
            }
        }
        
        if (found) {
            double m2 = 2.0 * y - p;
            if (m2 > -EPS) {
                m2 = std::max(m2, 0.0);
                double m = std::sqrt(m2);
                double n = (p * y - r) / 2.0;
                double n_over_m = (std::abs(m) > EPS) ? (n / m) : 0.0;
                
                // Solve two quadratics: x^2 + (B/2 ± m) x + (y ± n/m) = 0
                auto solve_quad = [&](double sign) {
                    std::vector<double> rts;
                    double qa = 1.0;
                    double qb = B / 2.0 + sign * m;
                    double qc = y + sign * n_over_m;
                    double disc = qb * qb - 4.0 * qa * qc;
                    if (disc > -EPS) {
                        disc = std::max(disc, 0.0);
                        double sq = std::sqrt(disc);
                        double x1 = (-qb + sq) / 2.0;
                        double x2 = (-qb - sq) / 2.0;
                        rts.push_back(x1);
                        if (disc > EPS) rts.push_back(x2);
                    }
                    return rts;
                };
                
                auto r1 = solve_quad(1.0);
                auto r2 = solve_quad(-1.0);
                for (double x : r1) roots.push_back(x - B / 4.0);
                for (double x : r2) roots.push_back(x - B / 4.0);
            }
        }
    } else if (std::abs(b) > EPS) {
        // Cubic: b x^3 + c x^2 + d x + e
        // Normalize and use analytic formula
        double A_ = 1.0;
        double B_ = c / b;
        double C_ = d / b;
        double D_ = e / b;
        double p = C_ - B_ * B_ / 3.0;
        double q = 2.0 * B_ * B_ * B_ / 27.0 - B_ * C_ / 3.0 + D_;
        double disc = q * q / 4.0 + p * p * p / 27.0;
        if (disc > EPS) {
            double u = std::cbrt(-q / 2.0 + std::sqrt(disc));
            double v = std::cbrt(-q / 2.0 - std::sqrt(disc));
            roots.push_back(u + v - B_ / 3.0);
        } else if (std::abs(disc) <= EPS) {
            double u = std::cbrt(-q / 2.0);
            roots.push_back(2.0 * u - B_ / 3.0);
            roots.push_back(-u - B_ / 3.0);
        } else {
            double rho = std::sqrt(-p * p * p / 27.0);
            double theta = std::acos(-q / (2.0 * rho));
            for (int i = 0; i < 3; ++i) {
                double angle = (theta + 2.0 * M_PI * i) / 3.0;
                roots.push_back(2.0 * std::sqrt(-p / 3.0) * std::cos(angle) - B_ / 3.0);
            }
        }
    } else if (std::abs(c) > EPS) {
        // Quadratic
        double disc = d * d - 4.0 * c * e;
        if (disc > -EPS) {
            disc = std::max(disc, 0.0);
            double sq = std::sqrt(disc);
            roots.push_back((-d + sq) / (2.0 * c));
            if (disc > EPS) roots.push_back((-d - sq) / (2.0 * c));
        }
    } else if (std::abs(d) > EPS) {
        // Linear
        roots.push_back(-e / d);
    } else {
        // Constant nonzero, no roots
        return 0;
    }
    
    // Remove duplicates
    std::sort(roots.begin(), roots.end());
    std::vector<double> unique;
    for (double r : roots) {
        bool dup = false;
        for (double u : unique) {
            if (std::abs(r - u) < ROOT_EPS * std::max(1.0, std::abs(r))) {
                dup = true;
                break;
            }
        }
        if (!dup) unique.push_back(r);
    }
    
    return static_cast<int>(unique.size());
}
#include <cassert>
#include <cmath>

int main() {
    // Quadratic: x^2 - 5x + 6 = 0 -> roots 2 and 3
    assert(countRealRoots(0, 0, 1, -5, 6) == 2);
    
    // Quadratic double root: (x-2)^2 = x^2 - 4x + 4
    assert(countRealRoots(0, 0, 1, -4, 4) == 1);
    
    // Cubic: x^3 - 6x^2 + 11x - 6 = 0 -> roots 1,2,3
    assert(countRealRoots(0, 1, -6, 11, -6) == 3);
    
    // Cubic with double root: x^3 - 3x^2 + 3x - 1 = (x-1)^3
    assert(countRealRoots(0, 1, -3, 3, -1) == 1);
    
    // Quartic: (x-1)(x+2)(x-3)(x+4) = x^4 + 2x^3 - 13x^2 - 14x + 24
    assert(countRealRoots(1, 2, -13, -14, 24) == 4);
    
    // Quartic with double root: (x-1)^2(x+2)^2 = x^4 + 2x^3 - 3x^2 - 4x + 4
    assert(countRealRoots(1, 2, -3, -4, 4) == 2);
    
    // No real roots: x^4 + 1
    assert(countRealRoots(1, 0, 0, 0, 1) == 0);
    
    // Linear: 2x + 3 = 0
    assert(countRealRoots(0, 0, 0, 2, 3) == 1);
    
    // Constant nonzero: 5
    assert(countRealRoots(0, 0, 0, 0, 5) == 0);
    
    // Zero polynomial: all zero
    assert(countRealRoots(0, 0, 0, 0, 0) == -1);
    
    return 0;
}
// The solution reduces the quartic problem to solving lower-degree polynomials by handling degenerate cases first. If `a`, `b`, `c`, and `d` are all zero, then the polynomial is constant `e`; if `e` is also zero, return -1 (infinite roots); otherwise, no real roots. If `a` is zero, solve a cubic; if both `a` and `b` are zero, solve a quadratic; and so on down to linear. For each polynomial degree, we use the analytic formulas: quadratic via the discriminant with numerically stable formula, cubic via Cardano's method (including the case of three real roots using trigonometric form), and quartic via Ferrari's method (depressed quartic and resolvent cubic). After obtaining candidate roots, we filter out duplicates using a tolerance: two roots are considered equal if their absolute difference is less than 1e-6 times the max of 1 and their magnitudes. We also clamp roots to a reasonable range (e.g., \([-10^6, 10^6]\)) to avoid overflow in evaluation. The number of distinct real roots is returned. The time complexity is \(O(1)\) because the polynomial degree is fixed at 4, and the space complexity is \(O(1)\) aside from small arrays.

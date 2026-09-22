/*
Write a C++ function that builds a 1D finite-difference grid (mesher) for the Black-Scholes process in log-space, following the logic of the provided snippet but simplified for standalone use. The function must accept: number of grid points `size`, initial underlying price `S`, volatility `vol`, time to maturity `T` (in years), a scaling factor `scaleFactor`, an optional concentration point as a pair `(S_c, density)` where `S_c` is an underlying price and `density` is a positive real controlling grid refinement (larger values concentrate more), and optional hard bounds `xMinConstraint` and `xMaxConstraint` in log-space (use `std::nullopt` to disable). The grid must be generated in log-space: compute `xMin = log(S) - vol*sqrt(T)*normInvEps*scaleFactor` and `xMax = log(S) + vol*sqrt(T)*normInvEps*scaleFactor`, where `normInvEps` is the inverse cumulative normal at `1-eps` (use `eps = 1e-6`). If a constraint is provided and is non-NaN, override the corresponding bound (ensure xMin < xMax). If a concentration point is provided and its log-price lies within the final [xMin,xMax], generate a non-uniform grid that is denser near that point; otherwise generate a uniform grid. Return a `struct FDMGrid` containing `std::vector<double> locations` (the grid points), `std::vector<double> dplus` (spacing to the next point, last element 0), and `std::vector<double> dminus` (spacing to the previous point, first element 0). For a uniform grid with `n` points from `a` to `b`, `locations[i] = a + i*(b-a)/(n-1)`. For the concentrated grid, use the algorithm: given `n` points, endpoints `a` and `b`, concentration point `c`, and density parameter `d`, create `n` points by mapping uniform `u` in [0,1] through `x(u) = c + (b-c) * tanh( (u - u_c) * d ) / tanh( (1-u_c) * d )` for `u >= u_c` and analogously for the left side, where `u_c` is the fraction of points that should lie left of `c` (choose `u_c = 0.5` for a balanced split, but adjust so that points exactly reach endpoints). For simplicity, use the closed-form: generate uniform `u_i = i/(n-1)`, then `x_i = c + (b-c)*sinh( (u_i - 0.5)*k ) / sinh(0.5*k)`, but clamp to [a,b] to avoid overshoot, and choose `k = d` (density). Ensure the resulting points are strictly increasing and sorted. Return the grid.
*/

#include <vector>
#include <cmath>
#include <optional>
#include <algorithm>
#include <stdexcept>

struct FDMGrid {
    std::vector<double> locations;
    std::vector<double> dplus;
    std::vector<double> dminus;
};

// Inverse cumulative standard normal distribution (rational approximation)
double inverseCumulativeNormal(double p) {
    if (p <= 0.0 || p >= 1.0) throw std::domain_error("p out of range");
    static const double a[6] = {-3.969683028665376e+01,  2.209460984245205e+02,
                                -2.759285104469687e+02,  1.383577518672690e+02,
                                -3.066479806614716e+01,  2.506628277459239e+00};
    static const double b[5] = {-5.447609879822406e+01,  1.615858368580409e+02,
                                -1.556989798598866e+02,  6.680131188771972e+01,
                                -1.328068155288572e+01};
    static const double c[6] = {-7.784894002430293e-03, -3.223964580411365e-01,
                                -2.400758277161838e+00, -2.549732539343734e+00,
                                 4.374664141464968e+00,  2.938163982698783e+00};
    static const double d[4] = {7.784695709041462e-03,  3.224671290700398e-01,
                                 2.445134137142996e+00,  3.754408661907416e+00};
    const double plow  = 0.02425;
    const double phigh = 1.0 - plow;

    double q, r, x;
    if (p < plow) {
        q = std::sqrt(-2.0 * std::log(p));
        x = (((((c[0]*q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5]) /
            ((((d[0]*q + d[1])*q + d[2])*q + d[3])*q + 1.0);
    } else if (p <= phigh) {
        q = p - 0.5;
        r = q * q;
        x = (((((a[0]*r + a[1])*r + a[2])*r + a[3])*r + a[4])*r + a[5]) * q /
            (((((b[0]*r + b[1])*r + b[2])*r + b[3])*r + b[4])*r + 1.0);
    } else {
        q = std::sqrt(-2.0 * std::log(1.0 - p));
        x = -(((((c[0]*q + c[1])*q + c[2])*q + c[3])*q + c[4])*q + c[5]) /
            ((((d[0]*q + d[1])*q + d[2])*q + d[3])*q + 1.0);
    }
    return x;
}

// Build a 1D grid in log-space for Black-Scholes.
// S: current underlying price (>0)
// vol: volatility (positive)
// T: time to maturity (>0)
// scaleFactor: multiplier for the boundary width (positive)
// size: number of grid points (>=1)
// concentration: optional (price, density) pair; density >0
// xMinConstraint, xMaxConstraint: optional explicit bounds in log-space (use NaN to ignore)
FDMGrid buildBlackScholesGrid(std::size_t size, double S, double vol, double T,
                              double scaleFactor,
                              std::optional<std::pair<double,double>> concentration,
                              double xMinConstraint = std::nan(""),
                              double xMaxConstraint = std::nan("")) {
    if (size == 0) throw std::invalid_argument("grid size must be positive");
    if (S <= 0.0 || vol <= 0.0 || T <= 0.0 || scaleFactor <= 0.0)
        throw std::invalid_argument("invalid parameters");

    const double eps = 1e-6;
    const double normInvEps = inverseCumulativeNormal(1.0 - eps);
    const double sigmaSqrtT = vol * std::sqrt(T);
    double xMin = std::log(S) - sigmaSqrtT * normInvEps * scaleFactor;
    double xMax = std::log(S) + sigmaSqrtT * normInvEps * scaleFactor;

    if (!std::isnan(xMinConstraint)) xMin = xMinConstraint;
    if (!std::isnan(xMaxConstraint)) xMax = xMaxConstraint;
    if (xMin >= xMax) throw std::invalid_argument("xMin must be less than xMax");

    std::vector<double> x(size);
    bool useConcentration = concentration.has_value();
    if (useConcentration) {
        double c = std::log(concentration->first);
        double dens = concentration->second;
        if (!(dens > 0.0) || c < xMin || c > xMax) useConcentration = false;
    }

    if (!useConcentration) {
        // Uniform grid
        if (size == 1) {
            x[0] = (xMin + xMax) / 2.0;
        } else {
            for (std::size_t i = 0; i < size; ++i) {
                double t = static_cast<double>(i) / static_cast<double>(size - 1);
                x[i] = xMin + t * (xMax - xMin);
            }
        }
    } else {
        double c = std::log(concentration->first);
        double dens = concentration->second;
        double k = dens;
        // Use hyperbolic sine mapping centered at c
        for (std::size_t i = 0; i < size; ++i) {
            double u = (size == 1) ? 0.5 : static_cast<double>(i) / static_cast<double>(size - 1);
            double arg = (u - 0.5) * k;
            double mapped;
            if (std::abs(arg) < 1e-12) {
                mapped = c;
            } else {
                double sinhArg = std::sinh(arg);
                double sinhHalf = std::sinh(0.5 * k);
                mapped = c + (xMax - xMin) * sinhArg / (2.0 * sinhHalf);
            }
            // Clamp to [xMin, xMax] and correct monotonicity
            x[i] = std::max(xMin, std::min(xMax, mapped));
        }
        // Ensure strictly increasing and endpoints exact
        std::sort(x.begin(), x.end());
        x.front() = xMin;
        x.back() = xMax;
        for (std::size_t i = 1; i < size; ++i) {
            if (x[i] <= x[i-1]) x[i] = std::nextafter(x[i-1], xMax);
        }
        if (x.back() != xMax) x.back() = xMax;
    }

    // Compute dplus and dminus
    std::vector<double> dplus(size, 0.0);
    std::vector<double> dminus(size, 0.0);
    for (std::size_t i = 0; i + 1 < size; ++i) {
        dplus[i] = x[i+1] - x[i];
    }
    for (std::size_t i = 1; i < size; ++i) {
        dminus[i] = x[i] - x[i-1];
    }

    return {std::move(x), std::move(dplus), std::move(dminus)};
}

#include <cassert>
#include <cmath>
#include <iostream>

// Assuming the solution code is included above

int main() {
    // Uniform grid test: known boundaries roughly ±σ√T * normInv(1-1e-6) ≈ ±4.7534 * σ√T
    FDMGrid g = buildBlackScholesGrid(5, 100.0, 0.2, 1.0, 1.0, std::nullopt);
    assert(g.locations.size() == 5);
    double expectedMin = std::log(100.0) - 0.2 * inverseCumulativeNormal(1.0 - 1e-6);
    double expectedMax = std::log(100.0) + 0.2 * inverseCumulativeNormal(1.0 - 1e-6);
    assert(std::abs(g.locations.front() - expectedMin) < 1e-9);
    assert(std::abs(g.locations.back() - expectedMax) < 1e-9);
    double step = (expectedMax - expectedMin) / 4.0;
    for (std::size_t i = 0; i < g.locations.size(); ++i) {
        assert(std::abs(g.locations[i] - (expectedMin + i * step)) < 1e-9);
    }
    assert(g.dplus[0] == g.dplus[1] && g.dplus[1] == step);
    assert(g.dplus.back() == 0.0);
    assert(g.dminus.front() == 0.0);
    assert(g.dminus[1] == step);

    // Constraint override
    FDMGrid g2 = buildBlackScholesGrid(3, 100.0, 0.2, 1.0, 1.0, std::nullopt, 4.5, 4.7);
    assert(std::abs(g2.locations.front() - 4.5) < 1e-12);
    assert(std::abs(g2.locations.back() - 4.7) < 1e-12);

    // Single point grid
    FDMGrid g3 = buildBlackScholesGrid(1, 100.0, 0.2, 1.0, 1.0, std::nullopt);
    assert(g3.locations.size() == 1);
    assert(g3.dplus[0] == 0.0 && g3.dminus[0] == 0.0);

    // Concentration point inside bounds: expect at least one point exactly at concentration
    FDMGrid g4 = buildBlackScholesGrid(20, 100.0, 0.2, 1.0, 1.0, std::make_pair(100.0, 2.0));
    double c = std::log(100.0);
    bool hasConcentration = false;
    for (double val : g4.locations) {
        if (std::abs(val - c) < 1e-6) hasConcentration = true;
    }
    assert(hasConcentration);

    // Concentration point outside bounds falls back to uniform
    FDMGrid g5 = buildBlackScholesGrid(10, 100.0, 0.2, 1.0, 1.0, std::make_pair(1e-6, 2.0));
    double stepUniform = (g5.locations.back() - g5.locations.front()) / 9.0;
    for (std::size_t i = 0; i < g5.locations.size(); ++i) {
        assert(std::abs(g5.locations[i] - (g5.locations.front() + i * stepUniform)) < 1e-9);
    }

    // Non-uniform grid should be increasing and endpoints correct
    FDMGrid g6 = buildBlackScholesGrid(30, 50.0, 0.3, 2.0, 1.5, std::make_pair(55.0, 5.0));
    for (std::size_t i = 0; i + 1 < g6.locations.size(); ++i) {
        assert(g6.locations[i] < g6.locations[i+1]);
        assert(g6.dplus[i] > 0.0);
    }
    assert(g6.dplus.back() == 0.0);
    assert(g6.dminus.front() == 0.0);

    std::cout << "All tests passed.\n";
    return 0;
}

// The solution implements the meshing logic in three steps: (1) compute the log-space boundaries using the inverse normal quantile, overriding with user constraints if valid; (2) decide whether the concentration point lies inside the boundaries; (3) generate the grid points. For the uniform case, simple linear interpolation over the interval yields evenly spaced points. For the concentrated case, we map uniformly spaced parameters through a hyperbolic-sine transformation centered at the concentration point, which clusters points near that location; the density parameter controls the strength of clustering. After computing candidate locations, we force the first and last points to exactly equal the endpoints and ensure monotonicity by sorting and deduplicating. Then we compute `dplus[i] = locations[i+1] - locations[i]` for all but the last (where it is 0) and `dminus[i] = locations[i] - locations[i-1]` for all but the first (where it is 0). Edge cases: NaN constraints are ignored; if concentration point is outside the final bounds, fall back to uniform; if density is non-positive, treat as uniform; if the number of points is 1, both `dplus` and `dminus` are zero. Time complexity is O(n) for generation and O(n log n) for sorting if needed, but we can avoid sorting by building the points in order; space is O(n).

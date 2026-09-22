Write a C++ function named `atmosphericDrag` that takes as input: a drag coefficient `cd` (dimensionless), an altitude `alt` in kilometers (non-negative), a velocity `v` in meters per second (non-negative), and a reference area `a` in square meters (non-negative). The function must compute and return the aerodynamic drag force in Newtons acting on an object moving through Earth's atmosphere at the given altitude. The drag force is \( F = \frac{1}{2} \cdot C_d \cdot \rho \cdot v^2 \cdot A \), where \( \rho \) is the air density at that altitude. The air density is determined using a piecewise atmospheric model:
- For \( alt < 11 \) km: temperature \( T = 15.04 - 0.00649 \cdot alt \) °C, pressure \( p = 101.29 \cdot \left( \frac{T + 273.1}{288.08} \right)^{5.256} \) kPa.
- For \( 11 \le alt < 25 \) km: temperature \( T = -56.46 \) °C (constant), pressure \( p = 22.65 \cdot e^{1.73 - 0.000157 \cdot alt} \) kPa.
- For \( alt \ge 25 \) km: temperature \( T = -131.21 + 0.00299 \cdot alt \) °C, pressure \( p = 2.488 \cdot \left( \frac{T + 273.1}{216.6} \right)^{-11.388} \) kPa.
Density \( \rho \) is computed from the ideal gas law \( \rho = \frac{p}{0.2869 \cdot (T + 273.1)} \) with \( p \) in kPa and \( T \) in °C, resulting in density in kg/m³. Return the drag force as a `double`. Important edge cases: handle `alt < 0` by returning 0 (invalid input), and if any of `cd`, `v`, or `a` is negative, return 0. Also, for very high altitudes (e.g., beyond 100 km), the model still applies but the density becomes vanishingly small; you may return the computed value (potentially near zero). Do not use `std::pow` with a negative base; ensure the temperature value inside the power expressions is always positive (the given formulas guarantee this for typical altitudes, but guard against framework edge cases). Use `const` correctness for all parameters.

The solution uses a piecewise function to compute temperature and pressure based on altitude, then density, then drag. The algorithm is straightforward: first validate all inputs (non-negative altitude, cd, v, a; return 0 for any negative or invalid). Then, based on the altitude range, compute `T` in °C and `p` in kPa using the provided empirical formulas. For the middle range (11–25 km), `T` is constant; for the lower and upper ranges, `T` depends linearly on altitude. In the upper range, an additional check is needed: the exponent in the pressure formula is `-11.388`, and the base is `(T + 273.1) / 216.6`; the base is always positive because `T + 273.1` > 0 for the given range (even at 100 km, `T` ≈ -131.21 + 0.00299*100 = -130.92, so `T+273.1` ≈ 142.18 > 0). For the lower range, the base `(T + 273.1) / 288.08` is also positive because `T` is at most 15.04 and at least -56.46 (well above -273.1). After computing `p` and `T`, compute `rho = p / (0.2869 * (T + 273.1))`. Finally, compute drag `F = 0.5 * cd * rho * v * v * a`. Edge cases: for `alt >= 25` km, the pressure formula yields extremely small values, but `pow` is safe. For `alt` such that `T` becomes negative in the lower range (only possible at `alt` slightly below 11 km, but `T = 15.04 - 0.00649*11 ≈ 14.87`, so fine). Time complexity is O(1), space O(1).

#include <cmath>

// Computes aerodynamic drag force (N) given drag coefficient, altitude (km),
// velocity (m/s), and reference area (m^2) using a piecewise Earth atmosphere model.
double atmosphericDrag(double cd, double alt, double v, double a) {
    // Validate inputs: all must be non-negative.
    if (cd < 0.0 || alt < 0.0 || v < 0.0 || a < 0.0) {
        return 0.0;
    }

    double T;  // temperature in Celsius
    double p;  // pressure in kPa

    if (alt >= 25000.0) {
        // Upper stratosphere: T linear in alt, p uses exponent -11.388
        T = -131.21 + 0.00299 * alt;
        double base = (T + 273.1) / 216.6;
        p = 2.488 * std::pow(base, -11.388);
    } else if (alt >= 11000.0) {
        // Middle stratosphere: constant T, p exponential decrease
        T = -56.46;
        p = 22.65 * std::exp(1.73 - 0.000157 * alt);
    } else {
        // Troposphere: T linear, p power law with exponent 5.256
        T = 15.04 - 0.00649 * alt;
        double base = (T + 273.1) / 288.08;
        p = 101.29 * std::pow(base, 5.256);
    }

    // Ideal gas law to compute density (kg/m^3)
    double rho = p / (0.2869 * (T + 273.1));

    // Drag force: F = 0.5 * cd * rho * v^2 * A
    return 0.5 * cd * rho * v * v * a;
}

#include <cassert>
#include <cmath>

// Function declaration (or include the solution above).
double atmosphericDrag(double cd, double altKm, double v, double a);

int main() {
    // Sea level (0 km): expected rho ≈ 1.225 kg/m³.
    // Drag = 0.5 * 0.5 * 1.225 * (10)^2 * 2 = 61.25 N
    assert(std::abs(atmosphericDrag(0.5, 0.0, 10.0, 2.0) - 61.25) < 0.2);

    // 11 km boundary: expected drag ≈ 1828 N for these inputs.
    double drag11 = atmosphericDrag(1.0, 11.0, 100.0, 1.0);
    assert(std::abs(drag11 - 1828.0) < 30.0);

    // 25 km boundary: expected drag ≈ 100.3 N.
    double drag25 = atmosphericDrag(1.0, 25.0, 50.0, 2.0);
    assert(std::abs(drag25 - 100.3) < 2.0);

    // Invalid inputs return 0.
    assert(atmosphericDrag(-1.0, 0.0, 10.0, 2.0) == 0.0);
    assert(atmosphericDrag(0.5, -1.0, 10.0, 2.0) == 0.0);
    assert(atmosphericDrag(0.5, 0.0, -1.0, 2.0) == 0.0);
    assert(atmosphericDrag(0.5, 0.0, 10.0, -1.0) == 0.0);

    // Very high altitude (100 km) gives a small positive value.
    double dragHigh = atmosphericDrag(1.0, 100.0, 1000.0, 1.0);
    assert(dragHigh > 0.0 && dragHigh < 1e-3);
}

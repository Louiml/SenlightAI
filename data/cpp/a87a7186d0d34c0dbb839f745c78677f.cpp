// Write a C++ function named `calculatePeakingFilterCoefficients` that computes double-precision coefficients for a peaking audio filter given the sampling frequency `Fs` in Hz, the center frequency `Fc` in Hz, the gain `GaindB` in decibels (range -15 to +15), and the quality factor `Q` (range 0.25 to 12). The function must return a small struct containing the three coefficients `A0`, `B1`, and `B2` (all scaled by a factor of 2), and also the linear gain `G` defined as `10^(GaindB/20) - 1`. The formulas are: `t0 = 2 * PI * Fc / Fs`; if `GaindB >= 0`, then `D = 1`, otherwise `D = 1 / (1 + G)`; `b2 = -0.5 * (2*Q - D*t0) / (2*Q + D*t0)`; `b1 = (0.5 - b2) * cos(t0)`; `a0 = (0.5 + b2) / 2`. The returned coefficients are `A0 = 2*a0`, `B1 = 2*b1`, `B2 = 2*b2`, and `G` as defined. Ensure the function handles edge cases where `2*Q + D*t0` could be zero (e.g., very small Q or extreme frequencies) by returning a flag or using a safe fallback (e.g., set coefficients to zero). Do not include `main`; provide only the function and struct.
// The solution involves straightforward computation of the filter coefficients using the provided formulas. First, calculate the linear gain `G` from `GaindB` using `pow(10.0, GaindB/20.0) - 1.0`. Then compute `t0 = 2 * PI * Fc / Fs` (using `double` for precision). Determine `D` based on the sign of `GaindB`: if non-negative, `D = 1.0`; otherwise `D = 1.0 / (1.0 + G)`. Next, compute the denominator `denom = 2 * Q + D * t0`. To avoid division by zero, check if `abs(denom) < 1e-12`; if so, set all coefficients to zero and return a boolean flag indicating failure. Otherwise, compute `b2`, then `b1`, then `a0`. Finally, scale by 2 and store results in the struct. Edge cases: `GaindB` exactly zero yields `G = 0` and `D = 1`, which is fine; negative gains produce `G` negative and `D` less than 1; extreme `Fc` near Nyquist could make `t0` large, but formula remains stable; Q can be as low as 0.25, but denominator remains positive since `D*t0` is small relative to `2*Q` in normal ranges. Time complexity is O(1) with constant space. The function uses `double` for all intermediate calculations to preserve precision.
#include <cmath>
#include <limits>

struct PeakingCoefficients {
    double A0;
    double B1;
    double B2;
    double G;
    bool valid; // false if numerical issues occurred (e.g., division by zero)
};

// Compute double-precision peaking filter coefficients.
// Fs : sampling frequency in Hz (> 0)
// Fc : center frequency in Hz (0 < Fc < Fs/2)
// GaindB : gain in dB, range -15 to +15
// Q : quality factor, range 0.25 to 12
PeakingCoefficients calculatePeakingFilterCoefficients(double Fs, double Fc, double GaindB, double Q) {
    const double PI = 3.14159265358979323846;
    const double eps = 1e-12;

    PeakingCoefficients result;
    result.valid = false;
    result.A0 = result.B1 = result.B2 = result.G = 0.0;

    // Guard against invalid inputs
    if (Fs <= 0.0 || Fc < 0.0 || Fc >= Fs / 2.0 || Q <= 0.0) {
        return result;
    }

    // G = 10^(GaindB/20) - 1
    result.G = std::pow(10.0, GaindB / 20.0) - 1.0;

    // t0 = 2 * Pi * Fc / Fs
    double t0 = 2.0 * PI * Fc / Fs;

    // D = 1 if GaindB >= 0, else 1 / (1 + G)
    double D = (GaindB >= 0.0) ? 1.0 : 1.0 / (1.0 + result.G);

    // Denominator for b2; avoid division by zero
    double denom = 2.0 * Q + D * t0;
    if (std::fabs(denom) < eps) {
        return result;
    }

    // b2 = -0.5 * (2Q - D t0) / (2Q + D t0)
    double b2 = -0.5 * (2.0 * Q - D * t0) / denom;

    // b1 = (0.5 - b2) * cos(t0)
    double b1 = (0.5 - b2) * std::cos(t0);

    // a0 = (0.5 + b2) / 2
    double a0 = (0.5 + b2) / 2.0;

    // Scale coefficients by 2
    result.A0 = 2.0 * a0;
    result.B1 = 2.0 * b1;
    result.B2 = 2.0 * b2;
    result.valid = true;

    return result;
}
#include <cassert>
#include <cmath>

int main() {
    const double PI = 3.14159265358979323846;

    // Test 1: Gain = 0 dB, Q = 1, Fc = 1000, Fs = 8000
    {
        auto r = calculatePeakingFilterCoefficients(8000.0, 1000.0, 0.0, 1.0);
        assert(r.valid);
        double t0 = 2.0 * PI * 1000.0 / 8000.0;
        double b2 = -0.5 * (2.0 - t0) / (2.0 + t0);
        double b1 = (0.5 - b2) * std::cos(t0);
        double a0 = (0.5 + b2) / 2.0;
        assert(std::fabs(r.A0 - 2.0 * a0) < 1e-9);
        assert(std::fabs(r.B1 - 2.0 * b1) < 1e-9);
        assert(std::fabs(r.B2 - 2.0 * b2) < 1e-9);
        assert(std::fabs(r.G - 0.0) < 1e-12);
    }

    // Test 2: Positive gain, e.g., +6 dB
    {
        auto r = calculatePeakingFilterCoefficients(48000.0, 1000.0, 6.0, 2.0);
        assert(r.valid);
        double G = std::pow(10.0, 6.0 / 20.0) - 1.0;
        double t0 = 2.0 * PI * 1000.0 / 48000.0;
        double D = 1.0;
        double b2 = -0.5 * (2.0 * 2.0 - D * t0) / (2.0 * 2.0 + D * t0);
        double b1 = (0.5 - b2) * std::cos(t0);
        double a0 = (0.5 + b2) / 2.0;
        assert(std::fabs(r.A0 - 2.0 * a0) < 1e-9);
        assert(std::fabs(r.B1 - 2.0 * b1) < 1e-9);
        assert(std::fabs(r.B2 - 2.0 * b2) < 1e-9);
        assert(std::fabs(r.G - G) < 1e-9);
    }

    // Test 3: Negative gain, e.g., -6 dB
    {
        auto r = calculatePeakingFilterCoefficients(8000.0, 500.0, -6.0, 0.5);
        assert(r.valid);
        double G = std::pow(10.0, -6.0 / 20.0) - 1.0;
        double t0 = 2.0 * PI * 500.0 / 8000.0;
        double D = 1.0 / (1.0 + G);
        double b2 = -0.5 * (2.0 * 0.5 - D * t0) / (2.0 * 0.5 + D * t0);
        double b1 = (0.5 - b2) * std::cos(t0);
        double a0 = (0.5 + b2) / 2.0;
        assert(std::fabs(r.A0 - 2.0 * a0) < 1e-9);
        assert(std::fabs(r.B1 - 2.0 * b1) < 1e-9);
        assert(std::fabs(r.B2 - 2.0 * b2) < 1e-9);
        assert(std::fabs(r.G - G) < 1e-9);
    }

    // Test 4: Extreme Q (min 0.25) and low frequency
    {
        auto r = calculatePeakingFilterCoefficients(8000.0, 50.0, 3.0, 0.25);
        assert(r.valid);
        double G = std::pow(10.0, 3.0 / 20.0) - 1.0;
        double t0 = 2.0 * PI * 50.0 / 8000.0;
        double b2 = -0.5 * (2.0 * 0.25 - t0) / (2.0 * 0.25 + t0);
        double b1 = (0.5 - b2) * std::cos(t0);
        double a0 = (0.5 + b2) / 2.0;
        assert(std::fabs(r.A0 - 2.0 * a0) < 1e-9);
        assert(std::fabs(r.B1 - 2.0 * b1) < 1e-9);
        assert(std::fabs(r.B2 - 2.0 * b2) < 1e-9);
    }

    // Test 5: Invalid input (Fc >= Fs/2) should return valid=false
    {
        auto r = calculatePeakingFilterCoefficients(8000.0, 4000.0, 0.0, 1.0);
        assert(!r.valid);
    }

    // Test 6: Gain at max +15 dB and min -15 dB
    {
        auto r1 = calculatePeakingFilterCoefficients(44100.0, 1000.0, 15.0, 12.0);
        assert(r1.valid);
        auto r2 = calculatePeakingFilterCoefficients(44100.0, 1000.0, -15.0, 12.0);
        assert(r2.valid);
        // Check G values
        assert(std::fabs(r1.G - (std::pow(10.0, 15.0/20.0) - 1.0)) < 1e-9);
        assert(std::fabs(r2.G - (std::pow(10.0, -15.0/20.0) - 1.0)) < 1e-9);
    }

    // Test 7: Coefficients are real numbers (no NaN/Inf)
    {
        auto r = calculatePeakingFilterCoefficients(48000.0, 20000.0, -12.0, 4.0);
        assert(r.valid);
        assert(std::isfinite(r.A0) && std::isfinite(r.B1) && std::isfinite(r.B2));
    }

    return 0;
}

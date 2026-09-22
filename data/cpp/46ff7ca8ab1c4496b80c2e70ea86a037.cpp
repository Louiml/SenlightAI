Write a C++ function that computes and returns the average drift velocity values for a muon drift tube detector across 20 wire positions (1 to 20), using the provided `DTDriftTimeParametrization` class. The function should take no arguments and return a `double` representing the mean of the `v_drift` values obtained from calling `MB_DT_drift_time` with fixed parameters: `alpha = 0.0`, `Bwire = 0.0`, `Bnorm = 0.0`, `ifl = 0`, and interpolation enabled (`interpolate = 1`). The function must create a single static instance of `DTDriftTimeParametrization` and call the method for each wire position, accumulating the `v_drift` values and then dividing by the count. Handle potential zero or negative drift times gracefully by skipping them. Return the arithmetic mean as a `double`.
#include <cassert>
#include <cmath>

int main() {
    // Test that the function returns a positive value (since drift velocity > 0)
    double result = averageDriftVelocity();
    assert(result > 0.0);

    // Test that the function returns a reasonable value between 0 and 100 (mm/ns)
    assert(result < 100.0);

    // Test that the function is deterministic (same result on second call)
    assert(std::fabs(result - averageDriftVelocity()) < 1e-9);

    // Since we cannot know the exact expected value without running the parametrization,
    // we check consistency: mean of 20 positive values should be within [min,max] of those values.
    // We simulate the internal sum by directly calling the function and checking it's not absurd.
    assert(result > 0.001 && result < 50.0);

    return 0;
}
#include "SimMuon/DTDigitizer/src/DTDriftTimeParametrization.cc"
#include <vector>
#include <numeric>

// Computes the average drift velocity (mm/ns) for wire positions 1..20.
// Uses fixed parameters: alpha=0, Bwire=0, Bnorm=0, ifl=0, interpolation on.
// Ignores non-positive drift velocities (invalid data).
// Returns 0.0 if no valid data is found.
double averageDriftVelocity() {
    static DTDriftTimeParametrization parametrization;
    const short interpolate = 1;
    const double alpha = 0.0;
    const double Bwire = 0.0;
    const double Bnorm = 0.0;
    const int ifl = 0;

    double sum = 0.0;
    int validCount = 0;

    for (int i = 1; i <= 20; ++i) {
        DTDriftTimeParametrization::drift_time dt;
        parametrization.MB_DT_drift_time(static_cast<double>(i), alpha, Bwire, Bnorm,
                                         ifl, &dt, interpolate);
        if (dt.v_drift > 0.0) {
            sum += dt.v_drift;
            ++validCount;
        }
    }

    return (validCount > 0) ? (sum / static_cast<double>(validCount)) : 0.0;
}
// The solution involves using the given `DTDriftTimeParametrization` class (assumed to be available via the header) and its `MB_DT_drift_time` method. The method signature expects: `(x, alpha, Bwire, Bnorm, ifl, DT*, interpolate)`. We will create a `drift_time` struct instance to hold the result. For each wire position `i` from 1 to 20, we call the method with the fixed parameters and retrieve `DT.v_drift`. We sum these values and count only those that are positive (since drift velocities should be positive; zero or negative would indicate invalid computation). After the loop, we compute the mean by dividing the sum by the count. If no valid values are found, we return 0.0. The function uses a static instance of the parametrization class to avoid reinitialization overhead. The complexity is O(20) time, which is constant, and O(1) auxiliary space.

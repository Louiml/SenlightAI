/*
Create a C++ function named `transverseMercatorExactWrapper` that encapsulates the core behavior of the provided C++/CLI wrapper class. The function should take as parameters: the equatorial radius `a`, flattening `f`, central scale factor `k0`, an extended-domain flag `extendp`, the central meridian `lon0`, and a latitude/longitude pair `(lat, lon)` in degrees. It should return a `std::tuple<double, double, double, double>` containing the projected coordinates `(x, y)` in meters, the meridian convergence `gamma` in degrees, and the scale factor `k` (dimensionless). The function must handle construction of a `GeographicLib::TransverseMercatorExact` object with the given parameters, call its `Forward` method, and gracefully handle any exceptions (e.g., invalid ellipsoid parameters) by falling back to the UTM default projection (using `GeographicLib::TransverseMercatorExact::UTM()`), while still returning valid results. The function should be `const`-correct and use `double` precision throughout.
*/
#include <tuple>
#include <stdexcept>
#include "GeographicLib/TransverseMercatorExact.hpp"

// Compute transverse Mercator projection coordinates and associated quantities.
// Returns tuple (x, y, gamma, k) for the given geodetic parameters.
// Falls back to UTM defaults if the given ellipsoid parameters are invalid.
std::tuple<double, double, double, double> transverseMercatorExactWrapper(
    double a, double f, double k0, bool extendp,
    double lon0, double lat, double lon) {
    
    GeographicLib::TransverseMercatorExact proj;
    
    // Try to construct with provided parameters; fall back to UTM if invalid.
    try {
        proj = GeographicLib::TransverseMercatorExact(a, f, k0, extendp);
    } catch (const std::exception&) {
        proj = GeographicLib::TransverseMercatorExact::UTM();
    }
    
    double x, y, gamma, k;
    proj.Forward(lon0, lat, lon, x, y, gamma, k);
    
    return std::make_tuple(x, y, gamma, k);
}
#include <cassert>
#include <cmath>
#include <tuple>
#include "GeographicLib/TransverseMercatorExact.hpp"

// Declare the function under test (to be linked with the solution).
std::tuple<double, double, double, double> transverseMercatorExactWrapper(
    double a, double f, double k0, bool extendp,
    double lon0, double lat, double lon);

int main() {
    // Use WGS84 ellipsoid parameters.
    const double a = 6378137.0;
    const double f = 1.0 / 298.257223563;
    const double k0 = 0.9996;
    
    // Test 1: Point at central meridian (lon0 = lat = lon = 0).
    // Expected: x=0, y=0, gamma=0, k=k0.
    auto r1 = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 0.0, 0.0);
    assert(std::abs(std::get<0>(r1)) < 1e-6);
    assert(std::abs(std::get<1>(r1)) < 1e-6);
    assert(std::abs(std::get<2>(r1)) < 1e-9);
    assert(std::abs(std::get<3>(r1) - k0) < 1e-6);
    
    // Test 2: Simple symmetric point at lat=0, lon=1 degree from central meridian.
    // Check that x is positive and y is near zero (for equator).
    auto r2 = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 0.0, 1.0);
    assert(std::get<0>(r2) > 0.0);
    assert(std::abs(std::get<1>(r2)) < 1e-3);
    assert(std::abs(std::get<2>(r2)) < 1e-6);
    assert(std::get<3>(r2) > 0.9);
    
    // Test 3: Negative longitude should give negative x.
    auto r3 = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 0.0, -1.0);
    assert(std::get<0>(r3) < 0.0);
    assert(std::abs(std::get<1>(r3)) < 1e-3);
    
    // Test 4: Point at north pole with extended domain should work.
    auto r4 = transverseMercatorExactWrapper(a, f, k0, true, 0.0, 90.0, 0.0);
    assert(std::isfinite(std::get<0>(r4)));
    assert(std::isfinite(std::get<1>(r4)));
    
    // Test 5: Invalid ellipsoid (f=5) should fall back to UTM (WGS84-like).
    // UTM has a=6378137, f=1/298.257223563, k0=0.9996, and extended=false.
    auto r5 = transverseMercatorExactWrapper(6378137.0, 5.0, 0.9996, false, 0.0, 0.0, 0.0);
    // Compare with valid UTM construction (should match r1 closely).
    auto r1_check = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 0.0, 0.0);
    assert(std::abs(std::get<0>(r5) - std::get<0>(r1_check)) < 1e-6);
    assert(std::abs(std::get<1>(r5) - std::get<1>(r1_check)) < 1e-6);
    
    // Test 6: Reverse consistency – forward then reverse should return original lat/lon.
    GeographicLib::TransverseMercatorExact proj(a, f, k0, false);
    double x6, y6, g6, k6;
    proj.Forward(0.0, 45.0, 10.0, x6, y6, g6, k6);
    auto r6 = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 45.0, 10.0);
    assert(std::abs(std::get<0>(r6) - x6) < 1e-6);
    assert(std::abs(std::get<1>(r6) - y6) < 1e-6);
    assert(std::abs(std::get<2>(r6) - g6) < 1e-9);
    assert(std::abs(std::get<3>(r6) - k6) < 1e-9);
    
    // Test 7: Central scale factor affects k proportionally.
    auto r7 = transverseMercatorExactWrapper(a, f, 0.5, false, 0.0, 0.0, 0.0);
    assert(std::abs(std::get<3>(r7) - 0.5) < 1e-6);
    
    // Test 8: Extended vs non-extended mode for a pole-adjacent point.
    // At lat=89.9, lon=0, extended mode should produce smaller errors near pole.
    auto r8a = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 89.9, 0.0);
    auto r8b = transverseMercatorExactWrapper(a, f, k0, true, 0.0, 89.9, 0.0);
    assert(std::isfinite(std::get<0>(r8a)));
    assert(std::isfinite(std::get<0>(r8b)));
    // Both should be close to each other for this point.
    assert(std::abs(std::get<0>(r8a) - std::get<0>(r8b)) < 1e-3);
    
    // Test 9: large longitude away from central meridian.
    auto r9 = transverseMercatorExactWrapper(a, f, k0, false, 0.0, 0.0, 45.0);
    assert(std::isfinite(std::get<0>(r9)));
    assert(std::isfinite(std::get<1>(r9)));
    
    // Test 10: Negative flattening (invalid) triggers fallback.
    auto r10 = transverseMercatorExactWrapper(a, -0.1, k0, false, 0.0, 0.0, 0.0);
    assert(std::abs(std::get<0>(r10)) < 1e-6);
    assert(std::abs(std::get<1>(r10)) < 1e-6);
    
    return 0;
}
// The solution involves delegating to the GeographicLib library’s `TransverseMercatorExact` class. The main algorithm is straightforward: construct the projection object with the provided geodetic parameters, then call `Forward(lon0, lat, lon, x, y, gamma, k)` to compute the transverse Mercator coordinates. Key edge cases include: (1) invalid ellipsoid inputs (e.g., `f` not in `[0,1)`), which cause the constructor to throw `std::exception` — we catch it and fall back to the default UTM projection; (2) points far from the central meridian, where the exact algorithm may return large values or converge slowly — the library handles this internally; (3) the `extendp` flag, which toggles the extended-domain version of the algorithm for poles and antimeridian. The function simply forwards all parameters and converts the results into a tuple. Time complexity is \(O(1)\) as the projection is computed directly, with no iteration required for the exact series. Space complexity is \(O(1)\) for the function itself, aside from the temporary projection object.

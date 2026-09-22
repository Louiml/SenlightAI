Write a C++ function `geodeticToGeocentric(double lat, double lon, double h, double a, double f)` that converts a point given in geodetic coordinates (latitude `lat` in degrees, longitude `lon` in degrees, ellipsoidal height `h` in meters) on a reference ellipsoid defined by equatorial radius `a` (meters) and flattening `f` (unitless, typically `1/298.257223563` for WGS84) into geocentric Cartesian coordinates `(X, Y, Z)` in meters. The function must return a `std::array<double, 3>` containing `X`, `Y`, and `Z` in that order. Handle the standard WGS84 ellipsoid constants (`a = 6378137.0`, `f = 1/298.257223563`), and compute the eccentricity squared as `e2 = f * (2 - f)`. Use the standard conversion formulas: `N = a / sqrt(1 - e2 * sin^2(lat))`, `X = (N + h) * cos(lat) * cos(lon)`, `Y = (N + h) * cos(lat) * sin(lon)`, and `Z = (N * (1 - e2) + h) * sin(lat)`. Ensure the function is robust for inputs at the poles, equator, and at the center of the Earth where `h = -a`. The function should be declared as `const`-correct (i.e., it does not modify inputs) and must not depend on any external libraries beyond `<cmath>` and `<array>`.
The solution directly applies the standard geodetic-to-geocentric conversion formulas. First, convert latitude and longitude from degrees to radians using `M_PI / 180.0`. Compute `sin(lat)` and `cos(lat)`; for longitude, compute `sin(lon)` and `cos(lon)`. Then compute the prime vertical radius of curvature `N = a / sqrt(1 - e2 * sin^2(lat))`. From there, compute `X = (N + h) * cos(lat) * cos(lon)`, `Y = (N + h) * cos(lat) * sin(lon)`, and `Z = (N * (1 - e2) + h) * sin(lat)`. Edge cases: at the exact poles (`lat = ±90°`), `cos(lat)` is zero, which yields `X = Y = 0` and `Z` is determined correctly. For `h = -a` (center of the Earth) for the spherical case, the formulas still work because `N = a` when `e2 = 0`; for ellipsoidal, the formulas remain valid. The function has constant time complexity `O(1)` and constant space complexity `O(1)`.
#include <cmath>
#include <array>

// Convert geodetic to geocentric Cartesian coordinates.
// lat, lon in degrees; h in meters.
// Returns {X, Y, Z} in meters.
std::array<double, 3> geodeticToGeocentric(double lat, double lon, double h,
                                           double a, double f) {
    const double degToRad = M_PI / 180.0;
    const double e2 = f * (2.0 - f);  // eccentricity squared

    const double sphi = std::sin(lat * degToRad);
    const double cphi = std::cos(lat * degToRad);
    const double slam = std::sin(lon * degToRad);
    const double clam = std::cos(lon * degToRad);

    const double N = a / std::sqrt(1.0 - e2 * sphi * sphi);

    const double X = (N + h) * cphi * clam;
    const double Y = (N + h) * cphi * slam;
    const double Z = (N * (1.0 - e2) + h) * sphi;

    return {X, Y, Z};
}
#include <cassert>
#include <cmath>
#include <array>

// Function declaration (copied for completeness)
std::array<double, 3> geodeticToGeocentric(double lat, double lon, double h,
                                           double a, double f);

int main() {
    const double a = 6378137.0;           // WGS84 equatorial radius (m)
    const double f = 1.0 / 298.257223563; // WGS84 flattening

    // Test 1: Equator, prime meridian, h=0 → X≈a, Y≈0, Z≈0
    auto p1 = geodeticToGeocentric(0.0, 0.0, 0.0, a, f);
    assert(std::abs(p1[0] - a) < 1e-6);
    assert(std::abs(p1[1]) < 1e-6);
    assert(std::abs(p1[2]) < 1e-6);

    // Test 2: North pole, h=0 → X=Y=0, Z≈a*(1-f) (smaller than a)
    auto p2 = geodeticToGeocentric(90.0, 0.0, 0.0, a, f);
    assert(std::abs(p2[0]) < 1e-6);
    assert(std::abs(p2[1]) < 1e-6);
    assert(std::abs(p2[2] - a * (1 - f)) < 1e-6);

    // Test 3: Equator, lon=90°, h=0 → X≈0, Y≈a, Z≈0
    auto p3 = geodeticToGeocentric(0.0, 90.0, 0.0, a, f);
    assert(std::abs(p3[0]) < 1e-6);
    assert(std::abs(p3[1] - a) < 1e-6);
    assert(std::abs(p3[2]) < 1e-6);

    // Test 4: Lat=45°, lon=0°, h=0 → X≈Z≈something, Y=0
    auto p4 = geodeticToGeocentric(45.0, 0.0, 0.0, a, f);
    assert(std::abs(p4[1]) < 1e-6);
    assert(std::abs(p4[0] - p4[2]) < 1e-6);
    assert(p4[0] > 0 && p4[2] > 0);

    // Test 5: Height addition: lat=0, lon=0, h=1000 → X≈a+1000
    auto p5 = geodeticToGeocentric(0.0, 0.0, 1000.0, a, f);
    assert(std::abs(p5[0] - (a + 1000.0)) < 1e-3);

    // Test 6: Negative height: lat=0, lon=0, h=-100 → X≈a-100
    auto p6 = geodeticToGeocentric(0.0, 0.0, -100.0, a, f);
    assert(std::abs(p6[0] - (a - 100.0)) < 1e-3);

    // Test 7: Center of Earth (h=-a) for spherical case (f=0) → X=Y=Z=0
    auto p7 = geodeticToGeocentric(0.0, 0.0, -a, a, 0.0);
    assert(std::abs(p7[0]) < 1e-3);
    assert(std::abs(p7[1]) < 1e-3);
    assert(std::abs(p7[2]) < 1e-3);

    // Test 8: South pole, lon arbitrary, h=0 → X=Y=0, Z negative
    auto p8 = geodeticToGeocentric(-90.0, 45.0, 0.0, a, f);
    assert(std::abs(p8[0]) < 1e-6);
    assert(std::abs(p8[1]) < 1e-6);
    assert(p8[2] < 0);

    return 0;
}

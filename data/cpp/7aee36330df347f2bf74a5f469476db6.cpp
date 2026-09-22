/*
Write a C++ function named `compute_diffuse_psf` that, given a 2D grid of radial distances (in meters) from the optical axis in the focal plane, a detector pixel side length (in meters), the instrument f-number, and an array of mirror aperture diameters (in meters, one per mirror), returns a 2D grid of the normalized diffuse point spread function (PSF) contribution from particulate contamination on the mirrors. The PSF must be computed by summing, over each mirror, the scattered irradiance at each radial position using a modified Harvey BRDF model. The BRDF is defined as `b * pow(1 + pow((sin(theta) - sin(theta0)) / l, 2), s / 2)`, with `theta = radius * NA / mirror_aperture`, `NA = 1/(2*fnumber)`, and `theta0 = 0`. Each mirror contributes `E_ent * pi * mirror_aperture[0]^2 * BRDF * NA^2 * (1/mirror_aperture[m])^2 * detector_size^2 / (E_ent * pi * mirror_aperture[0]^2)`, effectively simplifying to `BRDF * NA^2 * detector_size^2 * (mirror_aperture[0]^2 / mirror_aperture[m]^2)`. The result must be a new 2D grid the same dimensions as the input radius grid. Assume the input radial grid contains non-negative values, and treat the radial distance grid as a dense matrix (no sparse representation). The function must not modify the input radius grid and must handle any non-empty grid size.
*/
#include <vector>
#include <cmath>

// Helper: modified Harvey BRDF function.
double modified_harvey_brdf(double theta, double theta0, double b, double s, double l) {
    double val = (std::sin(theta) - std::sin(theta0)) / l;
    return b * std::pow(1.0 + val * val, s / 2.0);
}

/**
 * Compute the normalized diffuse PSF from particulate contamination on multiple mirrors.
 * 
 * @param radius      2D grid of radial distances from optical axis in meters (non-negative).
 * @param detector_size Side length of a square detector pixel in meters (positive).
 * @param fnumber     Instrument f-number (positive).
 * @param mirror_apertures Array of aperture diameters for each mirror in meters (positive).
 * @param b1, s1, l1  First set of Harvey parameters (per mirror? assumed same for all mirrors here).
 * @param b2, s2, l2  Second set of Harvey parameters.
 * @return 2D grid of normalized diffuse PSF values (same dimensions as radius).
 */
std::vector<std::vector<double>> compute_diffuse_psf(
    const std::vector<std::vector<double>>& radius,
    double detector_size,
    double fnumber,
    const std::vector<double>& mirror_apertures,
    double b1, double s1, double l1,
    double b2, double s2, double l2)
{
    const double na = 1.0 / (2.0 * fnumber);
    const double first_aperture_sq = mirror_apertures[0] * mirror_apertures[0];
    const double pixel_area = detector_size * detector_size;

    int height = radius.size();
    int width = (height > 0) ? radius[0].size() : 0;

    std::vector<std::vector<double>> psf(height, std::vector<double>(width, 0.0));

    for (int m = 0; m < static_cast<int>(mirror_apertures.size()); ++m) {
        double aperture = mirror_apertures[m];
        double aperture_sq = aperture * aperture;
        double scale = na * na * pixel_area * first_aperture_sq / aperture_sq;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                double theta = radius[i][j] * na / aperture;
                double brdf = modified_harvey_brdf(theta, 0.0, b1, s1, l1)
                            + modified_harvey_brdf(theta, 0.0, b2, s2, l2);
                psf[i][j] += brdf * scale;
            }
        }
    }

    return psf;
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume solution function and helper are above.

int main() {
    // Simple case: 1 mirror, constant radius grid
    std::vector<std::vector<double>> radius = {{0.0, 0.001}, {0.002, 0.003}};
    std::vector<double> apertures = {0.1};
    auto psf = compute_diffuse_psf(radius, 13e-6, 7.0, apertures, 8.13, -2.17, 0.0026, 0.00244, -0.881, 0.0431513);

    // At radius=0, theta=0, BRDF = b1 + b2
    double brdf_zero = 8.13 * pow(1.0 + pow(0.0,2), -2.17/2.0) + 0.00244 * pow(1.0 + pow(0.0,2), -0.881/2.0);
    double na_sq = 1.0 / (2.0 * 7.0);
    na_sq *= na_sq;
    double pixel = 13e-6 * 13e-6;
    double expected_zero = brdf_zero * na_sq * pixel; // aperture ratio = 1
    assert(std::fabs(psf[0][0] - expected_zero) < 1e-15);

    // Check that all values are non-negative
    for (auto& row : psf)
        for (double v : row)
            assert(v >= 0.0);

    // Check dimensions preserved
    assert(psf.size() == 2);
    assert(psf[0].size() == 2);

    // Check that larger radii give smaller PSF (since s is negative)
    assert(psf[0][0] > psf[0][1]);
    assert(psf[0][1] > psf[1][1]);

    // Test with multiple mirrors: PSF should be sum from each mirror
    std::vector<double> apertures3 = {0.1, 0.2, 0.15};
    auto psf3 = compute_diffuse_psf(radius, 13e-6, 7.0, apertures3, 8.13, -2.17, 0.0026, 0.00244, -0.881, 0.0431513);
    // At zero radius, each mirror contributes same brdf_zero, but scale changes:
    // mirror0: 1.0 * na^2 * pixel
    // mirror1: (0.1^2)/(0.2^2) = 0.25 times
    // mirror2: (0.1^2)/(0.15^2) = 0.444444...
    double scale_sum = 1.0 + 0.25 + 0.4444444444444444;
    double expected_zero3 = brdf_zero * na_sq * pixel * scale_sum;
    assert(std::fabs(psf3[0][0] - expected_zero3) < 1e-10);

    // Test with a single-point grid
    std::vector<std::vector<double>> single = {{0.0}};
    auto psf_single = compute_diffuse_psf(single, 25e-6, 7.0, apertures, 0.557, -1.36716, 0.0025166, 0.37724, -2.40281, 0.0116923);
    assert(psf_single.size() == 1 && psf_single[0].size() == 1);
    assert(psf_single[0][0] > 0.0);
}
// The solution directly implements the physical model. For each radial point, the scattering angle is computed by scaling the radial distance by `NA` and dividing by the current mirror’s aperture. The BRDF is evaluated as the sum of two Harvey terms (using parameter sets b1,s1,l1 and b2,s2,l2). The scattered irradiance per mirror is then multiplied by the pixel area and normalized by the incident power on the first mirror (which cancels in the simplified formula). A loop over mirrors accumulates the contribution. The algorithm runs in O(mirrors * height * width) time, which is O(3 * N) for a grid with N elements. Space usage is O(N) for the output grid, plus constant overhead. Edge cases: radial distance of zero yields theta = 0, which is fine; extremely large radii may cause `sin(theta)` to exceed 1 if theta exceeds π/2, but since we assume radii are physically reasonable (within the detector field) this is not handled. Also, the aperture values are assumed positive and non-zero. The function must use `std::vector<double>` or a custom 2D container; to keep it self-contained, we define a simple 2D vector of doubles using `std::vector<std::vector<double>>`. Const correctness is applied to inputs, and the output is returned by value.

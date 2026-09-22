// Write a standalone C++ function that models atmospheric infrared transmissivity and signal/background contribution calculation based on a simplified version of the IrAtmosphere logic. The function should take as input: a vector of waveband center wavelengths, a vector of waveband widths, a vector of transmissivity coefficients (one per waveband), the range to a target in meters, a vector representing the target's signature per waveband (where each entry contains lower bound, upper bound, and radiant intensity, in groups of three), the sensor's lower and upper wavelength limits, the background radiance, and a flag indicating whether the target is in sky (true) or on ground (false). The function should compute and return a pair of doubles: total signal received and total background radiance received at the sensor, applying exponential transmissivity decay (coefficient * -0.001 * range) for each waveband, accounting for wavelength overlap between each waveband and the sensor's spectral range, and distributing simple signatures evenly across bands if no per-band signature is provided.
// The solution computes total signal and background by iterating over each waveband. For each band, we first determine the lower and upper band bounds from center and width. We compute the total wavelength range covered by all bands (from the lowest lower bound to the highest upper bound). We then find the overlap between the band bounds and the sensor's wavelength limits. The overlap ratio is the portion of the band that falls within the sensor's spectral response. For the signature, if the provided signature vector is empty, we distribute the simple signature (given as a single value stored in the first element of the vector) evenly across bands, applying fractionOfBandToTotal and overlapRatio. If per-band signature is provided (length equals 3*numBands), we read intensity from positions i*3+2. The transmissivity for each band is exp(coefficient[i] * -0.001 * range). We accumulate totalSignal and totalBackground accordingly. For background, we multiply backgroundRadiance by fractionOfBandToTotal and overlapRatio, then by transmissivity. Edge cases include empty input vectors (return zeros), zero band widths (avoid division by zero), sensor limits outside all bands (overlap ratio becomes zero). Time complexity is O(N) where N is number of bands; space complexity is O(1) beyond input storage.
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

/**
 * Compute total signal and background radiance received by an IR sensor after atmospheric attenuation.
 * 
 * @param bandCenters  Center wavelengths of each atmospheric waveband (in same units as sensor limits).
 * @param bandWidths   Width of each waveband.
 * @param transCoefs   Transmissivity coefficients (absorption+scattering) per km for each waveband.
 * @param range        Distance to target in meters.
 * @param signature    Either empty (indicating simple signature across all bands) or contains
 *                     triplets: lower bound, upper bound, radiant intensity for each band.
 * @param simpleSignature  Used when signature is empty: the radiant intensity distributed evenly.
 * @param sensorLower  Lower wavelength limit of sensor.
 * @param sensorUpper  Upper wavelength limit of sensor.
 * @param backgroundRadiance  Uniform background radiance (sky or earth) in consistent units.
 * 
 * @return Pair of (totalSignal, totalBackground) in watts/sr or equivalent units.
 */
std::pair<double, double> computeIrAtmosphereContribution(
    const std::vector<double>& bandCenters,
    const std::vector<double>& bandWidths,
    const std::vector<double>& transCoefs,
    double range,
    const std::vector<double>& signature,
    double simpleSignature,
    double sensorLower,
    double sensorUpper,
    double backgroundRadiance)
{
    // If no bands provided, return zero contribution.
    if (bandCenters.empty() || bandWidths.empty() || transCoefs.empty()) {
        return {0.0, 0.0};
    }

    const size_t numBands = bandCenters.size();
    
    // Compute total wavelength range covered by all bands (from lowest lower bound to highest upper bound).
    double lowestLower = bandCenters[0] - bandWidths[0] / 2.0;
    double highestUpper = bandCenters[0] + bandWidths[0] / 2.0;
    for (size_t i = 1; i < numBands; ++i) {
        double lower = bandCenters[i] - bandWidths[i] / 2.0;
        double upper = bandCenters[i] + bandWidths[i] / 2.0;
        lowestLower = std::min(lowestLower, lower);
        highestUpper = std::max(highestUpper, upper);
    }
    double totalWavelengthRange = highestUpper - lowestLower;
    if (totalWavelengthRange <= 0.0) {
        return {0.0, 0.0};
    }

    // Determine if signature is provided per band (length == 3 * numBands) or simple (empty).
    bool hasPerBandSignature = (signature.size() == 3 * numBands);

    double totalSignal = 0.0;
    double totalBackground = 0.0;

    for (size_t i = 0; i < numBands; ++i) {
        double center = bandCenters[i];
        double width = bandWidths[i];
        if (width <= 0.0) continue; // skip invalid widths

        double lowerBand = center - width / 2.0;
        double upperBand = center + width / 2.0;
        double bandSpan = upperBand - lowerBand;

        // Fraction of this band relative to entire wavelength coverage.
        double fractionOfBandToTotal = bandSpan / totalWavelengthRange;

        // Overlap between this band and sensor spectral limits.
        double overlapLower = std::max(lowerBand, sensorLower);
        double overlapUpper = std::min(upperBand, sensorUpper);
        double overlapLength = std::max(0.0, overlapUpper - overlapLower);
        double overlapRatio = (bandSpan > 0.0) ? (overlapLength / bandSpan) : 0.0;

        // Transmissivity: exp(coefficient * -0.001 * range)
        double coef = (i < transCoefs.size()) ? transCoefs[i] : 0.0;
        double trans = std::exp(coef * -0.001 * range);

        // Determine radiant intensity in this band.
        double radiantIntensity = 0.0;
        if (hasPerBandSignature) {
            // Per-band signature: intensity at index i*3 + 2.
            radiantIntensity = signature[i * 3 + 2];
        } else {
            // Simple signature: distribute evenly across bands, applying overlap ratio.
            radiantIntensity = simpleSignature * fractionOfBandToTotal * overlapRatio;
        }

        totalSignal += radiantIntensity * trans;

        // Background contribution: uniform background radiance distributed by band and overlap.
        double backgroundInBand = backgroundRadiance * fractionOfBandToTotal * overlapRatio;
        totalBackground += backgroundInBand * trans;
    }

    return {totalSignal, totalBackground};
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is defined above. This test verifies correctness.
int main() {
    // Test 1: Single band, zero transmissivity coefficient, full overlap, simple signature.
    {
        std::vector<double> centers = {5.0};
        std::vector<double> widths = {2.0};
        std::vector<double> coefs = {0.0};
        double range = 1000.0; // 1 km
        std::vector<double> signature; // empty => simple
        double simpleSig = 10.0;
        double sensorLower = 4.0;
        double sensorUpper = 6.0;
        double background = 2.0;
        auto result = computeIrAtmosphereContribution(centers, widths, coefs, range, signature, simpleSig, sensorLower, sensorUpper, background);
        // totalWavelengthRange = 2.0; fraction = 1.0; overlapRatio = 1.0; trans = exp(0)=1
        // signal = 10 * 1 * 1 = 10
        // background = 2 * 1 * 1 = 2
        assert(std::abs(result.first - 10.0) < 1e-9);
        assert(std::abs(result.second - 2.0) < 1e-9);
    }

    // Test 2: Two bands, partial overlap, nonzero transmissivity.
    {
        std::vector<double> centers = {3.0, 7.0};
        std::vector<double> widths = {2.0, 2.0};
        std::vector<double> coefs = {1.0, 0.5};
        double range = 2000.0; // 2 km
        std::vector<double> signature; // simple
        double simpleSig = 20.0;
        double sensorLower = 4.0;
        double sensorUpper = 8.0;
        double background = 5.0;
        auto result = computeIrAtmosphereContribution(centers, widths, coefs, range, signature, simpleSig, sensorLower, sensorUpper, background);
        // Band 1: lower=2, upper=4, bandSpan=2; totalRange=6 (2 to 8); fraction=1/3
        // overlap with sensor [4,8] => [4,4] length 0 => overlapRatio=0
        // signal = 20 * (1/3) * 0 = 0; background = 5 * (1/3)*0 = 0
        // Band 2: lower=6, upper=8, bandSpan=2; fraction=1/3
        // overlap [6,8] length 2 => overlapRatio=1
        // trans = exp(0.5 * -0.001 * 2000) = exp(-1.0) ≈ 0.367879
        // signal = 20 * (1/3) * 1 * 0.367879 ≈ 2.45253
        // background = 5 * (1/3)*1*0.367879 ≈ 0.613132
        assert(std::abs(result.first - (20.0/3.0 * std::exp(-1.0))) < 1e-9);
        assert(std::abs(result.second - (5.0/3.0 * std::exp(-1.0))) < 1e-9);
    }

    // Test 3: Per-band signature provided.
    {
        std::vector<double> centers = {5.0};
        std::vector<double> widths = {2.0};
        std::vector<double> coefs = {0.2};
        double range = 500.0;
        // signature triplets: lower, upper, intensity
        std::vector<double> signature = {4.0, 6.0, 30.0}; // one band
        double simpleSig = 0.0; // ignored
        double sensorLower = 4.5;
        double sensorUpper = 5.5;
        double background = 1.0;
        auto result = computeIrAtmosphereContribution(centers, widths, coefs, range, signature, simpleSig, sensorLower, sensorUpper, background);
        // band lower=4, upper=6; totalRange=2; fraction=1.0
        // overlap [4.5, 5.5] length 1.0 => overlapRatio = 1.0/2.0 = 0.5
        // trans = exp(0.2 * -0.001 * 500) = exp(-0.1) ≈ 0.904837
        // signal = 30.0 * 0.5 * 0.904837 ≈ 13.5726
        // background = 1.0 * 1.0 * 0.5 * 0.904837 ≈ 0.452419
        assert(std::abs(result.first - (30.0 * 0.5 * std::exp(-0.1))) < 1e-9);
        assert(std::abs(result.second - (1.0 * 0.5 * std::exp(-0.1))) < 1e-9);
    }

    // Test 4: Empty input.
    {
        std::vector<double> centers, widths, coefs;
        double range = 100.0;
        std::vector<double> signature;
        double simpleSig = 5.0;
        double sensorLower = 0.0, sensorUpper = 100.0;
        double background = 3.0;
        auto result = computeIrAtmosphereContribution(centers, widths, coefs, range, signature, simpleSig, sensorLower, sensorUpper, background);
        assert(result.first == 0.0);
        assert(result.second == 0.0);
    }

    // Test 5: No overlap with sensor.
    {
        std::vector<double> centers = {5.0};
        std::vector<double> widths = {2.0};
        std::vector<double> coefs = {0.0};
        double range = 100.0;
        std::vector<double> signature;
        double simpleSig = 10.0;
        double sensorLower = 10.0, sensorUpper = 20.0; // well above band
        double background = 2.0;
        auto result = computeIrAtmosphereContribution(centers, widths, coefs, range, signature, simpleSig, sensorLower, sensorUpper, background);
        // overlap length = 0, so signal and background = 0
        assert(result.first == 0.0);
        assert(result.second == 0.0);
    }

    return 0;
}

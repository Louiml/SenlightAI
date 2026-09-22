// Write a C++ function named `adjustSignalsByCovariate` that takes a vector of probe intensities (floats, possibly containing NaN values), a vector of corresponding covariate values (floats, possibly NaN), and a bin count (positive integer). The function should perform a simplified covariate-based median bin adjustment: group probes into bins by equal partitioning of the sorted covariate values (each bin gets as close to equal number of probes as possible, except that bins with NaN covariate or NaN intensity are excluded entirely). For each bin, compute the median of the intensities of autosome-like probes (all probes, no chromosome filtering in this simplified version). Then compute a grand median across all non-NaN intensities from all bins. For each probe with a non-NaN intensity and a valid bin, adjust the intensity by multiplying the original intensity by the grand median divided by the bin median. If a bin median is 0.0, do not adjust probes in that bin. Probes with NaN intensity or NaN covariate are left unchanged. The function should modify the input vector in-place and return nothing. Assume covariate values may repeat, and the order of probes must be preserved. If the input vector is empty or the bin count is less than 1, the function returns without modifying anything. The binning rule: sort the indices by covariate value, then distribute indices sequentially into `numBins` bins, with bin sizes differing by at most 1, where the first `numBins - (size % numBins)` bins get `size / numBins` elements and the remaining bins get `size / numBins + 1` elements, when `size` is the number of probes with non-NaN covariate (and non-NaN intensity, because those are excluded? No, binning is based on covariate only; probes with NaN intensity still consume a slot in the bin and affect bin size, but their intensity is not included in median computation). Actually, for simplicity, define: first collect all probe indices where covariate is not NaN. Sort these indices by covariate value. Partition them evenly into `numBins` bins. Then for each bin, compute the median of intensities from those indices that also have non-NaN intensity. Compute grand median from all such intensities. Then adjust.
The algorithm proceeds in three phases. First, filter out probes with NaN covariate values, creating a list of valid indices. Sort this list by covariate value using a stable sort (equal covariate values can be in any order). Next, partition the sorted list into `numBins` contiguous groups. The size of each bin is either `size / numBins` or `size / numBins + 1` such that the total is `size`. For each bin, compute the median of the intensities of the probes in that bin, ignoring NaN intensities. To compute the median, copy the non-NaN intensities into a temporary vector, use `std::nth_element` to find the middle element (for even length, take the lower middle, which is conventional for this kind of adjustment, as in the original code). If a bin has no non-NaN intensities, its median is set to 0.0. Also collect all non-NaN intensities into a global vector to compute the grand median. After computing all bin medians, compute the grand median from the global vector. Finally, iterate over the valid indices (the ones with non-NaN covariate) in original order (or any order, because we are just writing back to the original vector using indices). For each such probe, if its intensity is non-NaN and its bin's median is not 0.0, set `intensity[index] = intensity[index] * grandMedian / binMedian` (to do it in-place). Probes not in the valid list, or with NaN intensity, or with bin median equal to 0.0, remain unchanged. Edge cases: empty input → do nothing. `numBins` <= 0 → do nothing. If all covariate values are NaN → no bins, return. If `numBins` > number of valid probes, then some bins will be empty; for those bins, median is set to 0.0, so probes assigned to them (none) won't be adjusted. Time complexity: sorting the valid indices takes O(n log n) where n is number of valid probes; collecting and computing medians takes O(n) per bin with `nth_element` being linear on average, so overall O(n log n) due to sorting. Space complexity: O(n) for the index list, temporary vectors, and bin assignment array.
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <cstddef>

/**
 * @brief Adjust probe intensities by covariate-based median bin normalization.
 * 
 * Probes with NaN covariate values are ignored entirely. The remaining probes
 * are sorted by covariate value and partitioned into numBins bins with as
 * equal sizes as possible. Within each bin, the median of non-NaN intensities
 * is computed. Then a grand median is computed from all non-NaN intensities.
 * Each probe with non-NaN intensity and a bin median that is not zero is
 * adjusted by multiplying its intensity by grandMedian / binMedian. Probes
 * with NaN intensity, NaN covariate, or whose bin median is zero are left
 * unchanged. The operation is in-place.
 * 
 * @param intensities The vector of probe intensities to modify.
 * @param covariateValues Corresponding covariate values (same size).
 * @param numBins Number of bins to use for the adjustment.
 */
void adjustSignalsByCovariate(std::vector<float>& intensities,
                              const std::vector<float>& covariateValues,
                              int numBins)
{
    // Early exit conditions
    if (intensities.empty() || covariateValues.empty() || intensities.size() != covariateValues.size()) {
        return;
    }
    if (numBins <= 0) {
        return;
    }

    // Collect indices of probes with non-NaN covariate values.
    std::vector<std::size_t> validIndices;
    for (std::size_t i = 0; i < covariateValues.size(); ++i) {
        if (!std::isnan(covariateValues[i])) {
            validIndices.push_back(i);
        }
    }

    if (validIndices.empty()) {
        return;
    }

    // Sort indices by covariate value.
    std::stable_sort(validIndices.begin(), validIndices.end(),
                     [&covariateValues](std::size_t a, std::size_t b) {
                         return covariateValues[a] < covariateValues[b];
                     });

    // Determine bin sizes (as equal as possible).
    const std::size_t n = validIndices.size();
    const std::size_t baseSize = n / static_cast<std::size_t>(numBins);
    const std::size_t remainder = n % static_cast<std::size_t>(numBins);

    // Assign each valid index a bin number (0 .. numBins-1).
    std::vector<int> binAssignment(n, -1);
    std::size_t pos = 0;
    for (int b = 0; b < numBins; ++b) {
        std::size_t binSize = baseSize + (static_cast<std::size_t>(b) < remainder ? 1 : 0);
        for (std::size_t k = 0; k < binSize; ++k) {
            binAssignment[pos + k] = b;
        }
        pos += binSize;
    }

    // Compute bin medians and collect all non-NaN intensities.
    std::vector<float> binMedians(static_cast<std::size_t>(numBins), 0.0f);
    std::vector<float> allData;
    allData.reserve(n);

    for (int b = 0; b < numBins; ++b) {
        std::vector<float> binData;
        for (std::size_t i = 0; i < n; ++i) {
            if (binAssignment[i] == b) {
                float intensity = intensities[validIndices[i]];
                if (!std::isnan(intensity)) {
                    binData.push_back(intensity);
                    allData.push_back(intensity);
                }
            }
        }
        if (!binData.empty()) {
            std::size_t mid = binData.size() / 2;
            std::nth_element(binData.begin(), binData.begin() + static_cast<std::ptrdiff_t>(mid), binData.end());
            binMedians[static_cast<std::size_t>(b)] = binData[mid];
        }
    }

    // Compute grand median.
    if (allData.empty()) {
        return; // No non-NaN intensities at all, nothing to adjust.
    }
    std::size_t grandMid = allData.size() / 2;
    std::nth_element(allData.begin(), allData.begin() + static_cast<std::ptrdiff_t>(grandMid), allData.end());
    float grandMedian = allData[grandMid];

    // Adjust intensities in-place for all valid probes with valid bin and non-NaN intensity.
    for (std::size_t i = 0; i < validIndices.size(); ++i) {
        std::size_t idx = validIndices[i];
        float intensity = intensities[idx];
        if (std::isnan(intensity)) {
            continue;
        }
        int bin = binAssignment[i];
        float binMedian = binMedians[static_cast<std::size_t>(bin)];
        if (binMedian != 0.0f) {
            intensities[idx] = intensity * grandMedian / binMedian;
        }
    }
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function is assumed to be declared above (included from the solution).
// Here we declare it again for the test file (in a real project it would be in a header).
void adjustSignalsByCovariate(std::vector<float>& intensities,
                              const std::vector<float>& covariateValues,
                              int numBins);

int main() {
    // Basic case: two bins, simple values.
    {
        std::vector<float> intensities = {2.0f, 4.0f, 6.0f, 8.0f};
        std::vector<float> covariates   = {1.0f, 2.0f, 3.0f, 4.0f};
        adjustSignalsByCovariate(intensities, covariates, 2);
        // Bins: [0,1] and [2,3]. Medians: (2+4)/2 = 3 (but we take lower median: 2? Actually nth_element returns 2 for [2,4], and for [6,8] returns 6). Grand median of all data [2,4,6,8] is 4 (lower mid). Adjust: bin0 median=2 → 2*4/2=4, 4*4/2=8; bin1 median=6 → 6*4/6=4, 8*4/6≈5.333.
        assert(std::abs(intensities[0] - 4.0f) < 1e-5);
        assert(std::abs(intensities[1] - 8.0f) < 1e-5);
        assert(std::abs(intensities[2] - 4.0f) < 1e-5);
        assert(std::abs(intensities[3] - (8.0f * 4.0f / 6.0f)) < 1e-5);
    }

    // NaN covariate values are excluded.
    {
        std::vector<float> intensities = {10.0f, 20.0f, 30.0f};
        std::vector<float> covariates   = {NAN, 2.0f, 3.0f};
        adjustSignalsByCovariate(intensities, covariates, 1);
        // Only one valid probe (index 1) with intensity 20. Bin median = 20, grand median = 20 → unchanged.
        assert(intensities[0] == 10.0f);
        assert(intensities[1] == 20.0f);
        assert(intensities[2] == 30.0f);
    }

    // NaN intensity leaves probe unchanged.
    {
        std::vector<float> intensities = {NAN, 10.0f};
        std::vector<float> covariates   = {1.0f, 2.0f};
        adjustSignalsByCovariate(intensities, covariates, 1);
        // Bin median = 10, grand median = 10, adjust index 1: 10*10/10=10, index 0 remains NaN.
        assert(std::isnan(intensities[0]));
        assert(std::abs(intensities[1] - 10.0f) < 1e-5);
    }

    // Bin median zero causes no adjustment for that bin.
    {
        // With one bin and all intensities NaN except one? Actually to get a bin median 0, we need no non-NaN intensities in a bin.
        std::vector<float> intensities = {5.0f, 7.0f}; // both non-NaN
        std::vector<float> covariates   = {1.0f, 2.0f};
        adjustSignalsByCovariate(intensities, covariates, 2);
        // With n=2, numBins=2, each bin has one probe. Bin medians: 5 and 7. Grand median of [5,7] is 5 (lower mid). Adjust bin0: 5*5/5=5, bin1: 7*5/7=5.
        assert(std::abs(intensities[0] - 5.0f) < 1e-5);
        assert(std::abs(intensities[1] - 5.0f) < 1e-5);
    }

    // Edge: empty vector.
    {
        std::vector<float> intensities;
        std::vector<float> covariates;
        adjustSignalsByCovariate(intensities, covariates, 3);
        assert(intensities.empty());
    }

    // Edge: invalid bin count.
    {
        std::vector<float> intensities = {1.0f, 2.0f};
        std::vector<float> covariates   = {1.0f, 2.0f};
        adjustSignalsByCovariate(intensities, covariates, 0);
        assert(intensities[0] == 1.0f && intensities[1] == 2.0f);
    }

    // All covariates NaN → no change.
    {
        std::vector<float> intensities = {3.0f, 4.0f};
        std::vector<float> covariates   = {NAN, NAN};
        adjustSignalsByCovariate(intensities, covariates, 2);
        assert(intensities[0] == 3.0f && intensities[1] == 4.0f);
    }

    // Test with more probes than bins, ensures equal distribution.
    {
        std::vector<float> intensities = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        std::vector<float> covariates   = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        adjustSignalsByCovariate(intensities, covariates, 2);
        // n=5, baseSize=2, remainder=1 → bin0 has 3 elements (indices 0,1,2), bin1 has 2 (indices 3,4).
        // Bin0 medians of {1,2,3}: median = 2. Bin1 medians of {4,5}: median = 4.
        // Grand median of all {1,2,3,4,5}: lower mid = 3.
        // Adjust bin0 probes: 1*3/2=1.5, 2*3/2=3, 3*3/2=4.5; bin1: 4*3/4=3, 5*3/4=3.75.
        assert(std::abs(intensities[0] - 1.5f) < 1e-5);
        assert(std::abs(intensities[1] - 3.0f) < 1e-5);
        assert(std::abs(intensities[2] - 4.5f) < 1e-5);
        assert(std::abs(intensities[3] - 3.0f) < 1e-5);
        assert(std::abs(intensities[4] - 3.75f) < 1e-5);
    }

    return 0;
}

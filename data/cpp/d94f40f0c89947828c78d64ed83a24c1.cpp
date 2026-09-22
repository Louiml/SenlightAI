Implement a C++ function `sampleDistribution` that generates a specified number of samples from a discrete distribution defined by a vector of positive weights, and returns a histogram (a vector of counts) for a given number of bins. The function should take parameters: a vector of weights (each strictly greater than zero), the number of samples to draw, and the number of histogram bins. It should use a uniform random number generator (e.g., `std::mt19937` with `std::random_device` seed) and perform weighted sampling without replacement. The output is a vector of integers where each element represents the number of times a sample fell into that bin. The bins correspond to indices in the weight vector: bin `i` gets incremented whenever the sampled index equals `i`. Ensure the function is deterministic-mockable: the random engine should be passed by reference or as a parameter to allow testing. Handle edge cases: if weights vector is empty or number of bins is zero, return an empty vector. The total weight may be large, so use a 64-bit integer for accumulation to avoid overflow. Complexity: O(bins + samples * log(bins)) time and O(bins) space, where bins is the number of weights.
#include <cassert>
#include <vector>
#include <random>
#include <iostream>

// Include the solution function here (in a real test, it would be included from header)

int main() {
    // Test 1: Uniform weights -> all bins roughly equal (use small sample and exact checks)
    std::vector<int> w1 = {1, 1, 1, 1};
    std::mt19937 rng1(42);
    auto hist1 = sampleDistribution(w1, 400, 4, rng1);
    // With seed 42, we can check exact distribution? Not deterministic across implementations,
    // but we can check total count equals samples and all bins are within reasonable range.
    int sum1 = 0;
    for (int c : hist1) sum1 += c;
    assert(sum1 == 400);
    assert(hist1.size() == 4);
    for (int c : hist1) assert(c > 0); // all bins visited

    // Test 2: One bin always selected
    std::vector<int> w2 = {5};
    std::mt19937 rng2(7);
    auto hist2 = sampleDistribution(w2, 100, 1, rng2);
    assert(hist2.size() == 1 && hist2[0] == 100);

    // Test 3: Empty weights -> empty result
    std::vector<int> w3;
    std::mt19937 rng3(1);
    auto hist3 = sampleDistribution(w3, 10, 5, rng3);
    assert(hist3.empty());

    // Test 4: numBins=0 -> empty
    std::vector<int> w4 = {1, 2};
    std::mt19937 rng4(2);
    auto hist4 = sampleDistribution(w4, 10, 0, rng4);
    assert(hist4.empty());

    // Test 5: Deterministic with seed - verify counts for a specific seed
    // Use a very skewed distribution: only first bin should be selected with high probability
    std::vector<int> w5 = {1000, 1};
    std::mt19937 rng5(12345);
    auto hist5 = sampleDistribution(w5, 1000, 2, rng5);
    // Since first weight dominates, first bin should have >990 counts (actually ~999)
    assert(hist5[0] > 990);
    assert(hist5[1] < 10);

    // Test 6: Verify total count matches samples for various cases
    std::vector<int> w6 = {3, 1, 4, 1, 5};
    std::mt19937 rng6(99);
    auto hist6 = sampleDistribution(w6, 567, 5, rng6);
    int sum6 = 0;
    for (int c : hist6) sum6 += c;
    assert(sum6 == 567);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <cstdint>
#include <algorithm>
#include <random>
#include <stdexcept>

/**
 * @brief Generate a histogram of weighted random samples.
 * 
 * @param weights Vector of positive weights for each bin (index = bin).
 * @param numSamples Number of random samples to draw.
 * @param numBins Number of histogram bins (must equal weights.size()).
 * @param rng Random number generator (e.g., std::mt19937) passed by reference.
 * @return std::vector<int> Counts per bin. Empty if weights empty or numBins==0.
 */
std::vector<int> sampleDistribution(
    const std::vector<int>& weights,
    int numSamples,
    int numBins,
    std::mt19937& rng)
{
    // Handle edge cases
    if (weights.empty() || numBins <= 0) {
        return std::vector<int>();
    }

    // Reserve and initialize histogram
    std::vector<int> histogram(numBins, 0);

    // Build cumulative distribution (CDF) using 64-bit to avoid overflow
    std::vector<std::uint64_t> cdf(weights.size(), 0);
    std::uint64_t total = 0;
    for (size_t i = 0; i < weights.size(); ++i) {
        total += static_cast<std::uint64_t>(weights[i]);
        cdf[i] = total;
    }

    // If total is zero (should not happen if all weights > 0, but guard)
    if (total == 0) {
        return histogram; // all counts remain zero
    }

    // Generate samples
    std::uniform_int_distribution<std::uint64_t> dist(0, total - 1);
    for (int s = 0; s < numSamples; ++s) {
        std::uint64_t r = dist(rng);
        // Find first bin where CDF > r (r is in [0, total-1])
        auto it = std::upper_bound(cdf.begin(), cdf.end(), r);
        size_t index = it - cdf.begin();
        // Clamp to valid range (should never happen if weights positive)
        if (index >= static_cast<size_t>(numBins)) {
            index = numBins - 1;
        }
        histogram[index]++;
    }

    return histogram;
}
// The solution uses the standard approach for weighted sampling: precompute a cumulative distribution function (CDF) from the weights. For each sample, generate a uniform random number in [0, total_weight) (using a 64-bit integer to avoid precision issues), then use `std::lower_bound` on the CDF to find the corresponding bin index. That index is incremented in the histogram. The CDF is built once before sampling. The random engine is passed by reference to allow reproducibility. Edge cases: if weights are empty or bins requested is zero, return an empty vector. If weights contain positive values only (as specified), the CDF is strictly increasing, so `lower_bound` works correctly. For performance, the cumulative sum uses `long long` (or `uint64_t`) to avoid overflow. The function is `const`-correct for input parameters, and the random engine is non-const since it is modified. Time complexity: constructing the CDF is O(bins), each sample uses `lower_bound` on the CDF which takes O(log(bins)), so total O(bins + samples * log(bins)). Space: O(bins) for the CDF and histogram.

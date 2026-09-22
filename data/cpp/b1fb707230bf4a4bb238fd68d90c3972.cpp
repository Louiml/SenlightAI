Write a C++ function `computeSkinProbabilityMatrix` that takes two 2D histogram arrays (skin and non-skin) of non-negative integer counts, both of size `HIST_SIZE x HIST_SIZE`, and returns a 2D array of the same dimensions containing the posterior probability that a pixel belongs to skin given its observed color channel pair (u, v), computed via Bayes' theorem. The input arrays represent counts for discrete color bins (e.g., U and V channels in YUV color space). The function must assume histograms are rectangular and non-empty (each has at least one total count), and must handle the edge case where a color bin has zero total count (skin + non-skin) by setting the probability to 0.0 to avoid division by zero. The output should be a `std::vector<std::vector<double>>` where each entry `probabilities[u][v] = P(skin | color)`.

// The core algorithm applies Bayes' theorem: `P(skin | color) = P(color | skin) * P(skin) / P(color)`. First, compute the total counts: `totalSkin`, `totalNonSkin`, and `total = totalSkin + totalNonSkin`. The prior `P(skin) = totalSkin / total`. For each bin (u, v), compute `P(color) = (skin[u][v] + nonSkin[u][v]) / total` and `P(color | skin) = skin[u][v] / totalSkin`. The posterior is then `P(color | skin) * P(skin) / P(color)`. Edge case: if `P(color) == 0` (i.e., both histograms have zero count at that bin), the conditional probability is undefined, so set it to 0.0 (avoiding division by zero). Also ensure all input counts are non-negative; if any negative value appears, treat it as 0 or throw an exception for robustness. Time complexity is O(HIST_SIZE^2) because we iterate over every bin once, and space complexity is O(HIST_SIZE^2) for the output matrix.

#include <vector>
#include <stdexcept>

// Compute posterior skin probabilities for each (u, v) bin using Bayes' theorem.
std::vector<std::vector<double>> computeSkinProbabilityMatrix(
    const std::vector<std::vector<unsigned long>>& skinHist,
    const std::vector<std::vector<unsigned long>>& nonSkinHist) {
    
    if (skinHist.empty() || nonSkinHist.empty() ||
        skinHist.size() != nonSkinHist.size() ||
        skinHist[0].size() != nonSkinHist[0].size()) {
        throw std::invalid_argument("Histograms must be non-empty and have matching dimensions.");
    }
    
    const size_t rows = skinHist.size();
    const size_t cols = skinHist[0].size();
    
    unsigned long totalSkin = 0, totalNonSkin = 0;
    for (size_t u = 0; u < rows; ++u) {
        if (skinHist[u].size() != cols || nonSkinHist[u].size() != cols) {
            throw std::invalid_argument("All rows must have consistent column counts.");
        }
        for (size_t v = 0; v < cols; ++v) {
            totalSkin += skinHist[u][v];
            totalNonSkin += nonSkinHist[u][v];
        }
    }
    
    if (totalSkin == 0 || totalNonSkin == 0) {
        throw std::invalid_argument("Each histogram must have at least one total count.");
    }
    
    const unsigned long total = totalSkin + totalNonSkin;
    const double probSkin = static_cast<double>(totalSkin) / total;
    
    std::vector<std::vector<double>> probabilities(rows, std::vector<double>(cols, 0.0));
    
    for (size_t u = 0; u < rows; ++u) {
        for (size_t v = 0; v < cols; ++v) {
            const unsigned long countSkin = skinHist[u][v];
            const unsigned long countNonSkin = nonSkinHist[u][v];
            const unsigned long countTotal = countSkin + countNonSkin;
            
            if (countTotal > 0) {
                const double probColour = static_cast<double>(countTotal) / total;
                const double probColourGivenSkin = static_cast<double>(countSkin) / totalSkin;
                probabilities[u][v] = probColourGivenSkin * probSkin / probColour;
            } else {
                probabilities[u][v] = 0.0;
            }
        }
    }
    
    return probabilities;
}

#include <cassert>
#include <cmath>
#include <vector>

int main() {
    // Simple case: 1x1 histograms with positive counts.
    std::vector<std::vector<unsigned long>> skin1 = {{10}};
    std::vector<std::vector<unsigned long>> nonSkin1 = {{30}};
    auto result1 = computeSkinProbabilityMatrix(skin1, nonSkin1);
    // P(skin|color) = (10/40 * 40/40) / (40/40) = 10/40 = 0.25
    assert(std::fabs(result1[0][0] - 0.25) < 1e-9);

    // 2x2 case with one zero-count bin.
    std::vector<std::vector<unsigned long>> skin2 = {{5, 0}, {2, 3}};
    std::vector<std::vector<unsigned long>> nonSkin2 = {{5, 1}, {0, 0}};
    auto result2 = computeSkinProbabilityMatrix(skin2, nonSkin2);
    // totalSkin = 10, totalNonSkin = 6, total = 16, probSkin = 10/16 = 0.625
    // Bin (0,0): skin=5, nonSkin=5, total=10 → P(color)=10/16, P(color|skin)=5/10 → posterior = (5/10)*0.625/(10/16) = 0.5*0.625/0.625 = 0.5
    assert(std::fabs(result2[0][0] - 0.5) < 1e-9);
    // Bin (0,1): skin=0, nonSkin=1, total=1 → P(color)=1/16, P(color|skin)=0/10=0 → posterior = 0
    assert(std::fabs(result2[0][1] - 0.0) < 1e-9);
    // Bin (1,0): skin=2, nonSkin=0, total=2 → P(color)=2/16, P(color|skin)=2/10=0.2 → posterior = 0.2*0.625/(0.125) = 1.0
    assert(std::fabs(result2[1][0] - 1.0) < 1e-9);
    // Bin (1,1): skin=3, nonSkin=0, total=3 → P(color)=3/16, P(color|skin)=3/10=0.3 → posterior = 0.3*0.625/(0.1875) = 1.0
    assert(std::fabs(result2[1][1] - 1.0) < 1e-9);

    // Edge case: a bin with zero total count should yield 0.0.
    std::vector<std::vector<unsigned long>> skin3 = {{0}};
    std::vector<std::vector<unsigned long>> nonSkin3 = {{0}};
    // But both histograms have zero total, so this must throw.
    bool threw = false;
    try {
        computeSkinProbabilityMatrix(skin3, nonSkin3);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Edge case: one histogram has zero total, the other has positive.
    std::vector<std::vector<unsigned long>> skin4 = {{1}};
    std::vector<std::vector<unsigned long>> nonSkin4 = {{0}};
    threw = false;
    try {
        computeSkinProbabilityMatrix(skin4, nonSkin4);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

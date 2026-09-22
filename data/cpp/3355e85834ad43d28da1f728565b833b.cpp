// Write a C++ function `double pearsonCorrelation(const std::vector<double>& data, const std::vector<double>& noise)` that computes the Pearson correlation coefficient between two equal-length numeric sequences. The function must return `0.0` if either sequence has zero variance (i.e., all values identical), and must handle empty input by returning `0.0` as well. The correlation coefficient is defined as the covariance of the two sequences divided by the product of their standard deviations, where each standard deviation is the square root of the variance (using the population formula with denominator \(n\)). The function must not modify the input vectors, must use `const` references for parameters, and must include proper validation for mismatched sizes (returning `0.0` in that case as well).
The solution computes the Pearson correlation coefficient using the standard formula: \(\rho = \frac{\sum (x_i - \bar{x})(y_i - \bar{y})}{\sqrt{\sum (x_i - \bar{x})^2} \sqrt{\sum (y_i - \bar{y})^2}}\). The algorithm first checks for empty or size-mismatched inputs, returning `0.0` because correlation is undefined without paired data. Then it computes the means of both sequences in a single pass, followed by a second pass that accumulates the covariance and variance sums. After the second pass, it checks if either variance sum is zero (which implies constant sequence) and returns `0.0` to avoid division by zero. Finally, it computes the correlation as covariance divided by the product of square roots of variances. Edge cases include: (1) empty vectors → `0.0`, (2) vectors of unequal length → `0.0`, (3) any vector with all identical values → `0.0` (since correlation is undefined), (4) negative values which are handled naturally by the formulas. Time complexity is \(O(n)\) because we perform two linear passes over the data (one for means, one for sums), and space complexity is \(O(1)\) since we only store a few scalar accumulators.
#include <vector>
#include <cmath>

// Compute Pearson correlation coefficient between two equal-length numeric sequences.
// Returns 0.0 if inputs are empty, mismatched, or either sequence has zero variance.
double pearsonCorrelation(const std::vector<double>& data, const std::vector<double>& noise) {
    // Validate input: empty or mismatched sizes -> undefined, return 0.0
    if (data.empty() || data.size() != noise.size()) {
        return 0.0;
    }

    const std::size_t n = data.size();

    // Compute means of both sequences
    double meanData = 0.0;
    double meanNoise = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        meanData += data[i];
        meanNoise += noise[i];
    }
    meanData /= static_cast<double>(n);
    meanNoise /= static_cast<double>(n);

    // Accumulate covariance and variance sums
    double cov = 0.0;
    double varData = 0.0;
    double varNoise = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double diffData = data[i] - meanData;
        const double diffNoise = noise[i] - meanNoise;
        cov += diffData * diffNoise;
        varData += diffData * diffData;
        varNoise += diffNoise * diffNoise;
    }

    // If either variance is zero, correlation is undefined
    if (varData == 0.0 || varNoise == 0.0) {
        return 0.0;
    }

    // Compute correlation coefficient
    return cov / (std::sqrt(varData) * std::sqrt(varNoise));
}
#include <cassert>
#include <cmath>
#include <vector>

// Declare the function under test (assuming it's in the same translation unit or included)
double pearsonCorrelation(const std::vector<double>& data, const std::vector<double>& noise);

int main() {
    // Perfect positive correlation
    std::vector<double> a1 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> b1 = {2.0, 4.0, 6.0, 8.0};
    assert(std::fabs(pearsonCorrelation(a1, b1) - 1.0) < 1e-9);

    // Perfect negative correlation
    std::vector<double> a2 = {1.0, 2.0, 3.0};
    std::vector<double> b2 = {3.0, 2.0, 1.0};
    assert(std::fabs(pearsonCorrelation(a2, b2) - (-1.0)) < 1e-9);

    // Zero correlation (independent sequences)
    std::vector<double> a3 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> b3 = {5.0, 6.0, 7.0, 8.0}; // linear but shift, actually correlated? Use explicit independent
    // Use orthogonal pattern: squares vs cubes? Simple: use {1,2,3,4} and {1,0,-1,0}? Let's construct genuinely uncorrelated
    std::vector<double> independent = {1.0, 0.0, -1.0, 0.0}; // mean=0, symmetric
    double corr_zero = pearsonCorrelation(a3, independent);
    assert(std::fabs(corr_zero) < 1e-9); // should be near zero

    // Constant sequence returns 0.0
    std::vector<double> const_a = {5.0, 5.0, 5.0};
    std::vector<double> var_b = {1.0, 2.0, 3.0};
    assert(pearsonCorrelation(const_a, var_b) == 0.0);

    // Empty vectors return 0.0
    std::vector<double> empty1;
    std::vector<double> empty2;
    assert(pearsonCorrelation(empty1, empty2) == 0.0);

    // Mismatched sizes return 0.0
    std::vector<double> small = {1.0, 2.0};
    std::vector<double> large = {1.0, 2.0, 3.0};
    assert(pearsonCorrelation(small, large) == 0.0);

    // Single element: both variances zero -> 0.0
    std::vector<double> one = {3.5};
    std::vector<double> one2 = {7.2};
    assert(pearsonCorrelation(one, one2) == 0.0);

    // Non-trivial correlation with known value
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {2.0, 4.0, 5.0, 4.0, 5.0};
    double expected = 0.8; // known approximate
    double actual = pearsonCorrelation(x, y);
    assert(std::fabs(actual - expected) < 0.01);

    return 0;
}

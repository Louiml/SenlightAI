// Implement a C++ function that computes several statistical properties of a sample stored in a `std::vector<double>`. The function must accept a vector of real numbers and return a formatted string containing the minimum, maximum, sum, mean, median, variance (uncorrected, dividing by n), standard deviation (square root of variance), relative standard deviation (std/mean), median absolute deviation (MAD), and quantile for a specified probability p (with p between 0 and 1). The sample can be empty, in which case all statistics except size (0) should be replaced with "N/A". For non-empty samples, handle both odd and even sizes for median and MAD correctly, and for quantile, round the index outward (up for p > 0.5, down for p ≤ 0.5). The output format should be a single string with named keys and values separated by commas, e.g., `"min=..., max=..., sum=..., mean=..., med=..., var=..., stddev=..., relstddev=..., mad=..., q=..."`. The function must be const-correct and use only standard C++ libraries.

The solution involves a single pass over the input vector to compute sum, min, max, and mean (sum divided by size). The median requires sorting a copy of the vector; then for odd size take the middle element, for even size average the two central elements. Variance is computed via a second pass accumulating (x - mean)^2 and dividing by size (uncorrected). Standard deviation is the square root of variance. Relative standard deviation is std/mean (ensure mean non-zero; if zero, return "N/A" for that field). MAD: compute deviations |x - median|, then find the median of those deviations; for odd count take the middle sorted value, for even count average the two central values (same as median logic). Quantile: compute index = floor(p * (size-1)) if p ≤ 0.5, else ceil(p * (size-1)), clamp to range, and return the element at that index from the sorted copy. Edge cases: empty vector returns "size=0" and all other keys as "N/A". For a single-element sample, median and MAD equal that element, variance = 0, relative std = 0 if mean non-zero. Time complexity: O(n log n) due to sorting for median/MAD/quantile, plus O(n) for sum and variance; space O(n) for sorted copy and deviations vector. Use `std::ostringstream` for formatting with fixed precision (e.g., 6 decimals).

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>

// Compute statistical properties of a sample and return a formatted string.
// The vector can be empty; statistics are "N/A" except size=0.
// p is a probability for quantile (0<=p<=1). The index is rounded outward.
std::string sampleStatistics(const std::vector<double>& sample, double p = 0.5) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(6);

    if (sample.empty()) {
        out << "size=0, min=N/A, max=N/A, sum=N/A, mean=N/A, med=N/A, "
            << "var=N/A, stddev=N/A, relstddev=N/A, mad=N/A, q=N/A";
        return out.str();
    }

    size_t n = sample.size();

    // First pass: sum, min, max
    double sum = 0.0;
    double minVal = sample[0];
    double maxVal = sample[0];
    for (double x : sample) {
        sum += x;
        if (x < minVal) minVal = x;
        if (x > maxVal) maxVal = x;
    }
    double mean = sum / static_cast<double>(n);

    // Variance
    double sqSum = 0.0;
    for (double x : sample) {
        double diff = x - mean;
        sqSum += diff * diff;
    }
    double variance = sqSum / static_cast<double>(n);
    double stddev = std::sqrt(variance);
    double relstddev = (mean != 0.0) ? (stddev / std::fabs(mean)) : std::numeric_limits<double>::quiet_NaN();

    // Sorted copy for median and quantile
    std::vector<double> sorted = sample;
    std::sort(sorted.begin(), sorted.end());

    // Median
    double median;
    if (n % 2 == 1) {
        median = sorted[n / 2];
    } else {
        median = 0.5 * (sorted[n/2 - 1] + sorted[n/2]);
    }

    // MAD
    std::vector<double> deviations;
    deviations.reserve(n);
    for (double x : sample) {
        deviations.push_back(std::fabs(x - median));
    }
    std::sort(deviations.begin(), deviations.end());
    double mad;
    if (deviations.size() % 2 == 1) {
        mad = deviations[deviations.size() / 2];
    } else {
        size_t mid = deviations.size() / 2;
        mad = 0.5 * (deviations[mid - 1] + deviations[mid]);
    }

    // Quantile
    size_t idx;
    if (p > 0.5) {
        idx = static_cast<size_t>(std::ceil(p * static_cast<double>(n - 1)));
    } else {
        idx = static_cast<size_t>(std::floor(p * static_cast<double>(n - 1)));
    }
    if (idx >= n) idx = n - 1;
    double quantile = sorted[idx];

    out << "size=" << n << ", ";
    out << "min=" << minVal << ", ";
    out << "max=" << maxVal << ", ";
    out << "sum=" << sum << ", ";
    out << "mean=" << mean << ", ";
    out << "med=" << median << ", ";
    out << "var=" << variance << ", ";
    out << "stddev=" << stddev << ", ";
    if (mean == 0.0) {
        out << "relstddev=N/A, ";
    } else {
        out << "relstddev=" << relstddev << ", ";
    }
    out << "mad=" << mad << ", ";
    out << "q=" << quantile;
    return out.str();
}

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

// The solution function is declared above (sampleStatistics).

int main() {
    // Empty sample
    std::string empty = sampleStatistics({}, 0.5);
    assert(empty.find("size=0") != std::string::npos);
    assert(empty.find("min=N/A") != std::string::npos);
    assert(empty.find("mean=N/A") != std::string::npos);

    // Single element
    std::string single = sampleStatistics({5.0}, 0.5);
    assert(single.find("min=5.000000") != std::string::npos);
    assert(single.find("max=5.000000") != std::string::npos);
    assert(single.find("mean=5.000000") != std::string::npos);
    assert(single.find("med=5.000000") != std::string::npos);
    assert(single.find("var=0.000000") != std::string::npos);
    assert(single.find("mad=0.000000") != std::string::npos);

    // Odd size, known values: {1,2,3,4,5}
    std::string odd = sampleStatistics({1.0,2.0,3.0,4.0,5.0}, 0.5);
    assert(odd.find("min=1.000000") != std::string::npos);
    assert(odd.find("max=5.000000") != std::string::npos);
    assert(odd.find("sum=15.000000") != std::string::npos);
    assert(odd.find("mean=3.000000") != std::string::npos);
    assert(odd.find("med=3.000000") != std::string::npos);
    assert(odd.find("var=2.000000") != std::string::npos);
    assert(odd.find("stddev=1.414214") != std::string::npos);
    assert(odd.find("mad=1.000000") != std::string::npos);

    // Even size: {2,4,6,8}
    std::string even = sampleStatistics({2.0,4.0,6.0,8.0}, 0.25);
    assert(even.find("min=2.000000") != std::string::npos);
    assert(even.find("max=8.000000") != std::string::npos);
    assert(even.find("sum=20.000000") != std::string::npos);
    assert(even.find("mean=5.000000") != std::string::npos);
    assert(even.find("med=5.000000") != std::string::npos);
    assert(even.find("var=5.000000") != std::string::npos);
    // Quantile p=0.25 => index floor(0.25*3)=0 => 2
    assert(even.find("q=2.000000") != std::string::npos);

    // Quantile p=0.75 for {1,2,3,4} -> ceil(0.75*3)=3 -> 4
    std::string qtest = sampleStatistics({1.0,2.0,3.0,4.0}, 0.75);
    assert(qtest.find("q=4.000000") != std::string::npos);

    // Negative values and relative std
    std::string neg = sampleStatistics({-2.0, -4.0, -6.0}, 0.5);
    // mean = -4, var = (4+0+4)/3 = 2.666667
    assert(neg.find("mean=-4.000000") != std::string::npos);
    assert(neg.find("var=2.666667") != std::string::npos);
    // relstddev = stddev / |mean| = sqrt(2.666667)/4 ≈ 0.408248
    assert(neg.find("relstddev=0.408248") != std::string::npos);

    // Duplicate values
    std::string dup = sampleStatistics({3.0, 3.0, 3.0}, 0.5);
    assert(dup.find("min=3.000000") != std::string::npos);
    assert(dup.find("max=3.000000") != std::string::npos);
    assert(dup.find("med=3.000000") != std::string::npos);
    assert(dup.find("var=0.000000") != std::string::npos);
    assert(dup.find("mad=0.000000") != std::string::npos);

    return 0;
}

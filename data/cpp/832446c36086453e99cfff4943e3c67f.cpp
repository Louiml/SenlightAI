// Create a C++ function `analyzeVectorStats` that takes a `const std::vector<double>&` (by const reference) and returns a `std::map<std::string, double>` containing the following statistics as keys: "min", "max", "mean", "median", "variance", "stddev", "q1", "q2", "q3", and "mode" (if the mode is uniquely defined, otherwise use `std::nan("")`). The input vector may be empty (in which case all values should be `std::nan("")`), may contain negative numbers, duplicates, and need not be sorted. The median is the middle value when the data is sorted (for even size, the average of the two middle values). Quartiles: Q1 is the median of the lower half (excluding the overall median if the size is odd), Q3 is the median of the upper half (excluding the overall median if odd). Variance is the population variance (divide by N). Mode is the most frequent value; if multiple values tie for the highest frequency, return `std::nan("")`.

// The solution involves several steps. First, handle the empty input case by returning a map with all values set to `std::nan("")`. For non‑empty input, compute minimum and maximum by iterating once. Compute the mean as the sum divided by size. Then sort a copy of the data to compute median, quartiles, and mode. Median: if size odd, take the middle; if even, average the two middle. For quartiles: split the sorted data into lower and upper halves. If the size is odd, exclude the median element; if even, split exactly in half. Compute Q1 as the median of the lower half and Q3 as the median of the upper half. Variance: sum of squared differences from the mean divided by size. Standard deviation is the square root. Mode: use a frequency map (unordered_map) to count occurrences; if the maximum count is strictly greater than all other counts (i.e., unique), use that value, else `std::nan("")`. Important edge cases: size 1 sets median, Q1, Q3 all to that single value; variance and stddev are 0. For data with ties in frequency, mode is NaN. Time complexity: sorting is O(N log N), other operations are O(N) (freq map is O(N) average). Space complexity: O(N) for the sorted copy and O(N) for the frequency map.

#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <limits>

std::map<std::string, double> analyzeVectorStats(const std::vector<double>& data) {
    std::map<std::string, double> stats;
    double nan_val = std::numeric_limits<double>::quiet_NaN();

    if (data.empty()) {
        stats["min"] = nan_val;
        stats["max"] = nan_val;
        stats["mean"] = nan_val;
        stats["median"] = nan_val;
        stats["variance"] = nan_val;
        stats["stddev"] = nan_val;
        stats["q1"] = nan_val;
        stats["q2"] = nan_val;
        stats["q3"] = nan_val;
        stats["mode"] = nan_val;
        return stats;
    }

    size_t n = data.size();
    double sum = 0.0;
    double min_val = data[0];
    double max_val = data[0];
    for (double x : data) {
        sum += x;
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }
    double mean = sum / static_cast<double>(n);

    // Sorted copy for median, quartiles, mode
    std::vector<double> sorted(data);
    std::sort(sorted.begin(), sorted.end());

    // Median (q2)
    double median;
    if (n % 2 == 1) {
        median = sorted[n / 2];
    } else {
        median = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
    }

    // Quartiles
    double q1, q3;
    if (n == 1) {
        q1 = q3 = sorted[0];
    } else {
        size_t mid = n / 2;
        std::vector<double> lower(sorted.begin(), sorted.begin() + mid);
        std::vector<double> upper;
        if (n % 2 == 1) {
            upper.assign(sorted.begin() + mid + 1, sorted.end());
        } else {
            upper.assign(sorted.begin() + mid, sorted.end());
        }
        // Median of lower half
        size_t m = lower.size();
        if (m == 0) q1 = nan_val;
        else if (m % 2 == 1) q1 = lower[m / 2];
        else q1 = (lower[m / 2 - 1] + lower[m / 2]) / 2.0;
        // Median of upper half
        size_t m2 = upper.size();
        if (m2 == 0) q3 = nan_val;
        else if (m2 % 2 == 1) q3 = upper[m2 / 2];
        else q3 = (upper[m2 / 2 - 1] + upper[m2 / 2]) / 2.0;
    }

    // Variance and stddev
    double var_sum = 0.0;
    for (double x : data) {
        double diff = x - mean;
        var_sum += diff * diff;
    }
    double variance = var_sum / static_cast<double>(n);
    double stddev = std::sqrt(variance);

    // Mode: unique maximum frequency
    std::unordered_map<double, int> freq;
    for (double x : data) {
        freq[x]++;
    }
    int max_count = 0;
    double mode_val = nan_val;
    bool unique = true;
    for (const auto& pair : freq) {
        if (pair.second > max_count) {
            max_count = pair.second;
            mode_val = pair.first;
            unique = true;
        } else if (pair.second == max_count) {
            unique = false;
        }
    }
    if (!unique || max_count == 1) {
        // If all values occur once, there are multiple modes (all of them), so NaN
        if (max_count == 1 && freq.size() > 1) mode_val = nan_val;
        else if (!unique) mode_val = nan_val;
    }

    stats["min"] = min_val;
    stats["max"] = max_val;
    stats["mean"] = mean;
    stats["median"] = median;
    stats["variance"] = variance;
    stats["stddev"] = stddev;
    stats["q1"] = q1;
    stats["q2"] = median;
    stats["q3"] = q3;
    stats["mode"] = mode_val;

    return stats;
}

#include <cassert>
#include <cmath>
#include <map>
#include <string>
#include <vector>

// (Function declaration or include of the above solution goes here)

int main() {
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0, 5.0};
    auto s1 = analyzeVectorStats(v1);
    assert(s1["min"] == 1.0);
    assert(s1["max"] == 5.0);
    assert(s1["mean"] == 3.0);
    assert(s1["median"] == 3.0);
    assert(std::abs(s1["variance"] - 2.0) < 1e-9);
    assert(std::abs(s1["stddev"] - std::sqrt(2.0)) < 1e-9);
    assert(s1["q1"] == 2.0);
    assert(s1["q2"] == 3.0);
    assert(s1["q3"] == 4.0);
    assert(s1["mode"] == 3.0);  // unique max

    std::vector<double> v2 = {1.0, 2.0, 3.0, 4.0};
    auto s2 = analyzeVectorStats(v2);
    assert(s2["median"] == 2.5);
    assert(s2["q1"] == 1.5);
    assert(s2["q3"] == 3.5);

    std::vector<double> v3 = {1.0, 1.0, 2.0, 2.0, 3.0};
    auto s3 = analyzeVectorStats(v3);
    assert(std::isnan(s3["mode"]));  // tie between 1 and 2

    std::vector<double> v4 = {5.0};
    auto s4 = analyzeVectorStats(v4);
    assert(s4["min"] == 5.0);
    assert(s4["mean"] == 5.0);
    assert(s4["variance"] == 0.0);
    assert(s4["q1"] == 5.0);
    assert(s4["q3"] == 5.0);

    std::vector<double> v5 = {};
    auto s5 = analyzeVectorStats(v5);
    assert(std::isnan(s5["min"]));
    assert(std::isnan(s5["max"]));
    assert(std::isnan(s5["median"]));

    std::vector<double> v6 = {-10.0, -10.0, 0.0, 10.0, 10.0};
    auto s6 = analyzeVectorStats(v6);
    assert(s6["min"] == -10.0);
    assert(s6["max"] == 10.0);
    assert(s6["mean"] == 0.0);
    assert(s6["median"] == 0.0);
    assert(s6["q1"] == -10.0);
    assert(s6["q3"] == 10.0);
    assert(std::isnan(s6["mode"]));  // tie at -10 and 10

    std::vector<double> v7 = {2.0, 2.0, 2.0, 3.0};
    auto s7 = analyzeVectorStats(v7);
    assert(s7["mode"] == 2.0);
    assert(s7["variance"] == 0.25);
}

Write a C++ function named `computeTravelTimePercentiles` that takes as input a vector of vectors of non-negative double values (each inner vector represents the sum of lane-kilometers in each travel-time-ratio bin for one time period, with bins ranging from ratio 0 to a maximum ratio), along with a vector of percentile break percentages (integers between 0 and 100), and a resolution factor (an integer, e.g., 100, meaning each bin covers 1/resolution in ratio units). The function should return a vector of strings, one per input time period, where each string contains the percentile travel-time-ratio values formatted to two decimal places separated by spaces, followed by a space, then the total lane-kilometers divided by 1000 formatted as an integer (rounded to nearest integer, using floor for .5 and above? Use standard rounding: add 0.5 and cast to int). If a time period has total sum equal to 0, the output string should be empty for that period. The function must handle percentiles by scanning cumulative sums and assigning the bin index where the cumulative percentage first reaches or exceeds the target percentile. For the final percentile values, divide the bin index by the resolution factor. The number of percentile values corresponds to the size of the percentile break vector (which must be sorted ascending). Apply const correctness where appropriate, and assume all input vectors are non-empty and all inner vectors have the same length. Provide a solution that is self-contained with necessary headers (e.g., <vector>, <string>, <cmath>, <cstring>) but do not include a main function.

// The core algorithm processes each time period independently. For a given period, sum all bin values to obtain the total. If total is zero, output an empty string. Otherwise, compute the cumulative sum across bins. For each requested percentile (in the given order, sorted ascending), maintain a pointer `k1` that starts at 0. As we accumulate bin sums, compute the current percentage = (int)(cumulative_sum * 100.0 / total + 0.5) (standard rounding, but note this is integer percentage, so rounding half-up). Then, while there are remaining percentiles and the current `percent_break[k]` is less than or equal to this percentage, assign the current bin index `j` as the percentile value, then increment `k1`. This is similar to the original snippet. After scanning all bins, any percentiles not assigned (if cumulative never reaches 100% due to rounding) remain at their default value (e.g., set to 0, but in practice it should always reach 100% for the last bin, because total sum is divided by total, so last cumulative percentage is exactly 100). However, due to integer rounding, it's safest to initialize the percentile array to zero and after the loop set any unassigned percentiles to the last bin index (NUM_SUM_BINS - 1). In the solution, we can pre-initialize `percentile[k]` to the last bin index. Then, format each percentile value as `(double)percentile[k] / RESOLUTION` with two decimal places using a helper to format. Also compute total_km = (long long)(total / 1000.0 + 0.5) (round to nearest integer). The output string for one period concatenates the percentiles with spaces. Edge cases include empty vectors (though specification says non-empty), total zero, percentiles beyond 100, and rounding. Time complexity is O(P * B) where P = number of periods, B = number of bins, because for each period we scan all bins and for each bin we may assign multiple percentiles, but since each percentile is assigned at most once, the inner loop over percentiles is amortized O(B + percentile_count) per period. Space complexity is O(percentile_count) for the auxiliary array per period, plus the output string.

#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>

// Helper to format a double with 2 decimal places.
static std::string formatDouble(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

// Compute travel time percentile distribution for each time period.
// Each inner vector in 'sumBin' holds sums for bins (ratio indices).
// 'percentBreaks' are percentile thresholds (0-100) sorted ascending.
// 'resolution' is bin granularity (e.g., 100 means 1/100 ratio per bin).
// Returns a vector of strings; empty string if that period has total 0.
std::vector<std::string> computeTravelTimePercentiles(
    const std::vector<std::vector<double>>& sumBin,
    const std::vector<int>& percentBreaks,
    int resolution
) {
    size_t numPeriods = sumBin.size();
    size_t numBins = sumBin[0].size();
    size_t numPercentiles = percentBreaks.size();

    std::vector<std::string> output(numPeriods);

    for (size_t i = 0; i < numPeriods; ++i) {
        double total = 0.0;
        for (double val : sumBin[i]) {
            total += val;
        }

        if (total == 0.0) {
            output[i] = "";
            continue;
        }

        // percentile[k] stores bin index for each requested break.
        std::vector<int> percentile(numPercentiles, static_cast<int>(numBins - 1));

        size_t k1 = 0;
        double sum = 0.0;

        for (size_t j = 0; j < numBins; ++j) {
            sum += sumBin[i][j];
            int percent = static_cast<int>(sum * 100.0 / total + 0.5);

            while (k1 < numPercentiles && percentBreaks[k1] <= percent) {
                percentile[k1] = static_cast<int>(j);
                ++k1;
            }
        }

        // Build the output string.
        std::string line;
        line.reserve(numPercentiles * 7 + 12);
        for (size_t k = 0; k < numPercentiles; ++k) {
            if (k > 0) line += ' ';
            line += formatDouble(static_cast<double>(percentile[k]) / resolution);
        }
        // Append total lane-km (rounded to nearest integer).
        long long totalKm = static_cast<long long>(total / 1000.0 + 0.5);
        line += ' ';
        line += std::to_string(totalKm);

        output[i] = line;
    }

    return output;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (omitted for brevity in test, but assumed present).

int main() {
    // Simple case: one period, 10 bins, each bin has exactly 1 lane-km.
    // Percentile breaks: 50, 80, 100. Resolution = 100.
    std::vector<std::vector<double>> data1 = {{1,1,1,1,1,1,1,1,1,1}};
    std::vector<int> breaks1 = {50, 80, 100};
    auto res1 = computeTravelTimePercentiles(data1, breaks1, 100);
    // Cumulative at bin 4 (0-indexed) is 5/10=50%, at bin 7 is 8/10=80%, at bin 9 is 100%.
    // So percentiles = 4,7,9 -> ratios 0.04, 0.07, 0.09.
    // Total = 10 lane-km -> 10/1000 = 0.01 -> rounded to 0.
    assert(res1.size() == 1);
    assert(res1[0] == "0.04 0.07 0.09 0");

    // Two periods, second period has total zero.
    std::vector<std::vector<double>> data2 = {{5,5,0,0,0}, {0,0,0,0,0}};
    std::vector<int> breaks2 = {50, 100};
    auto res2 = computeTravelTimePercentiles(data2, breaks2, 1);
    // First period: total=10. Cumulative at bin0 = 5 => 50%, bin1 = 10 => 100%.
    // percentiles bin0=0, bin1=1 -> ratios 0.0, 1.0.
    // total = 10/1000 = 0.01 -> 0.
    assert(res2.size() == 2);
    assert(res2[0] == "0.00 1.00 0");
    assert(res2[1] == "");

    // Single bin, multiple periods, infinite total.
    std::vector<std::vector<double>> data3 = {{7.0}, {3.5}};
    std::vector<int> breaks3 = {0, 100};
    auto res3 = computeTravelTimePercentiles(data3, breaks3, 100);
    // Both periods have total > 0. Bin0 always gives 100% at first bin, so both percentiles are bin0.
    // ratio = 0/100 = 0.00.
    // totals: 7/1000=0.007 ->0; 3.5/1000=0.0035 ->0.
    assert(res3.size() == 2);
    assert(res3[0] == "0.00 0.00 0");
    assert(res3[1] == "0.00 0.00 0");

    // Higher total to test rounding of totalKm.
    std::vector<std::vector<double>> data4 = {{500.0, 500.0}};
    std::vector<int> breaks4 = {50};
    auto res4 = computeTravelTimePercentiles(data4, breaks4, 10);
    // total=1000, bin0=500 => 50% at bin0, so percentile=0 -> ratio 0.0.
    // totalKm = 1000/1000 = 1.0 -> 1.
    assert(res4.size() == 1);
    assert(res4[0] == "0.00 1");

    // Many bins, ensure no out-of-bounds.
    std::vector<double> big(200, 1.0);
    std::vector<std::vector<double>> data5 = {big};
    std::vector<int> breaks5 = {25, 50, 75};
    auto res5 = computeTravelTimePercentiles(data5, breaks5, 1);
    // total=200. Cumulative at bin49 =50 ->25%, bin99=100->50%, bin149=150->75%.
    // ratios: 49, 99, 149. total=200/1000=0.2 ->0.
    assert(res5.size() == 1);
    assert(res5[0] == "49.00 99.00 149.00 0");

    return 0;
}

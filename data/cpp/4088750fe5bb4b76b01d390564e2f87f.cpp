// Write a C++ function that takes a `std::vector<double>` of bin identifiers (which may contain duplicates and appear in any order) and a second `std::vector<double>` of corresponding values (same length), then returns a `std::map<double, double>` where each key is a unique bin identifier from the first vector, and its mapped value is the sum of all values from the second vector whose corresponding bin identifier equals that key. The bins and values vectors must have equal sizes; if they are empty, return an empty map. The function should be named `sumByBin`, and both input vectors should be passed as `const std::vector<double>&` for efficiency and correctness. Assume bin identifiers are finite real numbers (no NaN or infinity), and values can be any finite `double` (positive, negative, zero). The function must be self-contained (only include necessary standard headers).
// The core idea is to accumulate sums for each unique bin identifier using a `std::map<double, double>` as the container, because it automatically sorts keys and allows efficient insertion/lookup via `operator[]` or `insert`. The algorithm iterates through the vectors in parallel, using the bin identifier as the key and adding the corresponding value to the map's entry for that bin. Since `std::map::operator[]` default-constructs a zero value for a new key, the accumulation is straightforward. Edge cases: empty input (return empty map), single element (map has one entry), duplicate bins (sums accumulate), and negative values (sums correctly). The main risk is mismatched vector lengths; we must handle this by throwing `std::invalid_argument` or `std::length_error` — the task does not specify, so we can choose to throw `std::invalid_argument` for clarity. Time complexity is \(O(n \log k)\) where \(n\) is the number of elements and \(k\) is the number of unique bins, because each map insertion/lookup is \(O(\log k)\). Space complexity is \(O(k)\) for the map, plus \(O(1)\) auxiliary. Using `const` references avoids copies. The implementation will include `<map>`, `<vector>`, `<stdexcept>`, and optionally `<utility>`.
#include <map>
#include <vector>
#include <stdexcept>

/**
 * @brief Sums values grouped by their corresponding bin identifiers.
 * 
 * @param bins A vector of bin identifiers (may contain duplicates).
 * @param values A vector of values with the same length as bins.
 * @return std::map<double, double> A map from each unique bin to the sum of its values.
 * @throws std::invalid_argument if the input vectors have different sizes.
 */
std::map<double, double> sumByBin(const std::vector<double>& bins, const std::vector<double>& values) {
    if (bins.size() != values.size()) {
        throw std::invalid_argument("bins and values must have the same length");
    }

    std::map<double, double> binToSum;

    for (std::size_t i = 0; i < bins.size(); ++i) {
        binToSum[bins[i]] += values[i];
    }

    return binToSum;
}
#include <cassert>
#include <map>
#include <vector>

// Declaration of the solution function (assumed to be included from the solution header/source)
std::map<double, double> sumByBin(const std::vector<double>& bins, const std::vector<double>& values);

int main() {
    // Basic case with duplicates
    std::vector<double> bins1 = {1.0, 2.0, 1.0, 3.0, 2.0, 1.0};
    std::vector<double> vals1 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    std::map<double, double> result1 = sumByBin(bins1, vals1);
    std::map<double, double> expected1 = {{1.0, 10.0}, {2.0, 7.0}, {3.0, 4.0}};
    assert(result1 == expected1);

    // Empty input
    std::vector<double> bins2;
    std::vector<double> vals2;
    assert(sumByBin(bins2, vals2).empty());

    // Single element
    std::vector<double> bins3 = {5.0};
    std::vector<double> vals3 = {-2.5};
    std::map<double, double> result3 = sumByBin(bins3, vals3);
    assert(result3.size() == 1 && result3[5.0] == -2.5);

    // Negative values and unsorted bins
    std::vector<double> bins4 = {10.0, -5.0, 10.0, -5.0};
    std::vector<double> vals4 = {1.5, -3.0, 2.5, 4.0};
    std::map<double, double> result4 = sumByBin(bins4, vals4);
    std::map<double, double> expected4 = {{-5.0, 1.0}, {10.0, 4.0}};
    assert(result4 == expected4);

    // All same bin
    std::vector<double> bins5 = {0.0, 0.0, 0.0};
    std::vector<double> vals5 = {1.0, 2.0, 3.0};
    std::map<double, double> result5 = sumByBin(bins5, vals5);
    assert(result5.size() == 1 && result5[0.0] == 6.0);

    // Zero values and zero bins
    std::vector<double> bins6 = {0.0, 0.0, 0.0};
    std::vector<double> vals6 = {0.0, 0.0, 0.0};
    std::map<double, double> result6 = sumByBin(bins6, vals6);
    assert(result6.size() == 1 && result6[0.0] == 0.0);

    // Test that mismatched sizes throw
    bool threw = false;
    try {
        std::vector<double> badBins = {1.0};
        std::vector<double> badVals = {1.0, 2.0};
        sumByBin(badBins, badVals);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}

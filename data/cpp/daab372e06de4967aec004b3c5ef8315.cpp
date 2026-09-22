// Write a C++ function named `averageExtremes` that takes a `std::vector<int>` of daily temperature readings and returns a `std::tuple<int, int, int>` containing the average (integer division, rounded toward zero), the minimum, and the maximum temperatures, in that order. The function must handle an empty input vector by returning a tuple of three zeros. The input vector should be passed by `const` reference to avoid unnecessary copying. The function must be declared in a header or as a free function in a source file (without `main`), and must be compilable with C++17 or later.
The solution processes the vector in a single pass. If the vector is empty, return `{0,0,0}` immediately. Otherwise, initialize `min`, `max`, and `sum` using the first element. Then iterate from the second element onward (or all elements if using a range-based loop from the beginning after initialization), updating the sum, and comparing each element with the current min and max. After the loop, compute the average as `sum / temperatures.size()` using integer division (which truncates toward zero). The tuple is constructed with `std::make_tuple(avg, min, max)`. Edge cases: single-element vector (avg=min=max=that element), all negative values, all equal values, and large numbers that may cause integer overflow in `sum`—for robustness, the sum is stored as `long long` to avoid overflow in typical scenarios. Time complexity is O(n), space complexity is O(1) besides the result tuple.
#include <tuple>
#include <vector>

// Returns a tuple of (average, minimum, maximum) for the input temperatures.
// For an empty vector, returns (0, 0, 0).
std::tuple<int, int, int> averageExtremes(const std::vector<int>& temperatures) {
    if (temperatures.empty()) {
        return std::make_tuple(0, 0, 0);
    }

    int min = temperatures.front();
    int max = temperatures.front();
    long long sum = 0;

    for (const int& value : temperatures) {
        sum += value;
        if (value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
    }

    int avg = static_cast<int>(sum / static_cast<long long>(temperatures.size()));
    return std::make_tuple(avg, min, max);
}
#include <cassert>
#include <tuple>
#include <vector>

// Declaration of the function under test (assumed to be available from the solution).
std::tuple<int, int, int> averageExtremes(const std::vector<int>& temperatures);

int main() {
    // Basic positive values
    std::vector<int> v1 = {10, 20, 30, 40};
    auto r1 = averageExtremes(v1);
    assert(std::get<0>(r1) == 25);
    assert(std::get<1>(r1) == 10);
    assert(std::get<2>(r1) == 40);

    // Mixed positive and negative
    std::vector<int> v2 = {-5, 0, 5, 10};
    auto r2 = averageExtremes(v2);
    assert(std::get<0>(r2) == 2);  // (10)/4 = 2 (integer division)
    assert(std::get<1>(r2) == -5);
    assert(std::get<2>(r2) == 10);

    // Single element
    std::vector<int> v3 = {7};
    auto r3 = averageExtremes(v3);
    assert(std::get<0>(r3) == 7);
    assert(std::get<1>(r3) == 7);
    assert(std::get<2>(r3) == 7);

    // Duplicate values
    std::vector<int> v4 = {3, 3, 3, 3};
    auto r4 = averageExtremes(v4);
    assert(std::get<0>(r4) == 3);
    assert(std::get<1>(r4) == 3);
    assert(std::get<2>(r4) == 3);

    // All negatives with truncation toward zero (e.g., -1, -2 => sum -3, avg -1)
    std::vector<int> v5 = {-1, -2, -3};
    auto r5 = averageExtremes(v5);
    assert(std::get<0>(r5) == -2);  // -6/3 = -2
    assert(std::get<1>(r5) == -3);
    assert(std::get<2>(r5) == -1);

    // Empty vector
    std::vector<int> v6;
    auto r6 = averageExtremes(v6);
    assert(std::get<0>(r6) == 0);
    assert(std::get<1>(r6) == 0);
    assert(std::get<2>(r6) == 0);

    // Larger set with descending order
    std::vector<int> v7 = {100, 50, 25, 0};
    auto r7 = averageExtremes(v7);
    assert(std::get<0>(r7) == 43);  // 175/4 = 43
    assert(std::get<1>(r7) == 0);
    assert(std::get<2>(r7) == 100);

    return 0;
}

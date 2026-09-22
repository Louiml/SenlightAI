Write a C++ function `ratioReport` that accepts a non-empty vector of integers and returns a `std::array<double, 3>` containing, in order, the ratio of positive numbers, the ratio of negative numbers, and the ratio of zeros, each rounded to exactly six decimal places. The function must not print anything; instead, it must return the three ratios so the caller can format them as needed. The input vector may contain large values, only positive numbers, only zeros, or a mix, and the function should handle negative numbers correctly. The returned values must use standard double-precision arithmetic and be rounded to six decimal places using `std::round` or a similar method to avoid floating-point representation issues.
// The solution requires a single pass over the input vector, counting occurrences of positive (>0), negative (<0), and zero (==0) elements. After counting, compute each ratio as `count / total` where `total` is the vector’s size. To round to exactly six decimal places, multiply each ratio by 1,000,000, round to the nearest integer using `std::round`, then divide by 1,000,000. This ensures the returned double has at most six fractional digits, matching typical output precision. Edge cases: an all-zero vector yields ratios {0.0, 0.0, 1.0}; an all-positive vector yields {1.0, 0.0, 0.0}; the size is guaranteed non-empty so no division by zero. Time complexity is O(n) for counting, O(1) auxiliary space (only three counters). The rounding step is O(1) per ratio.
#include <array>
#include <vector>
#include <cmath>

// Returns the ratios of positive, negative, and zero elements respectively,
// each rounded to exactly six decimal places.
std::array<double, 3> ratioReport(const std::vector<int>& arr) {
    int pos = 0, neg = 0, zero = 0;
    for (const int& val : arr) {
        if (val > 0) {
            ++pos;
        } else if (val < 0) {
            ++neg;
        } else {
            ++zero;
        }
    }

    const double total = static_cast<double>(arr.size());
    auto rounded = [](double ratio) -> double {
        return std::round(ratio * 1'000'000.0) / 1'000'000.0;
    };

    return {
        rounded(pos / total),
        rounded(neg / total),
        rounded(zero / total)
    };
}
#include <cassert>
#include <vector>
#include <array>
#include <cmath>

// The solution function is assumed to be defined above.
std::array<double, 3> ratioReport(const std::vector<int>& arr);

int main() {
    // Mixed case
    auto r1 = ratioReport({1, -2, 0, 3, -4, 0});
    assert(std::fabs(r1[0] - 0.333333) < 1e-9);
    assert(std::fabs(r1[1] - 0.333333) < 1e-9);
    assert(std::fabs(r1[2] - 0.333333) < 1e-9);

    // All positive
    auto r2 = ratioReport({5, 7, 9});
    assert(std::fabs(r2[0] - 1.0) < 1e-9);
    assert(std::fabs(r2[1] - 0.0) < 1e-9);
    assert(std::fabs(r2[2] - 0.0) < 1e-9);

    // All zeros
    auto r3 = ratioReport({0, 0, 0, 0});
    assert(std::fabs(r3[0] - 0.0) < 1e-9);
    assert(std::fabs(r3[1] - 0.0) < 1e-9);
    assert(std::fabs(r3[2] - 1.0) < 1e-9);

    // Single negative
    auto r4 = ratioReport({-42});
    assert(std::fabs(r4[0] - 0.0) < 1e-9);
    assert(std::fabs(r4[1] - 1.0) < 1e-9);
    assert(std::fabs(r4[2] - 0.0) < 1e-9);

    // Mixed with one of each type, size 3
    auto r5 = ratioReport({1, -1, 0});
    assert(std::fabs(r5[0] - 0.333333) < 1e-9);
    assert(std::fabs(r5[1] - 0.333333) < 1e-9);
    assert(std::fabs(r5[2] - 0.333333) < 1e-9);

    // Two positives, one zero, one negative (size 4)
    auto r6 = ratioReport({2, 4, -8, 0});
    assert(std::fabs(r6[0] - 0.5) < 1e-9);
    assert(std::fabs(r6[1] - 0.25) < 1e-9);
    assert(std::fabs(r6[2] - 0.25) < 1e-9);

    // Large vector with no zeros
    std::vector<int> many;
    for (int i = 1; i <= 100; ++i) many.push_back(i % 2 == 0 ? -i : i);
    auto r7 = ratioReport(many);
    assert(std::fabs(r7[0] - 0.5) < 1e-9);
    assert(std::fabs(r7[1] - 0.5) < 1e-9);
    assert(std::fabs(r7[2] - 0.0) < 1e-9);

    return 0;
}

/*
Write a C++ function that, given a positive integer `n` and a positive double `h`, returns a `std::vector<double>` containing the heights of the `n-1` equally spaced horizontal lines that divide a triangle of base `n` (or equivalently height `h`) into `n` equal-area strips, measured from the apex downward. The i-th element (1-indexed, from apex) must be `h * sqrt(i / n)` for `i = 1` to `n-1`. The function must handle `n=1` (return an empty vector), and produce results accurate to at least 12 decimal places. Do not include any output formatting; just compute and return the vector.
*/

#include <vector>
#include <cmath>

// Compute heights of horizontal cuts that divide a triangle of height h into n equal-area strips.
// Returns a vector of n-1 heights from apex downward, each = h * sqrt(i/n) for i=1..n-1.
std::vector<double> equalAreaCutHeights(int n, double h) {
    std::vector<double> cuts;
    cuts.reserve(n > 0 ? n - 1 : 0);
    if (n <= 1) {
        return cuts; // no cuts for n=0 or n=1.
    }
    const double inv_n = 1.0 / static_cast<double>(n);
    for (int i = 1; i < n; ++i) {
        cuts.push_back(h * std::sqrt(static_cast<double>(i) * inv_n));
    }
    return cuts;
}

#include <cassert>
#include <cmath>
#include <vector>

// Declaration of the function under test (already defined above in solution).
std::vector<double> equalAreaCutHeights(int n, double h);

int main() {
    // n=1 -> no cuts
    std::vector<double> r1 = equalAreaCutHeights(1, 10.0);
    assert(r1.empty());

    // n=2 -> one cut at h * sqrt(1/2)
    std::vector<double> r2 = equalAreaCutHeights(2, 10.0);
    assert(r2.size() == 1);
    assert(std::fabs(r2[0] - 10.0 * std::sqrt(0.5)) < 1e-12);

    // n=3 -> cuts at h*sqrt(1/3) and h*sqrt(2/3)
    std::vector<double> r3 = equalAreaCutHeights(3, 9.0);
    assert(r3.size() == 2);
    assert(std::fabs(r3[0] - 9.0 * std::sqrt(1.0/3.0)) < 1e-12);
    assert(std::fabs(r3[1] - 9.0 * std::sqrt(2.0/3.0)) < 1e-12);

    // n=4, h=1 -> cuts at sqrt(0.25), sqrt(0.5), sqrt(0.75)
    std::vector<double> r4 = equalAreaCutHeights(4, 1.0);
    assert(r4.size() == 3);
    assert(std::fabs(r4[0] - std::sqrt(0.25)) < 1e-12);
    assert(std::fabs(r4[1] - std::sqrt(0.5)) < 1e-12);
    assert(std::fabs(r4[2] - std::sqrt(0.75)) < 1e-12);

    // n=1, h=0 (degenerate but valid)
    assert(equalAreaCutHeights(1, 0.0).empty());

    // n=5, check last cut is h*sqrt(4/5)
    std::vector<double> r5 = equalAreaCutHeights(5, 100.0);
    assert(r5.size() == 4);
    assert(std::fabs(r5.back() - 100.0 * std::sqrt(4.0/5.0)) < 1e-12);

    return 0;
}

// The problem is derived from the geometric fact that to divide a triangle into equal-area horizontal strips, the cut heights from the apex follow a square-root scaling: if the total height is `h`, the distance from the apex to the i-th cut (where `i` runs from 1 to `n-1`, with `n` strips) is `h * sqrt(i / n)`. This is because area scales as the square of the linear dimension. So we iterate `i` from 1 to `n-1`, compute `h * sqrt(static_cast<double>(i) / n)`, and push the result into a vector. Edge cases: `n` must be at least 1; if `n==1`, no cuts exist, return empty vector. If `n` is large (e.g., up to 1e6), the loop is fine. Complexity: O(n) time, O(n) space for the result. Precision: using `double` and `sqrt` is sufficient for 12 decimals for typical inputs, but ensure we cast integer `i` and `n` to double before division to avoid integer truncation. No special handling for `h=0` is required; it would yield all zeros.

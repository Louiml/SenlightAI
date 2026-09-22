Write a C++ function `fillGaps` that takes a vector of key-value pairs where keys are unique integers that may have gaps (missing consecutive integers) and values are doubles representing numeric data associated with those keys. The function should produce a new vector that contains records for every integer key from the minimum key to the maximum key, filling in missing values using one of three interpolation methods: nearest neighbor (method 0), linear interpolation (method 1), or cubic spline interpolation (method 2). The input is sorted by key ascending; missing keys are filled in between existing ones. For nearest neighbor, each missing value takes the value of the closest existing key (choose the lower key on ties). For linear interpolation, values are computed along the straight line between bracketing known values. For spline interpolation, use a natural cubic spline through all known points. The output must preserve the original known values exactly. Return the filled vector as pairs of (key, value). If the input has fewer than 2 records, return a copy of the input.
The solution processes the input sorted by key. First, identify the minimum and maximum keys. Create an output vector of size (maxKey - minKey + 1). For each position, if the key exists in the input, copy its value; otherwise, compute the missing value. For nearest neighbor (method 0): find the closest known key by searching left and right from the missing key; if distances are equal, use the left neighbor. This can be done by scanning the input vector and for each gap between consecutive known records, assign half the gap to the left value and half to the right value. For linear (method 1): for each gap between consecutive known records (keyA, valA) and (keyB, valB), fill positions keyA+1 ... keyB-1 with `valA + (i - keyA) * (valB - valA) / (keyB - keyA)`. For spline (method 2): build a natural cubic spline interpolant using all known points (keys as x, values as y). Then evaluate the spline at each missing key. Edge cases: gaps at the very beginning or end cannot occur because we start at min and end at max, and those are known keys. If the input has fewer than 2 points, spline is undefined; fall back to returning the input as-is (or linear/nearest as appropriate). Time complexity is O(N + G) where N is the number of known records and G is the total number of gaps, which equals the size of the output. For the spline, building it takes O(N) and evaluating each missing point O(1) after precomputation, so overall O(N + G). Space complexity is O(N + G) for the output and the spline's internal arrays.
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <cassert>

// A simple natural cubic spline implementation.
class CubicSpline {
public:
    void build(const std::vector<double>& x, const std::vector<double>& y) {
        n = x.size();
        this->x = x;
        this->y = y;
        a = y;
        b.resize(n);
        c.resize(n);
        d.resize(n);
        if (n < 2) return;
        std::vector<double> h(n - 1);
        for (size_t i = 0; i < n - 1; ++i) h[i] = x[i+1] - x[i];
        std::vector<double> alpha(n - 1);
        for (size_t i = 1; i < n - 1; ++i) {
            alpha[i] = 3.0 * (a[i+1] * h[i-1] - a[i] * (x[i+1] - x[i-1]) + a[i-1] * h[i]) / (h[i-1] * h[i]);
            // Correct formula: 3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1]
        }
        // Actually compute properly:
        for (size_t i = 1; i < n - 1; ++i) {
            alpha[i] = 3.0 * (y[i+1] - y[i]) / h[i] - 3.0 * (y[i] - y[i-1]) / h[i-1];
        }
        // Solve tridiagonal system
        std::vector<double> l(n), mu(n), z(n);
        l[0] = 1.0;
        mu[0] = 0.0;
        z[0] = 0.0;
        for (size_t i = 1; i < n - 1; ++i) {
            l[i] = 2.0 * (x[i+1] - x[i-1]) - h[i-1] * mu[i-1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i-1] * z[i-1]) / l[i];
        }
        l[n-1] = 1.0;
        z[n-1] = 0.0;
        c[n-1] = 0.0;
        for (int j = static_cast<int>(n) - 2; j >= 0; --j) {
            size_t i = static_cast<size_t>(j);
            c[i] = z[i] - mu[i] * c[i+1];
            b[i] = (y[i+1] - y[i]) / h[i] - h[i] * (c[i+1] + 2.0 * c[i]) / 3.0;
            d[i] = (c[i+1] - c[i]) / (3.0 * h[i]);
        }
    }

    double evaluate(double xval) const {
        if (n < 2) return n == 1 ? y[0] : 0.0;
        size_t i = 0;
        while (i < n - 1 && xval > x[i+1]) ++i;
        if (i >= n - 1) i = n - 2;
        double dx = xval - x[i];
        return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
    }

private:
    size_t n;
    std::vector<double> x, y, a, b, c, d;
};

// Fill gaps in a sequence of (key, value) pairs.
// The input vector 'records' must be sorted by key (ascending) and keys are unique integers.
// method: 0 = nearest neighbor, 1 = linear, 2 = cubic spline.
std::vector<std::pair<int, double>> fillGaps(
        const std::vector<std::pair<int, double>>& records,
        int method) {
    if (records.size() < 2) {
        return records;
    }

    int minKey = records.front().first;
    int maxKey = records.back().first;
    size_t outputSize = static_cast<size_t>(maxKey - minKey + 1);
    std::vector<std::pair<int, double>> result;
    result.reserve(outputSize);

    if (method == 0) {
        // Nearest neighbor: for each gap between consecutive known records,
        // fill the first half with left value, second half with right value.
        // Handle tie by assigning to left (so ceil((gapSize)/2) from left).
        for (size_t idx = 0; idx + 1 < records.size(); ++idx) {
            int keyA = records[idx].first;
            double valA = records[idx].second;
            int keyB = records[idx+1].first;
            double valB = records[idx+1].second;
            int gapSize = keyB - keyA; // >= 1 since keys are unique and sorted
            int assignToLeft = (gapSize % 2 == 0) ? gapSize / 2 : (gapSize + 1) / 2;
            // Fill from keyA to keyA+assignToLeft-1 with valA, then the rest with valB
            for (int key = keyA; key < keyB; ++key) {
                if (key < keyA + assignToLeft) {
                    result.emplace_back(key, valA);
                } else {
                    result.emplace_back(key, valB);
                }
            }
        }
        // Add the last known record
        result.emplace_back(records.back());
        return result;
    }

    if (method == 1) {
        // Linear interpolation
        for (size_t idx = 0; idx + 1 < records.size(); ++idx) {
            int keyA = records[idx].first;
            double valA = records[idx].second;
            int keyB = records[idx+1].first;
            double valB = records[idx+1].second;
            for (int key = keyA; key < keyB; ++key) {
                double t = static_cast<double>(key - keyA) / static_cast<double>(keyB - keyA);
                result.emplace_back(key, valA + t * (valB - valA));
            }
        }
        result.emplace_back(records.back());
        return result;
    }

    // method == 2: Cubic spline
    std::vector<double> xs, ys;
    xs.reserve(records.size());
    ys.reserve(records.size());
    for (const auto& rec : records) {
        xs.push_back(static_cast<double>(rec.first));
        ys.push_back(rec.second);
    }
    CubicSpline spline;
    spline.build(xs, ys);
    for (int key = minKey; key <= maxKey; ++key) {
        double val = spline.evaluate(static_cast<double>(key));
        result.emplace_back(key, val);
    }
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The solution function is declared here (normally would be in a header)
std::vector<std::pair<int, double>> fillGaps(
        const std::vector<std::pair<int, double>>& records,
        int method);

bool closeEnough(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Test 1: Nearest neighbor, simple gap
    std::vector<std::pair<int, double>> input = {{1, 10.0}, {4, 20.0}};
    auto result = fillGaps(input, 0);
    assert(result.size() == 4);
    assert(result[0].first == 1 && closeEnough(result[0].second, 10.0));
    assert(result[1].first == 2 && closeEnough(result[1].second, 10.0)); // tie -> left
    assert(result[2].first == 3 && closeEnough(result[2].second, 20.0));
    assert(result[3].first == 4 && closeEnough(result[3].second, 20.0));

    // Test 2: Linear interpolation, gap of 3
    input = {{0, 0.0}, {3, 3.0}};
    result = fillGaps(input, 1);
    assert(result.size() == 4);
    assert(closeEnough(result[0].second, 0.0));
    assert(closeEnough(result[1].second, 1.0));
    assert(closeEnough(result[2].second, 2.0));
    assert(closeEnough(result[3].second, 3.0));

    // Test 3: Spline on known points (should reproduce exact values at known keys)
    input = {{0, 0.0}, {1, 1.0}, {2, 4.0}, {3, 9.0}}; // y = x^2
    result = fillGaps(input, 2);
    assert(result.size() == 4);
    assert(closeEnough(result[0].second, 0.0));
    assert(closeEnough(result[1].second, 1.0));
    assert(closeEnough(result[2].second, 4.0));
    assert(closeEnough(result[3].second, 9.0));

    // Test 4: Spline with gaps, check smoothness at midpoints (linear for x^2 behavior)
    input = {{0, 0.0}, {2, 4.0}}; // y = x^2 with gap at x=1
    result = fillGaps(input, 2);
    assert(result.size() == 3);
    assert(closeEnough(result[0].second, 0.0));
    assert(closeEnough(result[1].second, 1.0)); // spline should produce ~1
    assert(closeEnough(result[2].second, 4.0));

    // Test 5: No gaps, output should match input exactly
    input = {{5, 2.5}, {6, 3.5}, {7, 4.5}};
    result = fillGaps(input, 0);
    assert(result.size() == 3);
    for (size_t i = 0; i < input.size(); ++i) {
        assert(result[i].first == input[i].first);
        assert(closeEnough(result[i].second, input[i].second));
    }

    // Test 6: Single record (edge case) -> returns copy
    input = {{10, 1.0}};
    result = fillGaps(input, 1);
    assert(result.size() == 1);
    assert(result[0].first == 10 && closeEnough(result[0].second, 1.0));

    // Test 7: Negative keys and values
    input = {{-2, -4.0}, {0, 0.0}};
    result = fillGaps(input, 1);
    assert(result.size() == 3);
    assert(result[0].first == -2 && closeEnough(result[0].second, -4.0));
    assert(result[1].first == -1 && closeEnough(result[1].second, -2.0));
    assert(result[2].first == 0 && closeEnough(result[2].second, 0.0));

    return 0;
}

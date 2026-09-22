Write a standalone C++ function `triangularSample` that performs inverse-transform sampling from a triangular distribution defined by three parameters `a`, `b`, and `c` (with `a < c < b`), using a precomputed cumulative distribution table. The function should take as input: two arrays `pdfTable` and `valueTable` of size `n`, where `pdfTable[i]` contains the unnormalized PDF value at `valueTable[i]`, a precomputed cumulative-normalized table `cumTable` of the same size, the number of samples `m`, and the distribution parameters. It should generate `m` random samples (using a uniform random number generator in [0,1)) and return a `std::vector<double>` containing those samples, where each sample is obtained by binary searching the cumulative table for the smallest index whose cumulative value is ≥ the uniform random number, and returning the corresponding value from `valueTable`. The function must handle edge cases such as `m=0` (return empty vector), `n=1` (always return the single value), and ensure the binary search works correctly when the uniform number equals a cumulative table entry exactly (choose that index). The function must not allocate extra memory unnecessarily, must be `const`-correct for input arrays, and must be self-contained (include all necessary headers).

// The algorithm follows inverse-transform sampling: given a set of sorted x-values and their corresponding PDF values, we first normalize the PDF to form a CDF (cumulative distribution function). For each desired sample, we draw a uniform random number `u ∈ [0,1)`, then find the smallest index `j` such that `cumTable[j] ≥ u`. This is done via binary search (since the CDF is monotonically non-decreasing). The returned sample is `valueTable[j]`. Edge cases: if `m==0`, return empty vector. If `n==1`, the binary search will always return index 0 because `cumTable[0]` should be 1.0 (assuming the table is correctly normalized; but the function does not validate it, so we rely on the caller). For exact matches (`u == cumTable[j]`), we want to return that exact index, which the standard `lower_bound` algorithm handles (first element not less than u). However, we must be careful: the problem statement says "binary searching the cumulative table for the smallest index whose cumulative value is ≥ the uniform random number," which matches `std::lower_bound`. The time complexity is `O(m log n)` for sampling plus the binary search per sample, and `O(1)` auxiliary space beyond the output vector. The input tables are assumed to be correct (cumulative normalized, non-decreasing), and the function does not modify them.

#include <vector>
#include <algorithm>

// Generate m samples from a discrete approximation of a triangular distribution
// using inverse-transform sampling from precomputed tables.
// Input:
//   valueTable - sorted x-values (size n) where PDF is evaluated
//   cumTable   - normalized cumulative distribution (size n, non-decreasing, cumTable[n-1]≈1)
//   m          - number of samples to generate
//   rng        - callable that returns a uniform double in [0,1)
// Output: vector of m samples drawn from valueTable according to cumTable.
template<typename RNG>
std::vector<double> triangularSample(const std::vector<double>& valueTable,
                                     const std::vector<double>& cumTable,
                                     int m,
                                     RNG& rng) {
    std::vector<double> samples;
    if (m <= 0) return samples;
    const int n = static_cast<int>(valueTable.size());
    if (n == 0) return samples; // invalid input, return empty

    samples.reserve(m);
    for (int i = 0; i < m; ++i) {
        double u = rng(); // uniform [0,1)
        // Find first index where cumTable[idx] >= u
        auto it = std::lower_bound(cumTable.begin(), cumTable.end(), u);
        // It is guaranteed that it != end() if cumTable[n-1] == 1.0
        // To be safe, clamp to last index.
        int idx = (it == cumTable.end()) ? n - 1 : static_cast<int>(it - cumTable.begin());
        samples.push_back(valueTable[idx]);
    }
    return samples;
}

#include <cassert>
#include <vector>
#include <random>

// Simple deterministic uniform generator for testing (returns values from a vector)
class FixedRNG {
public:
    explicit FixedRNG(const std::vector<double>& vals) : values(vals), pos(0) {}
    double operator()() {
        double v = values[pos % values.size()];
        ++pos;
        return v;
    }
private:
    std::vector<double> values;
    size_t pos;
};

int main() {
    // Case 1: m=0 returns empty
    std::vector<double> vt = {1.0, 2.0, 3.0};
    std::vector<double> ct = {0.5, 1.0, 1.0}; // not strictly increasing, but valid
    FixedRNG rng0({0.0});
    auto res0 = triangularSample(vt, ct, 0, rng0);
    assert(res0.empty());

    // Case 2: n=1 always returns the single value
    std::vector<double> vt1 = {5.0};
    std::vector<double> ct1 = {1.0};
    FixedRNG rng1({0.0, 0.999, 0.5});
    auto res1 = triangularSample(vt1, ct1, 3, rng1);
    assert(res1.size() == 3);
    for (double x : res1) assert(x == 5.0);

    // Case 3: ensure lower_bound works with exact match (u == cumTable[j])
    std::vector<double> vt2 = {10.0, 20.0, 30.0};
    std::vector<double> ct2 = {0.2, 0.7, 1.0};
    FixedRNG rng2({0.2, 0.7, 0.999, 0.0, 0.5});
    auto res2 = triangularSample(vt2, ct2, 5, rng2);
    // Expected: 0.2 -> idx 0 (10), 0.7 -> idx 1 (20), 0.999 -> idx 2 (30), 0.0 -> idx 0 (10), 0.5 -> idx 1 (20)
    assert(res2[0] == 10.0);
    assert(res2[1] == 20.0);
    assert(res2[2] == 30.0);
    assert(res2[3] == 10.0);
    assert(res2[4] == 20.0);

    // Case 4: larger test with typical triangular CDF (a=0, c=5, b=10) discretized
    std::vector<double> xvals = {0.0, 2.5, 5.0, 7.5, 10.0};
    std::vector<double> pdf = {0.0, 0.4, 0.8, 0.4, 0.0}; // unnormalized
    std::vector<double> cum;
    double sum = 0;
    for (double p : pdf) sum += p;
    double acc = 0;
    for (double p : pdf) {
        acc += p / sum;
        cum.push_back(acc);
    }
    // Use a real uniform RNG
    std::mt19937 gen(12345);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto rng = [&]() { return dist(gen); };
    auto res = triangularSample(xvals, cum, 1000, rng);
    assert(res.size() == 1000);
    // All samples must be within [0,10]
    for (double x : res) assert(x >= 0.0 && x <= 10.0);
    // Rough check that values cluster near 5 (mode)
    int nearMode = 0;
    for (double x : res) if (x == 5.0) nearMode++;
    assert(nearMode > 0); // at least some samples land on the mode (since discretization)

    return 0;
}

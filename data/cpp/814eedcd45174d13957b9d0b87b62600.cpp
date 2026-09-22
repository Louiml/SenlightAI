Write a C++ function `compute_histogram_partition` that takes as input a vector of unsigned integers (input data), a number of bins, and a partition factor `alpha` (a floating-point value in [0,1]). The function must compute a histogram of the input values and then process only a fraction of the histogram bins on the "host" side (simulating a CPU/GPU split). Specifically, the function should compute a full histogram (counting occurrences of each value in the input modulo the number of bins), then for the first `floor(alpha * n_bins)` bins, it should sum the counts from a "host contribution" that is computed by brute-force scanning the input for those bins only, while the remaining bins get their counts from the full histogram counts. The final output should be a vector of unsigned integers of size `n_bins`, where the first `n_cpu_bins` entries are the host-side computed counts (from scanning the input and counting only values that map to those bins), and the entries from `n_cpu_bins` to the end are taken directly from the precomputed full histogram counts. The function must handle edge cases where `alpha` is 0 (all bins from the precomputed histogram), where `alpha` is 1 (all bins from host-side scanning), and where the input is empty (return a zero histogram). Ensure that the function is self-contained, uses only standard C++ headers, and does not depend on any external libraries or file I/O.
// The solution computes a full histogram of the input data by iterating over all input values and incrementing the bin `value % n_bins`. This full histogram provides the counts for the bins that are not assigned to the host partition. For the host-dedicated bins (the first `n_cpu_bins = floor(alpha * n_bins)` bins), the function must compute counts by scanning the input again and only incrementing the bin if the bin index is less than `n_cpu_bins`; this simulates the host portion of a heterogeneous computation. This approach is straightforward: first pass computes the full histogram; second pass selectively computes the host contribution for the first `n_cpu_bins`. Edge cases: if `alpha` is 0, `n_cpu_bins` is 0, so the host pass does nothing, and the result is just the full histogram; if `alpha` is 1, `n_cpu_bins` equals `n_bins`, so the host pass recomputes the entire histogram and the full histogram is not used for output (though it is still computed, which is acceptable for simplicity). If the input is empty, both passes do nothing and the output vector is all zeros. The time complexity is `O(2 * N + n_bins)` where `N` is the size of the input vector, because the input is scanned twice. Space complexity is `O(n_bins)` for the two histogram vectors (full histogram and host histogram) plus the output vector, which can be constructed in place to reduce memory; for clarity, the solution uses two vectors and then copies the appropriate parts. The algorithm is deterministic and handles duplicate values naturally since each occurrence increments the bin counter.
#include <vector>
#include <cmath>
#include <cstdint>

/**
 * Computes a partitioned histogram.
 *
 * For a given input vector, counts the occurrences of each value modulo n_bins.
 * The first floor(alpha * n_bins) bins are computed by a brute-force host pass
 * (scanning the input only for those bins), while the remaining bins are taken
 * from a precomputed full histogram.
 *
 * @param input      Vector of unsigned integers (input data).
 * @param n_bins     Number of histogram bins (must be > 0).
 * @param alpha      Fraction of bins (in [0.0, 1.0]) assigned to the host pass.
 * @return           A vector of size n_bins with the partitioned histogram counts.
 *
 * @note If alpha == 0, all bins come from the full histogram.
 *       If alpha == 1, all bins are computed by the host pass.
 *       An empty input yields a zero histogram.
 */
std::vector<unsigned int> compute_histogram_partition(
    const std::vector<unsigned int>& input,
    int n_bins,
    double alpha) {
    // Validate inputs; n_bins must be positive, alpha in [0,1].
    if (n_bins <= 0) {
        return {}; // or throw, but returning empty for invalid input
    }
    if (alpha < 0.0) alpha = 0.0;
    if (alpha > 1.0) alpha = 1.0;

    // Number of bins to be computed by the "host" pass (first bins).
    int n_cpu_bins = static_cast<int>(std::floor(alpha * n_bins));
    // Clamp to valid range, though floor already ensures n_cpu_bins <= n_bins.
    if (n_cpu_bins > n_bins) n_cpu_bins = n_bins;
    if (n_cpu_bins < 0) n_cpu_bins = 0;

    // Full histogram: counts for all bins.
    std::vector<unsigned int> full_histo(n_bins, 0);
    for (unsigned int val : input) {
        // Use modulo to map value to bin (avoid overflow by using unsigned arithmetic).
        unsigned int bin = val % static_cast<unsigned int>(n_bins);
        ++full_histo[bin];
    }

    // Host-partition histogram: only for bins 0 .. n_cpu_bins-1.
    std::vector<unsigned int> host_histo(n_bins, 0);
    if (n_cpu_bins > 0) {
        for (unsigned int val : input) {
            unsigned int bin = val % static_cast<unsigned int>(n_bins);
            // Only count if the bin belongs to the host partition.
            if (static_cast<int>(bin) < n_cpu_bins) {
                ++host_histo[bin];
            }
        }
    }

    // Construct final result: host bins from host_histo, rest from full_histo.
    std::vector<unsigned int> result(n_bins, 0);
    for (int i = 0; i < n_bins; ++i) {
        if (i < n_cpu_bins) {
            result[i] = host_histo[i];
        } else {
            result[i] = full_histo[i];
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cmath>

// The function under test (provided in the Solution section).
std::vector<unsigned int> compute_histogram_partition(
    const std::vector<unsigned int>& input,
    int n_bins,
    double alpha);

// Helper to compute a reference histogram (full counts).
std::vector<unsigned int> reference_hist(const std::vector<unsigned int>& input, int n_bins) {
    std::vector<unsigned int> h(n_bins, 0);
    for (unsigned int v : input) {
        h[v % n_bins]++;
    }
    return h;
}

int main() {
    // Test 1: alpha = 0.25, n_bins = 8, simple input.
    std::vector<unsigned int> in1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int bins1 = 8;
    double alpha1 = 0.25; // floor(0.25*8) = 2 bins from host
    auto res1 = compute_histogram_partition(in1, bins1, alpha1);
    auto full1 = reference_hist(in1, bins1);
    // Host bins (0,1) should match full histogram (since each value appears exactly once in those bins).
    assert(res1[0] == full1[0]); // bin 0: values 0,8 -> count 2
    assert(res1[1] == full1[1]); // bin 1: value 1,9 -> count 2
    // Remaining bins (2..7) from full histogram.
    for (int i = 2; i < bins1; ++i) {
        assert(res1[i] == full1[i]);
    }

    // Test 2: alpha = 0.0 (all from full histogram).
    std::vector<unsigned int> in2 = {5, 5, 7, 9, 5, 3};
    int bins2 = 5;
    auto res2 = compute_histogram_partition(in2, bins2, 0.0);
    auto full2 = reference_hist(in2, bins2);
    assert(res2 == full2);

    // Test 3: alpha = 1.0 (all from host pass, same as full).
    auto res3 = compute_histogram_partition(in2, bins2, 1.0);
    assert(res3 == full2);

    // Test 4: empty input produces zero histogram.
    std::vector<unsigned int> in4;
    auto res4 = compute_histogram_partition(in4, 10, 0.5);
    assert(res4.size() == 10);
    for (unsigned int v : res4) assert(v == 0);

    // Test 5: larger input with duplicates, alpha=0.5.
    std::vector<unsigned int> in5 = {2, 2, 3, 3, 3, 4, 4, 4, 4, 5};
    int bins5 = 6;
    double alpha5 = 0.5; // floor(0.5*6)=3 bins from host
    auto res5 = compute_histogram_partition(in5, bins5, alpha5);
    auto full5 = reference_hist(in5, bins5);
    // Host bins 0,1,2: verify counts by manual scan.
    // Values: 2->bin2, 3->bin3, 4->bin4, 5->bin5. So bins 0 and 1 are zero in full.
    // Host pass should also yield zero for 0 and 1, and for bin2 count=2.
    assert(res5[0] == 0);
    assert(res5[1] == 0);
    assert(res5[2] == 2); // two 2's
    // Remaining bins (3,4,5) from full histogram.
    for (int i = 3; i < bins5; ++i) {
        assert(res5[i] == full5[i]);
    }

    // Test 6: alpha value just below 1, e.g., 0.99 with n_bins=10 -> n_cpu_bins=9.
    std::vector<unsigned int> in6 = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; // all map to bin 0
    int bins6 = 10;
    auto res6 = compute_histogram_partition(in6, bins6, 0.99);
    // Host bins 0..8: bin0 should have count 10, bins 1..8 have 0.
    assert(res6[0] == 10);
    for (int i = 1; i < 9; ++i) assert(res6[i] == 0);
    // Bin 9 from full histogram also 0.
    assert(res6[9] == 0);

    // Test 7: n_bins larger than input size, ensure modulo works.
    std::vector<unsigned int> in7 = {100};
    int bins7 = 1000;
    auto res7 = compute_histogram_partition(in7, bins7, 0.001); // floor(1)=0 bins from host
    assert(res7.size() == 1000);
    assert(res7[100] == 1);
    int nonzero = 0;
    for (unsigned int v : res7) if (v != 0) ++nonzero;
    assert(nonzero == 1);

    // Test 8: alpha = 0.5, n_bins odd (e.g., 7) to test floor behavior.
    std::vector<unsigned int> in8 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int bins8 = 7;
    auto res8 = compute_histogram_partition(in8, bins8, 0.5); // floor(3.5)=3 bins
    auto full8 = reference_hist(in8, bins8);
    // Host bins 0,1,2 should be recomputed; compare with full.
    for (int i = 0; i < 3; ++i) assert(res8[i] == full8[i]);
    for (int i = 3; i < bins8; ++i) assert(res8[i] == full8[i]);

    return 0;
}

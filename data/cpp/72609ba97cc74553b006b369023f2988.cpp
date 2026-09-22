// Write a standalone C++ function named `spike_transfer_function` that takes two sorted vectors of spike times (`ch1_times` and `ch2_times`), a step size `tau_delta`, a maximum delay `tau_max`, and a tolerance `epsilon_tau`. The function must return a `std::vector<double>` where element at index `k` corresponds to the count of spike pairs `(spike_from_ch2, spike_from_ch1)` such that the condition  
// `ch1_time >= (ch2_time - epsilon_tau + k * tau_delta)` AND `ch1_time < (ch2_time + epsilon_tau + k * tau_delta)` holds. The number of elements is `floor(tau_max / tau_delta) + 1`. For example, if `tau_max = 1.0` and `tau_delta = 0.5`, there are `floor(1.0/0.5)+1 = 3` bins: `d = 0.0, 0.5, 1.0`. All inputs are non‑negative, `tau_delta > 0`, `tau_max >= 0`, and `epsilon_tau >= 0`. The input vectors are not necessarily sorted, but you may assume they contain valid spike times (non‑negative). The function must be self‑contained (include all necessary headers) and use `double` for all time values. Do not use any global variables.
// The algorithm directly mimics the provided MATLAB/MEX loop: for each delay bin `d` (from 0 to `tau_max` in steps of `tau_delta`), count every pair `(ch1_time, ch2_time)` that satisfies the inequality. This requires a triple nested loop: outer over `d` (number of bins `B = floor(tau_max/tau_delta)+1`), middle over each `ch1_time`, inner over each `ch2_time`. For each pair, check if `ch1_time` lies in the half‑open interval `[ch2_time - epsilon_tau + d, ch2_time + epsilon_tau + d)`. The count `M` is accumulated per bin and stored in the result vector. Edge cases: if either input vector is empty, the result is all zeros (loop naturally does nothing). If `tau_max` is not an exact multiple of `tau_delta`, the loop must still run up to `tau_max` as long as `d <= tau_max` (floating‑point comparison may need care, but since all values are non‑negative and `tau_delta` > 0, a simple `d <= tau_max` works with tolerance; to avoid infinite loops, we use `d <= tau_max` and increment by `tau_delta`, but floating‑point error might cause an extra iteration – we can compute the exact number of bins as `static_cast<size_t>(std::floor(tau_max / tau_delta)) + 1` and loop accordingly). Time complexity is \(O(B \cdot N_1 \cdot N_2)\), where \(N_1\) and \(N_2\) are lengths of the input vectors. Space complexity is \(O(B)\) for the result. The solution avoids modifying inputs, uses `const` references, and is safe for empty inputs.
#include <vector>
#include <cmath>
#include <cstddef>

// Count spike pairs within tolerance for each delay bin.
// Parameters:
//   ch1_times : vector of spike times from channel 1 (non-negative)
//   ch2_times : vector of spike times from channel 2 (non-negative)
//   tau_delta : step size between delay bins (>0)
//   tau_max   : maximum delay (>=0)
//   epsilon_tau : tolerance window half-width (>=0)
// Returns a vector where element i is the count for delay d = i*tau_delta.
std::vector<double> spike_transfer_function(
    const std::vector<double>& ch1_times,
    const std::vector<double>& ch2_times,
    double tau_delta,
    double tau_max,
    double epsilon_tau) {

    // Number of bins: floor(tau_max / tau_delta) + 1
    std::size_t num_bins = static_cast<std::size_t>(std::floor(tau_max / tau_delta)) + 1;
    std::vector<double> M_of_tau(num_bins, 0.0);

    const std::size_t len_ch1 = ch1_times.size();
    const std::size_t len_ch2 = ch2_times.size();

    if (len_ch1 == 0 || len_ch2 == 0) {
        return M_of_tau; // all zeros
    }

    for (std::size_t tau_i = 0; tau_i < num_bins; ++tau_i) {
        double d = tau_i * tau_delta;
        double M = 0.0;
        for (std::size_t spi = 0; spi < len_ch1; ++spi) {
            double ch1_time = ch1_times[spi];
            for (std::size_t spj = 0; spj < len_ch2; ++spj) {
                double ch2_time = ch2_times[spj];
                double lower = ch2_time - epsilon_tau + d;
                double upper = ch2_time + epsilon_tau + d;
                if (ch1_time >= lower && ch1_time < upper) {
                    M += 1.0;
                }
            }
        }
        M_of_tau[tau_i] = M;
    }

    return M_of_tau;
}
#include <cassert>
#include <vector>
#include <cmath>

// The solution function must be declared above or included here.
// For testing, we replicate the function here (or include it).
std::vector<double> spike_transfer_function(
    const std::vector<double>& ch1_times,
    const std::vector<double>& ch2_times,
    double tau_delta,
    double tau_max,
    double epsilon_tau);

int main() {
    // Simple case: one pair exactly aligned at d=0
    {
        std::vector<double> ch1 = {1.0};
        std::vector<double> ch2 = {1.0};
        auto res = spike_transfer_function(ch1, ch2, 0.5, 1.0, 0.1);
        assert(res.size() == 3);
        // d=0: ch1=1.0 in [0.9, 1.1) -> yes
        // d=0.5: interval [1.4, 1.6) -> no
        // d=1.0: interval [1.9, 2.1) -> no
        assert(res[0] == 1.0);
        assert(res[1] == 0.0);
        assert(res[2] == 0.0);
    }

    // Pair with positive delay: ch2 earlier, ch1 later
    {
        std::vector<double> ch1 = {2.0};
        std::vector<double> ch2 = {1.0};
        double tau_delta = 1.0, tau_max = 2.0, eps = 0.1;
        auto res = spike_transfer_function(ch1, ch2, tau_delta, tau_max, eps);
        assert(res.size() == 3); // d=0,1,2
        // d=0: interval [0.9, 1.1) -> no
        // d=1: interval [1.9, 2.1) -> yes
        // d=2: interval [2.9, 3.1) -> no
        assert(res[0] == 0.0);
        assert(res[1] == 1.0);
        assert(res[2] == 0.0);
    }

    // Empty input vectors
    {
        std::vector<double> ch1, ch2 = {1.0};
        auto res = spike_transfer_function(ch1, ch2, 0.5, 1.0, 0.0);
        assert(res.size() == 3);
        assert(res[0] == 0.0 && res[1] == 0.0 && res[2] == 0.0);
    }

    // Multiple pairs and multiple bins
    {
        std::vector<double> ch1 = {0.0, 2.0};
        std::vector<double> ch2 = {0.0, 1.0};
        double tau_delta = 0.5, tau_max = 1.0, eps = 0.5;
        auto res = spike_transfer_function(ch1, ch2, tau_delta, tau_max, eps);
        // bins: d=0,0.5,1.0
        // For d=0: intervals around each ch2: [ -0.5,0.5 ) and [0.5,1.5)
        // ch1[0]=0.0 in first, ch1[1]=2.0 in none -> total 1
        // For d=0.5: intervals [0.0,1.0) and [1.0,2.0)
        // ch1[0]=0.0 in first, ch1[1]=2.0? second is [1.0,2.0) -> no, so total 1
        // For d=1.0: intervals [0.5,1.5) and [1.5,2.5)
        // ch1[0]=0.0 no, ch1[1]=2.0 in second? [1.5,2.5) yes -> total 1
        assert(res[0] == 1.0);
        assert(res[1] == 1.0);
        assert(res[2] == 1.0);
    }

    // Test that non-exact tau_max gives correct bin count
    {
        std::vector<double> ch1 = {0.0};
        std::vector<double> ch2 = {0.0};
        double tau_delta = 0.3, tau_max = 1.0, eps = 0.0;
        auto res = spike_transfer_function(ch1, ch2, tau_delta, tau_max, eps);
        // bins: d=0.0,0.3,0.6,0.9 (floor(1/0.3)=3, +1=4)
        assert(res.size() == 4);
        assert(res[0] == 1.0); // d=0 matches exactly
        assert(res[1] == 0.0);
        assert(res[2] == 0.0);
        assert(res[3] == 0.0);
    }

    // Large tolerance covers everything in one bin
    {
        std::vector<double> ch1 = {1.0, 2.0};
        std::vector<double> ch2 = {1.0, 2.0};
        double tau_delta = 1.0, tau_max = 1.0, eps = 10.0;
        auto res = spike_transfer_function(ch1, ch2, tau_delta, tau_max, eps);
        // For d=0: every pair qualifies -> 4 pairs
        // For d=1: also all qualify -> 4 pairs
        assert(res[0] == 4.0);
        assert(res[1] == 4.0);
    }

    return 0;
}

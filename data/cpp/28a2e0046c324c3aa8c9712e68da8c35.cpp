// Implement a C++ function named `simulate_synaptic_plasticity` that models a simplified version of the synaptic consolidation process from the provided code snippet. The function takes the following parameters: `N1` (number of presynaptic neurons), `N2` (number of postsynaptic neurons), `C` (average indegree per postsynaptic neuron), `T` (number of training patterns), `alpha1` and `alpha2` (probabilities of high firing rate for each population), `Wb` (baseline weight), `Ws` (consolidated weight), and a `seed` for reproducible random number generation. The function should simulate the following: (1) generate `N2` postsynaptic neurons each with connections to `C` randomly chosen presynaptic neurons (allow multapses, use fixed indegree), initializing each connection weight to `Wb`; (2) for each of `T` training patterns, generate binary firing patterns for both populations where each neuron independently fires at a "high" rate with probability `alpha` (represented as boolean true) and "low" rate otherwise; (3) for each postsynaptic neuron that is firing high, for each of its presynaptic connections, if the presynaptic neuron is also firing high, set that connection's weight to `Ws` (consolidate); (4) after all training, return a `std::vector<double>` of length `N2` containing for each postsynaptic neuron the total input signal `S_i = sum( weight_j * rate_j )` computed using a test pattern identical to the first training pattern. The function must use a deterministic pseudo-random generator (e.g., `std::mt19937` seeded with `seed`) for reproducibility. Assume `N1`, `N2`, `C`, `T` are positive integers, `0 < alpha1, alpha2 < 1`, `Wb < Ws`, and all inputs are valid. The output should be the vector of total input signals for each postsynaptic neuron, in the same order as neurons are indexed.

#include <cassert>
#include <cmath>
#include <vector>

// The solution function is assumed to be defined above.
// Below is a main function with assertions.

int main() {
    // Test 1: Small deterministic case with known outcome.
    // N1=2, N2=2, C=1, T=1, alpha1=1.0, alpha2=1.0 (always high).
    // Connections: each postsynaptic neuron gets one presynaptic neuron.
    // Since both always fire high, all weights become Ws.
    // Input signal for each = Ws * 1.0 = Ws.
    {
        int N1 = 2, N2 = 2, C = 1, T = 1;
        double alpha1 = 1.0, alpha2 = 1.0;
        double Wb = 0.1, Ws = 1.0;
        unsigned int seed = 42;
        auto res = simulate_synaptic_plasticity(N1, N2, C, T,
                                                alpha1, alpha2, Wb, Ws, seed);
        assert(res.size() == 2);
        for (double val : res) {
            assert(std::fabs(val - Ws) < 1e-9);
        }
    }

    // Test 2: No consolidation if alpha1 or alpha2 is zero.
    // With alpha1=0, no presynaptic neuron fires high, so weights stay Wb.
    // Input signal = 0 (since all rates are 0).
    {
        int N1 = 3, N2 = 2, C = 2, T = 5;
        double alpha1 = 0.0, alpha2 = 0.5;
        double Wb = 0.1, Ws = 1.0;
        unsigned int seed = 7;
        auto res = simulate_synaptic_plasticity(N1, N2, C, T,
                                                alpha1, alpha2, Wb, Ws, seed);
        assert(res.size() == 2);
        for (double val : res) {
            assert(std::fabs(val) < 1e-9);
        }
    }

    // Test 3: Single postsynaptic neuron, single connection, reproducible.
    // N1=1, N2=1, C=1, T=2, alpha1=0.5, alpha2=1.0.
    // Post always fires high, pre fires high with prob 0.5.
    // The first pattern determines whether consolidation occurs.
    // Since deterministic seed, the result is fixed.
    {
        int N1 = 1, N2 = 1, C = 1, T = 2;
        double alpha1 = 0.5, alpha2 = 1.0;
        double Wb = 0.1, Ws = 1.0;
        unsigned int seed = 123;
        auto res1 = simulate_synaptic_plasticity(N1, N2, C, T,
                                                 alpha1, alpha2, Wb, Ws, seed);
        auto res2 = simulate_synaptic_plasticity(N1, N2, C, T,
                                                 alpha1, alpha2, Wb, Ws, seed);
        assert(res1.size() == 1);
        assert(res1[0] == res2[0]); // reproducibility
        // Weight is either Ws (if first pre high) or Wb (if first pre low)
        // and rate is 1 if high, 0 if low.
        // So result is either Ws or 0.
        assert(std::fabs(res1[0] - Ws) < 1e-9 || std::fabs(res1[0]) < 1e-9);
    }

    // Test 4: Larger test with multiple neurons and training steps.
    // Ensure that the function runs without error and returns correct size.
    {
        int N1 = 20, N2 = 10, C = 5, T = 50;
        double alpha1 = 0.2, alpha2 = 0.3;
        double Wb = 0.5, Ws = 2.0;
        unsigned int seed = 999;
        auto res = simulate_synaptic_plasticity(N1, N2, C, T,
                                                alpha1, alpha2, Wb, Ws, seed);
        assert(res.size() == static_cast<size_t>(N2));
        // Each value should be between 0 and C * Ws (max sum)
        for (double val : res) {
            assert(val >= 0.0);
            assert(val <= C * Ws + 1e-9);
        }
    }

    // Test 5: If T=0, no consolidation, and test pattern is not defined.
    // But our function always uses first training pattern; with T=0, we
    // cannot save it, so we skip T=0. Instead test with T=1 and alpha1=alpha2=0
    // to ensure all weights remain Wb and rates are all 0, so result is 0.
    {
        int N1 = 5, N2 = 3, C = 2, T = 1;
        double alpha1 = 0.0, alpha2 = 0.0;
        double Wb = 0.1, Ws = 1.0;
        unsigned int seed = 0;
        auto res = simulate_synaptic_plasticity(N1, N2, C, T,
                                                alpha1, alpha2, Wb, Ws, seed);
        assert(res.size() == 3);
        for (double val : res) {
            assert(std::fabs(val) < 1e-9);
        }
    }

    return 0;
}

#include <vector>
#include <random>
#include <cstddef>

// Simulate synaptic consolidation and compute input signals for a test pattern.
// The test pattern is identical to the first training pattern.
// Returns a vector of total input signal for each postsynaptic neuron.
std::vector<double> simulate_synaptic_plasticity(
    int N1, int N2, int C, int T,
    double alpha1, double alpha2,
    double Wb, double Ws,
    unsigned int seed) {

    // Random number generator with deterministic seed
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    // Create random connections: each postsynaptic neuron has C presynaptic connections
    std::vector<std::vector<int>> conn(N2, std::vector<int>(C));
    std::vector<std::vector<double>> w(N2, std::vector<double>(C, Wb));

    // Uniform integer distribution for presynaptic neuron indices
    std::uniform_int_distribution<int> dist_int(0, N1 - 1);
    for (int i2 = 0; i2 < N2; ++i2) {
        for (int ic = 0; ic < C; ++ic) {
            conn[i2][ic] = dist_int(rng);
        }
    }

    // Store first training pattern for later evaluation
    std::vector<bool> first_rate1(N1);
    std::vector<bool> first_rate2(N2);

    // Training loop
    for (int t = 0; t < T; ++t) {
        // Generate firing patterns for layer 1
        std::vector<bool> rate1(N1);
        for (int i1 = 0; i1 < N1; ++i1) {
            rate1[i1] = (uniform(rng) < alpha1);
        }
        // Generate firing patterns for layer 2
        std::vector<bool> rate2(N2);
        for (int i2 = 0; i2 < N2; ++i2) {
            rate2[i2] = (uniform(rng) < alpha2);
        }

        // Save first pattern for evaluation
        if (t == 0) {
            first_rate1 = rate1;
            first_rate2 = rate2;
        }

        // Synaptic consolidation: if both pre and post are high, set weight to Ws
        for (int i2 = 0; i2 < N2; ++i2) {
            if (rate2[i2]) {
                for (int ic = 0; ic < C; ++ic) {
                    int i1 = conn[i2][ic];
                    if (rate1[i1]) {
                        w[i2][ic] = Ws;
                    }
                }
            }
        }
    }

    // Compute total input signal for each postsynaptic neuron using first pattern
    std::vector<double> result(N2);
    for (int i2 = 0; i2 < N2; ++i2) {
        double sum = 0.0;
        for (int ic = 0; ic < C; ++ic) {
            int i1 = conn[i2][ic];
            double rate = first_rate1[i1] ? 1.0 : 0.0;
            sum += w[i2][ic] * rate;
        }
        result[i2] = sum;
    }

    return result;
}

// The solution involves three main steps: network creation, training, and evaluation. First, create a directed random graph with `N2` nodes (postsynaptic neurons), each having exactly `C` outgoing connections to randomly chosen presynaptic neurons (indices in `[0, N1-1]`), allowing duplicates (multapses). Store connections as a 2D vector `conn` of size `N2 x C` and weights as a 2D vector `w` of same size, initialized to `Wb`. For training, for each of `T` patterns, generate two boolean vectors `rate1` (length `N1`) and `rate2` (length `N2`) where each element is independently true with probability `alpha1` or `alpha2`, respectively. For each postsynaptic neuron `i2` with `rate2[i2] == true`, iterate over its `C` connections; if `rate1[conn[i2][ic]] == true`, set `w[i2][ic] = Ws`. After all training, evaluate using the first training pattern (store it during training). For each postsynaptic neuron `i2`, compute `S = sum_{ic=0}^{C-1} w[i2][ic] * (rate1[conn[i2][ic]] ? 1.0 : 0.0)` (since rates are binary, use 1.0 for high, 0.0 for low). Return the vector of these sums. Edge cases: `C` could be large, but we allocate fixed-size vectors. The random generator must be used consistently; use a single `std::mt19937` seeded with `seed` and a `std::uniform_real_distribution<double>` or `std::bernoulli_distribution` for generating booleans. Time complexity: `O(N2 * C * T)` for training plus `O(N2 * C)` for evaluation. Space complexity: `O(N2 * C)` for connections and weights, plus `O(N1 + N2)` for rate vectors.

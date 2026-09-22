Write a standalone C++ function named `estimate_factor_marginals` that computes approximate marginal probabilities for each factor in a factor graph using Gibbs sampling. The function takes a factor graph representation (a vector of factor tables, where each factor table is a vector of doubles representing unnormalized potentials over all configurations of that factor's variables), a variable-state initialization (a vector of unsigned integers giving the current state of each variable), a burn-in sweep count, a spacing sweep count, a sample count (all unsigned integers), and a random number generator seed (unsigned integer). The function must return a vector of vectors of doubles, where the outer index corresponds to factor ID and the inner vector holds the estimated marginal probabilities for each configuration of that factor (normalized so each factor's marginals sum to 1). The Gibbs sampling must be implemented from scratch: at each sweep, systematically update each variable in order by sampling a new value for that variable from its conditional distribution given the current states of all other variables, computed from the factor potentials. Use the provided initial variable states as the starting point (no separate burn-in annealing is required), perform the specified number of burn-in sweeps, then for each of `sample_count` samples, perform `spacing_sweeps` sweeps (or 0 if spacing is 0, meaning one sweep per sample) and record the resulting variable state to accumulate counts for each factor configuration. Handle the edge case where a factor has zero total probability mass across all configurations in the samples by assigning a uniform distribution for that factor. Assume all factor tables are non-empty, all factor configurations are valid, and the variable-state vector is consistent with the factor dimensions.
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Single binary variable, single factor with potential [1, 3]
    // Expected marginals close to [0.25, 0.75] (exact for large samples)
    {
        std::vector<std::vector<double>> factor_tables = {{1.0, 3.0}};
        std::vector<std::vector<unsigned int>> factor_variables = {{0}};
        std::vector<unsigned int> card = {2};
        std::vector<unsigned int> init = {0};
        auto m = estimate_factor_marginals(factor_tables, factor_variables, card,
                                          init, 100, 0, 100000, 42);
        assert(m.size() == 1);
        assert(m[0].size() == 2);
        assert(std::fabs(m[0][0] - 0.25) < 0.01);
        assert(std::fabs(m[0][1] - 0.75) < 0.01);
    }

    // Test 2: Two independent binary variables, factor over first [1,1], factor over second [2,1]
    // First factor marginals [0.5,0.5], second [2/3,1/3]
    {
        std::vector<std::vector<double>> factor_tables = {{1.0, 1.0}, {2.0, 1.0}};
        std::vector<std::vector<unsigned int>> factor_variables = {{0}, {1}};
        std::vector<unsigned int> card = {2, 2};
        std::vector<unsigned int> init = {0, 1};
        auto m = estimate_factor_marginals(factor_tables, factor_variables, card,
                                          init, 100, 0, 100000, 7);
        assert(m.size() == 2);
        assert(std::fabs(m[0][0] - 0.5) < 0.01);
        assert(std::fabs(m[0][1] - 0.5) < 0.01);
        assert(std::fabs(m[1][0] - 2.0/3.0) < 0.01);
        assert(std::fabs(m[1][1] - 1.0/3.0) < 0.01);
    }

    // Test 3: Factor with zero total probability in samples (zero table)
    // Expect uniform marginals
    {
        std::vector<std::vector<double>> factor_tables = {{0.0, 0.0, 0.0, 0.0}}; // factor over 2 binary vars
        std::vector<std::vector<unsigned int>> factor_variables = {{0, 1}};
        std::vector<unsigned int> card = {2, 2};
        std::vector<unsigned int> init = {0, 0};
        auto m = estimate_factor_marginals(factor_tables, factor_variables, card,
                                          init, 10, 0, 100, 123);
        assert(m.size() == 1);
        assert(m[0].size() == 4);
        for (double val : m[0]) {
            assert(std::fabs(val - 0.25) < 1e-9);
        }
    }

    // Test 4: Factor over two variables with coupling, check that marginals sum to 1 and are valid
    {
        std::vector<std::vector<double>> factor_tables = {{10.0, 1.0, 1.0, 10.0}};
        std::vector<std::vector<unsigned int>> factor_variables = {{0, 1}};
        std::vector<unsigned int> card = {2, 2};
        std::vector<unsigned int> init = {0, 1};
        auto m = estimate_factor_marginals(factor_tables, factor_variables, card,
                                          init, 200, 2, 5000, 99);
        assert(m.size() == 1);
        double sum = 0.0;
        for (double val : m[0]) {
            assert(val >= 0.0 && val <= 1.0);
            sum += val;
        }
        assert(std::fabs(sum - 1.0) < 1e-9);
    }

    // Test 5: Single variable with three states, potential [1,2,1], check approximate [0.25,0.5,0.25]
    {
        std::vector<std::vector<double>> factor_tables = {{1.0, 2.0, 1.0}};
        std::vector<std::vector<unsigned int>> factor_variables = {{0}};
        std::vector<unsigned int> card = {3};
        std::vector<unsigned int> init = {2};
        auto m = estimate_factor_marginals(factor_tables, factor_variables, card,
                                          init, 50, 0, 50000, 5);
        assert(m[0].size() == 3);
        assert(std::fabs(m[0][0] - 0.25) < 0.01);
        assert(std::fabs(m[0][1] - 0.5) < 0.01);
        assert(std::fabs(m[0][2] - 0.25) < 0.01);
    }

    return 0;
}
#include <vector>
#include <cstdint>
#include <random>
#include <numeric>
#include <cassert>
#include <algorithm>

// Compute approximate factor marginals via Gibbs sampling.
std::vector<std::vector<double>> estimate_factor_marginals(
    const std::vector<std::vector<double>>& factor_tables,
    const std::vector<std::vector<unsigned int>>& factor_variables,
    const std::vector<unsigned int>& variable_cardinalities,
    std::vector<unsigned int> initial_state,
    unsigned int burnin_sweeps,
    unsigned int spacing_sweeps,
    unsigned int sample_count,
    unsigned int seed)
{
    const unsigned int num_variables = variable_cardinalities.size();
    const unsigned int num_factors = factor_tables.size();

    // Validate inputs
    assert(num_variables > 0);
    assert(num_factors > 0);
    assert(initial_state.size() == num_variables);
    for (unsigned int v = 0; v < num_variables; ++v) {
        assert(initial_state[v] < variable_cardinalities[v]);
    }
    for (unsigned int f = 0; f < num_factors; ++f) {
        assert(factor_variables[f].size() > 0);
        unsigned int configs = 1;
        for (unsigned int var_idx : factor_variables[f]) {
            configs *= variable_cardinalities[var_idx];
        }
        assert(factor_tables[f].size() == configs);
    }

    // Random number generator
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    // Current state
    std::vector<unsigned int> state = initial_state;

    // Helper to compute the absolute index of a factor configuration given a state
    auto compute_abs_index = [&](unsigned int factor_id, const std::vector<unsigned int>& st) -> unsigned int {
        const auto& vars = factor_variables[factor_id];
        unsigned int index = 0;
        unsigned int stride = 1;
        for (auto it = vars.rbegin(); it != vars.rend(); ++it) {
            index += st[*it] * stride;
            stride *= variable_cardinalities[*it];
        }
        return index;
    };

    // Burn-in sweeps
    for (unsigned int sweep = 0; sweep < burnin_sweeps; ++sweep) {
        for (unsigned int v = 0; v < num_variables; ++v) {
            // Compute conditional weights for variable v
            std::vector<double> weights(variable_cardinalities[v], 1.0);
            for (unsigned int f = 0; f < num_factors; ++f) {
                const auto& vars = factor_variables[f];
                // Check if factor involves v
                auto it = std::find(vars.begin(), vars.end(), v);
                if (it == vars.end()) continue;

                // For each possible value of v, compute factor contribution
                for (unsigned int val = 0; val < variable_cardinalities[v]; ++val) {
                    // Build a temporary state copy with v set to val
                    std::vector<unsigned int> temp_state = state;
                    temp_state[v] = val;
                    // Compute index of factor f under temp_state
                    unsigned int index = 0;
                    unsigned int stride = 1;
                    for (auto rit = vars.rbegin(); rit != vars.rend(); ++rit) {
                        index += temp_state[*rit] * stride;
                        stride *= variable_cardinalities[*rit];
                    }
                    weights[val] *= factor_tables[f][index];
                }
            }

            // Normalize weights (guard against all zeros, but assume positive)
            double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
            if (sum <= 0.0) {
                // Fallback: uniform (should not happen for valid input)
                std::fill(weights.begin(), weights.end(), 1.0 / weights.size());
            } else {
                for (double& w : weights) w /= sum;
            }

            // Sample new value for v
            double r = uniform(rng);
            double cum = 0.0;
            unsigned int new_val = 0;
            for (unsigned int val = 0; val < variable_cardinalities[v]; ++val) {
                cum += weights[val];
                if (r <= cum) {
                    new_val = val;
                    break;
                }
            }
            // In case r is exactly 1.0 (should not happen with uniform distribution)
            if (r >= 1.0) new_val = variable_cardinalities[v] - 1;
            state[v] = new_val;
        }
    }

    // Initialize marginals counts
    std::vector<std::vector<double>> marginals(num_factors);
    for (unsigned int f = 0; f < num_factors; ++f) {
        marginals[f].assign(factor_tables[f].size(), 0.0);
    }

    // Sampling phase
    for (unsigned int s = 0; s < sample_count; ++s) {
        // Perform spacing sweeps (or at least one if spacing_sweeps == 0)
        unsigned int sweeps_to_do = (spacing_sweeps == 0) ? 1 : spacing_sweeps;
        for (unsigned int sw = 0; sw < sweeps_to_do; ++sw) {
            for (unsigned int v = 0; v < num_variables; ++v) {
                // Same conditional update as above (could factor into lambda, but keep simple)
                std::vector<double> weights(variable_cardinalities[v], 1.0);
                for (unsigned int f = 0; f < num_factors; ++f) {
                    const auto& vars = factor_variables[f];
                    auto it = std::find(vars.begin(), vars.end(), v);
                    if (it == vars.end()) continue;

                    for (unsigned int val = 0; val < variable_cardinalities[v]; ++val) {
                        std::vector<unsigned int> temp_state = state;
                        temp_state[v] = val;
                        unsigned int index = 0;
                        unsigned int stride = 1;
                        for (auto rit = vars.rbegin(); rit != vars.rend(); ++rit) {
                            index += temp_state[*rit] * stride;
                            stride *= variable_cardinalities[*rit];
                        }
                        weights[val] *= factor_tables[f][index];
                    }
                }
                double sum = std::accumulate(weights.begin(), weights.end(), 0.0);
                if (sum <= 0.0) {
                    std::fill(weights.begin(), weights.end(), 1.0 / weights.size());
                } else {
                    for (double& w : weights) w /= sum;
                }
                double r = uniform(rng);
                double cum = 0.0;
                unsigned int new_val = 0;
                for (unsigned int val = 0; val < variable_cardinalities[v]; ++val) {
                    cum += weights[val];
                    if (r <= cum) {
                        new_val = val;
                        break;
                    }
                }
                if (r >= 1.0) new_val = variable_cardinalities[v] - 1;
                state[v] = new_val;
            }
        }

        // Record sample into marginals
        for (unsigned int f = 0; f < num_factors; ++f) {
            unsigned int idx = compute_abs_index(f, state);
            marginals[f][idx] += 1.0;
        }
    }

    // Normalize each factor's marginals
    for (unsigned int f = 0; f < num_factors; ++f) {
        double total = std::accumulate(marginals[f].begin(), marginals[f].end(), 0.0);
        if (total > 0.0) {
            for (double& val : marginals[f]) val /= total;
        } else {
            // Zero counts: uniform distribution
            double uniform_val = 1.0 / marginals[f].size();
            std::fill(marginals[f].begin(), marginals[f].end(), uniform_val);
        }
    }

    return marginals;
}
// The solution implements a basic Gibbs sampler over a factor graph. The core idea is to iteratively update each variable by sampling from its conditional distribution given all others. For each variable, the conditional distribution is proportional to the product of all factor potentials that involve that variable, evaluated over the current states of the other variables while varying the target variable's state. To compute this, for each factor involving the variable, we iterate over all possible assignments to all variables in that factor, find those assignments that match the current states of all except the target variable, and accumulate the factor potential for each value of the target variable. After computing the unnormalized conditional weights for each possible state of the variable, we normalize and sample from this distribution (using a uniform random number and cumulative sums). After the burn-in phase, we generate samples and for each sample, iterate over every factor, compute the absolute index of the factor configuration given the current variable states, and increment a count for that configuration. After all samples, each factor's counts are normalized to sum to 1; if a factor has zero total count, we replace it with a uniform distribution. Time complexity is \(O(\text{sweeps} \times V \times F \times K)\), where \(V\) is the number of variables, \(F\) is the number of factors, and \(K\) is the maximum number of configurations per factor (the product of cardinalities for variables in a factor). Space complexity is \(O(V + F \times K)\) for the variable states and the marginals/counts arrays. Edge cases include factors with zero probability mass in all samples, which are handled by uniform fallback, and cases where a variable's conditional distribution is zero for all states (which should not happen if factors are positive, but if it does, the sampling step will pick the first state with lowest index; we could add a guard to fallback to uniform, but the problem statement assumes non-empty and valid inputs, so we proceed with standard normalization and assertion that at least one state has positive weight). For simplicity, we assume the initial variable states are within valid ranges and any variable state has a non-zero probability under at least one factor combination (so the conditional is not all zeros). The random number generator uses a seeded `std::mt19937` and `std::uniform_real_distribution` for sampling.

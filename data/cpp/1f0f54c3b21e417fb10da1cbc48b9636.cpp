Write a C++ function named `simulate_discrete_distribution` that takes four non-negative integer weights (representing probabilities for categories 0, 1, 2, and 3) and an integer `sample_count` (≥ 1). The function must simulate drawing `sample_count` samples from a `std::discrete_distribution` with those weights using a `std::mt19937` random generator seeded by `std::random_device`. It should return a `std::array<int, 4>` where each element is the count of times that category index was sampled. The function must handle the case where all weights are zero by throwing an `std::invalid_argument` exception, and must use appropriate `const` correctness for parameters that are not modified.

// The core idea is to create a `std::discrete_distribution<int>` from the four weights. This distribution internally computes cumulative probabilities and returns category indices 0..3 with probabilities proportional to the weights. For each of the `sample_count` iterations, call `distribution(generator)` and increment the corresponding count in a `std::array<int,4>` initialized to zeros. The random generator must be seeded once using `std::random_device` to ensure non-deterministic behavior. Edge cases: if all weights are zero, the distribution constructor might throw `std::invalid_argument`; to be safe, explicitly check for the sum being zero and throw the same exception. Also ensure that `sample_count` is at least 1, though if it's 0 we could return all zeros without error, but the task says ≥1. Time complexity is \(O(\text{sample_count})\) because each sample is drawn in constant time (the distribution uses precomputed probabilities). Space complexity is \(O(1)\) for the counts array plus the distribution object's internal state.

#include <array>
#include <random>
#include <stdexcept>

// Simulate drawing sample_count times from a discrete distribution with the
// given weights for categories 0,1,2,3. Returns the count of each category.
std::array<int, 4> simulate_discrete_distribution(
    const std::array<int, 4>& weights, int sample_count) {
    
    // Validate weights: all must be non-negative, and at least one positive.
    int sum = 0;
    for (int w : weights) {
        if (w < 0) {
            throw std::invalid_argument("Weights must be non-negative");
        }
        sum += w;
    }
    if (sum == 0) {
        throw std::invalid_argument("At least one weight must be positive");
    }
    if (sample_count < 1) {
        throw std::invalid_argument("sample_count must be at least 1");
    }

    // Random generator seeded with a hardware source.
    std::random_device rd;
    std::mt19937 gen(rd());

    // Create the discrete distribution from the weights.
    std::discrete_distribution<int> dist(weights.begin(), weights.end());

    // Count occurrences.
    std::array<int, 4> counts = {0, 0, 0, 0};
    for (int i = 0; i < sample_count; ++i) {
        ++counts[dist(gen)];
    }
    return counts;
}

#include <cassert>
#include <array>
#include <numeric>
#include <functional>

int main() {
    // Test 1: Uniform weights (all equal) - each category should be near 2500 for 10000 samples.
    auto counts1 = simulate_discrete_distribution({1,1,1,1}, 10000);
    int total1 = 0;
    for (int c : counts1) total1 += c;
    assert(total1 == 10000);
    for (int c : counts1) {
        assert(c > 2000 && c < 3000); // reasonable range for 10000 samples
    }

    // Test 2: Single category with all weight - always returns 0 for others.
    auto counts2 = simulate_discrete_distribution({0,0,0,100}, 50);
    assert(counts2[0] == 0 && counts2[1] == 0 && counts2[2] == 0 && counts2[3] == 50);

    // Test 3: Only one positive weight in middle.
    auto counts3 = simulate_discrete_distribution({0, 5, 0, 0}, 1000);
    assert(counts3[1] == 1000 && counts3[0] + counts3[2] + counts3[3] == 0);

    // Test 4: Throws on all-zero weights.
    bool threw = false;
    try {
        simulate_discrete_distribution({0,0,0,0}, 10);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: Throws on negative weight.
    threw = false;
    try {
        simulate_discrete_distribution({1,-1,2,3}, 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: Throws on sample_count < 1.
    threw = false;
    try {
        simulate_discrete_distribution({1,1,1,1}, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 7: Weight ratios roughly respected (using 20000 samples for stability)
    auto counts7 = simulate_discrete_distribution({10, 30, 60, 0}, 20000);
    int total7 = 0;
    for (int c : counts7) total7 += c;
    assert(total7 == 20000);
    // Category 3 should never appear (weight 0)
    assert(counts7[3] == 0);
    // Category 0 should be roughly 10% of 20000 = 2000
    assert(counts7[0] > 1500 && counts7[0] < 2500);
    // Category 1 roughly 30% = 6000
    assert(counts7[1] > 5000 && counts7[1] < 7000);
    // Category 2 roughly 60% = 12000
    assert(counts7[2] > 10000 && counts7[2] < 14000);
}

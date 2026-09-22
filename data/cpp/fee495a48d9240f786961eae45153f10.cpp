// Write a C++ function that performs tournament selection on a population of candidate solutions, given a population represented as a vector of vectors of doubles (each inner vector is one candidate solution) and a parallel vector of fitness scores (one double per candidate). The function must return a new vector of the same size, where each position is filled by copying one of the original candidates. For each output position, independently select 3 random candidates (with replacement, i.e., indices can repeat) from the population, and choose the one with the highest fitness score; ties can be broken arbitrarily (e.g., if two candidates have equal score, either may be chosen). The function should assume that the population is non‑empty, that both vectors have equal size, and should not modify the input vectors. Use a uniform integer random generator for indices (e.g., `std::mt19937` seeded with a fixed value for reproducibility, or a passed‑in random engine; for simplicity, use `std::mt19937` with a fixed seed 42). The returned vector must contain exactly the same candidates (by value copy) as the input, just re‑arranged according to tournament selection.

The algorithm is straightforward: for each of the `n` output slots, we generate three random indices in the range `[0, n-1]` (using a uniform integer distribution). Then we compare the fitness scores at those indices and pick the index with the maximum value. If there are ties, any choice is acceptable, so comparing with `>=` is fine. We must be careful to use a good random engine and distribution. Since `std::mt19937` is deterministic when seeded with a fixed value, the output will be reproducible, which is desirable for testing. Complexity: For `n` candidates, we do `n` iterations, each performing three random draws and a constant number of comparisons, so time is `O(n)`. Space: we allocate a new vector of size `n`, so extra space is `O(n)`. The function should take the population and scores by `const` reference, and return by value. Edge cases: if the population has size 1, all three random indices will be 0, and the result is just a copy of the single candidate; this works naturally. If the scores contain `NaN` or `-inf`, comparisons behave per IEEE, but typical finite scores are assumed.

#include <vector>
#include <random>
#include <cstddef>

// Perform tournament selection of size 3 on a population.
// Input: population - vector of candidate solutions (each is a vector<double>)
//        fitness - parallel vector of fitness scores (higher is better)
// Precondition: population.size() == fitness.size() and population non-empty.
// Returns a new vector of the same size, where each element is a copy of one
// candidate chosen by a 3-way tournament among three random candidates.
std::vector<std::vector<double>> tournamentSelection(
    const std::vector<std::vector<double>>& population,
    const std::vector<double>& fitness)
{
    const std::size_t n = population.size();
    std::vector<std::vector<double>> selected(n);

    // Fixed seed for reproducible results in tests.
    std::mt19937 gen(42);
    std::uniform_int_distribution<std::size_t> dist(0, n - 1);

    for (std::size_t i = 0; i < n; ++i) {
        // Pick three random candidate indices.
        std::size_t a = dist(gen);
        std::size_t b = dist(gen);
        std::size_t c = dist(gen);

        // Determine the winner (highest fitness; ties broken arbitrarily).
        std::size_t winner = a;
        if (fitness[b] > fitness[winner]) winner = b;
        if (fitness[c] > fitness[winner]) winner = c;

        // Copy the winning candidate.
        selected[i] = population[winner];
    }

    return selected;
}

#include <cassert>
#include <cmath>

int main() {
    // Simple test: all fitnesses distinct, we can check that every output
    // element is one of the original candidates (by value equality).
    std::vector<std::vector<double>> pop = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    std::vector<double> fit = {0.5, 1.0, 1.5};
    auto result = tournamentSelection(pop, fit);

    assert(result.size() == 3);
    // Every result row must be one of the three original rows.
    for (const auto& row : result) {
        bool found = false;
        for (const auto& orig : pop) {
            if (row == orig) { found = true; break; }
        }
        assert(found);
    }

    // Test with a single candidate – should just return a copy.
    std::vector<std::vector<double>> pop2 = {{42.0}};
    std::vector<double> fit2 = {0.0};
    auto result2 = tournamentSelection(pop2, fit2);
    assert(result2.size() == 1);
    assert(result2[0] == pop2[0]);

    // Test with all fitnesses equal – every output must still be an element.
    std::vector<std::vector<double>> pop3 = {{1.0}, {2.0}, {3.0}};
    std::vector<double> fit3 = {1.0, 1.0, 1.0};
    auto result3 = tournamentSelection(pop3, fit3);
    for (const auto& row : result3) {
        bool found = false;
        for (const auto& orig : pop3) {
            if (row == orig) { found = true; break; }
        }
        assert(found);
    }

    // Test with many candidates to ensure the algorithm runs without issues.
    std::vector<std::vector<double>> pop4(100, std::vector<double>(2, 0.0));
    std::vector<double> fit4(100);
    for (int i = 0; i < 100; ++i) fit4[i] = static_cast<double>(i);
    auto result4 = tournamentSelection(pop4, fit4);
    assert(result4.size() == 100);

    // With a deterministic seed, the result is reproducible.
    auto result4_again = tournamentSelection(pop4, fit4);
    assert(result4 == result4_again);

    return 0;
}

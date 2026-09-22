// Implement a C++ function named `geneticExtrema` that performs a genetic algorithm to find the approximate maximum and minimum values of the mathematical function `f(x) = sin(x) * exp(2*x + sin(x)) + x^3 + x^2` over the real interval `[ -10, 10 ]`. The function must take as parameters the population size (a positive integer), the number of generations (a non-negative integer), the lower bound, and the upper bound of the search interval, and return a `std::pair<double, double>` where the first element is the approximate `x` that maximizes `f(x)` and the second element is the approximate `x` that minimizes `f(x)`. The genetic algorithm must use the following components exactly as described: (1) population initialization with uniform random real numbers within the search bounds, (2) parent selection via tournament selection with a tournament size of 3, choosing the best (highest `f(x)`) among randomly picked individuals, (3) uniform arithmetic crossover where the offspring is `alpha * parent1 + (1 - alpha) * parent2` with `alpha` uniformly random in `[0,1]`, (4) mutation with a rate of 10% that adds a uniformly random perturbation within ±5% of the total interval width, clamping to the bounds if necessary, and (5) full generational replacement where the entire population is replaced by newly created offspring each generation. After the specified number of generations, identify the individuals in the final population with the highest and lowest `f(x)` values and return their `x` positions. Your function must be deterministic in structure and rely only on standard C++ libraries; use a fixed seed (e.g., `srand(42)` inside the function) so that repeated calls produce identical results for the same parameters. The function must not print anything and must avoid any global state.

The solution follows the classic genetic algorithm paradigm for continuous optimization. We first define the objective function `f(x)` as a separate helper function for reuse. Population initialization creates `popSize` uniformly distributed random numbers in `[lower, upper]` using `rand()`. For each generation, we build a completely new population of the same size. For each new individual, we select two parents independently via tournament selection: each tournament picks `3` random individuals from the current population and returns the one with the highest `f(x)`. Crossover combines the two parents using a random arithmetic blend. Mutation may adjust the offspring with a small random shift; if the result goes outside bounds, it is clamped. After all generations, we iterate through the final population to find both the maximum and minimum `f(x)` by comparing function values, tracking the corresponding `x`. Edge cases include `popSize == 0` (should return a pair of zeros or handle gracefully) and `generations == 0` (should evaluate the initial population only). Time complexity is `O(generations * popSize * (tournamentSize + 1))` because each individual in each generation requires two tournaments of size 3 plus crossover and mutation. Space complexity is `O(popSize)` for storing the current and new populations. Using a fixed seed ensures reproducibility.

#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>

// Objective function to optimize: f(x) = sin(x) * exp(2x + sin(x)) + x^3 + x^2
double geneticObjective(double x) {
    return std::sin(x) * std::exp(2.0 * x + std::sin(x)) + std::pow(x, 3) + std::pow(x, 2);
}

// Initialize a population of random real numbers in [lower, upper]
std::vector<double> initializePopulation(int popSize, double lower, double upper) {
    std::vector<double> population(popSize);
    for (int i = 0; i < popSize; ++i) {
        population[i] = lower + static_cast<double>(std::rand()) / RAND_MAX * (upper - lower);
    }
    return population;
}

// Tournament selection: pick the best (highest f(x)) among 3 random candidates
double tournamentSelection(const std::vector<double>& population) {
    const int tournamentSize = 3;
    double best = population[std::rand() % population.size()];
    for (int i = 1; i < tournamentSize; ++i) {
        double candidate = population[std::rand() % population.size()];
        if (geneticObjective(candidate) > geneticObjective(best)) {
            best = candidate;
        }
    }
    return best;
}

// Arithmetic crossover: alpha * parent1 + (1 - alpha) * parent2
double arithmeticCrossover(double parent1, double parent2) {
    double alpha = static_cast<double>(std::rand()) / RAND_MAX;
    return alpha * parent1 + (1.0 - alpha) * parent2;
}

// Mutation with probability 0.1, perturb by ±5% of interval width, clamp to bounds
double mutate(double individual, double lower, double upper) {
    const double mutationRate = 0.1;
    if (static_cast<double>(std::rand()) / RAND_MAX < mutationRate) {
        double width = upper - lower;
        individual += (static_cast<double>(std::rand()) / RAND_MAX - 0.5) * width * 0.1;
        individual = std::max(lower, std::min(upper, individual));
    }
    return individual;
}

// Main genetic algorithm: returns pair of x values (maximizer, minimizer)
std::pair<double, double> geneticExtrema(int popSize, int generations, double lower, double upper) {
    // Handle invalid population size gracefully
    if (popSize <= 0) {
        return {0.0, 0.0};
    }

    // Fixed seed for reproducibility
    std::srand(42);

    // Initialize population
    std::vector<double> population = initializePopulation(popSize, lower, upper);

    // Evolve for the specified number of generations
    for (int gen = 0; gen < generations; ++gen) {
        std::vector<double> newPopulation;
        newPopulation.reserve(popSize);
        for (int i = 0; i < popSize; ++i) {
            double parent1 = tournamentSelection(population);
            double parent2 = tournamentSelection(population);
            double child = arithmeticCrossover(parent1, parent2);
            child = mutate(child, lower, upper);
            newPopulation.push_back(child);
        }
        population = newPopulation;
    }

    // Find the x that maximizes f(x) and the x that minimizes f(x)
    double maxX = population[0];
    double minX = population[0];
    for (size_t i = 1; i < population.size(); ++i) {
        if (geneticObjective(population[i]) > geneticObjective(maxX)) {
            maxX = population[i];
        }
        if (geneticObjective(population[i]) < geneticObjective(minX)) {
            minX = population[i];
        }
    }

    return {maxX, minX};
}

#include <cassert>
#include <cmath>
#include <utility>

int main() {
    // Fixed seed makes results deterministic; test with small parameters
    auto result1 = geneticExtrema(50, 100, -10.0, 10.0);
    // Verify the returned x values are within bounds
    assert(result1.first >= -10.0 && result1.first <= 10.0);
    assert(result1.second >= -10.0 && result1.second <= 10.0);

    // Test with zero generations: should return the best and worst from initial random population
    auto result2 = geneticExtrema(20, 0, -5.0, 5.0);
    // Check that the maximizer's f(x) >= minimizer's f(x)
    assert(geneticObjective(result2.first) >= geneticObjective(result2.second));

    // Test with a single-generation run to check stability under repeated calls
    auto result3a = geneticExtrema(30, 10, -10.0, 10.0);
    auto result3b = geneticExtrema(30, 10, -10.0, 10.0);
    // Same seed means identical results
    assert(result3a.first == result3b.first);
    assert(result3a.second == result3b.second);

    // Test with population size 1 (always returns that one individual)
    auto result4 = geneticExtrema(1, 5, -1.0, 1.0);
    assert(result4.first == result4.second);

    // Test with negative lower and positive upper, ensure outputs never exceed bounds
    auto result5 = geneticExtrema(100, 200, 0.0, 1.0);
    assert(result5.first >= 0.0 && result5.first <= 1.0);
    assert(result5.second >= 0.0 && result5.second <= 1.0);

    // Test with zero population size (edge case)
    auto result6 = geneticExtrema(0, 10, -10.0, 10.0);
    assert(result6.first == 0.0 && result6.second == 0.0);

    // Test that the maximizer indeed has a higher f(x) than the minimizer after many generations
    auto result7 = geneticExtrema(200, 500, -10.0, 10.0);
    assert(geneticObjective(result7.first) > geneticObjective(result7.second));

    return 0;
}

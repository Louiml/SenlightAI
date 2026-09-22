// You are given a fixed-size population of candidate solutions represented as `std::vector<int>` where each integer is a binary gene (0 or 1). Write a C++ function `std::vector<int> evolveKnapsack(const std::vector<int>& initialPopulation)` that runs a simplified genetic algorithm for 80 generations to maximize the fitness function `fitness(const std::vector<int>& chromosome)`, which returns the sum of all genes (i.e., the number of 1's in the chromosome). The algorithm must: (1) at each generation, sort the population by fitness in descending order; (2) directly copy the top 20% of the most fit individuals to the next generation; (3) fill the remaining 80% of the next generation by crossing over two randomly selected parents (selected uniformly from the entire current population) using single-point crossover, where the crossover point is chosen randomly between 1 and chromosome length-1, producing exactly two offspring; (4) with a fixed probability of 10%, mutate a randomly selected individual from the *current* generation by flipping exactly one random gene (0→1 or 1→0) before that individual is used as a parent in the next generation; (5) stop early if the best fitness equals the chromosome length (perfect adaptation). The population size is the length of the input vector (assume it is even and at least 4). The initial population is given as a flat vector where the first `N` entries are chromosome 0, the next `N` are chromosome 1, etc., where `N` is the chromosome length (i.e., the number of genes per individual). The function must return the final population, flattened in the same format. Note: Use a deterministic pseudo-random generator (e.g., `std::mt19937` seeded with 42) for reproducibility, and ensure the function is pure (no global state). The input chromosome length `N` is derived from the vector size divided by the population size, which must be computed from the vector size assuming the number of individuals equals the chromosome length (i.e., total size = N * N).

// The task is a simplified genetic algorithm (GA) with a trivial fitness function (maximize number of 1's). We first derive `N` from the input size: `totalSize = input.size()`, and since population size equals chromosome length, we have `N = sqrt(totalSize)`. We then structure the population as a `std::vector<std::vector<int>>` of `N` chromosomes each of length `N`. The main loop runs up to 80 generations. In each generation, we evaluate fitness (sum of genes) for all `N` individuals, store them in parallel vectors of (fitness, chromosome), and sort descending by fitness. Early termination: if the lowest-fitness individual (the last in sorted order) has fitness `N`, stop. For selection, we take the top `ceil(0.2*N)` individuals directly into the next generation. We then repeatedly select two distinct random parents (indices 0..N-1) from the entire current population (including those not selected for elitism), compute two children via single-point crossover: choose a crossover point `k` from 1 to N-1, child1 = genes[0..k-1] from parent1 + genes[k..N-1] from parent2, child2 is the complement. These children are added to the next generation until we reach `N` total individuals. Mutation: before generating parents, with probability 0.1 we pick a random index in the current population and flip one random gene in that chromosome; this mutation happens once per generation. Important edge cases: N must be a perfect square (guaranteed by input format); population size even ensures we always fill exactly with two children per crossover step; if an odd number of slots remain, we still add both children but then trim to exactly N (but with even N and top20% being an integer, this should not happen—we enforce by computing elitismCount = (int)ceil(0.2*N) and then while population size < N, we loop adding two children; if that overshoots, we truncate). Time complexity: each generation sorts N elements O(N log N) plus crossover O(N) per offspring, so total O(generations * N log N) in time, and O(N^2) space for the population. Since N is at most ~32 for this problem (to keep it simple), this is trivial.

#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

// Evaluate fitness: number of 1's in a chromosome.
int fitness(const std::vector<int>& chromosome) {
    int sum = 0;
    for (int gene : chromosome) sum += gene;
    return sum;
}

// Single-point crossover of two parents, returns two children.
std::pair<std::vector<int>, std::vector<int>> crossover(
        const std::vector<int>& parent1,
        const std::vector<int>& parent2,
        int point) {
    int n = parent1.size();
    std::vector<int> child1(n), child2(n);
    for (int i = 0; i < n; ++i) {
        if (i < point) {
            child1[i] = parent1[i];
            child2[i] = parent2[i];
        } else {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }
    return {child1, child2};
}

// Flip one random gene in a chromosome.
void mutate(std::vector<int>& chromosome, std::mt19937& rng) {
    int n = chromosome.size();
    std::uniform_int_distribution<int> dist(0, n - 1);
    int idx = dist(rng);
    chromosome[idx] = 1 - chromosome[idx];
}

// Run the genetic algorithm for 80 generations or until perfect fitness.
std::vector<int> evolveKnapsack(const std::vector<int>& initialPopulation) {
    if (initialPopulation.empty()) return {};

    int totalSize = initialPopulation.size();
    int n = static_cast<int>(std::sqrt(totalSize));
    if (n * n != totalSize || n < 2 || n % 2 != 0) {
        throw std::invalid_argument("Population size must be a perfect square and even chromosome length.");
    }

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> selectDist(0, n - 1);
    std::uniform_int_distribution<int> pointDist(1, n - 1);
    std::uniform_int_distribution<int> probDist(0, 99);

    // Build population: n chromosomes of length n.
    std::vector<std::vector<int>> population(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            population[i][j] = initialPopulation[i * n + j];
        }
    }

    for (int generation = 0; generation < 80; ++generation) {
        // Evaluate fitness.
        std::vector<std::pair<int, std::vector<int>>> evaluated;
        evaluated.reserve(n);
        for (const auto& chrom : population) {
            evaluated.emplace_back(fitness(chrom), chrom);
        }

        // Sort descending by fitness.
        std::sort(evaluated.begin(), evaluated.end(),
                  [](const auto& a, const auto& b) { return a.first > b.first; });

        // Early termination if least fit individual is perfect.
        bool perfect = evaluated.back().first == n;
        if (perfect) break;

        // Elitism: keep top 20% (ceiling) directly.
        int elitismCount = static_cast<int>(std::ceil(0.2 * n));
        std::vector<std::vector<int>> nextPopulation;
        nextPopulation.reserve(n);
        for (int i = 0; i < elitismCount; ++i) {
            nextPopulation.push_back(evaluated[i].second);
        }

        // Mutation: with 10% probability, mutate one random individual in the current population.
        if (probDist(rng) < 10) {
            int idx = selectDist(rng);
            mutate(population[idx], rng);
            // Note: the mutation affects the parents used below.
        }

        // Fill remaining slots by crossover.
        while (static_cast<int>(nextPopulation.size()) < n) {
            int p1 = selectDist(rng);
            int p2 = selectDist(rng);
            if (p1 == p2) continue;  // avoid same parent
            int point = pointDist(rng);
            auto [child1, child2] = crossover(population[p1], population[p2], point);
            nextPopulation.push_back(child1);
            if (static_cast<int>(nextPopulation.size()) < n) {
                nextPopulation.push_back(child2);
            }
        }

        // Ensure exactly n individuals (should already be).
        if (static_cast<int>(nextPopulation.size()) > n) {
            nextPopulation.resize(n);
        }

        population = std::move(nextPopulation);
    }

    // Flatten result.
    std::vector<int> result;
    result.reserve(n * n);
    for (const auto& chrom : population) {
        result.insert(result.end(), chrom.begin(), chrom.end());
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Helper to compute final best fitness from flattened result.
    auto bestFitness = [](const std::vector<int>& flattened, int n) {
        int best = 0;
        for (int i = 0; i < n; ++i) {
            int sum = 0;
            for (int j = 0; j < n; ++j) sum += flattened[i * n + j];
            if (sum > best) best = sum;
        }
        return best;
    };

    // Test 1: All zeros, N=4. Should evolve to all ones (perfect fitness 4).
    std::vector<int> zeros(16, 0);
    auto result = evolveKnapsack(zeros);
    assert(result.size() == 16);
    assert(bestFitness(result, 4) == 4);

    // Test 2: All ones, N=4. Should remain perfect immediately.
    std::vector<int> ones(16, 1);
    result = evolveKnapsack(ones);
    assert(bestFitness(result, 4) == 4);

    // Test 3: Mixed initial population, N=2? But N must be even, so N=2 is okay (size=4).
    std::vector<int> mix = {0,1, 1,0};  // two chromosomes of length 2
    result = evolveKnapsack(mix);
    assert(result.size() == 4);
    // After evolution, best fitness should be 2 (perfect).
    int sum1 = result[0] + result[1];
    int sum2 = result[2] + result[3];
    assert(std::max(sum1, sum2) == 2);

    // Test 4: N=4, initial with some ones.
    std::vector<int> partial = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    result = evolveKnapsack(partial);
    assert(bestFitness(result, 4) == 4);

    // Test 5: N=6 (size=36), all zeros, should converge to all ones.
    std::vector<int> zeros36(36, 0);
    result = evolveKnapsack(zeros36);
    assert(result.size() == 36);
    assert(bestFitness(result, 6) == 6);

    // Test 6: Invalid size should throw.
    bool threw = false;
    try {
        evolveKnapsack({1,2,3});  // not a perfect square
    } catch (...) { threw = true; }
    assert(threw);

    // Test 7: N=2, all zeros, should become perfect.
    std::vector<int> zeros4(4, 0);
    result = evolveKnapsack(zeros4);
    assert(bestFitness(result, 2) == 2);

    // Test 8: N=4, highly fit but not perfect, should still reach perfect.
    std::vector<int> almost = {1,1,1,0, 1,1,0,1, 1,0,1,1, 0,1,1,1};
    result = evolveKnapsack(almost);
    assert(bestFitness(result, 4) == 4);

    // Test 9: Large N=8, all zeros, check size and perfect convergence.
    std::vector<int> zeros64(64, 0);
    result = evolveKnapsack(zeros64);
    assert(result.size() == 64);
    assert(bestFitness(result, 8) == 8);

    // Test 10: Deterministic with seed 42: same input gives same output.
    std::vector<int> input = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    auto r1 = evolveKnapsack(input);
    auto r2 = evolveKnapsack(input);
    assert(r1 == r2);

    return 0;
}

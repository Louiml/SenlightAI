/*
Write a standalone C++ function named `detectAllOnesChromosome` that takes a 2D vector of integers (`std::vector<std::vector<int>>`), where each inner vector represents a chromosome with exactly 10 genes (each gene being either 0 or 1). The function should simulate the behavior from the provided Genetic class snippet: it must return `true` if at least one chromosome in the population consists entirely of 1s (i.e., all 10 genes equal 1), and `false` otherwise. The function must handle populations of any size (including empty and single-chromosome cases), should not modify the input (use `const` reference), and must treat any out-of-range gene values (e.g., values other than 0 or 1) as breaking the run of consecutive 1s (i.e., such a chromosome cannot be all ones). Do not rely on any external classes or file I/O; the solution must be self-contained.
*/
#include <vector>

// Returns true if at least one chromosome consists entirely of 1s (all 10 genes equal 1).
// Assumes each chromosome has exactly 10 genes; if a chromosome has a different length or
// contains any value other than 1, it is considered not all-ones.
bool detectAllOnesChromosome(const std::vector<std::vector<int>>& chromosomePopulation) {
    for (const auto& chromosome : chromosomePopulation) {
        // Check if this chromosome has exactly 10 genes and all are 1.
        if (chromosome.size() == 10) {
            bool allOnes = true;
            for (int gene : chromosome) {
                if (gene != 1) {
                    allOnes = false;
                    break;
                }
            }
            if (allOnes) {
                return true;
            }
        }
    }
    return false;
}
#include <cassert>
#include <vector>

// Declaration from the solution
bool detectAllOnesChromosome(const std::vector<std::vector<int>>& chromosomePopulation);

int main() {
    // Test 1: Population with one all-ones chromosome (size 10)
    std::vector<std::vector<int>> pop1 = {
        {1,1,1,1,1,1,1,1,1,1},
        {0,1,0,1,0,1,0,1,0,1}
    };
    assert(detectAllOnesChromosome(pop1) == true);

    // Test 2: No all-ones chromosome
    std::vector<std::vector<int>> pop2 = {
        {1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0}
    };
    assert(detectAllOnesChromosome(pop2) == false);

    // Test 3: Empty population
    std::vector<std::vector<int>> pop3;
    assert(detectAllOnesChromosome(pop3) == false);

    // Test 4: Chromosome with wrong length (9 genes) but all ones – should not count
    std::vector<std::vector<int>> pop4 = {
        {1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1}
    };
    assert(detectAllOnesChromosome(pop4) == true); // second one counts

    // Test 5: Chromosome with a value other than 0 or 1 (e.g., 2) – not all ones
    std::vector<std::vector<int>> pop5 = {
        {1,1,1,1,1,1,1,1,1,2},
        {1,1,1,1,1,1,1,1,1,1}
    };
    assert(detectAllOnesChromosome(pop5) == true); // second one counts

    // Test 6: Only one chromosome, all ones, length 10
    std::vector<std::vector<int>> pop6 = {{1,1,1,1,1,1,1,1,1,1}};
    assert(detectAllOnesChromosome(pop6) == true);

    // Test 7: Only one chromosome, all zeros
    std::vector<std::vector<int>> pop7 = {{0,0,0,0,0,0,0,0,0,0}};
    assert(detectAllOnesChromosome(pop7) == false);

    // Test 8: Population with mixed-length inner vectors, but one valid all-ones
    std::vector<std::vector<int>> pop8 = {
        {1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1}
    };
    assert(detectAllOnesChromosome(pop8) == true);

    // Test 9: Population where inner vectors have length 10 but contain non-1 values
    std::vector<std::vector<int>> pop9 = {
        {1,0,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1}
    };
    assert(detectAllOnesChromosome(pop9) == true); // second chromosome

    // Test 10: All inner vectors length not equal to 10
    std::vector<std::vector<int>> pop10 = {
        {1,1,1},
        {1,1,1}
    };
    assert(detectAllOnesChromosome(pop10) == false);

    return 0;
}
// The problem reduces to scanning each chromosome (row) to determine if every element equals 1. The provided snippet's logic checks for a count of 1s reaching 10 exactly only when the last index (j==9) is reached, and it also tries to break early on non-zero non-one values. However, a simpler and more robust approach is to check each chromosome for any element that is not 1; if found, that chromosome is skipped. If a chromosome passes the scan (all elements are 1), return `true` immediately. Edge cases include: an empty population (return `false`), an empty inner vector (cannot be all ones because length isn't 10, so skip), chromosomes with values other than 0 or 1 (treat as not all ones), and exactly one chromosome that is all ones (should return true). Time complexity is O(m * n) where m is number of chromosomes and n is fixed at 10, but could be general. Space complexity is O(1) auxiliary, as we only use a loop counter and a boolean flag.

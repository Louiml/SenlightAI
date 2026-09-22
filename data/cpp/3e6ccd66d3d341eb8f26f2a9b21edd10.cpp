Write a C++ function `simulate_classifier_rounds` that takes a vector of real numbers representing the initial fitness values of a population of classifiers (the number of classifiers is the vector's size) and a non-negative integer `rounds`. The function simulates an evolutionary selection process where, in each round, every classifier's fitness is updated using the rule `new_fitness = old_fitness * 0.9 + 0.1 * (average_fitness_of_all_classifiers_in_that_round)`, and then the classifier with the lowest fitness is removed from the population (if there is a tie, remove the one with the smallest index). After exactly `rounds` rounds of simultaneous updates and removals, return a vector of the remaining fitness values sorted in descending order. If `rounds` is greater than or equal to the initial population size, return an empty vector (population becomes extinct). The input vector may contain negative values, zeros, or duplicates; the function must handle these correctly and not modify the input vector.

// The simulation requires careful handling of simultaneous updates: all new fitness values for a round must be computed from the same population state before any removal occurs. For each round, first compute the average of the current fitness values. Then create a new vector where each element is `0.9 * old + 0.1 * average`. After updating all elements, find the index of the smallest value in the updated vector (if multiple, pick the smallest index). Remove that element. Repeat for the specified number of rounds. If at any point the population size becomes zero, break early. Edge cases include: `rounds = 0` should return the input sorted descending; negative values are fine; when only one classifier remains, the average equals that value, so the update keeps it constant; if `rounds >= initial size`, the population will eventually become empty (since each round removes one), so return an empty vector. Time complexity: for `r` rounds and `n` initial classifiers, each round costs O(n) for the average and O(n) for finding the minimum and removal (if using vector erase, O(n) per erase), giving O(r * n) in the worst case, and with r ≤ n typically O(n²). Space complexity is O(n) for storing the population.

#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

// Simulate rounds of fitness update and removal, returning sorted remaining fitnesses.
// The input vector is not modified. `rounds` is a non-negative integer.
std::vector<double> simulate_classifier_rounds(const std::vector<double>& initial_fitness, int rounds) {
    if (rounds < 0) return {}; // defensive, though task says non-negative
    std::vector<double> population = initial_fitness;

    for (int r = 0; r < rounds; ++r) {
        if (population.empty()) break;

        // Compute average of current population
        double sum = std::accumulate(population.begin(), population.end(), 0.0);
        double avg = sum / static_cast<double>(population.size());

        // Simultaneously update all fitness values
        std::vector<double> updated;
        updated.reserve(population.size());
        for (double val : population) {
            updated.push_back(val * 0.9 + 0.1 * avg);
        }

        // Find the index of the smallest value (ties: smallest index)
        size_t min_idx = 0;
        for (size_t i = 1; i < updated.size(); ++i) {
            if (updated[i] < updated[min_idx]) {
                min_idx = i;
            }
        }

        // Remove the found element (using erase on the updated vector)
        updated.erase(updated.begin() + static_cast<std::ptrdiff_t>(min_idx));
        population = std::move(updated);
    }

    // Sort descending
    std::sort(population.begin(), population.end(), std::greater<double>());
    return population;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

int main() {
    // Test 1: zero rounds returns sorted input
    std::vector<double> v1 = {3.0, 1.0, 2.0};
    auto res1 = simulate_classifier_rounds(v1, 0);
    assert(res1.size() == 3);
    assert(std::fabs(res1[0] - 3.0) < 1e-9);
    assert(std::fabs(res1[1] - 2.0) < 1e-9);
    assert(std::fabs(res1[2] - 1.0) < 1e-9);

    // Test 2: one round on a single element: average equals value, stays constant
    std::vector<double> v2 = {5.0};
    auto res2 = simulate_classifier_rounds(v2, 1);
    assert(res2.size() == 1);
    assert(std::fabs(res2[0] - 5.0) < 1e-9);

    // Test 3: two elements, one round: both updated, min removed
    std::vector<double> v3 = {10.0, 0.0};
    auto res3 = simulate_classifier_rounds(v3, 1);
    assert(res3.size() == 1);
    // avg = 5.0, new values: 10*0.9+0.1*5=9.5, 0*0.9+0.1*5=0.5 => remove 0.5, keep 9.5
    assert(std::fabs(res3[0] - 9.5) < 1e-9);

    // Test 4: rounds >= initial size leads to empty
    std::vector<double> v4 = {1.0, 2.0, 3.0};
    assert(simulate_classifier_rounds(v4, 3).empty());
    assert(simulate_classifier_rounds(v4, 5).empty());

    // Test 5: duplicates and negative values
    std::vector<double> v5 = {-1.0, -1.0, 0.0};
    auto res5 = simulate_classifier_rounds(v5, 1);
    // avg = -2/3 ≈ -0.6666667
    // new: -1*0.9+0.1*(-0.6667) = -0.96667, same for second, 0*0.9+0.1*(-0.6667) = -0.06667
    // min is -0.96667 (index 0), remove it, remaining: -0.96667, -0.06667
    assert(res5.size() == 2);
    assert(std::fabs(res5[0] - (-0.0666666667)) < 1e-6);
    assert(std::fabs(res5[1] - (-0.9666666667)) < 1e-6);

    // Test 6: input vector is not modified
    std::vector<double> v6 = {7.0, 3.0};
    auto original = v6;
    auto res6 = simulate_classifier_rounds(v6, 1);
    assert(v6 == original);
    assert(res6.size() == 1);
    // avg=5, new: 7*0.9+0.5=6.8, 3*0.9+0.5=3.2 => remove 3.2, keep 6.8
    assert(std::fabs(res6[0] - 6.8) < 1e-9);

    std::cout << "All tests passed.\n";
    return 0;
}

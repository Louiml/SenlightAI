Write a standalone C++ function that simulates a simplified version of Drude particle detection and thermostat assignment. Given a set of particles with masses and a list of Drude particle pairs (where each Drude particle has an index `p` and its parent real atom index `p1`), the function must identify all real particles that should be thermostated. Real particles are those with mass > 0.0 that are NOT part of any Drude pair (neither as Drude particle nor as its parent). The function should return two things: a sorted vector of real particle indices that are thermostated (those not involved in any Drude pair), and a vector of `std::pair<int,int>` Drude pairs (the input pairs, unchanged). Handle edge cases where a particle with mass > 0 may appear multiple times in Drude pairs (should be excluded from real set entirely), and ensure the output real particles are sorted in ascending order. Also validate that the input Drude pairs are non-empty (throw `std::invalid_argument` if empty) and that every Drude pair index is within the particle count (throw `std::out_of_range` if any index ≥ numParticles). The function signature should be: `std::pair<std::vector<int>, std::vector<std::pair<int,int>>> identifyThermostatedParticles(int numParticles, const std::vector<double>& masses, const std::vector<std::pair<int,int>>& drudePairs)`. The function must not modify the input vectors.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function to test (could be declared in a header in practice)
std::pair<std::vector<int>, std::vector<std::pair<int,int>>> 
identifyThermostatedParticles(int numParticles, 
                              const std::vector<double>& masses, 
                              const std::vector<std::pair<int,int>>& drudePairs);

int main() {
    // Test 1: basic case with 5 particles, one Drude pair
    {
        std::vector<double> masses = {1.0, 2.0, 0.0, 4.0, 5.0};
        std::vector<std::pair<int,int>> pairs = {{1, 3}};
        auto result = identifyThermostatedParticles(5, masses, pairs);
        assert((result.first == std::vector<int>{0, 4}));
        assert((result.second == pairs));
    }

    // Test 2: particle appears as both Drude and parent in different pairs
    {
        std::vector<double> masses = {1.0, 2.0, 3.0, 4.0, 5.0};
        std::vector<std::pair<int,int>> pairs = {{0, 2}, {2, 4}};
        auto result = identifyThermostatedParticles(5, masses, pairs);
        assert((result.first == std::vector<int>{1, 3}));  // 0,2,4 all removed
    }

    // Test 3: all particles are in Drude pairs → empty real set
    {
        std::vector<double> masses = {1.0, 1.0, 1.0, 1.0};
        std::vector<std::pair<int,int>> pairs = {{0, 1}, {2, 3}};
        auto result = identifyThermostatedParticles(4, masses, pairs);
        assert(result.first.empty());
        assert(result.second == pairs);
    }

    // Test 4: zero-mass particles are ignored and not in output
    {
        std::vector<double> masses = {0.0, 2.0, 0.0, 4.0};
        std::vector<std::pair<int,int>> pairs = {{0, 2}};  // particles 0 and 2 have zero mass but in pair
        auto result = identifyThermostatedParticles(4, masses, pairs);
        assert((result.first == std::vector<int>{1, 3}));  // zero-mass removed, but 0,2 already not in set
    }

    // Test 5: duplicate pairs
    {
        std::vector<double> masses = {1.0, 2.0, 3.0, 4.0};
        std::vector<std::pair<int,int>> pairs = {{0, 1}, {0, 1}};
        auto result = identifyThermostatedParticles(4, masses, pairs);
        assert((result.first == std::vector<int>{2, 3}));
    }

    // Test 6: invalid - empty pairs
    {
        bool threw = false;
        try {
            std::vector<double> masses = {1.0, 2.0};
            identifyThermostatedParticles(2, masses, std::vector<std::pair<int,int>>{});
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 7: invalid - out of range index
    {
        bool threw = false;
        try {
            std::vector<double> masses = {1.0, 2.0};
            std::vector<std::pair<int,int>> pairs = {{5, 1}};
            identifyThermostatedParticles(2, masses, pairs);
        } catch (const std::out_of_range&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 8: invalid - wrong masses size
    {
        bool threw = false;
        try {
            std::vector<double> masses = {1.0};
            std::vector<std::pair<int,int>> pairs = {{0, 0}};
            identifyThermostatedParticles(2, masses, pairs);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Test 9: sorted order even if input unsorted
    {
        std::vector<double> masses = {1.0, 1.0, 1.0, 1.0, 1.0};
        std::vector<std::pair<int,int>> pairs = {{4, 2}};
        auto result = identifyThermostatedParticles(5, masses, pairs);
        assert((result.first == std::vector<int>{0, 1, 3}));
    }

    // Test 10: single particle not in any pair
    {
        std::vector<double> masses = {3.0};
        std::vector<std::pair<int,int>> pairs = {{0, 0}};  // self-pair, but still removes it
        auto result = identifyThermostatedParticles(1, masses, pairs);
        assert(result.first.empty());
    }

    return 0;
}

#include <vector>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <utility>

/**
 * Identify real particles to be thermostated in a Drude system.
 * 
 * Real particles are those with mass > 0 that are not part of any Drude pair
 * (neither as the Drude particle nor as its parent). The function returns
 * a sorted vector of those particle indices, along with the original Drude pairs.
 *
 * @param numParticles  Total number of particles in the system.
 * @param masses        Mass of each particle (size must equal numParticles).
 * @param drudePairs    Non-empty list of Drude pairs (p, p1) where p is the Drude
 *                      particle and p1 is its parent real atom.
 * @return A pair containing: (1) sorted vector of thermostated real particle indices,
 *         (2) the original drudePairs vector (unchanged).
 * @throws std::invalid_argument if drudePairs is empty or masses.size() != numParticles.
 * @throws std::out_of_range if any index in drudePairs is >= numParticles or negative.
 */
std::pair<std::vector<int>, std::vector<std::pair<int,int>>> 
identifyThermostatedParticles(int numParticles, 
                              const std::vector<double>& masses, 
                              const std::vector<std::pair<int,int>>& drudePairs) {
    // Validate inputs
    if (drudePairs.empty()) {
        throw std::invalid_argument("Drude pairs cannot be empty");
    }
    if (static_cast<int>(masses.size()) != numParticles) {
        throw std::invalid_argument("masses size must equal numParticles");
    }
    for (const auto& pr : drudePairs) {
        if (pr.first < 0 || pr.first >= numParticles ||
            pr.second < 0 || pr.second >= numParticles) {
            throw std::out_of_range("Drude pair index out of range");
        }
    }

    // Collect all positive-mass particles into a set
    std::set<int> realSet;
    for (int i = 0; i < numParticles; ++i) {
        if (masses[i] > 0.0) {
            realSet.insert(i);
        }
    }

    // Remove all particles that appear in any Drude pair
    for (const auto& pr : drudePairs) {
        realSet.erase(pr.first);   // Drude particle
        realSet.erase(pr.second);  // parent real atom
    }

    // Convert set to sorted vector (set is already sorted, but explicit for clarity)
    std::vector<int> thermostated(realSet.begin(), realSet.end());
    std::sort(thermostated.begin(), thermostated.end());

    // Return the list and the original pairs
    return std::make_pair(thermostated, drudePairs);
}

// The solution approach follows the logic from the DrudeNoseHooverIntegrator's initialization: first collect all particle indices with mass > 0 into a set (to allow fast removal). Then iterate through each Drude pair and erase both the Drude particle index (`p`) and its parent (`p1`) from the set – this handles duplicates naturally since `std::set::erase` is idempotent. After processing all pairs, the remaining elements in the set are the real particles that should be thermostated. Convert the set to a sorted vector (sets are already sorted, but explicit sorting ensures correctness if using an unordered set). Validate inputs upfront: check that `drudePairs` is not empty, and that every index in the pairs is in the range `[0, numParticles)`. Also check that `masses` has exactly `numParticles` elements (throw `std::invalid_argument` otherwise). Important edge cases: a particle could appear as both a Drude particle and a parent in different pairs – removing it from the set twice is fine; a particle with mass ≤ 0 should never be in the set, so erasing it has no effect; if all particles are part of Drude pairs, the real set becomes empty. Time complexity is O(N + M log N) where N is number of particles and M is number of Drude pairs (due to set operations); space complexity is O(N) for the set and output vector.

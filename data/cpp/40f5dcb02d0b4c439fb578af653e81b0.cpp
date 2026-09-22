/*
You are given information about `n` candies, each with a flavor `type` (0 or 1), a required strength `height` (how strong the monster must be to eat it), and a mass gain `mass` (strength increase after eating it). You start with a given initial `strength` and can choose to begin by eating only candies of flavor 0 or only flavor 1. After eating a candy, your current flavor preference flips to the other flavor (you must eat a candy of the opposite flavor next). You may eat a candy only if its required strength is ≤ your current strength, and each candy can be eaten at most once. You may also eat any candy of the currently preferred flavor that is reachable, but you must always choose, among all currently reachable and uneaten candies of the preferred flavor, the one with the **largest mass** (ties are arbitrary). The process stops when no candy of the preferred flavor is reachable. Write a C++ function `int maxCandies(int n, int initialStrength, const std::vector<int>& type, const std::vector<int>& height, const std::vector<int>& mass)` that returns the maximum number of candies you can eat overall, assuming you can pick either starting flavor (0 or 1) optimally. The input vectors have indices 0..n-1.
*/

#include <vector>
#include <algorithm>

// Simulates the candy-eating process for a given starting flavor.
// Returns the number of candies eaten.
int simulate(int n, int initialStrength, const std::vector<int>& type,
             const std::vector<int>& height, const std::vector<int>& mass,
             int startingType) {
    int strength = initialStrength;
    int candies = 0;
    int currentType = startingType;
    // 0 = not evaluated yet, 1 = reachable but not eaten, 2 = eaten
    std::vector<int> status(n, 0);

    while (true) {
        // Mark all currently reachable and uneaten candies.
        for (int i = 0; i < n; ++i) {
            if (status[i] == 0 && height[i] <= strength) {
                status[i] = 1;
            }
        }

        // Find the best (largest mass) reachable candy of the current type.
        int bestMass = -1;
        int bestId = -1;
        for (int i = 0; i < n; ++i) {
            if (status[i] == 1 && type[i] == currentType && mass[i] > bestMass) {
                bestMass = mass[i];
                bestId = i;
            }
        }

        if (bestId == -1) {
            break;
        }

        // Eat the chosen candy.
        ++candies;
        strength += mass[bestId];
        status[bestId] = 2;
        currentType ^= 1; // flip flavor
    }

    return candies;
}

// Returns the maximum number of candies that can be eaten.
int maxCandies(int n, int initialStrength, const std::vector<int>& type,
               const std::vector<int>& height, const std::vector<int>& mass) {
    return std::max(simulate(n, initialStrength, type, height, mass, 0),
                    simulate(n, initialStrength, type, height, mass, 1));
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// Provide a global main that runs asserts.

int main() {
    // Example 1: trivial case, no candy reachable initially.
    {
        std::vector<int> type = {0, 1};
        std::vector<int> height = {10, 10};
        std::vector<int> mass = {5, 5};
        assert(maxCandies(2, 5, type, height, mass) == 0);
    }

    // Example 2: only one type reachable, but flips allow alternating.
    {
        std::vector<int> type = {0, 1, 0};
        std::vector<int> height = {1, 5, 10};
        std::vector<int> mass = {1, 10, 5};
        // Start with 0: can eat candy 0 (strength 1→2), then type 1: candy 1 (strength 2→12), then type 0: candy 2 (strength 12→17) => 3.
        assert(maxCandies(3, 1, type, height, mass) == 3);
    }

    // Example 3: tie in mass, should still count all.
    {
        std::vector<int> type = {0, 0, 1, 1};
        std::vector<int> height = {1, 1, 2, 2};
        std::vector<int> mass = {3, 3, 3, 3};
        // Start with 0: eat any 0 (strength becomes 4), then 1: eat any 1 (strength 7), then 0: eat other 0, then 1: eat other 1 => 4.
        assert(maxCandies(4, 1, type, height, mass) == 4);
    }

    // Example 4: starting flavor matters.
    {
        std::vector<int> type = {0, 1, 0};
        std::vector<int> height = {2, 1, 100};
        std::vector<int> mass = {10, 1, 100};
        // Start 0: candy 0 (strength 2→12), then type1: candy1 (12→13), then type0: candy2 uneatable => 2.
        // Start 1: candy1 (strength1→2), then type0: candy0 (2→12), then type1: no reachable 1 => 2.
        assert(maxCandies(3, 1, type, height, mass) == 2);
    }

    // Example 5: larger test with sequential chain.
    {
        std::vector<int> type = {0, 1, 0, 1, 0};
        std::vector<int> height = {1, 2, 3, 4, 5};
        std::vector<int> mass = {1, 1, 1, 1, 1};
        // Start 0: strength goes 1,2,3,4,5,6 => can eat all 5.
        assert(maxCandies(5, 1, type, height, mass) == 5);
    }

    // Example 6: cannot eat due to flavor barrier.
    {
        std::vector<int> type = {0, 0};
        std::vector<int> height = {1, 1};
        std::vector<int> mass = {1, 1};
        // Start 0: eat one (strength2), then need type1 but none => 1. Start 1: none reachable => 0. Max=1.
        assert(maxCandies(2, 1, type, height, mass) == 1);
    }

    return 0;
}

// The core idea is to simulate the eating process for each possible starting flavor (0 and 1) and then take the maximum. The simulation must be careful: at each step, you first mark all candies that become reachable (height ≤ current strength and not yet eaten), then among those marked but uneaten candies with the current preferred flavor, pick the one with the largest mass (if multiple, any). After eating it, increment candy count, add its mass to strength, mark it as eaten (status 2), and flip the preferred flavor. The process repeats until no such candy exists. Important edge cases: candies of the other flavor may still be reachable but cannot be eaten until the flavor flips; the initial marking must be done before picking the first candy; if no candy of the starting flavor is reachable, the function returns 0 for that starting flavor. Also, because the mass gain can increase strength, previously unreachable candies might become reachable later, so the marking step must be done inside the loop. The greedy choice of the largest mass among reachable candies of the preferred flavor is essential; a different choice might lead to a worse total count. Time complexity is O(n^2) in the worst case because each of the at most n candies eaten can trigger a scan of all n candies to mark and then another scan to find the best. Space complexity is O(n) for the status array (we can reuse a vector of char). The solution returns max(simulate(0), simulate(1)).

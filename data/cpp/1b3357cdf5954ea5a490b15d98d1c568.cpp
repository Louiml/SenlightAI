Write a standalone C++ function that simulates a simplified predator-prey-farmer ecosystem update for a single predator cell. Given a predator cell's current energy, a list of nearby prey cell distances (already computed and passed as a vector of structs containing `distance` and `is_prey`), and a time step `dt`, the function must update the predator's energy according to the following rules: (1) If any prey is within a detection radius of 2.0 units beyond the sum of radii (here simplified as `distance <= detection_radius`), the predator "eats" the nearest such prey (the one with smallest distance) and gains +100 energy. (2) Energy decays exponentially at rate 0.00025 per unit time: `energy /= (1.0 + dt * 0.00025)`. (3) If after eating and decaying the energy falls below 0.1, the function returns `true` to indicate death; otherwise returns `false`. The function must be `const`-correct for the input parameter, mutate the energy reference, and handle the edge case where multiple prey are within range (only the nearest is consumed). Do not include any I/O, main, or external dependencies beyond standard headers.

The solution requires iterating over the list of nearby cells to find the nearest prey within detection radius. The detection radius is fixed at 2.0. If multiple prey are within range, select the one with smallest distance. Only one prey is consumed per update, granting a flat +100 energy bonus. After the eating step, apply the exponential decay formula exactly as specified: `energy = energy / (1.0 + dt * decay_rate)`. Finally, compare the updated energy to a threshold of 0.1; if strictly less, return `true` (death), else `false`. Edge cases include: empty neighbor list (energy still decays), prey present but all out of range (no gain), and energy already below threshold after decay (death). Complexity is O(N) where N is the number of neighbors, using O(1) auxiliary space. The function signature should accept the neighbor list by const reference to avoid copying and allow mutation of energy via reference.

#include <vector>
#include <limits>
#include <cmath>

// A simple struct representing a nearby cell.
struct NearbyCell {
    double distance;
    bool is_prey;
};

// Updates predator energy based on eating nearest prey and decay.
// Returns true if the predator dies (energy < 0.1 after update).
bool update_predator_energy(double& energy, const std::vector<NearbyCell>& neighbors, double dt) {
    const double detection_radius = 2.0;
    const double energy_gain = 100.0;
    const double decay_rate = 0.00025;
    const double death_threshold = 0.1;

    double best_distance = std::numeric_limits<double>::infinity();
    bool found_prey = false;

    for (const NearbyCell& cell : neighbors) {
        if (cell.is_prey && cell.distance <= detection_radius) {
            if (cell.distance < best_distance) {
                best_distance = cell.distance;
                found_prey = true;
            }
        }
    }

    if (found_prey) {
        energy += energy_gain;
    }

    energy /= (1.0 + dt * decay_rate);

    return energy < death_threshold;
}

#include <cassert>
#include <vector>

// struct NearbyCell and function update_predator_energy are assumed defined above.

int main() {
    // Test 1: No neighbors, energy decays but remains above threshold.
    double e1 = 50.0;
    std::vector<NearbyCell> n1 = {};
    assert(update_predator_energy(e1, n1, 1.0) == false);
    assert(std::abs(e1 - (50.0 / 1.00025)) < 1e-9);

    // Test 2: One prey within radius -> eat and gain energy.
    double e2 = 1.0;
    std::vector<NearbyCell> n2 = {{1.5, true}};
    assert(update_predator_energy(e2, n2, 1.0) == false);
    assert(std::abs(e2 - (101.0 / 1.00025)) < 1e-9);

    // Test 3: Multiple prey, only nearest is eaten.
    double e3 = 10.0;
    std::vector<NearbyCell> n3 = {{3.0, true}, {0.5, true}, {2.0, false}};
    assert(update_predator_energy(e3, n3, 1.0) == false);
    // Only the 0.5 prey is eaten, gain +100, then decay.
    assert(std::abs(e3 - (110.0 / 1.00025)) < 1e-9);

    // Test 4: Prey out of range -> no gain.
    double e4 = 20.0;
    std::vector<NearbyCell> n4 = {{2.5, true}, {10.0, true}};
    assert(update_predator_energy(e4, n4, 1.0) == false);
    assert(std::abs(e4 - (20.0 / 1.00025)) < 1e-9);

    // Test 5: Death when energy drops below threshold after decay.
    double e5 = 0.05;
    std::vector<NearbyCell> n5 = {{1.0, true}}; // gain 100, so death unlikely, but test low energy with no prey.
    std::vector<NearbyCell> n5b = {};
    assert(update_predator_energy(e5, n5b, 1.0) == true);

    // Test 6: Exact threshold boundary (0.1) -> not death.
    double e6 = 0.1;
    std::vector<NearbyCell> n6 = {};
    // After decay, energy < 0.1, so death true.
    assert(update_predator_energy(e6, n6, 1.0) == true);

    // Test 7: Large dt decays energy significantly but prey keeps alive.
    double e7 = 1.0;
    std::vector<NearbyCell> n7 = {{0.1, true}};
    assert(update_predator_energy(e7, n7, 100.0) == false);
    // 101 / (1 + 100*0.00025) = 101 / 1.025 ≈ 98.5366 > 0.1

    // Test 8: Zero dt, no decay, just gain.
    double e8 = 5.0;
    std::vector<NearbyCell> n8 = {{1.0, true}};
    assert(update_predator_energy(e8, n8, 0.0) == false);
    assert(e8 == 105.0);

    // Test 9: Negative dt (should not happen, but decay formula still applied).
    double e9 = 100.0;
    std::vector<NearbyCell> n9 = {};
    assert(update_predator_energy(e9, n9, -0.5) == false);
    assert(std::abs(e9 - (100.0 / (1.0 - 0.5*0.00025))) < 1e-9);

    // Test 10: Multiple prey, nearest at radius exactly 2.0 is eaten.
    double e10 = 0.5;
    std::vector<NearbyCell> n10 = {{2.0, true}, {2.1, true}};
    assert(update_predator_energy(e10, n10, 0.5) == false);
    assert(std::abs(e10 - (100.5 / (1.0 + 0.5*0.00025))) < 1e-9);

    return 0;
}

// Write a standalone C++ function `simulateDecay` that performs a Monte Carlo simulation of a two-stage radioactive decay chain (parent → daughter → granddaughter) using a fixed time step. The function should accept the initial number of parent nuclei, initial number of daughter nuclei, decay constant for the parent, decay constant for the daughter, total simulation time, and time step size as parameters (all as `float` or `double`). It should simulate the decay processes repeatedly over each time step, returning a vector of strings (or a custom struct) that records at each time step (starting from t=0) the simulation time, current parent count, current daughter count, and current granddaughter count. The decay probability for a single nucleus in one time step is the decay constant multiplied by the time step (assume constants are per unit time and probabilities are small). For each time step, process parent decays first (each parent nucleus decays with probability λp·Δt), then daughter decays (each daughter nucleus decays with probability λd·Δt). Use `rand()` seeded with `srand(time(0))` once at the start of the simulation for reproducibility. The function must handle edge cases like zero initial counts, zero decay constants, and time steps that don’t evenly divide the total time (simulate until the final time is reached, using the provided step size as a fixed increment, and include the final time point even if it’s not an exact multiple). Return the recorded data as a `std::vector<std::array<float,4>>` or a vector of a simple struct.
The solution simulates the decay process using discrete time steps. The main algorithm: at each time step, we first compute the number of parent decays by iterating over each current parent nucleus and generating a random number between 0 and 1; if that number is less than or equal to the decay probability (λp·Δt), we decrement parent count and increment daughter count. Then, we do the same for daughter decays: iterate over the current daughter count (after parent decays), and for each, if a random number is ≤ λd·Δt, decrement daughter and increment granddaughter. After processing both stages, we advance time by Δt and record the state. This mirrors the given snippet but uses passed parameters instead of user input and returns data instead of writing to a file. Edge cases: if λp or λd are zero, the corresponding loop will never trigger; if initial counts are zero, the loops handle that naturally. The time stepping may produce a final t less than tf if tf is not a multiple of h; the loop condition should be while (t <= tf + 1e-9) to include the final time point. Since we process decay events sequentially within a step, we must store the current np and nd values and update them carefully—iterating over `j` from 0 to `np` while modifying `np` inside the loop can cause issues; the correct approach is to capture the initial count for that stage before the loop, or better, use a for loop that runs `np` times but since np changes, we use a local variable `currentParentCount = np; for (j=0; j<currentParentCount; j++)`. Similarly for daughters. This avoids the bug in the original snippet where the loop upper bound changes dynamically. Time complexity: O(number_of_steps × (initial parent + initial daughter)) in the worst case, but on average proportional to the total number of decays plus initial counts across steps. Space complexity: O(number_of_steps) for storing the output.
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Struct to hold simulation state at one time step
struct DecayState {
    float time;
    float parent;
    float daughter;
    float granddaughter;
};

// Simulate a two-stage radioactive decay chain using Monte Carlo method.
// Returns a vector of DecayState records for each time step from t=0 to tf.
std::vector<DecayState> simulateDecay(float initialParent, float initialDaughter,
                                      float lambdaParent, float lambdaDaughter,
                                      float finalTime, float timeStep) {
    std::vector<DecayState> results;
    
    // Initialize random seed once
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(0));
        seeded = true;
    }
    
    // Current state
    float parent = initialParent;
    float daughter = initialDaughter;
    float granddaughter = 0.0f;
    float t = 0.0f;
    
    // Record initial state
    results.push_back({t, parent, daughter, granddaughter});
    
    // Simulation loop: continue until we exceed finalTime
    while (t < finalTime + 1e-9f) {
        // Parent decay stage: each parent nucleus decays with probability lambdaParent * timeStep
        int currentParentCount = static_cast<int>(parent);
        for (int i = 0; i < currentParentCount; ++i) {
            float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            if (random <= lambdaParent * timeStep) {
                parent -= 1.0f;
                daughter += 1.0f;
            }
        }
        
        // Daughter decay stage: each daughter nucleus decays with probability lambdaDaughter * timeStep
        int currentDaughterCount = static_cast<int>(daughter);
        for (int i = 0; i < currentDaughterCount; ++i) {
            float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
            if (random <= lambdaDaughter * timeStep) {
                daughter -= 1.0f;
                granddaughter += 1.0f;
            }
        }
        
        // Advance time
        t += timeStep;
        
        // Record state at this time step
        results.push_back({t, parent, daughter, granddaughter});
    }
    
    return results;
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution code is included above

int main() {
    // Test 1: Zero decay constants, no changes
    auto result1 = simulateDecay(100, 50, 0.0f, 0.0f, 2.0f, 1.0f);
    assert(result1.size() == 3); // t=0, t=1, t=2
    assert(result1[0].parent == 100 && result1[0].daughter == 50 && result1[0].granddaughter == 0);
    assert(result1[1].parent == 100 && result1[1].daughter == 50);
    assert(result1[2].parent == 100 && result1[2].daughter == 50);
    
    // Test 2: Zero initial counts, nothing to decay
    auto result2 = simulateDecay(0, 0, 0.5f, 0.5f, 1.0f, 0.5f);
    assert(result2.size() == 3); // t=0, t=0.5, t=1.0
    for (const auto& state : result2) {
        assert(state.parent == 0 && state.daughter == 0 && state.granddaughter == 0);
    }
    
    // Test 3: Very high decay probability (lambda*dt > 1), all decays happen
    auto result3 = simulateDecay(5, 0, 2.0f, 2.0f, 1.0f, 1.0f);
    // At t=0: parent=5, daughter=0, granddaughter=0
    // At t=1: all parents decay to daughters (p=0,d=5), then all daughters decay (p=0,d=0,g=5)
    assert(result3.size() == 2);
    assert(result3[1].parent == 0);
    assert(result3[1].daughter == 0);
    assert(result3[1].granddaughter == 5);
    
    // Test 4: Final time not a multiple of time step
    auto result4 = simulateDecay(10, 0, 0.0f, 0.0f, 2.5f, 1.0f);
    // t=0,1,2,3 (since 2.5 <= 3, but loop stops when t > finalTime+eps)
    // Actually loop: t=0 → record, then while t<2.5: t=1, record; t=2, record; t=3, record; then t=4 > 2.5 so stop
    // So we expect t values: 0,1,2,3
    assert(result4.size() == 4);
    assert(std::fabs(result4[0].time - 0.0f) < 1e-6);
    assert(std::fabs(result4[3].time - 3.0f) < 1e-6);
    
    // Test 5: Single step with one nucleus decaying with probability 1
    auto result5 = simulateDecay(1, 0, 1.0f, 0.0f, 1.0f, 1.0f);
    assert(result5.size() == 2);
    assert(result5[1].parent == 0);
    assert(result5[1].daughter == 1);
    assert(result5[1].granddaughter == 0);
    
    // Test 6: Negative time step should be handled gracefully? Our function doesn't validate, but test with positive
    // Just ensure no crash for typical values
    auto result6 = simulateDecay(1000, 1000, 0.1f, 0.05f, 10.0f, 0.5f);
    assert(result6.size() == 21); // t=0 to t=10 with 0.5 step
    // Ensure counts never go negative (property check)
    for (const auto& state : result6) {
        assert(state.parent >= -0.01f); // allow float rounding
        assert(state.daughter >= -0.01f);
        assert(state.granddaughter >= -0.01f);
    }
    
    return 0;
}

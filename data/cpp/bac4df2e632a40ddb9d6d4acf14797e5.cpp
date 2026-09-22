// Write a C++ function named `montyHallWinProbabilities` that takes two integer parameters: `doors`, the number of doors in the Monty Hall problem (assumed to be at least 2), and `simulations`, the number of random trials to run (assumed to be at least 1). The function should simulate the classic Monty Hall scenario for both strategies—*staying with the initial choice* and *switching after a host opens a losing door*—and return a `std::pair<double, double>` containing the winning probabilities (as percentages, between 0 and 100) for the "no-switch" strategy first and the "switch" strategy second. Use a uniform random number generator. For each trial, randomly place a prize behind one door, randomly choose an initial door for the player, then for the "switch" strategy, have the host open one of the remaining doors that does not hide the prize (the host must open a door that is not the player's initial choice and not the prize door), and the player switches to the other unopened door. For the "no-switch" strategy, simply check if the initial choice matched the prize. The function must be deterministic in behavior given the same seed, so include a seed parameter (integer) as the third argument, and use it to initialize the random generator. The function should not print anything.

The core idea is to run independent simulations for each strategy, using the same underlying random process for door selection. The algorithm for one trial:  
1. Place the prize at a random door `target = rand() % doors`.  
2. Player initially chooses random door `choose = rand() % doors`.  
3. For "no switch": win if `choose == target`.  
4. For "switch": The host opens a door `open` that is not `choose` and not `target`. Since there are at least 2 doors, such a door always exists (if `doors == 2` and `choose != target`, the only remaining door is the prize, so the host opens the other door; if `choose == target`, the host can open any of the other doors). After that, the player switches to the remaining unopened door `switchChoice` which is the only door not equal to `choose` and not equal to `open`. Win if `switchChoice == target`. Because of the logic, when `choose != target`, the host is forced to open the only other non-prize door, so `switchChoice` becomes the prize door, guaranteeing a win. When `choose == target`, the host opens a losing door and switching leads to a loss. Thus, the switch strategy wins whenever the initial choice was wrong, which has probability `(doors-1)/doors`, but the simulation counts it directly. After running all trials, compute the win count divided by simulations times 100. Edge cases: `doors` at least 2, `simulations` positive, and the random generator must be seeded per call to avoid repeating identical sequences (by seeding with the provided integer). Time complexity: O(simulations) for each strategy (or combined O(simulations) if we run both in the same loop, but the specification says each independently—so it's actually O(2*simulations) = O(simulations) asymptotically). Space complexity: O(1) auxiliary.

#include <random>
#include <utility>

// Simulate the Monty Hall problem for a given number of doors and trials.
// Returns a pair of winning probabilities (in percent) for "no switch" and "switch".
// The behavior is deterministic for a given seed.
std::pair<double, double> montyHallWinProbabilities(int doors, int simulations, int seed) {
    // Validate inputs (assuming positive integers as per task description).
    if (doors < 2 || simulations < 1) {
        return {0.0, 0.0}; // Or could throw, but we handle gracefully.
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, doors - 1);

    int winsNoSwitch = 0;
    int winsSwitch = 0;

    for (int i = 0; i < simulations; ++i) {
        int prizeDoor = dist(rng);          // Door containing the prize
        int initialChoice = dist(rng);      // Player's initial choice

        // No switch strategy: win if initial choice is prize door.
        if (initialChoice == prizeDoor) {
            ++winsNoSwitch;
        }

        // Switch strategy: host opens a losing door (not prize, not initial).
        int openedDoor;
        do {
            openedDoor = dist(rng);
        } while (openedDoor == prizeDoor || openedDoor == initialChoice);

        // The player switches to the only remaining unopened door.
        int switchChoice = -1;
        for (int d = 0; d < doors; ++d) {
            if (d != initialChoice && d != openedDoor) {
                switchChoice = d;
                break;
            }
        }

        if (switchChoice == prizeDoor) {
            ++winsSwitch;
        }
    }

    double probNoSwitch = (100.0 * winsNoSwitch) / simulations;
    double probSwitch   = (100.0 * winsSwitch)   / simulations;
    return {probNoSwitch, probSwitch};
}

#include <cassert>
#include <cmath>

int main() {
    // Test with 3 doors and a large number of simulations for statistical stability.
    auto result3 = montyHallWinProbabilities(3, 100000, 12345);
    // Expected theoretical: no-switch ~33.33%, switch ~66.67%.
    assert(std::abs(result3.first - 33.33) < 1.0);
    assert(std::abs(result3.second - 66.67) < 1.0);

    // Test with 3 doors and deterministic small seed: probabilities exactly computable.
    auto resultSmall = montyHallWinProbabilities(3, 10, 42);
    // Since we can't know exact random sequence, just verify output is in [0,100].
    assert(resultSmall.first >= 0.0 && resultSmall.first <= 100.0);
    assert(resultSmall.second >= 0.0 && resultSmall.second <= 100.0);

    // Test with 2 doors: switching wins when initial choice is wrong (prob 1/2).
    auto result2 = montyHallWinProbabilities(2, 100000, 777);
    assert(std::abs(result2.first - 50.0) < 1.0);
    assert(std::abs(result2.second - 50.0) < 1.0);

    // Test that the same seed produces identical results.
    auto r1 = montyHallWinProbabilities(10, 5000, 99);
    auto r2 = montyHallWinProbabilities(10, 5000, 99);
    assert(r1.first == r2.first && r1.second == r2.second);

    // Test with a single simulation: both probabilities are 0% or 100%.
    auto one = montyHallWinProbabilities(5, 1, 1);
    assert((one.first == 0.0 || one.first == 100.0));
    assert((one.second == 0.0 || one.second == 100.0));

    // Test with many doors: switch strategy should approach (doors-1)/doors.
    auto resultMany = montyHallWinProbabilities(100, 200000, 2024);
    // For 100 doors, no-switch ~1%, switch ~99%.
    assert(std::abs(resultMany.first - 1.0) < 0.5);
    assert(std::abs(resultMany.second - 99.0) < 0.5);

    // Test with minimum allowable doors=2, simulations=1.
    auto minCase = montyHallWinProbabilities(2, 1, 55);
    assert(minCase.first >= 0.0 && minCase.first <= 100.0);
    assert(minCase.second >= 0.0 && minCase.second <= 100.0);
}

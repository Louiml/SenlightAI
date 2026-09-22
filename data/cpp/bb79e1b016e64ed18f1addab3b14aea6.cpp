Write a C++ function named `simulateGameStates` that takes a non-negative integer `seed`, a `uint32_t` starting time, an `unsigned int` numberOfBonuses, a `float` huntOnPlayerTimeLength, a `float` neutralTimeLength, a `float` huntOnGhostsTimeLength, a `float` ghostSpeedCoeficient, an `unsigned int` probabilityOfMovingBonus, and an `unsigned int` gameMode. The function must simulate the game logic of `CGameInfo` class for exactly 20 consecutive `updateTime` calls, each advancing time by exactly 1000 milliseconds. After each update, the function must record the current game state and the current score. The function should return a `std::string` that concatenates each state (as an integer from 0 to 6, corresponding to `GAMESTATE_HUNT_ON_PLAYER=0`, `GAMESTATE_NEUTRAL=1`, `GAMESTATE_HUNT_ON_GHOSTS=2`, `GAMESTATE_LOST_LIFE=3`, `GAMESTATE_VICTORY=4`, `GAMESTATE_LOST=5`, and `GAMESTATE_INIT=6`) followed by a colon and the score, with each pair separated by a semicolon. The simulation must start in `GAMESTATE_HUNT_ON_PLAYER` (state 0) with score 0, level 1, lives = 3 (`NUMBER_OF_LIVES`), and `numberOfPickedBonuses_ = 0`. The `timeOfLastChange_` is initialized to the starting time. The function must replicate the exact behavior of the `updateTime` method, including the state transitions with timing thresholds multiplied by 1000 (converting seconds to milliseconds), the `calcNeutralTimeLength` formula using level_ = 1 and the given neutralTimeLength, and the `rand()` usage for `nextMovingBonusIn_`. Since no bonuses are picked during this simulation, the initial state will transition to neutral and then back, and so on. Ignore any randomness for moving bonuses (they won't be generated because `movingBonusGenerated_` starts false and `generateMovingBonus` is not called). The function must produce a string with exactly 20 entries.

#include <cassert>
#include <string>

// Function declaration (from solution)
std::string simulateGameStates(
    unsigned int seed,
    uint32_t startTime,
    unsigned int numberOfBonuses,
    float huntOnPlayerTimeLength,
    float neutralTimeLength,
    float huntOnGhostsTimeLength,
    float ghostSpeedCoeficient,
    unsigned int probabilityOfMovingBonus,
    unsigned int gameMode);

int main() {
    // Case 1: Very short huntOnPlayer time (0.5s) and neutral time (0.5s)
    // Starting state 0, after 1000ms (1s) since 0.5s passed, switch to 1.
    // Then after 1000ms from change, neutral threshold = 1000*0.5*0.9=450, clamped to 1000.
    // So from change at t=1000, at t=2000 timeFromLastChange=1000, not >1000, so stays in 1.
    // Actually need >1000, so at t=3000 it switches back to 0.
    // Pattern: 0,1,1,1,0,1,1,1,... after first transition.
    std::string r1 = simulateGameStates(1, 0, 10, 0.5f, 0.5f, 1.0f, 1.0f, 50, 0);
    // Let's compute expected manually:
    // t=1000 -> state 0->1
    // t=2000 -> state 1, timeFromLastChange=1000, not >1000 -> still 1
    // t=3000 -> state 1, timeFromLastChange=2000 >1000 -> switch to 0
    // t=4000 -> state 0, timeFromLastChange=1000 >500 -> switch to 1
    // t=5000 -> state 1, timeFromLastChange=1000 not >1000 -> still 1
    // t=6000 -> state 1, timeFromLastChange=2000 >1000 -> switch to 0
    // ... pattern: states: 0(1),1(2),1(3),0(4),1(5),1(6),0(7),1(8),1(9),0(10),1(11),1(12),0(13),1(14),1(15),0(16),1(17),1(18),0(19),1(20)
    // Scores all 0.
    std::string expected1 = "0:0;1:0;1:0;0:0;1:0;1:0;0:0;1:0;1:0;0:0;1:0;1:0;0:0;1:0;1:0;0:0;1:0;1:0;0:0;1:0;";
    assert(r1 == expected1);

    // Case 2: Long huntOnPlayer time (100s) and long neutral time (100s)
    // At 1000ms, timeFromLastChange=1000, threshold=100000, so stays in state 0.
    // All states stay 0.
    std::string r2 = simulateGameStates(2, 0, 10, 100.0f, 100.0f, 1.0f, 1.0f, 50, 0);
    std::string expected2 = "";
    for (int i = 0; i < 20; ++i) {
        expected2 += "0:0;";
    }
    assert(r2 == expected2);

    // Case 3: Exactly threshold condition (huntOnPlayerTime=1.0s)
    // At t=1000, timeFromLastChange=1000, condition is >1000 (strict), so not satisfied.
    // Remains 0 for all 20 steps.
    std::string r3 = simulateGameStates(3, 0, 10, 1.0f, 1.0f, 1.0f, 1.0f, 50, 0);
    std::string expected3 = "";
    for (int i = 0; i < 20; ++i) {
        expected3 += "0:0;";
    }
    assert(r3 == expected3);

    // Case 4: huntOnPlayerTime=0.1s, neutralTimeLength=0.1s
    // At t=1000, switch to 1. Then neutral threshold: coef=0.9, 1000*0.1*0.9=90, clamped to 1000.
    // At t=2000, diff from change=1000, not >1000 stay 1. At t=3000, diff=2000 >1000 switch to 0.
    // Pattern same as case 1, but with different initial timing? Actually same because thresholds clamp to 1000.
    std::string r4 = simulateGameStates(4, 0, 10, 0.1f, 0.1f, 1.0f, 1.0f, 50, 0);
    assert(r4 == expected1); // same pattern as case 1

    // Case 5: Start time non-zero, e.g., 5000.
    // Same behavior because time differences are based on start, so pattern identical.
    std::string r5 = simulateGameStates(5, 5000, 10, 0.5f, 0.5f, 1.0f, 1.0f, 50, 0);
    assert(r5 == expected1);

    return 0;
}

#include <string>
#include <cstdint>
#include <cstdlib>

// Simulates the CGameInfo state machine for 20 time steps of 1000 ms each.
// Returns a semicolon-separated string of "state:score" pairs.
std::string simulateGameStates(
    unsigned int seed,
    uint32_t startTime,
    unsigned int numberOfBonuses,
    float huntOnPlayerTimeLength,
    float neutralTimeLength,
    float huntOnGhostsTimeLength,
    float ghostSpeedCoeficient,
    unsigned int probabilityOfMovingBonus,
    unsigned int gameMode)
{
    // Ignore unused parameters for this simulation.
    (void)seed;
    (void)numberOfBonuses;
    (void)huntOnGhostsTimeLength;
    (void)ghostSpeedCoeficient;
    (void)probabilityOfMovingBonus;
    (void)gameMode;

    // Constants from original code.
    const unsigned int NUMBER_OF_LIVES = 3;
    const unsigned int LOST_LIFE_DURATION = 3; // seconds, not used here
    const unsigned int WIN_DURATION = 5; // seconds, not used here

    // State enumerations matching original: 0=HUNT_ON_PLAYER, 1=NEUTRAL, ...
    unsigned int currentState = 0;
    unsigned int level = 1;
    unsigned int lives = NUMBER_OF_LIVES;
    unsigned int score = 0;
    unsigned int numberOfPickedBonuses = 0;
    uint32_t timeOfLastChange = startTime;
    uint32_t time = startTime;

    std::string result;

    // Seed rand() as in original constructor (for consistency, though not used).
    std::srand(startTime);

    // Simulate exactly 20 updates, each advancing time by 1000 ms.
    for (int step = 0; step < 20; ++step) {
        time += 1000;
        uint32_t timeFromLastChange = time - timeOfLastChange;

        // Update state machine (simplified: only states 0 and 1 reachable).
        if (currentState == 0) { // HUNT_ON_PLAYER
            if (timeFromLastChange > static_cast<uint32_t>(1000 * huntOnPlayerTimeLength)) {
                timeOfLastChange = time;
                currentState = 1; // NEUTRAL
            }
        } else if (currentState == 1) { // NEUTRAL
            // compute neutral time length using level = 1
            float coef = (-1.0f / 10.0f) * static_cast<float>(level) + 1.0f;
            if (coef < 0.1f) {
                coef = 0.1f;
            }
            int timeThreshold = static_cast<int>(1000.0f * neutralTimeLength * coef);
            if (timeThreshold < 1000) {
                timeThreshold = 1000;
            }
            if (timeFromLastChange > static_cast<uint32_t>(timeThreshold)) {
                timeOfLastChange = time;
                currentState = 0; // HUNT_ON_PLAYER
            }
        }
        // All other states would only change if events occurred; none occur here.

        // Append "state:score" to result.
        result += std::to_string(currentState) + ":" + std::to_string(score) + ";";
    }

    return result;
}

// The solution simulates a simplified game-state machine. The main algorithm:
// 1. Initialize a `CGameInfo`-like structure with the given parameters: state = 0 (HUNT_ON_PLAYER), score = 0, level = 1, lives = 3, timeOfLastChange_ = startTime, time_ = startTime.
// 2. For 20 iterations, add 1000 to `time_` (since each update advances 1000 ms).
// 3. Compute `timeFromLastChange = time_ - timeOfLastChange_`.
// 4. Apply the same switch logic as in the original `updateTime`:
//    - State 0: if `timeFromLastChange > 1000 * huntOnPlayerTimeLength`, set `timeOfLastChange_ = time_` and change state to 1.
//    - State 1: compute `coef = (-1.0f/10.0f) * level_ + 1.0f`; clamp to >= 0.1; `timeThreshold = 1000.0f * neutralTimeLength * coef`; clamp to >= 1000; if `timeFromLastChange > timeThreshold`, change state to 0.
//    - Other states: not reachable because no events occur, so break.
// 5. After updating state, append the current state (as integer) and score to the output string with a colon between and a semicolon after each pair (including the last).
// 6. Since `time_` is `uint32_t`, the start time should be small enough (e.g., 0) to avoid overflow within 20000 ms.
//
// Edge cases:
// - `neutralTimeLength` could be 0, but the clamp ensures threshold >= 1000.
// - Level is constant at 1, so coef = 0.9.
// - The initial state is 0, and first update at time+1000; if huntOnPlayerTimeLength is very small (e.g., 0.1), the condition `1000 > 100` triggers, so it switches to neutral. Then in the next update, it might switch back after 1000 > threshold (~900). So the pattern alternates.
// - Scores remain 0 throughout because no bonuses are picked.
// - `probabilityOfMovingBonus` is ignored; we just seed `rand()` with the starting time as in the original constructor for consistency, but since no call to `calcTimeToMovingBonus` affects state, it's irrelevant for the result.
// - The function must be `const`-correct for the struct but not for simulation.
//
// Time complexity: O(20) = O(1). Space complexity: O(1) for the simulation, plus O(1) for the output string (fixed length).

/*
Write a C++ function that simulates the core odometer logic from the given SystemC snippet: given a boolean `start` signal, a boolean `reset` signal, and an integer `num_pulses` (representing `NUM_PULSES_FOR_DIST_INCR`, which is at least 1), the function should track total and partial distances as `double` values in units of distance increments (each pulse group counts as 1 increment). The function processes a finite sequence of clock cycles: for each cycle, it receives the current `start` and `reset` signals (booleans). The total distance accumulates by 1 per cycle when `start` is true, else resets to 0. The partial distance accumulates by 1 per cycle when `start` is true, but resets to 0 whenever `reset` transitions (i.e., changes its boolean value from previous cycle) while `start` is true; if `start` is false, both distances reset to 0. Additionally, there is a pulse collection mechanism: only after every `num_pulses` received pulses (each pulse is like a "tick" that occurs before the cycle's start/reset logic) does the distance computation proceed for that cycle; otherwise, the distances do not change. However, since the snippet uses `AWAIT(NUM_PULSES_FOR_DIST_INCR)` which waits for that many pulses, we simplify: the function will receive a vector of pairs representing each cycle: `(pulse_count, start, reset)` where `pulse_count` is the number of pulses in that cycle (can be 0 or more, but for simplicity assume it's always 1 if you want exact behavior). Actually, to make it independent, we'll define: the function takes a vector of cycles, each cycle provides `start` and `reset` booleans, and a global pulse counter increments by 1 per cycle; when the pulse counter reaches `num_pulses`, we process that cycle's start/reset logic as above, then reset pulse counter to 0. Cycles that don't reach the pulse threshold are ignored. The function should return a struct `OdometerResult` containing `total_dist` and `partial_dist` as doubles, after processing all cycles. If `num_pulses` is 0 or negative, treat it as 1 (always process every cycle). The reset transition detection uses the previous cycle's reset value (initialize previous reset to false before first cycle). Edge cases: empty input vector results in zero distances; `start` false at any cycle resets both to 0 immediately (even if pulse threshold not met? No, only when threshold met, else distance unchanged). Provide the function with a descriptive name.
*/

#include <vector>
#include <cstddef>

struct OdometerResult {
    double total_dist;
    double partial_dist;
};

// Process a series of cycles with pulse counting and start/reset signals.
OdometerResult computeDistances(const std::vector<std::pair<bool, bool>>& cycles, int num_pulses) {
    if (num_pulses < 1) num_pulses = 1; // clamp invalid values

    double total = 0.0;
    double partial = 0.0;
    int pulse_count = 0;
    bool prev_reset = false; // initial previous reset as false

    for (const auto& cycle : cycles) {
        bool start = cycle.first;
        bool reset = cycle.second;

        ++pulse_count;
        if (pulse_count < num_pulses) {
            continue; // not enough pulses yet, ignore this cycle
        }
        // Pulse threshold met, process this cycle
        pulse_count = 0; // reset counter for next group

        if (!start) {
            // Both distances reset when start is false
            total = 0.0;
            partial = 0.0;
            // Do not update prev_reset in this case per original logic
        } else {
            // start is true
            total += 1.0; // each increment counts as 1

            // Partial distance: check reset transition
            if (prev_reset != reset) {
                partial = 0.0; // reset occurred
            } else {
                partial += 1.0;
            }
            prev_reset = reset; // update for next processed cycle
        }
    }

    return {total, partial};
}

#include <cassert>
#include <vector>
#include <utility>

// Solution function declaration (assume it is defined above or included)
// OdometerResult computeDistances(const std::vector<std::pair<bool, bool>>&, int);

int main() {
    // Test 1: Basic accumulation with num_pulses=1, no reset
    std::vector<std::pair<bool,bool>> c1 = {{true,false}, {true,false}, {true,false}};
    OdometerResult r1 = computeDistances(c1, 1);
    assert(r1.total_dist == 3.0);
    assert(r1.partial_dist == 3.0);

    // Test 2: num_pulses=2 means only every other cycle processes
    std::vector<std::pair<bool,bool>> c2 = {{true,false}, {true,false}, {true,false}, {true,false}};
    OdometerResult r2 = computeDistances(c2, 2);
    // Cycles 2 and 4 process (since pulse_count reaches 2), each adds 1
    assert(r2.total_dist == 2.0);
    assert(r2.partial_dist == 2.0);

    // Test 3: Reset transition resets partial but not total
    std::vector<std::pair<bool,bool>> c3 = {{true,false}, {true,false}, {true,true}, {true,false}};
    OdometerResult r3 = computeDistances(c3, 1);
    // total: 1+1+1+1=4
    // partial: cycle1 (prev false==false) ->1, cycle2 ->2, cycle3 (prev false != true) ->0, cycle4 (prev true != false) ->0? Actually prev updated to true at cycle3, then cycle4 reset false differs -> partial=0
    assert(r3.total_dist == 4.0);
    assert(r3.partial_dist == 0.0);

    // Test 4: start false resets both
    std::vector<std::pair<bool,bool>> c4 = {{true,false}, {false,false}, {true,false}};
    OdometerResult r4 = computeDistances(c4, 1);
    // cycle1: total=1, partial=1 (prev false==false)
    // cycle2: start false -> both 0
    // cycle3: start true, prev_reset still false (not updated on false start), reset false==false -> partial=1
    assert(r4.total_dist == 1.0);
    assert(r4.partial_dist == 1.0);

    // Test 5: Empty input
    std::vector<std::pair<bool,bool>> c5;
    OdometerResult r5 = computeDistances(c5, 3);
    assert(r5.total_dist == 0.0);
    assert(r5.partial_dist == 0.0);

    // Test 6: num_pulses invalid (0 or negative) treated as 1
    std::vector<std::pair<bool,bool>> c6 = {{true,false}, {true,false}};
    OdometerResult r6a = computeDistances(c6, 0);
    OdometerResult r6b = computeDistances(c6, -5);
    assert(r6a.total_dist == 2.0 && r6a.partial_dist == 2.0);
    assert(r6b.total_dist == 2.0 && r6b.partial_dist == 2.0);

    // Test 7: num_pulses larger than number of cycles -> no processing
    std::vector<std::pair<bool,bool>> c7 = {{true,false}, {true,false}};
    OdometerResult r7 = computeDistances(c7, 3);
    assert(r7.total_dist == 0.0);
    assert(r7.partial_dist == 0.0);

    // Test 8: Reset transition on first processed cycle (initial prev_reset false)
    std::vector<std::pair<bool,bool>> c8 = {{true,true}};
    OdometerResult r8 = computeDistances(c8, 1);
    // prev false != true -> partial resets to 0, total=1
    assert(r8.total_dist == 1.0);
    assert(r8.partial_dist == 0.0);

    return 0;
}

// The solution processes the input sequence cycle by cycle while maintaining a pulse counter. For each cycle, we increment the pulse counter. Only when the pulse counter reaches `num_pulses` (or is always treated as reached if `num_pulses ≤ 1` after adjustment) do we apply the distance update logic. When applying, we first check the `start` flag: if `start` is false, both total and partial distances are set to 0. If `start` is true, total distance increments by 1.0, and for partial distance we check the reset transition: compare current `reset` with previous reset (stored from last applied cycle; initialize to false). If they differ, partial resets to 0, else partial increments by 1.0. Then store current reset as previous for next applied cycle. After applying, reset the pulse counter to 0. Cycles where pulse counter does not reach threshold do not change any state, including the previous reset value (because the original code's `AWAIT` only processes when enough pulses occur, and the reset detection uses the last processed cycle's reset). Important edge cases: `num_pulses` less than 1 needs to be clamped to 1. Empty input: no cycles, so distances remain 0. If `start` is false on a cycle that triggers processing, both distances reset to 0, and we should update previous reset? In the original code, when start is false, it sets `partial_compute_dist = 0.0` and doesn't touch `prev_reset`. So we should not update previous reset on a false-start cycle. Also when start is true and reset transitions, partial resets to 0, and prev_reset is updated. Also when start is true and no reset transition, partial increments. Time complexity is O(n) for n cycles, space O(1) beyond input.

Implement a C++ function that simulates the core logic of a firmware-based filament retraction system. The function should manage retraction state for multiple extruders, applying retract and recover moves while tracking which extruders are currently retracted and managing any "swap retract" state. The function must accept: an array of retraction lengths for each extruder, a retraction Z-hop height, a retraction feedrate, a recover feedrate, an optional swap retraction length, an optional swap recover extra length, and a boolean indicating whether this is a retract (true) or recover (false). It must also accept an active extruder index and a swap flag (whether this is a swap retract/recover operation). The function should prevent double operations (e.g., retracting an already-retracted extruder) and, on recover, automatically clear the swap state if it was set. The function should update an output state structure that tracks: which extruders are retracted, which are swap-retracted, the current retract amount per extruder, and any accumulated Z-hop, and it should return a sequence of moves (e.g., a vector of strings) representing the deltas: a Z raise (on retract) or Z lower (on recover), then an E move for the retract/recover amount, and possibly an extra E recovery if configured. The function must be standalone, use standard C++ only, and handle edge cases like zero Z-hop, zero extra recovery, invalid active extruder index, and duplicate operations.
// The solution models the firmware logic by maintaining four state arrays (retracted, retracted_swap, current_retract, and a scalar current_hop) and applying the retract/recover logic step by step. The main algorithm is: on a retract request, first check if the extruder is already retracted (for the same operation type) and if so, return early; if this is a swap retract, also check the swap-retracted flag for duplicate prevention. Then, compute the base retract length (either the normal or swap length). If retracting: set the current retract length for the active extruder, record an E move equal to that length (as a negative delta, since retracting withdraws filament), and if the Z-hop is nonzero and not already hopped, record a Z upward move equal to the hop and update current_hop. If recovering: first, if there is a current_hop, record a Z downward move equal to the hop and reset it to zero; then, if there is an extra recovery length (normal or swap-specific), record an additional E move of that length; finally, record an E move to push filament back the base retract length (positive delta), and reset the current retract length to zero. After the operation, update the retracted flags and, if it was a swap operation, update the swap-retracted flag. Edge cases include: if the active extruder index is out of bounds, return with no moves; if the base retract length is zero, still update state but record no E move (or record zero); if a recover is requested while the extruder is not retracted, ignore it (no-op). Time complexity is O(1) for the operation itself, plus O(n) to construct the output vector of strings (where n is the number of moves, at most 3). Space complexity is O(1) for state plus O(n) for the returned vector.
#include <vector>
#include <string>
#include <cmath>

// State structure for the retraction system
struct RetractState {
    bool retracted[16];       // which extruders are currently retracted
    bool retracted_swap[16];  // which extruders are swap-retracted
    float current_retract[16]; // current retract amount per extruder
    float current_hop;         // active Z hop amount (0 if none)
};

/**
 * Simulate firmware-based retract/recover for a single extruder.
 *
 * @param state          Output state to update (in/out)
 * @param retracting     true for retract, false for recover
 * @param active_extruder Index of the active extruder (0-based)
 * @param swapping       true for swap retract/recover (used only for state and lengths)
 * @param retract_lengths    Base retract lengths per extruder (mm)
 * @param retract_zraise     Z hop height (mm, 0 for none)
 * @param retract_feedrate   Feedrate for retract (mm/s, used only for move label)
 * @param recover_feedrate   Feedrate for recover (mm/s, used only for move label)
 * @param swap_retract_length Swap retract length (mm)
 * @param swap_recover_extra  Extra recover length on swap (mm)
 * @param recover_extra       Extra recover length on normal recover (mm)
 *
 * @return Vector of move descriptions (e.g., "Z raise 2.5", "E retract -3.0")
 */
std::vector<std::string> firmware_retract(
    RetractState& state,
    bool retracting,
    int active_extruder,
    bool swapping,
    const float* retract_lengths,
    float retract_zraise,
    float retract_feedrate,
    float recover_feedrate,
    float swap_retract_length,
    float swap_recover_extra,
    float recover_extra
) {
    std::vector<std::string> moves;
    const int MAX_EXTRUDERS = 16;
    if (active_extruder < 0 || active_extruder >= MAX_EXTRUDERS) {
        return moves; // invalid extruder, no-op
    }

    // Prevent duplicate operations: if already in the target state, ignore
    if (state.retracted[active_extruder] == retracting) {
        return moves;
    }
    // For swap operations, also check the swap-specific flag to prevent duplicates
    if (swapping && state.retracted_swap[active_extruder] == retracting) {
        return moves;
    }
    // On recover, if this extruder was swap-retracted, treat as swap recover
    if (!retracting && state.retracted_swap[active_extruder]) {
        swapping = true;
    }

    // Determine base retract length
    float base_retract = swapping ? swap_retract_length : retract_lengths[active_extruder];

    if (retracting) {
        // Set the current retract length for this extruder
        state.current_retract[active_extruder] = base_retract;
        // Record the E retract move (negative delta, withdraw filament)
        if (std::abs(base_retract) > 0.0001f) {
            moves.push_back("E retract " + std::to_string(-base_retract));
        }
        // Apply Z hop if configured and not already hopped
        if (state.current_hop == 0.0f && retract_zraise > 0.01f) {
            state.current_hop = retract_zraise;
            moves.push_back("Z raise " + std::to_string(retract_zraise));
        }
    } else {
        // If there was a hop, undo it
        if (state.current_hop != 0.0f) {
            moves.push_back("Z lower " + std::to_string(state.current_hop));
            state.current_hop = 0.0f;
        }
        // Apply extra recovery if configured
        float extra = swapping ? swap_recover_extra : recover_extra;
        if (extra > 0.0f) {
            moves.push_back("E recover_extra " + std::to_string(extra));
        }
        // Record the main E recover move (positive delta, push filament back)
        if (std::abs(base_retract) > 0.0001f) {
            moves.push_back("E recover " + std::to_string(base_retract));
        }
        // Clear current retract length
        state.current_retract[active_extruder] = 0.0f;
    }

    // Update state flags
    state.retracted[active_extruder] = retracting;
    if (swapping) {
        state.retracted_swap[active_extruder] = retracting;
    }

    // (Ignoring feedrate in the move string for simplicity, but could be included)
    return moves;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <string>

// Include the solution function here (or link)

int main() {
    // Setup: 2 extruders, retract length 3.0 each
    float retract_lengths[2] = {3.0f, 3.0f};
    RetractState st = {}; // zero-initialized

    // Test 1: Basic retract on extruder 0
    auto moves = firmware_retract(st, true, 0, false,
        retract_lengths, 2.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves.size() == 2);
    assert(st.retracted[0] == true);
    assert(st.retracted[1] == false);
    assert(std::fabs(st.current_retract[0] - 3.0f) < 0.0001f);
    assert(std::fabs(st.current_hop - 2.0f) < 0.0001f);
    assert(moves[0] == "E retract -3.000000");
    assert(moves[1] == "Z raise 2.000000");

    // Test 2: Duplicate retract should be ignored
    auto moves_dup = firmware_retract(st, true, 0, false,
        retract_lengths, 2.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves_dup.empty());
    assert(st.retracted[0] == true);

    // Test 3: Recover on extruder 0 (undo hop, then recover E)
    auto moves_rec = firmware_retract(st, false, 0, false,
        retract_lengths, 2.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves_rec.size() == 2);
    assert(moves_rec[0] == "Z lower 2.000000");
    assert(moves_rec[1] == "E recover 3.000000");
    assert(st.retracted[0] == false);
    assert(std::fabs(st.current_retract[0] - 0.0f) < 0.0001f);
    assert(std::fabs(st.current_hop - 0.0f) < 0.0001f);

    // Test 4: Recover without having retracted should be ignored
    auto moves_norec = firmware_retract(st, false, 1, false,
        retract_lengths, 2.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves_norec.empty());
    assert(st.retracted[1] == false);

    // Test 5: Swap retract on extruder 1 (with swap length 5.0, extra recovery 1.5)
    auto moves_swap = firmware_retract(st, true, 1, true,
        retract_lengths, 0.0f, 50.0f, 30.0f, 5.0f, 1.5f, 0.0f);
    assert(moves_swap.size() == 1); // no hop because retract_zraise=0
    assert(moves_swap[0] == "E retract -5.000000");
    assert(st.retracted[1] == true);
    assert(st.retracted_swap[1] == true);
    assert(std::fabs(st.current_retract[1] - 5.0f) < 0.0001f);

    // Test 6: Swap recover (should clear swap flag and apply extra)
    auto moves_swap_rec = firmware_retract(st, false, 1, false,
        retract_lengths, 0.0f, 50.0f, 30.0f, 5.0f, 1.5f, 0.0f);
    // Since swap flag was set, it should recover as swap: extra + main
    assert(moves_swap_rec.size() == 2);
    assert(moves_swap_rec[0] == "E recover_extra 1.500000");
    assert(moves_swap_rec[1] == "E recover 5.000000");
    assert(st.retracted[1] == false);
    assert(st.retracted_swap[1] == false);

    // Test 7: Invalid extruder index (should no-op)
    auto moves_invalid = firmware_retract(st, true, 99, false,
        retract_lengths, 2.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves_invalid.empty());

    // Test 8: Zero retract length (should produce no E move but state updates)
    float zero_len[1] = {0.0f};
    RetractState st2 = {};
    auto moves_zero = firmware_retract(st2, true, 0, false,
        zero_len, 1.0f, 50.0f, 30.0f, 0.0f, 0.0f, 0.0f);
    assert(moves_zero.size() == 1); // only Z hop, no E move
    assert(moves_zero[0] == "Z raise 1.000000");
    assert(st2.retracted[0] == true);

    return 0;
}

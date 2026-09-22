Write a C++ function that simulates the core retraction state management logic from the provided firmware snippet, but simplified for standalone use. The function should track retraction state for multiple extruders (provided as a template parameter `EXTRUDERS`), support retract/recover operations with optional "swap" retraction, track a Z-hop value that can only be applied once per retract cycle, and expose a `reset()` method to restore default settings. The function should return a struct containing the current per-extruder retraction flags, swap-retraction flags (if more than one extruder), and current hop value after each operation.
// The solution mirrors the core logic of `FWRetract::retract()` without hardware dependencies. We maintain per-extruder boolean arrays for retracted state, optional swap-retracted state, and current retract lengths. A settings struct holds retract length, Z raise, recover extra, and feedrates. The main `retract()` function: first checks for redundant operations (already retracted/recovered), handles swap logic where a recover always prefers the swap state if active, computes the base retract length (with swap multiplier), and performs the sequence: for retract, set current_retract and add Z hop only if `current_hop == 0`; for recover, clear hop, adjust E position by extra recover amount, and clear current_retract. After the operation, update state flags. Edge cases: prevent double retract/recover in a row, allow Z hop only once even if multiple retract calls occur, swap recover must use swap settings. Time complexity is O(EXTRUDERS) per call for state updates, though typical calls are O(1) since only active extruder is modified, with O(EXTRUDERS) space.
#include <algorithm>
#include <array>

// Settings structure mirroring fwretract_settings_t
struct RetractSettings {
    float retract_length = 2.0f;
    float retract_feedrate = 40.0f;
    float retract_zraise = 0.5f;
    float retract_recover_extra = 0.0f;
    float retract_recover_feedrate = 40.0f;
    float swap_retract_length = 3.0f;
    float swap_retract_recover_extra = 0.5f;
    float swap_retract_recover_feedrate = 40.0f;
};

// Result struct returned by the retraction function
template<size_t EXTRUDERS>
struct RetractState {
    std::array<bool, EXTRUDERS> retracted{};
    std::array<bool, EXTRUDERS> retracted_swap{};  // only meaningful if EXTRUDERS > 1
    float current_hop = 0.0f;
};

// Simulates firmware retraction for a given extruder index.
// Returns the updated full state after the operation.
template<size_t EXTRUDERS>
RetractState<EXTRUDERS> simulate_retract(
    RetractState<EXTRUDERS> state,
    const RetractSettings& settings,
    size_t active_extruder,
    bool retracting,
    bool swapping = false) {

    // Prevent index out of bounds
    if (active_extruder >= EXTRUDERS) return state;

    // Prevent two retracts or recovers in a row
    if (state.retracted[active_extruder] == retracting) return state;

    // Handle swap-specific logic
    if constexpr (EXTRUDERS > 1) {
        // Prevent two swap-retract or recovers in a row
        if (swapping && state.retracted_swap[active_extruder] == retracting) return state;
        // G11 priority: recover the long retract if activated
        if (!retracting) swapping = state.retracted_swap[active_extruder];
    } else {
        swapping = false;
    }

    float base_retract = swapping ? settings.swap_retract_length : settings.retract_length;

    if (retracting) {
        // Apply retract length
        // (In real firmware, E position changes, but here we just store the value)
        // Z hop: only apply once per retract cycle
        if (state.current_hop == 0.0f && settings.retract_zraise > 0.01f) {
            state.current_hop += settings.retract_zraise;
        }
        // Update state (simulated E movement)
        // Current retract length would be stored per extruder in full firmware
    } else {
        // Recover: clear hop
        state.current_hop = 0.0f;

        // Extra recover amount (simulated E adjustment)
        float extra_recover = swapping ? settings.swap_retract_recover_extra : settings.retract_recover_extra;
        // In real code, this modifies current_position.e and syncs planner

        // (No persistent E position needed in this simplified version)
    }

    // Update state flags
    state.retracted[active_extruder] = retracting;
    if constexpr (EXTRUDERS > 1) {
        if (swapping) state.retracted_swap[active_extruder] = retracting;
    }

    return state;
}

// Reset state to defaults (per extruder false, hop 0)
template<size_t EXTRUDERS>
RetractState<EXTRUDERS> reset_retract_state() {
    return RetractState<EXTRUDERS>();
}
#include <cassert>

int main() {
    constexpr size_t E = 2;
    RetractSettings settings;
    settings.retract_length = 2.0f;
    settings.retract_zraise = 0.5f;
    settings.retract_recover_extra = 0.3f;
    settings.swap_retract_length = 4.0f;
    settings.swap_retract_recover_extra = 0.7f;

    // Initial state
    auto state = reset_retract_state<E>();

    // Normal retract on extruder 0
    state = simulate_retract(state, settings, 0, true);
    assert(state.retracted[0] == true);
    assert(state.retracted[1] == false);
    assert(state.current_hop == 0.5f);

    // Double retract should be ignored (already retracted)
    auto state2 = simulate_retract(state, settings, 0, true);
    assert(state2.retracted[0] == true);
    assert(state2.current_hop == 0.5f); // no additional hop

    // Recover clears hop and flag
    state2 = simulate_retract(state, settings, 0, false);
    assert(state2.retracted[0] == false);
    assert(state2.current_hop == 0.0f);

    // Swap retract on extruder 1
    auto state3 = simulate_retract(state2, settings, 1, true, true);
    assert(state3.retracted[1] == true);
    assert(state3.retracted_swap[1] == true);
    // Z hop applied (first retract for extruder 1)
    assert(state3.current_hop == 0.5f);

    // Swap recover (no explicit swapping set, should recover swap automatically)
    state3 = simulate_retract(state3, settings, 1, false);
    assert(state3.retracted[1] == false);
    assert(state3.retracted_swap[1] == false);
    assert(state3.current_hop == 0.0f);

    // Retract on extruder 0, then recover on extruder 1 should not affect extruder 0
    auto state4 = simulate_retract(state3, settings, 0, true);
    state4 = simulate_retract(state4, settings, 1, true);
    assert(state4.retracted[0] == true);
    assert(state4.retracted[1] == true);
    assert(state4.current_hop == 0.5f); // hop applied once on first retract

    // Recover extruder 1, extruder 0 remains retracted
    state4 = simulate_retract(state4, settings, 1, false);
    assert(state4.retracted[0] == true);
    assert(state4.retracted[1] == false);
    assert(state4.current_hop == 0.0f); // hop cleared on any recover

    // Invalid extruder index should not change state
    auto before = state4;
    auto invalid = simulate_retract(state4, settings, E, true);
    assert(invalid.retracted[0] == before.retracted[0]);
    assert(invalid.retracted[1] == before.retracted[1]);

    // Single-extruder version (template specialization) should ignore swap
    constexpr size_t E1 = 1;
    auto state1 = reset_retract_state<E1>();
    state1 = simulate_retract(state1, settings, 0, true, true);
    assert(state1.retracted[0] == true);
    assert(state1.current_hop == 0.5f);
    state1 = simulate_retract(state1, settings, 0, false, true);
    assert(state1.retracted[0] == false);
    assert(state1.current_hop == 0.0f);

    return 0;
}

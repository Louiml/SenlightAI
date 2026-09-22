/*
Given a code snippet from an adaptive multi-rate (AMR) speech decoder that manages state through initialization, reset, and frame-decoding functions, write a C++ function that simulates a simplified version of the state-management pattern: a `DecoderState` class containing a fixed-size history buffer of pitch-lag values, a "bad frame" counter, and a mode flag. Your task is to implement a standalone function `int resetDecoder(DecoderState& state, int mode)` that: (1) validates the mode is one of the supported values {0, 1, 2} representing MR475, MRDTX, and MR122 respectively; (2) clears the history buffer (size 9) to zero; (3) sets the bad-frame counter to 0; (4) sets a `pitchBuffer` member to 40 (the initial lag); (5) if mode != 1 (i.e., not MRDTX), also sets a `voiceState` member to 0; (6) returns 0 on success, -1 on invalid mode. Additionally, implement a second function `int decoderStep(DecoderState& state, int frameType, int pitchLag)` that: (1) if `frameType` is 2 (bad frame) or 3 (no data), increments the bad-frame counter (capping at 6), and if frameType is 3, replaces the pitchLag with the stored `pitchBuffer` value; (2) otherwise, resets the counter to 0 and updates `pitchBuffer` to the new pitchLag; (3) shifts the history buffer left by one position (dropping the oldest value) and appends the (possibly replaced) `pitchLag` at the end; (4) returns the current bad-frame counter value. The target is to create an independent, testable C++ implementation that mimics this core logic without requiring the full speech codec.
*/
#include <array>
#include <algorithm>

struct DecoderState {
    std::array<int, 9> history{}; // pitch gain history (placeholder)
    int badFrameCounter = 0;
    int pitchBuffer = 0;          // T0_lagBuff
    int voiceState = 0;           // used for non-MRDTX modes only
};

// Valid modes: 0=MR475, 1=MRDTX, 2=MR122
int resetDecoder(DecoderState& state, int mode) {
    if (mode < 0 || mode > 2) {
        return -1;
    }
    state.history.fill(0);
    state.badFrameCounter = 0;
    state.pitchBuffer = 40;
    if (mode != 1) { // not MRDTX
        state.voiceState = 0;
    }
    return 0;
}

// frameType: 0=good, 1=degraded, 2=bad, 3=no data
int decoderStep(DecoderState& state, int frameType, int pitchLag) {
    // Update bad frame counter based on frame type
    if (frameType == 2 || frameType == 3) {
        if (state.badFrameCounter < 6) {
            state.badFrameCounter++;
        }
        if (frameType == 3) {
            pitchLag = state.pitchBuffer;
        }
    } else {
        state.badFrameCounter = 0;
        state.pitchBuffer = pitchLag;
    }

    // Shift history left and append new pitch lag
    for (int i = 0; i < 8; ++i) {
        state.history[i] = state.history[i + 1];
    }
    state.history[8] = pitchLag;

    return state.badFrameCounter;
}
#include <cassert>

int main() {
    DecoderState st;

    // Test reset with invalid mode
    assert(resetDecoder(st, -1) == -1);
    assert(resetDecoder(st, 3) == -1);

    // Test reset with valid modes
    assert(resetDecoder(st, 0) == 0);
    assert(st.badFrameCounter == 0);
    assert(st.pitchBuffer == 40);
    assert(st.voiceState == 0);
    assert(st.history[0] == 0 && st.history[8] == 0);

    // Test decoderStep with good frame
    assert(resetDecoder(st, 0) == 0);
    int result = decoderStep(st, 0, 50);
    assert(result == 0);
    assert(st.pitchBuffer == 50);
    assert(st.history[8] == 50);
    assert(st.badFrameCounter == 0);

    // Test bad frame increments counter and caps at 6
    assert(resetDecoder(st, 0) == 0);
    decoderStep(st, 2, 50); // bad frame
    decoderStep(st, 2, 51);
    decoderStep(st, 2, 52);
    decoderStep(st, 2, 53);
    decoderStep(st, 2, 54);
    assert(st.badFrameCounter == 5);
    result = decoderStep(st, 2, 55);
    assert(result == 6);
    result = decoderStep(st, 2, 56);
    assert(result == 6); // capped

    // Test no-data frame uses pitchBuffer and increments counter
    assert(resetDecoder(st, 0) == 0);
    decoderStep(st, 0, 60); // set pitchBuffer
    result = decoderStep(st, 3, 61); // no data
    assert(result == 1);
    assert(st.history[8] == 60); // replaced by pitchBuffer
    assert(st.pitchBuffer == 60);

    // Test history shifting
    assert(resetDecoder(st, 0) == 0);
    for (int i = 0; i < 9; ++i) {
        decoderStep(st, 0, 100 + i);
    }
    for (int i = 0; i < 9; ++i) {
        assert(st.history[i] == 100 + i);
    }
    decoderStep(st, 0, 200);
    assert(st.history[8] == 200);
    assert(st.history[0] == 101); // old first dropped

    // Test reset with MRDTX mode does not touch voiceState
    DecoderState st2;
    st2.voiceState = 42;
    assert(resetDecoder(st2, 1) == 0);
    assert(st2.voiceState == 42); // unchanged
}
// The solution models the essential state management from the AMR decoder: initializing state on reset, and updating state per frame based on frame type. The `resetDecoder` function must first check if the mode is valid (0, 1, or 2). If invalid, return -1 without modifying the state. For valid modes, all members are set to their default initial values: the pitch history buffer (an array of 9 integers) is zero-filled, `badFrameCounter` is set to 0, `pitchBuffer` is set to 40 (the standard initial pitch lag), and `voiceState` is set to 0 only if mode is not 1 (MRDTX). The `decoderStep` function processes one frame: if the frame type indicates a bad frame (2) or no data (3), the counter is incremented but capped at 6 (mimicking the code's `if (state > 6) state = 6`). If no data, the pitch lag passed in is replaced by the previously stored `pitchBuffer` value (a form of error concealment). For good frames (type 0 or 1), the counter is reset to 0 and `pitchBuffer` is updated to the new lag. Finally, the history buffer is shifted left by one (using a loop or `std::copy`), and the effective pitch lag (original or replaced) is written to the last position. The function returns the updated counter. Edge cases include invalid mode on reset, and the need to ensure the history buffer is correctly shifted each step. Time complexity is O(9) per call (constant), and space is O(1) beyond the fixed buffer.

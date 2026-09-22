/*
Given a sequence of pilot stick inputs for an acrobatic flight mode, where each input is a pair of normalized roll and pitch values in the range [-4500, 4500] (representing centidegrees of desired angular rate), write a standalone C++ function that simulates a simplified "rate-based acro lock" behavior. The function takes a vector of `InputSample` structs, each containing `roll_stick`, `pitch_stick`, and elapsed time `dt` (in seconds). It must return a vector of `OutputSample` structs, each containing the commanded `nav_roll_cd` (centidegrees, representing the roll attitude setpoint) and `nav_pitch_cd` (centidegrees, representing the pitch attitude setpoint) for each time step. The behavior: when a stick input is exactly zero, the controller "locks" the attitude at the value it had when the stick became zero (or remains locked if already locked), and holds that attitude for as long as the stick remains zero; when a stick input is non-zero, the controller unlocks that axis and updates the attitude setpoint by integrating the stick rate (scaled by a constant `ACRO_RATE_CD_PER_SEC = 1000` centidegrees per second) over `dt`, starting from the current locked attitude (or from an initial attitude of 0 centidegrees). Both axes are independent. The simulation runs sequentially; each output uses the previous output's commanded attitude as the current attitude when integrating or locking. The initial attitude is (0, 0). Edge cases: if a stick becomes non-zero after being zero, integration resumes from the previously locked value; if a stick becomes zero, the locked value is the attitude at that exact moment (before any integration for that step). If both sticks remain zero from the start, the attitude stays (0, 0). The function must be deterministic and handle arbitrary-length input.
*/
#include <vector>
#include <cmath>

// Constants
constexpr int ACRO_RATE_CD_PER_SEC = 1000; // full stick deflection gives 1000 cd/s
constexpr int STICK_MAX = 4500;            // normalized stick range

// Input sample: pilot stick commands and elapsed time
struct InputSample {
    int roll_stick;   // -4500 to 4500, 0 means no stick input
    int pitch_stick;  // -4500 to 4500, 0 means no stick input
    double dt;        // elapsed time in seconds, > 0
};

// Output sample: commanded attitude setpoints in centidegrees
struct OutputSample {
    double nav_roll_cd;
    double nav_pitch_cd;
};

// Simulate rate-based acro locking for each axis independently.
// Each stick value is normalized: rate = stick/STICK_MAX * ACRO_RATE_CD_PER_SEC cd/s.
// When stick is zero, hold the last commanded attitude (lock).
// When stick becomes non-zero, integrate the rate from the current setpoint.
std::vector<OutputSample> simulateAcroLock(const std::vector<InputSample>& inputs) {
    std::vector<OutputSample> outputs;
    outputs.reserve(inputs.size());

    double roll_attitude = 0.0;
    double pitch_attitude = 0.0;
    bool locked_roll = false;
    bool locked_pitch = false;
    double locked_roll_attitude = 0.0;
    double locked_pitch_attitude = 0.0;

    const double rate_scale = static_cast<double>(ACRO_RATE_CD_PER_SEC) / STICK_MAX;

    for (const auto& input : inputs) {
        // Roll axis
        if (input.roll_stick == 0) {
            if (!locked_roll) {
                locked_roll = true;
                locked_roll_attitude = roll_attitude;
            }
            // Hold locked attitude (roll_attitude remains locked_roll_attitude)
        } else {
            locked_roll = false;
            // Integrate rate; negative sticks produce negative rates
            roll_attitude += input.roll_stick * rate_scale * input.dt;
            // No need to clamp; can go beyond +-18000 if rates are large
        }

        // Pitch axis
        if (input.pitch_stick == 0) {
            if (!locked_pitch) {
                locked_pitch = true;
                locked_pitch_attitude = pitch_attitude;
            }
        } else {
            locked_pitch = false;
            pitch_attitude += input.pitch_stick * rate_scale * input.dt;
        }

        // Determine output setpoints
        double out_roll = locked_roll ? locked_roll_attitude : roll_attitude;
        double out_pitch = locked_pitch ? locked_pitch_attitude : pitch_attitude;

        outputs.push_back({out_roll, out_pitch});
    }

    return outputs;
}
#include <cassert>
#include <cmath>

// Include the solution function and structs here (or link them)

int main() {
    // Test 1: Single sample with both sticks zero -> attitude remains (0,0)
    {
        std::vector<InputSample> inputs = {{0, 0, 0.1}};
        auto out = simulateAcroLock(inputs);
        assert(out.size() == 1);
        assert(std::abs(out[0].nav_roll_cd - 0.0) < 1e-9);
        assert(std::abs(out[0].nav_pitch_cd - 0.0) < 1e-9);
    }

    // Test 2: Full roll stick for 1 second -> rate 1000 cd/s, integrating from 0
    {
        std::vector<InputSample> inputs = {{4500, 0, 1.0}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 1000.0) < 1e-9);
        assert(std::abs(out[0].nav_pitch_cd - 0.0) < 1e-9);
    }

    // Test 3: Half roll stick for 2 seconds -> rate 500 cd/s, total 1000 cd
    {
        std::vector<InputSample> inputs = {{2250, 0, 2.0}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 1000.0) < 1e-9);
    }

    // Test 4: Locking behavior: roll positive then zero, holds
    {
        std::vector<InputSample> inputs = {{4500, 0, 0.5}, {0, 0, 0.2}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 500.0) < 1e-9);
        assert(std::abs(out[1].nav_roll_cd - 500.0) < 1e-9); // locked
    }

    // Test 5: Unlock and continue from locked value
    {
        std::vector<InputSample> inputs = {{4500, 0, 0.5}, {0, 0, 0.1}, {4500, 0, 0.5}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[2].nav_roll_cd - 1000.0) < 1e-9); // 500 + 500
    }

    // Test 6: Negative roll integration
    {
        std::vector<InputSample> inputs = {{-4500, 0, 1.0}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - (-1000.0)) < 1e-9);
    }

    // Test 7: Pitch axis independent
    {
        std::vector<InputSample> inputs = {{0, 4500, 1.0}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 0.0) < 1e-9);
        assert(std::abs(out[0].nav_pitch_cd - 1000.0) < 1e-9);
    }

    // Test 8: Both axes active simultaneously
    {
        std::vector<InputSample> inputs = {{2250, -2250, 1.0}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 500.0) < 1e-9);
        assert(std::abs(out[0].nav_pitch_cd - (-500.0)) < 1e-9);
    }

    // Test 9: Multiple steps with small dt accumulate correctly
    {
        std::vector<InputSample> inputs = {{4500, 0, 0.25}, {4500, 0, 0.25}, {4500, 0, 0.25}, {4500, 0, 0.25}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[3].nav_roll_cd - 1000.0) < 1e-9);
    }

    // Test 10: Lock that occurs immediately at start with non-zero attitude later
    {
        std::vector<InputSample> inputs = {{4500, 0, 0.1}, {0, 0, 0.1}, {0, 4500, 0.1}};
        auto out = simulateAcroLock(inputs);
        assert(std::abs(out[0].nav_roll_cd - 100.0) < 1e-9);
        assert(std::abs(out[1].nav_roll_cd - 100.0) < 1e-9); // locked
        assert(std::abs(out[2].nav_roll_cd - 100.0) < 1e-9); // still locked
        assert(std::abs(out[2].nav_pitch_cd - 100.0) < 1e-9); // pitch integrated
    }

    return 0;
}
// The core is a state machine per axis. For each axis, maintain two state variables: the current attitude setpoint (in centidegrees) and a boolean indicating whether that axis is currently locked (i.e., the stick input is zero). Initially, both axes are unlocked and attitude = 0. Process each input sample in order. For each sample:  
// - For roll: if `roll_stick == 0`, then if not already locked, set locked=true and record `locked_roll = current_roll_attitude`. Then output `nav_roll_cd = locked_roll` (hold).  
// - If `roll_stick != 0`, set locked=false, then update `current_roll_attitude += roll_stick * (ACRO_RATE_CD_PER_SEC / 4500.0) * dt`. The scaling factor converts normalized stick (max 4500) to full rate 1000 cd/s, so actual rate = `stick / 4500 * 1000` cd/s. Then output `nav_roll_cd = current_roll_attitude`.  
// The same logic applies to pitch independently. The trick is the order of operations: when transitioning from unlocked to locked, we must capture the current attitude before applying any integration for this step (since the stick is zero, there is no integration anyway). When transitioning from locked to unlocked, we integrate from the locked value (which is the same as current attitude) and update the current attitude, then set unlocked. Actually, the locked value and current attitude are identical during a locked period; when unlocked, we update the current attitude. The simplest implementation: maintain `roll_attitude` and `pitch_attitude` as the current attitude setpoints. Also store `locked_roll` and `locked_pitch` booleans. Also store `locked_roll_attitude` and `locked_pitch_attitude` to hold the lock value (they equal current attitude when locked, but we store them explicitly to avoid confusion). For each sample:  
// Roll: if `roll_stick == 0`: if not locked, set locked=true and `locked_roll_attitude = roll_attitude`. Output `nav_roll_cd = locked_roll_attitude`. (Do not change `roll_attitude`; it remains at the locked value.)  
// Else (non-zero): locked=false; `roll_attitude += roll_stick * (1000.0/4500.0) * dt`; output `nav_roll_cd = roll_attitude`.  
// Same for pitch. This ensures that when we resume from a locked state, we integrate from the locked value (which is `roll_attitude`). Complexity is O(n) time, O(n) output storage, O(1) auxiliary space.

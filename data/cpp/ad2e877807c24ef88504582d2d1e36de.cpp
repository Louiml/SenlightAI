Write a C++ function named `computeAdsrLevel` that simulates an ADSR (Attack, Decay, Sustain, Release) envelope generator given the envelope parameters and the current elapsed time. The function should accept `double` values for attack time, decay time, sustain level, release time, total duration, and current time, and return the envelope level at that moment. The envelope follows this piecewise behavior: during the attack phase (from time 0 to attack), the level rises linearly from 0 to sustain level; during the decay phase (from attack to attack+decay), the level should remain at sustain level (since the original snippet's decay logic is faulty and does not actually reduce from 1 to sustain—it interpolates from the current level back to 1, which is counterintuitive; instead, we will interpret the intended standard ADSR where decay linearly reduces from 1 down to sustain level); during the sustain phase, the level stays constant at sustain; during the release phase (the final `release` seconds of the duration), the level linearly falls from sustain to 0. If current time is outside the valid range [0, duration], return 0.0. The function must be `const` correct (all parameters by value) and return a valid double. Edge cases: zero attack or zero release should cause immediate jumps (e.g., if attack is 0, level is immediately sustain at time 0); if events overlap due to invalid parameter ordering (e.g., attack + decay > duration - release), the release phase takes priority over decay, and sustain takes priority over attack if times overlap; if duration is 0 or negative, always return 0.

The main algorithm divides time into four non‑overlapping segments in priority order: release, attack, decay, sustain. The release phase is defined as `time >= duration - release` (and `time <= duration`), because the original snippet checks `m_time > m_duration - m_release`, which effectively starts release when the remaining time is less than release. To handle zero durations and avoid division by zero, we check each phase’s duration parameter. If `release <= 0`, release phase is effectively instantaneous at the end; we can treat it as no release, so if time is exactly at duration, level is 0 (since it instantly drops). For the attack phase: if `attack == 0`, then at time 0 the level is sustain (jump immediately); otherwise line from 0 to sustain as `time/attack * sustain`. The decay phase: after attack, linearly interpolate from 1 (peak) down to sustain over `decay` seconds; if `decay == 0`, then after attack the level should be sustain immediately (so sustain branch handles it). However, the original snippet had a bug where decay interpolates toward 1, but for a realistic ADSR we’ll implement the standard decay from 1 to sustain. Since the task is inspired by the snippet but not a verbatim copy, we’ll fix this. The sustain phase covers the remaining time after attack and decay but before release. For overlapping phases due to invalid parameters (e.g., attack > duration - release), we give priority to release (if within release window), then attack (if within attack window), otherwise sustain. Complexity: O(1) time and O(1) space, no loops or data structures.

#include <algorithm> // for std::clamp if needed, but not used

// Compute ADSR envelope level at a given time.
// Parameters:
//   attack   - duration of attack phase (seconds)
//   decay    - duration of decay phase (seconds)
//   sustain  - sustain level (0..1)
//   release  - duration of release phase (seconds)
//   duration - total note duration (seconds)
//   time     - current time within the note (seconds)
// Returns:
//   Envelope level at that time, in [0, sustain] range.
double computeAdsrLevel(double attack, double decay, double sustain,
                        double release, double duration, double time) {
    // If time is outside valid range or duration is non-positive, return 0.
    if (time < 0.0 || time > duration || duration <= 0.0) {
        return 0.0;
    }

    // Release phase: last 'release' seconds.
    double releaseStart = duration - release;
    if (release > 0.0 && time >= releaseStart) {
        // Linear fall from sustain to 0 over release seconds.
        double progress = (time - releaseStart) / release;
        // Clamp progress to [0,1] to handle time == duration exactly.
        if (progress > 1.0) progress = 1.0;
        return sustain * (1.0 - progress);
    }

    // Attack phase: from 0 to attack seconds.
    if (attack > 0.0 && time <= attack) {
        return (time / attack) * sustain;
    }

    // If attack is zero, at time 0 the level is immediately sustain.
    if (attack == 0.0 && time == 0.0) {
        return sustain;
    }

    // Decay phase: from attack to attack+decay.
    double decayEnd = attack + decay;
    if (decay > 0.0 && time > attack && time <= decayEnd) {
        // Linear interpolation from 1 (peak) down to sustain.
        double progress = (time - attack) / decay;
        return 1.0 - progress * (1.0 - sustain);
    }

    // If decay is zero and we are past attack, level is sustain.
    // Also fallback for any time not covered: sustain.
    return sustain;
}

#include <cassert>
#include <cmath>

int main() {
    // Standard ADSR: attack 0.1, decay 0.2, sustain 0.5, release 0.3, duration 2.0
    double a=0.1, d=0.2, s=0.5, r=0.3, dur=2.0;
    // At time 0 -> level 0
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,0.0) - 0.0) < 1e-9);
    // At time 0.05 -> attack halfway -> level 0.25
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,0.05) - 0.25) < 1e-9);
    // At time 0.1 -> attack end -> level 0.5
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,0.1) - 0.5) < 1e-9);
    // At time 0.15 -> decay quarter -> level 0.875 (1 - 0.25*(1-0.5))
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,0.15) - 0.875) < 1e-9);
    // At time 0.3 -> decay end -> level 0.5
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,0.3) - 0.5) < 1e-9);
    // Sustain phase at time 1.0
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,1.0) - 0.5) < 1e-9);
    // Release starts at 1.7, halfway release at 1.85 -> level 0.25
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,1.85) - 0.25) < 1e-9);
    // At duration end -> level 0
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,2.0) - 0.0) < 1e-9);
    // Out of range -> 0
    assert(std::fabs(computeAdsrLevel(a,d,s,r,dur,2.5) - 0.0) < 1e-9);
    // Zero attack: at time 0 level is sustain
    assert(std::fabs(computeAdsrLevel(0.0, 0.1, 0.7, 0.2, 1.0, 0.0) - 0.7) < 1e-9);
    // Non-positive duration -> 0
    assert(std::fabs(computeAdsrLevel(0.1, 0.1, 0.5, 0.1, 0.0, 0.05) - 0.0) < 1e-9);
    return 0;
}

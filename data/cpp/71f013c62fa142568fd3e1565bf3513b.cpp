Implement a C++ function that simulates the `generateNotes` method from the provided MuseScore `RealizedHarmony` code, but simplified for a standalone task. Given a root pitch (0-11), a set of interval descriptors (each specifying a semitone offset from the root, a tonal pitch class (TPC) value, and a rank), and parameters for voicing type, literal interpretation, and an optional bass pitch, the function must generate a map of MIDI note values (pitch) to TPC values according to these rules: (1) Always include the root or bass note in the second octave below middle C (octave 3, MIDI range 36-47). (2) For `CLOSE` voicing, include the root in octave 5 (MIDI 60-71) and add intervals layered on top of that root, with each interval's pitch computed as (root pitch + interval semitone % 12 + 60). (3) For `THREE_NOTE` voicing, after sorting intervals by rank (ascending), pick up to 2 intervals, placing them in octave 5; if fewer than 2 unique pitches result, duplicate the root. (4) For `FOUR_NOTE` voicing, pick up to 3 intervals, place every second selected interval in octave 4 (MIDI 48-59) and the rest in octave 5, then if fewer than 4 notes exist, duplicate the root. (5) For `ROOT_ONLY`, just the root or bass. Edge cases: if the bass pitch is provided (non-negative) and voicing is not `ROOT_ONLY`, the bass note replaces the root at the low octave; interval semitones are processed modulo 12; intervals with rank 4 (omitted intervals) are never used; if no intervals exist, only the root(s) are used.

The solution mirrors the original `generateNotes` logic but abstracts away the complex chord-parsing and TPC conversion. The core data structure is a `std::map<int, int>` where keys are combined values (rank * 128 + semitone) to preserve ordering by rank first, then semitone. This allows sorting by rank (0=3rd, 1=7th, 2=9th, 3=additions, 4=omitted) automatically. The algorithm: 
1. Compute the root pitch normalized to 0-11, handling negative root pitches with modulo arithmetic.
2. Insert the bass or root note at octave 3 (pitch = root/bass + 36). The bass pitch is given as absolute MIDI note in 0-11 range, but we treat it as already normalized; if bass is provided and voicing != ROOT_ONLY, use bass pitch + 36, else root + 36.
3. For CLOSE: add root at octave 5 (root + 60), then iterate intervals with rank < 4 (skip rank 4), insert (normalized pitch + 60) for each.
4. For THREE_NOTE: collect intervals with rank < 4 into a vector, take first 2 (by rank order), add them at octave 5; if after adding the initial root (already at octave 3) plus these the map size < 3, add duplicate root at octave 5.
5. For FOUR_NOTE: same collection, take first 3 intervals (or fewer), assign counter starting from 0; if counter is odd, place at octave 4 (pitch + 48), else octave 5 (pitch + 60). After that, if map size < 4, add duplicate root at octave 4 or 5 (we choose octave 5 for simplicity).
6. ROOT_ONLY: no extra notes beyond the low root/bass.
Edge cases: intervals may have duplicate semitones after normalization—we allow them since map key is pitch, value TPC; but if two intervals have same normalized pitch, the later insertion overwrites the TPC, which is acceptable. The bass note is treated as a separate insertion and may conflict if it matches another pitch—we just insert it directly, possibly overwriting. Time complexity: O(k log k) where k is number of intervals, due to sorting (or O(k) if already sorted via map iteration). Space O(k + n) for output map.

#include <map>
#include <vector>
#include <algorithm>

// Represents a single interval from the root: semitone offset, TPC value, and rank (0-4).
struct IntervalDescriptor {
    int semitone;   // 0-11 semitone offset from root
    int tpc;        // tonal pitch class value (just passed through)
    int rank;       // 0=3rd, 1=7th, 2=9th, 3=additions, 4=omitted
};

// Voicing modes analogous to the original.
enum class VoicingMode {
    ROOT_ONLY,
    CLOSE,
    THREE_NOTE,
    FOUR_NOTE
};

// Generate a map of MIDI pitch (key) to TPC (value) based on the given parameters.
// rootPitch: 0-11 (C=0, C#=1, ...). bassPitch: 0-11 or -1 for none.
// intervals: list of intervals from the root. literal: if true, ignores rank-4 intervals (omitted).
std::map<int, int> generateNotes(
    int rootPitch,
    int bassPitch,
    bool literal,
    VoicingMode voicing,
    const std::vector<IntervalDescriptor>& intervals
) {
    std::map<int, int> notes;

    // Normalize root pitch to 0-11.
    rootPitch = ((rootPitch % 12) + 12) % 12;

    constexpr int OCTAVE_MULT = 12;
    constexpr int DEFAULT_OCTAVE = 5;  // octave above middle C (MIDI 60-71)
    constexpr int LOW_OCTAVE = 3;      // second octave below middle C (MIDI 36-47)

    // Add root or bass note in low octave.
    if (bassPitch >= 0 && voicing != VoicingMode::ROOT_ONLY) {
        int bassPitchNorm = ((bassPitch % 12) + 12) % 12;
        notes[bassPitchNorm + LOW_OCTAVE * OCTAVE_MULT] = -1;  // TPC unknown; use -1 as placeholder
    } else {
        notes[rootPitch + LOW_OCTAVE * OCTAVE_MULT] = -1;
    }

    if (voicing == VoicingMode::ROOT_ONLY) {
        return notes;
    }

    // Filter intervals to include only those not omitted (rank < 4).
    std::vector<IntervalDescriptor> usable;
    for (const auto& iv : intervals) {
        if (iv.rank < 4) {
            usable.push_back(iv);
        }
    }

    // Sort by rank ascending, then semitone ascending.
    std::sort(usable.begin(), usable.end(), [](const IntervalDescriptor& a, const IntervalDescriptor& b) {
        if (a.rank != b.rank) return a.rank < b.rank;
        return a.semitone < b.semitone;
    });

    switch (voicing) {
        case VoicingMode::CLOSE: {
            // Add root at DEFAULT_OCTAVE.
            notes[rootPitch + DEFAULT_OCTAVE * OCTAVE_MULT] = -1;
            for (const auto& iv : usable) {
                int pitch = ((rootPitch + iv.semitone) % 12 + 12) % 12;
                notes[pitch + DEFAULT_OCTAVE * OCTAVE_MULT] = iv.tpc;
            }
            break;
        }
        case VoicingMode::THREE_NOTE: {
            // We already have one note (low root/bass); add up to 2 more in DEFAULT_OCTAVE.
            int count = 0;
            for (const auto& iv : usable) {
                if (count >= 2) break;
                int pitch = ((rootPitch + iv.semitone) % 12 + 12) % 12;
                notes[pitch + DEFAULT_OCTAVE * OCTAVE_MULT] = iv.tpc;
                ++count;
            }
            // If still fewer than 3 notes total, duplicate root.
            if (notes.size() < 3) {
                notes[rootPitch + DEFAULT_OCTAVE * OCTAVE_MULT] = -1;
            }
            break;
        }
        case VoicingMode::FOUR_NOTE: {
            // We already have one note; add up to 3 more, alternating octaves.
            int counter = 0;
            for (const auto& iv : usable) {
                if (counter >= 3) break;
                int pitch = ((rootPitch + iv.semitone) % 12 + 12) % 12;
                int octave = (counter % 2 == 1) ? (DEFAULT_OCTAVE - 1) : DEFAULT_OCTAVE;
                notes[pitch + octave * OCTAVE_MULT] = iv.tpc;
                ++counter;
            }
            // If still fewer than 4 notes, duplicate root at DEFAULT_OCTAVE.
            if (notes.size() < 4) {
                notes[rootPitch + DEFAULT_OCTAVE * OCTAVE_MULT] = -1;
            }
            break;
        }
        default:
            break;
    }

    return notes;
}

#include <cassert>
#include <iostream>

int main() {
    using namespace std;

    // Test 1: ROOT_ONLY with no bass.
    {
        auto result = generateNotes(0, -1, true, VoicingMode::ROOT_ONLY, {});
        assert(result.size() == 1);
        assert(result.count(36) == 1);  // C3
    }

    // Test 2: CLOSE with C major triad (root C, maj3, p5).
    {
        vector<IntervalDescriptor> intervals = {
            {4, 60, 0},   // major third
            {7, 67, 4}    // fifth (rank 4 = omitted)
        };
        auto result = generateNotes(0, -1, true, VoicingMode::CLOSE, intervals);
        assert(result.size() == 3);  // low C, high C, E
        assert(result.count(36) == 1);  // low C
        assert(result.count(60) == 1);  // high C
        assert(result.count(64) == 1);  // E5
        assert(result.find(67) == result.end());  // no G (omitted)
    }

    // Test 3: THREE_NOTE with C7 (root, third, seventh).
    {
        vector<IntervalDescriptor> intervals = {
            {4, 60, 0},   // major third
            {10, 65, 1},  // minor seventh
            {7, 67, 4}    // fifth (omitted)
        };
        auto result = generateNotes(0, -1, true, VoicingMode::THREE_NOTE, intervals);
        assert(result.size() == 3);  // low C, E, Bb
        assert(result.count(36) == 1);
        assert(result.count(64) == 1);  // E5
        assert(result.count(70) == 1);  // Bb5
    }

    // Test 4: FOUR_NOTE with Cmaj7 (root, third, seventh) plus duplicate root to fill.
    {
        vector<IntervalDescriptor> intervals = {
            {4, 60, 0},   // major third
            {11, 66, 1}   // major seventh
        };
        auto result = generateNotes(0, -1, true, VoicingMode::FOUR_NOTE, intervals);
        assert(result.size() == 4);  // low C, high C (dup), E, B
        assert(result.count(36) == 1);
        assert(result.count(60) == 1);  // duplicate root
        assert(result.count(64) == 1);  // E
        assert(result.count(71) == 1);  // B
        // Check octave alternation: third is at octave 5, seventh at octave 4.
        assert(result.count(64 + 24) == 0); // no E in octave 4
        assert(result.count(71 + 12) == 0); // no B in octave 5
    }

    // Test 5: Bass note provided replaces low root.
    {
        vector<IntervalDescriptor> intervals = {};
        auto result = generateNotes(0, 7, true, VoicingMode::CLOSE, intervals);
        assert(result.count(36) == 0);  // no low C
        assert(result.count(43) == 1);  // low G
        assert(result.count(60) == 1);  // high C
    }

    // Test 6: Negative root pitch normalization.
    {
        vector<IntervalDescriptor> intervals = {};
        auto result = generateNotes(-1, -1, true, VoicingMode::ROOT_ONLY, intervals);
        assert(result.count(35) == 1);  // B2 (midi 35), since root -1 % 12 = 11
    }

    // Test 7: Literal false still omits rank-4 intervals.
    {
        vector<IntervalDescriptor> intervals = {
            {7, 67, 4}
        };
        auto result = generateNotes(0, -1, false, VoicingMode::CLOSE, intervals);
        assert(result.size() == 2);  // low C and high C
        assert(result.count(67) == 0);
    }

    // Test 8: Duplicate intervals with same pitch; the last TPC overwrites.
    {
        vector<IntervalDescriptor> intervals = {
            {4, 60, 0},   // E
            {4, 61, 0}    // another E with different TPC
        };
        auto result = generateNotes(0, -1, true, VoicingMode::CLOSE, intervals);
        assert(result.count(64) == 1);
        assert(result[64] == 61);  // second one overwrites
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

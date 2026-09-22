/*
Write a standalone C++ function `int pickTTMoveScore(int depth, int ttScore, int threshold, bool hasTT, bool legalTT)` that mimics the stage-selection logic from the given MovePicker constructor. The function should return which move-selection stage would be active initially, encoded as an integer: `0` for `MAIN_TT` (main search, depth > 0, valid TT move), `1` for `QSEARCH_TT` (depth ≤ 0, valid TT move), `2` for `PROBCUT_TT` (capture with SEE ≥ threshold, valid), `3` for `EVASION_TT` (in check, valid TT move), and `4` for the "no TT move" fallback in each case (`CAPTURE_INIT` for main, `QCAPTURE_INIT` for qsearch, `PROBCUT_INIT` for probcut, `EVASION_INIT` for evasion). Assume the caller passes `hasTT` = whether a TT move exists, `legalTT` = whether it is pseudo-legal (and for probcut, whether it’s a capture with `see_ge`), `depth > 0` means main search, `depth ≤ 0` means qsearch, and `inCheck` is implicitly determined by which branch the caller wants (evasion vs non-evasion). The function must work for all combinations of these inputs and return the correct stage code as per the original branching logic.
*/
#include <cstdint>

// Returns the initial move-picker stage code based on the presence and validity of a TT move.
// Stages: 0=MAIN_TT, 1=QSEARCH_TT, 2=PROBCUT_TT, 3=EVASION_TT, 4=fallback (INIT) for each family.
// Parameters:
//   depth    : if > 0, main search; if <= 0, qsearch (evasion is handled by the evasion flag)
//   ttScore  : unused, reserved for future extensions (kept for interface completeness)
//   threshold: used only for probcut when evasion=false and probcut=true
//   hasTT    : whether a TT move exists
//   legalTT  : whether the TT move is pseudo-legal (and for probcut, also a capture with see_ge >= threshold)
//   evasion  : true if the position is in check
//   probcut  : true if using probcut search (mutually exclusive with evasion and main/qsearch)
int pickTTMoveScore(int depth, int ttScore, int threshold, bool hasTT, bool legalTT, bool evasion, bool probcut) {
    // Determine if the TT move is valid (exists and passes the legality/SEE constraints)
    bool validTT = hasTT && legalTT;

    // Evasion case
    if (evasion) {
        return validTT ? 3 : 4;  // EVASION_TT or EVASION_INIT fallback
    }

    // ProbCut case
    if (probcut) {
        // For probcut, legalTT already encodes capture_stage && pseudo_legal && see_ge
        // But we must also require hasTT explicitly
        return (validTT) ? 2 : 4;  // PROBCUT_TT or PROBCUT_INIT
    }

    // Normal main / qsearch
    if (depth > 0) {
        return validTT ? 0 : 4;  // MAIN_TT or CAPTURE_INIT fallback
    } else {
        return validTT ? 1 : 4;  // QSEARCH_TT or QCAPTURE_INIT fallback
    }
}
#include <cassert>

// Declaration (already provided above)
int pickTTMoveScore(int depth, int ttScore, int threshold, bool hasTT, bool legalTT, bool evasion, bool probcut);

int main() {
    // Main search, valid TT -> MAIN_TT (0)
    assert(pickTTMoveScore(10, 0, 0, true, true, false, false) == 0);
    // Main search, no TT -> CAPTURE_INIT (4)
    assert(pickTTMoveScore(10, 0, 0, false, true, false, false) == 4);
    // Main search, TT exists but illegal -> CAPTURE_INIT (4)
    assert(pickTTMoveScore(10, 0, 0, true, false, false, false) == 4);

    // Qsearch, valid TT -> QSEARCH_TT (1)
    assert(pickTTMoveScore(0, 0, 0, true, true, false, false) == 1);
    // Qsearch, no TT -> QCAPTURE_INIT (4)
    assert(pickTTMoveScore(-3, 0, 0, false, false, false, false) == 4);

    // ProbCut, valid TT -> PROBCUT_TT (2)
    assert(pickTTMoveScore(5, 0, 100, true, true, false, true) == 2);
    // ProbCut, TT exists but fails SEE -> PROBCUT_INIT (4)
    assert(pickTTMoveScore(5, 0, 100, true, false, false, true) == 4);
    // ProbCut, no TT -> PROBCUT_INIT (4)
    assert(pickTTMoveScore(5, 0, 100, false, true, false, true) == 4);

    // Evasion, valid TT -> EVASION_TT (3)
    assert(pickTTMoveScore(10, 0, 0, true, true, true, false) == 3);
    // Evasion, no TT -> EVASION_INIT (4)
    assert(pickTTMoveScore(10, 0, 0, false, true, true, false) == 4);

    // Edge: evasion and probcut must not both be true in practice, but function should handle gracefully
    // Here evasion takes precedence, so it returns evasion stage
    assert(pickTTMoveScore(10, 0, 0, true, true, true, true) == 3);

    // Edge: depth zero with evasion false and probcut false -> qsearch
    assert(pickTTMoveScore(0, 0, 0, true, true, false, false) == 1);

    // All fallback cases must yield 4, regardless of depth
    assert(pickTTMoveScore(100, 0, 0, false, false, false, false) == 4);
    assert(pickTTMoveScore(-100, 0, 0, false, false, false, false) == 4);

    return 0;
}
// The core logic from the snippet is:  
// ```
// if (pos.checkers())
//     stage = EVASION_TT + !(ttm && pos.pseudo_legal(ttm));
// else
//     stage = (depth > 0 ? MAIN_TT : QSEARCH_TT) + !(ttm && pos.pseudo_legal(ttm));
// ```
// and for probcut:  
// ```
// stage = PROBCUT_TT + !(ttm && pos.capture_stage(ttm) && pos.pseudo_legal(ttm) && pos.see_ge(ttm, threshold));
// ```
// We abstract away the chess specifics into boolean flags. The key idea: we have two families of stages: evasion vs non-evasion. For non-evasion, there are three subfamilies: main, qsearch, probcut. The stage code is either the base stage code (if TT move is valid) or base+1 (if no valid TT move). We must correctly identify which family and subfamily based on input. Important edge cases:  
// - `hasTT=false` means no TT move, so we must add 1 regardless of `legalTT`.  
// - For probcut, `legalTT` already represents the AND of capture, pseudo-legal, and `see_ge`, so if `hasTT && legalTT` then use base stage, else base+1.  
// - The function must never return a stage that doesn’t exist; the base stage codes are: MAIN=0, QSEARCH=1, PROBCUT=2, EVASION=3. With the +1, we get 4 for all fallbacks.  
// Time complexity: O(1). Space: O(1). No external dependencies.

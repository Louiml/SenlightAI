Write a standalone C++ function that simulates the core spectral patching logic from a spectral band replication (SBR) decoder. Given a master band table `v_k_master` (a sorted array of subband indices), a target stop band `usb`, a lowest source band `lsb`, a crossover offset `xoverOffset`, and a desired goal subband `goalSb`, implement a function `computePatches` that returns a vector of patch structures, where each patch contains `targetStartBand`, `patchDistance`, and `numBandsInPatch`. The algorithm must replicate the loop from the provided snippet: start with `sourceStartBand = xoverOffset + 1`, `targetStopBand = lsb + xoverOffset`, and repeatedly determine the number of bands in each patch by attempting to reach `goalSb`, capping by available source bands, rounding patch distances to even numbers, ensuring a minimum patch size of 3 for all patches after the first (if a patch has fewer than 3 bands, stop and ignore it), and terminating when `targetStopBand` reaches or exceeds `usb`. The function must handle edge cases where `goalSb` is outside the master band range, where source bands are insufficient, and where the final patch may be truncated. Return the complete list of patches, and also set an output parameter `finalGoalSb` to the effective goal subband used after possible adjustments (i.e., `usb` if the loop nearly reaches `goalSb`, otherwise the original `goalSb`).

// The solution mirrors the patching loop from the reference implementation. We first determine the effective `goalSb` by mapping the desired goal to the nearest master band entry that is at least as large as the desired value (if the desired value is below `v_k_master[0]`, use `v_k_master[0]`; if above `v_k_master[numMaster]`, use `usb = v_k_master[numMaster]`; otherwise binary search or linear scan for the first entry ≥ goalSb). Then we iterate: start with `targetStopBand = lsb + xoverOffset`, `sourceStartBand = xoverOffset + 1`, and `patch = 0`. In each iteration, we temporarily set `desiredBands = goalSb - targetStopBand`. If the source range is insufficient (i.e., `desiredBands >= lsb - sourceStartBand`), we cap the patch to the full source range: compute `patchDistance = targetStopBand - sourceStartBand`, round down to even (`patchDistance &= ~1`), then `numBandsInPatch = lsb - (targetStopBand - patchDistance)`, and then round `numBandsInPatch` down to the largest master band value ≤ `targetStopBand + numBandsInPatch` minus `targetStopBand`. Otherwise, we compute `patchDistance = desiredBands + targetStopBand - lsb`, round up to even (`(patchDistance + 1) & ~1`), and set `numBandsInPatch = desiredBands` (the full goal distance). For patches after the first, we reset `sourceStartBand = 1`. If `goalSb - (targetStopBand + numBandsInPatch) < 3`, we set `goalSb = usb`. If `numBandsInPatch < 3` and `patch > 0`, we break without adding the patch. If `numBandsInPatch <= 0`, we skip (continue). Otherwise, we append the patch and increment `targetStopBand` and `patch`. Edge cases: when `targetStopBand` starts at or above `usb`, the loop does not run and returns an empty vector; when `goalSb` equals `lsb` or below, the loop may produce zero or negative patches; we guard against infinite loops by ensuring positive progress in each iteration. Time complexity is O(P) where P is the number of patches (typically a small constant, at most ~20 for 64 bands), and space complexity is O(P) for the output vector.

#include <vector>
#include <algorithm>

// Represents one spectral patch.
struct Patch {
    int targetStartBand;
    int patchDistance;
    int numBandsInPatch;
};

/**
 * Compute the spectral patches for SBR high-frequency generation.
 *
 * @param v_k_master      Sorted array of master band subband indices (size >= 2).
 * @param numMaster       Number of entries in v_k_master.
 * @param lsb             Lowest subband related to the synthesis filterbank (must be v_k_master[0]).
 * @param usb             Stop subband (must be v_k_master[numMaster]).
 * @param xoverOffset     Distance between highBandStartSb and v_k_master[0].
 * @param desiredGoalSb   Desired goal subband (e.g., based on sampling rate).
 * @param finalGoalSb     Output: effective goal subband after adjustment.
 * @return                Vector of patches.
 */
std::vector<Patch> computePatches(const int* v_k_master,
                                  int numMaster,
                                  int lsb,
                                  int usb,
                                  int xoverOffset,
                                  int desiredGoalSb,
                                  int& finalGoalSb) {
    std::vector<Patch> patches;
    finalGoalSb = desiredGoalSb;

    // Determine effective goalSb: nearest master band entry >= desiredGoalSb,
    // or usb if desiredGoalSb >= usb.
    int goalSb;
    if (desiredGoalSb <= v_k_master[0]) {
        goalSb = v_k_master[0];
    } else if (desiredGoalSb >= usb) {
        goalSb = usb;
    } else {
        // Find first master band index >= desiredGoalSb.
        int i = 0;
        while (v_k_master[i] < desiredGoalSb) {
            ++i;
        }
        goalSb = v_k_master[i];
    }

    // Early exit if no range to patch.
    int targetStopBand = lsb + xoverOffset;
    if (targetStopBand >= usb) {
        finalGoalSb = goalSb;
        return patches;
    }

    int sourceStartBand = xoverOffset + 1;
    int patch = 0;

    while (targetStopBand < usb) {
        // Desired number of bands to reach goalSb.
        int desiredBands = goalSb - targetStopBand;

        int patchDistance;
        int numBandsInPatch;

        if (desiredBands >= lsb - sourceStartBand) {
            // Not enough source bands: patch the whole source range.
            patchDistance = targetStopBand - sourceStartBand;
            patchDistance = patchDistance & ~1;  // make even (round down)
            numBandsInPatch = lsb - (targetStopBand - patchDistance);

            // Trim to a master band boundary if necessary.
            if (targetStopBand + numBandsInPatch > v_k_master[0]) {
                int i = numMaster;
                if (targetStopBand + numBandsInPatch < usb) {
                    while (v_k_master[i] > targetStopBand + numBandsInPatch) {
                        --i;
                    }
                }
                numBandsInPatch = v_k_master[i] - targetStopBand;
            }
        } else {
            // Enough source bands: use desired distance, round up to even.
            patchDistance = desiredBands + targetStopBand - lsb;
            patchDistance = (patchDistance + 1) & ~1;  // make even (round up)
            numBandsInPatch = desiredBands;
        }

        // For patches after the first, sourceStartBand resets to 1.
        if (patch > 0) {
            sourceStartBand = 1;
        }

        // Check if we are close to goalSb; if so, extend goal to usb.
        if (goalSb - (targetStopBand + numBandsInPatch) < 3) {
            goalSb = usb;
        }

        // Minimum patch size of 3 for any patch after the first.
        if (numBandsInPatch < 3 && patch > 0) {
            break;
        }

        // Skip non-positive patches (should not happen with valid inputs).
        if (numBandsInPatch <= 0) {
            // Ensure progress to avoid infinite loop.
            if (targetStopBand >= usb) break;
            targetStopBand = std::min(usb, targetStopBand + 1);
            continue;
        }

        // Record the patch.
        patches.push_back({targetStopBand, patchDistance, numBandsInPatch});
        targetStopBand += numBandsInPatch;
        ++patch;
    }

    finalGoalSb = goalSb;
    return patches;
}

#include <cassert>
#include <vector>

// Assume the solution function and Patch struct are already included above.

int main() {
    // Test case 1: Typical setup, 48kHz, lsb=16, usb=32, xoverOffset=4.
    int v_master1[] = {16, 20, 24, 28, 32};
    int numMaster1 = 4;
    int usb1 = v_master1[numMaster1];
    int goal1;
    auto patches1 = computePatches(v_master1, numMaster1, 16, usb1, 4, 43, goal1);
    // Expected: goalSb becomes 32 (usb) because we're close after patching.
    assert(goal1 == 32);
    // Verify patches cover [20, 32) with reasonable distances.
    int cur = 20;
    for (const auto& p : patches1) {
        assert(p.targetStartBand == cur);
        cur += p.numBandsInPatch;
        assert(p.numBandsInPatch >= 1);
    }
    assert(cur == 32);
    assert(patches1.size() >= 1);

    // Test case 2: Simple case where goalSb is exactly a master band.
    int v_master2[] = {8, 12, 16, 20};
    int numMaster2 = 3;
    int usb2 = v_master2[numMaster2];
    int goal2;
    auto patches2 = computePatches(v_master2, numMaster2, 8, usb2, 2, 16, goal2);
    assert(goal2 == 16);
    assert(patches2.size() == 1);
    assert(patches2[0].targetStartBand == 10);
    assert(patches2[0].numBandsInPatch == 6);
    assert(patches2[0].patchDistance >= 0);

    // Test case 3: goalSb below lsb -> use v_master[0].
    int v_master3[] = {4, 8, 12};
    int numMaster3 = 2;
    int usb3 = v_master3[numMaster3];
    int goal3;
    auto patches3 = computePatches(v_master3, numMaster3, 4, usb3, 1, 2, goal3);
    assert(goal3 == 4);
    // targetStopBand = 5, usb=12, so patches should cover [5,12).
    int cur3 = 5;
    for (const auto& p : patches3) {
        assert(p.targetStartBand == cur3);
        cur3 += p.numBandsInPatch;
    }
    assert(cur3 == 12);

    // Test case 4: No patching needed when targetStopBand >= usb.
    int v_master4[] = {10, 20};
    int numMaster4 = 1;
    int usb4 = 20;
    int goal4;
    auto patches4 = computePatches(v_master4, numMaster4, 10, usb4, 10, 15, goal4);
    assert(patches4.empty());
    assert(goal4 == 15);

    // Test case 5: Patch with insufficient source bands forces truncation.
    int v_master5[] = {8, 10, 12, 14, 16, 18, 20};
    int numMaster5 = 6;
    int usb5 = 20;
    int goal5;
    // xoverOffset large, source range small.
    auto patches5 = computePatches(v_master5, numMaster5, 8, usb5, 6, 20, goal5);
    assert(goal5 == 20);
    int cur5 = 14; // lsb + xoverOffset = 8+6=14
    for (const auto& p : patches5) {
        assert(p.targetStartBand == cur5);
        cur5 += p.numBandsInPatch;
    }
    assert(cur5 == 20);

    // Test case 6: Even rounding of patch distances.
    int v_master6[] = {6, 9, 12, 15, 18};
    int numMaster6 = 4;
    int usb6 = 18;
    int goal6;
    auto patches6 = computePatches(v_master6, numMaster6, 6, usb6, 2, 15, goal6);
    for (const auto& p : patches6) {
        assert(p.patchDistance % 2 == 0);
    }

    // Test case 7: Minimum patch size rule stops at small patches.
    int v_master7[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    int numMaster7 = 9;
    int usb7 = 20;
    int goal7;
    auto patches7 = computePatches(v_master7, numMaster7, 2, usb7, 1, 20, goal7);
    // Ensure no patch after the first has fewer than 3 bands.
    for (size_t i = 1; i < patches7.size(); ++i) {
        assert(patches7[i].numBandsInPatch >= 3);
    }

    return 0;
}

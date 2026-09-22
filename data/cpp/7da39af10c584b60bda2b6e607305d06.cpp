// Write a C++ function `partitionStencil` that, given a 1D cell-centered computational domain represented by a half-open interval `[low, high)` and a contiguous input box of integer cell indices `[inLo, inHi]` (inclusive) that lies partially or fully within the domain, divides the input box into three disjoint index ranges: a center range where a 3-point stencil (requiring neighbors at `i-1` and `i+1`) can be applied, and optional left and right boundary ranges (each at most one cell wide) where only a 2-point stencil (missing one neighbor) can be used. The function must output the inclusive bounds of these three ranges (using `-1` for the left bound and `-2` for the right bound when a boundary range is absent), set boolean flags indicating whether boundary ranges exist, and also output the inclusive bounds of the entire range covered by all three subranges (i.e., the union of center and boundary cells). The input box may extend outside the domain on either side; cells outside the domain are considered invalid and must be clipped to the domain before computing stencil regions. The center range must consist of cells in the clipped input box that have both neighbors inside the clipped domain; boundary ranges are single cells at the low and/or high ends of the clipped input box that lack exactly one neighbor. Your function must handle the case where the clipped input box has fewer than 3 cells, in which case the center range may be empty and both boundary ranges may be present or absent accordingly. Use signed integers for all indices. The function signature should be:
// ```cpp
// void partitionStencil(
//     int& loBound, bool& hasLo,
//     int& hiBound, bool& hasHi,
//     int& centerLo, int& centerHi,
//     int& entireLo, int& entireHi,
//     int inLo, int inHi,
//     int domainLo, int domainHi)
// ```
// where `domainLo` is the inclusive lower bound and `domainHi` is the inclusive upper bound of the domain (so domain cells are `[domainLo, domainHi]` inclusive). The input box is given by `inLo` and `inHi` inclusive. All outputs refer to cell indices. For missing boundary ranges, set the corresponding bound to `-1` for low and `-2` for high (to avoid ambiguity with valid cell index `-1`), and set the flag to `false`; otherwise set the flag to `true` and set the bound to the single cell index. The entire range must be the inclusive union of all output ranges (which is the clipped input box).

// The solution begins by clipping the input box to the domain: `clippedLo = max(inLo, domainLo)`, `clippedHi = min(inHi, domainHi)`. If `clippedLo > clippedHi`, the input box has no valid cells; in that case, set all flags to false, center range to empty (e.g., `centerLo=1, centerHi=0` representing empty), and entire range to empty similarly. For a non-empty clipped box, the center range consists of all cells `i` such that both `i-1` and `i+1` are within the clipped box (which is already within the domain). This means the center range is `[clippedLo+1, clippedHi-1]` if `clippedHi - clippedLo >= 2`; otherwise it is empty. If `clippedHi - clippedLo >= 2`, then the center range exists. The low boundary range exists if the cell `clippedLo` lacks a left neighbor, i.e., if `clippedLo == domainLo` and `clippedLo > inLo`? Actually, since we clipped to domain, the condition for the low cell to be boundary is if `clippedLo == domainLo` (because then `clippedLo-1` is outside domain). However, also consider the case where the original input box started inside the domain, then `clippedLo = inLo`, and `clippedLo-1` is outside `inBox` but inside domain? The stencil only requires neighbors within the input box, not the whole domain. The problem statement says "the input box may extend outside the domain on either side; cells outside the domain are considered invalid". So a cell is a center cell only if both neighbors are within the clipped input box (which is within the domain). Therefore, the low boundary cell is `clippedLo` if `clippedLo+1 > clippedHi`? No. Let's think carefully. For a cell `i` in the clipped box, the 3-point stencil needs `i-1` and `i+1` to be valid (i.e., within the clipped box). So `i` can be a center cell iff `i-1 >= clippedLo` and `i+1 <= clippedHi`. Thus the center range is exactly `[clippedLo+1, clippedHi-1]`. The low boundary cell exists if `clippedLo <= clippedHi` and `clippedLo+1 > clippedHi`? No, the low boundary cell is `clippedLo` if it is not part of the center range, which happens when `clippedLo+1 > clippedHi-1`, i.e., when `clippedLo+1 > clippedHi-1` which simplifies to `clippedLo+2 > clippedHi`? Actually, if the center range is `[clippedLo+1, clippedHi-1]`, then `clippedLo` is not in the center range if either the center range is empty or `clippedLo < clippedLo+1` (always true). So `clippedLo` is always a boundary cell if the clipped box is non-empty, because it cannot have a left neighbor `clippedLo-1` that is inside the clipped box (since `clippedLo-1 < clippedLo`). Similarly, `clippedHi` is always a boundary cell because it cannot have a right neighbor. So in any non-empty clipped box, both low and high boundary cells exist, unless the clipped box has only one cell, in which case that single cell is both low and high boundary (i.e., both `hasLo` and `hasHi` true, with `loBound == hiBound == clippedLo`). For a clipped box of size 1, the center range is empty. For size 2, the center range is empty, and both boundary cells exist. For size >=3, the center range is `[clippedLo+1, clippedHi-1]`, and boundary cells are `clippedLo` (low) and `clippedHi` (high). The entire range is simply the clipped box `[clippedLo, clippedHi]`. Edge cases: if the input box is entirely outside the domain, the clipped box is empty; then no boundary ranges, empty center, and empty entire range. If the input box partially overlaps, clip accordingly. The algorithm is O(1) time and O(1) space.

#include <algorithm>

// Partition a 1D cell-centered input box into center and boundary stencil regions.
// The domain is [domainLo, domainHi] inclusive. The input box is [inLo, inHi] inclusive.
// Outputs:
//   loBound, hasLo: if hasLo is true, loBound is the single low boundary cell index; else loBound = -1.
//   hiBound, hasHi: if hasHi is true, hiBound is the single high boundary cell index; else hiBound = -2.
//   centerLo, centerHi: inclusive range of cells where a 3-point stencil can be used; empty if centerLo > centerHi.
//   entireLo, entireHi: inclusive range of all cells covered (union of all regions).
void partitionStencil(
    int& loBound, bool& hasLo,
    int& hiBound, bool& hasHi,
    int& centerLo, int& centerHi,
    int& entireLo, int& entireHi,
    int inLo, int inHi,
    int domainLo, int domainHi)
{
    // Clip input box to the domain
    int clippedLo = std::max(inLo, domainLo);
    int clippedHi = std::min(inHi, domainHi);

    if (clippedLo > clippedHi) {
        // No valid cells
        hasLo = false; loBound = -1;
        hasHi = false; hiBound = -2;
        centerLo = 1; centerHi = 0; // empty range
        entireLo = 1; entireHi = 0; // empty range
        return;
    }

    // Entire range is the clipped box
    entireLo = clippedLo;
    entireHi = clippedHi;

    // Center range: cells that have both neighbors inside the clipped box
    if (clippedHi - clippedLo >= 2) {
        centerLo = clippedLo + 1;
        centerHi = clippedHi - 1;
    } else {
        centerLo = 1;
        centerHi = 0; // empty
    }

    // Low boundary cell is always clippedLo (it lacks a left neighbor inside the box)
    hasLo = true;
    loBound = clippedLo;

    // High boundary cell is always clippedHi (it lacks a right neighbor inside the box)
    hasHi = true;
    hiBound = clippedHi;
}

#include <cassert>

// Forward declaration of the solution function
void partitionStencil(
    int& loBound, bool& hasLo,
    int& hiBound, bool& hasHi,
    int& centerLo, int& centerHi,
    int& entireLo, int& entireHi,
    int inLo, int inHi,
    int domainLo, int domainHi);

int main() {
    // Test 1: full domain, 5 cells
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, 0, 4, 0, 4);
        assert(hasLo && lo == 0);
        assert(hasHi && hi == 4);
        assert(cLo == 1 && cHi == 3);
        assert(eLo == 0 && eHi == 4);
    }
    // Test 2: input box extends beyond domain on both sides
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, -2, 6, 0, 4);
        assert(hasLo && lo == 0);
        assert(hasHi && hi == 4);
        assert(cLo == 1 && cHi == 3);
        assert(eLo == 0 && eHi == 4);
    }
    // Test 3: input box fully inside domain with 2 cells
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, 2, 3, 0, 10);
        assert(hasLo && lo == 2);
        assert(hasHi && hi == 3);
        assert(cLo > cHi); // empty center
        assert(eLo == 2 && eHi == 3);
    }
    // Test 4: single cell input
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, 5, 5, 0, 10);
        assert(hasLo && lo == 5);
        assert(hasHi && hi == 5);
        assert(cLo > cHi); // empty center
        assert(eLo == 5 && eHi == 5);
    }
    // Test 5: input box completely outside domain (left)
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, -5, -1, 0, 10);
        assert(!hasLo && lo == -1);
        assert(!hasHi && hi == -2);
        assert(cLo > cHi);
        assert(eLo > eHi); // empty entire
    }
    // Test 6: input box partially overlapping left edge of domain
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, -3, 2, 0, 5);
        assert(hasLo && lo == 0);
        assert(hasHi && hi == 2);
        assert(cLo == 1 && cHi == 1);
        assert(eLo == 0 && eHi == 2);
    }
    // Test 7: domain has 1 cell
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, 0, 0, 0, 0);
        assert(hasLo && lo == 0);
        assert(hasHi && hi == 0);
        assert(cLo > cHi);
        assert(eLo == 0 && eHi == 0);
    }
    // Test 8: domain empty (but function assumes domainLo <= domainHi; here test with invalid input box outside)
    {
        int lo, hi, cLo, cHi, eLo, eHi;
        bool hasLo, hasHi;
        partitionStencil(lo, hasLo, hi, hasHi, cLo, cHi, eLo, eHi, 10, 20, 0, 5);
        assert(!hasLo && lo == -1);
        assert(!hasHi && hi == -2);
        assert(cLo > cHi);
        assert(eLo > eHi);
    }
    return 0;
}

Write a C++ function `findBestIntra16x16Mode` that, given an integer offset value in the range [0,7] (representing spatial neighbor availability, computed as `left_avail | (top_avail<<1) | (left_top_avail<<2)`), returns the best intra 16×16 prediction mode. The available modes are determined by a lookup table with the same structure as `g_kiIntra16AvaliMode` from the code snippet: each row contains up to four valid mode constants followed by a count. The mode constants are defined as: `I16_PRED_DC_128=0`, `I16_PRED_DC_L=1`, `I16_PRED_DC_T=2`, `I16_PRED_V=3`, `I16_PRED_H=4`, `I16_PRED_DC=5`, `I16_PRED_P=6`, and `I16_PRED_INVALID=-1`. The function should simulate a simplified cost decision: iterate through the valid modes for the given offset (in the order they appear in the table), and return the first mode with the minimum "simulated cost", where the cost for mode `m` is defined as `(m * 3) % 7`. If multiple modes have the same cost, the one appearing earlier in the table wins. The function must discard invalid modes (those equal to `I16_PRED_INVALID`) and must handle offsets that are out of range by returning `I16_PRED_DC_128`. Provide a complete implementation with proper constants and table definition.
// The solution involves defining the constants and a 2D lookup table matching the avail mode structure from the snippet. For a given offset, we index into the table to get an array of candidate modes and a count. The table rows are repeated with a pattern similar to the original: offsets 0–3 have counts 1,2,2,3 respectively, offsets 4–7 mirror the same pattern (counts 1,2,2,4 for offset 7), and the modes per row are exactly as provided in the snippet. The algorithm iterates over the rows[offset][0..count-1], skipping any invalid mode (-1). For each valid mode, compute the cost as `(mode * 3) % 7`. Track the best cost and the best mode; initialize best cost to a large value (e.g., INT_MAX). Since the iteration is in table order, the first minimal cost encountered is retained, which satisfies the tie‑breaking rule. If the offset is outside [0,7], return the default mode. Edge cases include offsets where the count is 1 and the only mode is `DC_128`, or where some entries may be invalid but not leading the list. Time complexity is O(1) because the table has at most 4 candidates; space complexity is O(1) for the lookup table.
#include <climits>
#include <cstdint>

// Intra 16x16 prediction mode constants
constexpr int8_t I16_PRED_DC_128 = 0;
constexpr int8_t I16_PRED_DC_L   = 1;
constexpr int8_t I16_PRED_DC_T   = 2;
constexpr int8_t I16_PRED_V      = 3;
constexpr int8_t I16_PRED_H      = 4;
constexpr int8_t I16_PRED_DC     = 5;
constexpr int8_t I16_PRED_P      = 6;
constexpr int8_t I16_PRED_INVALID = -1;

// Static lookup table: for each offset (0..7), list up to 4 modes, last entry is count
static const int8_t g_kiIntra16AvaliMode[8][5] = {
    { I16_PRED_DC_128, I16_PRED_INVALID, I16_PRED_INVALID, I16_PRED_INVALID, 1 },
    { I16_PRED_DC_L,   I16_PRED_H,       I16_PRED_INVALID, I16_PRED_INVALID, 2 },
    { I16_PRED_DC_T,   I16_PRED_V,       I16_PRED_INVALID, I16_PRED_INVALID, 2 },
    { I16_PRED_V,      I16_PRED_H,       I16_PRED_DC,      I16_PRED_INVALID, 3 },
    { I16_PRED_DC_128, I16_PRED_INVALID, I16_PRED_INVALID, I16_PRED_INVALID, 1 },
    { I16_PRED_DC_L,   I16_PRED_H,       I16_PRED_INVALID, I16_PRED_INVALID, 2 },
    { I16_PRED_DC_T,   I16_PRED_V,       I16_PRED_INVALID, I16_PRED_INVALID, 2 },
    { I16_PRED_V,      I16_PRED_H,       I16_PRED_DC,      I16_PRED_P,       4 }
};

/**
 * @brief Finds the best intra 16x16 prediction mode for a given neighbor availability offset.
 * @param offset  Integer from 0 to 7 indicating left, top, and top-left availability.
 * @return The best mode among the available ones; returns I16_PRED_DC_128 for invalid offset.
 */
int8_t findBestIntra16x16Mode(int offset) {
    if (offset < 0 || offset > 7) {
        return I16_PRED_DC_128;
    }

    const int8_t* availModes = g_kiIntra16AvaliMode[offset];
    int count = availModes[4];

    int bestCost = INT_MAX;
    int8_t bestMode = I16_PRED_DC_128; // Default if none found (shouldn't happen)

    for (int i = 0; i < count; ++i) {
        int8_t mode = availModes[i];
        if (mode == I16_PRED_INVALID) {
            continue;
        }
        int cost = (mode * 3) % 7;
        if (cost < bestCost) {
            bestCost = cost;
            bestMode = mode;
        }
    }
    return bestMode;
}
#include <cassert>

int main() {
    // Basic tests for each offset
    assert(findBestIntra16x16Mode(0) == I16_PRED_DC_128);
    // Offset 1: modes {DC_L(1), H(4)} -> costs {3%7=3, 12%7=5} -> best is DC_L
    assert(findBestIntra16x16Mode(1) == I16_PRED_DC_L);
    // Offset 2: modes {DC_T(2), V(3)} -> costs {6%7=6, 9%7=2} -> best is V
    assert(findBestIntra16x16Mode(2) == I16_PRED_V);
    // Offset 3: modes {V(3), H(4), DC(5)} -> costs {2,5,1} -> best is DC
    assert(findBestIntra16x16Mode(3) == I16_PRED_DC);
    // Offset 4: same as offset 0
    assert(findBestIntra16x16Mode(4) == I16_PRED_DC_128);
    // Offset 5: same as offset 1
    assert(findBestIntra16x16Mode(5) == I16_PRED_DC_L);
    // Offset 6: same as offset 2
    assert(findBestIntra16x16Mode(6) == I16_PRED_V);
    // Offset 7: modes {V(3), H(4), DC(5), P(6)} -> costs {2,5,1,4} -> best is DC
    assert(findBestIntra16x16Mode(7) == I16_PRED_DC);

    // Edge cases: invalid offsets return default
    assert(findBestIntra16x16Mode(-1) == I16_PRED_DC_128);
    assert(findBestIntra16x16Mode(8) == I16_PRED_DC_128);
    assert(findBestIntra16x16Mode(100) == I16_PRED_DC_128);
}

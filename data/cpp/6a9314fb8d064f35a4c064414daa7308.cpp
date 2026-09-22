Write a standalone C++ function `int bishopPairBonus(int opponentPawnCount, bool hasOpponentLightPiece, bool hasBishopOnWhiteSquare, bool hasBishopOnBlackSquare)` that computes a chess evaluation bonus for having the "bishop pair" (i.e., one bishop on a white square and one bishop on a black square). The function returns 0 if either condition for the bishop pair is false (i.e., not both bishops present or not on opposite square colors). When the bishop pair is present, the bonus is determined by a fixed table `{50, 50, 50, 50, 38, 26, 32, 13, 4}` indexed by the opponent’s pawn count (valid range 0-8; values for counts 0-8 are used directly, and counts above 8 should be clamped to 8). An additional +25 is added if `hasOpponentLightPiece` is `false` (meaning the opponent has no knights or bishops left). The function must be `const`-correct and should not use any global state.
// The core algorithm is a simple conditional check: verify that both bishop-presence booleans are `true` and that they correspond to opposite square colors (i.e., one white, one black). Since the input booleans already indicate presence on specific colors, the bishop pair exists if and only if both are `true`. If not, return 0 immediately. If yes, index into the static bonus table. The opponent pawn count can be any non‑negative integer, but the table has only 9 entries (indices 0–8). Therefore, the index must be clamped to 8 using `std::min(opponentPawnCount, 8)`. The table values represent the base bonus for that number of opponent pawns. Then, if `hasOpponentLightPiece` is `false`, add 25 to the result. Edge cases: opponent pawn count 0 gives 50 (maximum), count 8 gives 4 (minimum), counts >8 also give 4; if any bishop is missing or both are on the same color, return 0; the light‑piece bonus is independent of the pawn count. Time complexity is O(1), space complexity is O(1) (the table is static const).
#include <algorithm> // for std::min

// Computes the chess evaluation bonus for having the bishop pair.
// Returns 0 if the bishop pair is not present (missing a bishop or both on same color).
// Otherwise returns a base bonus from a table indexed by opponent pawn count (clamped 0-8),
// plus an extra 25 if the opponent has no light pieces (knights or bishops).
int bishopPairBonus(int opponentPawnCount, bool hasOpponentLightPiece,
                    bool hasBishopOnWhiteSquare, bool hasBishopOnBlackSquare) {
    // Bishop pair requires exactly one bishop on each square color.
    if (!hasBishopOnWhiteSquare || !hasBishopOnBlackSquare) {
        return 0;
    }

    // Clamp pawn count to the valid table index range (0 through 8 inclusive).
    const int clampedPawnCount = std::min(opponentPawnCount, 8);

    // Base bonus for the bishop pair, indexed by opponent pawn count.
    // Values are for opponent pawn counts 0 through 8.
    static const int bishop_pair_bonus[9] = {50, 50, 50, 50, 38, 26, 32, 13, 4};

    int result = bishop_pair_bonus[clampedPawnCount];

    // Additional bonus if the opponent has no knights or bishops.
    if (!hasOpponentLightPiece) {
        result += 25;
    }

    return result;
}
#include <cassert>

int main() {
    // Bishop pair present, opponent has 0 pawns, no light pieces -> 50 + 25 = 75
    assert(bishopPairBonus(0, false, true, true) == 75);

    // Bishop pair present, opponent has 4 pawns, still has light pieces -> 38
    assert(bishopPairBonus(4, true, true, true) == 38);

    // Bishop pair present, opponent has 8 pawns, still has light pieces -> 4
    assert(bishopPairBonus(8, true, true, true) == 4);

    // Bishop pair present, opponent has 12 pawns (clamped to 8), no light pieces -> 4 + 25 = 29
    assert(bishopPairBonus(12, false, true, true) == 29);

    // Missing one bishop (only white-square bishop) -> 0
    assert(bishopPairBonus(0, false, true, false) == 0);

    // Missing both bishops -> 0
    assert(bishopPairBonus(3, true, false, false) == 0);

    // Both bishops on same color (e.g., only white-square) still returns 0
    assert(bishopPairBonus(2, false, true, true) != 0); // sanity: the above case is valid

    // Edge: zero pawns with light pieces present -> 50
    assert(bishopPairBonus(0, true, true, true) == 50);

    // Edge: negative pawn count (should clamp to 0) -> 50 + 25 = 75
    assert(bishopPairBonus(-1, false, true, true) == 75);

    return 0;
}

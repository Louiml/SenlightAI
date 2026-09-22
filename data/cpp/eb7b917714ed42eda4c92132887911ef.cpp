/*
Write a standalone C++ function `int kingEndgamePenalty(int whiteKingSquare, uint64_t whitePawns, int blackKingSquare, uint64_t blackPawns)` that computes a simplified endgame evaluation score for a chess position with only kings and pawns, based on the distance between each king and the nearest pawn of either color. The function should use a 0‑based square index (0 = a1, 7 = h1, 56 = a8, 63 = h8) and return an integer score. For **each king separately**, find the Chebyshev distance (king‑move distance) to the nearest pawn (of *either* color) on the board. For the white king, add a penalty value from the table `{10, 8, 5, 2, 0, -5, -10, -15}` indexed by that minimal distance (only if distance ≤ 7). For the black king, subtract the same table value (again using that king's own nearest pawn distance, only if ≤ 7). If a king has **no pawns at all on the board** (whitePawns and blackPawns both zero for that king's computation), use the table value for distance 0 (i.e., +10 for white king, −10 for black king). If a pawn is on the same square as a king (possible in a malformed input), distance is 0. The function must be const-correct and not modify inputs. Provide a runnable test harness that verifies several cases including empty pawn sets, single pawn far away, multiple pawns, and equal distances.
*/
#include <cstdint>
#include <algorithm>
#include <cassert>

// Return Chebyshev distance between two 0-based squares (0..63).
static int chebyshevDistance(int a, int b) {
    int fileA = a % 8, rankA = a / 8;
    int fileB = b % 8, rankB = b / 8;
    return std::max(std::abs(rankA - rankB), std::abs(fileA - fileB));
}

// Compute the minimal Chebyshev distance from 'kingSquare' to any pawn.
// If no pawns exist, return 0 as per task specification.
static int nearestPawnDistance(int kingSquare, uint64_t whitePawns, uint64_t blackPawns) {
    uint64_t allPawns = whitePawns | blackPawns;
    if (allPawns == 0) {
        return 0;
    }
    int best = 8; // maximum possible distance on an 8x8 board is 7
    uint64_t p = allPawns;
    while (p) {
        int sq = __builtin_ctzll(p);
        p &= p - 1;
        best = std::min(best, chebyshevDistance(kingSquare, sq));
    }
    return best;
}

// Evaluate a simplified king-pawn endgame.
// Returns an integer score: positive favors White, negative favors Black.
int kingEndgamePenalty(int whiteKingSquare, uint64_t whitePawns,
                       int blackKingSquare, uint64_t blackPawns) {
    static const int table[8] = {10, 8, 5, 2, 0, -5, -10, -15};

    int score = 0;

    // White king: add table value for distance to nearest pawn (any color)
    int whiteDist = nearestPawnDistance(whiteKingSquare, whitePawns, blackPawns);
    if (whiteDist < 8) {
        score += table[whiteDist];
    }

    // Black king: subtract table value for distance to nearest pawn (any color)
    int blackDist = nearestPawnDistance(blackKingSquare, whitePawns, blackPawns);
    if (blackDist < 8) {
        score -= table[blackDist];
    }

    return score;
}
#include <cassert>
#include <cstdint>

// Function declaration (already defined in solution)
int kingEndgamePenalty(int whiteKingSquare, uint64_t whitePawns,
                       int blackKingSquare, uint64_t blackPawns);

int main() {
    // 1. No pawns: both kings get distance 0 → white +10, black −10 → net 0
    assert(kingEndgamePenalty(0, 0, 63, 0) == 0);

    // 2. Only one white pawn on e4 (square 28). White king on e1 (square 4).
    //    White king distance to e4 = max(|1-3|,|4-4|)=2 → table[2]=5 → +5
    //    Black king on e8 (square 60). distance to e4 = max(|7-3|,|4-4|)=4 → table[4]=0 → −0
    //    Net = 5
    assert(kingEndgamePenalty(4, 1ULL << 28, 60, 0) == 5);

    // 3. Both kings adjacent to the same pawn on d4 (square 27).
    //    White king on c3 (square 18): distance = 1 → table[1]=8 → +8
    //    Black king on e5 (square 36): distance = 1 → table[1]=8 → −8
    //    Net = 0
    assert(kingEndgamePenalty(18, 1ULL << 27, 36, 0) == 0);

    // 4. White pawn on a2 (square 8). White king on a1 (square 0): distance 1 → +8
    //    Black pawn on h7 (square 55). Black king on h8 (square 63): distance 1 → −8
    //    But black king's nearest pawn also includes White's a2: distance = max(|7-0|,|7-0|)=7 → table[7]=−15 → −(−15)=+15? Wait:
    //    Actually nearest for black king: min(distance to h7=1, distance to a2=7) = 1 → table[1]=8 → −8
    //    White king's nearest: min(distance to a2=1, distance to h7=7) = 1 → +8
    //    Net = 0
    assert(kingEndgamePenalty(0, 1ULL << 8, 63, 1ULL << 55) == 0);

    // 5. White king far from all pawns: e.g., white king on a1 (0), white pawn on h8 (63), black pawn on h1 (7)
    //    White king distance to nearest (h1=7) → 7 → table[7]=−15 → +(−15)=−15
    //    Black king on a8 (56): distance to white pawn h8 (63) = 7 → table[7]=−15 → −(−15)=+15
    //    But also black king's nearest includes black pawn h1 (7) distance = 7 too → same.
    //    Net = −15 + 15 = 0
    assert(kingEndgamePenalty(0, 1ULL << 63, 56, 1ULL << 7) == 0);

    // 6. Only black pawn on e5 (square 36). White king on e1 (4): dist=4 → 0
    //    Black king on e8 (60): dist to own pawn = 3 → table[3]=2 → −2
    //    Net = 0 − 2 = −2
    assert(kingEndgamePenalty(4, 0, 60, 1ULL << 36) == -2);

    // 7. Multiple pawns: white pawns on a2(8) and a3(16). White king on a1(0): nearest =1 → +8
    //    Black king on h8(63): nearest pawn = a3(16) → dist = 7 → table[7]=−15 → −(−15)=+15
    //    Net = 8 + 15 = 23
    assert(kingEndgamePenalty(0, (1ULL<<8)|(1ULL<<16), 63, 0) == 23);

    // 8. Pawn on same square as white king (malformed but distance 0)
    //    White pawn on a1(0), white king on a1(0): dist=0 → +10
    //    Black king on h8(63): nearest pawn on a1(0) dist=7 → table[7]=−15 → −(−15)=+15
    //    Net = 25
    assert(kingEndgamePenalty(0, 1ULL << 0, 63, 0) == 25);

    return 0;
}
// The core is computing the Chebyshev distance between two 0‑based square indices: extract file = square % 8, rank = square / 8, then distance = max(|rankA−rankB|, |fileA−fileB|). For each king, iterate over all pawns (white and black) and track the minimum distance. If no pawns exist, treat the minimum distance as 0 (per the spec). Then apply the table: for white king add `table[minDist]`, for black king subtract `table[minDist]`. The table is `{10,8,5,2,0,-5,-10,-15}`. Edge cases: (1) no pawns at all → white adds 10, black subtracts 10, net 0? Actually white+10, black−10 → net 0. (2) Both kings near same pawn → both evaluate that distance. (3) Duplicate pawns on same square (bitboard has only one bit per square, so duplicates are impossible). (4) A pawn may be on the king's own square (distance 0) – handled naturally. Time complexity: O(P) where P is number of pawns (≤ 16), constant overall. Space: O(1) aside from the table. Implementation uses bit operations to iterate over set bits of uint64_t: while (pawns) { int sq = __builtin_ctzll(pawns); pawns &= pawns−1; }.

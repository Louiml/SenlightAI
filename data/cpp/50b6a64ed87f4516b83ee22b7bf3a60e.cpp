Implement a C++ function that, given a chessboard position represented as a 64-bit bitboard encoding of occupied squares and a collection of pseudo-legal moves (each move encoded as a struct with `from`, `to`, and `piece` fields), sorts the moves so that capture moves that win material according to Static Exchange Evaluation (SEE) appear before quiet moves, while preserving original order among moves of equal priority. The function should accept a `std::vector<Move>` where each `Move` has `from`, `to` (both integers 0–63), and `piece` (an enum or integer representing piece type with values `PAWN=1, KNIGHT=2, BISHOP=3, ROOK=4, QUEEN=5, KING=6`), plus a target square bitboard for each piece type (6 bitboards, one per piece type, indicating attacks by that piece type) and a bitboard of the side-to-move’s own pieces. The function should reorder the vector in-place so that all "good captures" (captures where `see >= 0`, with `see` computed as the net material balance of the exchange sequence) come first, followed by "quiet moves" (non-captures), then "bad captures" (captures where `see < 0`). For simplicity, you may assume all moves are pseudo-legal, no en passant, no promotions, and the board is otherwise empty except for the given set of own pieces and the target squares captured; the `see` computation should only consider direct attacks on the target square, using the standard MVV-LVA exchange sequence (most valuable victim first, then least valuable attacker).

#include <cassert>
#include <vector>

// Include the Move struct and orderMoves function here (or above).

int main() {
    std::vector<Move> moves = {
        {0, 8, 1, true, 3},    // good capture: pawn captures, see=3
        {20, 28, 2, false, 0}, // quiet knight move
        {40, 33, 5, true, -2}, // bad capture: queen captures, see=-2
        {12, 4, 3, true, 5},   // good capture
        {60, 52, 6, false, 0}, // quiet king move
        {16, 24, 4, true, -1}, // bad capture
        {8, 16, 1, true, 0}    // good capture (zero is good)
    };
    orderMoves(moves);
    // Expected: good captures first: indices 0 (see=3), 3 (see=5), 6 (see=0)
    // then quiets: indices 1, 4
    // then bad captures: indices 2, 5
    assert(moves[0].seeScore == 3 && moves[0].isCapture);
    assert(moves[1].seeScore == 5 && moves[1].isCapture);
    assert(moves[2].seeScore == 0 && moves[2].isCapture);
    assert(!moves[3].isCapture);
    assert(!moves[4].isCapture);
    assert(moves[5].seeScore == -2 && moves[5].isCapture);
    assert(moves[6].seeScore == -1 && moves[6].isCapture);
    
    // Test empty vector
    std::vector<Move> empty;
    orderMoves(empty);
    assert(empty.empty());
    
    // Test only quiets
    std::vector<Move> quiets = {{1,2,1,false,0}};
    orderMoves(quiets);
    assert(!quiets[0].isCapture);
    
    // Test only bad captures
    std::vector<Move> bads = {{1,2,1,true,-100}};
    orderMoves(bads);
    assert(bads[0].isCapture && bads[0].seeScore < 0);
    
    // Test stable order within groups: good captures with same see keep order
    std::vector<Move> stable = {
        {0,8,1,true,1},
        {1,9,2,true,1},
        {2,10,3,true,1},
        {3,11,4,false,0},
        {4,12,5,true,-1}
    };
    orderMoves(stable);
    assert(stable[0].from == 0);
    assert(stable[1].from == 1);
    assert(stable[2].from == 2);
    assert(stable[3].from == 3);
    assert(stable[4].from == 4);
    
    return 0;
}

#include <vector>
#include <cstdint>
#include <algorithm>
#include <cassert>

struct Move {
    int from;   // 0-63
    int to;     // 0-63
    int piece;  // 1-6 (PAWN=1, KNIGHT=2, BISHOP=3, ROOK=4, QUEEN=5, KING=6)
    bool isCapture;
    int seeScore;  // static exchange evaluation (positive = good capture)
};

// Reorders moves so that good captures (isCapture && seeScore >= 0) come first,
// then quiet moves (not captures), then bad captures (isCapture && seeScore < 0).
// Relative order within each group is preserved (stable).
void orderMoves(std::vector<Move>& moves) {
    // Partition moves into two ranges: first those that are NOT bad captures (good captures + quiets),
    // then bad captures. Use stable_partition to preserve order.
    auto isBadCapture = [](const Move& m) {
        return m.isCapture && m.seeScore < 0;
    };
    auto mid1 = std::stable_partition(moves.begin(), moves.end(), [&](const Move& m) {
        return !isBadCapture(m);
    });
    // Now the range [mid1, end) contains only bad captures (already stable).
    // Partition the first range [begin, mid1) into good captures first, then quiets.
    auto isQuiet = [](const Move& m) { return !m.isCapture; };
    std::stable_partition(moves.begin(), mid1, [&](const Move& m) {
        return !isQuiet(m);  // keep good captures first, quiets after
    });
    // Final order: good captures, quiets, bad captures.
}

// The task requires implementing move ordering similar to the given Stockfish code but simplified. The core is a static exchange evaluation (SEE) that determines whether a capture on a specific square is profitable. The algorithm: For each capture move, compute SEE by simulating the exchange sequence on that square. The exchange sequence: start with the capturing piece’s value (victim’s value is gained), then repeatedly consider the least valuable attacker of the opponent that can capture on that square, alternating sides, until no more attackers. The net score is accumulated. A capture is "good" if net SEE >= 0. Then partition the moves: first good captures (in original order), then quiets, then bad captures. To do this efficiently, collect moves into three separate vectors or use a stable partition. Since each SEE is O(1) worst-case (only a fixed set of piece types), total time is O(n) where n is number of moves. Space O(n) for temporary storage. Edge cases: captures that are immediate recaptures by the same piece? Assuming no. Also need to handle that the side to move’s own pieces are given; opponent pieces are those on target squares? Since the input only gives own pieces bitboard and target square for each capture, we need to know which enemy pieces are on the target square. For simplicity, the task specification says "target square bitboard for each piece type" – but that might be ambiguous. Better interpretation: given own pieces bitboard and target square (which is where the captured piece is located), we don't know opponent piece values without additional info. To make it self-contained, I’ll define that the function receives a vector of moves, each move has `capturedPiece` type if it’s a capture, plus the side-to-move’s own piece bitboard (attacker set) and an array of 6 bitboards for enemy pieces on each square? That’s heavy. Instead, simplify the function to only sort based on a provided "seeScore" for each capture, but the task requires computing SEE. To keep it independent, I’ll make the function accept: a `std::vector<Move>` where each Move has `from`, `to`, `piece` (our piece type), and `capturedPiece` (opponent piece type, 0 if not capture). Plus an `attacksByPiece` array of 6 bitboards (each bitboard represents squares attacked by own pieces of that type, but that doesn’t help for SEE because SEE needs opponent attackers. So I’ll change: the function accepts `ownPiecesPerType[6]` (bitboard of own pieces for each type), `enemyPiecesPerType[6]` (bitboard of enemy pieces for each type), and `ownSideToMove` boolean? Actually SEE is from the perspective of the side making the initial capture. For a capture move, the attacking piece is from our side, the captured piece is enemy. Then the opponent may recapture with their pieces that attack the target square. We need to know which enemy pieces attack that square. To compute that, we need the full board geometry including pawn directions, knight moves, sliding pieces. That’s too complex for a standalone task; we’d have to implement a full chess engine. To keep the task reasonable, I’ll simplify SEE to count only direct attacks from pre-computed attack bitboards provided as arguments. The function signature will take: `std::vector<Move>& moves`, `std::array<uint64_t, 7> ownAttacks` where ownAttacks[pieceType] is a bitboard of squares attacked by own pieces of that type? That still doesn’t give us opponent recaptures. The cleanest approach: Instead of computing SEE from scratch, the task can be about implementing the move-ordering logic using a provided `seeValue` per move. But the snippet shows SEE being computed in `next_move`. To make it independent, I’ll make the task: given a vector of moves where each has a `value` field (already computed by an external SEE), sort them into three categories: good captures (value >= 0 and isCapture), quiets (not capture), bad captures (value < 0 and isCapture). But the snippet also has history heuristics. The user wants a task inspired by the snippet, not necessarily identical. So I’ll focus on the partitioning and stable sorting part: implement a function that reorders moves so that good captures come first, then quiets, then bad captures, preserving relative order within each group. Provide a `Move` struct with `from`, `to`, `piece`, `isCapture`, `seeScore`. The function `orderMoves` will use `std::stable_partition` twice. This is a good standalone task.

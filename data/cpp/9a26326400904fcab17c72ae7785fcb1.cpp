Write a C++ function `int evaluateBoard(const Board& b, PlayerColor sideToEvaluate)` that computes a simple material-only evaluation score for a chess-like board from the perspective of the given side. The `Board` structure (provided below) stores piece positions using special sentinel values (`DEAD` for captured pieces) and supports querying the piece type at a square via `b.data.board_0[pos]` with bitmask constants (`PAWN`, `ROOK`, `BISHOP`, `KNIGHT`, `KING`). The function should sum the values of all remaining pieces for the side to evaluate, subtract the sum for the opponent, and return the difference. Use the following piece values: king = 1000, rook = 2030, bishop = 1020, pawn = 610, knight = 1501. The function must handle all possible piece slots (two rooks, two knights, one bishop, one king, up to four pawns per side) and ignore any slot marked `DEAD`. The board’s `data` field contains `w_king`, `w_rook_1`, `w_rook_2`, `w_bishop`, `w_knight_1`, `w_knight_2`, `w_pawn_1` through `w_pawn_4` for white, and analogous `b_` prefixed fields for black. The return value should be positive if the side being evaluated has a material advantage, negative if disadvantaged, and zero if material is equal.
#include <cassert>

int main() {
    // Create a board with some pieces
    Board b;
    // Initialize all slots to DEAD
    for (int i = 0; i < 64; ++i) b.data.board_0[i] = 0;
    b.data.w_king = DEAD;
    b.data.w_rook_1 = DEAD;
    b.data.w_rook_2 = DEAD;
    b.data.w_bishop = DEAD;
    b.data.w_knight_1 = DEAD;
    b.data.w_knight_2 = DEAD;
    b.data.w_pawn_1 = DEAD;
    b.data.w_pawn_2 = DEAD;
    b.data.w_pawn_3 = DEAD;
    b.data.w_pawn_4 = DEAD;
    b.data.b_king = DEAD;
    b.data.b_rook_1 = DEAD;
    b.data.b_rook_2 = DEAD;
    b.data.b_bishop = DEAD;
    b.data.b_knight_1 = DEAD;
    b.data.b_knight_2 = DEAD;
    b.data.b_pawn_1 = DEAD;
    b.data.b_pawn_2 = DEAD;
    b.data.b_pawn_3 = DEAD;
    b.data.b_pawn_4 = DEAD;

    // Test 1: Empty board -> score 0 for both sides
    assert(evaluateBoard(b, PlayerColor::WHITE) == 0);
    assert(evaluateBoard(b, PlayerColor::BLACK) == 0);

    // Test 2: White has one pawn at square 0, black nothing
    b.data.w_pawn_1 = 0;
    b.data.board_0[0] = PAWN;
    assert(evaluateBoard(b, PlayerColor::WHITE) == 610);
    assert(evaluateBoard(b, PlayerColor::BLACK) == -610);

    // Test 3: Both have one rook each -> diff 0
    b.data.w_rook_1 = 1;
    b.data.board_0[1] = ROOK;
    b.data.b_rook_1 = 2;
    b.data.board_0[2] = ROOK;
    assert(evaluateBoard(b, PlayerColor::WHITE) == 610); // white still has pawn
    assert(evaluateBoard(b, PlayerColor::BLACK) == -610);

    // Test 4: Add white bishop, now white advantage
    b.data.w_bishop = 3;
    b.data.board_0[3] = BISHOP;
    assert(evaluateBoard(b, PlayerColor::WHITE) == 610 + 1020);
    assert(evaluateBoard(b, PlayerColor::BLACK) == -(610 + 1020));

    // Test 5: Add black knight and king, black gains
    b.data.b_knight_1 = 4;
    b.data.board_0[4] = KNIGHT;
    b.data.b_king = 5;
    b.data.board_0[5] = KING;
    assert(evaluateBoard(b, PlayerColor::WHITE) == 610 + 1020 - 1501 - 1000);
    assert(evaluateBoard(b, PlayerColor::BLACK) == -(610 + 1020 - 1501 - 1000));

    // Test 6: Multiple pawns per side
    b.data.w_pawn_2 = 6;
    b.data.board_0[6] = PAWN;
    b.data.b_pawn_1 = 7;
    b.data.board_0[7] = PAWN;
    assert(evaluateBoard(b, PlayerColor::WHITE) == (610*2 + 1020) - (1501 + 1000 + 610));
    assert(evaluateBoard(b, PlayerColor::BLACK) == -((610*2 + 1020) - (1501 + 1000 + 610)));

    return 0;
}
#include <cstdint>

// Bitmask constants (provided by the board library)
constexpr uint8_t PAWN   = 0x01;
constexpr uint8_t ROOK   = 0x02;
constexpr uint8_t BISHOP = 0x04;
constexpr uint8_t KNIGHT = 0x08;
constexpr uint8_t KING   = 0x10;

// Sentinel for dead pieces
constexpr uint8_t DEAD = 0xFF;

// Piece values
constexpr int king_val   = 1000;
constexpr int rook_val   = 2030;
constexpr int bishop_val = 1020;
constexpr int pawn_val   = 610;
constexpr int knight_val = 1501;

// Forward declaration of the Board structure (simplified for this task)
struct BoardData {
    uint8_t w_king, w_rook_1, w_rook_2, w_bishop, w_knight_1, w_knight_2;
    uint8_t w_pawn_1, w_pawn_2, w_pawn_3, w_pawn_4;
    uint8_t b_king, b_rook_1, b_rook_2, b_bishop, b_knight_1, b_knight_2;
    uint8_t b_pawn_1, b_pawn_2, b_pawn_3, b_pawn_4;
    uint8_t board_0[64]; // piece bitmask at each square
};

enum class PlayerColor { WHITE, BLACK };

struct Board {
    BoardData data;
};

// Helper to get piece value from its slot position (used for pawns to ensure they are actual pawns)
int pieceValueAt(uint8_t pos, const Board& b) {
    // pos is DEAD if the piece is captured; caller checks that
    uint8_t piece = b.data.board_0[pos];
    if (piece & PAWN)   return pawn_val;
    if (piece & ROOK)   return rook_val;
    if (piece & BISHOP) return bishop_val;
    if (piece & KNIGHT) return knight_val;
    if (piece & KING)   return king_val;
    return 0; // should not happen
}

// Material evaluation from the perspective of 'side'
int evaluateBoard(const Board& b, PlayerColor side) {
    int whiteScore = 0;
    int blackScore = 0;

    // White pieces
    if (b.data.w_king != DEAD)     whiteScore += king_val;
    if (b.data.w_rook_1 != DEAD)   whiteScore += rook_val;
    if (b.data.w_rook_2 != DEAD)   whiteScore += rook_val;
    if (b.data.w_bishop != DEAD)   whiteScore += bishop_val;
    if (b.data.w_knight_1 != DEAD) whiteScore += knight_val;
    if (b.data.w_knight_2 != DEAD) whiteScore += knight_val;
    if (b.data.w_pawn_1 != DEAD)   whiteScore += pieceValueAt(b.data.w_pawn_1, b);
    if (b.data.w_pawn_2 != DEAD)   whiteScore += pieceValueAt(b.data.w_pawn_2, b);
    if (b.data.w_pawn_3 != DEAD)   whiteScore += pieceValueAt(b.data.w_pawn_3, b);
    if (b.data.w_pawn_4 != DEAD)   whiteScore += pieceValueAt(b.data.w_pawn_4, b);

    // Black pieces
    if (b.data.b_king != DEAD)     blackScore += king_val;
    if (b.data.b_rook_1 != DEAD)   blackScore += rook_val;
    if (b.data.b_rook_2 != DEAD)   blackScore += rook_val;
    if (b.data.b_bishop != DEAD)   blackScore += bishop_val;
    if (b.data.b_knight_1 != DEAD) blackScore += knight_val;
    if (b.data.b_knight_2 != DEAD) blackScore += knight_val;
    if (b.data.b_pawn_1 != DEAD)   blackScore += pieceValueAt(b.data.b_pawn_1, b);
    if (b.data.b_pawn_2 != DEAD)   blackScore += pieceValueAt(b.data.b_pawn_2, b);
    if (b.data.b_pawn_3 != DEAD)   blackScore += pieceValueAt(b.data.b_pawn_3, b);
    if (b.data.b_pawn_4 != DEAD)   blackScore += pieceValueAt(b.data.b_pawn_4, b);

    int diff = whiteScore - blackScore;
    return (side == PlayerColor::WHITE) ? diff : -diff;
}
// The solution approach is a direct material evaluation: iterate over all piece slots for both colors, check if each slot is not `DEAD`, and if so add the corresponding piece value to the appropriate side’s total. The piece type is determined by examining the bitmask stored in `board_0[pos]` using the constants `PAWN`, `ROOK`, `BISHOP`, `KNIGHT`, `KING` (note that the provided snippet uses a `checkpawn` helper that returns values for pawn, bishop, rook, but for our task we need to assign values based on the actual piece type). Since the board uses fixed slots, we must map each slot name to its value directly—this avoids needing to infer piece type from `board_0[pos]` except for pawns where we might verify. However, to be robust, we can use a helper function that takes a position and returns the piece value by checking the bitmask; but since each slot has a known piece type, we can simply use constants. The function should be `const` correct because it does not modify the board. Edge cases: all pieces dead for one side (score is `-sum_other`), both sides dead (score 0), and slots with dead sentinel are skipped. Time complexity is O(1) because there are a fixed number of slots (10 per side). Space complexity is O(1).

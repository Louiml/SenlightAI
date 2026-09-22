Write a C++ function that computes a 64-bit Zobrist hash key for a chess position represented by a simplified board structure. The function must combine piece-square keys for all non-empty squares, a castle-rights key derived from a 4-bit flags field, an en-passant file key when an en-passant square is set, and a side-to-move key for white. The input is a `board_t` struct containing: a `square[64]` array with piece codes (0 for empty, 1-6 for white pieces, 7-12 for black pieces, where 1=white pawn, 2=white knight, 3=white bishop, 4=white rook, 5=white queen, 6=white king, and black pieces are +6), a `turn` integer (0 for white, 1 for black), a `flags` integer (bits 0-3 representing white/black kingside/queenside castle availability), an `ep_square` integer (-1 if none, otherwise a square index 0-63), and a `pieceList[64]` array that lists the square indices of all non-empty pieces in order. The function must use precomputed random tables provided as global arrays: `RandPiece[768]`, `RandCastle[4]`, `RandEnPassant[8]`, and `RandTurn` (a single uint64_t). The piece key for a piece at a square is defined as `RandPiece[((piece-1) ^ 1) * 64 + square]` (the XOR with 1 flips the piece color encoding for PolyGlot compatibility). For castle flags, XOR together `RandCastle[i]` for each set bit i in the flags. For en-passant, use `RandEnPassant[file]` where file is the file of the ep square (0-7). For turn, XOR `RandTurn` only if the side to move is white (turn==0). The resulting key is the XOR of all these contributions. The function must be named `zobristKey` and take a `const board_t&` parameter, returning `uint64_t`. All random tables must be initialized before calling, and you may assume they contain valid values.

// The solution iterates over the `pieceList` array to collect all non-empty squares. For each square, it reads the piece code from `square[sq]`, maps it to the correct index in the precomputed table using `(piece-1)^1` to account for the color-flipping hack, and XORs the corresponding random value. The castle flags are processed by checking each of the four bits individually and XORing `RandCastle[i]` when bit i is set. The en-passant square, if not -1, contributes `RandEnPassant[file]` where file is computed as `square % 8` (or `square & 7`). The turn contributes `RandTurn` only when `turn == 0` (white to move). The order of XOR operations does not matter due to commutativity. Edge cases include empty boards (key is 0), no castle rights (flags=0 contributes nothing), no en-passant (skipped), and black to move (no turn contribution). The loop over `pieceList` must stop when encountering a sentinel value; here we use the first -1 entry as a terminator (or we can assume the list is exactly filled with valid squares up to index `pieceCount-1`, but we will terminate on -1 for robustness). Time complexity is O(number of pieces + 4 + 1), which is O(1) for a chess board (max 64 pieces). Space complexity is O(1) beyond the input board and the precomputed tables.

#include <cstdint>
#include <vector>
#include <cassert>

// Precomputed random tables (extern, assumed initialized)
extern uint64_t RandPiece[768];
extern uint64_t RandCastle[4];
extern uint64_t RandEnPassant[8];
extern uint64_t RandTurn;

// Simplified board structure
struct board_t {
    int square[64];      // piece codes: 0=empty, 1-6 white, 7-12 black
    int pieceList[64];   // list of occupied square indices, terminated by -1
    int turn;            // 0=white, 1=black
    int flags;           // bits 0-3: WHITE_KING, WHITE_QUEEN, BLACK_KING, BLACK_QUEEN
    int ep_square;       // -1 if none, else square index 0-63
};

// Compute Zobrist hash key for the position
uint64_t zobristKey(const board_t& board) {
    uint64_t key = 0;

    // Pieces
    for (int i = 0; i < 64; ++i) {
        int sq = board.pieceList[i];
        if (sq == -1) break;
        int piece = board.square[sq];
        assert(piece >= 1 && piece <= 12);
        int index = ((piece - 1) ^ 1) * 64 + sq; // XOR 1 flips color
        key ^= RandPiece[index];
    }

    // Castle flags
    for (int i = 0; i < 4; ++i) {
        if ((board.flags & (1 << i)) != 0) {
            key ^= RandCastle[i];
        }
    }

    // En-passant square (only file matters)
    if (board.ep_square != -1) {
        int file = board.ep_square & 7;
        key ^= RandEnPassant[file];
    }

    // Side to move (white only)
    if (board.turn == 0) {
        key ^= RandTurn;
    }

    return key;
}

#include <cassert>
#include <cstdint>

// Mock random tables with deterministic values for testing
uint64_t RandPiece[768];
uint64_t RandCastle[4];
uint64_t RandEnPassant[8];
uint64_t RandTurn;

// Include the solution function here (as in )

int main() {
    // Initialize deterministic random values (simple pattern for verification)
    for (int i = 0; i < 768; ++i) RandPiece[i] = (uint64_t)i * 0x9E3779B97F4A7C15ULL;
    for (int i = 0; i < 4; ++i) RandCastle[i] = (uint64_t)(i + 1) * 0xBF58476D1CE4E5B9ULL;
    for (int i = 0; i < 8; ++i) RandEnPassant[i] = (uint64_t)(i + 10) * 0x94D049BB133111EBULL;
    RandTurn = 0x123456789ABCDEF0ULL;

    // Test 1: Empty board, black to move, no flags, no ep
    board_t b1 = {};
    for (int i = 0; i < 64; ++i) { b1.square[i] = 0; b1.pieceList[i] = -1; }
    b1.turn = 1; b1.flags = 0; b1.ep_square = -1;
    assert(zobristKey(b1) == 0); // no contributions

    // Test 2: Single white pawn at a1 (square 0), white to move, no flags, no ep
    board_t b2 = {};
    for (int i = 0; i < 64; ++i) { b2.square[i] = 0; b2.pieceList[i] = -1; }
    b2.square[0] = 1; // white pawn
    b2.pieceList[0] = 0;
    b2.pieceList[1] = -1;
    b2.turn = 0; b2.flags = 0; b2.ep_square = -1;
    uint64_t expected2 = RandPiece[((1-1)^1)*64 + 0] ^ RandTurn;
    assert(zobristKey(b2) == expected2);

    // Test 3: Same as test 2 but black to move -> turn contribution removed
    board_t b3 = b2;
    b3.turn = 1;
    uint64_t expected3 = RandPiece[0*64 + 0]; // no RandTurn
    assert(zobristKey(b3) == expected3);

    // Test 4: Add castle flags (bits 0 and 3 set) to test 2
    board_t b4 = b2;
    b4.flags = (1<<0) | (1<<3);
    uint64_t expected4 = expected2 ^ RandCastle[0] ^ RandCastle[3];
    assert(zobristKey(b4) == expected4);

    // Test 5: Add en-passant square at e3 (square index 20, file 4) to test 4
    board_t b5 = b4;
    b5.ep_square = 20;
    uint64_t expected5 = expected4 ^ RandEnPassant[4];
    assert(zobristKey(b5) == expected5);

    // Test 6: Multiple pieces, verify XOR consistency
    board_t b6 = {};
    for (int i = 0; i < 64; ++i) { b6.square[i] = 0; b6.pieceList[i] = -1; }
    // White knight at b1 (square 1), black rook at a8 (square 56)
    b6.square[1] = 2; b6.square[56] = 10; // black rook = 7+3
    b6.pieceList[0] = 1; b6.pieceList[1] = 56; b6.pieceList[2] = -1;
    b6.turn = 0; b6.flags = (1<<1); b6.ep_square = -1;
    uint64_t expected6 = RandPiece[((2-1)^1)*64 + 1]  // white knight
                       ^ RandPiece[((10-1)^1)*64 + 56] // black rook
                       ^ RandCastle[1] ^ RandTurn;
    assert(zobristKey(b6) == expected6);

    return 0;
}

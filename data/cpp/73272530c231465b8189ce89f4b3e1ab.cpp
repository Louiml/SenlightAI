// Write a C++ function named `countAliceSwaps` that takes a vector of coordinates (each with integer row and column values, where `Coordinate` is a struct with two public integer fields `row` and `col`), and simulates the dimension-swapping behavior inspired by Alice chess pieces. The function must return the total number of dimension swaps that occur when processing the moves in the given order. Each coordinate represents a move to a board position; after each move, the piece always swaps its dimension (from "normal" to "shadow" or vice versa), but a move is only valid if the destination position is not occupied by a piece of the same dimension. To simplify, assume the board is initially empty, and a piece placed on a coordinate will occupy that coordinate with its CURRENT dimension at the moment of placement. If an invalid move occurs (destination already occupied by same dimension), the swap for that move is cancelled (i.e., no swap occurs for that invalid move), and the piece remains in its current dimension. For all valid moves, the swap happens at the end of the move. Return the total count of successful swaps after processing all moves. The function should also handle the edge case of duplicate coordinates in the sequence.
#include <cassert>
#include <vector>

// The Coordinate struct and countAliceSwaps function are assumed to be included above.
int main() {
    // Empty input: no swaps.
    std::vector<Coordinate> empty;
    assert(countAliceSwaps(empty) == 0);

    // Single move: always valid -> 1 swap.
    std::vector<Coordinate> one = {Coordinate(1, 1)};
    assert(countAliceSwaps(one) == 1);

    // Two distinct coordinates: both valid -> 2 swaps.
    std::vector<Coordinate> two = {Coordinate(1, 1), Coordinate(2, 2)};
    assert(countAliceSwaps(two) == 2);

    // Same coordinate twice: first valid, second invalid (now occupied in dimension 1 after swap) -> only 1 swap.
    std::vector<Coordinate> duplicate = {Coordinate(1, 1), Coordinate(1, 1)};
    assert(countAliceSwaps(duplicate) == 1);

    // Alternating valid moves that revisit coordinates in different dimensions.
    // Move1: (0,0) dim0 -> swap to dim1
    // Move2: (1,1) dim1 -> swap to dim0
    // Move3: (0,0) dim0 -> now occupied in dim0? Actually dim0 has (0,0) from move1? No, move1 placed in dim0, then swapped to dim1. Move2 placed in dim1, swapped to dim0. So dim0 has (0,0) from move1. Move3 tries (0,0) in dim0 -> occupied, invalid. So only 2 swaps.
    std::vector<Coordinate> mixed = {Coordinate(0, 0), Coordinate(1, 1), Coordinate(0, 0)};
    assert(countAliceSwaps(mixed) == 2);

    // Test that coordinates in different dimensions do not block each other.
    // Move1: (5,5) dim0 -> swap to dim1
    // Move2: (5,5) but now currentDimension is 1, dim1 empty -> valid -> swap to dim0
    // Total 2 swaps.
    std::vector<Coordinate> crossDimensions = {Coordinate(5, 5), Coordinate(5, 5)};
    assert(countAliceSwaps(crossDimensions) == 2);

    // Longer sequence with a mix.
    // Moves: (0,0) valid swap->1, (1,0) valid swap->0, (0,0) invalid (occupied in dim0), (2,2) valid swap->1, (1,0) invalid? After step2 swap to dim0, then step3 invalid. Step4 valid placed in dim0? Wait currentDimension after step3 is still 0 (no swap). Step4 places (2,2) in dim0, swap to dim1. Step5 tries (1,0) in dim1, but (1,0) was placed in dim0 at step2, so dim1 empty -> valid, swap to dim0. Swaps: steps 1,2,4,5 = 4 swaps.
    std::vector<Coordinate> complex = {Coordinate(0, 0), Coordinate(1, 0), Coordinate(0, 0), Coordinate(2, 2), Coordinate(1, 0)};
    assert(countAliceSwaps(complex) == 4);

    return 0;
}
#include <vector>
#include <unordered_set>
#include <utility>

// Simple coordinate structure with row and col fields.
struct Coordinate {
    int row;
    int col;
    Coordinate(int r = 0, int c = 0) : row(r), col(c) {}
    bool operator==(const Coordinate& other) const {
        return row == other.row && col == other.col;
    }
};

// Hash specialization for Coordinate to use in unordered_set.
namespace std {
    template<>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate& coord) const {
            // Simple hash combining row and col (avoid zero hashes colliding).
            return (static_cast<size_t>(coord.row) * 1000003) ^ static_cast<size_t>(coord.col);
        }
    };
}

// Count successful dimension swaps for a sequence of moves.
// Each move attempts to place the piece at the given coordinate in its current dimension.
// Valid move: destination not occupied by same dimension -> place and swap dimension.
// Invalid move: occupied by same dimension -> no placement and no swap.
int countAliceSwaps(const std::vector<Coordinate>& moves) {
    std::unordered_set<Coordinate> dimension0;  // normal dimension pieces
    std::unordered_set<Coordinate> dimension1;  // shadow dimension pieces
    int currentDimension = 0;
    int swapCount = 0;

    for (const Coordinate& move : moves) {
        bool occupied = false;
        if (currentDimension == 0) {
            occupied = dimension0.find(move) != dimension0.end();
            if (!occupied) {
                dimension0.insert(move);
            }
        } else {
            occupied = dimension1.find(move) != dimension1.end();
            if (!occupied) {
                dimension1.insert(move);
            }
        }

        if (!occupied) {
            // Valid move: swap dimension and increment counter.
            currentDimension = 1 - currentDimension;
            ++swapCount;
        }
        // If occupied, do nothing (no swap).
    }

    return swapCount;
}
// We need to track two occupancy sets: one for the "normal" dimension (call it dimension 0) and one for the "shadow" dimension (dimension 1). We also need a variable `currentDimension` starting at 0. For each coordinate in the input vector:
// 1. Check if that coordinate is already occupied in the current dimension. If yes, the move is invalid, so no swap occurs; the current dimension stays as is. Do not place anything.
// 2. If not occupied, the move is valid. Place the piece in the current dimension (insert coordinate into that dimension's set). Then toggle the dimension: `currentDimension = 1 - currentDimension`, and increment the swap counter.
// After processing all moves, return the swap counter.
// Edge cases: The same coordinate could appear in both dimensions at different times if a swap occurred in between. That's fine because they are separate sets. If the same coordinate appears twice in the same dimension before being placed in the other dimension, the second is invalid. The algorithm runs in O(m) time where m is the number of moves, using O(m) space for the sets (at most m coordinates stored total). Since we only need to check existence and insert, using `unordered_set` or `set` gives O(1) average or O(log m) worst-case per operation. We'll use `std::unordered_set` for average O(1) and include necessary headers.

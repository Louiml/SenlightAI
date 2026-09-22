// Write a C++ function named `roundNumber` that takes three integers: the number of participants `n` (a power of two, 2 ≤ n ≤ 2^20), and two participant indices `a` and `b` (1-indexed, distinct, both ≤ n) in a single-elimination tournament bracket. The function must return an integer representing the round number in which players `a` and `b` will meet if the bracket is arranged as a perfect binary tree, with each round pairing adjacent players from the previous round (i.e., standard tournament seeding where matches in round 1 are (1,2), (3,4), ...). If they meet in the final round, return -1 (to indicate "Final!"), otherwise return the round number (1 for first round, 2 for second, etc.). You may assume the inputs are valid.

A standard single-elimination bracket with `n` players can be viewed as a binary tree. In any round, each player belongs to a block of size `2^(round-1)` players. Two players meet in round `r` if and only if their 0-indexed positions `a-1` and `b-1` first differ in the bit at position `r-1` when read from the least significant bit (i.e., the lowest set bit where their binary representations differ). Specifically, find the highest bit position `i` (0-indexed from LSB) where `(a-1)` and `(b-1)` differ. Then the meeting round is `i+1`. If the size of the block at that round equals `n` (i.e., `2^i == n`), then it's the final round, so return -1. Otherwise return `i+1`. Because `n` is a power of two, `(2 << i) == n` indicates final. Edge cases: if `a` and `b` are identical (not allowed by specs), the loop would never differ; but with valid distinct inputs, at least one differing bit exists. Complexity: O(log n) time (at most 20 iterations) and O(1) space.

#include <cstdint>

// Returns the round number where players a and b meet in a single-elimination
// tournament with n participants (n is a power of two, n >= 2).
// a and b are 1-indexed participant numbers (distinct, 1 <= a,b <= n).
// Returns -1 if they meet in the final round.
int roundNumber(int n, int a, int b) {
    // Convert to 0-indexed
    int64_t x = a - 1;
    int64_t y = b - 1;

    // Find the highest bit where x and y differ
    for (int bit = 19; bit >= 0; --bit) {
        if (((x >> bit) & 1) != ((y >> bit) & 1)) {
            // If the differing bit's block size equals n, it's the final
            if ((2L << bit) == static_cast<int64_t>(n)) {
                return -1; // Final round
            }
            return bit + 1; // Round number (1-indexed)
        }
    }
    // Should never happen with distinct inputs
    return 0;
}

#include <cassert>

// Declaration of the function under test (included in the solution)
int roundNumber(int n, int a, int b);

int main() {
    // n=2, only round is final
    assert(roundNumber(2, 1, 2) == -1);

    // n=4
    assert(roundNumber(4, 1, 2) == 1);   // first round
    assert(roundNumber(4, 3, 4) == 1);   // first round
    assert(roundNumber(4, 1, 3) == 2);   // semi-final (final for 4? actually round 2 is final)
    // For n=4, round 2 is the final, so:
    assert(roundNumber(4, 1, 3) == -1);  // they meet in final
    assert(roundNumber(4, 1, 4) == -1);  // final
    assert(roundNumber(4, 2, 3) == -1);  // final

    // n=8
    assert(roundNumber(8, 1, 2) == 1);
    assert(roundNumber(8, 7, 8) == 1);
    assert(roundNumber(8, 1, 3) == 2);   // meet in quarter-final (round 2)
    assert(roundNumber(8, 1, 5) == 3);   // meet in semi-final (round 3)
    assert(roundNumber(8, 1, 8) == -1);  // final (round 3 is final for n=8? actually round 3 is semi-final? wait)
    // For n=8, rounds: R1 (8 players), R2 (4), R3 (2)=final, so round 3 is final
    assert(roundNumber(8, 1, 8) == -1);  // final
    assert(roundNumber(8, 1, 4) == 2);   // meet in round 2 (quarter)
    assert(roundNumber(8, 2, 7) == -1);  // final

    // n=16
    assert(roundNumber(16, 1, 2) == 1);
    assert(roundNumber(16, 1, 3) == 2);
    assert(roundNumber(16, 1, 5) == 3);
    assert(roundNumber(16, 1, 9) == 4);  // round 4 is final for n=16? Actually rounds: 1,2,3,4 (final) -> -1
    assert(roundNumber(16, 1, 9) == -1); // final
    assert(roundNumber(16, 1, 8) == 3);  // meet in round 3 (semi)
    assert(roundNumber(16, 3, 4) == 1);  // adjacent

    // n=1024
    assert(roundNumber(1024, 1, 512) == 9); // 2^9=512, so round 9? Actually 2^(9-1)=256? let's compute: 1 and 512 differ at bit 9? 1-1=0, 512-1=511 (binary 111111111). They differ at bit 9? Highest diff bit is 9? 2^9=512, block size 512, final? n=1024, so not final -> round 10? Actually bit index 9 -> round 10? Wait: bit 0 is LSB. 0 and 511: binary 0 = 0...0, 511 = 0...111111111 (9 ones). Highest set bit in 511 is bit 8 (since 2^8=256, 2^9=512). So diff at bit 8, round=9, block size 2^8=256, not final. So round 9.
    assert(roundNumber(1024, 1, 512) == 9);

    return 0;
}

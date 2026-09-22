Write a C++ function `int towerBreakers(int n, int m)` that determines the winner of a game where there are `n` towers, each initially of height `m`. The game is played by two players who alternate turns. On each turn, a player selects a tower of height greater than 1 and reduces its height to a positive divisor of its current height (strictly smaller than the current height). The player who cannot make a move (i.e., all towers have height 1) loses. The function should return `1` if the first player wins, and `2` if the second player wins, assuming both play optimally. The inputs `n` and `m` are positive integers. The function must handle edge cases where `n = 1` or `m = 1` correctly. Provide a clean, standalone implementation without a `main` function.
The game is a variant of Nim with a twist on move options. Observe that each tower's height can only be reduced to a divisor, so the fundamental state of a tower is essentially its height parity or whether it is already at 1. For a single tower (`n = 1`), the first player can always win if `m > 1`, because they can reduce it to 1 in one move (since 1 divides any number), leaving the second player with no moves. If `m = 1`, the first player has no move and loses. For multiple towers, we need to analyze the parity of the number of towers with height > 1. If `n` is even, the second player can mirror the first player's moves on a paired tower, ensuring the first player eventually faces a situation where all towers are height 1 on the first player's turn, so the second player wins. If `n` is odd, the first player can reduce one tower to 1 (if `m > 1`), leaving an even number of towers with height > 1; then the first player adopts the mirror strategy and wins. If `m = 1` and `n` is odd, no moves are possible at all, so the first player loses (returns `2`). Edge cases: if `n == 1`, result is `1` if `m > 1`, else `2`. If `m == 1`, no moves possible regardless of `n`, so first player loses if `n >= 1` (but `n >= 1` always), so return `2`. Time complexity is O(1), space complexity O(1).
#include <iostream> // For completeness, not strictly needed here but good practice

// Determine winner of tower breakers game.
// Returns 1 if first player wins, 2 if second player wins.
// n: number of towers, m: initial height of each tower (positive).
int towerBreakers(int n, int m) {
    // If there is only one tower, first player wins unless it's already height 1.
    if (n == 1) {
        return (m > 1) ? 1 : 2;
    }
    // If all towers are height 1, no moves possible, second player wins.
    if (m == 1) {
        return 2;
    }
    // For multiple towers with height > 1:
    // If n is odd, first player can force a win.
    // If n is even, second player can mirror and win.
    return (n % 2 == 1) ? 1 : 2;
}
#include <cassert>

int main() {
    // Basic cases: single tower
    assert(towerBreakers(1, 1) == 2); // no moves, second wins
    assert(towerBreakers(1, 2) == 1); // reduce to 1, first wins
    assert(towerBreakers(1, 100) == 1); // any height > 1, first wins

    // Multiple towers with height 1: no moves, second wins
    assert(towerBreakers(2, 1) == 2);
    assert(towerBreakers(5, 1) == 2);

    // Even number of towers with height > 1: second wins
    assert(towerBreakers(2, 2) == 2);
    assert(towerBreakers(4, 3) == 2);
    assert(towerBreakers(10, 1) == 2); // but m=1 handled above

    // Odd number of towers with height > 1: first wins
    assert(towerBreakers(3, 2) == 1);
    assert(towerBreakers(5, 7) == 1);
    assert(towerBreakers(1, 1) == 2); // already checked

    // Larger values
    assert(towerBreakers(1000000000, 1) == 2);
    assert(towerBreakers(999999999, 999999999) == 1);
    assert(towerBreakers(1000000000, 999999999) == 2);

    // All passes
    return 0;
}

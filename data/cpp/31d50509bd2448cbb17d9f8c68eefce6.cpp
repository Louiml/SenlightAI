/*
Write a C++ function that determines whether two piles of coins, given their sizes `a` and `b`, can both be reduced to zero by repeatedly applying one of two moves: remove 2 coins from one pile and 1 coin from the other (in either direction). The function should take two non-negative integers `a` and `b` and return `true` if possible, `false` otherwise. The input values may be large (up to 10^9), and the function must handle edge cases where one pile is zero or both are zero.
*/
#include <cstdint>

// Returns true if two coin piles of sizes a and b can both be emptied
// by repeatedly removing (2,1) or (1,2) coins from the two piles.
bool canEmptyPiles(std::int64_t a, std::int64_t b) {
    return (a + b) % 3 == 0 && 2 * a >= b && 2 * b >= a;
}
#include <cassert>

int main() {
    // Basic cases
    assert(canEmptyPiles(0, 0) == true);
    assert(canEmptyPiles(1, 1) == false);   // sum 2 not divisible by 3
    assert(canEmptyPiles(2, 1) == true);    // (2,1) in one move
    assert(canEmptyPiles(1, 2) == true);
    assert(canEmptyPiles(3, 0) == false);   // 2*0 >= 3 fails
    assert(canEmptyPiles(0, 3) == false);
    assert(canEmptyPiles(4, 2) == true);    // two moves: (2,1)+(2,1)
    assert(canEmptyPiles(5, 1) == false);   // sum 6 divisible, but 2*1 >= 5 fails
    assert(canEmptyPiles(1000000000, 1000000000) == false); // sum not divisible by 3
    assert(canEmptyPiles(999999999, 999999999) == true);     // sum 1999999998 % 3 = 0, inequalities hold
    return 0;
}
// The solution relies on two necessary and sufficient conditions. First, the total number of coins removed per move is always 3, so the sum `a + b` must be divisible by 3. Second, after some moves, the difference between the piles must be balanced: if we apply `x` moves removing 2 from pile A and 1 from pile B, and `y` moves removing 1 from A and 2 from B, then we have `a = 2x + y` and `b = x + 2y`. Solving these gives `x = (2a - b)/3` and `y = (2b - a)/3`, which must be non-negative integers. This leads to the conditions `2*a >= b` and `2*b >= a`. Edge cases: both zero works (sum divisible by 3, and inequalities hold), and a single pile zero only works if the other is also zero (e.g., a=1, b=0 fails because 2*0 >= 1 is false). Time complexity is O(1) per query, space O(1).
